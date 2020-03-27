Sketch para comunicação de dispositivos IoT, através do protocolo de comunicação MQTT.
Código para conexão WiFi, conexão em múltiplos tópicos MQTT, e atualização OTA.

*OTA - Over The Air:
    Atualização é feita via WiFi, endereçada ao ip do micro controlador ESP na rede. 
    É feita utilizando o HostName que é setado no próprio Sketch, e pela senha (opicional).
    
*MQTT - Message Query Telemetry Transport:
    Protocolo M-2-M (Machine to Machine) de comunicação entre dispositivos. Onde utiliza-se o conceito de subscrição em tópicos e       publicação em tópicos.
    Protocolo mais leve do que o HTTP, contém menos cabeçalhos. 
    É indicado para micro controladores com baixo poder de processamento e memória.
    Pouco consumo de energia, e é idel para distribuição de informação para um ou mais dispositivos. [1]
    
[1] MQTT.org 
