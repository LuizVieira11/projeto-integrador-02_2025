#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 2
#define LORA_BAND 915E6

// Função de chamada dos leitores para validar as tags que estão em cada posição

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando receptor LoRa...");

  // Inicializa a interface SPI nos mesmos pina\\os usados pelo transmissor
  SPI.begin(18, 19, 23, 5); // SCK, MISO, MOSI, SS

  // Configura pinos do módulo LoRa
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  // Inicia o LoRa
  if (!LoRa.begin(LORA_BAND))
  {
    Serial.println("Falha ao iniciar LoRa! Verifique a fiação e a frequência.");
    while (1)
      ;
  }

  Serial.println("LoRa iniciado com sucesso!");
}

void loop()
{
  // Verifica se chegou algum pacote
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // Lê o conteúdo recebido
    String received = "";
    while (LoRa.available())
    {
      received += (char)LoRa.read();
    }

    // Mostra na tela
    Serial.println();
    Serial.println("Mensagem recebida via LoRa:");
    Serial.println(received);
    // Chama a função da lógica de prateleiras com a mensagem recebida como argumento
  }
}
