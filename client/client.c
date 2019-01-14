#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <sys/stat.h>
#include <errno.h>

#define PORT 4444
#define MAX 1024

off_t fsize(const char * filename)
{
   struct stat st;
   if(stat(filename,& st)==0)
         return st.st_size;
  fprintf(stderr,"cannnot determine size of %s: %s\n",filename,strerror(errno));
  return-1;
}

int myAtoi(char str[1024])
{
    int res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    // return result.
    return res;
}


int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024],op[1000];
	FILE *fp;
	char *client_server_files[1024];
	int bytesReceived = 0;
  char recvBuff[256], filename[30][1024];
	int i=0,j,t=0;
  int sz;



	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	int x;
	while(1){
    bzero(&buffer,256);
		printf("1.send file to server\n2.rev file from server\n ");
		scanf("%s", &buffer[0]);
		write(clientSocket, buffer, 256);
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		if(strcmp(buffer, "one") == 0)
	  {

				printf("enter the file to be sent to server");
				printf("Client: \t");
				scanf("%s", &buffer[0]);
				write(clientSocket, buffer, 256);
				fp=fopen(buffer,"rb");
        strcpy(filename[t],buffer);

        struct stat buff;
          if( stat(buffer,&buff) == -1)
          {
            perror("stat fail");
            exit(EXIT_FAILURE);
          }
          else
          {
            sz=buff.st_size;
          }
        bzero(&buffer,256);
        sprintf(buffer,"%d",sz);
        printf("%d",sz);
          //sending file size
        write(clientSocket,buffer,256);
          //sending contents
      	int count = 0;

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

              //printf("%d",nread);
							// If read was success, send data.
							if(nread > 0)
							{
                 int m;

									printf("Sending \n");
								 m=	write(clientSocket, buff, nread);
                 printf("m=%d",m);
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
			   }
	        //send(clientSocket,op,sizeof(op),0);
				// add files in char pointer array
				//sending(clientSocket,0);
	                    client_server_files[i++] = filename[t++];
	      //fclose(fp);
				printf("transfer done/n");
    }
		else if(strcmp(buffer, "two") == 0)
	  {
			  puts("---- Files btw Server and the Client ----");
        for (j = 0; j < i; j++) {
            puts(client_server_files[j]);
        }
				printf("Client: \t");
				scanf("%s", &buffer[0]);
				write(clientSocket, buffer, 256);
				FILE *fp;
		    fp=fopen(buffer,"w");

          bzero(buffer,256);
          int hii=read(clientSocket,buffer,256);
          printf("hii: %d\n",hii);
          long int sz = myAtoi(buffer);
          printf("sz: %ld\n",sz);
          if(NULL == fp)
          {
              printf("Error opening file");
              return 1;
          }
          int count=0;
        // Receive data in chunks of 256 bytes
        while(count <sz)
        {
          bytesReceived = read(clientSocket, recvBuff, 256);

            printf("Bytes received %d\n",bytesReceived);
            // recvBuff[n] = 0;

            fwrite(recvBuff, 1,bytesReceived,fp);
            // printf("%s \n", recvBuff);

            count+=bytesReceived;
        }


        if(count == sz)
        {
            printf("\n Succesfully \n");
        }
        else
        {
          printf("error\n" );
        }
        printf("the file has been transferred\n");
        fclose(fp);
				//receive(clientSocket,0);
		 }

	}

	return 0;
}

/*int main()
{
  File *f;
  int words=0;

  char c;

  f= fopen("glad.txt", "r");
  while((c=get(c)) != EOF)
  {
      fscanf(f , "%s", buffer);
      if(isspace(c) || c=='\t')
      words++;
  }
  write(sockfd, &words, sizeof);
  rewind(f);

  char ch;
  while(ch!=EOF)
  {
      fscanf(f , "%s", buffer);
      write(sockfd, buffer, 255);
      ch=fget(c);
  }
  printf()
}*/
