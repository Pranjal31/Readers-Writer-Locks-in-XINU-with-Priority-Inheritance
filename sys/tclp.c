
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

#define DEFAULT_LOCK_PRIO 20

#define assert(x,error) if(!(x)){ \
            kprintf(error);\
            return;\
            }
/*int mystrncmp(char* des,char* target,int n){
    int i;
    for (i=0;i<n;i++){
        if (target[i] == '.') continue;
        if (des[i] != target[i]) return 1;
    }
    return 0;
}*/

char outputl[10];
int countl;
void readerl (char msg, int lck, int wprio)
{
        int     ret;

        kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, READ, wprio);
        outputl[countl++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
        sleep (3);
        outputl[countl++]=msg;
        kprintf ("  %c: to release lock\n", msg);
	releaseall (1, lck);
}

void writerl (char msg, int lck, int wprio)
{
	kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, WRITE, wprio);
        outputl[countl++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
        sleep (3);
        outputl[countl++]=msg;
        kprintf ("  %c: to release lock\n", msg);
        releaseall (1, lck);
}

void tclp()
{
        countl = 0;
        int     lck;
        int     rd1, rd2, rd3, rd4;
        int     wr1,wr2;

        kprintf("\nTest 2: wait on locks with priority. Expected order of"
		" lock acquisition is: reader A, reader B, writer C, reader D & reader E\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 2 failed");

	rd1 = create(readerl, 2000, 20, "readerl", 3, 'A', lck, 50);
	rd2 = create(readerl, 2000, 20, "readerl", 3, 'B', lck, 45);
	rd3 = create(readerl, 2000, 20, "readerl", 3, 'D', lck, 20);
	//rd4 = create(readerl, 2000, 20, "readerl", 3, 'E', lck, 30);
        wr1 = create(writerl, 2000, 20, "writerl", 3, 'C', lck, 30);
        wr2 = create(writerl, 2000, 20, "writerl", 3, 'E', lck, 40);

	
        kprintf("-start wr C, then sleep 1s. lock granted to wr A\n");
        resume(wr1);
        sleep (1);

        kprintf("-start wr E, then sleep 1s. writer waits for the lock\n");
        resume(wr2);
        sleep10 (1);


        kprintf("-start reader A, B, D, all wait \n");
        resume (rd1);
	resume (rd2);
	resume (rd3);


        sleep (15);
        kprintf("output=%s\n", outputl);
        assert(mystrncmp(outputl,"ABABCCDEED",10)==0,"Test 2 FAILED\n");
        kprintf ("Test 2 OK\n");
}
