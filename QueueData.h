/**************************************************************************
Arxeio Ylopoihshs	: QueueData.h
Syggrafeas			: Y. Cotronis (Mathima Domes Dedomenon), P. Kontopoulos
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#ifndef __Job_Data_H__
#define __Job_Data_H__

#include <pthread.h>   /* For threads  */

typedef struct Job_Data{
        int     id;  // jod id = socket of client
        int last; // 1 : if it is the last file to be transfered
        pthread_mutex_t *mtxq;
        char    path[256];  // file name
} Job_Data;



int Queue_setValue (Job_Data *target, Job_Data source);
int Queue_printValue (Job_Data Elem);
int Queue_readValue (int fd,  Job_Data *Elem);
int Queue_writeValue (int fd, Job_Data Elem);

int Queue_iso(Job_Data Elem1, Job_Data Elem2);

#endif
