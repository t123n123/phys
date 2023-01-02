#ifdef WIN
#include <SDL.h>
#else
#include <SDL2/SDL.h> 
#endif

namespace graphics {  

	const int BG_A = 0xFF, BG_R = 0x28, BG_G = 0x2c, BG_B = 0x34;
	
	const int INIT_SCREEN_WIDTH = 800;
	const int INIT_SCREEN_HEIGHT = 800;

	int screenWidth = INIT_SCREEN_WIDTH;
	int screenHeight = INIT_SCREEN_HEIGHT;

	SDL_Window * gWindow = NULL;
	SDL_Renderer * gRenderer = NULL;

	bool initSDL() {
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

	bool closeSDL() {
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;

		SDL_Quit();
	}

	void drawRect(int pos_x, int pos_y, int width, int height, int colorR, int colorG, int colorB, int colorA) {
		SDL_Rect fillRect = {pos_x, pos_y, width, height};
		
		SDL_SetRenderDrawColor(gRenderer, colorR, colorG, colorB, colorA);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}

	void drawCircle(int pos_x, int pos_y, int radius, int colorR, int colorG, int colorB, int colorA) {
		
		SDL_SetRenderDrawColor(gRenderer, colorR, colorG, colorB, colorA);
		for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
		    int dx = radius - w; // horizontal offset
		    int dy = radius - h; // vertical offset
		    if ((dx*dx + dy*dy) <= (radius * radius)) {
				SDL_RenderDrawPoint(gRenderer, pos_x + dx, pos_y + dy);
		    }
		}
	    }
	}
	
	void drawLine(int x1, int y1, int x2, int y2, int colorR, int colorG, int colorB, int colorA) {
		SDL_SetRenderDrawColor(gRenderer, colorR, colorG, colorB, colorA);
		SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
	}

	void clearCircle(int pos_x, int pos_y, int radius) {
		drawCircle(pos_x, pos_y, radius, BG_R, BG_G, BG_B, BG_A);
	}
	
	void updateScreen() { 
		SDL_RenderPresent(gRenderer);
	}

	void clearScreen() {
		SDL_SetRenderDrawColor(gRenderer, BG_R, BG_G, BG_B, BG_A);
		SDL_RenderClear(gRenderer);
	}

}
