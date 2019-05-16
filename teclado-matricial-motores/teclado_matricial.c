#include <16F877A.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

#include <flex_kbd.c> // biblioteca do teclado matricial 3x4

// Variáveis do motor de passo

int1 sentido = 0;
int8 tempo, cont = 0; // variaveis pro timer 0
int8 passos[4] = {0x0A, 0x09, 0x05, 0x06};
signed int i = 0;

// Variáveis do motor dc

int16 velocidade_dc = 0;
int1 sentido_dc = 0;

char c; // caractere para guardar os valores do teclado matricial

#INT_RTCC //interrupção do timer 0
void  RTCC_isr(void) 
{

   // motor de passo
   
   if(tempo > 0){
      cont++;
      if (sentido == 1){
       if (i > 3) i = 0; 
         if(cont >= tempo){
            output_d(passos[i]);
            i++;
            cont = 0;
         }
      }else{
         if (i < 0) i = 3;
         if(cont >= tempo){
            output_d(passos[i]);
            i--;    
            cont = 0;
         }
      }
   }
   
   // motor dc
   
   if(sentido_dc == 1){
      set_pwm2_duty((int16)0); // ccp2
      delay_us(50);
      set_pwm1_duty((int16)velocidade_dc); // liga ccp1
   } else{
      set_pwm1_duty((int16)0); // ccp1
      delay_us(50);
      set_pwm2_duty((int16)velocidade_dc); // liga ccp2
   }
   
}


void main()
{
   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32|RTCC_8_BIT);      //1,6 ms overflow
   setup_timer_2(T2_DIV_BY_16,255,1);    p_timer_  //819 us overflow, 819 us interrupt
   
   // configuração do pwm
   
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   set_pwm1_duty((int16)0);
   set_pwm2_duty((int16)0);
   
   enable_interrupts(INT_RTCC);
   enable_interrupts(GLOBAL);
   
   kbd_init();
   
   char c;
   
   while(true)
   {
   
      velocidade_dc = read_adc(); // leitura da porta analógica A0
      delay_us(50);
      
      c = kbd_getc();
      delay_ms(1);
      
      if(input(PIN_A1)){ //botao do motor dc
         while(input(PIN_A1)) {;} // enquanto o botão tá apertado ele fica dentro do loop e só sai quando solta o botão
         if(sentido_dc == 1){
            sentido_dc = 0;
         } else{
            sentido_dc = 1;
         }
      }
      
      if(c != 0)
      {  
         
         
         switch(c){
            case '1':
               tempo = 85;
               break;
            case '2': 
               tempo = 65; 
               break;
            case '3': 
               tempo = 45; 
               break;
            case '4': 
               tempo = 25; 
               break;
            case '5': 
               tempo = 5; 
               break;
            case '0': 
               tempo = 0; 
               break;
            case '*': 
               sentido = 0; 
               break;
            case '#': 
               sentido = 1; 
               break;
            default: break;
         }
      }
      
      
      
   }
}
