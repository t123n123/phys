#include <SDL2/SDL.h> 
#include <stdio.h>
#include <string> 
#include <cmath> 
#include "phys_lib.h"

const int BG_A = 0xFF, BG_R = 0x11, BG_G = 0x11, BG_B = 0x11;
const int INIT_SCREEN_WIDTH = 1800;
const int INIT_SCREEN_HEIGHT = 1200;

int screenWidth = INIT_SCREEN_WIDTH;
int screenHeight = INIT_SCREEN_HEIGHT;

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;

bool init() {
	if(SDL_Init(SDL_INIT_VIDEO ) < 0) {
		printf("SLD could not initialize. SDL Error: %s\n", SDL_GetError());
		return false;
	} else {
		gWindow = SDL_CreateWindow("SDL Phys", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(gWindow == NULL) {
			printf("Could not create window. SDL Error: %s\n", SDL_GetError());
			return false;
		}else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL) {
				printf("Could not create renderer. SDL Error: %s\n", SDL_GetError());
				return false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				return true;
			}
		}
	}
}

bool close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

bool drawRect(int pos_x, int pos_y, int width, int height, int colorR, int colorG, int colorB, int colorA) {
	SDL_Rect fillRect = {pos_x, pos_y, width, height};
	
	SDL_SetRenderDrawColor(gRenderer, colorR, colorG, colorB, colorA);
	SDL_RenderFillRect(gRenderer, &fillRect);

}

void drawCircle(int pos_x, int pos_y, int radius, int colorR, int colorG, int colorB, int colorA) {
	SDL_SetRenderDrawColor(gRenderer, colorR, colorG, colorB, colorA);
	for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(gRenderer, pos_x + dx, pos_y + dy);
            }
        }
    }
}



Ball ball[100];
int color[100][3];
int balls = 7;
Ball targetBall;
int frames;

bool running = true;

void run(int delta_t) {
	frames--;
	// check in case nr of frames are limited 
	if(!frames) 
		running = false;
	
	for (int i = 0; i < balls; i ++) {
		ball[i].process_tick(delta_t);
	}

}


int main(int argc, char* argv[]) {		
	if(argc == 2) {
		balls = std::stoi(argv[1]);
	}

	if(argc == 3) {
		balls = std::stoi(argv[1]);
		frames = std::stoi(argv[2]);
	}
	
	if(!init()) {
		printf("Failed to initialize!\n");
	} else {

		SDL_SetRenderDrawColor(gRenderer, BG_R, BG_G, BG_B, BG_A);
		SDL_RenderClear(gRenderer);
		
		SDL_Event e;
		for(int i = 0; i < balls; i++) {
			ball[i] = Ball("Blue Ball", Vec2(100 + 10*i ,600), 2, Vec2(0,0.7 * (i == 0 ? 1 : 0)));		
			color[i][0] = (0xFF / balls) * i;
			color[i][1] = 0x22;
			color[i][2] = (0xFF / balls) * (balls - i);
		}

		targetBall = Ball("Target Ball", Vec2(1200, 800), 5, Vec2(0,0));

		ball[0].add_spring(&targetBall, 0.001, 100);

		for(int i = 1; i <= balls - 1; i++) {
			for(int j = 0; j < i; j++) {
				if(i == j + 1) {
					ball[i].add_spring(&ball[j], 0.001, 10);
					ball[j].add_spring(&ball[i], 0.001, 10);
				}
			}
		}

		auto last_frame_time = std::chrono::high_resolution_clock::now();
		while(running) {
			// Check window events
			while(SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					running = false;
				}
			}

			// Run simulation frame 
			auto current_time = std::chrono::high_resolution_clock::now();
			run( std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_frame_time).count() );
			last_frame_time = current_time;
		
			
			
			// draw ball positions 
			for(int i = 0; i < balls; i++) {
				int x = ball[i].pos.x;
				int y = ball[i].pos.y;
				int ball_size = ball[i].mass * 10; 
				drawCircle(x, y, ball_size,color[i][0], color[i][1], color[i][2], 0xFF);
			}

			// update screen
			SDL_RenderPresent(gRenderer);
			// clear positions and draw trail
			// TODO: fix trails being overwriten
			for(int i = 0; i < balls; i++) {
				int x = ball[i].pos.x;
				int y = ball[i].pos.y;
				int ball_size = ball[i].mass * 10; 
				drawCircle(x, y, ball_size, BG_R, BG_G, BG_B, BG_A);
				//drawCircle(x, y, 5, 0x00, 0xFF, 0x00, 0xFF);
			}

		}
	}
	char ch = std::getchar();

	close();

}
