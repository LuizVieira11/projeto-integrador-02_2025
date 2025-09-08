#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* SSID = "Gustav";
const char* PASSWORD = "1109*Mafe";

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

WebServer server(80);

void handleLigar(){
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", "LED aceso");
  Serial.println("Comando '/ligar' recebido");
}

void handleDesligar(){
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/plain", "LED apagado");
  Serial.println("Comando '/desligar' recebido");
}

void handleNotFound(){
  server.send(404, "text/plain", "Nao encontrado");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("Conectando Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("Para controlar o LED, use o seguinte IP: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("meu-esp")) {
    Serial.println("Erro ao configurar o mDNS!");
    return;
  }
  Serial.println("mDNS iniciado. Acesse via http://meu-esp.local");
  
  server.on("/ligar", handleLigar);
  server.on("/desligar", handleDesligar);

  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
