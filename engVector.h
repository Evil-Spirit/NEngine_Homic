#ifndef NENGINE_VECTORS
#define NENGINE_VECTORS
#include <math.h>
#include "engConsts.h"
#include "engVars.h"
#include "GL/gl.h"
class tVector {
public:
	double x,y,z;
};
typedef float tMatrix[4][4];
/////////////////////////////////////////////////////////////////////////////////////////////////
tVector vecVec(double x, double y, double z) {
	tVector tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;

	return tmp;
}

tVector operator+ (tVector a,tVector b) {
	a.x+=b.x;
	a.y+=b.y;
	a.z+=b.z;
	return a;
}
tVector operator- (tVector a,tVector b) {
	a.x-=b.x;
	a.y-=b.y;
	a.z-=b.z;
	return a;
}
double operator& (tVector a,tVector b) {
	return a.x*b.x+a.y*b.y+a.z*b.z;
}
tVector operator* (tVector a,tVector b) {
	tVector c;
	c.x = a.y*b.z - b.y*a.z;
	c.y = a.z*b.x - b.z*a.x;
	c.z = a.x*b.y - b.x*a.y;
	return c;
}
tVector operator| (tVector a,tMatrix b) {
	tVector c;
	c.x =  b[0][0]*a.x+b[1][0]*a.y+b[2][0]*a.z+b[3][0];
	c.y =  b[0][1]*a.x+b[1][1]*a.y+b[2][1]*a.z+b[3][1]; 
	c.z =  b[0][2]*a.x+b[1][2]*a.y+b[2][2]*a.z+b[3][2];
	return c;
}
tVector operator* (tVector a,double b) {
	a.x*=b;
	a.y*=b;
	a.z*=b;
	return a;
}
tVector operator* (double b,tVector a) {
	a.x*=b;
	a.y*=b;
	a.z*=b;
	return a;
}
double operator! (tVector a) {
	return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}
double operator^ (tVector a, tVector b) {
	return (a&b)/!a*!b;
}
bool operator>	(tVector a, tVector b) { return !a>	!b; }
bool operator<	(tVector a, tVector b) { return !a<	!b; }
bool operator>=	(tVector a, tVector b) { return !a>=!b; }
bool operator<=	(tVector a, tVector b) { return !a<=!b; }
bool operator!=	(tVector a, tVector b) { return !a!=!b; }
bool operator==	(tVector a, tVector b) { return !a==!b; }

tVector operator+=	(tVector a, tVector b) { return a+b; }
tVector operator-=	(tVector a, tVector b) { return a-b; }
tVector operator*=	(tVector a, tVector b) { return a*b; }
tVector operator*=	(tVector a, double b) { return a*b; }
tVector operator|=	(tVector a, tMatrix b) { return a|b; }

void vecNormalize(tVector *v) {
	double len = !*v;
	v->x/=len;
	v->y/=len;
	v->z/=len;
}

tVector vecProject(tVector v, tMatrix m) {
	tVector tmp = v | m;
	if (fabs(tmp.z)<engMinZ) tmp.z = engMinZ;
	tmp.x = tmp.x / tmp.z * engFov;
	tmp.y = tmp.y / tmp.z * engFov;
	return tmp;
}

tVector vecRotOnly(tVector a, tMatrix b) {
	tVector c;
	c.x =  b[0][0]*a.x+b[1][0]*a.y+b[2][0]*a.z;
	c.y =  b[0][1]*a.x+b[1][1]*a.y+b[2][1]*a.z; 
	c.z =  b[0][2]*a.x+b[1][2]*a.y+b[2][2]*a.z;
	return c;
}
#endif
