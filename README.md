# cc65_vic20
VIC-20 demo code for use with the cc65 compiler suite and xvic

I came across a Facebook post by Dave Needham showing a "Rotating Box" program written in VIC-20 + Super Expander BASIC and wondered if I could port to C using cc65 and the TGI library.
Real-time calculation of sine and cosine is problematic because of the lack of floating point support in cc65, but I wote a separate command line program to pre-generate all the plot points.
Having gotten a cube to rotate, I added a 3-sided pyramid - I figured put the initial pot points by eye, rather than doing the math so all the faces might not be equilateral.

To play: (requires working cc65 and xvic installs)
* clone this repository
* cd wireframe
* make
* make test-cube # cube wireframe rotation
* make test-pyr3 # 3-sided pyramid wireframe rotation
