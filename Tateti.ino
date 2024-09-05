//Pines Leds
#define B11 41
#define G11 39
#define R11 37

#define B12 49
#define G12 51
#define R12 53

#define B13 A7
#define G13 A8
#define R13 A9

#define B21 35
#define G21 33
#define R21 31

#define B22 47
#define G22 45
#define R22 43

#define B23 A10
#define G23 A11
#define R23 A12

#define B31 29
#define G31 27
#define R31 25

#define B32 22
#define G32 24
#define R32 23

#define B33 A6
#define G33 A5
#define R33 A4

#define BLT A13
#define GLT A14
#define RLT A15

#define BUZ 70 //38

////Pines Teclado Matricial
#define C1 2
#define C2 18
#define C3 19
#define F1 14
#define F2 15
#define F3 16
#define F4 17

int arrayLeds;

#include <Keypad.h>
const byte FILASLEDS = 3;
const byte COLUMNASLEDS = 3;

const byte FILASTECLADO = 4;
const byte COLUMNASTECLADO = 3;

char teclado_matricial [FILASTECLADO][COLUMNASTECLADO] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
  
};

int tateti[FILASLEDS][COLUMNASLEDS] = {
  {11, 12, 13},
  {14, 15, 16},
  {17, 18, 19}
};

byte pines_filas[FILASTECLADO] = {F1, F2, F3, F4}; 
byte pines_columnas[COLUMNASTECLADO] = {C1, C2, C3}; 
Keypad keypad = Keypad( makeKeymap(teclado_matricial), pines_filas, pines_columnas, FILASTECLADO, COLUMNASTECLADO);

int jugadorActual = 0;
int Ficha_actual = 1;
int IDs_Fichas[6] = {1,1,2,2,3,3};

char teclaPresionada,destinoFicha;
bool alguienGano = false;
bool tiempoAgotado = false;
bool fichasPuestas = false;
bool fichaSeleccionada = false;
int  ganador = 2;
int cantidadMovimientos = 0;
char Posiciones_Rojo[3] = {'-', '-', '-'}; // Arreglo para las posiciones de fichas rojas
char Posiciones_Verde[3] = {'-', '-','-'}; // Arreglo para las posiciones de fichas verdes

void setup() {
  Serial.begin(9600);
  pinMode(BUZ,OUTPUT);

  pinMode(RLT,OUTPUT);
  pinMode(GLT,OUTPUT);
  pinMode(BLT,OUTPUT);
  
  pinMode(R11,OUTPUT);
  pinMode(G11,OUTPUT);
  pinMode(B11,OUTPUT);

  pinMode(R12,OUTPUT);
  pinMode(G12,OUTPUT);
  pinMode(B12,OUTPUT);

  pinMode(R13,OUTPUT);
  pinMode(G13,OUTPUT);
  pinMode(B13,OUTPUT);

  pinMode(R21,OUTPUT);
  pinMode(G21,OUTPUT);
  pinMode(B21,OUTPUT);

  pinMode(R22,OUTPUT);
  pinMode(G22,OUTPUT);
  pinMode(B22,OUTPUT);

  pinMode(R23,OUTPUT);
  pinMode(G23,OUTPUT);
  pinMode(B23,OUTPUT);

  pinMode(R31,OUTPUT);
  pinMode(G31,OUTPUT);
  pinMode(B31,OUTPUT);

  pinMode(R32,OUTPUT);
  pinMode(G32,OUTPUT);
  pinMode(B32,OUTPUT);

  pinMode(R33,OUTPUT);
  pinMode(G33,OUTPUT);
  pinMode(B33,OUTPUT);

  
}


