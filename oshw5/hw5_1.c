#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myfs_create(const char *filesystemname,int max_size){
	FILE *fs;
	fs = fopen(filesystemname,"wb");
	int i;
	char buf[1024];
	char s[10];
	bzero(s,10);
	bzero(buf,1024);
	for(i = 0; i < max_size;++i)
		fwrite(buf,sizeof(char),1024,fs);
	rewind(fs);
	sprintf(s,"%d",max_size);
	strcat(s," ");
	fwrite(s,sizeof(char),10,fs);
	fclose(fs);
}
int myfs_destroy(const char *filesystemname){
	remove(filesystemname);
}
int main(){
	int check = 1;
	char ch,tmp;
	char name[30];
	bzero(name,30);
	while(check){
		printf("(C)reate the filesystem\n");
		printf("(D)estroy the filesystem\n");
		printf("(Q)uit\n");
		scanf("%c%c",&ch,&tmp);
		if(ch == 'C'){
			printf("Enter the system name: ");
			scanf("%s%c",name,&tmp);
			printf("Enter the system size(kB):");
			int value;
			scanf("%d%c",&value,&tmp);
			myfs_create(name,value);
		}
		else if(ch == 'D'){
			printf("Enter the system name: ");
			scanf("%s%c",name,&tmp);
			myfs_destroy(name);
		}
		else if(ch == 'Q')		
			check = 0;
		else
			printf("some wrong input\n");
	}
}
