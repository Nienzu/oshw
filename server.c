#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>

#define B_SIZE 256
#define F_N_SIZE 256


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
		bzero(&buffer, B_SIZE);
		recv(client, &command, sizeof(command), 0);
		switch(command){
		case 'c':
			recv(client, &buffer, B_SIZE, 0);
			fp = fopen(buffer, "w");
			fclose(fp);
			break;
		case 'e':
			edit_file(client);
			break;
		case 'r':
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

void edit_file(int client){
	char buffer[B_SIZE];
	char filename[F_N_SIZE];
	bzero(&buffer, B_SIZE);
	bzero(&filename, F_N_SIZE);
	recv(client, &filename, F_N_SIZE, 0);
	int check = check_file(client, filename);
	int brecv=0;
	if(check == 1){
		FILE *fp = fopen(filename, "a");
		while(1){
			bzero(&buffer, B_SIZE);
			read(client, buffer, B_SIZE);
			if(buffer[0]=='!' && buffer[1]=='q')
				break;
			fprintf(fp, "%s\n", buffer);
		}
		fclose(fp);
	}
}

void list_file(int client){
	char cccc[100];
	bzero(&cccc, 100);
	cccc[0]='l';
	cccc[1]='s';
	FILE *fp = popen(cccc, "r");
	char buffer[B_SIZE];
	bzero(&buffer, B_SIZE);
	while((fgets(buffer, B_SIZE, fp) != NULL)){	
		send(client, buffer, B_SIZE, 0);
		bzero(&buffer, B_SIZE);
	}
	send(client, buffer, B_SIZE, 0);	
}

void trans_file(int client){
	char filename[F_N_SIZE];
	bzero(&filename, F_N_SIZE);
	recv(client, &filename, F_N_SIZE, 0);
	int i = check_file(client, filename);
	FILE *fp = fopen(filename, "rb");
	char buffer[B_SIZE];
	int bread ;
	while(i == 1){
		bzero(&buffer, B_SIZE);
		bread = fread(buffer, 1, B_SIZE, fp);
		if(bread > 0){	
			write(client, buffer, bread);	
			printf("Byte read %d:\n", bread);
		}
		if(bread < B_SIZE){
			if(feof(fp))
				printf("End of file.Copy complete.\n");
			if(ferror(fp))
				printf("error reading\n");
			break;
		}
	}
}
int check_file(int client, char filename[F_N_SIZE]){
	char buffer[B_SIZE];
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
