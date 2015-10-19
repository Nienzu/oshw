#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define B_SIZE 128   //Buffer_size
#define F_N_SIZE 128 //File_Name_size

void edit_file(int socket_fd, char filename[F_N_SIZE]);
int check_file(int socket, char filename[F_N_SIZE]);
int send_adv(int socket_fd, char buffer[B_SIZE]);
void create_file(int socekt_fd, char filename[F_N_SIZE]);


int main(){
	int socket_fd;
	struct sockaddr_in dest;
	char buffer[128];
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	/*Initialize dest*/
	bzero(&dest, sizeof(struct sockaddr_in));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	inet_aton("127.0.0.1", &dest.sin_addr);
	
	/*connecting to server*/
	int d = connect(socket_fd, (struct sockaddr* )&dest, sizeof(dest));
	char command='\0';
	char filename[128];
	while(1){
		bzero(&buffer, 128);
		bzero(&filename, 128);
		printf("==================================\nThis is command list:\n(C)在clinet端輸入檔名，在server端建立一空檔案\n(E)對server端上存在的檔案進行編輯並儲存\n(R)刪除server端的檔案\n(L)顯示server端上所有的檔案\n(D)輸入欲下載的檔名，從server端下載檔案\n===========================\n");
		printf("Please enter command:");
		scanf("%c", &command);
		send(socket_fd, &command, sizeof(command), 0);
		switch(command){
			case 'c' :
				printf("Enter the Filename:");
				scanf("%s", filename);
				scanf("%c", &command);//clear the std input buffer
				create_file(socket_fd, filename);
				break;
			case 'e':
				printf("Enter the file name:");
				printf("%s", filename);
				if (check_file(socket_fd, filename) == -1)
					printf("Error : File is not exsting.\n");
				else
					edit_file(socket_fd, filename);
				break;
			case 'r':
				printf("Enter the filename to remove:");
				scanf("%s", filename);
				scanf("%c", &command);
				if(check_file(socket_fd, filename) == -1)
					printf("Sorry there is no such file.\n");
				else {
					printf("Remove successed\n");
				}
				break;
			case 'l':
				printf("Below is the file in server:\n");
				break;
			case 'q':
				break;

		}
		if(command == 'q')
			break;
	}
	/*Receive*/
	//bzero(buffer, 128);
	//recv(socket_fd, buffer, sizeof(buffer), 0);
	//printf("%s %d\n", buffer, d);
	close(socket_fd);
	return 0;
}

void edit_file(int socket_fd, char filename[F_N_SIZE]){
		
	;
	
	
}


void create_file(int socket_fd , char filename[F_N_SIZE]){
	send_adv(socket_fd, filename);	
}


int check_file(int socket_fd, char filename[F_N_SIZE]){
	char check; 
	send_adv(socket_fd, filename);
	recv(socket_fd, &check, sizeof(check), 0);
	
	return check == 'y' ? 1 : -1 ;
}

/*Rewrite send function with check*/
int send_adv(int socket_fd, char buffer[B_SIZE]){
	int check=0;
	//send(socket_fd, &length, sizeof(length));
	check = send(socket_fd, buffer, sizeof(buffer), 0);	
	return check;
	/*
	while(check != legnth){
		//printf("Erro happen, resent again...");
		check = send(socket_fd, &length, sizeof(length));
		check = send(socket_fd, filename, sizeof(filename), 0);	
	}
	*/
}
