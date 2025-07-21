/*
 * Rotating Box by Dave Needham was originally in VIC-20 Super Expander Basic
 * This cc65 C version by Stephen Garriga
**/
#include <stdlib.h>
#include <conio.h>
#include <tgi.h>
#include <cc65.h>

/* the plot points are all precalulated because cc65 doesn't have math.h or
 * float support
**/
#include "cube.h"

/* Use static local variables for speed */
#pragma static-locals (1)

void plot()
{
	int point_set;

	tgi_setcolor (1);

	for (point_set=0; point_set < NUM_POINTS; point_set++) {
		tgi_clear ();

		tgi_line(x[point_set][0], y[point_set][1], x[point_set][3], y[point_set][3]);
		tgi_lineto(x[point_set][1], y[point_set][0]);
		tgi_lineto(x[point_set][2], y[point_set][2]);
		tgi_lineto(x[point_set][0], y[point_set][1]);

		tgi_line(x[point_set][0], y[point_set][1], x[point_set][0] + 25, y[point_set][1] - 25);
		tgi_line(x[point_set][0] + 25, y[point_set][1] - 25, x[point_set][3] + 25, y[point_set][3] - 25);
		tgi_line(x[point_set][3] + 25, y[point_set][3] - 25, x[point_set][3], y[point_set][3]);

		tgi_line(x[point_set][2], y[point_set][2], x[point_set][2] + 25, y[point_set][2] - 25);
		tgi_line(x[point_set][2] + 25, y[point_set][2] - 25, x[point_set][1] + 25, y[point_set][0] - 25);
		tgi_line(x[point_set][1] + 25, y[point_set][0] - 25, x[point_set][1], y[point_set][0]);

		tgi_line(x[point_set][3] + 25, y[point_set][3] - 25, x[point_set][1] + 25, y[point_set][0] - 25);
		tgi_line(x[point_set][0] + 25, y[point_set][1] - 25, x[point_set][2] + 25, y[point_set][2] - 25);

	}
}

int main (void)
{
	unsigned char err;
	char *k = (char *)0x00c6; // # chars in kbd buff

	clrscr ();

#if DYN_DRV
	/* Load the graphics driver */
	cprintf ("initializing... mompls\r\n");
	tgi_load_driver (tgi_stddrv);
#else
	/* Install the graphics driver */
	tgi_install (tgi_static_stddrv);
#endif
	err = tgi_geterror ();
	if (err  != TGI_ERR_OK) {
		cprintf ("Error #%d initializing graphics.\r\n%s\r\n",
				err, tgi_geterrormsg (err));
		if (doesclrscrafterexit ()) {
			cgetc ();
		}
		exit (EXIT_FAILURE);
	};
	cprintf ("ok.\n\r");

	/* Initialize graphics */
	tgi_init ();

	while(!*k)
		plot();
        *k = 0;

	/* Shut down gfx mode and return to textmode */
	tgi_done ();

	/* Done */
	return EXIT_SUCCESS;
}
