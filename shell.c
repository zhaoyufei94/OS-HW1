#define HISTORY 100
#define MAX_NUM 300

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
    int cmd_num = 0, cmd_n = 0, i = 0, j = 0, k = 0;
    char *cmd[HISTORY];
    char *tmp_cmd;
    char *path;
    size_t buffersize = 20;
    size_t length = 0;
    int his_ful = 0;

    while(cmd_n < MAX_NUM){
        
        if(cmd_num == HISTORY){
        	his_ful = 1;
        	cmd_num = 0;
        }

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
		    //printf("exit simple shell!\n");
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
        	//else{
        	//	printf("Working Directory changed to %s\n", tmp_cmd);
        	//	system("ls");
        	//}
        	cmd_num++;
        	cmd_n++;
        	continue;
        }
		
        /*build-in command history*/
		if(!strncmp(cmd[cmd_num], "history", 7)){
			if(!strcmp(cmd[cmd_num], "history -c")){
				cmd_num = -1;
				his_ful = 0;
				//printf("History cleared!\n");
			}
			else{
			if(!his_ful){
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
					else
					if(length == 10){
					i = 10 * (((int) tmp_cmd[0]) - 48) + ((int) tmp_cmd[1]) - 48;
					if(i > cmd_num)
					printf("there is less than %d records in command history\n", i);
					else printf("%d %s\n", i, cmd[i]);
					}
					if(length > 10)
					printf("error: the maximum of history retrieving is 99!\n");
				}
			}
			else{
				if(length == 7){
					for(i = cmd_num + 1; i < HISTORY; i++)
						printf("%d %s\n", i - cmd_num - 1, cmd[i]);
					for(i = 0; i < cmd_num + 1; i++)
						printf("%d %s\n", HISTORY - cmd_num + i - 1, cmd[i]);
				}
				else if(!strncmp(cmd[cmd_num], "history ", 8)){
					tmp_cmd = (char *)malloc((length - 8) * sizeof(char));
					strcpy(tmp_cmd, (cmd[cmd_num] + 8));

					if(length == 9){
						i = (int) tmp_cmd[0] - 48;
						if((i + cmd_num + 1) < HISTORY)
							printf("%d %s\n", i, cmd[i + cmd_num + 1]);
						else
						printf("%d %s\n", i, cmd[i + cmd_num - HISTORY + 1]);
					}
					if(length == 10){
						i = 10*(((int)tmp_cmd[0])-48)+((int)tmp_cmd[1])-48;
						if((i + cmd_num + 1) < HISTORY)
							printf("%d %s\n", i, cmd[i + cmd_num + 1]);
						else
							printf("%d %s\n",i,cmd[i+cmd_num-HISTORY+1]);
					}
					if(length > 10)
					printf("error: the maximum of history retrieving is 99!\n");
				}
			}
			}
			cmd_num++;
			cmd_n++;
			continue;
		}

		/*check for space
        for(i = 0; i < length; i++)
            if(cmd[cmd_num][i] == 32){
                printf("error: Command cannot include space. Please re-enter.\n");
                break;
            }
        if(i < length){
        	cmd_num++;
        	cmd_n++;
        	continue;
        }
        */
        

        /*run executable command*/
        

		pid_t id;
        if((id=fork()) == 0){
        	char *args[10] = {NULL};
        	tmp_cmd = cmd[cmd_num];
        	
        	for(i = 0, j = 0, k = 0; i <= length; i++){
        		if(k == 0){
        				path = (char *)malloc(length * sizeof(char));
        				strncpy(path, tmp_cmd, j);
        			}
        		if(cmd[cmd_num][i] == 32){
        			

        			args[k] = (char *)malloc(length * sizeof(char));
        			strncpy(args[k++], tmp_cmd, j+1);
        			tmp_cmd = &cmd[cmd_num][i+1];
        			j = 0;
        		}
        		else{
        			j++;
        		}
        	}
        	args[k] = (char *)malloc(length * sizeof(char));
        	strncpy(args[k++], tmp_cmd, j+1);
        	for(i = 0; i < k+1; i++)
        		printf("args[%d]:%s\n", i, args[i]);
        	
        	for(i = 0; path[i] != '\0'; i++){
        		if(path[i] == 47){
        			tmp_cmd = &path[i+1];
        			j = i;
        		}
        	}
        	printf("i:%d, j:%d\n", i, j);
        	strncpy(args[0], tmp_cmd, i-j);
        	printf("path:%s\n", path);
        	for(i = 0; i < k+1; i++)
        		printf("args[%d]:%s\n", i, args[i]);
		
      	
      		if(execv(path,args) == -1)
       			printf("error: %s\n", strerror(errno));
        	return 0;
        }
        else{
        	//printf("pid: %d executed!\n", id);
        	wait(&id);
        }
        
        	        
        cmd_num++;
        cmd_n++;
    }
}
