#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myfs_create(const char *filesystemname,int max_size){
	FILE *File_sys;
	File_sys = fopen(filesystemname,"wb");
	int i;
	char buf[1024];
	char size[10];
	bzero(size,10);
	bzero(buf,1024);

	for(i = 0; i < max_size;++i)
		fwrite(buf,sizeof(char),1024,File_sys);
	rewind(File_sys);
	sprintf(size,"%d",max_size);
	strcat(size," ");
	fwrite(size,sizeof(char),10,File_sys);
	fclose(File_sys);
	return 0;
}
int myfs_destroy(const char *filesystemname){
	remove(filesystemname);
	return 0;
}
int main(){
	int check = 1;
	char command,tmp;
	char name[30];
	bzero(name,30);
	while(check){
		printf("(C)reate the filesystem.\n");
		printf("(D)estroy the filesystem.\n");
		printf("(Q)uit.\n");
		scanf("%c%c",&command,&tmp);
		if(command == 'C'){
			printf("Enter the system name: ");
			scanf("%s%c",name,&tmp);
			printf("Enter the system size in KB :");
			int value;
			scanf("%d%c",&value,&tmp);
			myfs_create(name,value);
		}
		else if(command == 'D'){
			printf("Enter the system name: ");
			scanf("%s%c",name,&tmp);
			myfs_destroy(name);
		}
		else if(command == 'Q')		
			check = 0;
		else
			printf("No this command.\n");
	}
}
