/**************************************************************************
Arxeio Ylopoihshs	: dataServer.c
Syggrafeas			: P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#include "Header.h"

/********************Declaration of Global Variables*****************************************************/

Queue_Header FileQueue;
int t_option = 1, t_socket;
pointer_thread_variables global_thread_variables;
#define MaxThreadPoolSize 0

/********************Declaration of Signal Handler******************************************************/

void sigint_handler(){
   //printf("SIGINT received\n");
   t_option = 0; // set to 0 so that worker threads and main break their loop
   close(t_socket);
   Queue_katastrofi(&FileQueue);
   pthread_mutex_destroy(&(global_thread_variables->mtx));
   pthread_cond_destroy(&(global_thread_variables->cond_nonempty));
   pthread_cond_destroy(&(global_thread_variables->cond_nonfull));
   free(global_thread_variables);
   printf("\nServer is shutting down.\n");
   exit(0);
}

/********************Declaration of Functions**********************************************************/
void perror_exit(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
void produce(Job_Data temp);
void consume(int *argp);

void *p_thread_f(void *argp){ /* Producer Thread function */

   thread_args *arguments = argp;
   int newsock = (int) arguments->newsock;
   pthread_mutex_t *mtxclient = (pthread_mutex_t *) arguments->mtxq;
   int err;
   if (err = pthread_detach(pthread_self())) {/* Detach thread */
       perror2("pthread_detach", err);
       exit(1); 
       }

   char buf[256];
   int i, j=0;
   for (i=0;i<256;i++)
     buf[i] = '\0';
   if (read_all_buffer1(newsock, &i, sizeof(int)) < 0)
     perror_exit("Producer read size of path from socket");
   //printf("i: %d\n",i);

   if (read_all_buffer1(newsock, buf, i) < 0)
     {   perror_exit("Producer read path from socket");
     }

   printf("[Thread: %lu]: About to scan directory %s\n", pthread_self(), buf);
   int num_of_files[1];
   num_of_files[0] = 0;
   counting(buf, FileQueue, newsock, num_of_files);
   //printf("Num Of Files : %d\n",num_of_files[0]);
   if (write_all_buffer1(newsock, num_of_files, sizeof(int)) < 0)
     perror_exit("Producer write number of files to socket");
   //printf("Write number of files\n");
   reading(buf, global_thread_variables, newsock, mtxclient, num_of_files);
   free(arguments);
   pthread_exit(NULL);
}

void *c_thread_f(void *argp){ /* Consumer Thread function */
   //printf("I am the newly created thread %lu\n", pthread_self());
   int * args = (int *) argp;
   int newsock = (int) args[0], pos = (int) args[1], thread_pool_size = (int) args[2];
   int err;
   //printf("args %d   %d   %d\n", newsock, pos, thread_pool_size);
   if (err = pthread_detach(pthread_self())) {/* Detach thread */
       perror2("pthread_detach", err);
       exit(1); 
       }
   while (t_option)
     {   consume(args);
          pthread_cond_broadcast(&(global_thread_variables->cond_nonfull));
          usleep(500000);
     }
   printf("[Thread: %lu]: Consumer is shutting down.\n", pthread_self());
   pthread_exit(NULL);
}

/***************************************************************************************************/

int main(int argc, char *argv[]) {
   int             port, sock, newsock, i, thread_pool_size, queue_size, hostent_error, j, hostent_return;
   struct sockaddr_in server, client;
   socklen_t clientlen;
   struct sockaddr *serverptr=(struct sockaddr *)&server;
   struct hostent *rem, *addrreturn;
   char hostent_buf[256];
   for(j=0;j<256;j++)
     hostent_buf[j] = '\0';
   rem  = malloc(sizeof(struct hostent));
   addrreturn = malloc(sizeof(struct hostent));
   global_thread_variables = malloc(sizeof(thread_variables));
/********************Signal Handler***************************************************************/
   static struct sigaction actT;
   sigset_t block_mask;
   sigemptyset(&block_mask);
   sigaddset(&block_mask,SIGINT);
   actT.sa_handler = sigint_handler;
   //sigfillset(&(actU.sa_mask));
   actT.sa_mask = block_mask;
   actT.sa_flags = SA_RESTART;
   sigaction(SIGINT, &actT, NULL);
/********************Reading Arguments***************************************************************/
   //printf("Argc = %d\n", argc);
   if (argc != 7)
     {   printf("Please give port number, thread pool size and queue size\n");
          exit(1);
     }
   printf("\nServer's parameters are:\n");
   for (i=0;i<argc-4;i++)
     {   if (!strcmp(argv[1+(2*i)], "-p"))
            {   port = atoi(argv[2+(2*i)]);
                 if (port <= 0)
                   {   printf("Invalid port. \n");
                        return 1;
                   }
            }
          else if (!strcmp(argv[1+(2*i)], "-s"))
            {   thread_pool_size = atoi(argv[2+(2*i)]);
                 if (thread_pool_size <= 0)
                   {   printf("Thread pool size out of Limits. \n");
                        return 1;
                   }
                 if (MaxThreadPoolSize)
                   if (thread_pool_size > 50)
                     {   printf("Thread pool size out of Limits. \n");
                          return 1;
                     }
            }
          else if (!strcmp(argv[1+(2*i)], "-q"))
            {   queue_size = atoi(argv[2+(2*i)]);
                 if (queue_size <= 0)
                   {   printf("Queue size out of Limits. \n");
                        return 1;
                   }
             }
          else
             {   printf("Wrong argument!\n.");
                  return 1;
             }
   }
   printf("        port: %d\n",port);
   printf("        thread_pool_size: %d\n",thread_pool_size);
   printf("        queue_size: %d\n",queue_size);
/********************Opening Ports*******************************************************************/
   /* Create socket */
   if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
       perror_exit("socket");
   server.sin_family = AF_INET;       /* Internet domain */
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(port);      /* The given port */
   /* Bind socket to address */
   if (bind(sock, serverptr, sizeof(server)) < 0)
       perror_exit("bind");
   t_socket = sock;
/********************Initialize Queue******************************************************************/
   Queue_dimiourgia(&FileQueue);
   Queue_set_Pool_Size(FileQueue, thread_pool_size);
   Queue_set_Queue_Size(FileQueue, queue_size);
/********************Initialize Worker Threads, Mutex and Condition Varaibles*******************************/
   pthread_mutex_init(&(global_thread_variables->mtx), 0);
   pthread_cond_init(&(global_thread_variables->cond_nonempty), 0);
   pthread_cond_init(&(global_thread_variables->cond_nonfull), 0);
   pthread_t *worker;
   int error, *args;

   if ((worker = malloc(thread_pool_size * sizeof(pthread_t))) == NULL) {
     perror("malloc");  exit(1); }
   for (i=0;i<thread_pool_size;i++)
     {   args =  malloc(sizeof(int)*3);
          args[0] = -1;
          args[1] = i;
          args[2] = thread_pool_size;
          //printf("sendind args %d   %d   %d\n", (int) args[0], (int) args[1], (int) args[2]);
          if (error = pthread_create(worker+i, NULL, c_thread_f, (void *) args)) { /* New thread */
            perror2("pthread_create", error);
            exit(1);
          }
     }
   printf("Server was successfully initialized...\n");
/********************Listen for Connections************************************************************/
   /* Listen for connections */
   if (listen(sock, SOMAXCONN) < 0) perror_exit("listen");
   printf("Listening for connections to port %d\n", port);
   while (t_option) { struct sockaddr *clientptr=(struct sockaddr *)&client;
       clientlen = sizeof(client);
       /* accept connection */
       if ((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept");
       /* Find client's address */
    	if ((hostent_return = gethostbyaddr_r( &(client.sin_addr.s_addr), sizeof(client.sin_addr.s_addr), client.sin_family, rem, hostent_buf, 256, &addrreturn, &hostent_error)) == -1) {
   	    herror("gethostbyaddr"); exit(1);}
    	printf("Accepted connection from %s\n", rem->h_name);
    	//printf("Accepted connection\n");
        pthread_t *thr;
        int n = 1;
        thread_args *arguments;
        arguments = malloc(sizeof(thread_args));
        pthread_mutex_t *mtxclient = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(mtxclient, 0);
        if ((thr = malloc(n * sizeof(pthread_t))) == NULL) {
          perror("malloc");  exit(1); }
        int err, status, status2;
        arguments->newsock = newsock;
        arguments->mtxq = mtxclient;
        if (err = pthread_create(thr+0, NULL, p_thread_f, (void *) arguments)) { /* New thread */
          perror2("pthread_create", err);
          exit(1);
        }
  //printf("\nI am original thread %lu and I created thread %lu\n", pthread_self(), *thr);
  //if (err = pthread_join(*(thr+0), (void **) &status)) { /* Wait for thread */
      //perror2("pthread_join", err); /* termination */
      //exit(1);
      //}
  //printf("Thread %lu exited with code %d\n", *(thr+0), status);
  //printf("Thread %lu just before exiting (Original)\n", pthread_self());
        //child_server(newsock);
    	//close(newsock); /* parent closes socket to client */
    }
   printf("[Thread: %lu]: Server is shutting down.\n", pthread_self());
   int err, status, status2;
   for (i=0;i<thread_pool_size;i++){
      if (err = pthread_join(*(worker+i), (void **) &status)) { /* Wait for thread */
        perror2("pthread_join", err); /* termination */
        exit(1);
      }
   }
   //close(sock);
   //Queue_katastrofi(&FileQueue);
   //pthread_exit(NULL);
   return 0;
}

void produce(Job_Data temp) {

   pthread_mutex_lock(&(global_thread_variables->mtx));
   while (Queue_full(FileQueue))
     {   //printf(">> Found Buffer Full \n");
          pthread_cond_wait(&(global_thread_variables->cond_nonfull), &(global_thread_variables->mtx));
     }
   Queue_eisagogiTelos(FileQueue, temp);
   Queue_periexomenoTrexon(FileQueue, &temp);
   //strcpy(buf, temp_jD.path);
   printf("[Thread: %lu]: Adding file %s to the queue...\n", pthread_self(), temp.path);
   pthread_mutex_unlock(&(global_thread_variables->mtx));

   //printf("Closing connection.\n");
   //close(newsock);  /* Close socket */
}

void consume(int argp[]) {

   int filedes_s;
   ssize_t nread;
   Job_Data temp_jD;
   int i=0, pos, size_f;
   char buffer[1];
   struct stat statbuf;
   char buf[256];
   for (i=0;i<256;i++)
     buf[i] = '\0';

   buffer[0] = '\0';
   temp_jD.id = 0;
   pthread_mutex_lock(&(global_thread_variables->mtx));
   while (Queue_keni(FileQueue))
     {   //printf(">> Found Buffer Empty \n");
          pthread_cond_wait(&(global_thread_variables->cond_nonempty), &(global_thread_variables->mtx));
     }
   for (i=0;i<256;i++)
     temp_jD.path[i] = '\0';
   Queue_prwtos(FileQueue);
   Queue_periexomenoTrexon(FileQueue, &temp_jD);
   Queue_exagogiArxh(FileQueue);
   printf("[Thread: %lu]: Received task: <%s, %d>\n", pthread_self(), temp_jD.path, temp_jD.id);
   pthread_mutex_unlock(&(global_thread_variables->mtx));
   pthread_mutex_lock(temp_jD.mtxq);
   i = strlen(temp_jD.path);
   //if (write_all_buffer1(temp_jD.id, &i, sizeof(int)) < 0)
     //perror_exit("Server write");
   //printf("Write length of buffer to send\n");

   strcpy(buf,temp_jD.path);
   if (write_all_buffer1(temp_jD.id, buf, sizeof(buf)) < 0)
     perror_exit("Consumer->consume write path to socket");
   //printf("Write buffer\n");
   if ((filedes_s = open(temp_jD.path, O_RDONLY)) < 0)
     {   perror("Consumer->consume failed to open the file.\n");
          exit(-1);
     }

   if (stat(temp_jD.path, &statbuf) < 0)
     {   perror(temp_jD.path);
     }
   size_f = (int)statbuf.st_size;
   if (write_all_buffer1(temp_jD.id, &size_f, sizeof(int)) < 0)
     perror_exit("Consumer->consume write file size to socket");
   //printf("File Size %d\n", size_f);
   //printf("Write size of file\n");

   size_t serverpagesize;
   serverpagesize = (size_t)sysconf(_SC_PAGESIZE);
   if (write_all_buffer1(temp_jD.id, &serverpagesize, sizeof(size_t)) < 0)
     perror_exit("Consumer->consume write server pagesize to socket");
   //printf("Page Size %d\n", (int) serverpagesize);
   //printf("Write size of page\n");

/********************Send file through socket***********************************************************/
   char *s_buffer;
   int rest;
   if (size_f > 0 && serverpagesize > 0)
     rest = size_f/serverpagesize;
   if (rest == 0 || size_f%serverpagesize > 0)
     rest++;
/* If file size is bigger or smaller than page size then rest + 1 */
   //char temp;
   //if (read_all_buffer1(temp_jD.id, &temp, sizeof(char)) < 0)
   //  perror_exit("Server write");

   int k = 0, send =0;
   /*read_all_buffer1(temp_jD.id, &send, sizeof(int));
   printf("ready to send %d\n",send);
   write_all_buffer1(temp_jD.id, &send, sizeof(int));
   printf("ready to receive %d\n",send);
   read_all_buffer1(temp_jD.id, &send, sizeof(int));
   printf("ready to send %d\n",send);*/

   size_t nwrite, remaining_size = size_f, temp_size = 0;
   printf("[Thread: %lu]: About to read file %s\n", pthread_self(), temp_jD.path);
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
       nread = read_all_buffer1(filedes_s, s_buffer, temp_size);
       k = k + 1;
       //printf("read %d\n",k);
       //if (nread > 0)
          nwrite = write_all_buffer1(temp_jD.id, s_buffer, temp_size);
       //printf("send %d   ,    %d\n",k, k > size_f);
       free(s_buffer);
       //nread = read_all_buffer1(temp_jD.id, &send, sizeof(int));
     }
   //printf("Write file\n");
   close(filedes_s);
   //if (read_all_buffer1(temp_jD.id, &i, sizeof(int)) < 0)
     //{   perror("Server Worker Thread Read\n");
         //exit(-1);
     //}
   //if (read_all_buffer1(temp_jD.id, &buf, sizeof(buf)) < 0)
    //{   perror("Server Worker Thread Read\n");
        //exit(-1);
    //}
/***************************************************************************************************/
   pthread_mutex_unlock(temp_jD.mtxq);
   //pthread_mutex_unlock(&(global_thread_variables->mtx));
   if (temp_jD.last == 1)
     {   printf("[Thread: %lu]: Connection with socket %d closed\n", pthread_self(), temp_jD.id);
          close(temp_jD.id);
          free(temp_jD.mtxq);
     }
}


