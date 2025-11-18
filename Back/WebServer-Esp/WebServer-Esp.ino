#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

String itemCodigoBarras = "";

// Wi-fi
const char *SSID = "Gustav1";
const char *PASSWORD = "1109*Mafe";

// Configuração dos pinos LoRa
#define SS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 2
#define LORA_BAND 915E6

WebServer server(80);

void enviarLoRa(String mensagem)
{
  Serial.print("Enviando via LoRa: ");
  Serial.println(mensagem);

  LoRa.beginPacket();
  LoRa.print(mensagem);
  LoRa.endPacket();

  Serial.println("Pacote enviado");
}

void handleItemPost()
{
  // Verifica se o corpo da requisição (body) foi enviado
  if (!server.hasArg("plain"))
  {
    server.send(400, "application/json", "{\"error\":\"Body ausente\"}");
    return;
  }

  String body = server.arg("plain");
  Serial.println(body);

  DynamicJsonDocument doc(128);

  DeserializationError error = deserializeJson(doc, body);

  if (error)
  {
    Serial.print("Erro na interpretação do JSON:");
    Serial.println(error.c_str());
    server.send(400, "application/json", "{\"error\":\"JSON invalido\"}");
    return;
  }

  itemCodigoBarras = doc["codigo_barras"].as<String>();
  Serial.print("Código recebido: ");
  Serial.println(itemCodigoBarras);

  enviarLoRa(itemCodigoBarras);

  String response = "Item '" + itemCodigoBarras + "' enviado via LoRa";
  server.send(200, "text/plain", response);
}

void handleNotFound()
{
  server.send(404, "text/plain", "Rota não encontrada");
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("Conectando Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");

  if (!MDNS.begin("meu-esp"))
  {
    Serial.println("Erro ao configurar o mDNS!");
    return;
  }
  Serial.println("mDNS iniciado. Acesse via http://meu-esp.local");

  Serial.println("Inicializando LoRa...");
  SPI.begin(18, 19, 23, 5); // SCK, MISO, MOSI, SS
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(LORA_BAND))
  {
    Serial.println("Falha ao iniciar LoRa. Verifique conexões e frequência.");
    while (1)
      ;
  }
  Serial.println("LoRa iniciado com sucesso!");

  server.on("/item", HTTP_POST, handleItemPost);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  server.handleClient();
}
