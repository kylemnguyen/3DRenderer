#include "spheres.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t);


int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
	
	if(sphere == NULL || t == NULL) { // Sphere is a null pointer or t is a null obj.
		return 0;
	}

	Vec3 ray = rayPos;
	Vec3 dir = rayDir;
	Vec3 spherePos = sphere->pos;
	float sphereR = sphere->r;
	
	// Get a, b, c for the quadratic formula
	Vec3 vector = subtract(ray, spherePos);
	float a,b,c, discriminant, t1, t2;
	t1 = 0;
	t2 = 0;
	

	a = dot(dir, dir);
	b = 2.0f * dot(dir, vector);
	c = dot(vector,vector) - (sphereR * sphereR);

	// Check if intersects >>>>
	discriminant = b*b - 4*a*c;       
	if(discriminant < 0) {  // no intersection (2 complex roots)
		return 0;
	} else if  (discriminant >= 0) {
		float disc = sqrt(discriminant);
		t1 = (-b + disc) / (2.0f*a);
		t2 = (-b - disc) / (2.0f*a);
	}	

	if (t1 > 0 && t2 > 0) {
		*t = (t1 < t2) ? t1 : t2;
	} else if (t1 > 0) {
		*t = t1;
	} else if (t2 > 0) {
		*t = t2;
	} else {
		// Both t1 and t2 are negative, intersection is behind the ray
		return 0;
	}
	return 1;
}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
	// Create a sphere pointer
	Sphere *tempSphere;
	//Allocate space
	tempSphere = malloc(sizeof(Sphere));
	if(tempSphere  == NULL) {
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1);
	}
	
	//initalize values
	tempSphere->r = radius;
	tempSphere->pos = position;
	tempSphere->color = color;
	
	return tempSphere;

}

void worldInit(World *world) {

	if(world == NULL) exit(-1); // Failed to alloc mem for spheres arr.
	// Set variable values
	world->size = 0;
	world->capacity = 1;

	// Create space for spheres array;
	
	world->spheres = malloc(world->capacity * sizeof(Sphere *)); // (Sphere **)malloc(world->size * sizeof(Sphere *));
	
	if(world->spheres == NULL) {
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1); // Failed to alloc mem for spheres arr.
	}
}

void freeWorld(World *world) {
	// free every index 
	for (int i = 0; i < world->size; i++) {
        	free(world->spheres[i]);
    	}
	// free the sphere and set all vars to 0.
	free(world->spheres);
	if(world->spheres != NULL) {world->spheres = NULL;}
	world->size = 0;
	world->capacity = 0;	
	free(world);
}



void addSphere(World *world, Sphere *sphere) {
	//Append given sphere to the end of the world.spheres
	
	// if size++  == capacity < capacity * 2.
	if(world->size >= world->capacity) {
		world->capacity *= 2;
		
		//Realloc space for new capacity
		Sphere **temp = realloc(world->spheres, world->capacity * sizeof(Sphere *));
		if(temp == NULL) {
			fprintf(stderr, "Memory Allocation failed.\n");
			exit(-1);
		}
		world->spheres = temp;

	}

	world->spheres[world->size] = sphere; // add sphere to end of the array
	world->size += 1; // increment by 1

} 


