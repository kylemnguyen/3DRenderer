/*
 * contains main file.
 *
 *
 * */

#include "assg.h"

int main(int argc, char*argv[]) { 
	
	// get input and output files
	input = fopen(argv[1], "r"); // read file inputs
	if(input == NULL) { 
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1); 
	}
	output = fopen(argv[2], "w");
	if(output == NULL) { 
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1); 
	}

    //Initalize clrs, indexes and world

	clrs = NULL;
	clrIndex = NULL;
	cameraPos = (Vec3){0,0,0};

	//Initalize World
	world = malloc(sizeof(World));
	if(world == NULL) { 
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1);
	}
	worldInit(world);

	// get variables
	/*
	 *<image width (int)> <image height (int)>
	 <viewport height (float)>
	 <focal length (float)>
 	 <light position (3float)> <light brightness (float)>
	 <number of colors `m` (int)>
	 * */

	fscanf(input, "%d %d", &width, &height);
	fscanf(input, "%f", &vpHeight);
	fscanf(input, "%f", &focalLength);	
	fscanf(input, "%f %f %f %f", &lightSrc.x, &lightSrc.y, &lightSrc.z, &lightStr);
	fscanf(input, "%d", &clrsAmt);

	//Get aspect ratio & vwidth
	aspectRatio = (float)width / (float)height;
	vpWidth = aspectRatio * vpHeight;

	// make space for clrs
	unsigned int* tclr = realloc(clrs, clrsAmt * sizeof(int));
	if(tclr == NULL) { 
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1); 
	} // REALLOC FAILED
	
	clrs = tclr;

	// <color 1 (HEX)> <color 2 (HEX)> ... <color m (HEX)>
	// vvv to be updated
	for(int i = 0; i < clrsAmt; i++) {
		fscanf(input, "%x", clrs + i);
	}
	// ^^ Quicksort this arr
	Qsort(clrs, 0, clrsAmt - 1);

	
	/** <background color index (int)>
		<number of spheres `n` (int)>*/
	fscanf(input, "%d", &bgClrIndex);
	fscanf(input, "%d", &spheresAmt);

	//vvvvv remove this future me

	// make index arr
	int *temp = realloc(clrIndex, spheresAmt * sizeof(int));
	if(temp == NULL) { 
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(-1); 
	} // REALLOC FAILED
	clrIndex = temp;

	//Sooon get rid of this ^^^ and make pallete that is sorted

    //<sphere 1 position (3float)> <sphere 1 radius (float)> <sphere 1 color index (int)>  * sphere amt
    for(int i = 0; i < spheresAmt; i++) {
		
		float x,y,z,radius;
		Vec3 pos, color = {255,255,255};
		fscanf(input, "%f%f%f%f%d", &x, &y, &z, &radius, clrIndex + i);
		
		#ifdef FS
			color = unpackRGB(clrs[*(clrIndex + i)]);
		#endif
		pos.x = x;
		pos.y = y;
		pos.z = z;
		// color = colorpallete(colorindex); << find out this code
		
		Sphere *temp = createSphere(radius, pos, color); 
		addSphere(world, temp);
	}

	#ifdef MS1
		Output1();
	#endif
	#ifdef MS2
		Output2();
	#endif
	#ifdef FS
		final();
	#endif
	// Free all dynamic memory 
	fclose(input);
	fclose(output);
	free(clrs);
	free(clrIndex);
	freeWorld(world);
}

void final() {
	// OUTPUT PPM
	fprintf(output, "P3\n%d %d\n255\n", width, height);
	
	Vec3 horz, vert, corner;

	horz = (Vec3){vpWidth, 0.0f, 0.0f};
	vert = (Vec3){0.0f, vpHeight, 0.0f};
	corner = (Vec3) {
		cameraPos.x - (horz.x / 2) - (vert.x / 2),
		cameraPos.y - (horz.y / 2) - (vert.y / 2),
		cameraPos.z - focalLength
	};

	//Get each pixel of (w,h)
	for(int y = height - 1 ; y >= 0; y--) {

		for(int x = 0; x < width; x++) { 

			Vec3 clr = {0,0,0};

			for(int i = 0; i < 3; i++) {
				
				for(int j = 0;j < 3; j++) {

					Vec3 pixelClr, directionRay;
					float midX, midY;
					midX = (x + (j / 90.0f) / 3.0f) / (width - 1);
					midY = (y +(i / 90.0f) / 3.0f) / (height - 1);
					directionRay = add(corner, add(scalarMultiply(midX, horz), scalarMultiply(midY, vert))); // get direction ray "p + td"
					
					pixelClr = getColour(directionRay);
					clr = add(pixelClr, clr);

				}
				
			}

			clr = scalarDivide(clr, 9);
			writeColour(output, clr);
			
		}

		fprintf(output, "\n");
	}
}

