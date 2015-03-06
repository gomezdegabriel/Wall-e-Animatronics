  #include <Wire.h>
  #include <MsTimer2.h>  // Inutiliza PWM en pines 11 y 13 (Usa Timer2)
  #include <Servo.h>  // Inutiliza PWM en pines 9 y 10
  #include "Temp.h"

  
  #define NUM_DATOS 8 //Numero de datos a enviar
  #define NUM_MAX_MOVIMIENTOS_1 6
  #define NUM_MAX_MOVIMIENTOS_2 11
  #define NUM_MAX_MOVIMIENTOS_3 5
  #define NUM_MAX_MOVIMIENTOS_4 5
  
  const int buttonPin = 2;    // the number of the pushbutton pin
  int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin
  long lastDebounceTime = 0;  // the last time the output pin was toggled
  long debounceDelay = 50;    // the debounce time; increase if the output flickers
  bool wink=false;

//*****************************************************************************************
  void randomWink()
  {
      wink=true;
  }
  
//*****************************************************************************************
  void setup()
  {
      
      delay(1000); // Esperamos para que todos los dispositivos I2C esten listos al comenzar
      
      pinMode(buttonPin, INPUT);
      
      Wire.begin(); // Unirse al bus i2c (master no necesita direccion)
      
     // myservo.init(10,0,180,800,1000,0); 
    
      Serial.begin(9600);
      MsTimer2::set(random(2000,7000), randomWink );  
      MsTimer2::start();
      
      //Serial.println("Inicializado");
  }
  
//*****************************************************************************************

  int EXPRESION=1;

  const int MICRO_DRCH= 4;  
  const int MICRO_IZQD= 7;  
  
  int direccionSFR08_DRCH=0x70;
  int direccionTPA81= 0x68;
  
  byte temp[9];
  
  byte command;
  int led = 13;

  int j=0; // Expresión número j
  int vel=0;
  int m0Izqd=0;
  int m0Drch=0;
  int m1Izqd=0;
  int m1Drch=0;
  int cejaIzqd;
  int cejaDrch;
  int ceja;
  int aux;
      
  //*****************************************************************************************              
    signed char walleTristeDrch[NUM_MAX_MOVIMIENTOS_1][NUM_DATOS+1]={
              //  0         1         2        3        4        5       6          7              8
              //  sync     vel.      m0        m1     Ceja     ojo    brillo    t.transBrillo    duracion (0-127)
                {   1,     80,      -60,       -60,     0,      5,       40,         100,           0  }, 
                {   1,     70,     -85,      -100,     1,      5,       40,         100,           0  }, 
                {   1,     70,     -85,      -100,     4,      5,       40,         0,           4  }, 
                {   1,     70,     -30,      -100,     1,      5,       40,         0,           4  }, 
                {   1,     70,     -80,      -100,     4,      5,       40,         0,           2  }, 
                {   1,     70,     -85,      -100,     1,      5,       40,         0,           0  }

               };
               
    signed char walleTristeIzqd[NUM_MAX_MOVIMIENTOS_1][NUM_DATOS+1]={
  
              //  0         1         2        3        4        5         6           7              8
              //  sync     vel.      m0        m1     Ceja      ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     80,      -60,      -60,      0,       5,       20,         100,            0  }, 
                {   1,     70,     -85,     -100,      1,       5,       20,         100,             0  }, 
                {   1,     70,      -30,     -100,      4,       5,       20,         0,            4 }, 
                {   1,     70,     -85,     -100,      1,       5,       20,         0,             4  }, 
                {   1,     70,     -85,     -100,      4,       5,       20,         0,             2  }, 
                {   1,     70,     -85,     -100,      1,       5,       20,         0,             0  }

               };
               
