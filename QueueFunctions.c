/**************************************************************************
Arxeio Ylopoihshs	: QueueFunctions.c
Syggrafeas			: Y. Cotronis (Mathima Domes Dedomenon), P. Kontopoulos
Skopos				: Ylopihsh ATD OuraPlus vs 
                      Dipla Syndedemenh Kyklikh Lista me deiktes
Ana8ewrhsh			: P. Kontopoulos
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "QueueHeader.h"

typedef struct typos_komvou *typos_deikti;

typedef struct typos_komvou{
	Job_Data dedomena;	/*ta dedomena*/
	typos_deikti    epomenos; 	    /*deikths ston epomeno komvo*/
	typos_deikti    proigoumenos; 	/*deikths ston prohgoymeno komvo*/
} typos_komvou;	   /*o typos toy kombou ths ouraPs*/


typedef struct GeneralNode{
        typos_deikti Head,   /* h arxh ths Queue (NULL if list empty) */
                     Last,   /* to telos ths Queues (NULL if list empty) */
                     Current;/* o trexon komvos ths Queue (NULL if list empty) */
        int Pool_Size; //Thread Pool Size
        int Queue_Size; //Queue Max Size
        int ListSize;
}GeneralNode;
/*dhlwseis synarthsewn*/

/*dhmioyrgia kai katastrofh ouraPs*/
void Queue_dimiourgia(Queue_Header * const ouraP){

  *ouraP = malloc(sizeof(GeneralNode));
  (*ouraP)->Head = NULL;
  (*ouraP)->Last = NULL;
  (*ouraP)->Current = NULL;
  (*ouraP)->Pool_Size = 1;
  (*ouraP)->Queue_Size = 1;
  (*ouraP)->ListSize = 0;

} /* initializes Current as NULL  ++++ */

void Queue_katastrofi(Queue_Header * const ouraP){

    if (Queue_keni(*ouraP))
      { free(*ouraP);
      }
    else
      { while (Queue_megethos(*ouraP) != 0)
          { Queue_diagrafiTrexon(*ouraP);
          }
        free(*ouraP);
      }

}

int Queue_set_Pool_Size(const Queue_Header ouraP, int n){

  ouraP->Pool_Size = n;
  return 0;

}

int Queue_set_Queue_Size(const Queue_Header ouraP, int n){

  ouraP->Queue_Size = n;
  return 0;

}

/*prakseis elegxoy*/
int	Queue_keni(const Queue_Header ouraP){

  return (ouraP->Head == NULL && ouraP->Last == NULL && ouraP->Current == NULL && ouraP->ListSize == 0);

}

int Queue_Pool_Size(const Queue_Header ouraP){

  return (ouraP->Pool_Size);

}

int Queue_Size(const Queue_Header ouraP){

  return (ouraP->Queue_Size);

}

int Queue_megethos(const Queue_Header ouraP){

  return (ouraP->ListSize);

}

int	Queue_full(const Queue_Header ouraP){

  return (ouraP->Queue_Size == ouraP->ListSize);

}

/*prakseis prosbashs-allazei to Current*/
int Queue_periexomenoTrexon(const Queue_Header ouraP, Job_Data * const value){

  if (Queue_keni(ouraP))
    { return 0; 
    }
  else
    { if (ouraP->Current == NULL)
        { //printf("Invalid Current !\n");
          return 0;
        }
      else
        { if (Queue_setValue (value, ouraP->Current->dedomena))
            { return 1; }
          else
            { //printf("Unsuccessful assigning value to variable.\n");
              return 0;
            }
        }
    }

} /* error if Current invalid */

int Queue_allagiTrexon(const Queue_Header ouraP, Job_Data stoixeio){

  typos_deikti prosorinos;
  if (ouraP->Current == NULL)
    { //printf("Invalid Current !\n");
      return 0;
    }
  else
    { prosorinos = ouraP->Current;
      if (Queue_setValue (&prosorinos->dedomena, stoixeio))
        { //printf("Successful change of current node !\n");
          return 1;
        }
      else
        { //printf("Unsuccessful change of current node !\n");
          return 0;
        }
    }

} /* error if Current invalid */

/*prakseis eisagwgh-diagrafh Current as prodeiktis */
int Queue_eisagogiTelos(const Queue_Header ouraP, Job_Data stoixeio){

  typos_deikti prosorinos;
  int error;
  error = 0;
  prosorinos = malloc(sizeof(typos_komvou));
  if (prosorinos == NULL)
    { error = 1;
      return 0;
      //printf("Error at memory allocation.\n");
    }
  if (error == 0)
    { if (Queue_keni(ouraP))
        { ouraP->Head = prosorinos;
          ouraP->Current = prosorinos;
          ouraP->Last = prosorinos;
          prosorinos->epomenos = prosorinos;
          prosorinos->proigoumenos = prosorinos;
          if (Queue_setValue (&prosorinos->dedomena, stoixeio))
            { ouraP->ListSize = ouraP->ListSize + 1;
              return 1;
            }
          else
            { return 0;
              //printf("Unsuccessful assigning value to variable.\n");
            }
        }
      else if (ouraP->ListSize <= ouraP->Queue_Size)
        { prosorinos->proigoumenos = ouraP->Last;
          prosorinos->epomenos = ouraP->Head;
          ouraP->Head->proigoumenos = prosorinos;
          ouraP->Last->epomenos = prosorinos;
          ouraP->Last = prosorinos;
          ouraP->Current = prosorinos;
          if (Queue_setValue (&prosorinos->dedomena, stoixeio))
            { ouraP->ListSize = ouraP->ListSize + 1;
              return 1;
            }
          else
            { return 0; 
              //printf("Unsuccessful assigning value to variable.\n");
            }
        }
    }
   return 0;
} /* sto telos */

