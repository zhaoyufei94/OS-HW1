#include <stdio.h>
#include <stdlib.h>

int main(){
	int cmd_num = 0, i = 0;
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

	while(cmd_num < 10){
		cmd = (char *)malloc(buffersize * sizeof(char)); 
		if(cmd == NULL){
			perror("buffer allocation error!\n");
			exit(1);
		}

		printf("$");
		
		length = getline(&cmd, &buffersize, stdin);
		cmd[--length] = '\0';
		for(i = 0; i < length; i++)
			if(cmd[i] == 32){
				printf("error: Command cannot include space. Please re-enter.\n");
				break;
			}
		if(i < length) continue;

//		printf("%zu characters were read.\n",length);
// 		printf("You typed: '%s'\n",cmd);

		free(cmd);
		cmd_num ++;
	}
}
