#include <stdio.h> 
#include <utility>
#include <chrono> 
#include <string> 

bool running = true;
int frames = 10;


struct Vec2 {

	float x;
	float y;

	Vec2(float xx, float yy) : x(xx), y(yy) {}

	Vec2 operator+(const Vec2& a) const {
		return Vec2(a.x + x, a.y + y);
	}
	
	Vec2 operator+(const float& a) const {
		return Vec2(x + a, y + a);
	}

	Vec2 operator*(const float& a) const {
		return Vec2(x * a, y * a);
	}
}; 

struct Ball {
	char * name;
	Vec2 pos;	
	float mass;

	Vec2 velocity;

	Vec2 force = Vec2(0,0); 
	float friction_factor = 0.1;
	// acceleration = F / m 
	// velocity += acceleration 

	Ball(char * nname, Vec2 ppos, float mmass, Vec2 vvelocity) : name(nname), pos(ppos), mass(mmass), velocity(vvelocity) {}

	Vec2 compute_force() {
		return velocity * (-friction_factor);
	}

	void process_tick(int delta_t) {
		printf( "Process tick: ball_name->%s, d_time->%d \n", name, delta_t);

		force = compute_force();
		
		float milis = float(delta_t) / 1000000;

		Vec2 acc = force * ( 1 / mass);

		velocity = velocity + acc;
		
		pos = pos + (velocity * milis);
	}

	void print_ball() {
		printf( "Ball %s: pos->(%f,%f), mass->%f, velocity->(%f, %f)\n", name, pos.x, pos.y, mass, velocity.x, velocity.y);
	}

};


Ball ball = Ball("Blue Ball", Vec2(1,2), 3, Vec2(1,1));


void init() {
	printf("Init\n");

}

void run(int delta_t) {
	printf("Run Frame\n");
	frames--;
	if(!frames) 
		running = false;
	
	ball.process_tick(delta_t);

	ball.print_ball();

}

int main(int argc, char* argv[]) {		

	if(argc == 2) {
		frames = std::stoi(argv[1]);
	}

	init();
	auto last_frame_time = std::chrono::high_resolution_clock::now();
	while(running) {
		auto current_time = std::chrono::high_resolution_clock::now();
		run( std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_frame_time).count() );
		last_frame_time = current_time;
	}

}
