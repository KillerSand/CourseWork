#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
// new for sockets
#include <sys/socket.h>

typedef struct card{
	int value;
	char suit;
}card;

int main()
{
  int stuck = 0;
  srand(time(0));
  void ctrlc()
  {
    stuck = 1;
  }
  struct sigaction news;
  struct sigaction olds;
  news.sa_handler = ctrlc;
  news.sa_flags = 0;
  sigaction(SIGINT, &news, &olds);
  int ran=0,ran2 =51;
  int fd[2];
  card* deck = (card*) malloc(sizeof(card) * 52);
  int i;
  for(i=0;i < 13; ++i){
    deck[i].value = i+1;
    deck[i].suit = 'H';
	}
	for(i=0;i < 13; ++i){
    deck[i+13].value = i+1;
    deck[i+13].suit = 'C';
	}
	for(i=0;i < 13; ++i){
    deck[i+26].value = i+1;
    deck[i+26].suit = 'D';
	}
	for(i=0;i < 13; ++i){
    deck[i+39].value = i+1;
   deck[i+39].suit = 'S';
	}
	for(i=0;i < 51;++i){
		int rans = i + (rand() % (52 -i));
		card* temp = malloc(sizeof(card));
		temp -> suit = deck[rans].suit;
		temp -> value = deck[rans].value;
		deck[rans].value = deck[i].value;
		deck[rans].suit = deck[i].suit;
		deck[i].value = temp -> value;
		deck[i].suit = temp -> suit;
		free(temp);
	}
	
  int s1 = socket(AF_UNIX, SOCK_STREAM, 0);
  if ( s1 == -1 )
	  return -1;
  // create name1 + namelen1
  struct sockaddr name1 = {AF_UNIX, "server"};
  socklen_t namelen1 = sizeof(struct sockaddr);  
  // bind socket s1 to name1
  bind(s1, &name1, namelen1);

  // s1 listens for connection requests
  while(!stuck){
  if ( listen(s1, 4) == -1  )
  {
    printf("listen() fail\n");
    return -1;
  }
  // s2 is a new socket with s1's properties connected to s0
  int s2;
  
  s2 = accept(s1,NULL,NULL);
  if(s2 != -1){
	  pid_t pid = fork();
	  
	  if(pid == 0){
		for(i=0;i < 100;++i){
			++ran;
			if(ran >= 51)
				ran = 0;
			if(ran2 <= 0)
				ran2 = 51;
			card* temp = malloc(sizeof(card));
			temp -> suit = deck[ran].suit;
			temp -> value = deck[ran].value;
			deck[ran].value = deck[ran2].value;
			deck[ran].suit = deck[ran2].suit;
			deck[ran2].value = temp -> value;
			deck[ran2].suit = temp -> suit;
			free(temp);
			--ran;
		}
		close(fd[0]);
		// assign socket pair in fd
		fd[1] = s2; // for writes
		write(s2, deck, (sizeof(card) * 52));
	  }
	  if(pid > 0){
		  close(s2);
	  }
  }
  }
  close(s1); // unless in a listener loop
  free(deck);
  unlink("server");
  sigaction(SIGINT, &olds, &news);
  printf("\nClosing Server\n");
  free(deck);
  return 0;
}