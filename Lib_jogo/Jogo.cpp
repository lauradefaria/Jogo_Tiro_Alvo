#include "Jogo.h"

Jogo::Jogo(){
  num_alvo = 0;
  vel_alvo = 1500;
  num_erros = 0;
}

void Jogo::setup_servos(){
	//Set dos pinos para os servo motores
	alvo[0].attach(22);
	alvo[1].attach(23);
	alvo[2].attach(24);

	//Set da posição inicial dos servos
	alvo[0].write (90); // Posição superior do alvo1
	alvo[1].write (90); // Posição superior do alvo2
	alvo[2].write (90); // Posição superior do alvo3
}

// Função para gerar randomicamente
void Jogo::alvo_random(){
  num_alvo = random(0,3);
  alvo[num_alvo].write (180);
  delay (vel_alvo);
  alvo[num_alvo].write(90);
}

void Jogo::alvo_random1(){
  alvo[num_alvo].write(90);
  //num_alvo = random(0,3);
  set_num_alvo(random(0,3));
  alvo[num_alvo].write (180);
}

void Jogo::set_vel_alvo(int velocidade){
	vel_alvo = velocidade;
}

void Jogo::set_num_alvo(int alvo){
	num_alvo = alvo;
}

int Jogo::get_vel_alvo(){
	return vel_alvo;
}

int Jogo::get_num_alvo(){
	return num_alvo;
}

void Jogo::set_alvo(int num, int position){
	alvo[num].write(position);
}

int Jogo::get_alvo(){
	if (get_num_alvo() == 0){
		return 0;
	}else{
		return 2;
	}
}



/*
*	Como retornar o alvo ao mesmo tempo que o tiro o acerta.
*/
