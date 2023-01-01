#include <stdio.h> 
#include <utility>
#include <chrono> 
#include <string> 

#include "math_lib.cpp"

struct Ball {
	Vec2 pos;	
	float mass;

	Vec2 velocity;

	Vec2 force = Vec2(); 
	float friction_factor = 0.001;
	float g_const = 10;
	// acceleration = F / m 
	// velocity += acceleration 
	const static int MAX_SPRINGS = 40;
	const static int MAX_PAIRS = 40;
	int springs = 0;
	int gpairs = 0;
	float springK[MAX_SPRINGS];
	float springL[MAX_SPRINGS];
	Ball * spring_ball[MAX_SPRINGS];
	Ball * grav_ball[MAX_PAIRS];
	float grav_const[MAX_PAIRS];

	Ball(Vec2 ppos, float mmass, Vec2 vvelocity) :pos(ppos), mass(mmass), velocity(vvelocity) {}

	Ball() : pos(Vec2(0,0)), mass(1), velocity(Vec2(0,0)) {}

	Vec2 compute_force() {
		Vec2 total_force = Vec2(0,0);

		// add all forces from springs
		for(int i = 0; i < springs; i++) {
			total_force = total_force + compute_spring_force(i);
		}

		// add all forces from grav pairs 	
		for(int i = 0; i < gpairs; i++) {
			total_force = total_force + compute_gravity_force(i);
		}
		// add friction force 
		// friction = friction_factor * normal
		Vec2 friction_dir = velocity.norm();
		Vec2 friction_force = (friction_dir * (-friction_factor * mass * g_const)); 
		
		total_force = total_force + friction_force; 		
		return total_force;
	}

	void process_tick(int delta_t) {
		force = compute_force();
		
		float milis = float(delta_t) / 1000000;

		Vec2 acc = force * ( 1 / mass);

		velocity = velocity + (acc * (milis / 1000))  ;
		
		pos = pos + (velocity * milis);
	}

	void print_ball() {
		printf( "Ball : pos->(%f,%f), mass->%f, velocity->(%f, %f)\n", pos.x, pos.y, mass, velocity.x, velocity.y);
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

	int add_gravity(Ball * otherBall, float gravityConst) {
		if(gpairs < MAX_PAIRS) {
			grav_ball[gpairs] = otherBall;
			grav_const[gpairs] = gravityConst;
			gpairs++;
			return 0;
		} else {
			printf("Not enough space in struct to add another gravity pair\n");
			return 1;
		}
	}

	Vec2 compute_gravity_force(int gravity_ind) {
		Vec2 diff_vec = grav_ball[gravity_ind]->pos + (pos * -1);
		float dist = diff_vec.dist();

		float force_dist = (grav_const[gravity_ind] * mass * grav_ball[gravity_ind]->mass) / (dist * dist);

		Vec2 force_vec = diff_vec.norm() * force_dist;
		return force_vec;
	}
};
