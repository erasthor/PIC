#include <18F4550.h>
#device adc=10
#FUSES NOWDT, HSPLL, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOCPD, NOWRT, HS
#use delay (clock=4M, crystal=4M)
#use standard_io(A,B,D)
#include <lcd.c>


typedef enum {MAYOR, IGUAL, MENOR} comparacion_t;
const char comparaciones[] = {'>', '=', '<'};
int8 Data_H, Data_L, D_0, D_1,k,j,top;
int16 salida_digital;
float memoria,tension[2];
int i;

   void configurar_CAD(){
      SET_TRIS_B(0);
      lcd_init();   
      setup_adc(ADC_CLOCK_DIV_32); //Seleccionar TAD=32/Fosc
      setup_adc_ports(AN0_TO_AN1_ANALOG); //ENTRADA ANALOGICA PIN RA0,RA1   
   }//fin de la configuracion

  float convertir (int canal){
   set_adc_channel(canal);
   delay_us(10);
   return (5.0*read_adc()/1023.0);   
}//fin de convertir
  

   void resultado(){
                              
   lcd_gotoxy(5,1);
   printf(lcd_putc,"TENSIONES");
         
     for (int i=0;i<2;i++){//guardamos los valores medidos en vector tension de dos posiciones. 
      tension[i]=convertir(i);
      comparacion_t comparador = MAYOR;
    if (tension[0]== tension[1])
    comparador = IGUAL;
    if (tension[0] < tension[1])
    comparador = MENOR;
    if (tension[0] > tension[1])
    comparador = MAYOR;
         lcd_gotoxy(1,2);
         printf(lcd_putc,"VA:%1.2f %cVB:%1.2f",tension[0],comparaciones[comparador],tension[1]);//muestra el valor guardado en la posicion 0 del vector 1
     
         }
   }//fin del resultado 
   
   void escribir_eeprom(){// guarda datos en la memoria eeprom
    
      if(tension[0]>tension[1])
         salida_digital=tension[0]*1024/5;
      
      if(tension[1]>tension[0])
        salida_digital=tension[1]*1024/5;
        
         delay_ms(100);
         Data_H=make8(salida_digital,1); //divide y guarda el valor de la variable 
         Data_L=make8(salida_digital,0); 
         write_eeprom(k,Data_H);
         write_eeprom(k+1,Data_L);
         k+=2;
     
   }//fin del void escribir
   
   void leer_memoria(){
   top=k;
 
   while (j<top){
     
     D_0=READ_EEPROM(j);
     D_1=READ_EEPROM(j+1);
     memoria=(make16(D_0,D_1)*5)/1023.0;
     lcd_gotoxy(3,2);
     printf(lcd_putc,"%f",memoria);
     j+=2;
     delay_ms(500);
   }
   }//fin de la funcion leer
   
void main(){

   configurar_CAD();
   while(1){
       
      if(input(PIN_B0)==0b01){
         
         printf(lcd_putc,"\f");
         lcd_gotoxy(3,1);
         printf(lcd_putc,"GUARDADO");
         delay_ms(1000);
         
         if(tension[0]>tension[1])
            escribir_eeprom();
        
         if(tension[1]>tension[0])
            escribir_eeprom();
         
          }
      
      if(input(PIN_B1)==0b01){
         
         printf(lcd_putc,"\f");
         lcd_gotoxy(3,1);
         printf(lcd_putc,"Valores");
         leer_memoria();
      } 
      
      else{
      delay_ms(800);
      printf(lcd_putc,"\f");
      resultado();
      }
      
   }//fin del while
}//fin del void main