//*****************************************************************************************              
   
    
    signed char walleSorpresaDrch[NUM_MAX_MOVIMIENTOS_2][NUM_DATOS+1]={
              //  0         1         2        3        4        5         6           7              8
              //  sync     vel.      m0        m1     Ceja     ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     90,       40,       40,      0,       5,        100,          0,            3  }, 
                {   1,     100,      80,       40,      3,       5,       100,          0,            10 }, 
                {   1,     100,      80,      40,      0,      5,       100,         0,                6  }, 
                {   1,     94,       80,      40,      0,       5,       100,            0,            6 },
                {   1,     70,       40,       0,      0,       5,       100,            0,            6  },
                {   1,     97,      40,       0,      0,       5,       100,            0,             3  },
                {   1,     70,       40,       0,      0,       5,       100,            0,            6 },
                {   1,     94,     20,      40,      3,        5,       80,         0,              6  },
                {   1,     70,     40,       0,      3,        5,       80,         0,              6  },
                {   1,     97,     0,       0,      0,       5,       100,         0,              6  },
                {   1,     70,      40,       0,      0,       5,       100,         0,             3  }
               // {   1,     94,       20,       0,      0,       5,       100,            0,            100  },               
                //{   1,     80,       40,      15,      0,       5,       100,            0,            100  }, 
                //{   1,     90,       20,       0,      3,       5,       100,            0,            100  },
               // {   1,     80,       20,       0,      3,       5,       100,            0,            100  },
               // {   1,     97,       50,       0,      0,       5,       100,           70,            100  }

               };
               
    signed char walleSorpresaIzqd[NUM_MAX_MOVIMIENTOS_2][NUM_DATOS+1]={
              //  0         1         2        3        4        5       6            7                8
              //  sync     vel.     m0       m1     Ceja      ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     90,     40,       40,      0,        5,       80,         0,           3  }, 
                {   1,     100,    80,       40,      3,        5,       80,         0,          10  }, 
                {   1,     100,    80,      40,      0,        5,       80,         0,            6  },
                {   1,     97,     20,      40,      3,        5,       80,         0,            6  },
                {   1,     70,     20,       0,      3,        5,       80,         0,            6  },
                {   1,     97,     20,       0,      0,       5,       100,         0,            3  },
                {   1,     70,      40,       0,      0,       5,       100,        0,            6  },
                {   1,     97,       80,      40,      0,       5,       100,       0,            6 },
                {   1,     70,       40,       0,      0,       5,       100,       0,            6  },
                {   1,     97,      40,       0,      0,       5,       100,        0,            6  },
                {   1,     70,       40,       0,      0,       5,       100,       0,            3 }
                //{   1,     94,     20,       0,      0,        5,       80,         0,              100  }, 
                //{   1,     80,     40,      15,      0,        5,       80,         0,              100  }, 
                //{   1,     90,     80,       0,      0,        5,       80,         0,              100  },
                //{   1,     80,      50,      0,      0,        5,       80,         0,              100 },
               // {   1,     97,      50,      0,      0,        5,       80,         100,            100  }

               };
    
  //*****************************************************************************************                
  

    signed char walleDisgustoDrch[NUM_MAX_MOVIMIENTOS_3][NUM_DATOS+1]={
              //  0         1         2        3        4        5       6            7              8
              //  sync     vel.      m0        m1     Ceja     ojo     brillo    t.transBrillo    duracion (0-127)
                {   1,     92,      10,     100,      2,       5,        80,          100,            0  }, 
                {   1,     92,      80,     100,      0,       5,        80,          100,            0  }, 
                {   1,     92,      40,     100,      2,       5,        80,         0,               0  }, 
                {   1,     92,     -20,     100,      0,       5,        80,         0,               0  }, 
                {   1,     88,      40,     100,      0,       5,        80,         0,               0  }, 


               };
               
    signed char walleDisgustoIzqd[NUM_MAX_MOVIMIENTOS_3][NUM_DATOS+1]={
              //  0         1       2        3        4         5          6            7              8
              //  sync     vel.     m0       m1      Ceja       ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     92,     40,      100,      2,         5,       60,         100,             0  }, 
                {   1,     92,     80,      100,      0,         5,       60,         100,             0  }, 
                {   1,     92,     40,      100,      2,         5,       60,         100,               0  }, 
                {   1,     92,    -20,      100,      0,         5,       60,         100,               0  }, 
                {   1,     88,     40,      100,      0,         5,       60,         100,               0  },
  
               };
  //*****************************************************************************************                    
      signed char walleNormalDrch[NUM_MAX_MOVIMIENTOS_4][NUM_DATOS+1]={
              //  0         1      2         3        4        5         6            7              8
              //  sync     vel.    m0        m1     Ceja     ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     78,     20,      40,      0,       5,        40,          100,            0  }, 
                {   1,     78,     40,      20,      1,       5,        40,          100,            0  }, 
                {   1,     78,     20,      40,      0,       5,        40,          100,            0  }, 
                {   1,     78,     40,      20,      0,       5,        40,          100,            0  }, 
                {   1,     78,      0,      40,      0,       5,        40,          100,            0  }

               };
               
    signed char walleNormalIzqd[NUM_MAX_MOVIMIENTOS_4][NUM_DATOS+1]={
              //  0         1      2        3        4        5        6            7               8
              //  sync     vel.    m0       m1     Ceja      ojo       brillo    t.transBrillo    duracion (0-127)
                {   1,     78,     20,      40,      0,        5,       20,         100,             0  }, 
                {   1,     78,     40,      20,      1,        5,       20,         100,             0  }, 
                {   1,     78,     40,      40,      1,        5,       20,           0,             0  }, 
                {   1,     78,     20,      20,      0,        5,       20,           0,             0  }, 
                {   1,     78,     40,      40,      0,        5,       20,           0,             0  }
  
               };
               
   //*****************************************************************************************       

