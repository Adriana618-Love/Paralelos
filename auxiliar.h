#include <cuda.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>

#define RGB_COMPONENT_COLOR 255

#define BLUR_SIZE 5

using namespace std;

typedef struct {
	unsigned char red, green, blue;
} PPMPixel;

typedef struct {
	int x, y;
	PPMPixel *data;
} PPMImage;

unsigned char *readPPM(const char *filename, int &x, int &y)
{
	char buff[16];
	unsigned char* imgchar;
	FILE *fp;
	int c, rgb_comp_color;
	//open PPM file for reading
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}

	//read image size information
	if (fscanf(fp, "%d %d", &x, &y) != 2) {
		fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
		exit(1);
	}

	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
		fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
		exit(1);
	}

	//check rgb component depth
	if (rgb_comp_color != RGB_COMPONENT_COLOR) {
		fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
		exit(1);
	}

	while (fgetc(fp) != '\n');
	//memory allocation for pixel data
	imgchar = (unsigned char*)malloc(3 * x*y * sizeof(char));

	//read pixel data from file
	fread(imgchar, 3 * x, y, fp);


	fclose(fp);
	return imgchar;
}

void writePPM(unsigned char * img, int x, int y)
{
	FILE *fp;
	//open file for output
	fp = fopen("C:\\UCSP\\2019-I\\AP\\Practice\\blur\\blur\\img_blur.ppm", "wb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", "out");
		exit(1);
	}

	//write the header file
	//image format
	fprintf(fp, "P6\n");

	//image size
	fprintf(fp, "%d %d\n", x, y);

	// rgb component depth
	fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

	// pixel data
	fwrite(img, 3 * x, y, fp);
	fclose(fp);
}

unsigned char* readBMP(char* file_name, int &width, int &height) {
	FILE* img = fopen(file_name, "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, img);
	width = *(int*)&header[18];
	height = *(int*)&header[22];
	int size = width * height * 3;
	unsigned char* r_img = (unsigned char*)malloc(size * sizeof(unsigned char));
	fread(r_img, sizeof(unsigned char), size, img);
	fclose(img);
	return r_img;
}

void writeBMP(unsigned char* img, int width, int height) {
	FILE* f_img;
	int f_size = 54 + 3 * width* height;
	unsigned char file_header[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char info_header[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char pad[3] = { 0,0,0 };
	file_header[2] = (unsigned char)(f_size);
	file_header[3] = (unsigned char)(f_size >> 8);
	file_header[4] = (unsigned char)(f_size >> 16);
	file_header[5] = (unsigned char)(f_size >> 24);
	info_header[4] = (unsigned char)(width);
	info_header[5] = (unsigned char)(width >> 8);
	info_header[6] = (unsigned char)(width >> 16);
	info_header[7] = (unsigned char)(width >> 24);
	info_header[8] = (unsigned char)(height);
	info_header[9] = (unsigned char)(height >> 8);
	info_header[10] = (unsigned char)(height >> 16);
	info_header[11] = (unsigned char)(height >> 24);
	f_img = fopen("C:\\UCSP\\2019-I\\AP\\Practice\\blur\\blur\\img_blur.bmp", "wb");
	fwrite(file_header, 1, 14, f_img);
	fwrite(info_header, 1, 40, f_img);
	for (int i = height - 1; i >= 0; i--) {
		fwrite(img + (width * (height - i - 1) * 3), 3, width, f_img);
		fwrite(pad, 1, (4 - (width * 3) % 4) % 4, f_img);
	}
	free(img);
	fclose(f_img);
}