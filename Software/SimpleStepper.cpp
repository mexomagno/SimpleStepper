#include "Arduino.h"
#include "SimpleStepper.h"
#define NSTEPS 8
#define NPINS 4

const unsigned char _steps[]={B00001000,B00001100,B00000100,B00000110,B00000010,B00000011,B00000001,B00001001};
/*Definición del constructor.
	El constructor acepta pins del 0 al 7. Cualquier valor distinto tendrá comportamientos raros.*/
SimpleStepper::SimpleStepper(int pin1, int pin2, int pin3, int pin4){
	//inicializar pines
	_pins[0]=pin1;
	_pins[1]=pin2;
	_pins[2]=pin3;
	_pins[3]=pin4;
	for (int i = 0; i < NPINS; i++){
		pinMode(_pins[i], OUTPUT);
	}
	//valores iniciales de posicion, velocidad maxima y aceleracion
	_vmax=30;
	_pos=0;
	_a=4;
	_step_index=0;
	step(CLOCK);
}
/*step Mueve el rotor un paso, reloj o contrarreloj definido por dir. */
void SimpleStepper::step(Dir dir){
	//asignar nuevo subíndice
	_step_index= dir == CCLOCK ? (_step_index+1)%NSTEPS :  (_step_index + NSTEPS -1)%NSTEPS;
	//setear nuevo step en los pines correspondientes. Ser eficiente
	for (int i = 0; i < NPINS; i++){
		/*//clear
		PORTD &= ~(1<<_pins[i]);
		//set
			//crear máscara con bit para el pin i, según el step actual
		unsigned char MASK=(~_steps[_step_index] & (1<<i))>>i; //ojo, MASK contiene el inverso del bit a setear en el pin, guardado en el bit menos significativo. Algo como 0000000X con X = ~bit
			//setear en PORTD
		PORTD &= ~(MASK<<_pins[i]);
		//bit seteado*/
		digitalWrite(_pins[i],(_steps[_step_index] & (1 << i)) >> i);
	}
}
