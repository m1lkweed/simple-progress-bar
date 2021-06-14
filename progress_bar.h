// Simple command-line progress bar in C
// (c)m1lkweed 2020 GPLv3+
// Works on all ANSI-compliant, UNIX-like terminals with compatible fonts
//
// Progress bar has several styles:
// 0.) No style, bar snaps to nearest integer
// 1.) Halves, bar shows half-steps
// 2.) Gradient, bar shows quarter-steps
// 3.) Slices, bar shows eighth-steps (not supported on all terminals)
// 4.) Same as 0, but clears line from end of bar (useful when bar shrinks)
// 5.) Same as 1, but clears line from end of bar
// 6.) Same as 2, but clears line from end of bar
// 7.) Same as 3, but clears line from end of bar (not supported on all terminals)
//
// progress_bar() returns -1 for invalid locations, -2 for negative bar lengths, -3 for invalid styles, otherwise returns 0

#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

int progress_bar(const unsigned x, const unsigned y, long double length, int style, const char *color);

#ifdef PROGRESS_BAR_IMPLEMENTATION

#include <stdio.h>
#include <stdint.h>

int progress_bar(const unsigned x, const unsigned y, long double length, int style, const char *color){
	const char *s[] = {"\xe2\x96\x88", //shades
	                   "\xe2\x96\x91",
	                   "\xe2\x96\x92",
	                   "\xe2\x96\x93"};
	const char *h[] = {"\xe2\x96\x88", //slices
	                   "\xe2\x96\x8f",
	                   "\xe2\x96\x8e",
	                   "\xe2\x96\x8d",
	                   "\xe2\x96\x8c",
	                   "\xe2\x96\x8b",
	                   "\xe2\x96\x8a",
	                   "\xe2\x96\x89"};
	const char *safe_exit = "\x1b[u\x1b[39;49m"; //pop cursor position and reset colors
	if((x <= 0)||(y <= 0))return -1; //check valid inputs
	if(length < 0)return -2;
	fprintf(stdout, "\x1b[s\x1b[%d;%dH%s", x, y, color?color:""); //push and move cursor position
	switch(style % 4){
	case 0: while(0.5 <= length--)fputs(s[0], stdout);break; //no style
	case 1: while(1 <= length){fputs(h[0], stdout);--length;}if(length >= 0.5)fputs(h[4], stdout); break; //halves
	case 2: while(1 <= length){fputs(s[0], stdout);--length;}if(length >= 0.25)fputs(s[((int)(length*4))%4], stdout); break; //gradient
	case 3: while(1 <= length){fputs(s[0], stdout);--length;}if(length >= 0.125)fputs(h[((int)(length*8))%8], stdout); break; //eighths
	default: fputs(safe_exit, stdout); return -3;
	}
	if(style > 3)fputs("\x1b[0K", stdout); //clear line after bar
	fputs(safe_exit, stdout);
	fflush(stdout); //prevents visual errors
	return 0;
}

#endif
#endif
