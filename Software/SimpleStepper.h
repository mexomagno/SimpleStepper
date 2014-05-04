#include "Arduino.h"
#ifndef SimpleStepper_h
#define SimpleStepper_h
typedef enum direccion{
	CLOCK,
	CCLOCK
}Dir;
class SimpleStepper{
	public:
	/******métodos y variables públicos******/
		/*Constructor sin definir steps por vuelta*/
		SimpleStepper(int pin1, int pin2, int pin3, int pin4);
		/*Constructor definiendo explícitamente steps por vuelta. ELEGIR ESTE*/
		SimpleStepper(int steps_vuelta, int pin1, int pin2, int pin3, int pin4);
		/*step(dir) da un step en dirección dir*/
		void step(Dir dir);
		/*turnSteps(N, dir) gira N steps en dirección dir. OJO que para cada turnX() se truncará
		el valor de giro por uno válido segun número de steps.
		OJO también que, si bien se cuenta con half-stepping, el usuario ingresa steps compatibles
		con el número de steps que da el motor por naturaleza. Es decir, 1 step en realidad son 2 half-steps
		y no se puede dar 1 half-step. */
		void turnSteps(long N, Dir dir);
		/*turnRev(N, dir) gira N revoluciones en dirección dir*/
		void turnRev(float N, Dir dir);
		/*turnDeg(G, dir) gira G grados sexagesimales en dirección dir*/
		void turnDeg(float G, Dir dir);
		/*Setters*/
		void setMotorSteps(int n);
		void setPos(int n);
		void setVMax(float v);
		void setAcc(float a);
		/*Getters*/
		int getMotorSteps();
		int getPos();
		float getVMax();
		float getAcc();
	private:
	/******métodos y variables privados******/
			/*VARIABLES*/
		/*cantidad de steps por revolución*/
		int N_MOTOR_STEPS;
		/*posicion en steps, contado desde el inicio del programa*/
		int _pos;
		/*velocidad maxima en steps por segundo*/
		float _vmax;
		/*aceleracion en steps por segundo cuadrado*/
		float _a;
		/*delay, se fija con la aceleración*/
		unsigned long _delay;
		/*pins*/
		unsigned char _pins[4];
		/*indice del step actual*/
		int _step_index;

			/*MÉTODOS*/
		/*turn(N, dir) gira N pasos en dirección dir. Usado para implementar turnSteps(N,dir), turnRev(N,dir)*/
		void turn(long N, Dir dir);
		void init(int steps_vuelta, int pin1, int pin2, int pin3, int pin4);
};

#endif
