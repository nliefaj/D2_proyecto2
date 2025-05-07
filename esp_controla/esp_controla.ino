#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ND5_Network";
const char* password = "password123";

// Red fija
IPAddress local_IP(192,168,5,11);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);

WiFiUDP Udp;
const char* masterIP = "192.168.5.10"; // IP del master
const unsigned int masterPort = 4210;  // Puerto UDP donde escucha el master

HardwareSerial MySerial(2); // UART2 para leer del Nano

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // UART2
  
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Error configurando IP fija");
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al AP");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (MySerial.available()) {
    String data = MySerial.readStringUntil('\n');
    data.trim();

    Serial.println("Enviando: " + data);

    // Enviar al master vía UDP
    Udp.beginPacket(masterIP, masterPort);
    Udp.print(data);
    Udp.endPacket();
  }
}

