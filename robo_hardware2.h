
//nesse arquivo as funcoes devem ser somente declaradas
#ifndef ROBO_HARDWARE_H
#define ROBO_HARDWARE_H


#include <Arduino.h>
#include <Servo.h>

#include "cor.h"
#include "pinagem.h"
#include "Ultrasonic.h"
#include "MotorPasso.h"

#include "EEPROM2.h"
#include "CorTcs23.h"
#include "CorTcs34.h"

#include "led_botoes/Botao.h"
#include "led_botoes/Led.h"

enum{
	TCS23,
	TCS34
};

struct calibracao_dados{
	HSV brancoDir;
	HSV brancoEsq;
	HSV pretoDir;
	HSV pretoEsq;
	HSV verdeDir;
	HSV verdeEsq;
	HSV cinzaDir;
	HSV cinzaEsq;

	int refletanciaDir;
	int refletanciaMaisDir;
	int refletanciaEsq;
	int refletanciaMaisEsq;
	int refletanciaMeio;

};

struct refletancia_dados{
	
	float valorLedLigado;
	float valorLedDesligado;
	float valorDiferenca;
};

class robo_hardware:private pinagem{
private:

	#define AJUSTE_PINO_SENSOR 36
	#define OFF 0
  #define AJUSTE_MOTOR 0.65
	#define MAX_10_BITS 1023.0				//Maior valor que um numero de 10 bits pode obter
	
	#define CALIBRACAO_SONAR  40.4	//Valor para calibrar os sonares. Quanto maior esse valor menor a inclinação da reta de calibracao

	#define ENDERECO_EEPROM 0

public: 


  robo_hardware();
  void configurar(bool habilitar_garra=true);
	void habilitaTCS34();
	void habilitaTCS23();

  //lerSensorFimDeCurso(By Minnael); HAHAHAH
	inline const bool fimdocurso1Pressionado()const{return fimdocurso1.estaPressionado();}
  inline const bool fimdocurso2Pressionado()const{return fimdocurso2.estaPressionado();}
  inline const bool fimdocurso3Pressionado()const{return fimdocurso3.estaPressionado();}
	
  inline void acionarMotoresPasso(int velEsq, int velDir){motoresDePasso.acionarMotorPasso(velEsq, velDir);}
	inline void acionarMotoresPassoEmGraus(int vel, double grau){motoresDePasso.acionarMotoresEmGraus(vel, grau);}
	inline void acionarPassoAngEsq(int ang, int vel){motoresDePasso.acionarAngEsq(ang, vel);}
	inline void acionarPassoAngDir(int ang, int vel){motoresDePasso.acionarAngDir(ang, vel);}

	//As funcoes retornam o valor lido do sensor refletancia
    const float lerSensorDeLinha(const int sensor, bool ledLigado=true); //recebe um pino analogico (A0, A1, A2 e etc) e retorna um valor de 0 a 100 
	inline const float lerSensorLinhaEsq(){			return lerSensorDeLinha(SENSOR_LINHA_ESQUERDO);} //retorna um valor de 0 a 100 
	inline const float lerSensorLinhaMaisEsq(){	return lerSensorDeLinha(SENSOR_LINHA_MAIS_ESQUERDO);} //retorna um valor de 0 a 100 
	inline const float lerSensorLinhaDir(){			return lerSensorDeLinha(SENSOR_LINHA_DIREITO);} //retorna um valor de 0 a 100
	inline const float lerSensorLinhaMaisDir(){	return lerSensorDeLinha(SENSOR_LINHA_MAIS_DIREITO);} //retorna um valor de 0 a 100
	
	inline const float lerSensorLinhaMeio(){	return lerSensorDeLinha(SENSOR_LINHA_MEIO);} //retorna um valor de 0 a 100 #MEIO


