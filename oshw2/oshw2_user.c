#include <stdio.h>

int main(){
	FILE *fp = popen("cat /proc/batoto", "r");
	char buffer[100];
	while((fgets(buffer, 100, fp) != NULL)){ 
		printf("%s\n", buffer);
	}
	return 0;
}
