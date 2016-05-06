/**************************************************************************
Arxeio Ylopoihshs	: ReadWrite.c
Syggrafeas			: P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos (stackoverflow)
***************************************************************************/
#include  "Header.h"
#define MessagePrint 0

int read_all_buffer(int fd, void *buffer, int nbyte){

   int count, i=0, max = nbyte;
   while (nbyte > 0)
     {   count = read(fd,buffer + i,1);
          if (count < 0)
            {   perror("Read Error");
                 //return -1;
            }
          else if (count == 1)
            {   nbyte = nbyte - count;
                 i++;
            }
     }
   if (MessagePrint)
     printf("Done: %s\n",(char *)buffer);
   return max;
}

int write_all_buffer(int fd, void *buffer, int nbyte){

   int count, i=0, max = nbyte;
   while (nbyte > 0)
     {   count = write(fd,buffer + i,1);
          if (count < 0)
            {   perror("Write Error");
                 //return -1;
            }
          else if (count == 1)
            {   nbyte = nbyte - count;
                 i++;
            }
     }
   if (MessagePrint)
     printf("Done: %s\n",(char *)buffer);
   return max;
}

/********************Functions from stackoverflow******************************************************/

ssize_t read_all_buffer1(int fd, void *buffer, size_t nread){

   ssize_t count = 0;
   ssize_t ret;

   while (count < nread) 
     {   ret = read(fd,  buffer + count, nread - count);
          if (ret > 0)
            {   count += ret;
            }
          else if(ret == 0 || (ret < 0 && errno != EINTR))
            {   break;
            }
     }
   return count;
}

ssize_t write_all_buffer1(int fd, void *buffer, size_t nwrite){

   ssize_t count = 0;
   ssize_t ret;

   while (count < nwrite) 
     {   ret = write(fd,  buffer + count, nwrite - count);
          if (ret > 0)
            {   count += ret;
            }
          else if(ret == 0 || (ret < 0 && errno != EINTR))
            {   break;
            }
     }
   return count;
}
