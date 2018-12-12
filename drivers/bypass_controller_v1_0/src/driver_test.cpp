#include "DSC_controller.h"
#include "CSRMap.h"
#include <sys/time.h>
#include <errno.h>

static int timespec_check(struct timespec *t)
{
    if ((t->tv_nsec < 0) || (t->tv_nsec >= 1000000000))
        return -1;
    return 0;
}

void timespec_sub(struct timespec *t1, struct timespec *t2)
{
	if (timespec_check(t1) < 0) {
		fprintf(stderr, "invalid time #1: %lld.%.9ld.\n",
			(long long)t1->tv_sec, t1->tv_nsec);
		return;
	}
	if (timespec_check(t2) < 0) {
		fprintf(stderr, "invalid time #2: %lld.%.9ld.\n",
			(long long)t2->tv_sec, t2->tv_nsec);
		return;
	}
	t1->tv_sec -= t2->tv_sec;
	t1->tv_nsec -= t2->tv_nsec;
	if (t1->tv_nsec >= 1000000000) {
		t1->tv_sec++;
		t1->tv_nsec -= 1000000000;
	} else if (t1->tv_nsec < 0) {
		t1->tv_sec--;
		t1->tv_nsec += 1000000000;
	}
}

#define RESERVED_MEM_OFFSET 0x36b000000
#define BRAM_OFFSET 0x0
#define H2C_DSC_CTRL 0x3000
#define C2H_DSC_CTRL 0x4000



int main ()
{

    DSCcontroller *ptr = new DSCcontroller (BRAM_OFFSET, false, true, false, 
            RESERVED_MEM_OFFSET, RESERVED_MEM_OFFSET + 0x100000, 0x2000, 0x2 );
    int i = 100;
    long int total = 0;
    while (i > 0)
    {
    	struct timespec  tv1, tv2;
    	clock_gettime(CLOCK_MONOTONIC, &tv1);

    	ptr->IssueCommand();

    	clock_gettime(CLOCK_MONOTONIC, &tv2);
    	timespec_sub(&tv2, &tv1);
	total += tv2.tv_nsec;
    	//printf("#CLOCK_MONOTONIC %ld nsec.\n", tv2.tv_nsec);
	i--;
    }
    printf("\n#Total time %ld nsec.\n\n", total);
    delete ptr;


    // setup data that is to be sent out.
    tcCSRMap* mMap = new tcCSRMap ("/dev/mem", 1*1024*1024*1024, RESERVED_MEM_OFFSET);
    for (i = 0; i < 0x2000*2; i++)
    	mMap->WriteCtrlRegWord(i, i);

    // initialize controllers
    DSCcontroller *ptr_h2c = new DSCcontroller (H2C_DSC_CTRL, false, true, false, 
            RESERVED_MEM_OFFSET, 0x0, 0x2000, 0x2 );
    DSCcontroller *ptr_c2h = new DSCcontroller (C2H_DSC_CTRL, true, false, false, 
            0x0, RESERVED_MEM_OFFSET + 0x100000, 0x2000, 0x2 );
    
    // issue trigger
//    ptr_c2h->IssueCommand();
//    ptr_h2c->IssueCommand();
    
    // check write back
    for (i = 0x200; i < 0x300; i++)
        printf("%d\n", mMap->ReadCtrlRegWord(i + 0x100000));//rm[i]);

}
