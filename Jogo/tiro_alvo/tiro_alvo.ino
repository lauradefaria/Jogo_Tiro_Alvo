//TESTANDO BIBLIOTECA DO JOGO
#include <Jogo.h>
#include <Thread.h>
#include <ThreadController.h>
#include <NewPing.h>
#include <AFMotor.h>
#include <U8glib.h>

// DEFINIÇÕES DO SENSOR ULTRASSONICO 
#define trigger_pin 27
#define echo_pin 26
#define dist_maxima 32 //DISTÂNCIA DADA EM CENTÍMETROS
NewPing ultrasonic (trigger_pin, echo_pin, dist_maxima);

//DEFINIÇÃO DA LETRA CORRESPONDENTE AO ALVO, RETORNADA DO OPENCV 
#define PRIMEIRO 'a'
#define SEGUNDO 'b'
#define TERCEIRO 'c'
char ANTIGO = SEGUNDO;           //INICIA O JOGO NA SEGUNDA POSIÇÃO
int posicao = 2;
int inicia;
int i;

//DEFINE AS PORTAS DOS LEDS CORRESPONDENTES AOS ALVOS E AO BOTAO QUE INICIA O JOGO
#define LED1 13
#define LED2 12
#define LED3 11
#define botao 10
#define buzzer 9

//DEFINIÇÃO PARA CONTROLE DO MOTOR DA ARMA
AF_DCMotor motor(4); //SELECIONA O MOTOR 4

Jogo jogo; //CRIANDO O OBJETO DA CLASSE JOGO
ThreadController cpu;

Thread Tlevanta_alvo;                //A CADA TEMPO, VERIFICAR ESSAS FUNÇÕES
Thread debug_alvo;
Thread atualiza_distancia;
Thread acerta_alvo;


// FUNÇÃO QUE ATT DISTÂNCIA DA ARMA
int distancia = 0;
void atualiza_dist(){
  distancia = ultrasonic.ping_cm();
}

// FUNÇÃO P/ LEVANTAMENTO DOS ALVOS
int num_alvo = random(3);
int time_interval = 8000;
//int marcador_alvo = 0;
int flag_alvo0 = 0, flag_alvo1 = 0, flag_alvo2 = 0;
int tempo = 0;
int cont_alvo = 0;
int errou = 7;

void levanta_alvo(){     //MEXE O SERVO EM 90 GRAUS, LEVANTANDO UM DOS 3 ALVO ALEATORIAMENTE
  //jogo.set_alvo(num_alvo, 90);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  
  jogo.set_alvo(num_alvo, 90);

  if(tempo == 1){
    errou--;
    digitalWrite(buzzer, HIGH)
    delay(500);
    digitalWrite(buzzer, LOW)
    tempo = 0;
  }
  
  jogo.setup_servos();
  num_alvo = random(3);
  jogo.set_alvo(num_alvo, 180);
  tempo = 1;
  
  if(num_alvo == 1){
    digitalWrite(LED1, HIGH);
  }
  if(num_alvo == 2){
    digitalWrite(LED2, HIGH);
  }
  if(num_alvo == 3){
    digitalWrite(LED3, HIGH);
  }
}

