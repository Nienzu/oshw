#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char data[10000];
    FILE *fp;
    fp = fopen(argv[1], "r");
    fread(data, sizeof(char), 10000, fp);
    //read data from txt file
	int length = 0, status = 0, size = 0;
    int i, j, k;
    for(i=0; argv[2][i] != '\0'; i++)
        length++;
    for(i=0; data[i] != '\0'; i++)
        size++;
    size--;
	// data's total words
    int num;
    num = atoi(argv[3]);
    int begin[num];
    int end[num];
    int part = size/num;
    begin[0] = 0;
    for(i=1; i < num; i++){
        end[i-1] = part*i;
        begin[i] = end[i-1] - length + 1;
    }
    end[num-1] = size+1;
    //pid[] use to record the child's pid and to distinguish which is parent or child.
	pid_t pid[num];
    int *result;
    result = mmap(NULL, num*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    //shared memory setting

	//below is main searching process.
	for(i=0; i < num; i++) {
        pid[i] = fork();// this only parent will do
        if(pid[i] == 0) {
            int temp = 0, count = 0;
            for(j = begin[i]; j < end[i]; j++) {
                for(k = 0; k < length; k++) {
                    if((j+k) == end[i]) {
                        j = end[i];
                        break;
                    }
                    if(data[j+k] == argv[2][k])
                        count ++;
                }
                if(count == length)
                    temp++;
				count = 0;
            }
            printf("Proccess%d: %d \n", i+1, temp);
            result[i] = temp;
            exit(-1);
        } else if(pid[i] < 0)
            printf("Error happen!!\n");
    }
    for(i=0; i < num; ++i)
        wait(&status);
	//wait for child all finish.
    int total = 0;
    for(i=0; i < num; ++i)
        total += result[i];
    printf("total:%d\n",total);
    munmap(result,num*sizeof(int));
    fclose(fp);
    return 0;
}
