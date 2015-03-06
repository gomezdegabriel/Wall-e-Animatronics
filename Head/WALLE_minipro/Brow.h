#ifndef BROW_H
#define BROW_H

struct _BROW
{
	_SERVOW servoI;
	_SERVOW servoE;
	
        int theta2[5];
        int theta5[5];
        int mod;
        int nextMod;
        int finalMod;
	

        /*
        *@brief Inicializa y prepara los servo motores de la ceja
        *@param pinI Pin del servo interior de la ceja
        *@param minAngleI Angulo minimo al que puede moverse el servo interior
        *@param maxAngleI Angulo maximo al que puede moverse el servo interior
        *@param minPulseI Pulso necesario para mover 1 grado el servo interior
        *@param maxPulseI Pulso necesario para mover 180 grados el servo interior
        *@param pinE Pin del servo exterior de la ceja
        *@param minAngleE Angulo minimo al que puede moverse el servo exterior
        *@param maxAngleE Angulo maximo al que puede moverse el servo exterior
        *@param minPulseE Pulso necesario para mover 1 grado el servo exterior
        *@param maxPulseE Pulso necesario para mover 180 grados el servo exterior
        */
        void init(int pinI, int minAngleI, int maxAngleI, int minPulseI, int maxPulseI, int pinE, int minAngleE, int maxAngleE, int minPulseE, int maxPulseE);
        
        /*
        *@brief Obtiene el recorrdio en grados para llegar a la posicion especificada
        *@param theta Indica si es para el servo interior o para el servo exterior
        *@return Grados que faltan para completar el recorrido completo del servo indicado
        */
        int getTotalDif(bool theta);
        
        /*
        *@brief Pestañea una vez
        *@param time tiempo del pestañeo dividido entre 6 aproximadamente
        */   
        int getDif(bool theta);
        
        /*
        *@brief Pestañea una vez
        *@param time tiempo del pestañeo dividido entre 6 aproximadamente
        */
        void nextPos();
        
        /*
        *@brief Pestañea una vez
        *@param time tiempo del pestañeo dividido entre 6 aproximadamente
        */
        void checkPos();

	//void setLimits(float x1, float y1, float y2);
	//bool moveTo(int porcentI, int porcentE, bool master, int velocidad);
	
};

#endif

