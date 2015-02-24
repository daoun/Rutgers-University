#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "mypthread.h"


int current = 0; 
static ucontext_t *context;
mypthread_t mythread[SIZE];
static int start = 0;

//stack = char * (of 4096)

//malloc context
//state = ready
//first time create = initialize 0, 1

//created thread status = 1
//thread state = 1 means ready
//thread state = 0 means block

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, 
void *(*start_routine) (void *), void *arg)
{
	
	if(start == 0)
	{
		start = -1;
		mypthread_create(thread, attr, start_routine, arg);
	}
	
	while(mythread[current].status != 0) //looks for the next non-used thread
	{
		current = (current + 1) % SIZE;
	}
	
	thread->context = malloc(sizeof(ucontext_t));
	thread->context->uc_stack.ss_sp = malloc(sizeof(char) * 4096);
	thread->context->uc_stack.ss_size = sizeof(char) * 4096;
	getcontext(thread->context);
	thread->context->uc_link = thread->context;
	
	makecontext(thread->context, (void (*)())start_routine, sizeof(void *)/sizeof(int), arg);
	
	thread->tid = current;
	thread->status = 1;
	thread->state = 1;
	thread->arg = arg;
	thread->wqcur = 0;
	thread->wqsize = SIZE;
	
	
	mythread[current] = *thread;
	
	pthread_attr_init(attr);
	
	
	return 0;
}
void mypthread_exit(void *retval)
{
	mythread[current].status = 0; 
	free(&mythread[current].context);
	
	while(mythread[current].status != 1) //looks for the next runnable thread
	{
		current = (current + 1) % SIZE;
	}
	
	setcontext(mythread[current].context);
	//setcontext(set the next oepn thread
	
}

int mypthread_yield(void)
{
	
	//pick next runnable thread
	//save current context
	//switch to new context
	
	int oldcurrent = current;
	while(mythread[current].status != 1) //looks for the next runnable thread
	{
		current = (current + 1) % SIZE;
	}
	swapcontext(mythread[oldcurrent].context, mythread[current].context);
	
	return 0;
}

int mypthread_join(mypthread_t thread, void **retval)
{
	//change state = block
	//add main to w.q. of current
	
	//swap(main context, thread context)
	
	thread.state = 0;
	mythread[current].wq[mythread[current].wqcur] = thread.tid;
	swapcontext(mythread[current].context, thread.context);
	
	
	return 0; 
}


/*
//http://www.mathcs.emory.edu/~cheung/Courses/561/Syllabus/91-pthreads/create-threads.html	
	*/
