/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int addr = 0;
byte alto=0;
byte bajo=0;
unsigned int valor=0;

void setup()
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  
  //Direccion I2C del dispositivo
  //EEPROM.write(0, 4); 
 // delay(200);
  
//---------------------------------------------------
  
  //Configuracion Servo Inclinaciom
  valor=700;             // Valor minimo del pulso
  bajo= valor;
  alto= valor>>8;
  
  EEPROM.write(1, alto); 
  delay(200);

  EEPROM.write(2, bajo); 
  delay(200);
  
  valor=2300;            // Valor maximo del pulso
  bajo= valor;
  alto= valor>>8;
  
  EEPROM.write(3, alto); 
  delay(200);

  EEPROM.write(4, bajo); 
  delay(200);

//---------------------------------------------------

  // Configuracion Servo Rotacion Hemisferio
  valor=620;             // Valor minimo del pulso
  bajo= valor;
  alto= valor>>8;
  
  EEPROM.write(5, alto); 
  delay(200);

  EEPROM.write(6, bajo);
  
  valor=2100;            // Valor maximo del pulso
  bajo= valor;
  alto= valor>>8;
  EEPROM.write(7, alto);  
  delay(200);

  EEPROM.write(8, bajo);
  
  
//---------------------------------------------------
  // Configuracion Servo Ceja Interior
  valor=650;            // Valor minimo del pulso
  bajo= valor;
  alto= valor>>8;
  EEPROM.write(9, alto);  
  delay(200);
  
  EEPROM.write(10, bajo); 
  delay(200);


  valor=2070;            // Valor maximo del pulso
  bajo= valor;
  alto= valor>>8;
  EEPROM.write(11, alto);  
  delay(200);
  
  EEPROM.write(12, bajo); 
  delay(200);

  //---------------------------------------------------
  // Configuracion Servo Ceja Exterior
  valor=645;            // Valor minimo del pulso
  bajo= valor;
  alto= valor>>8;
  EEPROM.write(13, alto);  
  delay(200);
  
  EEPROM.write(14, bajo); 
  delay(200);


  valor=2400;            // Valor maximo del pulso
  bajo= valor;
  alto= valor>>8;
  EEPROM.write(15, alto);  
  delay(200);
  
  EEPROM.write(16, bajo); 
  delay(200);

//---------------------------------------------------
  
  //Configuracion Servo Inclinacion
  bajo =  0 ; // Angulo minimo
  alto = 140;   // Angulo maximo
  EEPROM.write(17, bajo); 
  delay(200);

  EEPROM.write(18, alto); 
  delay(200);

//---------------------------------------------------
  
  //Configuracion Servo Giro Hemisferio
  bajo= 90;    // Angulo minimo
  alto= 130;   // Angulo maximo
  EEPROM.write(19, bajo); 
  delay(200);

  EEPROM.write(20, alto); 
  delay(200);


//---------------------------------------------------
  
  //Configuracion Servo Ceja Interior
  bajo= 0; // Angulo minimo
  alto= 180;   // Angulo maximo
  EEPROM.write(21, bajo); 
  delay(200);

  EEPROM.write(22, alto); 
  delay(200);

//---------------------------------------------------
  
  //Configuracion Servo Ceja Exterior
  bajo= 0; // Angulo minimo
  alto= 180;   // Angulo maximo
  EEPROM.write(23, bajo); 
  delay(200);

  EEPROM.write(24, alto); 
  delay(200);
  
}

void loop()
{

  delay(10000);
}
