#include <stdio.h>
#include <stdlib.h>

int main(){

	char *cmd;
	size_t buffersize = 20;
	size_t length = 0;
    /*
	cmd = (char *)malloc(buffersize * sizeof(char)); 
	if(cmd = null){
		perror("buffer allocation error!\n");
		exit();
	}
	*/

	while(1){
		cmd = (char *)malloc(buffersize * sizeof(char)); 
		if(cmd = NULL){
			perror("buffer allocation error!\n");
			exit(1);
		}

		printf("$");
		
		length = getline(&cmd, &buffersize, stdin);
		cmd[--length] = '\0';

//		printf("%zu characters were read.\n",length);
// 		printf("You typed: '%s'\n",cmd);

		free(cmd);
	}
}