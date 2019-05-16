#include <semaforo.h>

// declaração dos leds e os respectivos pinos

#define PEDESTRE_VERMELHO PIN_D0
#define PEDESTRE_VERDE PIN_D1
#define CARRO_VERMELHO PIN_D4
#define CARRO_AMARELO PIN_D5
#define CARRO_VERDE PIN_D6
#define PEDESTRE_BOTAO PIN_B0

// delays

#define DELAY_AMARELO 1000 // delay de atenção do sinal
#define DELAY_CLICA_BOTAO 2000 // delay pra abrir o sinal após o pedestre apertar o botão (2 segundos)
#define DELAY_PEDESTRE 10000 // delay para o pedestre passar (10 segundos)

void main(){

   int i;

   output_low(CARRO_VERMELHO);
   output_low(CARRO_AMARELO);
   output_high(CARRO_VERDE);
   
   output_high(PEDESTRE_VERMELHO);
   output_low(PEDESTRE_VERDE);

   while(TRUE){

      
      if(input(PEDESTRE_BOTAO) == 1){ // leitura do estado do botão (1 p/ HIGH e 0 p/ LOW)
      
         // desliga o verde e acende o amarelo
         
         delay_ms(DELAY_CLICA_BOTAO);
         output_low(CARRO_VERDE);
         output_high(CARRO_AMARELO);
         
         // desliga o amarelo e acende o vermelho
         
         delay_ms(DELAY_AMARELO);
         output_low(CARRO_AMARELO);
         output_high(CARRO_VERMELHO);
         
         // desliga o vermelho do pedestre e acende o verde
         
         delay_ms(DELAY_AMARELO);
         output_low(PEDESTRE_VERMELHO);
         output_high(PEDESTRE_VERDE);
         
         delay_ms(DELAY_PEDESTRE); // tempo para o pedestre passar
         
         output_low(PEDESTRE_VERDE); // desliga o verde pro pedestre
         
         for(i=0;i<5;i++){
            output_high(PEDESTRE_VERMELHO);
            delay_ms(500);
            output_low(PEDESTRE_VERMELHO);
            delay_ms(500);
         }
         
         output_high(PEDESTRE_VERMELHO);
         
         // desliga o vermelho pros carros e acende o verde e volta pro estado inicial
         
         delay_ms(DELAY_AMARELO);
         output_low(CARRO_VERMELHO);
         output_high(CARRO_VERDE);
     
         output_low(PEDESTRE_BOTAO); // abaixa a saída do botão do pedestre
         
      }

   }

}
