#include <18F4550.h> //parea usar el pic 18F4550
#device  high_ints= TRUE   //activar interacciones 
#device adc=10    //indica el numero de bits que vamos a trabajar 
#FUSES NOWDT, HSPLL, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOCPD, NOWRT, HS
#use delay (clock = 4M)
#use standard_io(A,B,C,D,E)
//#byte portb = 0xf81 // Identificador para el puerto B.
#include <lcd2.c>

register _portb unsigned int8 PORTB;
register _trisb unsigned int8 TRISB;

int cont  = 0, Timer2, Poscaler;

void configurar_CAD(){ // setup
   
   lcd_init();   
   setup_adc(ADC_CLOCK_DIV_32); //Seleccionar TAD=32/Fosc
   setup_adc_ports(AN0_TO_AN1_ANALOG); //ENTRADA ANALOGICA PIN RA0,RA1
   TRISB = 0xFF;       //Configurar el puerto B
   ENABLE_INTERRUPTS(GLOBAL);    //Habilita todas las interrupciones
   ENABLE_INTERRUPTS(INT_EXT);   //Habilita la interrupción externa en el pin RBO
   set_adc_channel(0);  
   
   // Generemos una Señal cuadrada de 1 Khz
   Timer2=249;    //Se carga timer 2 con 249 como lo vimos en la pagina
   //Preescaler=4;  //Preescaler solo puede tomar valores de: 1 - 4 - 16
   //Para el preescaler colocamos "t2_div_by_4"
   Poscaler=1;  //Preescaler solo puede tomar valores de: 1
   setup_timer_2(t2_div_by_4,Timer2,Poscaler);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);       //Configurar modulo CCP1 en modo PWM
   
}//fin de la configuracion

#INT_EXT      
void interrupcion(){ //funcion para la interrupcion modo analogo o digital
   
   clear_interrupt(INT_EXT); //bajar bandera
   delay_ms(200);    //Retardo
   cont++;//Incremente la variable CONT
   if(cont==6)      //Si la variable CONT es igual a 10
      cont=0;        //Ponga en cero la variable CONT
   // cont = (cont == 6)? 0: cont++;
   
}// Fin de interrupcion 1    
     
void main(){

   configurar_CAD();
   unsigned int16 tension, duty;
   int velocidad = 0;
   
   while(1){
      
      //tension = read_adc();//convertir(tension);    //aqui llamamos la funcion CAD y le mandamos el pin con el que deseamos trabajar
      tension = read_adc();
      switch (cont){
         case 0:
            duty = tension;
            break;
         case 1:
            duty = 100;    //trabajando con el 10%
            velocidad = 10;
            break;
         case 2:
            duty = 250;    //trabajando con el 25%
            velocidad = 25;
            break;
         case 3:
            duty = 500;    //trabajando con el 50%
            velocidad = 50;
            break;
         case 4:
            duty = 750;    //trabajando con el 75%
            velocidad = 75;
            break;
         case 5:
            duty = 921;    //trabajando con el 90%
            velocidad = 90;
         break;
      }
      set_pwm1_duty(duty);
     
      lcd_gotoxy(1,1);
      printf(lcd_putc,"TENSION");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%f V", 5.0 * tension / 1023);
      lcd_gotoxy(9,1);
      printf(lcd_putc," N:%i",cont);
      lcd_gotoxy(9,2);
      printf(lcd_putc,"SPWM:%i%%:",velocidad);
        
   }//fin del while
}//fin del void main
