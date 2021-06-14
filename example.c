// sample program for progress_bar.h
// fills large array and shows progress

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

#define PROGRESS_BAR_IMPLEMENTATION
#include "progress_bar.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"

// isatty() works for all terminals, so we need to be
// creative to check for an actual tty vs. xterm
static int is_a_console(int fd){
	char arg;
	arg = 0;
	return (ioctl(fd, KDGKBTYPE, &arg) == 0
		&& ((arg == KB_101) || (arg == KB_84)));
}

static int open_a_console(const char *fnam){
	int fd;
	fd = open(fnam, O_RDWR);
	if(fd < 0 && errno == EACCES)
		fd = open(fnam, O_WRONLY);
	if(fd < 0 && errno == EACCES)
		fd = open(fnam, O_RDONLY);
	if(fd < 0)
		return -1;
	if(!is_a_console(fd)){
		close(fd);
		return -1;
	}
	return fd;
}

int getfd(const char *fnam){
	int fd;
	if(fnam){
		fd = open_a_console(fnam);
		if(fd >= 0)
			return fd;
		exit(1);
	}
	fd = open_a_console("/dev/tty");
	if(fd >= 0)
		return fd;
	fd = open_a_console("/dev/tty0");
	if(fd >= 0)
		return fd;
	fd = open_a_console("/dev/vc/0");
	if(fd >= 0)
		return fd;
	fd = open_a_console("/dev/console");
	if(fd >= 0)
		return fd;
	for(fd = 0; fd < 3; ++fd)
		if(is_a_console(fd))
			return fd;
	return -1;
}

int main(){
	double i;
	int style;
	//if getfd fails we're probably in a terminal emulator
	if(getfd(NULL) < 0){
		style = 3;   //assume emulators support UTF-8
	}else{
		style = 1;   //fallback for virtual terminals
	}
	
	for(i = 0; i < 100; i += 0.01){
		usleep(500L);
		progress_bar(1, 1, i, style, "");
	}

	style += 4;

	for(; i > 66; i -= 0.01){
		usleep(500L);
		progress_bar(1, 1, i, style, GREEN);
	}
	for(; i > 33; i -= 0.05){
		usleep(500L);
		progress_bar(1, 1, i, style, YELLOW);
	}
	for(; i > 0; i -= 0.1){
		usleep(500L);
		progress_bar(1, 1, i, style, RED);
	}
}
