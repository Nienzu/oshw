#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define B_SIZE 256  //Buffer_size
#define F_N_SIZE 256 //File_Name_size

void edit_file(int socket_fd, char filename[F_N_SIZE]);
int check_file(int socket, char filename[F_N_SIZE]);
int send_adv(int socket_fd, char buffer[B_SIZE]);
void create_file(int socekt_fd, char filename[F_N_SIZE]);
void list_file(int socket_fd);
void download_file(int socket_fd, char filename[F_N_SIZE]);

int main(){
	int socket_fd;
	struct sockaddr_in dest;
	char buffer[B_SIZE];
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
		bzero(&buffer, B_SIZE);
		bzero(&filename, F_N_SIZE);
		printf("==================================\nThis is command list:\n(C)在clinet端輸入檔名，在server端建立一空檔案\n(E)對server端上存在的檔案進行編輯並儲存\n(R)刪除server端的檔案\n(L)顯示server端上所有的檔案\n(D)輸入欲下載的檔名，從server端下載檔案\n(Q)離開。\n===========================\n");
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
				scanf("%s", filename);
				scanf("%c", &command);
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
				scanf("%c", &command);
				printf("Below is the file in server:\n\n");
				list_file(socket_fd);
				break;
			case 'd':
				printf("Enter the filename to download:");
				scanf("%s", filename);
				scanf("%c", &command);
				if(check_file)
					download_file(socket_fd, filename);
				
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

void edit_file(int socket_fd, char filename[F_N_SIZE]){
	int check = check_file(socket_fd, filename);		
	char buffer[B_SIZE];
	char clear;
	if(check == 1){
		printf("You can now edit %s now.\n", filename);
		printf("When you see $ mean you can edit, if not means the content is being transfer.\n");
		printf("You can enter : \"!q\" to finish the edit.\n");
		while(1){
			bzero(&buffer, B_SIZE);
			printf("$:");
			scanf("%[^\n]", buffer);
			scanf("%c", &clear);
			write(socket_fd, buffer, B_SIZE);	
			if(strcmp(buffer, "!q")==0)
				break;
		}
	}
	else
		printf("File isn't exist.\n");
	
}

void download_file(int socket_fd, char filename[F_N_SIZE]){	
	int check = check_file(socket_fd, filename);
	if(check==1){
		int brecv;
		char buffer[B_SIZE];
		bzero(&buffer, B_SIZE);
		FILE *fp = fopen(filename, "ab");
		while(check == 1){
		brecv = read(socket_fd, buffer, B_SIZE);
		printf("Byte recv : %d\n", brecv);
		fwrite(buffer, 1, brecv, fp);
		if(brecv < B_SIZE)
			break;

		}
		fclose(fp);
	}
	else
		printf("File does not exist.Please recheck.\n");
	
}

void list_file(int socket_fd){
	char buffer[B_SIZE];
	bzero(&buffer, B_SIZE);
	while(1){
		recv(socket_fd, buffer, B_SIZE, 0);
		printf("%s", buffer);
		if(buffer[0]=='\0')
			break;
	}

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
	check = send(socket_fd, buffer, B_SIZE, 0);	
	return check;
}
