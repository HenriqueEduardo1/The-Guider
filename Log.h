#ifndef LOG_H
#define LOG_H

#include "cor.h"
#include "EEPROM2.h"
#include <SD.h>

#define MAX_TAMANHO_LOG 1

#define SEPARADOR_CSV ";"

struct Refletancia{
	float esq;
	float dir;
};

struct DadosLog{
	private:
	unsigned long microSegundos; 	//4 bytes
	double segundos;				//8 bytes

	public:
	Refletancia reflet; 	//8 bytes
	HSV	hsvDir;						//12 bytes
	HSV hsvEsq;						//12 bytes
	short estado;					//2 bytes
	float motorEsq;				//4 bytes
	float motorDir;				//4 bytes

	void print(){
		Serial.print(microSegundos); 
		Serial.print(SEPARADOR_CSV);
		Serial.print(motorEsq);
		Serial.print(SEPARADOR_CSV);
		Serial.print(motorDir);	
		Serial.print(SEPARADOR_CSV);
		Serial.print(reflet.esq);
		Serial.print(SEPARADOR_CSV);
 		Serial.print(reflet.dir);
		Serial.println("");
	}

	void salvarTempo(){ 
		microSegundos = micros();
		segundos = microSegundos/1000000.0;
	}
	unsigned long getMicroSegundos(){return microSegundos;}
	double getSegundos(){return segundos;}

};

class Log{
	private:	
	DadosLog dados[MAX_TAMANHO_LOG];
	unsigned int posicaoParaAdicionar;
	unsigned int tamanhoVetorDados;
	String saida;
	File file;
	int tempo_gravacao;

	#define ENDERECO_EEPROM_LOG 100

	public:
	Log();
	Log(const Log &l);
	bool configurar();
	
	void adicionarDados(const DadosLog &entrada);
	void printDados();

	void salvarLog();
	void lerLog();
//	void write();

};

#endif