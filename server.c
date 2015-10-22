#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>

#define B_SIZE 1024   //Buffer max size
#define F_N_SIZE 1024 // Filename max size


void list_file(int client);
void trans_file(int client);
int check_file(int client, char filename[F_N_SIZE]);
void edit_file(int client);

int main(){
	int socket_fd;
	struct sockaddr_in dest;
	char buffer[B_SIZE];
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	dest.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socket_fd, (struct sockaddr*)&dest, sizeof(dest));

	listen(socket_fd, 20);
	FILE *fp;
	bzero(&buffer, B_SIZE);
	
	int client;
	struct sockaddr_in client_addr;
	socklen_t addlen;
	addlen = sizeof(client_addr);
	client = accept(socket_fd, (struct sockaddr*)&client_addr, &addlen);
	
	int check=0;
	char command='\0';
	while(1){
		bzero(&buffer, B_SIZE);  /*always clear the buffer*/
		recv(client, &command, sizeof(command), 0);
		switch(command){
		case 'c':
			recv(client, &buffer, B_SIZE, 0);
			check = check_file(client, buffer);
			//only when file doesn't exist, server will create the file.
			if(check == -1){
				fp = fopen(buffer, "w");
				fclose(fp);
				break;
			}
		case 'e':
			edit_file(client);
			break;
		case 'r':
			/* Here is the remove function. */
			recv(client, &buffer, B_SIZE, 0);
			check = check_file(client, buffer);
			if(check==1)
				remove(buffer);
			break;
		case 'l':
			list_file(client);
			break;
		case 'd':
			trans_file(client);
			break;
		case 'q':
			break;		
		}
		if(command == 'q')
			break;
	}
	close(socket_fd);	
	return 0;
}

/* edit the existing file on server. 
Variable 
	buffer : use to store the cotent to send.
	filename : file's name
	check : use to present the return value of check_file
*/
void edit_file(int client){
	char buffer[B_SIZE];
	char filename[F_N_SIZE];
	bzero(&buffer, B_SIZE);
	bzero(&filename, F_N_SIZE); //always clear ...
	recv(client, &filename, F_N_SIZE, 0);
	int check = check_file(client, filename);
	//first must check the file is exsting or not.
	if(check == 1){
		FILE *fp = fopen(filename, "a"); // edit file but not clean the original content.
		while(1){
			bzero(&buffer, B_SIZE);
			read(client, buffer, B_SIZE);
			if(buffer[0]=='!' && buffer[1]=='q') //when client send the !q will close the file and save.(just like vim...)
				break;
			fprintf(fp, "%s\n", buffer);
		}
		fclose(fp);
	}
}

/* just get the current file on server and send the list to client. 
Variable 
	cccc : is to present the command I may use.But here is just to represent ls.
	buffer : use to store the cotent to send.
*/
void list_file(int client){
	char cccc[100];
	bzero(&cccc, 100);
	cccc[0]='l';
	cccc[1]='s';
	//just use popen to implement the "ls" command and send it to client.
	FILE *fp = popen(cccc, "r");
	char buffer[B_SIZE];
	bzero(&buffer, B_SIZE);
	while((fgets(buffer, B_SIZE, fp) != NULL)){	
		send(client, buffer, B_SIZE, 0);
		bzero(&buffer, B_SIZE); //always clear the buffer
	}//gets the output of ls and send filename one by one to client.
	send(client, buffer, B_SIZE, 0);	
}


/* corresponding the download function in client. 
Variable
	filename : the file to transfer
	check : use to present the return value of check_file
	fp : file pointer
	buffer : use to send bit.
	bread : record the real byte read from the file.
*/
void trans_file(int client){
	char filename[F_N_SIZE];
	bzero(&filename, F_N_SIZE);
	recv(client, &filename, F_N_SIZE, 0);
	int check = check_file(client, filename);
	FILE *fp = fopen(filename, "rb");
	//read the file by byte, then send it by byte.
	char buffer[B_SIZE];
	int bread;
	while(check == 1){
		bzero(&buffer, B_SIZE);
		bread = fread(buffer, 1, B_SIZE, fp);
		//record the actually read in bit to control  the  transfer process.
		if(bread > 0){	
			write(client, buffer, bread);	
			printf("Byte read %d:\n", bread);// just print the transfering process.
		}
		if(bread < B_SIZE){ // when bread
			if(feof(fp))
				printf("End of file.Copy complete.\n");
			if(ferror(fp))
				printf("error reading\n"); 
			break;
		}
	}
}

/* corresponding the check_file gunction to client.c 
Variable
	check : to present the file is exsting or not.

Return value
	1 : file exist 
	-1 : file does not exist
*/
int check_file(int client, char filename[F_N_SIZE]){
	char check='\0';
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		check='n';
		send(client, &check, sizeof(check), 0);
		return -1;
	}
	else{
		check='y';
		send(client, &check, sizeof(check), 0);
		return 1;
	}
}
