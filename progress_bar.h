// simple command-line progress bar in C
// (c)m1lkweed 2020 GPLv3+
// works on all ANSI-compliant, UNIX-like terminals with compatible fonts
//
// progress bar has several styles:
// 0.) No style, bar snaps to nearest integer
// 1.) Halves, bar shows half-steps
// 2.) Gradient, bar shows quarter-steps
// 3.) Slices, bar shows eighth-steps (not supported on all terminals)
// 4.) Same as 0, but clears line from end of bar (useful when bar shrinks)
// 5.) Same as 1, but clears line from end of bar
// 6.) Same as 2, but clears line from end of bar
// 7.) Same as 3, but clears line from end of bar (not supported on all terminals)
//
//progress_bar returns -1 for invalid locations, -2 for negative bar lengths, -3 for invalid styles, otherwise returns 0

#include <stdio.h>

int progress_bar(unsigned short x, unsigned short y, long double length, int style, const char* color){
	const char* s[] = {"\xe2\x96\x88", //shades
	                   "\xe2\x96\x91",
	                   "\xe2\x96\x92",
	                   "\xe2\x96\x93"};
	const char* h[] = {"\xe2\x96\x88", //slices
	                   "\xe2\x96\x8f",
	                   "\xe2\x96\x8e",
	                   "\xe2\x96\x8d",
	                   "\xe2\x96\x8c",
	                   "\xe2\x96\x8b",
	                   "\xe2\x96\x8a",
	                   "\xe2\x96\x89"};
	const char* safe_exit = "\x1b[u\x1b[39;49m"; //pop cursor position and reset colors
	if((x == 0)||(y == 0))return -1; //check valid inputs
	if(length < 0)return -2;
	printf("\x1b[s\x1b[%d;%dH%s", x, y, color); //push and move cursor position
	switch(style % 4){
	case 0: while(0.5 <= length--)printf("%s",s[0]);break; //no style
	case 1: while(1 <= length){printf("%s",h[0]);--length;}if(length >= 0.5)printf("%s",h[4]);break; //halves
	case 2: while(1 <= length){printf("%s",s[0]);--length;}if(length >= 0.25)printf("%s",s[((int)(length*4))%4]);break; //gradient
	case 3: while(1 <= length){printf("%s",s[0]);--length;}if(length >= 0.125)printf("%s",h[((int)(length*8))%8]);break; //eighths
	default: printf(safe_exit); return -3;
	}
	if(style > 3)printf("\x1b[0K"); //clear line after bar
	printf(safe_exit);
	fflush(stdout); //prevents visual errors
	return 0;
}
