#include <Arduino.h>
#include <Servo.h>  // Inutiliza PWM en pines 9 y 10
#include "Temp.h"
#include "ServoW.h"

void _SERVOW:: init(int _pin, int _minAngle, int _maxAngle, int posIni)
{
	this-> minAngle = _minAngle;
	this-> maxAngle = _maxAngle;
	this-> theServo.attach(_pin);
	this->moveTo(posIni,true);


}

void _SERVOW:: init(int _pin, int _minAngle, int _maxAngle, int minPulse, int maxPulse, int posIni)
{
	this-> minAngle = _minAngle;
	this-> maxAngle = _maxAngle;
	this-> theServo.attach(_pin, minPulse, maxPulse);

	this->moveTo(posIni,true);


}

void _SERVOW::  moveTo(int data, bool porcent)
{
  
  if (porcent)
  {
	if(data < -100)
		data=-100;
	
	if (data>100)
		data=100;

	int newPos = map(data, -100, 100, this->minAngle, this->maxAngle);

	this->theServo.write(newPos);

        this->pos=newPos;

  }
  else
  {
        this->theServo.write(data);
        this->pos=data;
  }

}

void _SERVOW::  moveSoft(int porcent, int velocidad)
{
	if(porcent < -100)
		porcent=-100;
	
	if (porcent>100)
		porcent=100;

        // Si la velocidad es nula, salimos
        if(velocidad==0) { return;}
   
        // Esperaremos entre 0 y 200 ms
        int ms= map(velocidad, 1, 100, 100,0);
        
	int newPos = map(porcent, -100, 100, this->minAngle, this->maxAngle);

        if(newPos > this->pos)
        {
        	for(int i= this->pos; i <= newPos; i++)
        	{
        		this->theServo.write(i);
        		if (ms !=0) {delayW(ms);} // Llamar a la funcion provoca un aumento de tiempo considerable
        	}
        }
        else
        {
                for(int i= this->pos; i >= newPos; i--)
        	{
        		this->theServo.write(i);
        		if (ms !=0) {delayW(ms);} // Llamar a la funcion provoca un aumento de tiempo considerable
        	}
        }
        
        this->pos= newPos;

}