    inline const float lerSensorLinhaEsqSemRuido(){			return lerDadosSensorLinhaEsq().valorDiferenca;} //retorna um valor de 0 a 100  
	inline const float lerSensorLinhaMaisEsqSemRuido(){	return lerDadosSensorLinhaMaisEsq().valorDiferenca;} //retorna um valor de 0 a 100 
	inline const float lerSensorLinhaDirSemRuido(){			return lerDadosSensorLinhaDir().valorDiferenca;} //retorna um valor de 0 a 100
	inline const float lerSensorLinhaMaisDirSemRuido(){	return lerDadosSensorLinhaMaisDir().valorDiferenca;} //retorna um valor de 0 a 100
	
    inline const float lerSensorLinhaMeioSemRuido(){	return lerDadosSensorLinhaMeio().valorDiferenca;} //retorna um valor de 0 a 100 #MEIO

	const refletancia_dados lerDadosSensorDeLinha(const int sensor);
	
	inline const refletancia_dados lerDadosSensorLinhaEsq(){ return lerDadosSensorDeLinha(SENSOR_LINHA_ESQUERDO);}
	inline const refletancia_dados lerDadosSensorLinhaMaisEsq(){return lerDadosSensorDeLinha(SENSOR_LINHA_MAIS_ESQUERDO);}
	inline const refletancia_dados lerDadosSensorLinhaDir(){return lerDadosSensorDeLinha(SENSOR_LINHA_DIREITO);}
	inline const refletancia_dados lerDadosSensorLinhaMaisDir(){return lerDadosSensorDeLinha(SENSOR_LINHA_MAIS_DIREITO);}
	
	inline const refletancia_dados lerDadosSensorLinhaMeio(){return lerDadosSensorDeLinha(SENSOR_LINHA_MEIO);} //#MEIO



	//A funcao para acionar os motores de locomocao do robo
	//A funcao recebe um percentual de tensao do motor esquerdo e direito
	//A funcao so recebe valores que variem de 100 ate -100
  void acionarMotores(float percetualMotorEsquerdo, float percetualMotorDireito);

	//funcao para acionar os servomotores
  void acionarServoGarra1(int angInicial, int angFinal, int tempo);
  void acionarServoGarra2(int angInicial, int angFinal, int tempo);
  void acionarServoGarra1(int angFinal);
  void acionarServoGarra2(int angFinal);

	float lerSensorSonarFrontal();
	float lerSensorSonarEsq();
	float lerSensorSonarDir();  
  
	//funcoes para trabalhar com os sensores de Cor
  HSV getHSVEsquerdo(); //realiza a leitura do sensor de cor esquerdo e retorna uma estrutura HSV. Ver cor.h
  RGB getRGBEsquerdo(); //realiza a leitura do sensor de cor esquerdo e retorna uma estrutura RGB. Ver cor.h
  HSV getHSVDireito(); 	//realiza a leitura do sensor de cor direito e retorna uma estrutura HSV. Ver cor.h
  RGB getRGBDireito(); 	//realiza a leitura do sensor de cor direito e retorna uma estrutura RGB. Ver cor.h

	void salvarCalibracao(calibracao_dados cal);  
	void lerCalibracao(calibracao_dados &cal);

	void ligarLed(const int led)const;
	void desligarLed(const int led)const;
	void ligarTodosLeds()const;
	void desligarTodosLeds()const;

	inline const bool botao1Pressionado()const{return botao1.estaPressionado();}
	inline const bool botao2Pressionado()const{return botao2.estaPressionado();}
	inline const bool botao3Pressionado()const{return botao3.estaPressionado();}

private:

  static int tipoSensorCor;
  static Servo servoGarra1;
  static Servo servoGarra2;
  void tensao(float valor_por_cento,int pino);
  //SensorCor cor_direita(s2,s3, out);  
  
  //SensorCor corDireita;
  //SensorCor corEsquerda;

  CorTcs23 corDireita;
  CorTcs23 corEsquerda;

  CorTcs34 corDireita34;
  CorTcs34 corEsquerda34;


	Ultrasonic sonarFrontal;
	Ultrasonic sonarEsq;
	Ultrasonic sonarDir;
	
	Botao botao1, botao2, botao3, fimdocurso1, fimdocurso2, fimdocurso3;
	Led	led1, led2, led3;

  MotorPasso motoresDePasso;
};

static robo_hardware robo;

#endif
