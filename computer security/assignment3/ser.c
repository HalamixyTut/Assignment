#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>

int main(int argc, char **argv) {
        int   listenfd, connfd, clilen, n;
        struct sockaddr_in servaddr, cliaddr;
        char buff[100], rec[20], dirres[100], cwd[100];
        time_t ticks;

        if(argc!=2){
	        printf("Usage : ./serv <Port>");
		exit(1); 
	}

        /* Create a TCP socket */
        listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize server's address and well-known port */
	bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(atoi(argv[1]));   /* daytime server */

	/* Bind server�s address and port to the socket */
        bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));       
	/* Convert socket to a listening socket � max 100 pending clients*/
        listen(listenfd, 100); 
   

   for ( ; ; ) {
      /* Wait for client connections and accept them */
	clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        while(1){
                if((n = read(connfd, rec, 20)) > 0){
                        if(strcmp(rec, "ls") == 0){
                                DIR * dir = opendir("./");
                                if(dir == NULL){
                                        write(connfd, "open failed!\n", 12);
                                }
                                struct dirent * dirp;
                                memset(dirres, 0, sizeof(dirres));
                                while(1){
                                        dirp = readdir(dir);
                                        if(dirp == NULL) {
                                                break;
                                        }
                                        if(!strcmp(dirp->d_name, ".") == 0 && !strcmp(dirp->d_name, "..") == 0){
                                                strcat(dirres, dirp->d_name);
                                                strcat(dirres, "\n");   
                                        }   
                                }
                                write(connfd, dirres, strlen(dirres));
                                memset(dirres, 0, sizeof(dirres));
                                memset(rec, 0, sizeof(rec));
                        }else if(strcmp(rec, "pwd") == 0){
                                getcwd(cwd, 100);
                                write(connfd, cwd, strlen(cwd));
                                memset(cwd, 0 ,sizeof(cwd));
                                memset(rec, 0, sizeof(rec));
                        }
                }
        }
      close(connfd);
   }
}

