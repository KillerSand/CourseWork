#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>



int main(int args, char* argv[])
{
	char str[256];
	char mystring[256];
	int pos = 0;
	char **token = malloc(sizeof(char));
	char **multiToken = malloc(sizeof(char));
	char* path = ".";
	DIR* dirp;
	while(1){
		pid_t pid = 0;
		int cmds = 0;
		dirp = opendir(path);
		if(!dirp){
			printf("\nThere was an error opening %s",path);
			exit(1);
		}
		struct dirent* dentry = readdir(dirp);
		printf("\n");
		printf("%s ? ",getcwd(dentry -> d_name,sizeof(dentry->d_name)));
		fgets(str,sizeof(str),stdin);
		char* tok = strtok(str, " ");
		while(tok != NULL){
			token[pos] = tok;
			tok = strtok(NULL, " ");
			++pos;
			++cmds;
		}
		token[pos] = NULL;
		pos = 0;
		for(int j = 0; j < cmds; ++j){
			for(int i = 0; i < strlen(token[pos+j]); ++i){
					if(token[pos+j][i] == '\n')
						token[pos+j][i] = '\0';
				}
			}
		if(strcmp(token[pos],"exit") == 0){
			printf("\nNow Exiting.....\n");
			exit(1);
		}
		
		if(strcmp(token[pos],"cd") == 0){
			char *upath = (char*) malloc(strlen(token[pos+1]) + strlen(dentry->d_name) + 2);
			for(int i = 0; i < strlen(token[pos+1]); ++i){
				if(token[pos+1][i] == '\n')
					token[pos+1][i] = '\0';
			}
			strcpy(upath,dentry ->d_name);
			strcat(upath,"/");
			strcat(upath,token[pos+1]);
			if((chdir(token[pos+1]))){
				printf("\nCould not change directory to %s\n",token[pos+1]);
			}
			else{
				path = (char*) malloc(strlen(token[pos+1]) + strlen(dentry->d_name) + 2);
				strcpy(path,upath);
			}
			closedir(dirp);
		}
		else{
			if(strcmp(token[pos],"multi") == 0){
				FILE* fin;
				cmds = 0;
				fin = fopen(token[pos+1],"r");
				if (!fin)
				{
					printf("\nCan not open %s\n",token[pos+1]);
					exit(1);
				}
				
				while(fgets(mystring,sizeof(mystring),fin)){
					char* multiTok = strtok(mystring, " ");
					while(multiTok != NULL){
						multiToken[pos] = multiTok;
						multiTok = strtok(NULL, " ");
						++pos;
						++cmds;
						if(multiTok == NULL){
							multiToken[pos] = NULL;
						    pos = 0;
    					    for(int j = 0; j < cmds; ++j){
						        for(int i = 0; i < strlen(multiToken[pos+j]); ++i){
							        if(multiToken[pos+j][i] == '\n')
								        multiToken[pos+j][i] = '\0';
							        }
						        }
						    }
					    }
					cmds = 0;
					pos = 0;
					pid = fork();
					if ( pid == 0 ) // child
					{
						execvp(multiToken[pos], multiToken);
						exit(0);
					}
					else if ( pid > 0 ) // parent
					{
						wait(NULL);
					}
					else // pid < 0 -> fork error
					{
						printf("Bad fork()");
						exit(1);
					}
				}
			}			
			else{
				pid = fork();
				if ( pid == 0 ) // child
				{
					execvp(token[pos], token);
					exit(0);
				}
				else if ( pid > 0 ) // parent
				{
					wait(NULL);
				}
				else // pid < 0 -> fork error
				{
					printf("Bad fork()");
					exit(1);
				}
			}
		}	
	}
}
