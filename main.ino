/* EA076 - Projeto Final: Estacionamento

    Isabella Bigatto    138537
    Júlia Dias          156019

    Objetivo: implementar um sistema que reconhece, através de um par de sensores infravermelhos, a quantidade e quais são as vagas livres em um estacionamento,
    através de um display LCD  e LEDs verdes e vermelhos.

    Referências:
    - LCD e 74HC595: https://playground.arduino.cc/Main/LiquidCrystal
    - Arduino e 74HC595: http://br-arduino.org/2015/01/aumentando-as-portas-digitais-do-arduino-com-o-ci-74hc595-shift-register.html

*/

/* Bibliotecas usadas para processamento de expressoes regulares, interrupção de tempo e integração entre o LCD e o shift-register */
#include <stdio.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <SPI.h>

LiquidCrystal lcd(10);    //LCD recebe os dados pelo Latch do CI, que está ligado ao pino 10 do Arduino

/* Vetores criados para atribuir os LEDs das vagas e os sensores aos pinos digitais do MCU */
int vaga[] = {2, 3, 4, 5, 6, 7};
int sensor[] = {8, 9, 12};

/* Variáveis e vetor auxiliares */
int valor[3];            // O tamanho desse vetor deve ser definido de acordo com o tamanho de sensor[]
int n = 0, livre = 0;

/* Inicializações e definições dos pinos do Arduino como entrada ou saída */
void setup()
{
  for (n = 0; n < 6; n++) {
    pinMode(vaga[n], OUTPUT);
  }
  for (n = 0; n < 2; n++) {
    pinMode(sensor[n], INPUT);
  }
  Timer1.initialize(3000000);                 // Interrupção a cada 3s
  Timer1.attachInterrupt(medida);            // Associa a função medida() à interrupção periódica
  Serial.begin(9600);
  lcd.begin(16, 2);                           // Define o número de colunas e linhas do LCD
}

/* Função que realiza a valor dos sensores a cada interrupção de tempo */
void medida()
{
  for (n = 0; n <= 2; n++)
    valor[n] = digitalRead(sensor[n]);
}

/* Função que realiza a comunicação com o display LCD */
void displayLCD() {

  lcd.clear();                // Limpa a tela
  lcd.setCursor(2, 0);        // Posiciona o cursor na terceira coluna e primeira linha
  lcd.print("VAGAS LIVRES");
  lcd.setCursor(7, 1);
  lcd.print(livre, DEC);      // Imprime o valor da variável livre usada no loop()
  delay(1000);
  livre = 0;

}

void loop()
{

  displayLCD();

  /*  valor[] recebe os valores digitais medidos pelos sensores de cada vaga em medida().
      Um par de posições de vaga[] representa uma vaga do estacionamento, a primeira aciona o vermelho e a segunda o verde.
      Com isso, no código abaixo, cada valor de valor [] controla um par de LEDs.
      Da maneira que o hardware foi montado, os LEDs vermelhos estão associados às posições pares do vetor e os verdes às ímpares.
  */

  for (n = 0; n <= 2; n++) {

    if (valor[n] == 0) {             // valor[n] == 0 significa vaga livre
      digitalWrite(vaga[2 * n], LOW);
      digitalWrite(vaga[2 * n + 1], HIGH);
    }
    else {
      digitalWrite(vaga[2 * n], HIGH);
      digitalWrite(vaga[2 * n + 1], LOW);
    }

    /* O bloco abaixo contabiliza quantos LEDs verdes estão acesos para que a informação seja enviada ao LCD */
    if (digitalRead(vaga[2 * n + 1]) == HIGH) {
      livre++;
    }
  }
}
