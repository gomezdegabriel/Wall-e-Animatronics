/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it 
 * to the computer.
 * This example code is in the public domain.
 */

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
byte value=0;
byte alto=0;
byte bajo=0;
unsigned int valor=0;
byte address=0;

void setup()
{
  // initialize serial and wait for port to open:
    Serial.begin(9600);
  
  // read a byte from the current address of the EEPROM
    value = EEPROM.readByte(address);
    
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
    
    valor = EEPROM.readInt(1);
    
        Serial.print(1);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
    
    valor = EEPROM.readInt(3);
         Serial.print(3);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
    
    valor = EEPROM.readInt(5);
        Serial.print(5);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
        
    valor = EEPROM.readInt(7);
        Serial.print(7);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
        
     valor = EEPROM.readInt(9);
        Serial.print(9);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
        
     valor = EEPROM.readInt(11);
        Serial.print(11);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
        
     valor = EEPROM.readInt(13);
        Serial.print(13);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
        
     valor = EEPROM.readInt(15);
        Serial.print(15);
        Serial.print("\t");
        Serial.print(valor, DEC);
        Serial.println();
   
}

void loop()
{
    delay(10000);

}
