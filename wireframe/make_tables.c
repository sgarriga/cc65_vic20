/* Not the prettiest code, but cc65 doesn't support math.h or floats. As a
 * result I'm pre-calculating plot points. 
 * Scaling is because the original code used a 1024 x 1024 screen size.
 * TGI lib for the VIC-20 says 160 x 192 (zero based)
 * The cube table format is to best match xx's design
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define CENTER_X 450
#define CENTER_Y 550

#define REAL_MAXX 160
#define REAL_MAXY 192
#define PLOT_MAXX 1024
#define PLOT_MAXY 1024

static int scaleX(float x) {
	return ((int) ((float) x * (float) REAL_MAXX / (float) PLOT_MAXX));
}
static int scaleY(float y) {
	return ((int) ((float) y * (float) REAL_MAXY / (float) PLOT_MAXY));
}

void rotate(float rads, int xi, int yi, int *xo, int *yo) {
	float sin_a, cos_a, x, y;

	sin_a = sin(rads);
	cos_a = cos(rads);

	// shift to a 0, 0 origin
	x = xi - CENTER_X;
	y = yi - CENTER_Y;

	// rotate around 0, 0 then revert to prior origin
	*xo = scaleX((x * cos_a - y * sin_a) + (float) CENTER_X);
	*yo = scaleY((x * sin_a + y * cos_a) + (float) CENTER_Y);
}

int main() {
	typedef int ptype[4];

	ptype *x, *y;
	int num_points = 0, i;
	float sin_a, cos_a, rads;

	FILE *h_file = NULL;

	h_file = fopen("./cube.h", "w");
	if (h_file == NULL) {
		int r = errno;
		fprintf(stderr, "Error %d writing cube.h\n", r);
		exit(r);
	}

	fprintf(h_file, "/* Machine generated code (see %s) do not modify */\n\n", __FILE__);

#define LOOP for (float a=0.8; a <= 2.34; a+=0.07)
	LOOP
		num_points++; 
	fprintf(h_file, "#define NUM_POINTS %d\n\n", num_points);

	x = calloc(num_points, sizeof(ptype));
	if (x == NULL) {
		fprintf(stderr, "calloc failed\n");
		exit(-1);
	}
	y = calloc(num_points, sizeof(ptype));
	if (y == NULL) {
		fprintf(stderr, "calloc failed\n");
		exit(-1);
	}

	i = 0;
	LOOP {
		sin_a = sin(a);
		cos_a = cos(a);
		x[i][0] = scaleX((float) CENTER_X + (cos_a * 212.0));
		x[i][1] = scaleX((float) CENTER_X - (cos_a * 212.0));
		x[i][2] = scaleX((float) CENTER_X + (sin_a * 212.0));
		x[i][3] = scaleX((float) CENTER_X - (sin_a * 212.0));
		y[i][0] = scaleY((float) CENTER_Y + (sin_a * 212.0));
		y[i][1] = scaleY((float) CENTER_Y - (sin_a * 212.0));
		y[i][2] = scaleY((float) CENTER_Y + (cos_a * 212.0));
		y[i][3] = scaleY((float) CENTER_Y - (cos_a * 212.0));
                i++;
	}
	fprintf(h_file, "static const int x[NUM_POINTS][4] = {\n");
	for (i = 0; i < num_points - 1; i++)
		fprintf(h_file, "\t{ %d, %d, %d, %d },\n", x[i][0], x[i][1], x[i][2], x[i][3]);
	fprintf(h_file, "\t{ %d, %d, %d, %d }};\n", x[i][0], x[i][1], x[i][2], x[i][3]);

	fprintf(h_file, "static const int y[NUM_POINTS][4] = {\n");
	for (i = 0; i < num_points - 1; i++)
		fprintf(h_file, "\t{ %d, %d, %d, %d },\n", y[i][0], y[i][1], y[i][2], y[i][3]);
	fprintf(h_file, "\t{ %d, %d, %d, %d }};\n", y[i][0], y[i][1], y[i][2], y[i][3]);
	fclose(h_file);

	free(x);
	free(y);

	// -------------------------------

	h_file = fopen("./pyr3.h", "w");
	if (h_file == NULL) {
		int r = errno;
		fprintf(stderr, "Error %d writing cube.h\n", r);
		exit(r);
	}

	fprintf(h_file, "/* Machine generated code (see %s) do not modify */\n\n", __FILE__);

	num_points = (360 / 5) + 1;
	fprintf(h_file, "#define NUM_POINTS %d\n\n", num_points);

	x = calloc(num_points, sizeof(ptype));
	if (x == NULL) {
		fprintf(stderr, "calloc failed\n");
		exit(-1);
	}

	y = calloc(num_points, sizeof(ptype));
	if (y == NULL) {
		fprintf(stderr, "calloc failed\n");
		exit(-1);
	}

	for (int a=0, i=0; a < 360; a+=5, i++) {
		rads = (float) a * M_PI / 180.0;
		rotate(rads, CENTER_X -  25, CENTER_Y - 175, &x[i][0], &y[i][0]);
		rotate(rads, CENTER_X - 175, CENTER_Y + 225, &x[i][1], &y[i][1]);
		rotate(rads, CENTER_X + 175, CENTER_Y + 175, &x[i][2], &y[i][2]);
		rotate(rads, CENTER_X      , CENTER_Y      , &x[i][3], &y[i][3]);
	}

	fprintf(h_file, "typedef struct {int x; int y; } pair;\n");
	fprintf(h_file, "static const pair xy[NUM_POINTS][4] = {\n");
	for (i = 0; i < num_points - 1; i++)
		fprintf(h_file, "\t{ {%d, %d}, {%d, %d}, {%d, %d}, {%d, %d} },\n", x[i][0], y[i][0], x[i][1], y[i][1], x[i][2], y[i][2], x[i][3], y[i][3]);
	fprintf(h_file, "\t{ {%d, %d}, {%d, %d}, {%d, %d}, {%d, %d} }};\n", x[i][0], y[i][0], x[i][1], y[i][1], x[i][2], y[i][2], x[i][3], y[i][3]);
	fclose(h_file);

	free(x);
	free(y);

	exit(0);
}
