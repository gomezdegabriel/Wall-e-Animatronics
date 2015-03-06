#include <Wire.h>    // Comunicacion TWI/I2C
#include <Servo.h>   // Inutiliza PWM en pines 9 y 10 (Usa Timer1)
#include "ServoW.h"
#include "Eyes.h"    // Inutiliza PWM en pines 5 y 6 (Usa Timer0). Usa Timer2
#include "Temp.h"
#include "Brow.h"
#include <EEPROM.h>

/************************************
 * Declaracion de constantes         *
 *************************************/
// Pines disponibles para los motores
#define PIN_MOTOR1 10
#define PIN_MOTOR2 12
#define PIN_MOTOR3 13
#define PIN_MOTOR4 14
#define PIN_MOTOR5 15

// Pines disponibles para medidas analogicas
#define PIN_SENSOR1 16
#define PIN_SENSOR2 17

// Numero de registros I2C
#define NUM_REGISTERS 22


/************************************
 * Declaracion de variables globales *
 *************************************/
 
_SERVOW servoLateral;    // Servo Ceja Derecha Interior 
_SERVOW servoGiro;       // Servo Ceja Derecha Exterior
_BROW ceja;              // Grupo servos de la Ceja
_EYE eye;                // Grupo de Leds del Ojo


// Medida en cm de sensor analogico de IR
float cm;

// Flag de nuevo dato
volatile bool newData=false; 

//Variable auxiliar para convertir char en int
char char2int;    

// ------- Registros I2C -------
// ---------- R/W --------------
// registro 0x00    Señal sincronizacion de servos
// registro 0x01    Velocidad conjunto
// registro 0x02    Posicion Deseada Servo Cuello en %
// registro 0x03    Posicion Deseada Servo Hemisferio en %
// registro 0x04    Posicion Deseada Ceja (modo 0,1,2,3)
// registro 0x05    Tipo Ojo
// registro 0x06    Brillo de leds
// registro 0x07    Tiempo cambio de brillo

// ------------ R -------------
// registro 0x08   Movimiento Completado(1)- Movimiento en curso(0)

// registro 0x1A    Posicion Actual Servo Cuello en grados
// registro 0x1B    Posicion Actual Servo Hemisferio en grados
// registro 0x1C    Posicion Actual Servo Ceja interior en grados
// registro 0x1D    Posicion Actual Servo Ceja exterior en grados

// registro 0x10   Estado Led 0
// registro 0x11   Estado Led 1
// registro 0x12   Estado Led 2
// registro 0x13   Estado Led 3
// registro 0x14   Estado Led 4
// registro 0x15   Estado Led 5
// registro 0x16   Estado Led 6
// registro 0x17   Estado Led 7
// registro 0x18   Medida de sensor Analogico Distancia

 
volatile int r[NUM_REGISTERS];
// Flag que indica si se ha enviado alguna nueva posicion
volatile bool newMovement=false;
// Ultimo registro en que se uso
volatile byte numRegister;

/*******************************************************************************************
 *           Funciones               *
 *************************************/
void moveAll(bool sync);
int distance(int pin);   
void requestEvent();
void receiveEvent(int howMany);

/*******************************************************************************************
*      Configuracion Inicial        *
 *************************************/
void setup()
{
     // Serial.begin(9600);              //Iniciamos puerto serie a 96000 baudios

    // Direccion I2C del microcontrolador
    // almacenada en la direccion 0 de la EEPROM
    const uint8_t dir =  EEPROM.readByte(0);
    
    // Configuracion de los servos
    const unsigned int minPulseInclinacion =  EEPROM.readInt(1);
    const unsigned int maxPulseInclinacion =  EEPROM.readInt(3);
    const unsigned int minPulseHemisferio  =  EEPROM.readInt(5);  
    const unsigned int maxPulseHemisferio  =  EEPROM.readInt(7);      
    const unsigned int minPulseCejaI = EEPROM.readInt(9);    
    const unsigned int maxPulseCejaI = EEPROM.readInt(11);
    const unsigned int minPulseCejaE = EEPROM.readInt(13);
    const unsigned int maxPulseCejaE = EEPROM.readInt(15);
    
    const unsigned int minAngleInclinacion = EEPROM.readByte(17);
    const unsigned int maxAngleInclinacion = EEPROM.readByte(18);
    const unsigned int minAngleHemisferio = EEPROM.readByte(19);
    const unsigned int maxAngleHemisferio = EEPROM.readByte(20);
    const unsigned int minAngleCejaI = EEPROM.readByte(21);
    const unsigned int maxAngleCejaI = EEPROM.readByte(22);
    const unsigned int minAngleCejaE = EEPROM.readByte(23);
    const unsigned int maxAngleCejaE = EEPROM.readByte(24);
    
   // Preparamos los servos
   servoLateral.init(PIN_MOTOR1,minAngleInclinacion, maxAngleInclinacion, minPulseInclinacion, maxPulseInclinacion, 0);   
   servoGiro.init(PIN_MOTOR2,minAngleHemisferio, maxAngleHemisferio, minPulseHemisferio, maxPulseHemisferio, 0);
   ceja.init(PIN_MOTOR3, minAngleCejaI, maxAngleCejaI, minPulseCejaI, maxPulseCejaI, PIN_MOTOR4, minAngleCejaE, maxAngleCejaE, minPulseCejaE, maxPulseCejaE);

  // Configuramos el ojo
   eye.init();                
   eye.openEye(255,100);            // Abrimos el ojo

   analogReference(DEFAULT);        // Referencia analogica de 5 voltios
   
   Wire.begin(dir);                 // Asigamos la direccion al microcontrolador              
   Wire.onRequest(requestEvent);    // Evento para cuando el master pida datos
   Wire.onReceive(receiveEvent);    // Evento para cuando el master envia datos

  r[8]=1;
}

