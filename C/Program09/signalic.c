#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// new .h file to include
#include <signal.h>

// prototype for ctrl+c interupt handler
//void ctrlc();

int main(int args, char* argv[])
{

  // all variables that ctrlc needs access to

  void ctrlc()
  {
    printf("\n I will not be stopped! \n");
  }



  printf("\n");


  // structs to hold the old a new function pointers
  struct sigaction news;
  struct sigaction olds;

  // assign new handler function pointer
  news.sa_handler = ctrlc;
  news.sa_flags = 0;
  
  // used to pause / block process
  printf("\n Before install ...\n");
  int junk;
  printf("Enter first num : ");
  scanf("%d", &junk);

  // install our handler
  sigaction(SIGINT, &news, &olds);

  // used to pause / block process
  printf("\n After install ...\n");
  do {
    printf("Enter second num : ");
    scanf("%d", &junk);
  } while ( junk != -1 );

  // restore original handler
  sigaction(SIGINT, &olds, NULL);

  // used to pause / block process
  printf("\nAfter restore ...\n");
  printf("Enter third num : ");
  scanf("%d", &junk);

  printf("\n Natural term \n");

  return 0;
}


//definition for ctrl-c interupt handler
