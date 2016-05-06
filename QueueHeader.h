/***************************************************************************************
Arxeio Epikefalidas	: QueueHeader.h
Syggrafeas			: Y. Cotronis (Mathima Domes Dedomenon)
Skopos			    : 
  O typos ATD OuraPlus symperiferetai ws oura (3 basikes praxeis): 
     a) stoixeia eisagontai sto telos
     b) stoixeia exagontai apo thn arxh
     c) elegxoume gia kenh
   Epipleon omws mporoume
     d) na kini9oume se olh thn oura (prwtos, teleytaios, epomenos, proigoumenos)
     e) na doume kai na allaxoume to periexomeno tou trexontos stoixeiou
     f) na diagrapsoume ton trexonta kombo
     g) na anazhthsoume stoixeio me kapoio krithrio isothtas 
     
   Ylopoieitai me Olikh apokrypsi 
     h) dimiourgia kai katastrofi
     
Ana8ewrhsh: P. Kontopoulos
****************************************************************************************/
#ifndef __Queue__ 
#define __Queue__
#include "QueueData.h"

/*dhlwseis typou*/
typedef struct GeneralNode *Queue_Header;

/*dhlwseis synarthsewn*/

/*dhmioyrgia kai katastrofh ouraPs*/
void Queue_dimiourgia(Queue_Header * const ouraP);

void Queue_katastrofi(Queue_Header *const ouraP);

int Queue_set_Pool_Size(const Queue_Header ouraP, int n);

int Queue_set_Queue_Size(const Queue_Header ouraP, int n);

/*prakseis elegxoy*/
int	Queue_keni(const Queue_Header ouraP);

int Queue_Pool_Size(const Queue_Header ouraP);

int Queue_Size(const Queue_Header ouraP);

int Queue_megethos(const Queue_Header ouraP);

int	Queue_full(const Queue_Header ouraP);

/*prakseis prosbashs-allazei to Current*/
int Queue_periexomenoTrexon(const Queue_Header ouraP, Job_Data * const value); /* error if Current invalid */

int Queue_allagiTrexon(const Queue_Header ouraP, Job_Data stoixeio); /* error if Current invalid */

/*prakseis eisagwgh-diagrafh Current */
int Queue_eisagogiTelos(const Queue_Header ouraP, Job_Data stoixeio); /* panta sto telos */

int Queue_exagogiArxh(const Queue_Header ouraP); /*  panta apo thn arxh  */ 

int Queue_diagrafiTrexon(const Queue_Header ouraP); /* apo apoydhpote (Current) */

/* Epilogh Kombou Current */
int Queue_epomenos(const Queue_Header ouraP); /* if noNext no move + error */

int Queue_proigoymenos(const Queue_Header ouraP); /* if noPrev no move + error*/

int Queue_prwtos(const Queue_Header ouraP); /* to First - if empty error */

int Queue_teleytaios(const Queue_Header ouraP); /* to Last - if empty to Current=Last + error */

/*prakseis anazhthshs*/
int Queue_anazitisi(const Queue_Header ouraP, Job_Data stoixeio, 
                    int (*iso)(Job_Data , Job_Data));
#endif /*#ifndef __GenList__ */
