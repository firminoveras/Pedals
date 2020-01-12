#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ETH.h>
#include <SPI.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Wire.h>

#include "EEPROM_ESP32.h"
#include "NextionMessager.h"

#define WIFI_TIMEOUT 5000
#define PORT 3232

String SSID = "", PASS = "";
WiFiServer Server;

void setup() {
    Serial.begin(115200);
    Nextion.begin(115200, SERIAL_8N1, 16, 17, false, SERIAL_TIMEOUT);
    Nextion.Reset();
}

void loop() {
    Nextion.handle();
    if (Nextion.available() > 0) {
        String entrada = Nextion.readStringUntil(';');
        if (entrada.equals("scan")) scanWIFI();       
        if (entrada.equals("PW")) PASS = Nextion.readStringUntil(';');   
        if (entrada.equals("desconectar")) desconectarWIFI();             
        if (entrada.equals("conectar")) ConectarWIFI();   
        if (entrada.equals("SSID")) {
            SSID = Nextion.readStringUntil(';');
            if(SSID==Flash.getEEPROM_SSID()) Nextion.SetTexto("keybdA.show", Flash.getEEPROM_PASS());
            Nextion.SetPagina("keybdA");
        }     
    }
    if(WiFi.isConnected()){
        ArduinoOTA.handle();
    }
}

void scanWIFI() {    
    for (int i = 0; i < 6; i++) {
        Nextion.SetTexto("WIFI.SSID" + String(i), "");
        Nextion.SetImagem("WIFI.p" + String(7 + i), 8);
        Nextion.SetImagem("WIFI.p" + String(i),7);
    }
    Nextion.SetTexto("WIFI.TITULO", "Buscando Redes...");
    Nextion.SetPagina("WIFI");
    int QuantidadeRedes = WiFi.scanNetworks();
    for (int i = 0; i < QuantidadeRedes; i++) {
        Nextion.SetTexto("WIFI.SSID" + String(i), WiFi.SSID(i));
        Nextion.SetImagem("WIFI.p" + String(7 + i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? 10: (WiFi.SSID(i) == Flash.getEEPROM_SSID() ? 12:9));
        int RSSI= 6;   
        if(WiFi.RSSI(i)>-90) RSSI = 5;         
        if(WiFi.RSSI(i)>-70) RSSI = 4; 
        if(WiFi.RSSI(i)>-50) RSSI = 3;
        Nextion.SetImagem("WIFI.p" + String(i),RSSI);
    }
    Nextion.SetTexto("WIFI.TITULO", String(QuantidadeRedes)+" rede"+(QuantidadeRedes!=1?"s":"")+" encontrada"+(QuantidadeRedes!=1?"s":"")+".");
}

void ConectarWIFI(){
    Nextion.SetPagina("CONNECT");
    WiFi.begin(SSID.c_str(), PASS.c_str());
    Nextion.SetTexto("t1", "> SSID: "+SSID);
    int timeOut = millis();
    String dots = "";
    while((millis()-timeOut)<WIFI_TIMEOUT){
        if(WiFi.isConnected()) break;
        if(dots.length()<5)dots+="."; else dots="";
        Nextion.SetTexto("t2", "> Estabelecendo Conexao"+dots);
        delay(200);
    }
    Nextion.SetTexto("t2", "> Estabelecendo Conexao");
    delay(2000);
    Nextion.SetTexto("t3", "> Tentativa de Conexao Concluida");  
    if(WiFi.status()==WL_CONNECTED){
        Nextion.SetTexto("t4", "> Status: Conectado", VERDE);
        Nextion.SetTexto("t5", "> Endereco IP: "+WiFi.localIP().toString());   
        ArduinoOTA.onStart([]() {

        });
        ArduinoOTA.onProgress([](unsigned int progresso, unsigned int total) {

        });
        ArduinoOTA.onError([](ota_error_t error) {
            String erro = "";
            switch (error) {
                case OTA_AUTH_ERROR: erro = "Licenca Invalida"; break;
                case OTA_BEGIN_ERROR: erro = "Erro de Inicializacao"; break;            
                case OTA_CONNECT_ERROR: erro = "Erro de Conexao"; break;            
                case OTA_RECEIVE_ERROR: erro = "Erro de Recebimento"; break;
                case OTA_END_ERROR: erro = "Erro na Finalizacao"; break;
                default: erro = "Erro Desconhecido";
            }
        });
        ArduinoOTA.onEnd([]() {
            
        });
        ArduinoOTA.begin();
        Nextion.SetTexto("t6",  "> Configurando OTA: OK");
        Server.begin(PORT);
        Flash.setEEPROM_SSID(SSID);
        Flash.setEEPROM_PASS(PASS);
        Nextion.SetTexto("t7", "> Configurando Servidore Salvando Rede: OK");        
    }else{
        Nextion.SetTexto("t4", "> Status: Erro de Conexao", VERMELHO);
        Nextion.SetTexto("t5", "");  
    }
  
    Nextion.WriteNextion("CONNECT.t0.dir="+String(WiFi.isConnected()? 1:0));
}

void desconectarWIFI(){
    if(WiFi.isConnected()){
        Nextion.SetPagina("CONNECT");
        Nextion.WriteNextion("CONNECT.t0.dir=0");
        Nextion.SetTexto("t1", "> Desconectando de: "+SSID);
        WiFi.disconnect();
        String dots = "";
        while(WiFi.isConnected()){
            if(WiFi.isConnected()) break;
            if(dots.length()<5)dots+="."; else dots="";
            Nextion.SetTexto("t2", "> Encerrando a conexao"+dots);
            delay(200);
        }
        Nextion.SetTexto("t2", "> Encerrando a conexao");
        Nextion.SetTexto("t3", "> Desconectado", VERMELHO);
    }else Nextion.SetPagina("MENU");
}