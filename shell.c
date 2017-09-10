#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(){
    int cmd_num = 0, i = 0;
    char *cmd;
    char *tmp_cmd;
    size_t buffersize = 20;
    size_t length = 0;

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
        
        /*
        printf("%zu characters were read.\n",length);
        printf("You typed: '%s'\n",cmd);
        */

        if(!strcmp(cmd, "exit")){
        	printf("exit!\n");
        	return 0;
        }

        tmp_cmd = (char *)malloc((length+2) * sizeof(char));
        strcpy(tmp_cmd, "./");
        strcat(tmp_cmd, cmd);
        		
        if(fork() == 0){
        	char *args[]={tmp_cmd,NULL};
       		execvp(args[0],args);
        	return 0;
        }
        	        
        free(cmd);
        cmd_num ++;
    }
}
