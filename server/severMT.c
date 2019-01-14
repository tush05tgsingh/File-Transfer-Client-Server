#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>


#define PORT 4444
#define MAX  1024



int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;
	 FILE *fp;
	 int bytesReceived = 0;
     char recvBuff[256];

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024],op[1000];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else{
					if(strcmp(buffer, "1")== 0)
					{
						/*printf("Client: %s\n", buffer);
						send(newSocket, buffer, strlen(buffer), 0);
						bzero(buffer, sizeof(buffer));*/
						recv(newSocket, buffer, sizeof(buffer), 0);
						fp=fopen(buffer,"w");
						if(NULL == fp)
				    {
				        printf("Error opening file");
				        return 1;
				    }
              bytesReceived = read(newSocket, recvBuff, 256);
							printf("bytes:%d",bytesReceived);
				    // Receive data in chunks of 256 bytes
				    while(bytesReceived != 0)
				    {
							printf("bytes:%d",bytesReceived);
				        printf("Bytes received %d\n",bytesReceived);
				        // recvBuff[n] = 0;

				        fwrite(recvBuff, 1,bytesReceived,fp);
				        // printf("%s \n", recvBuff);
								bytesReceived = read(newSocket, recvBuff, 256);
				    }

            printf("bytes:%d",bytesReceived);
              printf("Bytes received %d\n",bytesReceived);
              // recvBuff[n] = 0;

              fwrite(recvBuff, 1,bytesReceived,fp);
              // printf("%s \n", recvBuff);
              bytesReceived = read(newSocket, recvBuff, 256);
              
				    if(bytesReceived < 0)
				    {
				        printf("\n Read Error \n");
				    }
            printf("the file has been transferred\n");
            fclose(fp);
							//receive(newSocket,0);
					}
					else if(strcmp(buffer, "2")== 0)
					{
                system("find . -maxdepth 1 -type f | sort");
								recv(newSocket, buffer, sizeof(buffer), 0);
								fp = fopen(buffer,"rb");
								send(newSocket, buffer, strlen(buffer), 0);
				        if(fp==NULL)
				        {
				            printf("File opern error");
				            return 1;
				        }
								while(1)
					      {
					            // First read file in chunks of 256 bytes
					            unsigned char buff[256]={0};
					            int nread = fread(buff,1,256,fp);
					            printf("Bytes read %d \n", nread);

					            // If read was success, send data.
					            if(nread > 0)
					            {
					                printf("Sending \n");
					                write(newSocket, buff, nread);
					            }

					            /*
					             * There is something tricky going on with read ..
					             * Either there was error, or we reached end of file.
					             */
					            if (nread < 256)
					            {
					                if (feof(fp))
					                    printf("End of file\n");
					                if (ferror(fp))
					                    printf("Error reading\n");
					                break;
					            }
											printf("tranfered file");
											fclose(fp);
					    }
              //sending(newSocket,0);
               //SendFileToClient(newSocket);
					}

				}
				/*printf("Client: %s\n", buffer);
				send(newSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));*/
			}
		}

	}

	close(newSocket);


	return 0;
}