int NUM_MAX_MOVIMIENTOS;
//*****************************************************************************************
void loop()
{

  //****************************************************************************************************************************     

if (EXPRESION == 0)
{
    NUM_MAX_MOVIMIENTOS = NUM_MAX_MOVIMIENTOS_1;
        /* Triste */       
    signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_1]= {2, 10, 2, 2, 10, 8};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_1]= {2, 6,  2, 2, 6, 4};
    /*signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_1]= {1, 1, 1, 1, 1, 1};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_1]= {1, 1,  1, 1, 1, 1};*/
}

//***************************************************************************************** 
if (EXPRESION== 1)
{
    NUM_MAX_MOVIMIENTOS = NUM_MAX_MOVIMIENTOS_2;
    /* Sorpresa */
    /*signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_2]= {3, 1, 1, 1, 8, 1, 3,1,8, 1};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_2]= {2, 1, 1, 1, 4, 1, 2,1,4, 1};*/
    signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_2]= {1, 1, 1, 1, 8, 1,8,1, 8, 1,8};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_2]=     {1, 1, 1, 1, 4, 1,4,1, 4, 1,4};
}

//***************************************************************************************** 
if (EXPRESION== 2)
{
    NUM_MAX_MOVIMIENTOS = NUM_MAX_MOVIMIENTOS_3;
    /* Disgusto */
    signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_3]= {1, 1, 1, 1, 1};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_3]= {1, 1, 1, 1, 1};
}

//***************************************************************************************** 
if (EXPRESION== 3)
{
     NUM_MAX_MOVIMIENTOS = NUM_MAX_MOVIMIENTOS_4;
    /* Normal */
    signed char num_movimientos_max[NUM_MAX_MOVIMIENTOS_4]= {1, 1, 1, 1, 1};
    signed char num_movimientos_min[NUM_MAX_MOVIMIENTOS_4]= {1, 1, 1, 1, 1};
}

signed char cuello[NUM_MAX_MOVIMIENTOS];

signed char thisExpDrch[NUM_MAX_MOVIMIENTOS][NUM_DATOS+1];
signed char thisExpIzqd[NUM_MAX_MOVIMIENTOS][NUM_DATOS+1];
  
