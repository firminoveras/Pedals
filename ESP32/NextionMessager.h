
#ifndef _NEXTIONMESSAGER_h
#define _NEXTIONMESSAGER_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
//#include "WProgram.h"
#endif

#define BRANCO			65535
#define CIANO			1496
#define CINZA_CLARO		16904
#define CINZA_ESCURO	6339
#define VERMELHO		36864

#define SERIAL_TIMEOUT 50

class NextionMessager : public HardwareSerial{
public:
	NextionMessager(int uart_nr);
	void handle();
	void WriteNextion(String mensagem);
	void SetPagina(String nomePagina);
	void SetTexto(String id, String texto);
	void SetTexto(String id, String texto, unsigned int cor);
	void SetValor(String id, int valor);
	void SetValor(String id, int valor, unsigned int cor);
	void SetImagem(String id, int idImagem);
	void Reset();
};
extern NextionMessager Nextion;
#endif

