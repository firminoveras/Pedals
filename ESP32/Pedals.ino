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

void setup() {
    Serial.begin(115200);
    Nextion.begin(115200, SERIAL_8N1, 16, 17, false, SERIAL_TIMEOUT);
    Nextion.Reset();
}

void loop() {
    Nextion.handle();
    if (Nextion.available() > 0) {
        String entrada = Nextion.readStringUntil(':');
        if (entrada.equals("scan")) scanWifi();
    }
}

void scanWifi() {
    Nextion.SetPagina("WIFI");
    Nextion.SetTexto("WIFI.TITULO", "Buscando Redes...");
    int QuantidadeRedes = WiFi.scanNetworks();
    for (int i = 0; i < QuantidadeRedes; i++) {
        Nextion.SetTexto("WIFI.SSID" + String(i), WiFi.SSID(i));
        Nextion.SetImagem("WIFI.p" + String(7 + i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? 10: 9);
        int RSSI= 6;   
        if(WiFi.RSSI(i)>-90) RSSI = 5;         
        if(WiFi.RSSI(i)>-70) RSSI = 4; 
        if(WiFi.RSSI(i)>-50) RSSI = 3;
        Nextion.SetImagem("WIFI.p" + String(i),RSSI);
    }
    Nextion.SetTexto("WIFI.TITULO", String(QuantidadeRedes)+" redes encontradas.");
}

void inicializeOTA() {
    ArduinoOTA.onStart([]() {});
    ArduinoOTA.onProgress([](unsigned int progresso, unsigned int total) {});
    ArduinoOTA.onError([](ota_error_t error) {
        String erro = "";
        switch (error) {
            case OTA_AUTH_ERROR:
                erro = "Licenca Invalida";
                break;
            case OTA_BEGIN_ERROR:
                erro = "Erro de Inicializacao";
                break;
            case OTA_CONNECT_ERROR:
                erro = "Erro de Conexao";
                break;
            case OTA_RECEIVE_ERROR:
                erro = "Erro de Recebimento";
                break;
            case OTA_END_ERROR:
                erro = "Erro na Finalizacao";
                break;
            default:
                erro = "Erro Desconhecido";
        }
    });
    ArduinoOTA.onEnd([]() {});
    ArduinoOTA.begin();
}