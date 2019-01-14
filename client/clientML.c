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
#define MAX 1024

void * receive(int newfd, int k)
{
 	 char buff[MAX], file_name[20],ch;
	int i,j;
  	double n,Total_Length=0, BufferSize=MAX, NoOfPkts, CurrentPktLength;
  	int flag1=0, flag2=0;

  while(flag1==0)
  {
		printf("Save file as:  ");
		gets(file_name);
		FILE* file_ptr=fopen(file_name,"w");
		do
		{
			      bzero(buff,MAX);

			     read(newfd,buff,sizeof(buff));


			      fprintf(file_ptr, "%s\n",buff);


	  }
		while(buff[1023]!='\0');
		fclose(file_ptr);
		printf("File Succesfully received\n!");
	}
 }
	/*printf("\n To Client: ");
      bzero(buff,MAX);
      n=0;
      while((buff[n++]=getchar())!='\n');

      write(newfd,buff,sizeof(buff));*/
void * sending(int newfd, int k)
{
			 	 char buff[MAX], file_name[20],ch;
				int i,j;
			  	double n,Total_Length=0, BufferSize=MAX, NoOfPkts, CurrentPktLength;
			  	int flag1=0, flag2=0;
     bzero(buff,sizeof(buff));

		printf("Enter the name of file to send: ");
		gets(file_name);
		FILE *file_ptr=fopen(file_name,"r");
		if(file_ptr==NULL)
		{
			printf("error in opening file");
		}


		while(ch=fgetc(file_ptr)!=EOF)
		{
			Total_Length++;
			//printf("Count: %d", Total_Length);
		}

		rewind(file_ptr);


		NoOfPkts=ceil(Total_Length/BufferSize);
		printf("No of packets: %f\n", NoOfPkts);
    		n=0;



		for(i=0;i<NoOfPkts;i++)
		{
			bzero(buff,sizeof(buff));
			if(Total_Length>BufferSize)
			{
				CurrentPktLength=BufferSize;;
				Total_Length=Total_Length-CurrentPktLength;
			}
			else
			{
				CurrentPktLength=Total_Length;
			}
			j=0;
			for(j=0;j<CurrentPktLength;j++)
			{
				buff[j]=fgetc(file_ptr);
			}

	   		//printf("Here1\n");
	   		 write(newfd,buff,sizeof(buff));
		//printf("Here2\n");


		}
	printf("\nFile Sent Successfully!\n");
	fclose(file_ptr);

      if((strncmp(buff,"exit",4))==0)
      {
        printf("Client Exit...\n");
        flag1=1;
      }
			return(0);
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
		printf("1.send file to server\n2.rev file from server\n ");
		scanf("%d",&x);
		printf("choice to end to server");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
			if(x==1)
			{

				printf("enter the file to be sent to server");
				printf("Client: \t");
				scanf("%s", &buffer[0]);

				send(clientSocket, buffer, strlen(buffer), 0);
				fp=fopen(buffer,"rb");

		    strcpy(filename[t],buffer);
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
									write(clientSocket, buff, nread);
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
		  else if(x==2)
			{
			  puts("---- Files btw Server and the Client ----");
        for (j = 0; j < i; j++) {
            puts(client_server_files[j]);
        }
				printf("Client: \t");
				scanf("%s", &buffer[0]);
				send(clientSocket, buffer, strlen(buffer), 0);
				FILE *fp;
				recv(clientSocket, buffer, sizeof(buffer), 0);
		    fp = fopen(buffer, "ab");
		    if(NULL == fp)
		    {
		        printf("Error opening file");
		        return 1;
		    }

		    // Receive data in chunks of 256 bytes
		    while((bytesReceived = read(clientSocket, recvBuff, 256)) > 0)
		    {
		        printf("Bytes received %d\n",bytesReceived);
		        // recvBuff[n] = 0;
		        fwrite(recvBuff, 1,bytesReceived,fp);
		        // printf("%s \n", recvBuff);
		    }

		    if(bytesReceived < 0)
		    {
		        printf("\n Read Error \n");

		    }
				printf("file transfered from server\n");
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
