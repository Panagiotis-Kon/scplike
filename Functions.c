/**************************************************************************
Arxeio Ylopoihshs	: DataFunctions.c
Syggrafeas			: P. Kontopoulos (Mathima Leitourgika)
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#include "Header.h"

int remove_dir(char *dirname, int removedir){

   DIR *dir;
   struct dirent *dirent;
   int type;

   if ( ( dir = opendir( dirname ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",dirname);
         return -1;
     }
   while ( ( dirent=readdir(dir) ) != NULL )
     {   if (dirent->d_ino == 0 ) continue;
          if ((strcmp(dirent->d_name,".") != 0) && (strcmp(dirent->d_name,"..") != 0))
            {   type = find_type(dirname, dirent->d_name); //type = 0  directory encountered  |||  type = 1  file encountered

                 char newname[255];
                 strcpy(newname, naming(dirname, dirent->d_name));

                 if (type == -1)
                   {   continue;
                   }
                 else if (type == 0) // if directory then call remove_dir
                   {   if (remove_dir(newname, 1) != 0)
                          {   perror(newname);
                               continue; 
                          }
                   }
                 else // if file then remove
                   {   if (remove(newname) != 0)
                          {   perror(newname);
                               continue; 
                          }
                   }
            }
     }
   closedir(dir);
   if ((remove(dirname) != 0) && (removedir == 1))
     {   perror(dirname);
          return -1;
     }
   return 0;
}

void reading(char source[], pointer_thread_variables global_thread_variables, int newsock, pthread_mutex_t *mtxclient, int num_of_files[]){

   DIR *dir_source;
   struct dirent *direntsource;
   struct dirent *entry = NULL;
   int type_s, i;
   long name_max;

   name_max = pathconf(source,_PC_NAME_MAX);
   entry = malloc(offsetof(struct dirent, d_name) + name_max + 1);
   if ( ( dir_source = opendir( source ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",source);
         exit(-1);
     }

   while (1)
     {   readdir_r(dir_source, entry, &direntsource);
          if ( direntsource == NULL ) break;
          if (direntsource->d_ino == 0 ) continue;
          if ((strcmp(direntsource->d_name,".") != 0) && (strcmp(direntsource->d_name,"..") != 0))
            {   type_s = find_type(source, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 if (type_s == -1)
                   {   continue;
                   }
                 char newname_s[256];
                 for (i=0;i<256;i++)
                   newname_s[i] = '\0';
                 strcpy(newname_s, naming(source, direntsource->d_name));
/**************************************************************************************************/
                 if (type_s == 0)  //Idio onoma kai einai kai ta 2 directories
                   {   reading(newname_s, global_thread_variables, newsock, mtxclient, num_of_files);
                   }
                 else  //Idio onoma alla sto source einai file kai sto target directory
                   {   Job_Data temp_jD;
                        temp_jD.id = newsock;
                        temp_jD.last = num_of_files[0];
                        temp_jD.mtxq = mtxclient;
                        num_of_files[0] = num_of_files[0] - 1;
                        for (i=0;i<256;i++)
                          temp_jD.path[i] = '\0';
                        strcpy(temp_jD.path, newname_s);
                        produce(temp_jD);
                        //strcpy(buf, temp_jD.path);
                        //printf("temp : %s\n",temp_jD.path);
                        pthread_cond_broadcast(&(global_thread_variables->cond_nonempty));
                        usleep(300000);
                   }
/**************************************************************************************************/
            }
     }
   closedir(dir_source);

}

void counting(char source[], const Queue_Header ouraP, int newsock, int num_of_files[]){

   DIR *dir_source;
   struct dirent *direntsource;
   struct dirent *entry = NULL;
   int type_s, i;
   long name_max;

   name_max = pathconf(source,_PC_NAME_MAX);
   entry = malloc(offsetof(struct dirent, d_name) + name_max + 1);
   if ( ( dir_source = opendir( source ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",source);
         exit(-1);
     }

   while (1)
     {   readdir_r(dir_source, entry, &direntsource);
          if ( direntsource == NULL ) break;
          if (direntsource->d_ino == 0 ) continue;
          if ((strcmp(direntsource->d_name,".") != 0) && (strcmp(direntsource->d_name,"..") != 0))
            {   type_s = find_type(source, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 if (type_s == -1)
                   {   continue;
                   }
                 char newname_s[256];
                 for (i=0;i<256;i++)
                   newname_s[i] = '\0';
                 strcpy(newname_s, naming(source, direntsource->d_name));
/**************************************************************************************************/
                 if (type_s == 0)  //Idio onoma kai einai kai ta 2 directories
                   {   counting(newname_s, ouraP, newsock, num_of_files);
                   }
                 else  //Idio onoma alla sto source einai file kai sto target directory
                   {   Job_Data temp_jD;
                        temp_jD.id = newsock;
                        for (i=0;i<256;i++)
                          temp_jD.path[i] = '\0';
                        strcpy(temp_jD.path, newname_s);
                        //printf("temp : %s\n",temp_jD.path);
                        num_of_files[0] = num_of_files[0] +1;
                   }
/**************************************************************************************************/
            }
     }
   closedir(dir_source);

}

void clearing(char source[], char target[]){

   DIR *dir_source, *dir_target;
   struct dirent *direntsource, *direnttarget;
   int type_s, type_t;

   if ( ( dir_source = opendir( source ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",source);
         exit(-1);
     }
   while ( ( direntsource=readdir(dir_source) ) != NULL )
     {   if (direntsource->d_ino == 0 ) continue;
          if ((strcmp(direntsource->d_name,".") != 0) && (strcmp(direntsource->d_name,"..") != 0))
            {   type_s = find_type(source, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 if (type_s == -1)
                   {   continue;
                   }
                 type_t = find_type(target, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered
                 if (type_t == -1)
                   {   char newname[255];
                        strcpy(newname, naming(source, direntsource->d_name));
                        if (type_s == 0)
                          {   if (remove_dir(newname, 1) != 0) //remove the directory if not exists in source
                                 {   perror(newname);
                                      continue; 
                                 }
                          }
                        if (type_s == 1) //remove the file if not exists in source
                          {   if (remove(newname) != 0)
                                 {   perror(newname);
                                      continue; 
                                 }
                          }
                   }
                 else
                   {   if (type_s == 0) //search subdirectories if not exist in source
                          {   char newname_s[255], newname_t[255];
                               strcpy(newname_s, naming(source, direntsource->d_name));
                               strcpy(newname_t, naming(target, direntsource->d_name));
                               clearing(newname_s, newname_t);
                          }
                   }
            }
     }
   closedir(dir_source);

}

