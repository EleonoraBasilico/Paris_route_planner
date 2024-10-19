using namespace std;

//Implemento una coda con priorità mediante lista ordinata

class Priority_queue {
      public:
               //Rimuove il primo elemento della coda e ne restituisce il valore
               int Extract_min() {
		 if (queue == NULL) 
		   return -1;   //Se la coda è vuota restituisco -1
		 else {
		   int min = queue->val;    //Memorizzo il valore del minimo
		   node* temp = queue->next;  //temp punta al secondo elemento della coda
		   free(queue);               //Libero la memoria
		   queue = temp;              
		   return min;
		 }
	       }

	       //Restituisce true se la coda è vuota, false altrimenti
	       bool Empty() {
		 return (queue == NULL);
	       }

	       //Inserisce in coda in posizione corretta il valore val con priorità prio
	       void Insert (int prio, int val) {
		 node* temp = queue;
		 if(temp == NULL || temp->prio > prio) {  //Se la coda è vuota oppure il nuovo elemento ha
		   node* new_node=new node;               //priorità minore del primo elemento della coda,
		   new_node->prio = prio;                 //allora lo aggiungo in testa alla coda. 
		   new_node->val = val;
		   new_node->next = temp;
		   queue = new_node;
		 }
		 else {
		   while(temp->next != NULL && prio > temp->next->prio) //Scorro la coda fino alla posizione corretta 
		     temp = temp->next;                                

		   node* new_node=new node;       //Individuata la posizione corretta, inserisco il nuovo
		   new_node->val = val;           //elemento.
		   new_node->prio = prio;
		   new_node->next = temp->next;
		   temp->next=new_node;
		 }
	       }

	       //Rimuove dalla coda l'eemento con priorità prio, se esiste
	       void Remove (int prio) {
		 node* temp = queue;
		 
                 if(temp != NULL) {            
		   if(temp->prio == prio) {    //Se il primo elemento della coda ha priorità prio
		     queue = queue->next;      //lo rimuovo facendo scorrere la coda.
		     free(temp);               //Libero la memria
		   }
		   else {

		     while(temp->next != NULL && prio > temp->next->prio) temp = temp->next;                          
		     
                     //Scorro la coda fino alla fine oppure fino a che non
		     //trovo un elemento con priorità minore o uguale 
		     //a prio. Infatti: se ho trovato un elemento con
		     //priorità prio procedo con la rimozione; se 
		     //invece ho trovato un elemento con priorità minore 
		     //di prio, allora sicuramente non esiste alcun 
		     //elemento con priorità prio.
                                 
                     if(temp->next != NULL && temp->next->prio == prio) {
		       node* temp2=temp->next;
		       temp->next = temp->next->next;
		       free(temp2);
		     }
		   }
		 }
	       }
private:
	       //Definisco una struct che contiene il singolo elemento della coda
	       struct node {
		 int prio, val;     //Ciascun elemento comprende tre campi:
		 node* next;        //   -il valore val: indica la fermata (stop_I)
	       };                   //   -la priorità prio: dipende dal tempo di arrivo alla fermata
                                    //   -un puntatore alla fermata successiva
	       
               node* queue = NULL;
};

