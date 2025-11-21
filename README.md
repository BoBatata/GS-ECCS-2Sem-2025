# ConnecTech

Este projeto implementa um dispositivo IoT baseado em ESP32 que monitora o nível de estresse de um usuário e integra mecânicas de gamificação, comunicação MQTT e interação com um planner gameficado.
O sistema se conectaria com um Planner Gameficado monitorando a quantidade de pontos e estresse para sugerir pausas automáticas.

O hardware funciona totalmente no Wokwi, e a comunicação é feita com o HiveMQ Cloud.

# Objetivo do Projeto

Criar um dispositivo IoT capaz de:

<ul>
    <li>Medir níveis de estresse</li>
    <li>Exibir mensagens no LCD 16x2 I2C</li>
    <li>Enviar telemetria via MQTT/TLS</li>
    <li>Receber comandos remotos do Planner</li>
    <li>Alertar o usuário quando o stress estiver muito alto</li>
    <li>Mostrar feedback visual (LED) e sonoro (buzzer)</li>
</ul>

O dispositivo simula um Sistema de Gestão Gamificado que monitora a saúde do usuário e responde a eventos do "planner gamificado".

# Componentes Utilizados

| Componentes                   | Função                           |
| ----------------------------- | -------------------------------- |
| ESP32 DevKit V1               | Microcontrolador Wi-Fi com TLS   |
| LCD I2C 20x4                  | Exibição de stress, alertas e XP |
| Potenciômetro (Simula stress) | Mede o nível de stress (0–100)   |
| LED                           | Indica alerta de stress          |
| Buzzer                        | Emite beep em stress elevado     |

# Como rodar o projeto

<ol>
    <li>Abra o link do projeto</li>
    <li>Rode o projeto</li>
    <li>Insira o host/senha/url do HiveMQ Cloud e Wifi</li>
    <li>Veja o LCD mostrando as leituras</li>
    <li>Use as mensagens para mandar comandos ao dispositivo</li>
    <li>Altere no potenciômetro o stress</li>
    <li>Veja o LCD mostrar as leituras</li>
</ol>

# Dependências

<ul>
    <li>Wifi.h</li>
    <li>WiFiClientSecure.h</li>
    <li>PubSubClient.h</li>
    <li>LiquidCrystal_I2C.h</li>
</ul>

# Tópicos MQTT Utilizados

| Tópico                             | Descrição                   |
| ---------------------------------- | --------------------------- | 
| `/planner/stress_monitor/attrs/s`  | Stress (%)                  | 
| `/planner/stress_monitor/attrs/b`  | Batimento cardíaco simulado | 
| `/planner/stress_monitor/attrs/t`  | Temperatura simulada        | 
| `/planner/stress_monitor/attrs`    | Mensagens gerais            |
| `/planner/stress_monitor/attrs/xp` | Retorno após comando XP     |

| Tópico                        | Payload    |
| ----------------------------- | ---------- | 
| `/planner/stress_monitor/cmd` | `"pause"`  | 
|                               | `"resume"` | 
|                               | `"xp:NUM"` |

# Link do Wokwi (Projeto Completamente Funcionando)

<a href="https://wokwi.com/projects/447556004284048385">Medidor de Estresse</a>

## Integrantes

| Nome | RM |
|------|-----------|
| **Isabela de Deus Ferreira**       | 565988 |
| **Thomaz Vasconcelos Mendes**       | 564805 |
| **Nicolas Tetsuo Kimura**           | 565377 |
