#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size;
char fs_name[100];

int myfs_file_open(const char *filename);
int myfs_file_close(int fd);
int myfs_file_create(const char *filename);
int myfs_file_delete(const char *filename);

int myfs_file_read(int fd, char *buf, int count){
	int i, max = 0, read = 0;
	FILE *File_sys;
	File_sys = fopen(fs_name,"rb");
	char block[1024], header[30], number[4];
	bzero(block,1024);
	bzero(header,30);
	bzero(number,4);
	buf = calloc(count,sizeof(char));
	max = count / 990;
	read = count % 990;
	if(read == 0 && max > 0){
		read = 990;
		--count;
	}
	count = 0;
	fseek(File_sys,fd * 1024,SEEK_SET);
	fread(block,sizeof(char),1024,File_sys);
	strncpy(header,block,30);
	if(max != 0)
		strncpy(buf,block + 34,990);
	else
		strncpy(buf,block + 34,read);
	rewind(File_sys);
	while(count != max){
		++count;
		for(i = 0;i < size;++i){
			fread(block,sizeof(char),1024,File_sys);
			strncpy(number,block + 30,4);
			if(strncmp(header,block,30) == 0 && count == atoi(number)) 
				if(count == max)
					strncat(buf,block + 34,read);
				else
					strncat(buf,block + 34,990);
		}
		rewind(File_sys);
	}
	fclose(File_sys);
	printf("******Content******\n");
	printf("%s\n",buf);
	printf("********END********\n");
	free(buf);
}

int myfs_file_write(int fd, char *buf, int count){
	FILE *File_sys;
	FILE *duplicate;
	File_sys = fopen(fs_name, "rb");
	duplicate = fopen("fs_tmp", "wb");
	char block[1024], header[30], number[4], tmp;
	bzero(block, 1024);
	bzero(header, 34);
	bzero(number, 4);
	int i, space = 0, write;

	for(i = 0;i < size;++i){
		fread(block, sizeof(char), 1024, File_sys);
		if(block[0] == '\0')
			space++;
	}
	if(count > (space * 990)){
		printf("File system is full\n");
		return 0;
	}
	buf = calloc(count,sizeof(char));
	printf("Enter the content:");
	scanf("%[^\n]",buf);
	scanf("%c",&tmp);
	space = count / 990;
	write = count % 990;
	if(space > 0){
		write = 990;
		--space; 
	}
	count = 0;
	rewind(File_sys);
	for(i = 0;i < size;++i){
		fread(block,sizeof(char),1024,File_sys);
		if(i != fd)
			fwrite(block,sizeof(char),1024,duplicate);
		else{
			strncpy(header,block,30);
			fwrite(block,sizeof(char),34,duplicate);
			fwrite(buf,sizeof(char),write,duplicate);
			if(space < 0){
				bzero(block,1024);
				fwrite(block,sizeof(char),990-write,duplicate);
			--space;
			++count;
			}
		}
	}
	fclose(File_sys);
	fclose(duplicate);
	rename("fs_tmp",fs_name);

	int variable ;
	while(space > -1){
		File_sys = fopen(fs_name,"rb");
		duplicate = fopen("fs_tmp","wb");
		bzero(block, 1024);
		sprintf(number, "%d", count);
		variable = 1;
		for(i = 0;i < size;++i){
			fread(block, sizeof(char), 1024, File_sys);
			if(block[0] != '\0')
				fwrite(block, sizeof(char), 1024, duplicate);
			else if (variable == 1){
				bzero(block, 1024);
				strcpy(block, header);
				strcat(block, number);
				fwrite(block, sizeof(char), 34, duplicate);
				fwrite(buf+990*count, sizeof(char), write, duplicate);
				++count;
				--space;
				variable = -1;
			}
		}
		fclose(File_sys);
		fclose(duplicate);
		rename("fs_tmp",fs_name);
	}
	free(buf);
}	

