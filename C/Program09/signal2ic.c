#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// new .h file to include
#include <signal.h>


// prototype for ctrl+c interupt handler
void ctrlc();


int main(int args, char* argv[])
{
  int var = 1;


// nesting allows access to local variables
//definition for ctrl-c interupt handler
void ctrlc()
{
  printf("\nI will not be stopped! %d\n", var);
  ++var;
}

  printf("\n");

  // structs to hold the old a new function pointers
  struct sigaction news;
  struct sigaction olds;

  // assign new handler function pointer
  news.sa_handler = ctrlc;
  news.sa_flags = 0;

  // used to pause / block process
  int junk;
  printf("Enter first num : ");
  scanf("%d", &junk);

  // install our handler
  sigaction( SIGINT, &news, &olds);

  // used to pause / block process
  do {
    printf("Enter second num : ");
    scanf("%d", &junk);
  } while ( junk != -1 );

  // restore original handler
  sigaction( SIGINT, &olds, NULL);


  printf("\nvar in main = %d\n", var++);

  // used to pause / block process
  printf("Enter third num : ");
  scanf("%d", &junk);

  printf("Natural term \n");

  return 0;
}



