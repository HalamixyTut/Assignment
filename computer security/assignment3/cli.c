#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
        int  sockfd, n; 
        char recvline[100], comm[20];
        struct sockaddr_in servaddr;

	if(argc!=3){
		printf("Usage : ./cli <Host Name> <Port>");
		exit(1); 
	}

        /* Create a TCP socket */
	if((sockfd=socket(AF_INET,SOCK_STREAM, 0)) < 0){
			perror("socket"); exit(2);}
	    
        /* Specify server�s IP address and port */
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2])); /* daytime server port */

        if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		perror("inet_pton"); exit(3);
        }

	/* Connect to the server */
       if (connect(sockfd,  (struct sockaddr *) &servaddr,sizeof(servaddr)) < 0 ) {
                 perror("connect"); exit(4); 
        }


        /* Read the date/time from socket */
        
        while(1){
                printf("ssh >");
                scanf("%s", comm);
                if(strcmp(comm, "exit") == 0){
                        close(sockfd);
                        break;
                }
                if(strcmp(comm, "ls") == 0 || strcmp(comm, "pwd") == 0){
                        write(sockfd, comm, strlen(comm));
                }else{
                        printf("Invalid Command\n");
                        continue;
                }
                while ((n = read(sockfd, recvline, 100)) > 0){
                        recvline[n] = '\0';        /* null terminate */
                        printf("%s\n", recvline); 
                        break;
                } 
                if (n < 0) { perror("read"); exit(5); }
        }         
} 