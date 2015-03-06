#include <Arduino.h>
#include "Temp.h"

void delayW(int ms)
{
  unsigned long int time = millis();
  
  while( (millis()-time) <= ms)
  {
  }
  
}

void setPwmTimer2()
{
   // Configurar timer 2 con factor de preescalado de 64
   TCCR2B |= (1<<CS22);
   
   // Configurar timer 2 en phase-correct pwm (8-bit)
   TCCR2A |= (1<<WGM20);  
}
