/**
 *@file main.cpp
 *@version 5.0
 *@date 19/11/2020
 *@author Alejandro Lopez
 *@title Proyecto Final en C++
*/

#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

#define IZQUIERDA 75
#define DERECHA 77

using namespace std;

std::mutex semaforo;

/**
 *@brief pintarPantalla     Captura la salida por pantalla.
 *@param handle   Puntero inteligente captura E/S y errores por pantalla.
 *@param  coord   Estructura que define las coordenadas en un búffer (0,0).
 *@return h, v   Devuelve las coordenadas por pantalla.
*/
void pintarPantalla(int h, int v){

  HANDLE idPantalla;
  COORD coord;


  idPantalla = GetStdHandle (STD_OUTPUT_HANDLE);                  //Salida consola
  coord.X = h;
  coord.Y = v;

  SetConsoleCursorPosition(idPantalla, coord);                    //Pinta coordenas

}


/**
 *@brief quitarParpadeo     Captura el cursor por pantalla y quita el parpadeo que produce.
 *@param dwSize     Especifica un número entre 1-100 e indica lo qie ocupa este.
 *@param bVisible   Se encarga de cambiar la visibilidad del cursor.
*/
void quitarParpadeo(){

  HANDLE idPantalla;
  idPantalla = GetStdHandle (STD_OUTPUT_HANDLE);                   //Control skin pantll
  CONSOLE_CURSOR_INFO cursorSkin;                                  // Estruct info cursor por pantll
  cursorSkin.dwSize = 2;
  cursorSkin.bVisible = false;

  SetConsoleCursorInfo (idPantalla,&cursorSkin);                   //Contrl caracts cursor  // ALIAS VARIABLE

}

/**
 *@brief limitarPantalla    Limita la pantalla por consola mediante unos símbolos ASCII, generando una estructura cuadrada.
*/
void limitarPantalla(){

  for(int i=2; i<78; i++){

    pintarPantalla(i,3);
     printf ("%c", 205);

    pintarPantalla(i,33);
     printf ("%c", 205);

  }

  for(int i=4; i<33; i++){

    pintarPantalla(2,i);
     printf ("%c", 186);

    pintarPantalla(77,i);
     printf ("%c", 186);

  }

// ============ ESQUINAS ============ //
  pintarPantalla(2,3);
   printf ("%c", 201);

  pintarPantalla(2,33);
   printf ("%c", 200);

  pintarPantalla(77,3);
   printf ("%c", 187);

  pintarPantalla(77,33);
   printf ("%c", 188);

}


/**
 *@brief inicio_juego    Función que recorre una String y muestra  sus carácteres mediante un retardo de tiempo.
 *@param inicio     Variable tipo String que contiene los carácteres de inicio del juego.
 *@param system     Estructura que invoca un proceso por linea de comandos
 *@return Devuelve el recorrido de la String. Mediante un printf.
*/
void inicio_juego(){

    string inicio[20] = {"I", "N","I", "C", "I", "A", "N","D", "O", " ", "J", "U", "E", "G", "O", " ", ".", ".", "." };
    pintarPantalla(30,15);
    system("color 2");

    for(int i=0;i<19;i++){

        cout << inicio[i];
        Sleep(150);
    }

}

/**
 *@brief Clase Nave que es controlada por el jugador.
*/
class Nave{

  int h, v, corazon, vida;

 public:

/**
 *@brief Constructor de la clase Nave.
 *@param _h coordenada del eje horizontal publicas.
 *@param _v coordenada del eje vertical publicas.
 *@param _corazon variable simbolica de vidas que le quedan al jugador.
 *@param _vida  variable númerica de vidas que le quedan al jugador.
*/
   Nave(int _h, int _v, int _corazon, int _vida): h(_h), v(_v),corazon(_corazon), vida(_vida){}     //CONSTRC SIMPLIFICADO

   int X(){
       return h;
    }

   int Y(){
       return v;
    }

