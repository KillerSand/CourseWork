#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// new for sockets
#include <sys/socket.h>

int my_pipe(int fd[2]);

void main()
{
  int fd[2];

  int stat = my_pipe(fd);
}


int my_pipe(int fd[2])
{
  // create socket s0
  int s0 = socket(AF_UNIX, SOCK_STREAM, 0);
  if ( s0 == -1 )
    return -1;
  // create name0 + namelen0
  struct sockaddr name0 = {AF_UNIX, "Barny"};
  socklen_t namelen0 = sizeof(struct sockaddr) + 6;
  // bind socket s0 to name0
  bind(s0, &name0, namelen0);


  // create socket s1
  int s1 = socket(AF_UNIX, SOCK_STREAM, 0);
  if ( s1 == -1 )
    return -1;
  // create name1 + namelen1
  struct sockaddr name1 = {AF_UNIX, "Fred"};
  socklen_t namelen1 = sizeof(struct sockaddr) + 5;  
  // bind socket s1 to name1
  bind(s1, &name1, namelen1);


  // s1 listens for connection requests
  if ( listen(s1, 1) == -1  )
  {
    printf("listen() fail\n");
    return -1;
  }
  printf("%s (%d) is listening for connection requests\n\n",
         name1.sa_data, namelen1);


  // s0 requests connection to s1
  if ( connect(s0, &name1, namelen1) == -1 )
  {
    printf("connect() fail\n");
    return -1;
  }
  printf("%s (%d) is requesting a connection",
         name0.sa_data, namelen0);
  printf(" to %s (%d)\n\n",
         name1.sa_data, namelen1); 


  // create name1_0 + namelen1_0 to be used for accept
  struct sockaddr name1_0 = {AF_UNIX, "?????"};
  socklen_t namelen1_0 = sizeof(struct sockaddr) + 6;
  
// copy s1's properties into name1_0 + namelen1_0
  getsockname(s1, &name1_0, &namelen1_0);
  // verify
  printf("Socket name1_0 = %s (%d)\n\n",
         name1_0.sa_data, namelen1_0);
 
  // s1 accepts connection request from s0
  // s2 is a new socket with s1's properties connected to s0
  // s1 no longer needed - unless in a listener loop
  int s2;
  if ( (s2 = accept(s1, &name1_0, &namelen1_0)) == -1  )
  {
    printf("accept() fail\n");
    return -1;
  }
  else
  {
    printf("%s (%d) has accepted a connection request",
         name1.sa_data, namelen1);
    printf(" from %s (%d)\n\n",
           name1_0.sa_data, namelen1_0); 

  // verify
  printf("Socket name1_0 = %s (%d)\n\n",
         name1_0.sa_data, namelen1_0);

    close(s1); // unless in a listener loop
  }


  // assign socket pair in fd
  fd[0] = s0; // for reads
  fd[1] = s2; // for writes


  // remove names for re-use
//  unlink("Barny");
//  unlink("Fred");

  return 0;
}
