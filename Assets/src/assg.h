#ifndef ASSG_H
#define ASSG_H


#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"

FILE *input, *output;

//Camera & Vectors
int height, width, clrsAmt, bgClrIndex, spheresAmt; 
float vpHeight, vpWidth, focalLength, aspectRatio, lightStr;
Vec3 lightSrc, bgClr, cameraPos;

//Spheres
unsigned int *clrs; // wont ever be neg.
int *clrIndex;

//World
World *world; 

void final();
void Output1();
void Output2();
void Qsort(unsigned int *arr, int low, int high);
Vec3 getColour(Vec3 directionRay);
#endif