    int Vidas(){
       return vida;
    }


/**
 *@brief pintar Muestra la nave por pantalla mediante ACSCII
*/
   void pintar(){                                       //FORMA DE LA NAVE

    pintarPantalla(h,v);
     printf("  %c", 143);
   }


/**
 *@brief borrar Funcón que borra el recorrido de la nave por la pantalla
*/
   void borrar(){

    pintarPantalla(h,v);
     printf("    ");

   if( h>76 ){

      pintarPantalla(77,v);

       printf ("%c", 186);

      pintarPantalla(77,v+1);
       printf ("%c", 186);

      pintarPantalla(77,v+2);
       printf ("%c", 186);

     }
   }


/**
 *@brief mostrarVidas    Función que muestra las vidas restantes que le quedan al jugador por pantalla
*/
   void mostrarVidas(){

    pintarPantalla (64,2);                                  //POSCN VIDAS
     printf ("Lives");

    pintarPantalla (70,2);
     printf ("      ");

    for(int i =0; i< corazon; i++){

        pintarPantalla (70+i,2);
        printf ("%c", 3);

    }
   }


/**
 *@brief quitarCorazon Función que quita la vida del jugador
 *@return Devuelve una vida menos
*/
   void quitarCorazon(){

       corazon--;

    }


/**
 *@brief mover Función que se encarga del movimiento de la nave por la pantalla.
 *@param kbhit Se encarga de capturar el teclado del jugador.
 *@param key Se encarga de leer un único caracter por teclado sin mostrarlo.
 *@param IZQUIERDA Es una macro ASCII de la una tecla.
 *@param DERECHA Es una macro ASCII de la una tecla.
 *@return Devuelve la nave pintada cada vez que se desplaza y las vidas que le quedan.
*/
    void mover(){

        if(kbhit()){

            char key = getch();
            borrar();

       switch( key ){

            case IZQUIERDA:

                if( h>3 ) {

                    h--;
                }
                break;

            case DERECHA:

                if( h+3 < 76 ) {    //SE LE SUMA SEGUN EL NUM DE CARCTS NAVE

                    h++;
                }
                break;

            }

            pintar();
            mostrarVidas();

        }

    }


/**
 *@brief naveGolpeada Función que controla las variables de colisión de la nave y su comportamiento.
 *@param semaforo.lock Marca el inicio de ejecución hasta que se cierre para ejecutarlo mediante un Hilo.
 *@param semaforo.unlock Concluye el final de ejecución junto al Lock para la estructura mutex.
 *@return Devuelve las vidas restantes, genera una colisión por pantalla y vuelte a pintar la nave.
*/
   void naveGolpeada(){


    semaforo.lock();

    if (corazon == 0){

     borrar();
     pintarPantalla (h,v);
      printf ("  %c  ",176);

     Sleep(200);

     borrar ();

     pintarPantalla (h,v);
      printf ("%c",176);

     borrar();
     vida --;
     corazon =3;
     mostrarVidas();
     pintar();

    }
    semaforo.unlock();
   }


/**
 *@brief sonidoColision Función que genera un sonido mediante la tarjeta de la placa base
 *@param Beep Se encarga de administrar la duración y el efecto del sonido.
*/

   void sonidoColision(){

    semaforo.lock();
    Beep(200,300);
    semaforo.unlock();

    }

};


/**
 *@brief Clase Ovni Se mueve de forma independiente
*/
class Ovni {

  int h,v;

 public:

/**
 *@brief Constructor de la clase Ovni.
 *@param _h coordenada del eje horizontal publicas.
 *@param _v coordenada del eje vertical publicas.
*/
  Ovni(int _h, int _v): h(_h), v(_v){}

  int X(){
    return h;
  }

