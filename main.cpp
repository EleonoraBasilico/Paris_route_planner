#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Priority_queue.h"

//Struct che contiene le informazioni relative ad un arco
//di una tratta di un trasporto pubblico
struct arc_pt {
  int to_stop;      //fermata di arrivo
  int dep_time;     //orario di partnza
  int arr_time;     //orario di arrivo
  int route_type;   //mezzo di trasporto usato
};

//Struct che contiene le informazioni relative
//ad un arco di camminata
struct arc_walk {
  int to_stop;      //fermata di arrivo
  int d_walk;       //distanza da percorrere
};                  

//Struct che contiene le informazioni relative ad uno 
//spostamento tra due fermate.
//La struct verrà usata nell'algoritmo di Dijkstra.
struct move_from_to {
  boolean visited = false;    //Dice se la fermata di arrivo è stata
                              //raggiunta per la prima volta.
  
  int last_node =-1;          //L'ultima fermata precedentemente raggiunta.

  int route_type = -2;        //Indica il tipo di trasporto pubblico usato, o
                              //se, eventualmente, ci si è mossi a piedi.
  
  int dep_time = 0;           //Orario di partenza del trasporto pubblico usato
                              //per spostarsi da last_node alla fermata di arrivo.

  int arr_time = INT_MAX;     //Orario di arrivo alla fermata nel caso ottimale.
                              //Svolge il ruolo di peso nell'algoritmo di Dijkstra:
                              //voglio che sia il più piccolo possibile.
};

vector<arc_walk>* walk_graph;   //Grafo con soli archi percorribili a piedi.
vector<arc_pt>* pt_graph;       //Grafo con soli archi dei trasporti pubblici.

int N;   //N è il massimo numero di fermate +1: il
         //+1 serve per la gestione degli array.

bool* exist;   //Array di booleani. exist[i]=true se esiste la
               //fermata associata all'indice i, false altrimenti.
               //L'array è necessario perchè non ad ogni indice è 
               //associata una fermata.

ifstream file_csv;  //Serve ad aprire i file contenenti le informazioni
                    //necessarie alla creazione dei grafi.

