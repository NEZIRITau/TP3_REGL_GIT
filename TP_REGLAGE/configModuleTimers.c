//----------------------------------------------------------------------------//
// Nom du projet 		:	Reglage Encodeur
// Nom du fichier 		:   configOscillateur.h 
// Date de cr�ation 	:   03.12.2020
// Date de modification : 	25.01.2022
// 
// Auteur 				: 	Philou (Ph. Bovey) 
//                      :   Michel Bonzon 
//                      :   Kevin Bougnon 
//
// Description 			: 	Canevas pour le laboratoire de r�glage avec la carte
//                          1601_HalfBridge 
//                         
// Remarques			: 
// 	    chemin pour trouver le headerfile 
//		C:\Program Files\Microchip\MPLAB C30\support\dsPIC33F\h
//
//      Hardware de la carte => ATTENTION PIC MONTER SUR LA CARTE DSPIC33FJ128MC802
//      K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc 
//
//	    doc pour le DSP : 
//		K:\ES\PROJETS\SLO\1601x_HalfBridge3x\doc\datasheets
// 
//----------------------------------------------------------------------------//
//--- librairie � inclure ---// 
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#endif

#include <stdint.h>

#include <xc.h>                 // appel de directive du compilateur C pour le Dspic 
#include "configTimer.h"

//-- d�claration de variables utilis�s dans diff�rents fichiers --// 
extern int8_t flagInterrupt;
extern long temps_1ms; 

//----------------------- CONFIGURATION TIMER1 -------------------------------//
//----------------------------------------------------------------------------//
//-- nom 				: Timer1Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration Timer1 
//-- remarque 			: datasheet DSPIC -> timer section 12 -> p195
//                        configuration du timer1 � 100kHz = 10us  
//                        Fcy : 36.85 [MHz]
//                        nb_tic = Fcyc / Ft1 
//                        36.85 x 10^6 / 100 x 10^3 = 36.85
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
void Timer1Init(void)
{
    T1CON = 0;              // Timer reset
    
    //-- configuration des registres de comptage 
 	TMR1=  0x0000;  	
	PR1 = 368;               // Timer1 period register = ?????
    
    //-- configuration des 3 registres li� au timer 1 --// 
    /*IFS0bits.T1IF = 0;      // interruption pas activ�e => remise � z�ro
    IPC0bits.T1IP = 6;      // priorit� 7 (la plus �lev�e)
    IEC0bits.T1IE = 1;*/      // activer l'interruption 
    
    //-- appel fct pour configuration des registres d'interruption --// 
    InterruptTimer1Init(); 

	T1CONbits.TON = 1;      // Enable Timer1 and start the counter
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer1Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration interruption li� au Timer1   
//-- remarque 			: datasheet DSPIC -> timer section 7 -> p89
//
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
inline void InterruptTimer1Init(void)
{
    //-- configuration des 3 registres li� au timer 1 --// 
    IFS0bits.T1IF = 0;      // interruption pas activ�e => remise � z�ro
    IPC0bits.T1IP = 5;      // priorit� 7 (la plus �lev�e)
    IEC0bits.T1IE = 1;      // activer l'interruption 
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer1Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration interruption li� au Timer1 
//                        pour avoir une interruption toutes les 10us
//-- remarque 			: datasheet DSPIC -> timer section 7 -> p89
//
//-- modification       : 25.01.2021
//----------------------------------------------------------------------------//
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt( void )
{
    //-- d�claration de variables --// 
    static int compteur_100us = 10;     // ayant un timer de 10us => 
                                        // initialisation d'une varibale � 
                                        // repr�sentant 1ms => 1ms / 10us = 100
    
    IFS0bits.T1IF = 0;      // interruption pas activ�e => remise � z�ro
    T1CONbits.TON = 0;      // Disable Timer1 
    
    //-- test pour savoir si 1ms est pass�e 
    if(compteur_100us > 0)
        compteur_100us--;         //-- d�cr�mente  
    else 
    {
        compteur_100us = 10;     //-- Remise � jour du compteur 
        temps_1ms++;             //-- variable de temps utilis� dans le main 
    }
    
    
    //-- test de test pour voir si on rentre dans l'interruption --// 
    if (flagInterrupt == 0) 
    {
        //LATBbits.LATB15 = 0;//~LATBbits.LATB15;
        flagInterrupt = 1; 
    }
    
    //~LATBbits.LATB15; //LATBbits.LATB15 = 0;//~LATBbits.LATB15;

    TMR1          = 0;          // remise � z�ro du registre du compteur 
	T1CONbits.TON = 1;          // Enable Timer1 
}
//----------------------- FIN CONFIGURATION TIMER1 ---------------------------//


//----------------------- CONFIGURATION TIMER2 -------------------------------//
//----------------------------------------------------------------------------//
//-- nom 				: Timer2Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration Timer1 
//-- remarque 			: datasheet DSPIC -> timer 2/3 section 13 -> p199
//                        configuration du timer1 � 10kHz = 100us  
//                        Fcy : 36.85 [MHz]
//                        nb_tic = Fcyc / Ft1 
//                        36.85 x 10^6 / 10 x 10^3 = 3685
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//
void Timer2Init(void)
{
    T2CON = 0;              // Timer reset en mode 16bit 
 	
 	TMR2 = 0x0000;          // MAJ des registres 
	PR2 = 3703;             // Timer1 period register = ?????
	
    //-- appel fct pour configuration des registres d'interruption --// 
    InterruptTimer2Init(); 
   
    //T2CONbits.TON = 1;      // Enable Timer2 and start the counter
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer2Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration interruption li� au Timer2   
//-- remarque 			: datasheet DSPIC -> interuption section 7 -> p89
//
//-- modification       : 10.02.2021
//----------------------------------------------------------------------------//
inline void InterruptTimer2Init(void)
{
    //-- configuration des 3 registres li� au timer 1 --// 
    IFS0bits.T2IF = 0;      // interruption pas activ�e => remise � z�ro
    IPC1bits.T2IP = 4;      // priorit� 7 (la plus �lev�e)
    IEC0bits.T2IE = 1;      // activer l'interruption 
}

//----------------------------------------------------------------------------//
//-- nom 				: InterruptTimer2Init
//-- entr�e - sortie 	: - / - 
//-- description 		: configuration interruption li� au Timer1   
//-- remarque 			: datasheet DSPIC -> timer section 7 -> p89
//
//-- modification       : 03.12.2020
//----------------------------------------------------------------------------//
void __attribute__((interrupt,no_auto_psv)) _T2Interrupt( void )
{
    IFS0bits.T2IF = 0;          // remise � z�ro du flag d'interruption 
    T2CONbits.TON = 0;          // Arr�t du timer 
    TMR2          = 0;          // remise � z�ro du registre du compteur du Timer2 
	
    
    
    
    T2CONbits.TON = 1;          // activation du timer 
}
//----------------------- FIN CONFIGURATION TIMER2 ---------------------------//