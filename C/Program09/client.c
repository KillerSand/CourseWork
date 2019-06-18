#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct card{
	int value;
	char suit;
}card;

int main(){
	char* input = (char*) malloc(sizeof(char));
	card* hand = (card*) malloc(sizeof(card) * 4);
	int bet = 0;
	int bank = 100;
	int betting = 0;
	int i;
	int count = 0;
	int won=0,played=0,loss=0;
	card* deck = (card*) malloc(sizeof(card) * 52);
	int s0 = socket(AF_UNIX, SOCK_STREAM, 0);
	if ( s0 == -1 )
		return -1;
	struct sockaddr test = {AF_UNIX, "server"};
	socklen_t testlen = sizeof(struct sockaddr); 
	connect(s0,&test,testlen);
	read(s0,deck,(sizeof(card) * 52));
	close(s0);
	printf("\nLet the game Begin\n");
	while(!betting){
		printf("\nYou have %i chips to bet\n",bank);
		printf("\nDo you wish to cash out or place a bet?(c:b): ");
		scanf("%s",input);
		if(strcmp("c",input) == 0){
			printf("\nYou cashed out with %i chips!\n",bank);
			printf("\nYou played %i times with %i wins and %i losses\n",played,won,loss);
			exit(0);
		}
		else if(strcmp("b", input) != 0){
			printf("\nYou did not enter a proper input\n");
		}
		else{
			printf("\nHow many chips do you wish to bet? ");
			scanf("%i",&bet);
			betting = 1;
			if(bet > bank){
				printf("\nYou do not have enough chips to place that bet\n");
			}
			++played;
			for(i=0;i < 4; ++i){
				if(count >= 51){
					printf("\nDeck ran out of cards\nFetching new deck from server!\n");
					count = 0;
					free(deck);
					deck = (card*) malloc(sizeof(card) * 52);
					connect(s0,&test,testlen);
					read(s0,deck,(sizeof(card) * 52));
					printf("\nNew deck has arrived!\n");
				}
				if(deck[count].value > 1 && deck[count].value < 13){
					hand[i].value = deck[count].value;
					hand[i].suit = deck[count].suit;
				}
				++count;					
				
			}
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					if (hand[j].value < hand[i].value){
						card* tmp = (card*) malloc(sizeof(card));
						tmp -> value = hand[i].value;
						tmp -> suit = hand[i].suit;
						hand[i].value = hand[j].value;
						hand[i].suit = hand[j].suit;
						hand[j].value = tmp -> value;
						hand[j].suit = tmp -> suit;
					}  
				}
			}			
			for(i=0;i < 4; ++i){
				if(hand[i].value == 1 || hand[i].value > 10){
					if(hand[i].value == 1)
						printf("\n%c : A\n",hand[i].suit);
					else if(hand[i].value == 11)
						printf("\n%c : J\n",hand[i].suit);
					else if(hand[i].value == 12)
						printf("\n%c : Q\n",hand[i].suit);
					else
						printf("\n%c : K\n",hand[i].suit);
				}
				else
					printf("\n%c : %i\n",hand[i].suit,hand[i].value);
			}
			if((hand[0].value == hand[1].value) && (hand[1].value == hand[2].value) &&
			(hand[2].value == hand[3].value)){
				printf("\nYou got a Four of a Kind, you win %i Chips\n",bet * 20825 + bet);
				bank += 20825 * bet;
				++won;
			}
			else if((hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit)
			&& (hand[0].value + 1 == hand[1].value) && (hand[1].value + 1 == hand[2].value) && (hand[2].value + 1 == hand[3].value)){
				printf("\nYou got a Straight Flush, you win %i Chips\n",bet * 6153 + bet);
				bank += 6153 * bet;
				++won;
			}
			else if(((hand[0].value == hand[1].value) && (hand[1].value == hand[2].value)) ||
			((hand[0].value == hand[2].value) && (hand[2].value == hand[3].value)) ||
			((hand[1].value == hand[2].value) && (hand[2].value == hand[3].value)) || 
			((hand[1].value == hand[2].value) && (hand[2].value == hand[3].value))){
				printf("\nYou got a Three of a Kind, you win %i Chips\n",bet * 108 + bet);
				bank += 108 * bet;
				++won;
			}
			else if(((hand[0].value == hand[1].value) && (hand[2].value == hand[3].value)) || 
			((hand[0].value == hand[2].value) && (hand[1].value == hand[3].value)) ||
			((hand[0].value == hand[3].value) && (hand[1].value == hand[2].value))){
				printf("\nYou got Two Pairs, you win %i Chips\n",bet * 96 + bet);
				bank += 96 * bet;
				++won;
			}
			else if((hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit)){
				printf("\nYou got a Flush, you win %i Chips\n",bet * 96 + bet);
				bank += 96 * bet;
				++won;
			}
			else if((hand[0].value == hand[1].value) || (hand[0].value == hand[2].value) || (hand[0].value == hand[3].value) ||
			(hand[1].value == hand[2].value) || (hand[1].value == hand[3].value) || (hand[2].value == hand[3].value)){
				printf("\nYou got a Pair, you win %i Chips\n",bet * 3 + bet);
				bank += 3 * bet;
				++won;
			}
			else{
				printf("\nYou did not win anything and lost your bet!\n");
				bank = bank - bet;
				++loss;
			}
			if(bank <= 0){
				betting= 1;
				printf("\nYou ran out of chips!\n");
			}
			else
				betting = 0;
		}
	}
			
}