int Queue_exagogiArxh(const Queue_Header ouraP){

  typos_deikti prosorinos;
  if (Queue_keni(ouraP))
        { return 0;
        }
      else
        { prosorinos = ouraP->Head;
          ouraP->Head = prosorinos->epomenos;
          if (ouraP->ListSize == 1)
            {  ouraP->Head = NULL;
               ouraP->Last = NULL;
               ouraP->Current = NULL;
               ouraP->ListSize = ouraP->ListSize - 1;
               free(prosorinos);
               return 1;
            }
          else
            {  ouraP->Head->proigoumenos = ouraP->Last;
               ouraP->Last->epomenos = ouraP->Head;
               ouraP->Current = ouraP->Head;
               ouraP->ListSize = ouraP->ListSize - 1;
               free(prosorinos);
               return 1;
            }
         }

}/*  sthn arxh  */ 

int Queue_diagrafiTrexon(const Queue_Header ouraP){

  typos_deikti prosorinos,temp;
  if (Queue_keni(ouraP))
    { //printf("List is empty,so no deletion took place !\n");
      return 0;
    }
  else
    { if (ouraP->ListSize == 1)
        {  prosorinos = ouraP->Current;
           ouraP->Head = NULL;
           ouraP->Last = NULL;
           ouraP->Current = NULL;
           ouraP->ListSize = ouraP->ListSize - 1;
           free(prosorinos);
           //printf("Successful deleting current node !\n");
           return 1;
        }
      else
        {  if (ouraP->Current == ouraP->Head)
             { prosorinos = ouraP->Current;
               Queue_epomenos(ouraP);
               ouraP->Head = ouraP->Head->epomenos;
               ouraP->Current->proigoumenos = prosorinos->proigoumenos;
               prosorinos->proigoumenos->epomenos = ouraP->Current;
               ouraP->ListSize = ouraP->ListSize - 1;
               free(prosorinos);
               //printf("Successful deleting current node !\n");
               return 1;
             }
           else if (ouraP->Current == ouraP->Last)
             { prosorinos = ouraP->Current;
               Queue_epomenos(ouraP);
               ouraP->Last = ouraP->Last->proigoumenos;
               ouraP->Current->proigoumenos = prosorinos->proigoumenos;
               prosorinos->proigoumenos->epomenos = ouraP->Current;
               ouraP->ListSize = ouraP->ListSize - 1;
               free(prosorinos);
               //printf("Successful deleting current node !\n");
               return 1;
             }
           else
             { prosorinos = ouraP->Current;
               Queue_epomenos(ouraP);
               ouraP->Current->proigoumenos = prosorinos->proigoumenos;
               prosorinos->proigoumenos->epomenos = ouraP->Current;
               ouraP->ListSize = ouraP->ListSize - 1;
               free(prosorinos);
               //printf("Successful deleting current node !\n");
               return 1;
             }
        }
    }

} /* diagrafei *trexonta* komvo apo apoydhpote */

/* Epilogh Kombou Current */

int Queue_epomenos(const Queue_Header ouraP){

  typos_deikti temp;
  if (Queue_keni(ouraP))
    { return 0; }
  else
    { temp = ouraP->Current;
      ouraP->Current = temp->epomenos;
      return 1;
    }

} /* if noNext no move + error */

int Queue_proigoymenos(const Queue_Header ouraP){

  typos_deikti temp;
  if (Queue_keni(ouraP))
    { return 0; }
  else
    { temp = ouraP->Current;
      ouraP->Current = temp->proigoumenos;
      return 1;
    }

} /* if noPrev no move + error*/

int Queue_prwtos(const Queue_Header ouraP){

  if (Queue_keni(ouraP))
    { return 0; }
  else
    { ouraP->Current = ouraP->Head;
      return 1;
    }

} /* to First - if empty to Head + error */

int Queue_teleytaios(const Queue_Header ouraP){

  if (Queue_keni(ouraP))
    { return 0; }
  else
    { ouraP->Current = ouraP->Last;
      return 1;
    }

} /* to Last - if empty to Last + null*/

/*prakseis anazhthshs*/

int Queue_anazitisi(const Queue_Header ouraP, Job_Data stoixeio, int (*iso)(Job_Data , Job_Data)){

      int i,brethike;
      Job_Data element;
      brethike = 0;
      if (Queue_prwtos(ouraP))
        { i = Queue_megethos(ouraP);
          while (i != 0 && brethike == 0)
            { Queue_periexomenoTrexon(ouraP, &element);
                  if ((*Queue_iso)(stoixeio, element) == 1)
                    { i = 0;
                      brethike = 1;
                    }
                  else if ((*Queue_iso)(stoixeio, element) == 0)
                    { Queue_epomenos(ouraP);
                      i--;
                    }
                  else
                    { i = 0;
                      brethike = 2;
                    }
/* Synergasia me thn iso : 0 = den exei brethei opote synexizei thn anazhthsh, 1 = brethike to stoixeio kai stamata h anazhthsh, 2 = to stoixeio pou phre apo thn lista einai megalytero apo to stoixeio pou psaxnoume opote stamata h anazhthsh */
            }
        }
      if (brethike == 1)
        { return 1; }
      else
        { return 0; }

}
