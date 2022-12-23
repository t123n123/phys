#include <stdio.h> 
#include <utility>
#include <chrono> 
#include <string> 
#include <math.h>

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

	Vec2 norm(float eps = 0.000001) const {
		if(x*x + y*y < eps) {
			printf("Vector aprox to 0\n");
			int randV = std::rand();
			return Vec2(std::sin(randV),std::cos(randV));
		} else {
			float dist = Vec2(x,y).dist();
			Vec2 norm = Vec2(x,y) * (1/dist);
			return norm;
		}
	}

	float dist() const {
		return std::sqrt(x*x + y*y);
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
	const static int MAX_SPRINGS = 40;
	int springs = 0;
	float springK[MAX_SPRINGS];
	float springL[MAX_SPRINGS];
	Ball * spring_ball[MAX_SPRINGS];


	Ball(char * nname, Vec2 ppos, float mmass, Vec2 vvelocity) : name(nname), pos(ppos), mass(mmass), velocity(vvelocity) {}

	Ball() : name("Blank"), pos(Vec2(0,0)), mass(1), velocity(Vec2(0,0)) {}

	Vec2 compute_force() {
		Vec2 total_force = Vec2(0,0);

		// add all forces from springs
		for(int i = 0; i < springs; i++) {
			total_force = total_force + compute_spring_force(i);
		}
		
		// add friction force 
		total_force = total_force + (velocity * (-friction_factor)); 

		return total_force;
	}

	void process_tick(int delta_t) {
		force = compute_force();
		
		float milis = float(delta_t) / 1000000;

		Vec2 acc = force * ( 1 / mass);

		velocity = velocity + acc;
		
		pos = pos + (velocity * milis);
	}

	void print_ball() {
		printf( "Ball %s: pos->(%f,%f), mass->%f, velocity->(%f, %f)\n", name, pos.x, pos.y, mass, velocity.x, velocity.y);
	}

	int add_spring(Ball * otherBall, float springKonst, float springLength) {
		if(springs < MAX_SPRINGS) {
			springK[springs] = springKonst;
			springL[springs] = springLength;
			spring_ball[springs] = otherBall;
			springs++;
			return 0;
		} else {
			printf("Not enough space in struct to add another spring\n");
			return 1;
		}
	}

	Vec2 compute_spring_force(int spring_ind) {
		Vec2 diff_vec = spring_ball[spring_ind]->pos + (pos * -1);
		Vec2 target_vec = diff_vec.norm() * springL[spring_ind];
		Vec2 force = (diff_vec + (target_vec * -1)) * springK[spring_ind];
		return force;
	}

};
