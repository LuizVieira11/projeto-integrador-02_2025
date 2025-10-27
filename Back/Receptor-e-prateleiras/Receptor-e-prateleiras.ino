#include <SPI.h>
#include <MFRC522.h>
#include <LoRa.h>

// --- Configuração dos Leitores ---
#define NUM_LEITORES 2 // 2 leitores

// Pinos SPI Compartilhados
#define SCK_PIN  12
#define MISO_PIN 19
#define MOSI_PIN 23

// Pinos SS (Chip Select) - ÚNICOS
#define SS_1_PIN  5  // Leitor 1
#define SS_2_PIN  17 // Leitor 2

// Pinos de Reset - ÚNICOS
#define RST_1_PIN 22 // Leitor 1
#define RST_2_PIN 21 // Leitor 2

// Pinos dos LEDs (um para cada leitor)
#define LED_1_PIN 25 // LED para o Leitor 1
#define LED_2_PIN 26 // LED para o Leitor 2 (D26)

// Pinos do LoRa
#define LORA_SS_PIN 13
#define LORA_DIO0_PIN 16
#define LORA_RST_PIN 14
#define LORA_BAND 915E6

// Cria os objetos para cada leitor
MFRC522 leitor1(SS_1_PIN, RST_1_PIN);
MFRC522 leitor2(SS_2_PIN, RST_2_PIN);

// Cria um array de ponteiros para facilitar o loop
MFRC522* leitores[NUM_LEITORES] = {
  &leitor1,
  &leitor2
};

// --- Variáveis Globais ---
byte uidParaBuscar[4];
byte uidTamanho = 4;
bool buscando = false;

/**
 * @brief Converte uma String (ex: "26 32 C8 01") em um array de bytes.
 */
bool parseUidString(String str) {
  str.trim();
  str.replace(" ", "");
  str.toUpperCase();

  if (str.length() != uidTamanho * 2) {
    Serial.print("Erro: O UID deve ter ");
    Serial.print(uidTamanho);
    Serial.println(" bytes (ex: '26 32 C8 01').");
    return false;
  }
  for (byte i = 0; i < uidTamanho; i++) {
    String byteHex = str.substring(i * 2, (i * 2) + 2);
    uidParaBuscar[i] = (byte) strtol(byteHex.c_str(), NULL, 16);
  }
  return true;
}

/**
 * @brief Compara o UID lido com o UID alvo (uidParaBuscar)
 */
bool comparaUID(byte* uidLido, byte tamanhoLido) {
  if (tamanhoLido != uidTamanho) {
    return false;
  }
  for (byte i = 0; i < uidTamanho; i++) {
    if (uidLido[i] != uidParaBuscar[i]) {
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\n=======================================");
  Serial.println("     Localizador de Estoque RFID + LoRa     ");
  Serial.println("=======================================");

  // Configura LEDs
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  digitalWrite(LED_1_PIN, LOW);
  digitalWrite(LED_2_PIN, LOW);

  // Inicializa SPI
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

  // Inicializa os leitores
  for (int i = 0; i < NUM_LEITORES; i++) {
    leitores[i]->PCD_Init();
    delay(10);
    byte v = leitores[i]->PCD_ReadRegister(MFRC522::VersionReg);
    if (v == 0x00 || v == 0xFF) {
      Serial.print("⚠  Leitor "); Serial.print(i + 1); Serial.println(" não detectado!");
    } else {
      Serial.print("✅ Leitor "); Serial.print(i + 1); Serial.print(" detectado. Versão: 0x");
      Serial.println(v, HEX);
    }
  }

  // Inicializa o LoRa
  Serial.println("\nInicializando LoRa...");
  SPI.begin(12, 19, 23, 32); // SCK, MISO, MOSI, SS
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("❌ Falha ao iniciar LoRa! Verifique conexões e frequência.");
    while (1);
  }
  Serial.println("✅ LoRa iniciado com sucesso!");

  Serial.println("\nAguardando mensagens via LoRa...");
}

/**
 * @brief Faz uma varredura COMPLETA em todos os leitores.
 */
void executarVarredura() {
  bool encontrado = false;
  digitalWrite(LED_1_PIN, LOW);
  digitalWrite(LED_2_PIN, LOW);

  Serial.println("Iniciando varredura...");

  for (int i = 0; i < NUM_LEITORES; i++) {
    leitores[i]->PCD_Init();
    delay(5);

    if (leitores[i]->PICC_IsNewCardPresent() && leitores[i]->PICC_ReadCardSerial()) {
      Serial.print(" -> Leitor "); Serial.print(i + 1); Serial.print(" detectou a tag: ");
      for (byte j = 0; j < leitores[i]->uid.size; j++) {
        Serial.print(leitores[i]->uid.uidByte[j] < 0x10 ? "0" : "");
        Serial.print(leitores[i]->uid.uidByte[j], HEX);
        if (j < leitores[i]->uid.size - 1) Serial.print(" ");
      }

      if (comparaUID(leitores[i]->uid.uidByte, leitores[i]->uid.size)) {
        Serial.println(" (✅ ALVO ENCONTRADO!)");
        encontrado = true;

        if (i == 0) {
          digitalWrite(LED_1_PIN, HIGH);
          delay(4000);
          digitalWrite(LED_1_PIN, LOW);
          break;
        } else if (i == 1) {
          digitalWrite(LED_2_PIN, HIGH);
          delay(4000);
          digitalWrite(LED_2_PIN, LOW);
          break;
        }

      } else {
        Serial.println(" (Tag errada)");
      }

      leitores[i]->PICC_HaltA();
      leitores[i]->PCD_StopCrypto1();
    } else {
      Serial.print(" -> Leitor "); Serial.print(i + 1); Serial.println(" está vazio.");
    }
  }

  Serial.println("-------------------------------");
  if (encontrado) {
    Serial.println("✅ SUCESSO! Tag localizada.");
  } else {
    Serial.println("❌ FALHA! Tag não encontrada.");
  }

  buscando = false;
}

/**
 * @brief Loop principal — agora recebe o UID via LoRa em vez de Serial.
 */
void loop() {
  // Verifica se há pacote LoRa recebido
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String msg = "";
    while (LoRa.available()) {
      msg += (char)LoRa.read();
    }

    msg.trim();
    Serial.print("\n📡 Mensagem LoRa recebida: ");
    Serial.println(msg);

    // Processa a string recebida como UID
    if (parseUidString(msg)) {
      Serial.print("OK. Buscando por: ");
      for (byte i = 0; i < uidTamanho; i++) {
        Serial.print(uidParaBuscar[i] < 0x10 ? "0" : "");
        Serial.print(uidParaBuscar[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      buscando = true;
    } else {
      Serial.println("❌ Erro ao interpretar UID recebido via LoRa.");
    }
  }

  // Continua a lógica original
  if (buscando) {
    executarVarredura();
  }

  delay(50);
}