/*******************************************************************************************
 *         Programa principal        *
 *************************************/
void loop()
{

   if (newData)
    {
        newData=false;
        
        // Si hay un nuevo movimiento
        if(newMovement)
        {
            newMovement=false;
            r[8]=0;
            
            // Para servos sincronizados
            if(r[0] >= 1)
            {
               moveAll(true);
            }
            // Para sevos no sincronizados
            else
            {
              moveAll(false);
            }
            
            r[8]=1;
        }
          
        // Si el brillo ha sido modificado
        if (eye.bright != r[6])
        {
            
            eye.setBright(map(r[6],0, 100, 0, 255), map(r[7],0, 100, 0, 2000));
        }
        
        // Gesto del ojo
        eye.mode(r[5]);             
    }
    
}

/*******************************************************************************************
 *           Funciones               *
 *************************************/

void moveAll (bool sync)
{
    int pos1, pos2;
    int pos[4];
    int inc[4];
    int dif[4];
    int difMax=0;   
    
    // Posiciones actuales
    pos[0]= servoLateral.pos;
    pos[1]= servoGiro.pos;
    pos[2]= map(ceja.servoI.pos, ceja.servoI.minAngle, ceja.servoI.maxAngle, 270, 90);
    pos[3]= map(ceja.servoE.pos, ceja.servoE.minAngle, ceja.servoE.maxAngle, 270, 90);
    
    if(r[1]==0){return;}
    int ms= map(r[1], 1, 100, 100, 0);
    
    // Posiciones finales
    int newPos[4];
    newPos[0]= map(r[2], -100, 100, servoLateral.minAngle, servoLateral.maxAngle);
    newPos[1]= map(r[3], -100, 100, servoGiro.minAngle, servoGiro.maxAngle);  
   
    ceja.finalMod=r[4];
    ceja.nextPos();
    
    int avance[4];    

    // Inicializacion
    for( int i=0; i<4; i++)
    {
        avance[i]=1;    
        inc[i]=0;        
    }
    
     // Condicion parada
     bool fin=false;    
     
     // Movemos los motores
     while ( !fin )
     { 

       // Siguiente posicion de la ceja en grados
        ceja.checkPos();
        newPos[2]= ceja.theta2[ceja.nextMod];
        newPos[3]= ceja.theta5[ceja.nextMod];
 
        // Calculamos sentido de girode todos los servos
        for( int i=0; i<4; i++)
        {
            if ( newPos[i] > pos[i]) 
            {
                inc[i]= +1;  // Incremento de la posicion
            }
            else if ( newPos[i] < pos[i]) 
            {
                inc[i]= -1;  // Decremento de la posicion
            }
        }
        
        // Calculamos avance de cada servo para sincronizarlos
        if (sync)
        {
             // Vemos cual es el motor que necesita mayor recorrido
             difMax=0;
             for (int i=0; i<2; i++)
             {
                 dif[i] = abs(newPos[i]-pos[i]);
                 if (dif[i] > difMax) 
                 { 
                     difMax = dif[i];
                 }
             }
             
             // Calculamos la diferencia maxima con el recorrido total de los servos de la ceja
             dif[2] = ceja.getTotalDif(true);
             dif[3] = ceja.getTotalDif(false);
             
             for (int i=2; i<4; i++)
             {
                 if (dif[i] > difMax) 
                 { 
                     difMax = dif[i];
                 }
             }
                
                 // Servos lateral y giro
                 for(int i=0; i<2; i++)
                 {
                     // Comprobamos si es necesario mover el motor
                     if (dif[i] != 0 && avance[i] < round( difMax/dif[i]))
                     {
                         inc[i] = 0;
                     }
                     // Si se mueve reseteamos el contador
                     else if((dif[i] != 0 && avance[i] >= round( difMax/dif[i])) )
                     {
                         avance[i]=0;
                     }
                 }
                   // Calculamos la diferencia hasta el siguiente estado de los servos de la ceja
                   dif[2] = ceja.getDif(true);
                   dif[3] = ceja.getDif(false);  
                  
                 // Sincronizamos la ceja hasta el estado intermedio 1
                 difMax=0;
                 // Servos cabeza
                 for (int i=2; i<4; i++)
                 {
                     if (dif[i] > difMax) 
                     { 
                         difMax = dif[i];
                     }
                 }
                 
                 // Servos ceja
                 for(int i=2; i<4; i++)
                 {
                     // Comprobamos si es necesario mover el motor
                     if (dif[i] != 0 && avance[i] < round( difMax/dif[i]))
                     {
                         inc[i] = 0;
                     }
                     // Si se mueve reseteamos el contador
                     else if((dif[i] != 0 && avance[i] >= round( difMax/dif[i])) )
                     {
                         avance[i]=0;
                     }
                 }                     
  
            } 
        
       // Comprobamos si algun motor ha terminado su movimiento
        for (int i=0; i<4; i++)
        {
            if (pos[i]==newPos[i]) { inc[i] = 0;}
        }
        
        // Calculamos la siguiente posicion
        for (int i=0; i<4; i++)
        {
            pos[i] = pos[i] + inc[i];
            avance[i]++;
        }
        
        pos1=map(pos[2], 90, 270, ceja.servoI.maxAngle, ceja.servoI.minAngle);
        pos2=map(pos[3], 90, 270, ceja.servoE.maxAngle, ceja.servoE.minAngle); 
       
 
        // movemos los motores
        servoLateral.moveTo(pos[0], false);
        servoGiro.moveTo(pos[1], false);
        ceja.servoI.moveTo(pos1, false);
        ceja.servoE.moveTo(pos2, false);
    
       
        if(pos[0]==newPos[0] && pos[1]==newPos[1] && pos[2]==newPos[2] && pos[3]==newPos[3])
        {
              ceja.checkPos();

              if ( ceja.finalMod==ceja.mod )
              {
                  fin=true;
                  break;
              }

        }
        
        if(ms != 0) 
        {
            delay(ms); 
        }
        
     }

     return;  
}
/*******************************************************************************************/
int distance(int pin)
{
  float cm=0;   
  const int sizeFiltro=10;  // Medida proporcionada por el filtro
  float B[sizeFiltro];	// Buffer con las últimas medidas (inicializamos en 10 cm)
  //static const float A[sizeFiltro]={0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};    // Ponderación de cada valor en el resultado actual

  float minCm , maxCm;

  minCm=1000;
  maxCm=0;

  // Realizamos 10 medidas
  for (int i=0; i< sizeFiltro;i++)
  {
    int sensor = analogRead(pin);  // Leemos el sensor
 
    // Obtenemos el valor en cm
    //B[iFiltro] = 6599.0171345645 * pow(sensor, -1.0789432859);   // Ref de 3 voltios     
    B[i] = 3944.0234066609 * pow(sensor, -1.0789432859);   // Ref de 5 voltios
    
    delay(30);
  }
  
  // Calculamos la media
   for(int i=0; i<sizeFiltro; i++)
   {
      cm= cm+ B[i]/ sizeFiltro;
   }

   //Calculamos maximo y minimo
   for(int i=0; i<sizeFiltro; i++)
   {
      if(B[i] > maxCm)
      {
        maxCm = B[i];
      }
      if (B[i] < minCm)
      {
        minCm = B[i];
      }   
      
   }

  if( (maxCm - minCm) > 5) 
  {
    cm= 40;
  }
  
  return (int)cm;

}

