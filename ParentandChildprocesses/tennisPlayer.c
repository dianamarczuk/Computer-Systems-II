//Diana Marczuk

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<wait.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/types.h>

const char*	playerNameCPtr	= "";

pid_t		opponentPid	= -1;

int		isServer	= 0;

int		shouldPlay	= 1;

int		isMyTurn	= 0;

void		swing		()
{
  sleep(1);
  isMyTurn		= 0;

  if  ((rand() % 4) == 0)
  {
    //  II.D.1.  YOUR CODE to send SIGUSR2 to opponentPid and
    //            to print a message of disappointment
    isMyTurn	= isServer;
    kill(opponentPid, SIGUSR2);
    printf("%s:     Gosh darn it!\n", playerNameCPtr);
    
  }
  else
  {
    //  II.D.2.  YOUR CODE to send SIGUSR1 to opponentPid and
    //           to print the sound of the racket hitting the ball
    printf("%s:     (Pong)!!! \n", playerNameCPtr);
    kill(opponentPid, SIGUSR1);
  }

}


void		sigTermHandler	(int		sigNum
				)
{
  //  II.E.  YOUR CODE to set shouldPlay to 0
  shouldPlay = 0;
}


void		sigUsr1Handler	(int		sigNum,
				 siginfo_t*	infoPtr,
				 void*		dataPtr
				)
{
  //  II.F.  YOUR CODE to set isMyTurn and opponentPid
  isMyTurn = 1;
  opponentPid = infoPtr -> si_pid;
}


void		sigUsr2Handler	(int		sigNum,
				 siginfo_t*	infoPtr,
				 void*		dataPtr
				)
{
  //  II.G.  YOUR CODE to set isMyTurn to isServer,
  //         and to send SIGUSR1 to the parent process.
  isMyTurn = isServer;
  kill(getppid(), SIGUSR1);

}


int		main		(int		argc,
				 char*		argv[]
				)
{
  //  II.C.1.  Reset the random number generator:
  srand(getpid());

  //  II.C.2.  YOUR CODE to set up either as the server or receiver player:
  // "program_name arg1" ---> argc of 2, argv
  if (argc > 1)
  {
  	opponentPid = atoi(argv[1]);
	playerNameCPtr = "server";
	isServer = 1;
	isMyTurn = 1;
  }
  else
  {
  	playerNameCPtr = "receiver";
  }

  //  II.C.3.  YOUR CODE to install the signal handlers
  struct sigaction	act;
 
  memset(&act, '\0', sizeof(act));
  sigemptyset(&act.sa_mask);
  act.sa_handler = sigTermHandler;
  sigaction(SIGTERM, &act, NULL);

  memset(&act, '\0', sizeof(act));
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_SIGINFO | SA_RESTART;
  act.sa_sigaction = sigUsr1Handler;
  sigaction(SIGUSR1, &act, NULL);

  memset(&act, '\0', sizeof(act));
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_SIGINFO | SA_RESTART;
  act.sa_sigaction = sigUsr2Handler;
  sigaction(SIGUSR2, &act, NULL);

  //  II.C.4.  Play the game:
  while  (shouldPlay)
  {
    while  (!isMyTurn)
      sleep(1);

    swing();
  }

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
