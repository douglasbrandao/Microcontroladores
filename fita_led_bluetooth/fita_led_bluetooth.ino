/* Código desenvolvido para controle de uma fita de led
 * Autores: Douglas Frota Brandão e Karine Rosa Ferreira Pires
 * Disciplina: Microcontroladores lecionada por Alberto Mascarenhas
 * Instituto Federal de Mato Grosso
 */

#include "BluetoothSerial.h"
#include <string.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// declaração de variáveis para armazenar a string enviada pelo app bluetooth

String valores_hexa;
char valores_hexa_array[20];

// declaração ponteiro para armazenar os valores de tokenização

const char* cores;

// variáveis inteiras para armazenar as cores rgb e o índice do último caractere

int ultimo_char, red_cor, green_cor, blue_cor;

const int red_pino = 13;
const int green_pino = 12;
const int blue_pino = 14;

// configuração frequencia pwm, canais e resolução de bits

const int frequencia = 5000;
const int red_canal = 0;
const int green_canal = 1;
const int blue_canal = 2;
const int bits = 8;

void setup() {
  Serial.begin(115200);

  // configuração do pwm letcSetup(canal, frequencia, resolução de bits)
  
  ledcSetup(red_canal, frequencia, bits);
  ledcSetup(green_canal, frequencia, bits);
  ledcSetup(blue_canal, frequencia, bits);

  // vinculo do canal com o pino ledcAttachPin(pino, canal)
  
  ledcAttachPin(red_pino, red_canal);
  ledcAttachPin(green_pino, green_canal);
  ledcAttachPin(blue_pino, blue_canal);
  
  SerialBT.begin("Fita LED - ESP32"); //Bluetooth device name
  Serial.println("O dispositivo foi iniciado. Fique a vontade para parear!");
}

void loop() {
  if (SerialBT.available()) { // verifica se está recebendo algum valor do bluetooth
    char caractere = SerialBT.read(); // lê o caractere recebido pela porta serial
    valores_hexa += caractere; // vai armazenando em um buffer temporário até encontrar um caractere ')' que indica o final do dado recebido
    if(caractere == ')'){ // se for ')', ele entra nessa condição
       ultimo_char = valores_hexa.length() - 1; // pega o índice do último caractere da string
       valores_hexa.remove(ultimo_char); // utiliza o método remove eremove o último caractere da string, que é o próprio ')' utilizando o índice que foi armazenado em ultimo_char
       
       if(valores_hexa == "ON"){ // se o botão on for apertado, os valores do duty cycle serão colocados em 255, 255, 255
          Serial.println(valores_hexa);
          ledcWrite(red_canal, 255); 
          ledcWrite(green_canal, 255);
          ledcWrite(blue_canal, 255);
       }else if(valores_hexa == "OFF"){ // se o botão off for apertado, os valores do duty cycle serão colocados em 0, 0, 0
          Serial.println(valores_hexa);
          ledcWrite(red_canal, 0); 
          ledcWrite(green_canal, 0);
          ledcWrite(blue_canal, 0);
       }else{ // e caso seja utilizado o color picker para alterar os valores, serão armazenados em variáveis conforme a a ordem de de recebimento da string enviado por bt (r, g, b)
          valores_hexa.toCharArray(valores_hexa_array, 20); // convertemos o objeto String para um array de caracteres para que possamos dividir a string
          cores = strtok(valores_hexa_array, ".");
          red_cor = atoi(cores); // recebemos a primeira posição e armazenamos na variávei red_cor
          cores = strtok(NULL, ".");
          green_cor = atoi(cores); // recebemos a segunda posição e armazenamos na variável green_cor
          cores = strtok(NULL, ".");
          blue_cor = atoi(cores); // recebemos a terceira posição e armazenamos na variável blue_cor

          // setamos o duty cycle a partir dos valores recebidos
          
          ledcWrite(red_canal, red_cor); 
          ledcWrite(green_canal, green_cor);
          ledcWrite(blue_canal, blue_cor);

          // impressão dos valores recebidos no serial
          
          Serial.println(red_cor);
          Serial.println(green_cor);
          Serial.println(blue_cor);
       }

       // limpa o buffer de armazenamento dos valores para ser feito uma nota leitura a cada loop
       
       valores_hexa = "";
       memset(valores_hexa_array, 0, sizeof(valores_hexa_array));
    }
  }
  delay(20);
}
