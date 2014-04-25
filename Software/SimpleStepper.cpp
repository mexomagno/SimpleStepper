#include "WProgram.h"
#include "SimpleStepper.h"
#define NSTEPS 8

//Definición del constructor
SimpleStepper::SimpleStepper(int pin1, int pin2, int pin3, int pin4){
	_pins[0]=pin1;
	_pins[1]=pin2;
	_pins[2]=pin3;
	_pins[3]=pin4;
	for (int i = 0; i < 4; i++){
		pinMode(_pins[i], OUTPUT);
	}
	//valores iniciales de posicion, velocidad maxima y aceleracion
	_vmax=30;
	_pos=0;
	_a=4;
	_steps[0]={B00001000,B00001100,B00000100,B00000110,B00000010,B00000011,B00000001,B00001001};
	_step_index=0;
}

SimpleStepper::step(boolean dir){
	_step_index= dir ? (_step_index+1)%NSTEPS : 
}