void loop() {

  //testLedsRGB();
  //testTeclado();
  encenderBuzzer(1,50);

  while (!alguienGano){
    mostrarTurnoJugador(jugadorActual);
    imprimirTablero();
    teclaPresionada = escucharTeclado();
    if (teclaPresionada == '0'){
      Serial.println("Tiempo Agotadooooooooooooo!");
      cantidadMovimientos--;
    }
    else{
      if (cantidadMovimientos < 6){
      actualizarPosiciones(jugadorActual,IDs_Fichas[cantidadMovimientos],teclaPresionada);
      }
    }


    cantidadMovimientos++;
    //Si ya pusieron las 6 fichas
    if (cantidadMovimientos > 6){
      //Habilito mover
      fichasPuestas = true;
      //Chequeo si presiono un lugar no vacio (osea selecciono una ficha propia)
      fichaSeleccionada = chequearFichaSeleccionada(jugadorActual,teclaPresionada);

      if (fichaSeleccionada){
        prenderAdyacentesDisponibles(teclaPresionada);
          Serial.print("ficha A mover: ");
          Serial.println(teclaPresionada);
        //ID_ficha_elegida = buscarPosicionEnArrayDeFichas()
        destinoFicha = escucharTeclado();
        apagarAzulesYorigen(teclaPresionada);
        teclaPresionada = destinoFicha;
      }else {
        Serial.println("No selecciono ficha propia");
        encenderBuzzer(1,300);
      }
    }else {
      fichaSeleccionada = true;

      //actualizarPosiciones(jugadorActual,IDs_Fichas[cantidadMovimientos],teclaPresionada);
    }
    if (fichaSeleccionada){
      Serial.print("lo movi a tecla: "); Serial.println(teclaPresionada);
      prenderLedJugado(jugadorActual,teclaPresionada);
      alguienGano = chequearVictoria(cantidadMovimientos);
    }
    siguienteTurno(jugadorActual);  
  }

  //Si alguien gano
  cantidadMovimientos = 0;
  mostrarGanador();
  reiniciarJuego();
  Serial.println("FIN de JUEGO");
}

//FUNCIONES DE JUEGO
bool chequearFichaSeleccionada(int jugActual,char teclaPres){
   
  //O: rojo, 1: verde
  switch (teclaPres){
    case '1':
      return (tateti[0][0] == jugActual); //Retorna verdadero si en el lugar hay una ficha del jugador
      break;

    case '2':
      return (tateti[0][1] == jugActual);
      break;  

    case '3':
      return (tateti[0][2] == jugActual);
      break;  

    case '4':
      return (tateti[1][0] == jugActual);
      break;  
      
    case '5':
      return (tateti[1][1] == jugActual);
      break;  
      
    case '6':
      return (tateti[1][2] == jugActual);
      break;  
      
    case '7':
      return (tateti[2][0] == jugActual);
      break;  
        
    case '8':
      return (tateti[2][1] == jugActual);
      break;  

    case '9':
      return (tateti[2][2] == jugActual);
      break;  
      
  }//FIN switch(teclaPres)  
}


//Apaga las adyacentes disponibles y la que se va a mover
void apagarAzulesYorigen(char teclaPres){
  //Apago azules, si no estan ocupadas
  switch (teclaPres){
    case '1':
      //Actualizo matriz
      tateti[0][0] = 11;
      //Apago el origen
      digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, LOW);      
      //Apago adyacentes si no estan ocupados
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
      }
      break;

    case '2':
      tateti[0][1] = 12;
      digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, LOW);      
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
      }
      break;  

    case '3':
      tateti[0][2] = 13;
      digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, LOW);      
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
      }
      break;  
      
    case '4':
      tateti[1][0] = 14;
      digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, LOW);      
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
      }
      break;
      
    case '5':
      tateti[1][1] = 15;
      digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);      
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
      }
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
      }
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
      }
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
      }  
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
      }
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, LOW);
      }
      break;
      
    case '6':
      tateti[1][2] = 16;
      digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, LOW);      
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, LOW);
      }
      break;
      
    case '7':
      tateti[2][0] = 17;
      digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, LOW);      
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
      }
      break;
        
    case '8':
      tateti[2][1] = 18;
      digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, LOW);      
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, LOW);
      }
      break;

    case '9':
      tateti[2][2] = 19;
      digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, LOW);      
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
      }  
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
      }
      break;
  }//END SWITCH


}

