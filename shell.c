#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(){
    int cmd_num = 0, i = 0;
    char *cmd[100];
    char *tmp_cmd;
    size_t buffersize = 20;
    size_t length = 0;
    int his_num = 0;

    while(cmd_num < 200){
        
        
        printf("$");
        
        /*read a line from stdin*/
        cmd[cmd_num] = (char *)malloc(buffersize * sizeof(char));
        if(cmd[cmd_num] == NULL){
            perror("buffer allocation error!\n");
            exit(1);
        }
        length = getline(&cmd[cmd_num], &buffersize, stdin);
        cmd[cmd_num][--length] = '\0';

        /*build-in command exit*/
		if(!strcmp(cmd[cmd_num], "exit")){
		    printf("exit simple shell!\n");
		    return 0;
		}

		/*build-in command cd*/
        if(!strncmp(cmd[cmd_num], "cd ", 3)){
        	for(i = 3; i < length; i++)
            	if(cmd[cmd_num][i] == 32){
                	printf("error: Directory cannot include space. Please re-enter.\n");
                	break;
            	}
        	if(i < length){
        		cmd_num ++;
        		continue;
        	}
        	tmp_cmd = (char *)malloc((length) * sizeof(char));
        	strcpy(tmp_cmd, (cmd[cmd_num]));
        	for(i = 0; i < 3; i++)
        		tmp_cmd++;

        	if(chdir(tmp_cmd) == -1)
        		printf("Failed to chang Directory\n");
        	else{
        		printf("Working Directory changed to %s\n", tmp_cmd);
        	//	system("ls");
        	}
        	cmd_num++;
        	continue;
        }
		
        /*build-in command history*/
		if(!strncmp(cmd[cmd_num], "history", 7)){
			if(length == 7)
				for(i = 0; i <= cmd_num; i++)
					printf("%d %s\n", i, cmd[i]);
			else if(!strncmp(cmd[cmd_num], "history ", 8)){
				tmp_cmd = (char *)malloc((length - 8) * sizeof(char));
				strcpy(tmp_cmd, (cmd[cmd_num] + 8));
				if(length == 9){
					i = (int) tmp_cmd[0] - 48;
					if(i > cmd_num)
						printf("there is less than %d records in command history\n", i);
					else printf("%d %s\n", i, cmd[i]);
				}
				if(length == 10){
					i = 10 * (((int) tmp_cmd[0]) - 48) + ((int) tmp_cmd[1]) - 48;
					if(i > cmd_num)
						printf("there is less than %d records in command history\n", i);
					else printf("%d %s\n", i, cmd[i]);
				}
				if(length > 10)
					printf("error: the maximum of history retrieving is 99!\n");
			}
			cmd_num++;
			continue;
		}

		/*check for space*/
        for(i = 0; i < length; i++)
            if(cmd[cmd_num][i] == 32){
                printf("error: Command cannot include space. Please re-enter.\n");
                break;
            }
        if(i < length){
        	cmd_num++;
        	continue;
        }
        

        /*run executable command*/
        pid_t id = fork();

        if(id > 0){
        	//printf("pid: %d executed!\n", id);
        	wait(&id);
        }
        else if(id == 0){
        	char *args[]={cmd[cmd_num],NULL};
       		execvp(args[0],args);
        	return 0;
        }
        else
        	printf("process execution error!\n");
        	        
        cmd_num ++;
    }
}
