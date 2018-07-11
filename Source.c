#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
const times=14000;
/*
void sort(unsigned long long*a,int l)
{
	int i,j;
	unsigned long long v;
	for(i=0;i<l-1;i++)
		for(l=i+1;j<l;j++)
		{
			if(a[i]>a[j])
			{
				v=a[i];
				a[i]=a[j];
				a[j]=v;
			}	
		}
}*/
/****************************************************************************************/
unsigned long long testfunc()
{
	struct timespec on;
	struct timespec off;
	void test(){};
	unsigned long long func_min=0;
	int i;
	for(i=0;i<times;i++)
	{
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&on);
		test();
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&off);
		func_min+=((off.tv_sec * 100000000)+off.tv_nsec)-((on.tv_sec*100000000)+on.tv_nsec);
	}
	/*sort(func_min,times);*/
	return (func_min/times);
}
/****************************************************************************************/
unsigned long long testsyscal()

{
	struct timespec sysc_on;
	struct timespec sysc_off;
	unsigned long long sys_min=0;
	int i;
	for(i=0;i<times;i++)
	{

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sysc_on);
		getpid();
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sysc_off);
		sys_min+=((sysc_off.tv_sec * 100000000)+sysc_off.tv_nsec)-((sysc_on.tv_sec*100000000)+sysc_on.tv_nsec);
	}
	/*sort(sys_min,times);*/

	return (sys_min/times);

}
/****************************************************************************************/
unsigned long long RW_sys()
{
	struct timespec rw_on;
	struct timespec rw_off;
	char x='x';
	unsigned long long rw_min=0; 
	pid_t pid;
	int i;
	for (i=0;i<times;i++)
	{
		int pipe_fd[2];
		if(pipe(pipe_fd)<0)
		{
		perror("pipe");
		}
		pid=fork();
		if(pid==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid==0)
		{
			close(pipe_fd[1]);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&rw_on);
			read(pipe_fd[0],&x,1);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&rw_off);
			close(pipe_fd[0]);
			exit(0);	
		}
		else
		{
			close(pipe_fd[0]);
			write(pipe_fd[1],&x,1);
			close(pipe_fd[1]);
		}
		rw_min+=((rw_off.tv_sec * 100000000)+rw_off.tv_nsec)-((rw_on.tv_sec*100000000)+rw_on.tv_nsec);
	}
	return (rw_min/times);
}
/****************************************************************************************/
void main(void)
{
	unsigned long long fortestfunc,forsyscal,forrw;
	fortestfunc=testfunc();
	printf("function call %llu\n\n",fortestfunc);
	forsyscal=testsyscal();
	printf("system call %llu\n\n",forsyscal);
	forrw=RW_sys();
	printf("read/write call %llu\n\n",forrw);
}


