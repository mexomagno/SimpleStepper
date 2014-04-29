#include "Arduino.h"

#ifndef SimpleStepper_h
#define SimpleStepper_h
typedef enum direccion{
	CLOCK,
	CCLOCK
}Dir;
class SimpleStepper{
	public:
	//métodos y variables públicos
	SimpleStepper(int pin1, int pin2, int pin3, int pin4);
	void step(Dir dir);
	private:
	//métodos y variables privados
	//posicion en steps, contado desde el inicio del programa
	int _pos;
	//velocidad maxima
	int _vmax;
	//aceleracion
	int _a;
	//pins
	unsigned char _pins[4];
	//arreglo de steps
	//unsigned char _steps[8];
	//indice del step actual
	int _step_index;
};

#endif
