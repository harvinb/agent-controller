#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>

char *x;
char *n;
char *w_path;
char *mechanism;

void printargs(){
	printf("w_path:%s\n",w_path);
}

void sig_handler(int signo)
{
  if (signo == SIGALRM)
    printf("received SIGALRM\n");
}

int main(int argc, char* argv[], char* envp[]){

	struct option long_options[]=
	{
		{"worker_path",		required_argument,	NULL,	'p'},
		{0,0,0,0}
	};

	int opt;

	while((opt=getopt_long(argc,argv,"p:",long_options,NULL))!=-1){
		switch(opt){
			case 'p':
				w_path=optarg;
				break;
			default:
				break;
		}
	}

	printargs();
	printf("envp: %s\n",(*envp));

	const char* s = getenv("HSA_TOOLS_LIB");
	printf("HSA_TOOLS_LIB :%s\n",(s!=NULL)? s : "getenv returned NULL");

	int i=0;

	struct childid {
		int id;
		int pipeid[2];
	};

	struct childid *cid=malloc(sizeof(struct childid));

	if(signal(SIGALRM, sig_handler)==SIG_ERR)
    	printf("Signal catch fail\n");

    printargs();


	cid->id=fork();

	//Fork success
	if(cid->id >= 0){ 
		//Child process
		if(cid->id == 0){
			char *newargv[]={w_path,NULL};
			char *newenviron[] = { NULL };


			execve(w_path,newargv,envp);
			perror("execve");
			exit(EXIT_FAILURE);
		} 
	} else {
		printf("Fork failed at child %d\n",i);
	}

	wait(NULL);

}