// Copiamos la expresión que queremos mostrar
      for(int j=0; j< NUM_MAX_MOVIMIENTOS; j++)
      {
          for(int i=0; i< NUM_DATOS+1; i++)
          {
              if (EXPRESION==1){
                  thisExpDrch[j][i]=walleTristeDrch[j][i];
                  thisExpIzqd[j][i]=walleTristeIzqd[j][i];
              }
              if (EXPRESION==2){
                  thisExpDrch[j][i]=walleSorpresaDrch[j][i];
                  thisExpIzqd[j][i]=walleSorpresaIzqd[j][i]; 
              }
              if (EXPRESION==3){
                  thisExpDrch[j][i]=walleDisgustoDrch[j][i];
                  thisExpIzqd[j][i]=walleDisgustoIzqd[j][i];  
              }
              if (EXPRESION==4){
                  thisExpDrch[j][i]=walleNormalDrch[j][i];
                  thisExpIzqd[j][i]=walleNormalIzqd[j][i]; 
                
              }
              

          }
          //Serial.println();
      }
  
  
   /* Lectura de sensores 
  
    
    
  //Lectura dispositivos I2C
    char dato=0;
    dato= leerDatoHemisferio (MICRO_DRCH, 0x08);
    Serial.print("dato1: ");
    Serial.println((int)dato);

    dato= leerDatoHemisferio (MICRO_IZQD, 0x08);
    Serial.print("dato2: ");
    Serial.println((int)dato);
    */
    
    //Seguimiento de la cabeza
    //tpa81(direccionTPA81);
    /*int distancia=distanciaFrontal (direccionSFR08_DRCH) ;
    if(distancia<=40){
      EXPRESION= ((EXPRESION+1) % 4)+1;
    }*/

    //seguir();
    
        int num_interp=0;
    
    if(true)
    {     

      // Numero de expresiones aleatorias entre expreiones base
        //num_interp= random(num_movimientos_min[j],num_movimientos_max[j]);
        num_interp=5;
        for (int p=0; p<num_interp; p++)
        {
          
            if (EXPRESION==1){
                wallETriste();
            }
            if (EXPRESION==2){
                wallESorpresa();
            }

            if (EXPRESION==3){
                wallEDisgusto();
            }

            if (EXPRESION==4){
                wallENormal();
            }

          //---------------------------------------------------------------------------    
                Wire.beginTransmission(MICRO_IZQD);   // Direccion dispositivo
                Wire.write(0x00);                     // Direccion registro inicial
                
                Wire.write(thisExpIzqd[j][0]); 
                Wire.write(vel); 
                Wire.write(m0Izqd); 
                Wire.write(m1Izqd); 
                Wire.write(cejaIzqd); 
                Wire.write(thisExpIzqd[j][5]); 
                Wire.write(thisExpIzqd[j][6]); 
                Wire.write(thisExpIzqd[j][7]); 
                              /* Serial.println("Izqd");

                                Serial.println((int)vel);
                                Serial.println((int)m0Izqd);
                                Serial.println((int)m1Izqd);
                                Serial.println((int)cejaIzqd);*/

                Wire.endTransmission(true); 
             
             //--------------------------------------------------------------------------
                Wire.beginTransmission(MICRO_DRCH);   // Direccion dispositivo
                Wire.write(0x00);                    // Direccion registro inicial
                              /*              Serial.println("Drch");

                                Serial.println((int)vel);
                                Serial.println((int)m0Drch);
                                Serial.println((int)m1Drch);
                                Serial.println((int)cejaDrch);*/
                Wire.write(thisExpDrch[j][0]); 
                Wire.write(vel); 
                Wire.write(m0Drch); 
                Wire.write(m1Drch); 
                Wire.write(cejaDrch); 
                Wire.write(thisExpDrch[j][5]); 
                Wire.write(thisExpDrch[j][6]); 
                Wire.write(thisExpDrch[j][7]);             
                
                Wire.endTransmission(true); 
                
               //--------------------------------------------------------------------------


    int distancia=distanciaFrontal (direccionSFR08_DRCH) ;
    if(distancia<=40){
      EXPRESION= ((EXPRESION+1) % 4);
      delay(2000);
      break;
    }

                 delay( thisExpDrch[j][8]*100 );
                 
                 waitEnd(MICRO_DRCH);   
                 waitEnd(MICRO_IZQD);
                 
    //--------------------------------------------------------------------------
        if(wink)
        {

       //--------------------------------------------------------------------------
            Wire.beginTransmission(MICRO_IZQD);   // Direccion dispositivo
            Wire.write(0x05);                     // Direccion registro de tipo ojo
            Wire.write(4);                        // Guiño
            Wire.endTransmission(true);           
                     
     //--------------------------------------------------------------------------     
             Wire.beginTransmission(MICRO_DRCH);   // Direccion dispositivo
             Wire.write(0x05);                     // Direccion registro de tipo ojo
             Wire.write(4);                        // Guiño
             Wire.endTransmission(true);           
             wink=false;
                   
                   //--------------------------------------------------------------------------
          }
         }
    }
    
  //Serial.println("j");

  //Serial.println(j);
   j++;
   j = j % NUM_MAX_MOVIMIENTOS;  
 

        
}
//******************************************************************************************
void wallETriste(){
          
          //Generamos las componentes aleatorias
          //Nueva velocidad
           vel= random(-5,+5);
           
           vel= walleTristeDrch[j][1]+vel;
           
           if(vel >100){vel=100;}
           if(vel >100){vel=100;}
           if(vel <0){vel=walleTristeDrch[j][1];}
           if(vel <0){vel=walleTristeDrch[j][1];}

           
           //Nuevo movimiento inclinacion
           aux= random(0,11);
            if(aux<3)
                aux=15;
            else if (aux<6)
                aux=30;
            else if(aux<9)
                aux=45;
            else
                aux=0;
            
            m0Drch= walleTristeDrch[j][2] + aux;
            m0Izqd= walleTristeIzqd[j][2] + aux;

           if(m0Drch >100){m0Drch=100;}
           if(m0Izqd >100){m0Izqd=100;}
           if(m0Drch <-100){m0Drch= -100;}
           if(m0Izqd <-100){m0Izqd= -100;}
                
           //Nuevo movimiento hemisferio
           aux= random(0,11);
            if(aux<3)
                aux=15;
            else if (aux<6)
                aux=15;
            else if(aux<9)
                aux=35;
            else
                aux=0;
                
            m1Drch= walleTristeDrch[j][3] + aux;
            m1Izqd=walleTristeIzqd[j][3] + aux;

           if(m1Drch >100){m1Drch=100;}
           if(m1Drch <-100){m1Drch=-100;}
           if(m1Izqd >100){m1Izqd=100;}
           if(m1Izqd <-100){m1Izqd=-100;}                  
                
           ceja= random(1,10);
            if(ceja<5)
                ceja=1;
            else
                ceja=4;
            
            cejaIzqd=ceja;
            cejaDrch=ceja;
}
//******************************************************************************************
void wallESorpresa(void){

     
          //Generamos las componentes aleatorias
          //Nueva velocidad

           vel= walleSorpresaDrch[j][1];
              
          //Nuevo movimiento inclinacion
           aux= random(1,10);
            if(aux<3)
                aux=-20;
            else if (aux<6)
                aux=20;
            else if(aux<9)
                aux=35;
            else
                aux=0;
            
            m0Drch= walleSorpresaDrch[j][2] + aux;
            m0Izqd= walleSorpresaIzqd[j][2] + aux;

           if(m0Drch >100){m0Drch=100;}
           if(m0Izqd >100){m0Izqd=100;}
           if(m0Drch <-100){m0Drch= -100;}
           if(m0Izqd <-100){m0Izqd= -100;}


          //Nuevo movimiento hemisferio
           aux= random(0,11);
            if(aux<3)
                aux= -30;
            else if (aux<5)
                aux= -60;
            else if(aux<9)
                aux= -80;
            else
                aux=0;
                
            m1Drch= walleSorpresaDrch[j][3] + aux;
            m1Izqd= walleSorpresaIzqd[j][3] + aux;
            
           if(m1Drch >100){m1Drch=100;}
           if(m1Izqd >100){m1Izqd=100;}
           if(m1Drch <-100){m1Drch=-100;}
           if(m1Izqd <-100){m1Izqd=-100;} 
          
                
           cejaIzqd=walleSorpresaIzqd[j][4];
           cejaDrch=walleSorpresaDrch[j][4];
}
//******************************************************************************************
void wallEDisgusto(void){

          //Generamos las componentes aleatorias
          //Nueva velocidad
          //Nueva velocidad
           vel= random(-3,+1);
           
           vel= walleDisgustoDrch[j][1]+vel;
           
           if(vel >100){vel=100;}
           if(vel >100){vel=100;}
           if(vel <0){vel=walleDisgustoDrch[j][1];}
           if(vel <0){vel=walleDisgustoDrch[j][1];}
          
          //Nuevo movimiento inclinacion
           aux= random(1,10);
            if(aux<4)
                aux= -20;
            else if (aux<6)
                aux= 20;
            else
                aux=0;
            
            m0Drch= walleDisgustoDrch[j][2] + aux;
            m0Izqd= walleDisgustoIzqd[j][2] + aux;

           if(m0Drch >100){m0Drch=100;}
           if(m0Izqd >100){m0Izqd=100;}
           if(m0Drch <-100){m0Drch= -100;}
           if(m0Izqd <-100){m0Izqd= -100;}
                
                
           //Nuevo movimiento hemisferio
            m1Drch= walleDisgustoDrch[j][3];
            m1Izqd= walleDisgustoIzqd[j][3];
             
             
             
             
             ceja=cejaIzqd;
             if(walleDisgustoIzqd[j][4]==0){
                 cejaIzqd= random(1,10);
              
                 if(cejaIzqd < 3)
                     cejaIzqd=0;
                 else if(cejaIzqd < 7)
                     cejaIzqd=1;
                 else
                     cejaIzqd=2;
                     
                 if(ceja != cejaIzqd) vel=vel+3;
                  
                 if(vel >100){vel=100;}
                 if(vel >100){vel=100;}
                 if(vel <0){vel=walleDisgustoDrch[j][1];}
                 if(vel <0){vel=walleDisgustoDrch[j][1];}
           }else
           {
               cejaIzqd=walleDisgustoIzqd[j][4];
           }
           
          ceja=cejaDrch;

           if(walleDisgustoDrch[j][4]==0){
                 cejaDrch= random(1,10);

                 if(cejaDrch<3)
                     cejaDrch=0;
                 else if(cejaDrch<7)
                     cejaDrch=1;
                else
                     cejaDrch=2;
                     
                  if(ceja != cejaIzqd) vel=vel+3;
                  
                 if(vel >100){vel=100;}
                 if(vel >100){vel=100;}
                 if(vel <0){vel=walleDisgustoDrch[j][1];}
                 if(vel <0){vel=walleDisgustoDrch[j][1];}          
           }else
           {
               cejaDrch=walleDisgustoDrch[j][4];
           }
}

