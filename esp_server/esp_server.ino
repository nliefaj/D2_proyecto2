#include <WiFi.h>
#include <WiFiUdp.h>

// Datos del Access Point
const char* ssid = "ND5_Network";//definimos el nombre de la red
const char* password = "password123";

// Red fija
IPAddress local_IP(192,168,5,10);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);

WiFiUDP Udp;
const unsigned int localUdpPort = 4210;  // Puerto UDP donde escucha el master
char incomingPacket[255]; // buffer para datos entrantes

HardwareSerial MySerial(2); // UART2 para enviar al microcontrolador

void setup() {
  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1, 16, 17); // UART2 TX=17, RX=16
  
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Error en configurar IP AP");
  }
  WiFi.softAP(ssid, password);
  Serial.println("Access Point iniciado");
  Serial.println(WiFi.softAPIP());

  Udp.begin(localUdpPort);
  Serial.printf("Servidor UDP escuchando en el puerto %d\n", localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) {
      incomingPacket[len] = 0; // Null-terminar
    }
    Serial.printf("Paquete UDP recibido: %s\n", incomingPacket);

    // Enviar por UART al microcontrolador
    MySerial.print(incomingPacket);
  }
}
