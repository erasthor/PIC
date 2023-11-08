#include <18F4550.h>
#use delay (clock=4000000)
#use standard_io(B,D)
#include <lcd.c>
#include "teclado.c"

char tecla;
int contador;
int suma=0;
int clave=10;
void contrasenacorrecta();
void configurar();
void leer_y_mostrar();

void configurar()
{
   setup_adc_ports(NO_ANALOGS);
   port_b_pullups(TRUE);
   kbd_init();
   lcd_init();
   return;
}

void leer_y_mostrar()
{
   tecla= kbd_getc();
   if (tecla!=0)
   {
   if (tecla>='0' && tecla<='9')
   {
     printf(lcd_putc," *");
     delay_ms(10);
     tecla=tecla-48;
     delay_ms(10);
     suma=suma+tecla;
   }
   }
  if (tecla=='=')
   {
   if(suma==clave)
   {
     lcd_gotoxy(1,1);
     printf(lcd_putc,"    You're       ");
    lcd_gotoxy(2,2);
     printf(lcd_putc,"  Terminator     ");
     delay_ms(10);
    suma=0;
   }
    else
    {
     lcd_gotoxy(1,1);
     printf(lcd_putc,"   INVALID      ");
    lcd_gotoxy(2,2);
     printf(lcd_putc,"  PASSWORD      ");
     delay_ms(1000);
     contador++;
     lcd_gotoxy(1,1);
     printf(lcd_putc,"               ");
    lcd_gotoxy(2,2);
    printf(lcd_putc,"  ENTRY:%u ",contador);
    delay_ms(500);
    lcd_gotoxy(1,2);
     printf(lcd_putc,"               ");
    delay_ms(5);
      lcd_gotoxy(5,1);
     printf(lcd_putc,"PASSWORD:         ");
     lcd_gotoxy(4,2);
    suma=0;
    }
     if (contador==3)
     {
        lcd_gotoxy(1,1);
     printf(lcd_putc,"                ");
     lcd_gotoxy(2,2);
     printf(lcd_putc,"                ");
     delay_ms(10);
     lcd_gotoxy(1,1);
     printf(lcd_putc,"     BLOCKED           ");
     lcd_gotoxy(2,2);
     printf(lcd_putc,"     SKYNET           ");
     delay_ms(1000);
     }
   }

  if (tecla==':')
  {
  contador=0; 
  reset_cpu();
  
  }
return;
}

 void main()
 {
 configurar();
 lcd_gotoxy(5,1);
printf(lcd_putc," WELCOME       ");
 lcd_gotoxy(5,2);
 printf(lcd_putc,"TO SKYNET     ");
 delay_ms(5000);
  lcd_gotoxy(5,1);
printf(lcd_putc,"PASSWORD:         ");
 lcd_gotoxy(5,2);
 printf(lcd_putc,"                 ");
  delay_ms(50);
lcd_gotoxy(4,2);
 while(1)
 {
leer_y_mostrar();
 }
 return;
 }
