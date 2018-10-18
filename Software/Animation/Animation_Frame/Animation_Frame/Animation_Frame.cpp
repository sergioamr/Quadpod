#include <stdio.h>
#include <stdint.h>

// Solve the Catmull-Rom parametric equation for a given time(t) and vector quadruple (p1,p2,p3,p4)
float interpolate_overhauser_float(float p1, float p2, float p3, float p4, float t)
{
	float t3, t2, c1, c2, c3, c4;
	t2 = t * t;
	t3 = t2 * t;
	c1 = -t3 + 2 * t2 - t;
	c2 = 3 * t3 - 5 * t2 + 2;
	c3 = -3 * t3 + 4 * t2 + t;
	c4 = t3 - t2;
	c1 /= 2;
	c2 /= 2;
	c3 /= 2;
	c4 /= 2;
	return(c1 * p1 + c2 * p2 + c3 * p3 + c4 * p4);
}

#define NUMBER_LEGS  4
struct joint
{

};

struct robot_leg
{
	uint8_t id;
	joint joints[3];
};

struct robot_frame
{
	robot_leg legs[4];
};

struct animation
{

};

void set_new_position(float *pos, float time)
{

}


int main()
{
    return 0;
}