//Prende en azul las luces adyacentes disponibles a la seleccionada
void prenderAdyacentesDisponibles(char teclaPres){

  switch (teclaPres){
    case '1':
      //Si esta vacio el lugar
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, HIGH);
      }
      break;

    case '2':
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, HIGH);
      }
      break;  

    case '3':
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, HIGH);
      }
      break;  
      
    case '4':
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, HIGH);
      }
      break;
      
    case '5':
      if (tateti[0][0] == 11){
          digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, HIGH);
      }
      if (tateti[0][1] == 12){
          digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, HIGH);
      }
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, HIGH);
      }
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, HIGH);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, HIGH);
      }  
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, HIGH);
      }
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, HIGH);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, HIGH);
      }
      break;
      
    case '6':
      if (tateti[0][2] == 13){
          digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, HIGH);
      }
      break;
      
    case '7':
      if (tateti[1][0] == 14){
          digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, HIGH);
      }
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, HIGH);
      }
      break;
        
    case '8':
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[2][0] == 17){
          digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, HIGH);
      }
      if (tateti[2][2] == 19){
          digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, HIGH);
      }
      break;

    case '9':
      if (tateti[1][1] == 15){
          digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, HIGH);
      }
      if (tateti[1][2] == 16){
          digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, HIGH);
      }  
      if (tateti[2][1] == 18){
          digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, HIGH);
      }
      break;
  }//END SWITCH
}

//Espera la tecla * para reiniciar el juego
void reiniciarJuego(){

  //Serial.println("Para reiniciar el juego presione la tecla *"); //print to serial the key that has been pressed
  delay(50);
  char key = keypad.getKey();
  
  if (key != NO_KEY && key == '*'){
    //Serial.println(key);
    alguienGano = false;
    limpiarMatriz();
  }

}

//Reinicio la matriz de juego tateti
void limpiarMatriz(){
  int contador = 11;
  for (int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      tateti[i][j] = contador;
      contador++;
    } 
  }
  imprimirTablero();
}

//Prendo todos los leds con el color del ganador
void mostrarGanador(){

  if (ganador == 0){
    digitalWrite(R11, HIGH); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
    digitalWrite(R12, HIGH); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
    digitalWrite(R13, HIGH); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
    digitalWrite(R21, HIGH); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
    digitalWrite(R22, HIGH); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
    digitalWrite(R23, HIGH); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
    digitalWrite(R31, HIGH); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
    digitalWrite(R32, HIGH); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
    digitalWrite(R33, HIGH); digitalWrite(G33, LOW); digitalWrite(B33, LOW);  
  }else if (ganador == 1){
    digitalWrite(R11, LOW); digitalWrite(G11, HIGH); digitalWrite(B11, LOW);
    digitalWrite(R12, LOW); digitalWrite(G12, HIGH); digitalWrite(B12, LOW);
    digitalWrite(R13, LOW); digitalWrite(G13, HIGH); digitalWrite(B13, LOW);
    digitalWrite(R21, LOW); digitalWrite(G21, HIGH); digitalWrite(B21, LOW);
    digitalWrite(R22, LOW); digitalWrite(G22, HIGH); digitalWrite(B22, LOW);
    digitalWrite(R23, LOW); digitalWrite(G23, HIGH); digitalWrite(B23, LOW);
    digitalWrite(R31, LOW); digitalWrite(G31, HIGH); digitalWrite(B31, LOW);
    digitalWrite(R32, LOW); digitalWrite(G32, HIGH); digitalWrite(B32, LOW);
    digitalWrite(R33, LOW); digitalWrite(G33, HIGH); digitalWrite(B33, LOW);
  }
  delay(300);
  //Apago todos
  digitalWrite(R11, LOW); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
  digitalWrite(R12, LOW); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
  digitalWrite(R13, LOW); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
  digitalWrite(R21, LOW); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
  digitalWrite(R22, LOW); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
  digitalWrite(R23, LOW); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
  digitalWrite(R31, LOW); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
  digitalWrite(R32, LOW); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
  digitalWrite(R33, LOW); digitalWrite(G33, LOW); digitalWrite(B33, LOW);
  delay(300);

}

