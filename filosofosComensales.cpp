#include <iostream>
#include <thread>
#include <semaphore.h>
#include <windows.h>
#include <string>
#include <unistd.h>

using namespace std;

//Iniciación de los semaforos 
sem_t tenedor[4];
sem_t comedor;

//Función de los filosofos
void filosofo (int numFilosofo){
    int numeroComidas = 0;      //Contador del número de veces que ha comido el filosofo
    while(numeroComidas < 6){
        int tiempoComer = rand() % 4 + 1;       //Tiempo que se detendra la ejecución del hilo simulando el tiempo de comida 
        //Estado pensando
        string msg = "\nEl filosofo " + to_string(numFilosofo + 1) + " empieza a pensar";
        cout << msg;
        //Estado comiendo
        sem_wait(&comedor);
        sem_wait(&tenedor[numFilosofo]);
        sem_wait(&tenedor[(numFilosofo + 1) % 5]);
        msg = "\nEl filosofo " + to_string(numFilosofo) + " empezo a comer con los tenedores " + 
                to_string(numFilosofo) + " y " + to_string((numFilosofo+1)%5);
        cout<<msg;
        sleep(tiempoComer);
        //Estado terminando de comer
        numeroComidas++;
        msg = "\nEl filosofo " + to_string(numFilosofo ) + " termino de comer, libera los tenedores " + 
                to_string(numFilosofo) + " y " + to_string((numFilosofo+1)%5) + ". Numero de comida = " +
                to_string(numeroComidas);
        cout<<msg;
        sem_post(&tenedor[numFilosofo]);
        sem_post(&tenedor[(numFilosofo+1) % 5]);
        sem_post(&comedor);
    }
}


int main(){

    //Inicialización de los semáforos para el comedor y los tenedores
    for(int x=0; x < 5; x++){
        sem_init(&tenedor[x], 0, 1);
    }
    sem_init(&comedor,0,4);

    //Creación e inicialización de los hilos de cada filosofo
    thread filosofo1(filosofo, 0);
    thread filosofo2(filosofo, 1);
    thread filosofo3(filosofo, 2);
    thread filosofo4(filosofo, 3);
    thread filosofo5(filosofo, 4);


    //Esperar a que todos los hilos terminen su ejecución
    filosofo1.join();
    filosofo2.join();
    filosofo3.join();
    filosofo4.join();
    filosofo5.join();

    cout << "\nTodos los filosofos han comido 6 veces y se encuentran satisfechos";
    return 0;
}