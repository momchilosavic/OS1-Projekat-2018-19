#include "Timer.h"
#include "Kernel.h"
#include "IVTEntry.h"
#include "thread.h"
#include "semaphor.h"
#include <stdio.h>

extern int userMain(int argc, char** argv);
//void tick() {}

void pr(){
	printf("A");
}


int main(int argc, char**argv){
	volatile int i = 1;
	Timer::inic();
		userMain(argc, argv);
	Timer::restore();
	return 0;
}