void imprimirTablero(){
  
  for (int i = 0; i<3 ; i++){
    for (int j = 0; j<3; j++){
      Serial.print(tateti[i][j]); Serial.print(",");
    }
    Serial.println("");
  }

}

bool chequearVictoria(int cantMovs){

  //Si no hay al menos 5 fichas jugadas, nadie gano
  if (cantMovs <= 4) return false;

  //Las posibilidades de ganar con led 11
  if (tateti[0][0] == tateti[0][1] && tateti[0][1] == tateti[0][2]){ //F1
    Serial.print("Hay ganador!!!"); Serial.println(tateti[0][0]);
    ganador = tateti[0][0];
    return true;
  }else if (tateti[0][0] == tateti[1][0] && tateti[1][0] == tateti[2][0]){ //C1
    Serial.print("Hay ganador!!!"); Serial.println(tateti[0][0]);
    ganador = tateti[0][0];
    return true; 
  }else if (tateti[0][0] == tateti[1][1] && tateti[1][1] == tateti[2][2]){ //D1
    Serial.print("Hay ganador!!!"); Serial.println(tateti[0][0]);
    ganador = tateti[0][0];
    return true; 
  }else if (tateti[1][0] == tateti[1][1] && tateti[1][1] == tateti[1][2]){ //F2
    Serial.print("Hay ganador!!!"); Serial.println(tateti[1][0]);
    ganador = tateti[1][0];
    return true; 
  }else if (tateti[2][0] == tateti[2][1] && tateti[2][1] == tateti[2][2]){ //F3
    Serial.print("Hay ganador!!!"); Serial.println(tateti[2][0]);
    ganador = tateti[2][0];
    return true;
  }else if (tateti[2][0] == tateti[1][1] && tateti[1][1] == tateti[0][2]){ //D2
    Serial.print("Hay ganador!!!"); Serial.println(tateti[2][0]);
    ganador = tateti[2][0];
    return true; 
  }else if (tateti[0][1] == tateti[1][1] && tateti[1][1] == tateti[2][1]){ //C2
    Serial.print("Hay ganador!!!"); Serial.println(tateti[0][1]);
    ganador = tateti[0][1];
    return true; 
  }else if (tateti[0][2] == tateti[1][2] && tateti[1][2] == tateti[2][2]){ //C3
    Serial.print("Hay ganador!!!"); Serial.println(tateti[0][2]);
    ganador = tateti[0][2];
    return true; 
  }else {
    return false;
  }
      
  

}

void siguienteTurno(int jugActual){
  jugadorActual = (jugActual+1)%2;
  Serial.print("Turno de: "); Serial.println(jugadorActual);
}

