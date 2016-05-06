/**************************************************************************
Arxeio Ylopoihshs	: QueueData.c
Syggrafeas			: Y. Cotronis (Mathima Domes Dedomenon), P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include "QueueData.h"
#define BUFSIZE 64

int Queue_setValue (Job_Data *target, Job_Data source){
	
  if (&source == NULL)
    { //printf("No data to set to target.\n"); 
      return 0;
    }
  else
    {   target->id = source.id;
         target->last = source.last;
         target->mtxq = source.mtxq;
          strcpy(target->path, source.path);
          return 1;
/* set data from source to target */
    }

}

int Queue_printValue(Job_Data ElemPtr){

   /*int i;
   printf("%d %s ",ElemPtr.id,ElemPtr.job);
   for (i=0;i<10;i++)
     {   if (ElemPtr.arguments[i][0] != '\0')
            printf("%s ",ElemPtr.arguments[i]);
          else
            break;
     }
   printf("%s\n",ElemPtr.status);
   return 0;
/* print data */
}

int Queue_readValue (int fd, Job_Data *ElemPtr){

  /*int value, args;
  int i;
  char buffer[BUFSIZE];
  for (i=0;i<10;i++)
    {   ElemPtr->arguments[i][0] = '\0';
    }
  read(fd,&args,sizeof(int));
  read(fd,&value,sizeof(int));
  ElemPtr->id = value;
  read(fd,buffer,BUFSIZE);
  strcpy(ElemPtr->job,buffer);
   for (i=0;i<args;i++)
     {   read(fd,buffer,BUFSIZE);
          strcpy(ElemPtr->arguments[i],buffer);
          //printf("%s\n",ElemPtr->arguments[i]);
     }
  read(fd,buffer,BUFSIZE);
  strcpy(ElemPtr->status,buffer);
  return 0;
/* read data from pipe with fd */
}

int Queue_writeValue(int fd, Job_Data ElemPtr){

   /*int i;
   int value = 0;
   char buffer[BUFSIZE];
   while (ElemPtr.arguments[value][0] != '\0')
     value++;
   write(fd,&value,sizeof(int));
   value = ElemPtr.id;
   write(fd,&value,sizeof(int));
   strcpy(buffer,ElemPtr.job);
   write(fd,buffer,BUFSIZE);
   for (i=0;i<10;i++)
     {   if (ElemPtr.arguments[i][0] != '\0')
            {   strcpy(buffer,ElemPtr.arguments[i]);
                 write(fd,buffer,BUFSIZE);
            }
     }
   strcpy(buffer,ElemPtr.status);
   write(fd,buffer,sizeof(ElemPtr.status));
   return 0;
/* write data to pipe with fd */
}

int Queue_iso(Job_Data Elem1, Job_Data Elem2){

  return Elem1.id == Elem2.id;
/* If Elem1 = Elem2 then returns 1, if Elem1 < Elem2 then returns 0 and if Elem1 > Elem2 then returns 0 */
}