/*******************************************************************************************/
// ---------------------------------------------------
// Se ejecuta cuando el master pide un dato
// ---------------------------------------------------
void requestEvent()
{
  
    // Lectura de los registros de escritura
    if (numRegister <= 0x08)
    {
       Wire.write(r[numRegister]);
       numRegister++; 
    }
        
    // Lectura posicion de los servos en grados
    else if (numRegister==0x1A)
    {
      Wire.write(servoLateral.pos);
      numRegister++; 
    }
    else if (numRegister==0x1B)
    {
       Wire.write(servoGiro.pos);
       numRegister++; 
    }
    else if (numRegister==0x1C)
    {
       Wire.write(ceja.servoI.pos);
       numRegister++; 
    }
    else if (numRegister==0x1D)
    {
       Wire.write( ceja.servoE.pos);
    }    
    
    // Lectura Estado de Leds
    else if (numRegister>=0x10 && numRegister<= 0x17)
    {
       Wire.write(eye.led[numRegister - 0x10]);
       numRegister++; 
    }
    
    // Lectura medida sensor analogico
    else if (numRegister==0x18)
    {
      Wire.write(distance(PIN_SENSOR1));
    }

}

/*******************************************************************************************/
// ---------------------------------------------------
// Se ejecuta cuando el master envia un dato
// ---------------------------------------------------
void receiveEvent(int howMany)
{
   // Esperamos hasta mientras llegan los datos
   while (Wire.available() == 0);
   
   // El primer dato es el registro
   numRegister= Wire.read();
   
   while(Wire.available() >= 1)
   {
       // Leemos el dato que se escribe en el registro
       char2int = Wire.read();
           
       // Mientras tengamos registros disponibles de escritura
       if (numRegister <= 0x07)
       {
           r[numRegister] = (int)* (signed char*) & char2int; 
           
           // Comprovamos si hay un nuevo movimiento
           if (numRegister==0x02 || numRegister==0x03 || numRegister== 0x04)
           {
               newMovement=true;
           }
       }

       // La proxima escritura consecutiva sera en el siguiente registro
       numRegister++;
   }     
   
   newData = true;
}