  int Y(){
    return v;
  }

/**
 *@brief pintar Función que pinta la forma del Ovni por pantalla.
*/
  void pintar(){

   pintarPantalla(h,v); printf("%c", 207);

  }

/**
 *@brief mover Función que se encarga de mover el ovni por la pantalla.
*/
  void mover(){

   pintarPantalla(h,v);
    printf(" ");



   if( h != 75){                            //Limite de Movimiento del Ovni

         h++;

    } else {
        v++;
        h=4;

    }

   pintar();

  }

/**
 *@brief sonidoColision Función que genera un sonido mediante la tarjeta de sonido de la placa base.
*/
  void sonidoColision(){

    Beep(200,300);

    }

/**
 *@brief llegaOvni Función encargada de delimitar el rango de movimiento del Ovni.
 *@return Devuelve  True o False dependiendo de su condicional.
*/
  bool llegaOvni() {

    if(v > 32){

            return true;

        } else {

            return false;
        }

    }

/**
 *@brief choque Función que comprueba si hay una colisión entre las coordenadas de la Nave y el Ovni.
 *@return Si se cumple la colisión genera una serie de funciones de la clase Nave.
*/
  void choque(class Nave &nave){

   if( h > nave.X() && h < nave.X()+3 && v >= nave.Y() && v+2 <= nave.Y()+2  ){

    nave.sonidoColision();
    nave.quitarCorazon();
    nave.borrar();
    nave.pintar();
    nave.mostrarVidas();

   }
  }
};


/**
 *@brief Clase Disparo Se origina a parti de la nave.
*/
class Disparo {

 int h,v;

 public:

/**
 *@brief Constructor de la clase Disparo.
 *@param _h coordenada del eje horizontal publicas.
 *@param _v coordenada del eje vertical publicas.
*/
 Disparo(int _h, int _v): h(_h),v(_v) {};

    int X(){

     return h;

     }

    int Y(){

      return v;

        }

/**
 *@brief mover Función que se encarga del movimiento del Disparo.
 *@return Muestra el recorrido del disparo por pantalla
*/
   void mover(){

        pintarPantalla(h,v);
         printf(" ");

        --v;

        pintarPantalla(h,v);
         printf("%c",250);
    }


/**
 *@brief fuera Función que se encarga del movimiento del Disparo.
 *@return Devuelve True o False si se sale de la limitación
*/
   bool fuera(){

        if(v == 4){

            return true;

        } else {

            return false;
        }
    }


};



