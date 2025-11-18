# NomeDoProjeto

Este projeto implementa um dispositivo IoT baseado em ESP32 que monitora o nível de estresse de um usuário e integra mecânicas de gamificação, comunicação MQTT e interação com um planner gameficado.
O sistema se conectaria com um Planner Gameficado monitorando a quantidade de pontos e estresse para sugerir pausas automáticas.

O hardware funciona totalmente no Wokwi, e a comunicação é feita com o HiveMQ Cloud.

# Link do Wokwi (Projeto Completamente Funcionando)

<a href="https://wokwi.com/projects/447556004284048385">Medidor de Estresse</a>

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

## Integrantes

| Nome | RM |
|------|-----------|
| **Isabela de Deus**       | xxxxxx |
| **Thomaz Vasconcelos Mendes**       | 564805 |
| **Nicolas Tetsuo Kimura**           | 565377 |

