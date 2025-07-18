/* Not the prettiest code, but cc65 doesn't support math.h or floats. As a
 * result I'm pre-calculating plot points. 
 * Scaling is because the original code used a 1024 x 1024 screen size.
 * TGI lib for the VIC-20 says 160 x 192 (zero based)
**/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define CENTER_X 350
#define CENTER_Y 550

#define REAL_MAXX 160
#define REAL_MAXY 192
#define PLOT_MAXX 1024
#define PLOT_MAXY 1024

int scaleX(int x) {
    return (int) ((double) x * (double) REAL_MAXX / (double) PLOT_MAXX);
}
int scaleY(int y) {
    return (int) ((double) y * (double) REAL_MAXY / (double) PLOT_MAXY);
}

int main() {
	int x1, x2, x3, x4;
	int y1, y2, y3, y4;
        int c = 0;
        double sin_a, cos_a;

        FILE *plots = NULL;

        plots = fopen("./plots.h", "w");
        if (plots == NULL) {
            int r = errno;
            fprintf(stderr, "Error %d opening plots.h\n", r);
            exit(r);
        }

        fprintf(plots, "/* Machine generated code (see %s) do not modify */\n\n", __FILE__);

#define LOOP for (float a=0.8; a <= 2.34; a+=0.07)
	LOOP
            c++; 
        fprintf(plots, "#define NUM_POINTS %d\n\n", c);

        fprintf(plots, "static const int x[NUM_POINTS][4] = {\n");
        LOOP {
                sin_a = sin(a);
                cos_a = cos(a);
		x1 = scaleX(CENTER_X + (cos_a * 212));
		x2 = scaleX(CENTER_X - (cos_a * 212));
		x3 = scaleX(CENTER_X + (sin_a * 212));
		x4 = scaleX(CENTER_X - (sin_a * 212));

		fprintf(plots, "\t{ %d, %d, %d, %d },\n", x1, x2, x3, x4);
	}
        // roll back over last comma
        fseek(plots, (strlen("\n") == 1) ? -2 : -3, SEEK_CUR);
        fprintf(plots, "};\n\n");

        fprintf(plots, "static const int y[NUM_POINTS][4] = {\n");
	LOOP {
                sin_a = sin(a);
                cos_a = cos(a);
		y1 = scaleY(CENTER_Y + (sin_a * 212));
		y2 = scaleY(CENTER_Y - (sin_a * 212));
		y3 = scaleY(CENTER_Y + (cos_a * 212));
		y4 = scaleY(CENTER_Y - (cos_a * 212));

		fprintf(plots, "\t{ %d, %d, %d, %d },\n", y1, y2, y3, y4);
	}
        // roll back over last comma
        fseek(plots, (strlen("\n") == 1) ? -2 : -3, SEEK_CUR);
        fprintf(plots, "};\n\n");
        fclose(plots);
        exit(0);
}
