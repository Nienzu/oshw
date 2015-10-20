#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>

#define B_SIZE 128
#define F_N_SIZE 128


void list_file(int client);

int main(){
	int socket_fd;
	struct sockaddr_in dest;
	char buffer[128];
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	dest.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socket_fd, (struct sockaddr*)&dest, sizeof(dest));

	listen(socket_fd, 20);
	FILE *fp;
	bzero(&buffer, 128);
	
	int client;
	struct sockaddr_in client_addr;
	socklen_t addlen;
	addlen = sizeof(client_addr);
	client = accept(socket_fd, (struct sockaddr*)&client_addr, &addlen);
	

	char command='\0';
	while(1){
		bzero(&buffer, B_SIZE);
		recv(client, &command, sizeof(command), 0);
		switch(command){
		case 'c':
			recv(client, &buffer, sizeof(buffer), 0);
			fp = fopen(buffer, "w");
			fclose(fp);
			break;
		case 'r':
			recv(client, &buffer, sizeof(buffer), 0);
			fp = fopen(buffer, "r");
			if(fp == NULL){
				command = 'n';
				send(client, &command, sizeof(command), 0);
			}
			else{
				command = 'y';
				send(client, &command, sizeof(command), 0);
				remove(buffer);
			}
			break;
		case 'l':
			list_file(client);

			break;
		case 'q':
			break;		
			//if(a==1)
				//close(socket_fd);
		//close(client);
		//send(client, buffer, sizeof(buffer), 0);

		}
		if(command == 'q')
			break;
	}
	close(socket_fd);	
	return 0;
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
		send(client, buffer, sizeof(buffer), 0);
		bzero(&buffer, B_SIZE);
	}
	send(client, buffer, sizeof(buffer), 0);	

}
