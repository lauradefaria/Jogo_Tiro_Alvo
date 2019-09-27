//TESTANDO BIBLIOTECA DO JOGO
#include <Jogo.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <String.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//DEFINIÇÃO DA LETRA CORRESPONDENTE AO ALVO, RETORNADA DO OPENCV 
#define PRIMEIRO 'a'
#define SEGUNDO 'b'
#define TERCEIRO 'c'
char ANTIGO = SEGUNDO;           //INICIA O JOGO NA SEGUNDA POSIÇÃO
int posicao = 2;
int inicia;                     //ARMAZENA O ESTADO DO BOTÃO
int i;

//DEFINE AS PORTAS DOS MOTORES, BUZZER E BOTAO QUE INICIA O JOGO
#define motor1 A0
#define motor2 A1
#define botao 10
#define buzzer 9

Jogo jogo;                           //CRIANDO O OBJETO DA CLASSE JOGO
ThreadController cpu;                //CRIANDO O OBJETO DA CLASSE THREAD

Thread Tlevanta_alvo;                //A CADA TEMPO, VERIFICAR ESSAS FUNÇÕES
Thread acerta_alvo;

// FUNÇÃO P/ LEVANTAMENTO E CHECAGEM DOS ALVOS
int num_alvo = random(3);
int time_interval = 8000;
int flag_alvo0 = 0, flag_alvo1 = 0, flag_alvo2 = 0;
int tempo = 0;
int cont_alvo = 0;                //PONTUAÇÃO
int errou = 7;                    //VIDAS

void levanta_alvo(){     //MEXE O SERVO EM 90 GRAUS, LEVANTANDO UM DOS 3 ALVO ALEATORIAMENTE
  
  jogo.set_alvo(num_alvo, 90);          //ABAIXA O ATUAL ALVO

  if(tempo == 1){                       //CASO O ALVO TENHA LEVANTADO-ABAIXADO E O USUARIO NAO ATIROU
    errou--;
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    tempo = 0;
  }

  lcd.setCursor(4,0);
  lcd.print("VIDAS = ");
  lcd.print(errou);
  lcd.setCursor(3,1);
  lcd.print("PONTOS = ");
  lcd.print(cont_alvo);
  
  jogo.setup_servos();
  num_alvo = random(3);              //DEFINE UM DOS MOTORES ALEATORIAMENTE E LEVANTA
  jogo.set_alvo(num_alvo, 180);
  tempo = 1;
}

void alvo_acertado(){         
  cont_alvo++;           //+1 NA PONTUAÇÃO
  
  if (flag_alvo0 == 1){ //ACERTOU O ALVO 1
    flag_alvo0 = 0;
  }
  if (flag_alvo1 == 1){ //ACERTOU O ALVO 2
    flag_alvo1 = 0;
  }
  if (flag_alvo2 == 1){ //ACERTOU O ALVO 3
    flag_alvo2 = 0;
  }
}

void setup() {
  
  jogo.setup_servos();              // MÉTODO DE SETUP DOS SERVO MOTORES (ALVOS)
  
  Tlevanta_alvo.setInterval(time_interval);
  Tlevanta_alvo.onRun(levanta_alvo);
  acerta_alvo.setInterval(60);
  acerta_alvo.onRun(alvo_acertado);
  
  cpu.add(&acerta_alvo);
  cpu.add(&Tlevanta_alvo);

  Serial.begin(9600);
  pinMode(motor1,OUTPUT);
  pinMode(motor2,OUTPUT);
  pinMode(botao,INPUT);
  pinMode(buzzer,OUTPUT);
  lcd.begin();
  lcd.backlight();
  
}

