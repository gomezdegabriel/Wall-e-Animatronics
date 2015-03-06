#ifndef EYES_H
#define EYES_H

struct _EYE
{
    
  static const uint8_t PIN_LUZ1=2;
  static const uint8_t PIN_LUZ2=3;
  static const uint8_t PIN_LUZ3=4;
  static const uint8_t PIN_LUZ4=5;
  static const uint8_t PIN_LUZ5=6;
  static const uint8_t PIN_LUZ6=7;
  static const uint8_t PIN_LUZ7=8;
  static const uint8_t PIN_LUZ8=9;
  static const uint8_t PIN_INTENSIDAD_LUZ=11;
  static const int NUM_LEDS=8;
  
   int bright;
   volatile bool led[NUM_LEDS];
     
  /* 
  *@brief Configura los pines del objeto y etablece los valores iniciales
  */
  void init();
  
  /* 
  *@brief Abre el ojo poco a poco
  *@param startBright brillo final del ojo
  */
  void openLazy();
  
  /* 
  *@brief Cierra el ojo
  *@param time tiempo en cerrar cada una de las 3 partes del ojo
  */
   void closeEye(int startBright, int time);
  
  /* 
  *@brief Abre el ojo
  *@param time tiempo en abrir cada una de las 3 partes del ojo
  *@param startBright brillo final del ojo
  */
   void openEye(int endBright, int time);
  
  /*
  *@brief Pestañea una vez
  *@param time tiempo del pestañeo dividido entre 6 aproximadamente
  */
   void wink(int time);
  
  /*
  *@brief Apaga o enciendo el vector de 8 leds
  *@param light Vector con el estado deseado de cada led
  */
   void setLight(volatile bool light[]);
  
  /*
  *@brief Cambia el brillo de forma gradual desde el brillo actual al deseado
  *@param time tiempo en realizar la transicion de un brillo a otro
  */
   void setBright(int newBright, int time);
  
  /*
  *@brief Cambia el brillo de forma instantanea
  */
   void setBright(int newBright);

  /* 
  *@brief Hace un gesto con el ojo
  */
  void mode(int newMode);
  
};

#endif


