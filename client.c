#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define B_SIZE 1024  //Buffer_size
#define F_N_SIZE 1024 //File_Name_size

void edit_file(int socket_fd, char filename[F_N_SIZE]);
int check_file(int socket, char filename[F_N_SIZE]);
void send_adv(int socket_fd, char buffer[B_SIZE]);
void create_file(int socekt_fd, char filename[F_N_SIZE]);
void list_file(int socket_fd);
void download_file(int socket_fd, char filename[F_N_SIZE]);
void clear_buffer();

int main(){
	int socket_fd;
	struct sockaddr_in dest;
	char buffer[B_SIZE];
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	char ip[20];
	bzero(&ip, 20);
	char command='\0';
	printf("Please enter the adress you want to connect:");
	scanf("%s", ip);
	scanf("%c", &command); // clear the \n 

	/*Initialize dest*/
	bzero(&dest, sizeof(struct sockaddr_in));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	inet_aton(ip, &dest.sin_addr);
	
	/*connecting to server*/
	int d = connect(socket_fd, (struct sockaddr* )&dest, sizeof(dest));
	char filename[F_N_SIZE];

	/* This is the main function of client. */
	while(1){
		bzero(&buffer, B_SIZE);
		bzero(&filename, F_N_SIZE);
		printf("==================================\nThis is command list:\n(C)在clinet端輸入檔名，在server端建立一空檔案\n(E)對server端上存在的檔案進行編輯並儲存\n(R)刪除server端的檔案\n(L)顯示server端上所有的檔案\n(D)輸入欲下載的檔名，從server端下載檔案\n(Q)離開。\n==================================\n");
		printf("Please enter command:");
		scanf("%c", &command);
		command = tolower(command);
		send(socket_fd, &command, sizeof(command), 0);
		switch(command){
			case 'c' :
				printf("Enter the Filename:");
				scanf("%s", filename);
				scanf("%c", &command);//clear \n
				if(check_file(socket_fd, filename == 0){
					create_file(socket_fd, filename);
					break;
				}
				else{
					printf("File is exist.\n");
					break;
				}
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
			default:
				printf("Oops, here is no such command!!\n");

				break;
		}
		if(command == 'q')
			break;
	}
	close(socket_fd);
	return 0;
}

/* edit the file on server 
Variable
	check : presnet the return value of check_file
	buffer : use to store the cotent to send.
	clear : use to clear \n
*/
void edit_file(int socket_fd, char filename[F_N_SIZE]){
	int check = check_file(socket_fd, filename);		
	char buffer[B_SIZE];
	char clear;
	if(check == 1){
		printf("You can edit %s now.\n", filename);
		printf("When you see $ mean you can edit, if not means the content is being transfer.\n");
		printf("You can enter : \"!q\" to finish the edit.\n");
		while(1){
			bzero(&buffer, B_SIZE);
			printf("$:");
			scanf("%[^\n]", buffer); //let scanf to accept space as input
			scanf("%c", &clear);
			write(socket_fd, buffer, B_SIZE);	
			if(strcmp(buffer, "!q")==0) //if user type only !q, will close the edit and save the file.
				break;
		}
	}
	else
		printf("File isn't exist.\n");
	
}

/* download the file from server to client
Variable
	check : presnet the return value of check_file
	brecv : record the real byte recived from server
	buffer : use to store temp content to write.
 */
void download_file(int socket_fd, char filename[F_N_SIZE]){	
	int check = check_file(socket_fd, filename);
	if(check==1){
		int brecv;
		char buffer[B_SIZE];
		bzero(&buffer, B_SIZE);  // Always set the buffer to empty
		FILE *fp = fopen(filename, "ab");   // download by bit
		while(check == 1){
		brecv = read(socket_fd, buffer, B_SIZE);
		printf("Byte recv : %d\n", brecv); //print the download process.
		fwrite(buffer, 1, brecv, fp); 
		if(brecv < B_SIZE)
			break;
		/* when  the read bit is less than B_SIZE, meaning finishing the download. */
		}
		fclose(fp);
	}
	else
		printf("File does not exist.Please recheck.\n");
	
}

/*  list the file on server. 
Variable 
	buffer : use to store temporary content to write.
*/
void list_file(int socket_fd){
	char buffer[B_SIZE];
	bzero(&buffer, B_SIZE);
	while(1){
		recv(socket_fd, buffer, B_SIZE, 0);
		printf("%s", buffer);
		if(buffer[0]=='\0')
			break;   // receive until the  server's popen's return value 's end.
	}

}

/* Create command. */
void create_file(int socket_fd , char filename[F_N_SIZE]){
	send_adv(socket_fd, filename);
	/* just send the filename created. */
}

/* This is normally use in many function to check the file exist in server or not. 
Variable 
	check : present the value of check_file

Return value
	1 : file exist
	-1 : file does not exist
*/
int check_file(int socket_fd, char filename[F_N_SIZE]){
	char check; 
	send_adv(socket_fd, filename);
	recv(socket_fd, &check, sizeof(check), 0);
	
	return check == 'y' ? 1 : -1 ;
}

/*Rewrite send function with check.Just for convinient...*/
void send_adv(int socket_fd, char buffer[B_SIZE]){
	send(socket_fd, buffer, B_SIZE, 0);	
}

