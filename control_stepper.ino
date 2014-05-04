#include <SimpleStepper.h>

/*SETUP:  
        -Pines 2, 3, 4 y 5 unidos a cables Blanco, Naranjo, Amarillo y Verde. Blanco corresponde a café en el motor.
        -Boton a pin 6 y pin 7. Simbolizan giro reloj y contrarreloj respectivamente.
        
        */
SimpleStepper motor=SimpleStepper(200,2,3,4,5);
unsigned char button_pins[]={6,7};
boolean button_pressed[]={false,false};

boolean debug=0;

void setup(){
  pinMode(button_pins[0],OUTPUT);
  pinMode(button_pins[1],OUTPUT);
  if (debug){
    pinMode(13,OUTPUT); //debug
  }
  if (debug) Serial.begin(9600);
  motor.setVMax(900);
  motor.setAcc(9000);
  
}

int indice=0;
boolean girar=false;
boolean direccion=false;

//funcion para testear y jugar con el motor
void revisarBotones(){
  for(int i=0;i<2;i++){
    if (digitalRead(button_pins[i])){
      if (!button_pressed[i]){
        //acciones a tomar al presionar boton
        //motor.step(i==0 ? CLOCK : CCLOCK);
        direccion = i==0 ? false : true;
        if (debug){
          digitalWrite(13,HIGH);
        }
        //seteos finales
        button_pressed[i]=true;
        girar=true;
        //debounce
        delay(5);
      }
    }
    else {
      if (button_pressed[i]){
        //acciones a tomar al soltar boton
        if (debug){
          digitalWrite(13,LOW);
        }
        //seteos finales
        button_pressed[i]=false;
        girar=false;
        //debounce
        delay(5);
      }
    }
  }
}
int cont=0;
boolean dir=false;
void loop(){
  //revisarBotones();
  /*if (girar){
    motor.step(dir ? CLOCK : CCLOCK);
    delay(4);
  }*/
  motor.turnDeg(90,CLOCK);
  delay(50);
  cont++;
  if (cont>=3){
    motor.turnDeg(180,CCLOCK);
    delay(50);
    cont=0;
  }
  
}
