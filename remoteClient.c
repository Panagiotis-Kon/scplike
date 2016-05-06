/**************************************************************************
Arxeio Ylopoihshs	: remoteClient.c
Syggrafeas			: P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#include "Header.h"

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void writing(char source[], int sock, int file_size, int serverpagesize);

int main(int argc, char *argv[]){

   int             port, sock, i, j;
   char            buf[256], hostname[256], *path;
   for (i=0;i<256;i++)
     buf[i] = '\0';
   for (i=0;i<256;i++)
     hostname[i] = '\0';
   struct sockaddr_in server;
   struct sockaddr *serverptr = (struct sockaddr*)&server;
   struct hostent *rem;
   if (argc != 7)
     {   printf("Please give server ip address, server port number and directory\n");
          exit(1);
     }
   printf("\nClient's parameters are:\n");
   for (i=0;i<argc-4;i++)
     {   if (!strcmp(argv[1+(2*i)], "-p"))
            {   port = atoi(argv[2+(2*i)]); /*Convert port number to integer*/
                 if (port <= 0)
                   {   printf("Invalid port. \n");
                        return 1;
                   }
            }
          else if (!strcmp(argv[1+(2*i)], "-i"))
            {   strcpy(hostname, argv[2+(2*i)]);
            }
          else if (!strcmp(argv[1+(2*i)], "-d"))
            {   //printf("%d\n",strlen(argv[2+(2*i)]));
                 int k = strlen(argv[2+(2*i)]);
                 path = (char *) malloc(k*sizeof(char));
                 if (path == NULL)
                   {   perror("Malloc Failed");
                        exit(1);
                   }
                 strcpy(path, argv[2+(2*i)]);
             }
          else
             {   printf("Wrong argument!\n.");
                  return 1;
             }
   }
   printf("        serverIP: %s\n",hostname);
   printf("        port: %d\n",port);
   printf("        directory: %s\n",path);
/********************Opening Ports*******************************************************************/
   /* Create socket */
   if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
     perror_exit("socket");
   /* Find server address */
   if ((rem = gethostbyname(hostname)) == NULL)
     {   herror("gethostbyname"); exit(1);
     }
   server.sin_family = AF_INET;       /* Internet domain */
   memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
   server.sin_port = htons(port);         /* Server port */
   /* Initiate connection */
   if (connect(sock, serverptr, sizeof(server)) < 0)
     perror_exit("connect");
   printf("Connecting to %s port %d\n", hostname, port);
   i = strlen(path);
   //printf("Size send: %d\n", i);
   if (write_all_buffer1(sock, &i, sizeof(int)) < 0)
     perror_exit("Client write size of path to socket");
   if (write_all_buffer1(sock, path, i) < 0)
     perror_exit("Client write path to socket");
   /* receive i-th character transformed */
   int max = 0;
   if (read_all_buffer1(sock, &max, sizeof(int)) < 0)
     perror_exit("Client read number of files from socket");
   //printf("Read number of files %d\n",max);

/********************Receive files********************************************************************/
   for (j=0;j<max;j++)
     {   //printf("%d file out of %d\n",j+1, max);
          //if (read_all_buffer1(sock, &i, sizeof(int)) < 0)
            //perror_exit("read");
            //printf("Read length of buffer to send %d\n",i);

         if (read_all_buffer1(sock, buf, sizeof(buf)) < 0)
           perror_exit("Client read path of file from socket");
         //printf("Read buffer %s\n",buf);

         int file_size =-1;
         if (read_all_buffer1(sock, &file_size, sizeof(int)) < 0)
           perror_exit("Client read file size from socket");
         //printf("Read size of file %d\n",file_size);

         size_t serverpagesize;
         serverpagesize = (size_t)sysconf(_SC_PAGESIZE);
         if (read_all_buffer1(sock, &serverpagesize, sizeof(size_t)) < 0)
           perror_exit("Client read server pagesize from socket");
         //printf("Page Size %d\n", (int) serverpagesize);
         //printf("Read size of page\n");
         writing(buf, sock, file_size, serverpagesize);
         //strcpy(buf,"OK");
         //i = strlen(buf) + 1;
         //if (write_all_buffer1(sock, &i, sizeof(int)) < 0)
           //perror_exit("write");
         //if (write_all_buffer1(sock, buf, sizeof(buf)) < 0)
          //perror_exit("write");
         for (i=0;i<256;i++)
           buf[i] = '\0';
   }
   printf("\nClosing Connection\n\n");
   close(sock);                 /* Close socket and exit */
   free(path);
   return 0;
}

