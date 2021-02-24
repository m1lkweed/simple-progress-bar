// sample program for progress_bar.h
// fills large array and shows progress

#include <unistd.h>
#include "progress_bar.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"

int main(){
	double i;
	for(i = 0; i < 100; i += 0.01){
		usleep(500L);
		progress_bar(1,1, i, 3, "");
	}
	for(; i > 66; i -= 0.01){
		usleep(500L);
		progress_bar(1,1, i, 7, GREEN);
	}
	for(; i > 33; i -= 0.05){
		usleep(500L);
		progress_bar(1,1, i, 7, YELLOW);
	}
	for(; i > 0; i -= 0.1){
		usleep(500L);
		progress_bar(1,1, i, 7, RED);
	}
}
