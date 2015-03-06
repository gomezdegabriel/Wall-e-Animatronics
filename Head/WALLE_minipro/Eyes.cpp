#include <Arduino.h>
#include "Temp.h"
#include "Eyes.h"


void _EYE::init()
{
 // Configuramos pin de intensidad como salida
  pinMode(PIN_INTENSIDAD_LUZ, OUTPUT);
  this->setBright(0);
  
  for(int thisLed=PIN_LUZ1; thisLed<=PIN_LUZ8; thisLed++)
  {
    pinMode(thisLed, OUTPUT);
    digitalWrite(thisLed, LOW);
  }
    
}

void _EYE::openLazy()
{
  int startBright=this->bright;
  setBright(1, 0);
  led[3]=true;
  led[1]=true;
  setLight(led);
  setBright(startBright/50+1, 800);
  setBright(0, 1000);
  setBright(startBright/15+1, 1000);

  led[5]=true;
  led[6]=true;
  setLight(led);
  setBright(startBright/10+1, 500);
  setBright(startBright/50+1, 400);
  
  led[5]=false;
  led[6]=false;
  setLight(led);
  setBright(startBright/255+1, 900);
  
  led[5]=true;
  led[6]=true;
  setLight(led);
  setBright(startBright/10,500); 
  
  led[4]=true;
  led[7]=true;
  setLight(led);
  setBright(startBright/5+1,200); 
  setBright(startBright/10+1,400); 

  led[4]=false;
  led[7]=false;
  setLight(led);
  setBright(startBright/15+1,100); 

  led[5]=false;
  led[6]=false;
  setLight(led);
  setBright(startBright/51+1,600); 
  
  setBright(startBright/15+1,400); 

  led[5]=true;
  led[6]=true;
  setLight(led);
  setBright(startBright/12+1,500); 
  led[4]=true;
  led[7]=true;
  setLight(led);
  setBright(startBright/10+1,500); 

  led[2]=true;
  setLight(led);
  setBright(startBright/5+1,1000); 
  wink(200);
  
  led[0]=true;
  setLight(led);
  setBright(startBright,600); 
  wink(150);
  
  delayW(1000);
}

// --------------------------------------------------
void _EYE::closeEye(int startBright, int time)
{
  delayW(time);
  setBright(startBright/2,time);
  led[0]=false;
  led[2]=false;
  setLight(led);
  
  setBright(startBright/4,time);
  led[5]=false;
  led[6]=false;
  led[4]=false;
  led[7]=false;
  setLight(led);

  
  setBright(1,time);
  led[3]=false;
  led[1]=false;
  setLight(led);
  setBright(0,0);
}

// ------------------------------------------------
void _EYE::openEye(int endBright, int time)
{

  setBright(1,0);
  
  led[3]=true;
  led[1]=true;
  setLight(led);
 
  setBright(endBright/4,time);
  led[5]=true;
  led[6]=true;
  led[4]=true;
  led[7]=true;
  setLight(led);
  
  setBright(endBright/2,time);
  led[0]=true;
  led[2]=true;
  setLight(led);
  setBright(endBright,time);

}

// ------------------------------------------------
void _EYE::wink(int time)
{
  int startBright=this->bright;
  
  closeEye(startBright, time);
  delayW(time+time/2);
  openEye(startBright, time);
    
}

// ------------------------------------------------
void _EYE::setLight( volatile bool light[])
{
  
  int i=0;
  for(int thisLed=PIN_LUZ1; thisLed<=PIN_LUZ8; thisLed++)
  {
    digitalWrite(thisLed, light[i]);
    i++;
  }
    
}

// ------------------------------------------------
void _EYE::setBright(int newBright, int time)
{
  int dif=0; 
  int next=1;
  int ms=1;
  
  if(time !=0)
  {
      if (newBright > bright)
      {
          dif=newBright-bright;
       
          if(dif<=time)
          {
            ms=time/dif;
          }
          else
          {
            next=dif/time;
          }
        
        
          for(int thisBright=bright; thisBright<=newBright; thisBright=thisBright+next)
          {
              analogWrite(PIN_INTENSIDAD_LUZ, thisBright);
              delayW(ms);  
          }
      }
      
      if (newBright < bright)
      {
        
        dif=bright-newBright;
        
  
          if(dif<=time)
          {
            ms=time/dif;
          }
          else
          {
            next=dif/time;
          }
        
        for(int thisBright=bright; thisBright>=newBright; thisBright=thisBright - next)
        {
          analogWrite(PIN_INTENSIDAD_LUZ, thisBright);
          delayW(ms);  
        }
      }
  }
  else
  {
    setBright(newBright);
  }
  
  this->bright=newBright;
  
}

// ------------------------------------------------
void _EYE::setBright(int newBright)
{
  analogWrite(PIN_INTENSIDAD_LUZ, newBright);
  this->bright=newBright;

}


void _EYE::mode(int newMode)
{
  
  switch (newMode)
  {
    case 1:
       openLazy();
       break;
    
    case 2:
       closeEye(this->bright, 200);
       break;
       
    case 3:
       openEye(this->bright, 200); 
       break;
       
    case 4:
        wink(30);
        break;
        
    default:
    break;
    
  }   
    
    
  
  
}