void prenderLedJugado(int jugActual, char teclaPres){

  //O: rojo, 1: verde
  switch (teclaPres){
    case '1':
      //Si esta vacio el lugar
      if (tateti[0][0] == 11){
        if (jugActual == 0){
          //Prendo 1 rojo
          digitalWrite(R11, HIGH); digitalWrite(G11, LOW); digitalWrite(B11, LOW);
          tateti[0][0] = 0;
        }else {
          //Prendo 1 verde
          digitalWrite(R11, LOW); digitalWrite(G11, HIGH); digitalWrite(B11, LOW);    
          tateti[0][0] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }
      break;

    case '2':
      //Si esta vacio el lugar
      if (tateti[0][1] == 12){    
        if (jugActual == 0){
          //Prendo 2 rojo
          digitalWrite(R12, HIGH); digitalWrite(G12, LOW); digitalWrite(B12, LOW);
          tateti[0][1] = 0;
        }else {
          //Prendo 2 verde
          digitalWrite(R12, LOW); digitalWrite(G12, HIGH); digitalWrite(B12, LOW);    
          tateti[0][1] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }
      break;  

    case '3':
      //Si esta vacio el lugar
      if (tateti[0][2] == 13){    
        if (jugActual == 0){
          //Prendo 3 rojo
          digitalWrite(R13, HIGH); digitalWrite(G13, LOW); digitalWrite(B13, LOW);
          tateti[0][2] = 0;
        }else {
          //Prendo 3 verde
          digitalWrite(R13, LOW); digitalWrite(G13, HIGH); digitalWrite(B13, LOW);    
          tateti[0][2] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);  
        cantidadMovimientos--;
      }
      break;
      
    case '4':
      //Si esta vacio el lugar
      if (tateti[1][0] == 14){    
        if (jugActual == 0){
          //Prendo 4 rojo
          digitalWrite(R21, HIGH); digitalWrite(G21, LOW); digitalWrite(B21, LOW);
          tateti[1][0] = 0;
        }else {
          //Prendo 4 verde
          digitalWrite(R21, LOW); digitalWrite(G21, HIGH); digitalWrite(B21, LOW);    
          tateti[1][0] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;      
      }
      break;
      
    case '5':
      //Si esta vacio el lugar
      if (tateti[1][1] == 15){    
        if (jugActual == 0){
          //Prendo 5 rojo
          digitalWrite(R22, HIGH); digitalWrite(G22, LOW); digitalWrite(B22, LOW);
          tateti[1][1] = 0;
        }else {
          //Prendo 5 verde
          digitalWrite(R22, LOW); digitalWrite(G22, HIGH); digitalWrite(B22, LOW);    
          tateti[1][1] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }
      break;
      
    case '6':
      //Si esta vacio el lugar
      if (tateti[1][2] == 16){    
        if (jugActual == 0){
          //Prendo 6 rojo
          digitalWrite(R23, HIGH); digitalWrite(G23, LOW); digitalWrite(B23, LOW);
          tateti[1][2] = 0;
        }else {
          //Prendo 6 verde
          digitalWrite(R23, LOW); digitalWrite(G23, HIGH); digitalWrite(B23, LOW);    
          tateti[1][2] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }

      break;
      
    case '7':
      //Si esta vacio el lugar
      if (tateti[2][0] == 17){    
        if (jugActual == 0){
          //Prendo 7 rojo
          digitalWrite(R31, HIGH); digitalWrite(G31, LOW); digitalWrite(B31, LOW);
          tateti[2][0] = 0;
        }else {
          //Prendo 7 verde
          digitalWrite(R31, LOW); digitalWrite(G31, HIGH); digitalWrite(B31, LOW);    
          tateti[2][0] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }

      break;
        
    case '8':
      //Si esta vacio el lugar
      if (tateti[2][1] == 18){    
        if (jugActual == 0){
          //Prendo 8 rojo
          digitalWrite(R32, HIGH); digitalWrite(G32, LOW); digitalWrite(B32, LOW);
          tateti[2][1] = 0;
        }else {
          //Prendo 8 verde
          digitalWrite(R32, LOW); digitalWrite(G32, HIGH); digitalWrite(B32, LOW);    
          tateti[2][1] = 1;
        }
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }
      break;

    case '9':
      //Si esta vacio el lugar
      if (tateti[2][2] == 19){    
        if (jugActual == 0){
          //Prendo 9 rojo
          digitalWrite(R33, HIGH); digitalWrite(G33, LOW); digitalWrite(B33, LOW);
          tateti[2][2] = 0;
        }else {
          //Prendo 9 verde
          digitalWrite(R33, LOW); digitalWrite(G33, HIGH); digitalWrite(B33, LOW);    
          tateti[2][2] = 1;
        }  
      }else {
        Serial.println("Lugar ocupado");  
        encenderBuzzer(1,300);
        cantidadMovimientos--;
      }
      break;
      
  }//FIN switch(teclaPres)
}

char escucharTeclado(){

  char presionada = NO_KEY;
  char key  = NO_KEY;
  int cuentaRegresiva = 100;

  //Mientras no se apriete una tecla
  while (key == NO_KEY || !tiempoAgotado){
    //Me quedo esperando
   // Serial.println("Presione una tecla y luego confirme con #");
    presionada = keypad.getKey();

    //Si presionaron una tecla y no es #
    if (presionada != NO_KEY && presionada != '#'){
      delay(100);
      //Muestro la tecla presionada
      //Serial.println(presionada);
      encenderBuzzer(1,50);
      //Espero confirmacion (#)
      key = keypad.getKey();

      //Mientras no presionen confirmacion o cambien de opinion
      while (key == NO_KEY){
       // Serial.println("Presione # para confirmar.");
        key = keypad.getKey();
        //Si apretaron una tecla y es la confirmacion
        if (key != NO_KEY && key == '#'){
          //Devuelvo la tecla presionada
          encenderBuzzer(1,50);
          return presionada;

         //Si cambiaron de opinion y no confirmaron (pero apretaron otra tecla)
        }else if (key != NO_KEY){
          //Actualizo la tecla presionada
          presionada = key;
          //Muestro nueva tecla presionada
         // Serial.println(presionada);
          //Seteo vacio para esperar confirmacion (#)
          key = NO_KEY;
        }
        delay(100);
      }
      
    }
    delay(100);
    cuentaRegresiva--;
    //Serial.println(cuentaRegresiva);
    //Si se agota el tiempo, devuelve 0
    if (cuentaRegresiva == 0 && fichasPuestas) return '0';
  }
}

void mostrarTurnoJugador(int actual){
  
  if (actual == 0){
    //muestro color Rojo en el led del turno
    digitalWrite(RLT, HIGH); digitalWrite(BLT, LOW); digitalWrite(GLT, LOW);
  }else{
    //muestro color Verde en el led del turno
    digitalWrite(GLT, HIGH); digitalWrite(BLT, LOW); digitalWrite(RLT, LOW);
  }

}


//FIN FUNCIONES DE JUEGO


//Test del Teclado
void testTeclado(){
  char key = keypad.getKey();
  if (key != NO_KEY){
    Serial.println(key); //print to serial the key that has been pressed
  }
}


//Test de los 10 leds RGBs
void testLedsRGB(){

  digitalWrite(B11,HIGH);
  delay(100);
  digitalWrite(B11,LOW);
  digitalWrite(G11,HIGH);
  delay(100); 
  digitalWrite(G11,LOW);
  digitalWrite(R11,HIGH);
  delay(100);
  digitalWrite(R11,LOW);
  delay(300);

  digitalWrite(B12,HIGH);
  delay(100);
  digitalWrite(B12,LOW);
  digitalWrite(G12,HIGH);
  delay(100); 
  digitalWrite(G12,LOW);
  digitalWrite(R12,HIGH);
  delay(100);
  digitalWrite(R12,LOW);
  delay(300);

  digitalWrite(B13,HIGH);
  delay(100);
  digitalWrite(B13,LOW);
  digitalWrite(G13,HIGH);
  delay(100); 
  digitalWrite(G13,LOW);
  digitalWrite(R13,HIGH);
  delay(100);
  digitalWrite(R13,LOW);
  delay(300);

  digitalWrite(B21,HIGH);
  delay(100);
  digitalWrite(B21,LOW);
  digitalWrite(G21,HIGH);
  delay(100); 
  digitalWrite(G21,LOW);
  digitalWrite(R21,HIGH);
  delay(100);
  digitalWrite(R21,LOW);
  delay(300);

  digitalWrite(B22,HIGH);
  delay(100);
  digitalWrite(B22,LOW);
  digitalWrite(G22,HIGH);
  delay(100); 
  digitalWrite(G22,LOW);
  digitalWrite(R22,HIGH);
  delay(100);
  digitalWrite(R22,LOW);
  delay(300);

  digitalWrite(B23,HIGH);
  delay(100);
  digitalWrite(B23,LOW);
  digitalWrite(G23,HIGH);
  delay(100); 
  digitalWrite(G23,LOW);
  digitalWrite(R23,HIGH);
  delay(100);
  digitalWrite(R23,LOW);
  delay(300);

  digitalWrite(B31,HIGH);
  delay(100);
  digitalWrite(B31,LOW);
  digitalWrite(G31,HIGH);
  delay(100); 
  digitalWrite(G31,LOW);
  digitalWrite(R31,HIGH);
  delay(100);
  digitalWrite(R31,LOW);
  delay(300);

  digitalWrite(B32,HIGH);
  delay(100);
  digitalWrite(B32,LOW);
  digitalWrite(G32,HIGH);
  delay(100); 
  digitalWrite(G32,LOW);
  digitalWrite(R32,HIGH);
  delay(100);
  digitalWrite(R32,LOW);
  delay(300);

  digitalWrite(B33,HIGH);
  delay(100);
  digitalWrite(B33,LOW);
  digitalWrite(G33,HIGH);
  delay(100); 
  digitalWrite(G33,LOW);
  digitalWrite(R33,HIGH);
  delay(100);
  digitalWrite(R33,LOW);
  delay(300);

  digitalWrite(BLT,HIGH);
  delay(100);
  digitalWrite(BLT,LOW);
  digitalWrite(GLT,HIGH);
  delay(100); 
  digitalWrite(GLT,LOW);
  digitalWrite(RLT,HIGH);
  delay(100);
  digitalWrite(RLT,LOW);
  delay(300);
  
}

void encenderBuzzer(int veces, int intervalo){

  for (int i = 0; i < veces; i++){
    digitalWrite(BUZ,HIGH);
    delay(intervalo);
    digitalWrite(BUZ,LOW);
    delay(intervalo);
  }
}





void actualizarPosiciones(int jugador, int ID_ficha, char posicion_Destino) {
    if (jugador == 0) { // Jugador rojo
        Posiciones_Rojo[ID_ficha - 1] = posicion_Destino;
    } else { // Jugador verde
        Posiciones_Verde[ID_ficha - 1] = posicion_Destino;
    }

    // Imprimir por puerto serie
    Serial.print("mov: ");
    if (jugador == 0) {
        Serial.print("R");
    } else {
        Serial.print("V");
    }
    Serial.print(ID_ficha);
    Serial.print(",");
    Serial.println(posicion_Destino);

    // Imprimir el estado actual de las posiciones
    Serial.print("posiciones_rojo: [");
    for (int i = 0; i < 3; i++) {
        Serial.print(Posiciones_Rojo[i]);
        if (i < 2) Serial.print(",");
    }
    Serial.println("]");

    Serial.print("posiciones_verde: [");
    for (int i = 0; i < 3; i++) {
        Serial.print(Posiciones_Verde[i]);
        if (i < 2) Serial.print(",");
    }
    Serial.println("]");
    Ficha_actual += 1;

}


void moverFicha(int jugador, int ID_ficha, int posicion_Destino) {
    // Verificar si la nueva posición está ocupada
    if (posicionOcupada(posicion_Destino)) {
        Serial.println("Error: la posición de destino ya está ocupada.");
        return;  // No se puede mover la ficha
    }
}


bool posicionOcupada(int posicion_Destino) {
    // Verifica si la posición está ocupada por cualquier ficha roja o verde
    for (int i = 0; i < 3; i++) {
        if (Posiciones_Rojo[i] == posicion_Destino || Posiciones_Verde[i] == posicion_Destino) {
            return true;  // Posición ocupada
        }
    }
    return false;  // Posición libre
}