void mira_alvo(int caso, char anterior){                               //FUNÇÃO QUE COLOCA A ARMA NA DIREÇÃO DO ALVO DESEJADO

  switch(caso){
    
    case 1:                                                             //CASO QUEIRA IR PARA O ALVO 1
      switch(anterior){                                                 //ANALISA ONDE A ARMA ESTAVA ANTES, PARA DECIDIR SE ANDA P/ ESQUEDA OU DIREITA
        case SEGUNDO:                        
          analogWrite(motor1, 255);
          analogWrite(motor2, 0);
          delay(250);
          break;
          
        case TERCEIRO:
          analogWrite(motor1, 255);
          analogWrite(motor2, 0);
          delay(500);
          break;
          
        default:
          break;       
      }
      ANTIGO = PRIMEIRO;                                                //POSIÇÃO ATUAL, QUE DEPOIS SE TORNARÁ A ANTIGA
      break;     
      
    case 2:                                                             //CASO QUEIRA IR PARA O ALVO 2
      switch(anterior){       
        case PRIMEIRO:
          analogWrite(motor1, 0);
          analogWrite(motor2, 255);
          delay(250);
          break;
          
        case TERCEIRO:
          analogWrite(motor1, 255);
          analogWrite(motor2, 0);
          delay(500);
          break;
          
        default:
          break;       
      }
      ANTIGO = SEGUNDO;
      break;

    case 3:                                                                   //CASO QUEIRA IR PARA O ALVO 3
      switch(anterior){       
        case PRIMEIRO:
          analogWrite(motor1, 0);
          analogWrite(motor2, 255);
          delay(500);
          break;
          
        case SEGUNDO:
          analogWrite(motor1, 0);
          analogWrite(motor2, 255);
          delay(250);
          break;
          
        default:
          break;       
      }
      ANTIGO = TERCEIRO;
      break;

    default:                                                                 //USUARIO ATIROU
      
      if ((ANTIGO == PRIMEIRO) && num_alvo == 0){                            //ACERTOU O PRIMEIRO ALVO  
        flag_alvo0 = 1;
          
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }
  
      if ((ANTIGO == SEGUNDO) && num_alvo == 1){                            //ACERTOU O SEGUNDO ALVO  
        flag_alvo1 = 1;
        
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }
  
      if ((ANTIGO == TERCEIRO) && num_alvo == 2){                            //ACERTOU O TERCEIRO ALVO  
        flag_alvo2 = 1;
          
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }

      if ((ANTIGO == PRIMEIRO) && num_alvo != 0){                            //SITUAÇÕES DE ERRO
        errou--;
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
      }

       if ((ANTIGO == SEGUNDO) && num_alvo != 1){
        errou--;
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
      }
  
      if ((ANTIGO == TERCEIRO) && num_alvo != 2){
        errou--;
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
      }
  
      if (flag_alvo0 == 0 && flag_alvo1 == 0 && num_alvo == 0){
        errou--;
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
      }
      tempo = 0;                                                            //O USUARIO ATIROU
      break;    
  }
}

void loop() {
  
  inicia = digitalRead(botao);

  if(inicia == HIGH){           //SE LIGAREM O BOTÃO O PROGRAMA INICIA
    
    cont_alvo == 0;
    while(inicia == HIGH) { 
      
      cpu.run();               //RODA TODAS AS THREADS
  
      if (Serial.available()) {
        char movimento = Serial.read();   //RECEBE DO OPENCV A INFORMAÇÃO DO QUADRANTE
  
        switch(movimento){
          case PRIMEIRO:
            mira_alvo(1, ANTIGO);
            break;
          case SEGUNDO:
            mira_alvo(2, ANTIGO);
            break;
          case TERCEIRO:
            mira_alvo(3, ANTIGO);
            break;
          default:
            mira_alvo(4, '0');
            break;
        }
  
        if (errou == 0){    //ZEROU TODAS AS VIDAS, DESLIGA O JOGO E REINICIA
          errou = 7;
          inicia = 0;
          lcd.setCursor(4,0);
          lcd.print("GAME OVER");
          delay(3000);
          mira_alvo(2, ANTIGO);       //COLOCA A ARMA NA POSIÇÃO INICIAL
        }
      }
      
      // PARA MOTOR QUANDO NÃO TIVER NADA ACIONADO
      analogWrite(motor1, 0);
      analogWrite(motor2, 0);
    }
  }

  else{                             //ENQUANTO O JOGO NÃO ESTIVER RODANDO

    String str = String(cont_alvo); //TRANSFORMA O INT-STRING
    Serial.println(str);            //ATUALIZA O PLACAR (ENVIA O VALOR PARA O COMPUTADOR)
    char *placar = Serial.read();   //LÊ O VALOR DO PLACAR 
    lcd.setCursor(5,0);
    lcd.print("PLACAR");
    lcd.setCursor(0,1);
    lcd.print("1 - PONTOS: ");
    lcd.print(placar);
    delay(2000);
  }
}
