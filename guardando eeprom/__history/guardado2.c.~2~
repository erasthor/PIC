#include <18F4550.h>
#device adc=10
#FUSES NOWDT, HSPLL, PUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOCPD, NOWRT, HS
#use delay (clock=4M, crystal=4M)
#use standard_io(A,B,D)



void main(){

int numero[100], n=0;

   while(True){
   
     for(int number = 0; number<256; number ++ ){
     
     write_eeprom(number, numero[number]);
     
     }//fin del for
   
   
   }//fin del while
   
}//fin del void main
