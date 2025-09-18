/*
 * Implement functions of vector.h
 *
 * */

#include "vector.h"
#include <stdio.h>
#include <math.h> // include sqrt()

// show funcs

// Implenent functions.
// w = v + u = (v1+u1, v2+u2, v3+u3)
Vec3 add(Vec3 v1, Vec3 v2) {

	/**
	Vec3 temp;

	temp.x = (v1.x) + (v2.x);
	temp.y = (v1.y) + (v2.y);
	temp.z = (v1.z) + (v2.z);
	*/
	return (Vec3){ (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z)};
}
// w = v - u = (v1-u2, v2-u2, v3-u3)
Vec3 subtract(Vec3 v1, Vec3 v2) {
	/**
	Vec3 temp;

	temp.x = (v1.x) - (v2.x);
	temp.y = (v1.y) - (v2.y);
	temp.z = (v1.z) - (v2.z);
	*/
	return (Vec3){ (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z)};
}
// sV = (sV1, sV2, sV3)
Vec3 scalarMultiply(float s, Vec3 v) {
	/**
	Vec3 temp;

	temp.x = s*v.x;
	temp.y = s*v.y;
	temp.z = s*v.z;
	*/
	return (Vec3){s*v.x, s*v.y, s*v.z};
}
// V/s = (v1/s, v2/s, v3/s);
Vec3 scalarDivide(Vec3 v, float d) {
	/**
	Vec3 temp;

	temp.x = v.x/d;
	temp.y = v.y/d;
	temp.z = v.z/d;
	*/
	return (Vec3){v.x/d, v.y/d, v.z/d};
}
Vec3 normalize(Vec3 v) {
	
	//float vlength = length(v);
	
	return scalarDivide(v, length(v));
}
// u dot s = v1*u1 + v2*u2 + v3*u3
float dot(Vec3 v1, Vec3 v2) {
	
	//float x, y, z; 
	
	//x = v1.x * v2.x;
    //y = v1.y * v2.y;
	//z = v1.z * v2.z;
    
	return ((v1.x*v2.x) +(v1.y*v2.y) + (v1.z*v2.z));
}

// length = sqrt(v1.x^2 + v1.y^2 + v1.z^2)
float length(Vec3 v) {

	float vlength = dot(v, v);
	return sqrt(vlength);
}

// length squared
float length2(Vec3 v){ 
	
	return dot(v,v);

}


// distance
float distance(Vec3 v1, Vec3 v2){ 

	Vec3 temp = subtract(v1,v2);
	return sqrt(length2(temp));
}

// distnace squared
float distance2(Vec3 v1, Vec3 v2) {
	
	Vec3 temp = subtract(v1,v2);
	return length2(temp);

}

