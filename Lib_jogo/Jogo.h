#ifndef JOGO_H
#define JOGO_H

/* Permite fazer uso, ter acesso as funções do Arduíno e dos Servos. */
#include <Arduino.h>
#include <Servo.h>
/* Permite fazer uso, ter acesso as funções do shield USB p/ joystick de PS3 bluetooth. */

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

 /**
  * Class Jogo - Possue os atributos e métodos.
  */
class Jogo {
  int marca_alvo;
  int num_alvo;
  int num_erros;
  Servo alvo[3]; //Alvo 1 > esquerdo      //Alvo 2 > Meio      // Alvo 3 > Direito

public:
  Jogo();
  void setup_servos();
  void alvo_random();
  void alvo_random1();
  void alvo_random_millis(int);
  void reseta_game();

  /**
   * Métodos seters e geters necessários
  */
  void set_vel_alvo(int);
  void set_num_alvo(int);
  int get_vel_alvo();
  int get_num_alvo();

  void set_alvo(int, int);
  int get_alvo();
};

#endif