/**
 *@brief Función Main donde se ejecuta el código
*/
int main(void){

  bool victoria = true;
  bool end_game = false;
  int puntos = 0;
  char nick[15];


  try {

  inicio_juego();

/**
 *@brief Limpia el búffer para posteriormente que escriba el nick el jugador.
*/
  fflush(stdin);
  pintarPantalla(20,15);
  printf("Introduce un nick, por favor: ");
  fflush(stdin);

/**
 *@brief Captura en el búffer lo que el jugador pulse por teclado.
*/
  scanf("%s",nick);
  system("cls");


  quitarParpadeo();
  limitarPantalla();

/**
 *@brief Inicializamos la clase Nave junto a unos parámetros ya inicializados.
*/
  Nave nave(36,32,3,3);
  nave.pintar();
  nave.mostrarVidas();

/**
 *@brief Creamos una lista que contenga punteros y un índice para poder recorrerlo posteriormente
*/
  list<Ovni*> O;                                          //Lista de Punteros
  list<Ovni*>::iterator indexO;                           //Creacion de un Indice para recorrer la lista


  list<Disparo*> D;
  list<Disparo*>:: iterator indexD;

/**
 *@brief Inicializamos los threads y añadimos los parámetros requeridos.
*/
  std::thread hilo1 (  &Nave::naveGolpeada, &nave );     //Se inicializa el Thread con la funct
  std::thread hilo2 (  &Nave::pintar, &nave );     //Se inicializa el Thread con la funct

/**
 *@brief Añadimos un nuevo Ovni en la lista junto a unas coordenas individuales.
*/
    O.push_back(new Ovni(10,6) );
    O.push_back(new Ovni(13,6) );
    O.push_back(new Ovni(16,6) );


    O.push_back(new Ovni(7,8) );
    O.push_back(new Ovni(10,8) );
    O.push_back(new Ovni(13,8) );
    O.push_back(new Ovni(16,8) );
    O.push_back(new Ovni(19,8) );


    O.push_back(new Ovni(4,10) );
    O.push_back(new Ovni(7,10) );
    O.push_back(new Ovni(10,10) );
    O.push_back(new Ovni(13,10) );
    O.push_back(new Ovni(16,10) );
    O.push_back(new Ovni(19,10) );
    O.push_back(new Ovni(22,10) );


/**
 *@brief Bucle que ejecuta el comienzo y su finalización mediante una variable Boolena.
*/
  while(!end_game){

  pintarPantalla(4,2);
  printf("Score: %d",puntos);

  pintarPantalla(32,2);
  printf("Nick: %s", nick);

   if(kbhit() ){

        char key = getch();

        if( key == 32 ){

            D.push_back(new Disparo( nave.X()+2,nave.Y()-1 ) );
        }
   }


/**
 *@brief Bucle que recorre toda la lista de Disparo.
 *@return Elimina el enlace y pasa al siguiente si el disparo sale de la pantalla.
*/
   for(indexD = D.begin();indexD != D.end();indexD++){

    (*indexD)->mover();                                                     //Contenido del puntero llama func Mover

    if((*indexD)->fuera() ){

        pintarPantalla( (*indexD)->X(), (*indexD)->Y());
         printf(" ");

        delete(*indexD);                                                   //libera RAM reservada
        indexD = D.erase(indexD);                                          //Borra el enlace

    }
  }

/**
 *@brief Bucle que recorre toda la lista de Ovni.
 *@return Elimina el enlace y pasa al siguiente si ha chocado.
*/
    for(indexO=O.begin();indexO!=O.end();indexO++){

     (*indexO)->mover();
     (*indexO)->choque(nave);



         if((*indexO)->llegaOvni() ){

            pintarPantalla( (*indexO)->X(), (*indexO)->Y());
             printf(" ");

            delete(*indexO);
            indexO = O.erase(indexO);
         }



     }


/**
 *@brief Bucle que recorre toda las listas de Ovni y Disparo.
 *@return Si concuerdan las coordenadas elimina ambos y genera puntos al jugador.
*/
    for(indexO=O.begin(); indexO!=O.end(); indexO++){
        for(indexD = D.begin();indexD != D.end();indexD++){

            if( (*indexO)-> X() == (*indexD)-> X() && (*indexO)-> Y() == (*indexD)-> Y() ){

                pintarPantalla( (*indexD)-> X(), (*indexD)-> Y() );
                printf(" ");
                delete(*indexD);
                indexD = D.erase(indexD);

                pintarPantalla( (*indexO)-> X(), (*indexO)-> Y() );
                printf(" ");
                delete(*indexO);
                indexO = O.erase(indexO);

                puntos+=100;

            }

        }
    }


    nave.naveGolpeada();
    nave.mover();

    Sleep(40);


    if( nave.Vidas() == 0){

        end_game = true;
        victoria = false;

    }

    if( puntos == 1500){

        end_game = true;

    }



  }


  /* =========================================== FIN JUEGO =========================================== */

  if(victoria) {

     system("cls");
      pintarPantalla(33,15);
      printf("Has Ganado");
      pintarPantalla(20,30);
      printf(" Tu puntuacion %s ha sido de %d",nick, puntos);

      getch();

  }else {
      system("cls");
      pintarPantalla(33,15);
      printf("GAME OVER");
      pintarPantalla(20,30);
      printf(" Tu puntuacion %s ha sido de %d",nick, puntos);

      getch();
  }

/**
 *@brief Condicional que comprueba la finalización de los Threads
 *@param joinable Comprueba la ejecución de los hilos
 *@param join comprueba si ha terminado la ejecución del hilo
*/
  if(hilo1.joinable()) {
        hilo1.join();
    }

    if(hilo2.joinable()) {
       hilo2.join();
    }

/**
 *@brief Función que atrapa errores de memoria.
 *@return muestra un mensaje si se ha llegado a producir un error.
*/
}catch(std::bad_alloc & e) {

    cout << "Error en la memoria." << e.what();

  }

}