//******************************************************************************************
void wallENormal(void){

          //Generamos las componentes aleatorias
          //Nueva velocidad
           
           vel= random(-3,+3);
           
           vel= walleNormalDrch[j][1]+vel;
           
           if(vel >100){vel=100;}
           if(vel >100){vel=100;}
           if(vel <0){vel=walleNormalDrch[j][1];}
           if(vel <0){vel=walleNormalDrch[j][1];}   
           
          //Nuevo movimiento inclinacion
           aux= random(1,10);
            if(aux<6)
                aux= -20;
            else
                aux= 20;
            
            m0Drch= walleNormalDrch[j][2] + aux;
            m0Izqd= walleNormalIzqd[j][2] + aux;

           if(m0Drch >100){m0Drch=100;}
           if(m0Izqd >100){m0Izqd=100;}
           if(m0Drch <-100){m0Drch= -100;}
           if(m0Izqd <-100){m0Izqd= -100;}
                

          //Nuevo movimiento hemisferio
           aux= random(1,10);
            if(aux<5)
                aux= -20;
            else if (aux<7)
                aux= -40;
            else
                aux=0;
                
            m1Drch= walleNormalDrch[j][3] + aux;
            m1Izqd= walleNormalIzqd[j][3] + aux;
            
           if(m1Drch >100){m1Drch=100;}
           if(m1Izqd >100){m1Izqd=100;}
           if(m1Drch <-100){m1Drch=-100;}
           if(m1Izqd <-100){m1Izqd=-100;}            
            
              
           if(walleNormalIzqd[j][4]==0 && walleNormalDrch[j][4]==0){
                 cejaIzqd= random(1,10);
                 cejaDrch= random(1,10);
            
                 if(cejaIzqd < 7){
                     cejaIzqd=0;
                 }
                 else{
                     cejaIzqd=1;
                 }
    
                     
                 if(cejaDrch < 7) {
                     cejaDrch=0;
                 }
                 else {
                     cejaDrch=1;
                 }

           }else
           {
               cejaIzqd=walleNormalIzqd[j][4];
               cejaDrch=walleNormalDrch[j][4];
           }
           
}


