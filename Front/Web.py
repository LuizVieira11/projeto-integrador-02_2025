import requests
import json
import time

# Carrega os projetos do JSON
with open('mock.json') as fp:
    data = json.load(fp)

projects = data["data"]

url = "http://meu-esp.local/item"

print('--- Pesquisa de Ativos ---')

while True:
    print('\nProjetos disponíveis:\n')
    for p in projects:
        print(f'{p["id"]}) {p["cor"]}')

    idProject = input('\nDigite o número correspondente cor que deseja encontrar (ou "sair" para encerrar): ')
    if idProject.lower() == "sair":
        print("Encerrando o programa...")
        break

    rfid_to_send = None
    for p in projects:
        if str(idProject) == str(p["id"]):
            rfid_to_send = p["RFIDCode"]
            print('\n')
            print('--------------------------------')
            print(f'RFID enviado: {rfid_to_send}')
            print('--------------------------------')
            print('\n')

    if rfid_to_send:
        payload = {"codigo_barras": rfid_to_send}
        try:
            response = requests.post(url, json=payload, timeout=5)
            if response.status_code == 200:
                print('RFID enviado com sucesso para o ESP!')
            else:
                print(f'Erro ao enviar RFID. Status code: {response.status_code}')
        except requests.exceptions.RequestException as e:
            print(f'Falha na comunicação com o ESP: {e}')
    else:
        print('ID de projeto não encontrado.')

    # Pequena pausa para não travar o terminal
    time.sleep(1)
