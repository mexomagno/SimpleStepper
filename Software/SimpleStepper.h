#include "WProgram.h"

#ifndef SimpleStepper_h
#define SimpleStepper_h
class SimpleStepper{
	public:
	//métodos y variables públicos
	SimpleStepper(int pin1, int pin2, int pin3, int pin4);
	void step(boolean dir);
	void 
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
	unsigned char _steps[8];
	//indice del step actual
	int _step_index;
};
#endif