//******************************************************************************************
int distanciaFrontal(int theDir) {
    byte byteAlto = 0x00;
    byte byteBajo = 0x00;
    int distancia;
    
    Wire.beginTransmission(theDir);
    Wire.write(0x00);  // Registro comando
    Wire.write(0x51);  // Especifica medida en cm
    Wire.endTransmission(false);
    
    delay(65);  // En la hoja de datos se dice que
                // eperemos 65ms
    
    Wire.beginTransmission(theDir);
    Wire.write(0x02);    // Registro de Distancia
    Wire.endTransmission(false);
    
    delay(65);
    Wire.requestFrom(theDir ,2);
    
    while (Wire.available() < 2);
    byteAlto = Wire.read();
    byteBajo = Wire.read();
    distancia = (byteAlto << 8) + byteBajo;
    
    //Serial.print ("distancia: ");

    //Serial.println(distancia);
            
            

    return distancia;  // retorna a la rutina desde donde se invocó
  }
  
 //******************************************************************************************
  void tpa81 (int theDir) {
  
    Wire.beginTransmission(theDir);
    Wire.write(0x01);  // Registro en el que empezamos a leer
    Wire.endTransmission();
 
    delay(40);  // Nos aseguramos de que la medida 
                // este actualizada
 
    Wire.requestFrom(theDir ,9);
  
    //Serial.println("esperando respuesta . . .");
    
    while (Wire.available() < 9);
    

    for (int i=0; i< 9; i++)
    {
        temp[i] = Wire.read();
        //Serial.print("temp[ ");
       // Serial.print(i);
      //  Serial.print(" ]");

        //Serial.println(temp[i]);

    }
    
    
  }
   //******************************************************************************************
  void waitEnd (int theDir) {
      
    char theEnd=0;

    while(theEnd != 1)
    {
        theEnd=leerDatoHemisferio(theDir,0x08);
        delay(30);

    }
         //Serial.println("wait");

       // Serial.println((int)theEnd);
  }
  
     //******************************************************************************************
 int leerDatoHemisferio (int theDir, int registro) {
      
    int dato=0;

    Wire.beginTransmission(theDir);
    Wire.write(registro);  // Registro que queremos leer
    Wire.endTransmission();
        
    Wire.requestFrom(theDir, 1);
      
    //while (Wire.available())
   // {
         dato = Wire.read();
   // }
    
    return dato;
    
  }
   //******************************************************************************************
