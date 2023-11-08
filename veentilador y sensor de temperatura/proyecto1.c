#include <18F4550.h>       //pic con el que vamos a trabajar
#device  high_ints= TRUE   //activar interacciones 
#device adc = 10           //le indicamos al programa el numero de bits para el ADC
#use delay (clock = 8000000)//frecuencia del crystal, en este caso sera de 8Mhz
#use standard_io(A,B,C)      //pines que se usaran en el codigo
#include <lcd_modificada.c>  //libreria modificada para usar los pines rs,rw y e en el puerto D

void set_up(){    //funcion para inicializar

   lcd_init(); //inicializamos la pantalla
   setup_adc(ADC_CLOCK_DIV_32); //Seleccionar TAD=32/Fosc
   SET_TRIS_B(0xFF);       //Configurar el puerto B como pin digital
   SET_TRIS_C(0x00);       //Configuracion del puerto C como pin digital
   setup_adc_ports(AN0_TO_AN1_ANALOG); //configuramos el pin AN0 YAN1 como pines analogicos
   enable_interrupts(int_ext1); //activamos la interrupcion externa del pin 2
   enable_interrupts(int_ext2); // activamos la interrupcion externa del pin 3
   enable_interrupts(global);//activamos las interacciones globales 

}//fin del void configurar

float convertir (int pin){      //Funcion para la conversion analogica digital
   set_adc_channel(pin);
   delay_us(12);
   return (5.0*read_adc()/1023.0); 
   return 0; 
}//fin de convertir


#int_EXT1   //para indicarle al programa que se encuentra primera interrupcion
void interrupcion_1(){

   clear_interrupt(INT_EXT1); //bajar la bandera al entrar a la interrupcion
   float temperatura[2]; // vector de 2 posiciones para guardar nuestras temperaturas
   char signo; // se alojara el signo de comparacion
   printf(lcd_putc,"\f"); // borrar pantalla 
   while(temperatura[0]<=60 || temperatura[1]<=60) // condicion para que nuestra interrupcion se salga cuando algna de las temperaturas es mayor a 60
   {
      temperatura[0] = (101.85)*convertir(0) - 45.0; // aqui usamos la ecuacion de la pendiente para calcular la temperatura con la grafica del sensor
      temperatura[1] = (50)*convertir(1) - 15.0;   // aqui usamos la ecuacion de la pendiente para calcular la temperatura con la grafica del sensor
   
   //comparaciones de temperaturas
   if(temperatura[0] > temperatura[1])
      {signo = ">";}
   
   else if(temperatura[0] < temperatura[1])
      {signo ="<";}
      
   else if(temperatura[0] == temperatura[1])
      {signo = "=";}
   
   //impresion de la informacion de la pantalla
      lcd_gotoxy(3,1);
      printf(lcd_putc,"T1");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%.2f",temperatura[0]);
      lcd_gotoxy(11,1);
      printf(lcd_putc,"T2");
      lcd_gotoxy(11,2);
      printf(lcd_putc,"%.2f",temperatura[1]);
      lcd_gotoxy(8,2);
      printf(lcd_putc,"%c",signo);
   }
   printf(lcd_putc,"\f");
}

#int_EXT2  HIGH////para indicarle al programa que aqui se encuentra segunda interrupcion
void interrupcion_2(){

   clear_interrupt(INT_EXT2);
   printf(lcd_putc,"\f");
   lcd_gotoxy(3,1);
   printf(lcd_putc,"VENTILANDO");//muestra el mensaje facilitado por el profesor
   output_high(PIN_C0);//ventilador1  a su maxima potencia
   output_high(PIN_C1);//ventilador 2 a su maxima potencia
   for(int i = 0; i<7; i++){
     lcd_gotoxy(6+i,2);//corrimiento para la impresion de puntos
     lcd_putc(".");
     delay_ms(1000);//tiempo de 1 segundo entre punto y punto
 
   }
   output_low(PIN_C0);//apagamos el motor 1
   output_low(PIN_C1);//apagamos el motor 2
   printf(lcd_putc,"\f");
}

void main(){

   set_up();
   
   float temperatura[2],por[2];
   float ciclo_trabajo[2];
   output_low(PIN_C0);
   output_low(PIN_C1);
   
   while(1){
   
   lcd_gotoxy(3,1);
   printf(lcd_putc,"Control de T");
   temperatura[0] = 101.85*convertir(0) - 45.0;
   temperatura[1] = 50*convertir(1) - 15.0;
  
  if(temperatura[0] > 49 && temperatura[0] <61) 
   {por[0] = 0.75;}
  if(temperatura[0] > 60 && temperatura[0] < 101)    
   {por[0] = 0.87;}
  if(temperatura[0] > 100)                            
   {por[0] = 1;}
  if(temperatura[1] > 49 && temperatura[1] < 76)    
   {por[1] = 0.65;}
  if(temperatura[1] > 76 && temperatura[1] < 101)    
   {por[1] = 0.76;}
  if(temperatura[1] > 100)                            
   {por[1] = 1;}
  
     ciclo_trabajo[0] = (por[0]*255) / 100;
     output_high(PIN_C0);
     delay_us(ciclo_trabajo[0]);
     output_low(PIN_C0);
     delay_us(255 - ciclo_trabajo[0]);
  
     ciclo_trabajo[1] = (por[1]* 255) / 100;
     output_high(PIN_C1);
     delay_us(ciclo_trabajo[1]);
     output_low(PIN_C1);
     delay_us(255 - ciclo_trabajo[1]);
   }//fin del while

}//fin del void main