Vec3 getColour(Vec3 ray) {

	Vec3 color = unpackRGB(clrs[bgClrIndex]);
	Vec3 directionRay = ray;
	float t = INFINITY;
	int sid;

	for(int i = 0; i < world->size; i++) {	
		float temp;
		if(doesIntersect(world->spheres[i], cameraPos, directionRay, &temp) && temp < t) {
			t = temp;
			sid = i;
		}				
	}

	if(t != INFINITY) {
		//color = I*S, I = min(1, Io), Io = brightness * ( max(dot(normal, lightdirection), 0) / distance(lightpose, intersection point) ^ 2 )

		// get intensity and interesctio npt and get colour and AGH I FUCKING HATE THIS SHIT THIS FUCKING CODE
		color = world->spheres[sid]->color;
		//printf("%d", world->spheres[sid]->color);

		Vec3 intersectionPoint = add(cameraPos, scalarMultiply(t, directionRay));
		Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[sid]->pos));
		Vec3 lightdirection;
		float dotV, dist, intensity;
		
		lightdirection = normalize(subtract(lightSrc, intersectionPoint));
		dotV = dot(normal, lightdirection);
		// Clamp
		if(dotV < 0) dotV = 0;

		dist = distance2(intersectionPoint, lightSrc);
		intensity = lightStr * (dotV / dist);

		// Clamp
		if(intensity > 1) intensity = 1;

		//SHADOW CHECK!!!
		Vec3 shadowCheck = add(intersectionPoint, scalarMultiply(0.0001, normal));//p + 0.0001n

		for(int i = 0; i < world->size; i++) {	
			float temp;
			if(doesIntersect(world->spheres[i], shadowCheck, lightdirection, &temp)) {
				if(temp > 0 && temp < distance(intersectionPoint, lightSrc)) {
					intensity = 0.01; // Is it 0.01 or 0.1?git
					break;
				}
				
			}				
		}

		color = scalarMultiply(intensity, color);

	}

	return color;
}

void Output2() {

	// OUTPUT PPM
	fprintf(output, "P3\n%d %d\n255\n", width, height);
	
	Vec3 horz, vert, corner;

	horz = (Vec3){vpWidth, 0.0f, 0.0f};
	vert = (Vec3){0.0f, vpHeight, 0.0f};
	corner = (Vec3) {
		cameraPos.x - (horz.x / 2) - (vert.x / 2),
		cameraPos.y - (horz.y / 2) - (vert.y / 2),
		cameraPos.z - focalLength
	};

	//Get each pixel of (w,h)
	for(int y = height - 1 ; y >= 0; y--) {
		for(int x = 0; x < width; x++) { 
			
			//get the center of the pixel
			float midX, midY;
			midX = (x + 0.5) / (width -  1);
			midY = (y + 0.5) / (height - 1);
			Vec3 color = bgClr; // color of sphere

			//Initalize variables to get sphere index, scalar t and direction ray
			Vec3 directionRay = add(corner, add(scalarMultiply(midX, horz), scalarMultiply(midY, vert))); // get direction ray "p + td"
			float t = INFINITY; //scalar t
			int sid; // sphere index

			// iterate through all spheres here find closest itersection pt.
			for(int i = 0; i < world->size; i++) {	
				float temp;
				if(doesIntersect(world->spheres[i], cameraPos, directionRay, &temp) && temp < t) {
					t = temp;
					sid = i;
				}				
			}

			if (t != INFINITY) {

				//color = I*S, I = min(1, Io), Io = brightness * ( max(dot(normal, lightdirection), 0) / distance(lightpose, intersection point) ^ 2 )

				// get intensity and interesctio npt and get colour and AGH I FUCKING HATE THIS SHIT THIS FUCKING CODE
				color = world->spheres[sid]->color;
				//color = white;

				Vec3 intersectionPoint = add(cameraPos, scalarMultiply(t, directionRay));
				Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[sid]->pos));
				Vec3 lightdirection;
				float dotV, dist, intensity;
				
				lightdirection = normalize(subtract(lightSrc, intersectionPoint));
				dotV = dot(normal, lightdirection);
				// Clamp
				if(dotV < 0) dotV = 0;

				dist = distance2(intersectionPoint, lightSrc);
				intensity = lightStr * (dotV / dist);

				// Clamp
				if(intensity > 1) intensity = 1;

				//Check if shadowed
				Vec3 shadowCheck = add(intersectionPoint, scalarMultiply(0.001, normal));//p + 0.0001n
				for(int i = 0; i < world->size; i++) {	
					float temp;
					if(doesIntersect(world->spheres[i], shadowCheck, lightdirection, &temp)) {
						if(temp > 0 && temp < distance(intersectionPoint, lightSrc)) {
							intensity = 0.03; // Is it 0.01 or 0.1?git
							break;
						}
						
					}				
				}

				color = scalarMultiply(intensity, color);
			} 
			writeColour(output, color); // printout colour
		}
		fprintf(output, "\n");
	}
}