void writing(char source[], int sock, int file_size, int serverpagesize){

   struct stat statbuf;
   int type_s, i, count=0, length=0, j=0;
   char buf[256];
   for (i=0;i<256;i++)
     {   buf[i] = '\0';
     }
   //printf("source : %s\n",source);
   i = 0;
   while (source[i] != '\0')
     {  length++;
         if (source[i] == '/')
           count++;
         i++;
     }
   //printf("%d\n",count);
   //strcpy(buf, "./Rec")
   while (buf[i] != '\0')
     {  count++;
     }
   //printf("106 buf : %s length : %d\n",buf, length);
   for (i=0;i<=length;i++)
     {   buf[i] = source[i];
          if (buf[i] == '/')
            {   //printf("buf : %s\n",buf);
                 if (stat( buf, &statbuf) == -1)
                   {  if (mkdir(buf, 0755) != 0)
                         {   printf("%s\n",strerror(errno));
                              exit(-1);
                         }
                       //printf("created directory %s \n", buf);
                   }
                 else
                   {   //printf("folder exists\n",buf);
                        j++;
                   }
            }
          else if (buf[i] == '\0')
            {   //printf("120 buf : %s\n",buf);
                 if (stat( buf, &statbuf) == -1)
                   {   //printf("no file %s \n", buf);
                        j--;
                   }
                 else if (remove(buf) != 0)
                   {   perror(buf);
                       exit(-1);
                   }
                 //printf("remove file %s \n", buf);
                 int filedes_s;
                 ssize_t nread, nwrite;
                 char buffer[1];
                 if ((filedes_s= open(source, O_RDWR | O_CREAT, 0755)) < 0)
                   {   perror("Failed to open the file.\n");
                        exit(-1);
                   }
/********************Read file from socket*************************************************************/
                 char *s_buffer;
                 int rest = file_size/serverpagesize;
                 //printf("rest %d\n",rest);
                 if (file_size > 0 && serverpagesize > 0)
                   rest = file_size/serverpagesize;
                 if (rest == 0 || file_size%serverpagesize > 0)
                   rest++;
                 //printf("t %d\n",file_size%serverpagesize);
                 //printf("rest %d\n",rest);
                 int k = 0, send =0;
                 /*write_all_buffer1(sock, &send, sizeof(int));
                 printf("ready to receive %d\n",send);
                 read_all_buffer1(sock, &send, sizeof(int));
                 printf("ready to send %d\n",send);
                 write_all_buffer1(sock, &send, sizeof(int));
                 printf("ready to receive %d\n",send);*/

                 size_t remaining_size = file_size, temp_size = 0;
                 while(k < rest)
                   {   if (remaining_size <= serverpagesize)
                          {   temp_size = remaining_size;
                          }
                        else
                          {   temp_size = serverpagesize;
                               remaining_size = remaining_size - serverpagesize;
                          }
                        s_buffer = malloc(temp_size*sizeof(char));
                        for (i=0;i<temp_size;i++)
                          s_buffer[i] = '\0';
                        nread = read_all_buffer1(sock, s_buffer, temp_size);
                        k = k + 1;
                        //printf("read %d\n",k);
                        //if (nread > 0)
                        nwrite = write_all_buffer1(filedes_s, s_buffer, temp_size);
                        //printf("send %d   ,    %d\n",k, k > file_size);
                        free(s_buffer);
                        //nread = read_all_buffer1(temp_jD.id, &send, sizeof(int));
                   }
                 close(filedes_s);
                 printf("Received: %s#%d#%d\n",source, file_size, serverpagesize);
                 //printf("created file %s \n", buf);
            }
     }
}

