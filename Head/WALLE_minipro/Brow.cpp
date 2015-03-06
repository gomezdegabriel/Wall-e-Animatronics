#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>  // Inutiliza PWM en pines 9 y 10
#include "ServoW.h"
#include "Brow.h"

#define DEBUG_ 0
void _BROW::init(int pinI, int minAngleI, int maxAngleI, int minPulseI, int maxPulseI, int pinE, int minAngleE, int maxAngleE, int minPulseE, int maxPulseE)
{
    this->servoI.init(pinI, minAngleI, maxAngleI, minPulseI, maxPulseI, -100);
    this->servoE.init(pinE, minAngleE, maxAngleE, minPulseE, maxPulseE, -100);
    //this->setLimits(7, 0, -0.95);
    this-> mod=0;
    this-> nextMod=0;
    this-> finalMod=0;

        // Angulos en grados
        this->theta2[0] = 267 ;
        this->theta5[0] = 267 ;
            
        this->theta2[1] = 230 ;
        this->theta5[1] = 230 ;
           
        this->theta2[2] = 220 ;
        this->theta5[2] = 140 ;
            
        this->theta2[3] = 140 ;
        this->theta5[3] = 200;
        
        this->theta2[4] = 215;
        this->theta5[4] = 233;
}

// Diferencia hasta el final
int _BROW::getTotalDif(bool theta)
{
  int dif=0;
  int theta2 = map ( this-> servoI.pos, this->servoI.maxAngle, this->servoI.minAngle, 90, 270 );    
  int theta5 = map ( this-> servoE.pos, this->servoE.maxAngle, this->servoE.minAngle, 90, 270 );
  
  if (nextMod==1)
  {
      if(theta){ dif= abs(this->theta2[this->finalMod]-this->theta2[1]) + abs(theta2 - this->theta2[1]);}
      else { dif= abs(this->theta5[this->finalMod]- this->theta5[1]) + abs(theta5 - this->theta5[1]);}
  }
  
  else
  {
      if(theta){ dif= abs(this->theta2[this->finalMod]- theta2 );}
      else { dif= abs(this->theta5[this->finalMod]- theta5) ;}
  }
  
  return dif;
  
}

// Diferencia hasta el siguiente estado
int _BROW::getDif(bool theta)
{
  int dif=0;
  int theta2 = map ( this-> servoI.pos, this->servoI.maxAngle, this->servoI.minAngle, 90, 270 ) ;    
  int theta5 = map ( this-> servoE.pos, this->servoE.maxAngle, this->servoE.minAngle, 90, 270 );    
  
  if(theta){ dif= abs(theta2 - this->theta2[this->nextMod]);}
  else { dif= abs(theta5 - this->theta5[this->nextMod]);}
 
  return dif;
  
}

void _BROW::nextPos()
{
  
    if (this->finalMod==4)
    {
        if(mod==0){ nextMod = 1;}
        if(mod==1){ nextMod = 4;}
        if(mod==2){ nextMod = 1;}
        if(mod==3){ nextMod = 4;}
    }
    if (this->finalMod==3)
    {
        if(mod==0){ nextMod = 1;}
        if(mod==1){ nextMod = 3;}
        if(mod==2){ nextMod = 1;}
        if(mod==4){ nextMod = 3;}
    }
    if (this->finalMod==2)
    {
        if(mod==0){ nextMod = 1;}
        if(mod==1){ nextMod = 2;}
        if(mod==3){ nextMod = 1;}
        if(mod==4){ nextMod = 1;}
    }
    if (this->finalMod==1)
    {
        if(mod==0){ nextMod = 1;}
        if(mod==2){ nextMod = 1;}
        if(mod==3){ nextMod = 1;}
        if(mod==4){ nextMod = 1;}
    }
    if (this->finalMod==0)
    {
        if(mod==1){ nextMod = 0;}
        if(mod==2){ nextMod = 1;}
        if(mod==3){ nextMod = 1;}
        if(mod==4){ nextMod = 1;}
    }
    
}

void _BROW::checkPos()
{
  int theta2 = map ( this-> servoI.pos, this->servoI.maxAngle, this->servoI.minAngle, 90, 270 ) ;    
  int theta5 = map ( this-> servoE.pos, this->servoE.maxAngle, this->servoE.minAngle, 90, 270 );    
  
   if (theta2 == this-> theta2[nextMod] && theta5== this->theta5[nextMod])
   {
       this->mod=nextMod;
       this->nextPos();
   }
  
}