int main(int argc,char **argv){
	FILE *File_sys;
	char ch,n,block[1024],tmp[1024],name[30];
	char *buf;
	int fd = 0,check = 1,count;
	bzero(block,1024);
	bzero(tmp,1024);
	bzero(name,30);
	bzero(fs_name,100);
	strcpy(fs_name,argv[1]);
	File_sys = fopen(argv[1],"rb");

	fread(block,sizeof(char),1024,File_sys);
	sscanf(block,"%d%s",&size,tmp);
	printf("Name of the File system: %s\n", fs_name);
	printf("Size of %s : %d KB\n", fs_name, size);
	rewind(File_sys);
	int i;
	fclose(File_sys);
	while(check){
		if(fd == 0){
			printf("(O)pen the file\n");
			printf("(C)reate the file\n");
			printf("(D)elete the file\n");
			printf("(Q)uit\n");
		}
		else{
			printf("(C)lose the file\n");
			printf("(R)ead the file\n");
			printf("(W)rite the file\n");
			printf("(Q)uit\n");
		}

		scanf("%c%c",&ch,&n);

		if(ch == 'O' && fd == 0){
			printf("Enter the file name:");
			scanf("%s%c",name,&ch);
			fd = myfs_file_open(name);
		}
		else if(ch == 'C' && fd != 0){
			fd = myfs_file_close(fd);
		}
		else if(ch == 'C' && fd == 0){
			printf("Enter the file name:");
			scanf("%s%c",name,&ch);
			myfs_file_create(name);
		}
		else if(ch == 'D' && fd == 0){
			printf("Enter the file name:");
			scanf("%s%c",name,&ch);
			myfs_file_delete(name);
		}
		else if(ch == 'R' && fd != 0){
			printf("Enter the size you want to read:");
			scanf("%d%c",&count,&ch);
			myfs_file_read(fd,buf,count);
		}
		else if(ch == 'W' && fd != 0){
			printf("Enter the size you want to write:");
			scanf("%d%c",&count,&ch);
			myfs_file_write(fd,buf,count);
		}
		else if(ch == 'Q')
			check = 0;
		else
			printf("Some wrong input\n");
		bzero(name,30);
		printf("********************\n");
	}
}

int myfs_file_open(const char *filename){
	FILE *File_sys;
	File_sys = fopen(fs_name,"rb");
	char block[1024];
	bzero(block,1024);
	fread(block,sizeof(char),1024,File_sys);
	char *check;
	check = strstr(block,filename);
	if(check == NULL){
		printf("File doesn't exist.\n");
		fclose(File_sys);
		return 0;
	}
	check = strtok(block," ");
	char name[30];
	bzero(name,30);
	int value = 0, ret = 0;
	while(check != NULL){
		if(strcmp(check,filename) == 0)
			ret = 1;
		else if(ret == 1){
			value = atoi(check);
			fclose(File_sys);
			return value;
		}
		check = strtok(NULL," ");
	}
}

int myfs_file_close(int fd){
	return 0;	
}

int myfs_file_create(const char *filename){
	FILE *File_sys;
	FILE *duplicate;
	File_sys = fopen(fs_name,"rb");
	duplicate = fopen("fs_tmp","wb");
	char block[1024],tmp[1024];
	bzero(block,1024);
	bzero(tmp,1024);
	fread(block,sizeof(char),1024,File_sys);
	char *check;
	check = strstr(block,filename);
	if(check != NULL){
		printf("File already exists!!\n");
		return 0;
	}
	int i, location = 0;
	for(;block[0] != '\0';++location){
		fread(block,sizeof(char),1024,File_sys);
		if(location >= size){
			printf("File system is full.\n");
			return 0;
		}
	}
	rewind(File_sys);
	fread(block,sizeof(char),1024,File_sys);
	strcat(block,filename);
	sprintf(tmp," %d",location);
	strcat(block,tmp);
	strcat(block," ");
	for(i = 0;i < size;++i){
		if(i == location){
			fwrite(filename,sizeof(char),30,duplicate);
			bzero(tmp,1024);
			fwrite(tmp,sizeof(char),994,duplicate);
		}
		else
			fwrite(block,sizeof(char),1024,duplicate);
		if(i != size -1)
			fread(block,sizeof(char),1024,File_sys);
	}
	fclose(File_sys);
	fclose(duplicate);
	rename("fs_tmp",fs_name);
}

int myfs_file_delete(const char *filename){
	FILE *File_sys;
	FILE *duplicate;
	File_sys = fopen(fs_name,"rb");
	duplicate = fopen("fs_tmp","wb");
	char block[1024],tmp[1024],name[30];
	bzero(block,1024);
	bzero(tmp,1024);
	bzero(name,30);
	fread(block,sizeof(char),1024,File_sys);
	char *check;
	check = strstr(block,filename);
	if(check == NULL){
		printf("File doesn't exist.\n");
		return 0;
	}
	int i;
	sprintf(name,"%d ",size);
	strcpy(tmp,name);
	for(i = 1;i < size;++i){
		fread(block,sizeof(char),1024,File_sys);
		strncpy(name,block,30);
		if(strcmp(name,filename) != 0 && name[0] != '\0'){
			strcat(tmp,name);
			sprintf(name," %d",i);
			strcat(tmp,name);
			strcat(tmp," ");
			bzero(name,30);
		}
	}
	bzero(name,30);
	fseek(File_sys,1024,SEEK_SET);
	for(i = 0;i < size;++i){
		if(strcmp(name,filename) == 0)
			bzero(tmp,1024);
		fwrite(tmp,sizeof(char),1024,duplicate);
		if(i != size - 1){
			fread(tmp,sizeof(char),1024,File_sys);
			bzero(name,30);
			strncpy(name,tmp,30);	
		}
	}
	fclose(File_sys);
	fclose(duplicate);
	rename("fs_tmp",fs_name);
}
