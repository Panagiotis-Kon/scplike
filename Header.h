/**************************************************************************
Arxeio Ylopoihshs	: Header.h
Syggrafeas			: P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/

#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */		
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <string.h>    /* For strerror */
#include <pthread.h>   /* For threads  */
#include  <fcntl.h>
#include  <stddef.h>

#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/times.h>

#include "QueueData.h"
#include "QueueHeader.h"
#define perror2(s,e) fprintf(stderr, "%s: %s\n", s, strerror(e))

typedef struct thread_variables *pointer_thread_variables;

typedef struct thread_variables{
        pthread_mutex_t mtx;
        pthread_cond_t cond_nonempty;
        pthread_cond_t cond_nonfull;
} thread_variables;

typedef struct thread_args{
        int     newsock;  // newsock = socket of client
        pthread_mutex_t *mtxq;
} thread_args;

/********************Declaration of Functions and DataFunctions******************************************/
char *naming(char dirname[], char name[]);
void printing(char name[]);
mode_t data_info(char data[], int verbose, int statistics[3]);
int find_type(char dirname[], char name[]);
int remove_dir(char *dirname, int removedir);
void reading(char source[], pointer_thread_variables global_thread_variables, int newsock, pthread_mutex_t *mtxclient, int *num_of_files);
void counting(char source[], const Queue_Header ouraP, int newsock, int *num_of_files);
void clearing(char source[], char target[]);

/********************Declaration of ReadWriteFunctions*************************************************/
int read_all_buffer(int fd, void *buffer, int nbyte);
int write_all_buffer(int fd, void *buffer, int nbyte);
ssize_t read_all_buffer1(int fd, void *buffer, size_t nread);
ssize_t write_all_buffer1(int fd, void *buffer, size_t nwrite);

