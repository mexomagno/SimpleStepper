#include "Arduino.h"
#include "SimpleStepper.h"
#define NSTEPS 8
#define NPINS 4
const unsigned char _steps[]={B00001000,B00001100,B00000100,B00000110,B00000010,B00000011,B00000001,B00001001};
/*Definición del constructor. SimpleStepper(pin1, pin2, pin3, pin4)
	El constructor acepta pins del 0 al 7. Cualquier valor distinto tendrá comportamientos raros.*/
	SimpleStepper::SimpleStepper(int pin1, int pin2, int pin3, int pin4){
		//fija valor por defecto de steps por vuelta
		init(200, pin1, pin2, pin3, pin4);
	}
	/* Constructor alternativo. Recibe cuántos steps da por vuelta, y los pines */
	SimpleStepper::SimpleStepper(int steps_vuelta, int pin1, int pin2, int pin3, int pin4){
		init(steps_vuelta, pin1, pin2, pin3, pin4);
	}
/*Modalidades de giro */
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
	void SimpleStepper::turnSteps(long N, Dir dir){
		turn(N, dir);
	}
	void SimpleStepper::turnRev(float N, Dir dir){
		long new_N=(long)(N*N_MOTOR_STEPS);
		turn(new_N, dir);
	}
	void SimpleStepper::turnDeg(float G, Dir dir){
		long new_N=(long)(N_MOTOR_STEPS*G*1.0/360.0);
		turn(new_N, dir);
	}
/*Setters*/
	void SimpleStepper::setMotorSteps(int n){
		if ((n>=4)&&(n<65536)){
			N_MOTOR_STEPS=n;
		}
		else
			fprintf(stderr, "ERROR: invalid value for motor steps (must be between 4 and 65535)\n");
	}
	void SimpleStepper::setPos(int n){
		_pos=n;
	}
	void SimpleStepper::setVMax(float v){
		if ((v>0)&&(v<65536)){
			_vmax=v;
		}
		else
			fprintf(stderr, "ERROR: invalid value for max speed (must be between 0+ and 65535)\n");
	}
	void SimpleStepper::setAcc(float a){
		if ((a>-65536)&&(a<65536)){
			_a=a;
		}
		else
			fprintf(stderr, "ERROR: invalid value for acceleration (must be between -65535 and 65535)\n");
	}
/*Getters*/
	int SimpleStepper::getMotorSteps(){
		return N_MOTOR_STEPS;
	}
	int SimpleStepper::getPos(){
		return _pos;
	}
	float SimpleStepper::getVMax(){
		return _vmax;
	}
	float SimpleStepper::getAcc(){
		return _a;
	}
/***********	MÉTODOS PRIVADOS	***********/
void SimpleStepper::init(int steps_vuelta, int pin1, int pin2, int pin3, int pin4){
	//inicializar pines
	_pins[0]=pin1;
	_pins[1]=pin2;
	_pins[2]=pin3;
	_pins[3]=pin4;
	for (int i = 0; i < NPINS; i++){
		pinMode(_pins[i], OUTPUT);
	}
	//PRESETS para valores internos. Seteables luego con setters.
	_vmax=30;
	_pos=0;
	_a=4;
	_step_index=0;
	N_MOTOR_STEPS=steps_vuelta; //valor por defecto de steps por vuelta
	step(CLOCK);
}
//turn(N, dir) efectivamente gira N pasos en dirección dir. Acá ocurre la magia de la aceleración.
void SimpleStepper::turn(long N, Dir dir){
	if (N<=0) {
		fprintf(stderr, "ERROR: invalid step number (must be greater than 0)\n");
		return;
	}//tomar en cuenta que independiente de la aceleración, se deben dar N steps exactamente.
	unsigned long tpos=0,tvel=0,dt,t0,t1; //en microsegundos. Velocidad y aceleración están en segundos. 1s=1000000ms
	float vel,v0=0,v_posible,p0=0;
	float pos=0;
	int half_N=floor(((float)N)/2.0);
	//steps recorridos cuando se llegó a vmax
	int steps_vmax=-1;
	//steps ya concluidos
	int cont_steps=0;
	t0=micros();
	while (cont_steps<N){ //busyWaiting para dar pasos
		Serial.print("tvel: ");
		Serial.print(tvel/1000000.0);
		Serial.print(" V: ");
		Serial.print(vel);
		Serial.print(" paso: ");
		Serial.print(cont_steps);
		Serial.print(" pos: ");
		Serial.println(pos);
		t1=micros();
		dt=t1-t0;
		tpos+=dt;
		tvel+=dt;
		t0=micros();
		//obtengo velocidad de este instante. Deja de aumentar si se llegó a vmax.
		v_posible=v0+_a*(tvel/1000000.0);
		if (v_posible >= _vmax){
			if (steps_vmax==-1){
				vel=_vmax;
				steps_vmax=cont_steps+1;
			}
		}
		else{
			vel= v_posible >= 0 ? v_posible : 0;
		}
		//obtengo posición teórica. Si pasa a un entero, hay que dar un step.
		pos=p0+vel*(tpos/1000000.0);
		if ((pos-cont_steps)>=1){//quiere decir que puedo dar un step
			step(dir);
			cont_steps++;
			if (((cont_steps+steps_vmax==N))||((cont_steps==half_N)&&(vel<_vmax))){
				//si la velocidad ya es máxima pero  se está llegando al punto donde hay que desacelerar, se desacelera
				//O si la velocidad máxima aún no se alcanza pero voy en la mitad, debo desacelerar.
				_a*=-1; //**OJO: SE ESTÁ MODIFICANDO VARIABLE INTERNA QUE DEBIERA SER CONSTANTE **/
				v0=vel;
				tvel=0;
				tpos=0;
				p0=pos;
			}
		}
	}
	_a*=-1;//**OJO: SE ESTÁ MODIFICANDO VARIABLE INTERNA QUE DEBIERA SER CONSTANTE **//
}

/*
Versión 0.1
El problema de los steps:
	-Es claro que se necesita tener una noción de la aceleración del motor. Si se pide dar N steps, estos deben darse
	acelerada y desaceleradamente hasta recorrer exactamente los steps pedidos.
	Formas posibles de mover el motor:
		1) Dar un solo step
		2) Dar N steps
		3) Dar N vueltas, con N un float
		4) Comenzar a girar infinitamente. Lógicamente esta función se complementa con una función Detenerse.

	Dadas estas variantes, se puede ver primeramente que:
		a) un solo step no se ve afectado por aceleración.
		b) Dar N steps toma en cuenta la aceleración. Se implementa haciendo uso de 1) y delay.
		c) Dar N vueltas, con N float, no es más que un caso particular de 2) y se implementa con 2), previa conversión vueltas -> steps
		d) Girar infinito es un caso general de 2) y se puede implementar con el mismo, quizás con N=-1 identificar ese paso.
		El problema de este caso es que como nunca termina, nunca devuelve el control al programa del arduino, por lo que
		nunca se podrá llamar a Detenerse. Este caso quedará para implementar después quizás haciendo uso de los timers.
		Consultar http://playground.arduino.cc/Main/LibraryList#Timing para esa etapa.

	Dicho esto, se implementarán 1 2 y 3 en esta versión.
	Cosas por mejorar:
		1) Eliminar dependencia de "digitalWrite" para función step(dir)
		2) Implementar "paralelismo" en el funcionamiento del motor, osea, usar timer interrupts
		3) 
*/
