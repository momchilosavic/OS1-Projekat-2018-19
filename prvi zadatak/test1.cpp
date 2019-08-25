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

#include <stdlib.h>
#include <time.h>

class TestThread : public Thread{
	public:
		static Semaphore* sem;
		TestThread() : Thread(1024, 2) {}
		~TestThread(){ waitToComplete();}
		void run(){
			volatile int i = 1;
			volatile int cnt = 0;
			while(i < 5){
				i++;
				cnt++;
				Kernel::lock = 1;
				printf("\nID: %d\tBroj: %d", this->getRunningId(), cnt);
				Kernel::lock = 0;
				if(Kernel::switch_after_lock == 1)
					dispatch();
				sem->wait(getRunningId()*5);
			}
		}
};

Semaphore* TestThread::sem  = new Semaphore(0);
static volatile int threadCount = 5;
int main(int argc, char**argv){
	Timer::inic();
		userMain(argc, argv);
	/*TestThread* tt[5];
	int i = 0;
	for(i = 0; i < threadCount; i++){
		tt[i] = new TestThread();
		tt[i]->start();
	}

	for(i = 0; i < threadCount; i++){
		delete tt[i];
	}*/

	Timer::restore();
	return 0;
}
