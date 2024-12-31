#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <errno.h> 
//errono: example you could set it your self or not => ./pipe ls bogus => 2=no such file exists, 22=invalid arguments 
//many sys calls will set errno themselves if error occurs
//pipe before it returns, it will set errono so when you are back into the main program, it is set


int main(int argc, char *argv[])
{
	if (argc<2){
		errno=EINVAL; //EINVA=22
		perror("No arguments provided");
		exit(errno);
	}

	 // for the first child in the chain process: redirec 0, 1 of 
	//child to write/read end of the pipe by dup2(pipe_fd[1],1) 
	//then close(fd[0],fd[1]) only for the child also redirect 0 to 3 with dup2 and repeat  pipe fork
	//after the first child parent responsibility is to redirect stdin to read()/3
	//then fork
	//the last program is stdout
        
    pid_t parent=getpid();
	for (int i=1;i<argc-1;i++){

      if(getpid()==parent){
		
        int fd[2];
		if(pipe(fd)<0){
			exit(EXIT_FAILURE);
		}
		pid_t child=fork();
		if(child<0){
			exit(EXIT_FAILURE);
		}


		if(child==0){ //child
		dup2(fd[1],1);
		close(fd[0]);
		close(fd[1]);
		if(execlp(argv[i],argv[i],NULL)==-1){
			errno=2;
		    perror(argv[i]);
		    exit(errno);
		  }
		}

		//parent
          dup2(fd[0],0);
		  close(fd[0]);
		  close(fd[1]);
		  
		  
		  int stat;
          waitpid(child, &stat, 0);
	      if (WEXITSTATUS(stat)!=0){
	         exit(WEXITSTATUS(stat));
	       }

	   }
	}

	pid_t child=fork();
	if (child==0){
	if(execlp(argv[argc-1],argv[argc-1],NULL)==-1){
		errno=2;
		perror(argv[argc-1]);
		exit(errno);
	}
	}
	
   int stat;
    
    waitpid(child, &stat, 0);
	if (WEXITSTATUS(stat)!=0){
	   exit(WEXITSTATUS(stat));
	}

}

//waitpid(pid, status, options)
//if a child encounters error and child exits early, but the parent will not: 
//WIFEXITED(status)=>true if child terminated normally 
//if error has occured WEXITSTAUS(stauts) non 0 if an error => more powerful. WIFEXITED is either true or false.
// But WEXISTSTATUS(status) tells you about the nature of the error
//echo $?==22
//int * stat;
//if WEXISTAUTUS(stat)!=0:
//exit(WXITSTAUTS(stat)) => this exits out of the parent process



//For n commands -> n-1 iterations bc we are handling programs in pairs
//when yoarrive at the last process, no need to create a new pipe 