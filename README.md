# 🏭 Automação de Estoques – Pick by Light

Projeto desenvolvido como parte do **Projeto Integrador 2025.2** do curso de **Engenharia da Computação – Universidade de Sorocaba (UNISO)**.  
O sistema propõe a automação do processo de localização de itens em estoque utilizando **ESP32**, **RFID** e **LoRa (RFM95)**, integrando conceitos de **IoT** e **Indústria 4.0**.

---

## 🎯 Objetivo do Projeto

Automatizar o processo de **identificação e localização de itens em estoque**, reduzindo erros humanos e otimizando o tempo de busca por materiais, por meio da comunicação sem fio e da integração entre hardware e software.

O sistema é dividido em dois conjuntos principais:

1. **📦 Prateleira Inteligente** – Realiza a leitura de tags RFID dos itens armazenados e acende o LED correspondente ao item solicitado.  
2. **💻 Terminal do Usuário** – Interface de controle que permite solicitar materiais e se comunica via LoRa com a prateleira.

---

## ⚙️ Arquitetura do Sistema

### 🧠 Componentes Principais
- **ESP32 DevKit v1**
- **Módulo LoRa RFM95 (915 MHz)**
- **Módulo RFID MFRC522**
- **LEDs indicadores**
- **Fonte 3.3V / cabos jumper**

---

## 📈 Benefícios do Sistema

- 🔍 Localização rápida e precisa de itens em estoque.  
- ⚙️ Redução de erros humanos e aumento da eficiência operacional.  
- 🌐 Comunicação sem fio de longo alcance e baixo consumo.  
- 💡 Integração de tecnologias emergentes (IoT, LoRa, RFID).  
- 🏗️ Solução escalável e de baixo custo, aplicável em diferentes ambientes industriais.  

---

## 🧑‍💻 Equipe de Desenvolvimento
| Nome                             | Função    |
| -------------------------------- | --------- |
| Felipe Eduardo Amaral de Almeida | Hardware  |
| Julio Cesar Gonçalves Vieira     | Hardware  |
| Leonardo de Paula Cardoso        | Front-end |
| Lucas Amaral Ferreira            | Back-end  |
| Luiz Gustavo Santos Vieira       | Back-end  |

---

## 🧭 Orientação
**Prof. Henri Marcos Esgalha Castelli**  
Projeto Integrador – Engenharia da Computação  
Universidade de Sorocaba (UNISO)

---

Este projeto é de uso educacional, desenvolvido para fins acadêmicos na Universidade de Sorocaba.  
Sinta-se à vontade para reutilizar o código com os devidos créditos aos autores.
