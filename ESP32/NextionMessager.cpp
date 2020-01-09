#include "NextionMessager.h"

NextionMessager Nextion(2);

NextionMessager::NextionMessager(int uart_nr) : HardwareSerial(uart_nr){
}

void NextionMessager::handle(){
	if(this && Serial && Serial.available() > 0){
		bool atualizando = true;
		unsigned long tempoInicial, tempoAtual;
		while(atualizando){
			if(Serial.available() > 0) write(Serial.read());
			if(available() > 0) Serial.write(read());
			tempoInicial = millis();
			while(Serial.available() == 0 && available() == 0){
				tempoAtual = millis();
				if(tempoAtual - tempoInicial > 3000) atualizando = false;
			}
		}
	}
}

void NextionMessager::WriteNextion(String Mensagem){
	if(this){
		print((Mensagem.endsWith("\n") ? Mensagem.substring(0, Mensagem.length() - 1) : Mensagem));
		write(0xFF);
		write(0xFF);
		write(0xFF);
	}
}

void NextionMessager::SetPagina(String nomePagina){
	WriteNextion("page " + nomePagina);
}

void NextionMessager::SetTexto(String id, String texto){
	WriteNextion(id + ".txt=\"" + texto + "\"");
}

void NextionMessager::SetTexto(String id, String texto, unsigned int cor){
	WriteNextion(id + ".txt=\"" + texto + "\"");
	WriteNextion(id + ".pco=" + String(cor));
}

void NextionMessager::SetValor(String id, int valor){
	WriteNextion(id + ".val=" + String(valor));
}

void NextionMessager::SetValor(String id, int valor, unsigned int cor ){
	WriteNextion(id + ".val=" + String(valor));
	WriteNextion(id + ".pco=" + String(cor));
}

void NextionMessager::SetImagem(String id, int idImagem){
	WriteNextion(id + ".pic=" + String(idImagem));
}

void NextionMessager::Reset(){
	WriteNextion("rest");
}
