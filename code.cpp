#include "ee.h"
#include "Arduino.h"


static char *morse[27]={(char*)".- ",(char*)"-... ",(char*)"-.-. ",(char*)"-.. ",(char*)". ",(char*)"..-. ",(char*)"--. ",(char*)".... ",(char*)".. ",(char*)".--- ",(char*)"-.- ",(char*)".-.. ",(char*)"-- ",(char*)"-. ",(char*)"--- ",(char*)".--. ",(char*)"--.- ",(char*)".-. ",(char*)"... ",(char*)"- ",(char*)"..- ",(char*)"...- ",(char*)".-- ",(char*)"-..- ",(char*)"-.-- ",(char*)"--.. ",(char*)"] "};

 char *stringhe[5]={(char*)"A[FEATHER[IN[THE[HAND[IS[BETTER[THAN[A[BIRD[IN[THE[AIR[",
		(char*)"A[SHORT[PENCIL[IS[USUALLY[BETTER[THAN[A[LONG[MEMORY[ANY[DAY",(char*)"ACCEPT[SOMETHING[THAT[YOU[CANNOT[CHANGE[AND[YOU[WILL[FEEL[BETTER[",
		(char*)"ADVENTURE[CAN[BE[REAL[HAPPINESS",(char*)"ALL[THEEFFORT[YOU[ARE[MAKING[WILL[ULTIMATELY[PAY[OFF["};
//sostituisco agli spazi il carattere [ per comodità, visto che tale carattere segue lo spazio nel codice ASCII

static char led=13;
char it1=-1;	//posso usare char perchè arriva a massimo 6 come valore
char it2=0; //posso usare char perchè arriva a massimo 140 come valore
char it3=0;	//posso usare char perchè arriva a massimo 27 come valore
char num=0;
EventMaskType myEvents;
TickRefType r;
char k=0;
char *t;


extern "C" {
DeclareTask( Task1);
DeclareEvent( ev_rallenta);
DeclareEvent( ev_termina);
}

int main(void) {
	init();
	pinMode(led, OUTPUT);
	StartOS(OSDEFAULTAPPMODE);

	return 0;
}


TASK( Task1) {
C:	CancelAlarm(al_termina); //cancello l'allarme impostato dalla frase precedente, poichè ad ogni fine frase si ritorna qui con il goto
	ClearEvent(ev_termina);	//elimino l'evento per lo stesso motivo di prima
	it1++;	//incremento it1 in modo da accedere alla frase successiva
	//inizio pausa
	GetCounterValue(SystemCounter, r);
	SetRelAlarm(al_termina,((unsigned)r)+50,((unsigned)r)+50);
	WaitEvent(ev_termina);
	//fine pausa
	ClearEvent(ev_termina);
	CancelAlarm(al_termina);
	GetCounterValue(SystemCounter, r);
	SetRelAlarm(al_termina,((unsigned)r)+18000,((unsigned)r)+18000);
	//starting sending phase
	while(it1!=5){
		it2=0;
		while(stringhe[it1][it2]!='\0'){
			num=(char)stringhe[it1][it2];
			t=morse[num-65];
			it3=0;
			char m=stringhe[it1][it2];
			while(t[it3]!=' '){
				if(t[it3]=='.'){
					k=0;
					while(k<=1){
						WaitEvent(ev_rallenta|ev_termina);
						GetEvent(Task1, &myEvents);
						if (myEvents & ev_rallenta){
							ClearEvent(ev_rallenta);
							if(k==0){
								digitalWrite(led, HIGH);
							}
							else{
								digitalWrite(led, LOW);
							}
						}
						else{
							//end sending phase
							goto C;
						}
						k++;
					}
				}
				else if(t[it3]=='-'){
					k=0;
					while(k<=3){
						WaitEvent(ev_rallenta|ev_termina);
						GetEvent(Task1, &myEvents);
						if (myEvents & ev_rallenta){
							ClearEvent(ev_rallenta);
						if(k<=2){
							digitalWrite(led, HIGH);
						}
						else{
							digitalWrite(led, LOW);
							}
						}
						else{
							//end sending phase
							goto C;
						}
						k++;
					}
				}
				else{
					k=0;
					while(k<=2){
						WaitEvent(ev_rallenta|ev_termina);
						GetEvent(Task1, &myEvents);
						if (myEvents & ev_rallenta){
							ClearEvent(ev_rallenta);
							digitalWrite(led, LOW);
						}
						else{
							//end sending phase
							goto C;
						}
						k++;
					}
				}
				it3++;
			}
			k=0;
			while(k<2){
				WaitEvent(ev_rallenta|ev_termina);
				GetEvent(Task1, &myEvents);
				if (myEvents & ev_rallenta){
					ClearEvent(ev_rallenta);
					digitalWrite(led, LOW);
					}
				else{
					//end sending phase
					goto C;
					}
				k++;
				}
			it2++;
			}
		}
	TerminateTask();
}


