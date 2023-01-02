#include <stdio.h>
#include <string> 
#include <cmath> 
#include <vector> 

#include "phys_lib.cpp"
#include "graphics.cpp"

Ball ball[100];
int color[100][3];
int balls = 5;
int frames;
int spring_segments = 50;
std::vector<std::pair<int,int>> springs; 
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

void drawFrame() {
	graphics::clearScreen();
	
	// draw springs 
	for(std::pair<int,int> ball_pair : springs) {
		Vec2 first_point = ball[ball_pair.first].pos;
		Vec2 last_point = ball[ball_pair.second].pos;
		Vec2 dir_vec = (last_point + (first_point * (-1)));
		Vec2 prep_vec = Vec2(-dir_vec.y, dir_vec.x);
		Vec2 prev_point = first_point;	
		//draw each segment
		for(int i = 0; i < spring_segments; i ++) {	
			Vec2 next_point = first_point + (dir_vec * ( (float) (i+1) / spring_segments)); 
			if(i != spring_segments - 1)
				next_point = next_point + (prep_vec.norm() * (i % 2 ? 1 : -1) * 10);
			
			graphics::drawLine(prev_point.x, prev_point.y, next_point.x, next_point.y, 0xFF, 0xFF, 0xFF, 0xFF);
			prev_point = next_point;
		}
	}
	
	// draw ball positions (and trails) 
	for(int i = 0; i < balls; i++) {
		int x = ball[i].pos.x;
		int y = ball[i].pos.y;
		int ball_size = 20; 
		graphics::drawCircle(x, y, ball_size,color[i][0], color[i][1], color[i][2], 0xFF);
	}

	// update screen
	graphics::updateScreen();

	/* 
	// clear positions (don't clear trail)
	// TODO: render trails on background renderer 
	for(int i = 0; i < balls; i++) {
		int x = ball[i].pos.x;
		int y = ball[i].pos.y;
		int ball_size = ball[i].mass * 10; 
		graphics::clearCircle(x, y, ball_size);
	}
	*/
}

void processEvents() {

	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT) {
			printf("Quit event\n");
			running = false;
		} else if(e.type == SDL_KEYDOWN) {
			printf("Pressed %s\n", SDL_GetKeyName(e.key.keysym.sym));
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					running = false;
				break;
				default: 
				break;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	// TODO: remove logic from rendering 
	if(argc == 2) {
		balls = std::stoi(argv[1]);
	}

	std::string flags = "0";
	if(argc == 3) {
		balls = std::stoi(argv[1]);
		flags = argv[2];
	}
	float global_friction = 0.05;
	if(argc == 4) {
		balls = std::stoi(argv[1]); 
		flags = argv[2];
		global_friction = std::stof(argv[3]);
	}

	
	if(!graphics::initSDL()) {
		printf("Failed to initialize!\n");
	} else {

		graphics::clearScreen();
		
		// Initialize stuff
		for(int i = 0; i < balls; i++) {
			ball[i] = Ball(Vec2(100 + 100*i ,500), 2, Vec2(0,0.05 * (i % 2 ? 1 : -1)));		
			ball[i].friction_factor = global_friction;
			color[i][0] = (0xFF / (balls-1)) * (i);
			color[i][1] = 0x22;
			color[i][2] = (0xFF / (balls-1)) * (balls - i - 1);
		}
		
		ball[0] = Ball(Vec2(450, 450), 100, Vec2()); 

		const float spring_constant = 0.01;
		const float spring_lenght = 150;
		if(flags == "s" || flags == "0")
		for(int i = 1; i <= balls - 1; i++) {
			for(int j = 0; j < i; j++) {
				//if(i == j + 1 || (i == balls - 1 && j == 0 )) {
				if(1) {
					ball[i].add_spring(&ball[j], spring_constant, spring_lenght);
					ball[j].add_spring(&ball[i], spring_constant, spring_lenght);
					springs.push_back({i, j});
				}
			}
		}
		
		const float gravity_constant = 50;
		if(flags == "g" || flags == "0")	
		for(int i = 1; i <= balls - 1; i++) {
			for(int j = 0; j < i; j++) {
				ball[i].add_gravity(&ball[j], gravity_constant);
				ball[j].add_gravity(&ball[i], gravity_constant);
			}
		}
		auto last_frame_time = std::chrono::high_resolution_clock::now();
		while(running) {
			// Check window events
			processEvents();

			// Run one simulation frame 
			auto current_time = std::chrono::high_resolution_clock::now();
			auto delta_time_in_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_frame_time).count();
			
			run(delta_time_in_nano);
			
			last_frame_time = current_time;
		
			drawFrame();
		}
	}

	graphics::closeSDL();

}
