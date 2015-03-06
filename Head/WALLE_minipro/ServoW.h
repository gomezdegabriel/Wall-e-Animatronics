#ifndef SERVOW_H
#define SERVOW_H

struct _SERVOW
{

	Servo theServo;
	int maxAngle;
	int minAngle;
	volatile int pos;
	
	void init(int pin, int _minAngle, int _maxAngle, int posIni);
        void init(int _pin, int _minAngle, int _maxAngle, int minPulse, int maxPulse, int posIni);
	void moveTo(int data, bool porcent);
	void moveSoft(int porcent, int velocidad);
	
};

#endif

