#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int args, char* argv[])
{
  printf("\n");
  // buffer used for pipe reads and writes
  char buffer[BUFFER_SIZE];

  // create an array of two file descriptors
  // for use with pipe() [0] - read, [1] - write
  int pipe_fds[2];

  // create a pipe - must be done before fork()
  // so that the read and write descriptors are
  // avialable to both parent and child
  int psts; // holds result of pipe creation attempt
  psts = pipe(pipe_fds);

  if ( psts == 0 ) // pipe sucess
  {
    pid_t pid = fork(); // create child process

    if ( pid > 0 ) // parent process
    {
      printf("I am the parent-reciever\n");
      close(pipe_fds[1]); // will not send

      // wait - read message from child when it arives
      int br = read(pipe_fds[0], buffer, BUFFER_SIZE);
      printf("%d bytes read, message = %s\n", br, buffer);

/*
// both child writes happened before first parent read
printf("%d bytes read, message = %s\n", br, buffer+strlen(buffer)+1);
*/                                               

//OR


// now try to do it again
br = read(pipe_fds[0], buffer, BUFFER_SIZE);
printf("%d bytes read, message = %s\n", br, buffer);


     // close(pipe_fds[0]); // done listening to child
      printf("Parent exiting ...\n");

    }
    else if ( pid == 0 ) // child process
    {
      printf("I am the child-sender\n");
      close(pipe_fds[0]); // will not recieve

      // create the message "Im alive and doing fine!"
      strcpy(buffer, "Im alive and doing fine!");
      // send the message back to parent
      write(pipe_fds[1], buffer, strlen(buffer) + 1);

// to try to force the second write to happen "after" the first read
sleep(1);
strcpy(buffer, "That's not what I wanted!!");
write(pipe_fds[1], buffer, strlen(buffer) + 1);

      close(pipe_fds[1]); // done talking back to parent
      printf("Child exiting ...\n");
    }
    else // ( pid == -1 ) -> fork failed
    {
      printf("Fork Error!\n");
      exit(2);
    }
  }
  else // ( psts == -1 ) -> pipe failed
  {
      printf("Pipe Error!\n");
      exit(1);
  }

  printf("\n");
  return 0;
}