void Output1() { 

	/* 
	 * 1 <background color (Vec3)> + <light position (Vec3)> = <result (Vec3)>
     * 2 <background color (Vec3)> - <light position (Vec3)> = <result (Vec3)>
	 * 3 <viewport width (float)> * <light position (Vec3)> = <result (Vec3)>
	 * 4 normalize<light position (Vec3)> = <result (Vec3)>
	 * a <sphere color (Vec3)> / <sphere radius (float)> = <result (Vec3)>
	 * b dot(<light position (Vec3)>, <sphere position (Vec3)>) = <result (float)>
	 * c distance(<light position (Vec3)>, <sphere position (Vec3)>) = <result (float)>
	 * d length<sphere position (Vec3)> = <result (float)>
	 **/

	Vec3 vector;
	Vec3 zero = {0,0,0};
	bgClr = zero;

	// 1
	vector = add(bgClr, lightSrc);
	fprintf(output, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
			bgClr.x, bgClr.y, bgClr.z, lightSrc.x, lightSrc.y, lightSrc.z, vector.x, vector.y, vector.z);
	// 2
	vector = subtract(bgClr, lightSrc);
        fprintf(output, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
                        bgClr.x, bgClr.y, bgClr.z, lightSrc.x, lightSrc.y, lightSrc.z, vector.x, vector.y, vector.z);
	// 3
	vector = scalarMultiply(vpWidth, lightSrc);
	fprintf(output, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
			vpWidth, lightSrc.x, lightSrc.y, lightSrc.z, vector.x, vector.y, vector.z);

	// 4
	vector = normalize(lightSrc);
	fprintf(output, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",	
	    		lightSrc.x, lightSrc.y, lightSrc.z, vector.x, vector.y, vector.z);

	for (int i = 0; i < world->size; i++) {
		fprintf(output, "\n");
		Vec3 one = {1,1,1};
		float value;
		Sphere *temp;
		temp = world->spheres[i];
		temp->color = one;
		
		//a
		vector = scalarDivide(temp->color, temp->r);
		fprintf(output, "(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
                                temp->color.x, temp->color.y, temp->color.z, temp->r, vector.x, vector.y, vector.z);
		//b
		value = dot(lightSrc, temp->pos);
		fprintf(output, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                                lightSrc.x, lightSrc.y, lightSrc.z, temp->pos.x, temp->pos.y, temp->pos.z, value);
		//c
		value = distance(lightSrc, temp->pos);
		fprintf(output, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                                lightSrc.x, lightSrc.y, lightSrc.z, temp->pos.x, temp->pos.y, temp->pos.z, value);
		//d
		value = length(temp->pos);
                fprintf(output, "length(%.1f, %.1f, %.1f) = %.1f\n",
                                temp->pos.x, temp->pos.y, temp->pos.z, value);

	}		
}

int partition(unsigned int *arr, int low, int high) {
	
	unsigned int pivot = arr[high];
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if(compareColor(&arr[j], &pivot) < 0) { // compare the colours
			i++;
			//swap colours
			unsigned int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}

	// last swap
	unsigned int temp = arr[i+1];
	arr[i + 1] = arr[high];
	arr[high] = temp;

	return i + 1;

}

void Qsort(unsigned int *arr, int low, int high) {
	if (low < high) {
		int x = partition(arr, low, high);

		Qsort(arr, low, x - 1);
		Qsort(arr, x + 1, high);
	}
}