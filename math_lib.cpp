#include <math.h>
#include <random> 
#define EPS 1e-6

struct Vec2 {
	float x;
	float y;

	Vec2() : x(0.0), y(0.0) {}
	Vec2(float xx, float yy) : x(xx), y(yy) {}
	
	Vec2 operator+(const Vec2& vec) const {
		return Vec2(vec.x + x, vec.y + y);
	}
	
	Vec2 operator+(const float& a) const {
		return Vec2(x + a, y + a);
	}

	Vec2 operator*(const float& a) const {
		return Vec2(x * a, y * a);
	}

	Vec2 norm(float eps = EPS) const {
		if(x*x + y*y < eps) {
			// if vector is to close to 0, return a random normal vector
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