void seguir(){
  tpa81(direccionTPA81);
  
  byte i=0;
  byte theMax=0;
  byte the;
  
    for (i=1; i< 9; i++)
    {
        if(temp[i]>theMax)
          theMax=temp[i];
      }
  
    if (theMax > temp[0]+50)
    {
        the=100;
        
    }
    
    if (theMax <temp[0]+0)
    {
        the=0;
        
    }

    Wire.beginTransmission(MICRO_IZQD);   // Direccion dispositivo
    Wire.write(0x00);                     // Direccion registro inicial
    Wire.write(1);           // Escribe el dato en cada registro  
    Wire.write(92);           // Escribe el dato en cada registro  
    Wire.write(the);           // Escribe el dato en cada registro  
    Wire.write(0);           // Escribe el dato en cada registro  
    Wire.write(0);           // Escribe el dato en cada registro  
    Wire.write(5);           // Escribe el dato en cada registro  
        Wire.write(100);           // Escribe el dato en cada registro  

    Wire.endTransmission(true); 
    
    Wire.beginTransmission(MICRO_DRCH);   // Direccion dispositivo
    Wire.write(0x00);                     // Direccion registro inicial
    Wire.write(1);           // Escribe el dato en cada registro  
    Wire.write(92);           // Escribe el dato en cada registro  
    Wire.write(the); 
    Wire.write(0);           // Escribe el dato en cada registro  
    Wire.write(0);           // Escribe el dato en cada registro  
    Wire.write(5);           // Escribe el dato en cada registro  
    Wire.write(100);           // Escribe el dato en cada registro  

    Wire.endTransmission(true); 
    delay(200);
}