//Creo i grafi
void Graph () {
  //Step 1: calcolo il valore di N.
           file_csv.open("network_nodes.csv");
           string line, second_last_line, last_line;
           //La stringa line verrà usata per convertire 
           //in dati le porzioni di file che contengono
           //le informazioni utili (indici delle fermate).
           //Le stringhe second_last_line e last_line 
           //memorizzano due righe consecutive. Ciò è
           //necessario poichè l'ultima riga del file è vuota
           //e il valore dell'indice associato all'ultima 
           //fermata si trova nella penultima riga.

           while(!file_csv.eof()) {
             second_last_line = last_line;
             getline(file_csv,last_line);
           }

           //Scorro il file fino alla fine: al termine del ciclo
           //while last_line contiene il valore dell'ultima riga
           //(che è vuota), mentre second_last_line contiene il
           //valore della penultima riga.

           file_csv.close();
           N = atoi(second_last_line.substr(0, second_last_line.find(";").c_str()) + 1;
           //Converto la stringa che memorizza la penultima
           //riga nel corrispondente numero e aggiungo 1.

  //Step 2: Creo l'array di booleani exist[] definito sopra.
           exist = new bool[N];
           cout << "Lettura di network_nodes...";
           file_csv.open("network_nodes.csv");
           getline(file_csv,line);  //Ignora la prima riga
           for(i=0; i<N; i++) exist[i] = false;
           while(!file_csv.eof()) {
             getline(file_csv, line, ';');
             int j = atoi(line.c_str());
             exist[j] = true;
             getline(file_csv, line);  //Ignoro il resto della riga
           }
           file_csv.close();
           cout << "Fine lettura di network_nodes." << endl;

  //Step 3: Leggo il file network_temporal_day e creo 
  //il grafo con le sole tratte percorribili a piedi.
	   cout << "Lettura di network_temporal_day...";
	   file_csv.open("network_temporal_day.csv");
	   getline(file_csv,line);  //Ignora la prima riga
	   while(!file_csv.eof()) {
	    
             //Estraggo da una riga del file le informazioni 
	     //utili e le converto in dati; i valori 
	     //corrispondenti saranno poi assunti dai
	     //diversi campi della struct new_arc.
	     //Itero il processo fino al termine del file,
	     //e cioè fino a quando a ciascuna riga non
	     //sarà fatto corrispondere un diverso arco
             //di trasporto pubblico.

	     arc_pt new_arc;
	     
             getline(file_csv,line, ';');
	     int j = atoi(line.c_str());

	     getline(file_csv,line, ';');
	     int info = atoi(line.c_str());
	     new_arc.to_stop = info;

	     getline(file_csv,line, ';');
	     int info = atoi(line.c_str());
	     new_arc.dep_time = info - 1481500800;
	     //I file riportano il tempo in Unix time, ovvero in secondi
	     //trascorsi dalla mezzanotte del 01/01/1970 in tempo UTC.
	     //I dati raccolti risalgono al 12/12/2016; la mezzanotte
	     //del 12/12/2016 ha Unix time 1481500800. Sottraendo questa
	     //quantità ai dati, otteniamo gli orari di partenza e di 
	     //arrivo dei mezzi di trasporto pubblico, relativi alla 
	     //mezzanotte. La modifica semplifica la scrittura dei 
	     //dati in input.

	     getline(file_csv,line, ';');
	     int info = atoi(line.c_str());
	     new_arc.arr_time = info - 141500800;

	     getline(file_csv, line, ';');
	     int info = atoi(line.c_str());
	     new_arc.route_type = info;

	     getline(file_csv,line);  //Ignora il resto della riga

	     pt_graph[j].push_back(new_arc);
	     //Inserisco nel grafo gli archi relativi all'indice j.
	     //Il grafo è creato tramite liste di adiacenza: a 
	     //ciascun indice (e cioè a ciascuna fermata) è 
	     //associata una lista di struct arco_pt, ognuna delle 
	     //quali rappresenta una diversa fermata raggiungibile
	     //a partire dalla fermata j tramite trasporto pubblico.
	   }
	   file_csv.close();
	   cout << "Fine lettura di network_temporal_day." <<endl;

  //Step 4: Leggo il file network_temporal_day e creo 
  //il grafo dei trasporti pubblici.
	   cout << "Lettura di network_walk...";
	   file_csv.open("network_walk.csv");
	   getline(file_csv,line);  //Ignora la prima riga
	   while(!file_csv.eof()) {

	     arc_walk A_to_B, B_to_A;
	     //Questo grafo, a differenza del precedente, non è
	     //orientato perchè ciascuna tratta è percorribile
	     //a piedi in entrambi i sensi. Bisogna quindi 
	     //inserire ciascun arco arc_walk sia nella lista
	     //di adiacenza della fermata di andata, che in 
	     //quella della fermata di ritorno.

	     getline(file_csv,line, ';');
	     int A = atoi(line.c_str());
	     B_to_A.to_stop = j;

	     getline(file_csv,line, ';');
	     int B = atoi(line.c_str());
	     A_to_B.to_stop = B;

	     getline(file_csv,line, ';');  //Ignora il terzo dato

	     getline(file_csv,line);
	     int info = atoi(line.c_str());
	     A_to_B.d_walk = info;
	     B_to_A.d_walk = info;

	     walk_graph[A].push_back(A_to_B);
	     walk_graph[B].push_back(B_to_A);
	   }
	   file_csv.close();
           cout << "Fine lettura di network_walk."<< endl;
	  }

 //Funzione che converte gli orari di arrivo e di partenza
 //forniti in secondi in una stringa della forma hh:mm:ss.
 //È impiegata nell'algoritmo di Dijkstra per la stampa 
 //del percorso ottimale.
	     string time_converter( int time) {

	     int hh = time/3600;
	     string hour = to_string(hh);
	     if(hh < 10) hour = "0" + hour;

	     int mm = (time - hh*3600)/60;
	     string min = to_string(mm);
	     if(mm < 10) min = "0" + min;

	     int ss = time - hh*3600 - mm*60;
	     string sec = to_string(ss);
	     if(ss < 10) sec = "0" + sec;

	     return hour+":"+min+":"+sec;
	   }

 //Algoritmo di Dijkstra rivisitato:
 //Il seguente algoritmo è una versione leggermente modificata
 //del noto algoritmo di Dijkstra. 
 //Le principali differenze con l'originale sono 2:
 //    1) Il peso di ciascun arco non è costante e non è noto 
 //       a priori: nel nostro caso è dato dal tempo necessario 
 //       a percorrere la tratta descritta dall'arco stesso.
 //       Esso però non dipende solo dall'effettivo tempo di
 //       percorrenza della tratta ma anche da un ipotetico tempo
 //       di attesa;
 //    2) Mentre in Dijkstra il nodo di partenza ha distanza
 //       0 da se stesso, nel nostro algoritmo il nodo di partenza
 //       ha distanza start_time da se stesso.
 //Allora, il peso di ciascun arco è dato dalla somma di tempo di
 //percorrenza, tempo di attesa e start_time, ma coincide sempre e 
 //comunque con il tempo di arrivo al nodo.
 //Ora, poichè nell'algoritmo di Dijkstra non è davvero necessario 
 //conoscere il peso di ciascun arco ( se disponiamo di un altro 
 //metodo per calcolare la distanza di un nodo da quello iniziale)
 //scegliamo come distanza di un generico nodo dalla sorgente il 
 //tempo di arrivo al nodo arr_time.

 void Dijkstra(int from, int to, int start_time, int max_time ) {
	    
             move_from_to* itinerary = new move_from_to[N];
	     itinerary[from].arr_time = start_time;
	     //Costruisco un array N-dimensionale di struct move_from_to,
	     //le quali contengono per ciascun nodo le informazioni che 
	     //è necessario memorizzare per l'implementazione 
	     //dell'algoritmo. 
	     //Mentre in Dijkstra inizializziamo a 0 la distanza
	     //del nodo di partenza da se stesso, qui la 
	     //inizializziamo a start_time, come spiegato sopra.
	     //La distanza degli altri nodi dal nodo di partenza
	     //è inizializzata a infinito (o meglio, a INT_MAX).

	     Priority_queue queue;
	     queue.Insert(from + start_time, from);
	     //Inserisco nella coda il nodo di partenza.
	     //La priorità di ciascun elemento della lista ordinata
	     //è data non solo dal tempo di arrivo, ma anche 
	     //dall'indice del nodo corrispondente. Questo perchè 
	     //supponiamo per semplicità che le priorità siano  
	     //diverse l'una dall'altra: la scelta di sommare al 
	     //tempo di arrivo l'indice del nodo è dovuta al fatto
	     //che ciascun indice compare al più una volta, e 
	     //questo garantisce l'unicità della priorità.

	     bool out_of_time = false;

	     while(!queue.Empty() && !itinerary[to].visited && !out_of_time) {

	       //L'algoritmo prosegue fino a che la coda non è vuota oppure
	       //fino a che il nodo finale non viene raggiunto oppure fino a 
	       //che non si sfora il tempo massimo.

	       int min = queue.Extract_min();
	       itinerary[min].visited = true;
	       if(itinerary[min].arr_time - start_time > max_time) 
		 out_of_time = true;
		
	       //Se il tempo impiegato a raggiungere il nodo min è maggiore
	       //del tempo massimo, allora riusciremmo a raggiungere il nodo
	       //finale dopo il tempo massimo, per cui termina il ciclo while.

	       else {

		 //Gli archi di camminata e gli archi di trasporto pubblico
		 //sono trattati separatamente perchè per i primi non
		 //esiste il tempo d'attesa; pertanto, il tempo di arrivo 
		 //al nodo successivo è dato dalla somma del tempo di arrivo
		 //al nodo corrente e del tempo di percorrenza a piedi.

		 for (arc_pt arc : pt_graph[min] ) {  //Scorro la lista di adiacenza di min

		   //Considero l'arco solo se il suo tempo di partenza
		   //è successivo al tempo di arrivo al nodo corrente,
		   //altrimenti non sarebbe possibile prendere il 
		   //mezzo di trasporto corrispondente.
		   if (arc.dep_time >= itinerary[min].arr_time) {
		    
                    if (!itinerary[arc.to_stop].visited && itinerary[arc.to_stop].arr_time > arc.arr_time ) {

		      //Sostituisco il precedente percorso ottimale per arrivare
		      //a arc.to_stop con con quello appena trovato.
		      queue.Remove( itinerary[arc.to_stop].arr_time + arc.to_stop );
		      itinerary[arc.to_stop].arr_time = arc.arr_time;
		      itinerary[arc.to_stop].last_node = min;
		      itinerary[arc.to_stop].route_type = arc.route_type;
		      itinerary[arc.to_stop].dep_time = arc.dep_time;
		      queue.Insert(arc.arr_time + arc.to_stop, arc.to_stop);
		    }
		   }
		 }

		 for (arc_walk arc : walk_graph[min]) {

		   int arr_time = itinerary[min].arr_time + arc.d_walk * 36/50;
		   //Calcolo il tempo di arrivo al nodo successivo sommando
		   //il tempo di arrivo al nodo corrente e il tempo impiegato
		   //a percorrere a piedi il tragitto supponendo di 
		   //camminare alla velocità di 5 km/h.

                   if (!itinerary[arc.to_stop].visited && itinerary[arc.to_stop].arr_time > arc.arr_time ) {
		      
		      queue.Remove( itinerary[arc.to_stop].arr_time + arc.to_stop );
		      itinerary[arc.to_stop].arr_time = arr_time;
		      itinerary[arc.to_stop].last_node = min;
		      itinerary[arc.to_stop].route_type = -1;
		      itinerary[arc.to_stop].dep_time = itinerary[min].arr_time;
		      queue.Insert(arr_time + arc.to_stop, arc.to_stop);
		    }
		 }
	       }
	       if (out_of_time) {
                 cout << "Non è possibile raggiungere il nodo finale: ho sforato il tempo massimo" << endl;
	       }
	       if (itinerary[to].visited) {

		 //Stampo il percorso ottimale trovato tornando indietro 
		 //fino al nodo di partenza.
		 int x = to;
		 while( itinerary[x].last_node != -1 ) {
		   cout << "[" << time_converter( itinerary[x].dep_time ) << "-" << time_converter( itinerary[x].arr_time) << "]" ;
		   cout << "vado da" << itinerary[x].last_node << "a" << x;
		   
                   switch( itinerary[x].route_type ) {
		   case -1:
		           cout << "vado a piedi ";
		           break;
		   case 0:
		           cout << "Prendo il tram ";
		           break;
		   case 1:
		           cout << "Prendo la metropolitana ";
		           break;
		   case 2:
		           cout << "Prendo il treno ";
		           break;
		   case 3:
		           cout << "Prendo l'autobus ";
		           break;
		   case 4:
		           cout << "Prendo il traghetto ";
		           break;
		   case 5:
		           cout << "Prendo il cablecar ";
		           break;
		   case 6:
		           cout << "Prendo la gondola ";
		           break;
		   case 7:
		           cout << "Prendo la funicolare ";
		           break;
		   }
		   cout << endl;
		   x = itinerary[x].last_node;
		 }
	       }
	       else {
		 cout << "Non è possibile raggiungere il nodo finale: il nodo iniziale non è connesso al nodo finale" << endl;
	       }
	     }


int main() {

  Graph(); //Inizializzazione grafo

  int from, to, start_time, max_time;
  bool repeat = true;

  while ( repeat ) {

    cout << "Inserisci l'indice della fermata di partenza o inserisci -1 per uscire" << endl;
    cin >> from;

    if(from == -1 || (from >=0 && from < N && exist[from])) 
      repeat = false;
    else {
      cout << "L'indice inserito non esiste" << endl;
    }
  }

  repeat = true;
  while ( repeat ) {

    cout << "Inserisci l'indice della fermata di arrivo" << endl;
    cin >> to;

    if ( to >= 0 && to < N && exist[to] )
      repeat = false;
    else {
      cout << "L'indice inserito non esiste" << endl;
    }
  }

  repeat = true;
  while ( repeat ) {

    string start;
    cout << "Inserisci l'orario di partenza (in formato hh:mm:ss)" << endl;
    cin >> start;

    //Verifico che l'input abbia esattamente 8 caratteri e 
    //che rispetti il formato hh:mm:ss.
    if ( start.length() == 8 && start[2] == ':' && start[5] == ':'
	 && isdigit(start[0]) && isdigit(start[1]) && isdigit(start[3])
	 && isdigit(stsart[4]) && isdigit(start[6]) && isdigit(start[7])) {

      int hh = atoi(start.substr(0,2).c_str());
      int mm = atoi(start.substr(3,2).c_str());
      int ss = atoi(start.substr(6,2).c_str());

      //Verifico che il tempo inserito abbia senso
      if ( hh < 24 && mm < 60 && ss < 60 ) {

	start_time = hh * 3600 + mm * 60 + ss;
	repeat = false;
      }
      else {
	cout << "L'orario inserito non ha senso, riprovare." << endl;
      }
    }
    else {
	cout << "Il formato non è corretto, riprovare." << endl;
      }
    }

  repeat = true;
  while ( repeat ) {

    string start;
    cout << "Inserisci la durata massima del tragitto (in formato hh:mm:ss)" << endl;
    cin >> start;

    //Come sopra, verifico che l'input abbia esattamente 8 caratteri e 
    //che rispetti il formato hh:mm:ss.
    if ( start.length() == 8 && start[2] == ':' && start[5] == ':'
	 && isdigit(start[0]) && isdigit(start[1]) && isdigit(start[3])
	 && isdigit(stsart[4]) && isdigit(start[6]) && isdigit(start[7])) {

      int hh = atoi(start.substr(0,2).c_str());
      int mm = atoi(start.substr(3,2).c_str());
      int ss = atoi(start.substr(6,2).c_str());

      if ( hh < 24 && mm < 60 && ss < 60 ) {

	max_time = hh * 3600 + mm * 60 + ss;
	repeat = false;
      }
      else {
	cout << "L'orario inserito non ha senso, riprovare." << endl;
      }
    }
    else {
	cout << "Il formato non è corretto, riprovare." << endl;
      }
    }

  Dijkstra(from, to, start_time, max_time);

  cout << endl << endl;

 return 0;
}