void alvo_acertado(){         
  cont_alvo++;          //+1 NA PONTUAÇÃO
  
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

/*void retorno_alvo(){          //FUNÇÃO QUE RETORNA STATUS DO ALVO LEVANTADO
  
  if (num_alvo == 0){
    //Serial.println("Debug_num_alvo - 0");
    marcador_alvo = 0;
  } 
  if(num_alvo == 1){
    //Serial.println("Debug_num_alvo - 1");
    marcador_alvo = 1;
  }
  if(num_alvo == 2){
    //Serial.println("Debug_num_alvo - 2");
    marcador_alvo = 2;
  }
}*/

void setup() {

  disp_graph_init();
  
  jogo.setup_servos();              // MÉTODO DE SETUP DOS SERVO MOTORES (ALVOS)
  
  Tlevanta_alvo.setInterval(time_interval);
  Tlevanta_alvo.onRun(levanta_alvo);
  debug_alvo.setInterval(200);
  debug_alvo.onRun(retorno_alvo);
  atualiza_distancia.setInterval(50);
  atualiza_distancia.onRun(atualiza_dist);
  acerta_alvo.setInterval(60);
  acerta_alvo.onRun(alvo_acertado);
  
  cpu.add(&acerta_alvo);
  cpu.add(&atualiza_distancia);
  cpu.add(&debug_alvo);
  cpu.add(&Tlevanta_alvo);
  
}

void mira_alvo(int caso, char anterior){               //FUNÇÃO QUE COLOCA A ARMA NA DIREÇÃO DO ALVO DESEJADO

  motor.setSpeed(460);
  switch(caso){
    
    case 1:                                                             //CASO QUEIRA IR PARA O ALVO 1
      switch(anterior){                                                 //ANALISA ONDE A ARMA ESTAVA ANTES, PARA DECIDIR SE ANDA P/ ESQUEDA OU DIREITA
        case SEGUNDO:                        
          motor.run(BACKWARD);
          delay(250);
          break;
          
        case TERCEIRO:
          motor.run(BACKWARD);
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
          motor.run(FORWARD);
          delay(250);
          break;
          
        case TERCEIRO:
          motor.run(BACKWARD);
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
          motor.run(FORWARD);
          delay(500);
          break;
          
        case SEGUNDO:
          motor.run(FORWARD);
          delay(250);
          break;
          
        default:
          break;       
      }
      ANTIGO = TERCEIRO;
      break;

    default:
      
      if ((distancia == 5 || distancia == 6) && num_alvo == 0){   //ACERTOU O PRIMEIRO ALVO
        flag_alvo0 = 1;
          
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }
  
      if ((distancia == 16 || distancia == 17) && num_alvo == 1){
        flag_alvo1 = 1;
        
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }
  
      if ((distancia == 28 || distancia == 29) && num_alvo == 2){
        flag_alvo2 = 1;
          
        if (time_interval >= 600){
          time_interval = time_interval - 200;
          Tlevanta_alvo.setInterval(time_interval);
        }
      }

      if ((distancia == 5 || distancia == 6) && num_alvo != 0){
        errou--;
        digitalWrite(buzzer, HIGH)
        delay(500);
        digitalWrite(buzzer, LOW)
      }

       if ((distancia == 16 || distancia == 17) && num_alvo != 1){
        errou--;
        digitalWrite(buzzer, HIGH)
        delay(500);
        digitalWrite(buzzer, LOW)
      }
  
      if ((distancia == 28 || distancia == 29) && num_alvo != 2){
        errou--;
        digitalWrite(buzzer, HIGH)
        delay(500);
        digitalWrite(buzzer, LOW)
      }
  
      if (flag_alvo0 == 0 && flag_alvo1 == 0 && num_alvo == 0){
        errou--;
        digitalWrite(buzzer, HIGH)
        delay(500);
        digitalWrite(buzzer, LOW)
      }
      tempo = 0;
      break;    
  }
}

void loop() {
  
  inicia = digitalRead(botao);
  
  while(inicia == HIGH) { //SE LIGAREM O BOTÃO O PROGRAMA INICIA

    cpu.run(); //RODA TODAS AS THREADS

    if (Serial.available()) {
      char movimento = Serial.read(); //RECEBE DO OPENCV A INFORMAÇÃO DO QUADRANTE

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
        placjogo.salvar(cont_alvo);  //ATUALIZA O PLACAR
        cont_alvo = 0;
        inicia = 0;
        mira_alvo(2, ANTIGO);       //COLOCA A ARMA NA POSIÇÃO INICIAL
      }
    }
    
    // PARA MOTOR QUANDO NÃO TIVER NADA ACIONADO
    motor.setSpeed(0);
    motor.run(RELEASE);
  }
}
