#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#define wifi_ssid                 "SSID_WIFI"
#define wifi_password             "SENHA_WIFI"

#define mqtt_server               "URL DO MQTT ou IP"
#define mqtt_user                 "USUARIO DO MQTT"         
#define mqtt_password             "SENHA DO MQTT" 
#define servidor_mqtt_port        "PORTA DO MQTT"    

#define ESPHostname "ESP8266_NodeMcu" //Definindo HostName para o ESP

#define mqtt_topico_sub "" //Topico para subscrever no MQTT
#define pino  5   //pino de controle/resposta do topico

unsigned long delayTime;

WiFiClient espClient;
PubSubClient client(espClient);   //passando o objeto de conexão para a biblioteca PubSub
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {    
  Serial.begin(115200);   //iniciando serial com 115200 de baud
  pinMode(pino, OUTPUT);  //setando o pino como saída
  setup_wifi();   //função para inicializar parametros wifi
   ArduinoOTA.setHostname(ESPHostname);   //setando o hostname para atualização OTA
  // ArduinoOTA.setPassword("admin"); //Setar senha para conexão OTA (opcional)
  ArduinoOTA.begin();

  int portInt = atoi(servidor_mqtt_port); //convertendo de string para int
  client.setServer(mqtt_server, portInt); //setando servidor mqtt
  client.setCallback(callback); //setando callback/ resposta payload recebida no tópico
  while(!Serial);    
    delayTime = 1000;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();  //espécie de guia do ESP na rede para atualização OTA
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
  }
}

void setup_wifi() { //função para inicializar parametros wifi
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);  //inicializando wifi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Função para imprimir na porta serial
void imprimirSerial(bool linha, String mensagem){
    if(linha){
      Serial.println(mensagem);
    }else{
      Serial.print(mensagem);
    }
}

void callback(char* topico, byte* mensagem, unsigned int tamanho) {
//Convertendo a mensagem recebida para string
  String strMensagem = String((char*)mensagem);
  String topic = String(topico);
  
  imprimirSerial(false, "Mensagem recebida! Topico: ");
  imprimirSerial(false, topic);
  imprimirSerial(false, ". Mensagem: ");
  imprimirSerial(true, strMensagem);

  //Executando o comando solicitado
  imprimirSerial(false, "Status do pino antes de processar o comando: ");
  imprimirSerial(true, String(digitalRead(pino)).c_str());
  
  
  //para multiplos topicos, deve condicionar o payload com o topico
  //Ex
  //if(topic == "casa/quarto" && strMensagem == "on")
  
  if(topic == "casa/quarto/tv" && strMensagem == "on"){
    imprimirSerial(true, "Setando estado do pino para nível alto...");
    digitalWrite(pino, HIGH);
  }else if(topic == "casa/quarto/tv" && strMensagem == "off"){
    imprimirSerial(true, "Setando estado do pino para nível baixo...");
    digitalWrite(pino, LOW);
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão ao MQTT...");
    // Criando um cliente aleatório para o MQTT
    String clientId = "ESP8266-";
    clientId += String(random(0xffff), HEX);
    // Tentando conexão
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("Conectado");
      client.subscribe(mqtt_topico_sub, 1); //subscrevendo ao topico definido
    } else {
      Serial.print("Falha, COD DO ERRO=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos... ");
      // Espera 5 segundos para tentar novamente
      delay(5000);
    }
  }
}