/*
void _BROW::setLimits(float x1, float y1, float y2)
{
    this->x5Limit=x1;
    this->y5Limit=y1;
    this->y6Limit=y2;
}*/
/*
bool _BROW::moveTo(int porcentI, int porcentE, bool master, int velocidad)
{
  
    // Si la velocidad es nula, salimos
    if(velocidad==0) { return false;}
        
    // Esperaremos entre 0 y 200 ms
    int ms= map(velocidad, 1, 100, 200,0);
     
    // Angulo en sentido antihorario desde un plano horizontal a la barra
    int newPosI= map(porcentI, -100, 100, 270 - this->servoI.minAngle, 270 - this->servoI.maxAngle);
    int newPosE= map(porcentE, -100, 100, 270 - this->servoE.minAngle, 270 - this->servoE.maxAngle);
        
    // Angulo en sentido antihorario desde un plano horizontal a la barra
    int posI= map(this->servoI.pos, 0, 180, 180, 0)  + 90;
    int posE= map(this->servoE.pos, 0, 180, 180, 0)  + 90;
        
    // Diferencia entre la posicion actual y la requerida
    int difI = posI-newPosI;
    int difE = posE-newPosE;
                  
    // Sentido de giro de los motores: false-> hacia abajo
    bool direccion2=false;
    bool direccion5=false;
        
    if (difI >= 0)
    {
        direccion2=true;
    }
        
    if (difE >= 0)
    {
        direccion5=true;
    }
        
    difI=abs(difI);
    difE=abs(difE);
        
    // Calcularemos con radianes
    float deg2rad=PI/180;
        
    // Angulo de la barra 2 en radianes
    float theta2 = posI*deg2rad; 

    // Angulo de la barra 5 en radianes
    float theta5 = posE*deg2rad;

    // Incrementamos en 1 grado en cada iteracion
    float inc = deg2rad;  

    // Posiciones de los puntos 1, 2, 3, 5 y 6
    float x1, y1, x2, y2, x3, y3, x5, y5, x6, y6;

    // Angulos
    float theta3, theta1p, theta2p;

    // Variables para el calculo de theta4
    float A, B, C, k, J1, J2, J1s, J2s, error;
        
    // Error maximo admitido en el calculo de theta4
    float epsilon = 0.01;
        
    // Indica si el servo secundario tiene que cambiar de sentido
    bool cambio=false;
        
    // Indica si se alcanza algun punto limite
    bool limit=false;

    // Valor que devuelve la funcion
    bool noColision=true;
                
    int deg;
        
    float theta4 = this->theta*deg2rad; // Aproximación inicial de theta4

    int avance=0;
        

    unsigned long int time;
        
        // En cada iteracion se gira al menos
        // un grado en al menos uno de los servos 
	for (int i = 0; i <= 360; i++)
	{
                if (ms!=0)
                {
                    time = millis();  
                }

                // Condiciones de salida
                if(difE==0 && !master) {break;}
                if(difI==0 &&  master) { break;}
            
                // Calculamos theta4               
		A= L1 + L2*cos(theta2) - L5*cos(theta5);
    	        B= L2*sin(theta2) - L5*sin(theta5);
		C= -(L3*L3-L4*L4-A*A-B*B)/(-2*L4);

		// Calculamos el valor de theta4 de forma iterativa
		// Buscamos la solución en un intervalo cercano a la
		// última solución
		float theta41=theta4-10*deg2rad;
		float theta42=theta4+10*deg2rad;
		float mitad= (theta42-theta41)/2;
		
		k=0;
		while ( abs(error) > epsilon && k < 20)
		{
         	    J1 = A*cos(theta41) + B*sin(theta41) + C;
                    J2 = A*cos(theta42) + B*sin(theta42) + C;
                    J1s = A*cos(theta41+mitad) + B*sin(theta41+mitad) + C;
                    J2s = A*cos(theta42-mitad) + B*sin(theta42-mitad) + C;

		    if(J1s*J2<=0)
		    {
  		        theta41= theta41+mitad;
		        theta4=theta41;
		        mitad= (theta42-theta41)/2;
		    }
		    else if(J2s*J1<0)
		    {
			theta42= theta42-mitad;
		        theta4=theta42;
		        mitad= (theta42-theta41)/2;
		    }
		    else
                    {
		        mitad=mitad/2;
		    }

		    error = A*cos(theta4) + B*sin(theta4) + C;
		    k++;
		}

                error=error+1;

		// Calculamos los puntos de giro de los eslabones
		 x1= L1+L2*cos(theta2);
		 y1= L2*sin(theta2);
		
		 x3= L5*cos(theta5);
		 y3= L5*sin(theta5);
		
		 x2= x3 + L4*cos(theta4);
		 y2= y3 + L4*sin(theta4);
		
		 theta3 = atan( (y1-y2)/ (x1-x2));

		// Calculamos los puntos clave de la estructura
		 theta1p= theta3 - 263.0*deg2rad;
		 x5= x1 + Li*cos(theta1p);
		 y5= y1 + Li*sin(theta1p);
		
		 theta2p= theta3 - 140.0*deg2rad;
		 x6= x2 + Le*cos(theta2p);
		 y6= y2 + Le*sin(theta2p);

          	// Comprobamos si se toca algun punto limite
                limit=false;
		if ( y5 <= y5Limit || x5 >= x5Limit || y6 <= y6Limit ) 
		{
                      if(DEBUG_)
                      {    
                          Serial.print("\ntheta2: ");
                          Serial.print(theta2/deg2rad);
                          Serial.print("\ntheta4: ");
                          Serial.print(theta4/deg2rad);
                          Serial.print("\ntheta5: ");
                          Serial.print(theta5/deg2rad);
                          Serial.print("\n x1: ");
                          Serial.print(x1);
                          Serial.print("\n x2: ");
                          Serial.print(x2);
                          Serial.print("\n x5: ");
                          Serial.print(x5);
                          Serial.print(" x5L: ");
                          Serial.print(x5Limit);
                
                          Serial.print("\n y5: ");
                          Serial.print(y5); 
                          Serial.print(" y5L: ");
                          Serial.print(y5Limit);  
                          
                          Serial.print("\n y6: ");
                          Serial.print(y6);
                          Serial.print(" y6L: ");
                          Serial.print(y6Limit);
                      }

		    limit = true;
                    noColision = false;
		}
                
                // Limit 
                if (limit && direccion2 != direccion5) 
                { 
                  if (master) { direccion5=direccion2; }
                  else { direccion2 = direccion5; }
                  
                  cambio=true;
                  
                  if (DEBUG_)
                  {                  
                      Serial.print("Cambio Direccion ");
                      Serial.print("\n\n\n");
                  }
                  
              	}
                
                // 
                else if(limit && direccion2 == direccion5)
                {

                    if (!master && (y5 <= y5Limit || x5 >= x5Limit) ) // direction2==direction5
                    {
                        theta2 = theta2 - inc;
                        deg = theta2/deg2rad - 90;
                        deg= map(deg, 0, 180, 180, 0);
                        this-> servoI.pos = deg;
		        this->servoI.theServo.write(deg);
                    }
                    else if (!master && (y6 <= y6Limit) )
                    {
                        theta5 = theta5 - inc;
                        deg = theta5/deg2rad - 90;
                        deg= map(deg, 0, 180, 180, 0);
                        this-> servoE.pos = deg;                        
		        this->servoE.theServo.write(deg);
                    }
                    else if ( master &&  (y6 <= y6Limit))
                    {
                        theta5 = theta5 - inc;
                        deg = theta5/deg2rad - 90;
                        deg= map(deg, 0, 180, 180, 0);
                        this-> servoE.pos = deg;
		        this->servoE.theServo.write(deg);
                    }
                    else if ( master && (y5 <= y5Limit || x5 >= x5Limit))
                    {
                        theta2 = theta2 - inc;
                        deg = theta2/deg2rad - 90;
                        deg= map(deg, 0, 180, 180, 0);
                        this-> servoI.pos = deg;                        
		        this->servoI.theServo.write(deg);
                    }
                    
                } 
                else if ( !limit )
                {
                  // Movemos los servos a la posicin calculada
                  deg = theta2/deg2rad - 90;
                  deg= map(deg, 0, 180, 180, 0);
                  this-> servoI.pos = deg;
                  this->servoI.theServo.write(deg);


                  deg = theta5/deg2rad - 90;
                  deg= map(deg, 0, 180, 180, 0);
                  this-> servoE.pos = deg;
                  this->servoE.theServo.write(deg);
                  


                  difI= abs(theta2/deg2rad-newPosI);
  	          difE= abs(theta5/deg2rad-newPosE);
  
                  this->theta = theta4/deg2rad;

    
                    // El secundario pasar por la posicion deseada
                    if (difI==0 && !master) {cambio=true;}
                    if (difE==0 &&  master) {cambio=true;}
                    
                    // Incrementamos el angulo dependiendo de la direccion
                    if(direccion2) { theta2 = theta2 - inc;}
                    else { theta2 = theta2 + inc; }
                    
                    if(direccion5) { theta5 = theta5 - inc; }
                    else { theta5 = theta5 + inc; }    
                     
                    // Decrementamos el angulo si se cumplen las condiciones
                    if (difE == 0 || avance < difI/difE || (cambio && master) || (!noColision && master) )
                    {
                        if(direccion5) { theta5 = theta5 + inc; }
                        else { theta5 = theta5 - inc; }
                    }    
                    
                    if (difI == 0 || avance < difE/difI || (cambio && !master) || (!noColision && !master) )
                    {
                        if(direccion2) { theta2 = theta2 + inc; }
                        else { theta2 = theta2 - inc; }
                    }    
                    
                    if (difI > difE && avance >= difI/difE)
                    {
                        avance=0;
                    }
                    
                    if ( difE > difI && avance >= difE/difI)
                    {
                        avance=0;
                    }
                    
                    avance++;
                    
                    if(DEBUG_)
                    {
                      Serial.print("\n\ntheta2: ");
                      Serial.print(deg);
    
                      Serial.print("\ntheta5: ");
                      Serial.print(deg);
                      
                      Serial.print("\n difI ");
                      Serial.print(difI);
                      Serial.print(" difE: ");
                      Serial.print(difE);        
                      Serial.print("\n\n: ");
                      
                      Serial.print(" Cambio: ");
                      Serial.print(cambio);
                    }
                    
                    if(ms != 0)
                    {
                         // Controlamos la velocidad                    
                        while( abs(millis()-time) <= ms )
                        {
                           
                        }
                    }
                  
                }

	}
    
        return noColision;
}*/

