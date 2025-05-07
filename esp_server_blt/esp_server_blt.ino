#include "BluetoothSerial.h"
BluetoothSerial SerialBT1;
BluetoothSerial SerialBT2;
HardwareSerial MySerial(2); // UART2

// Reemplaza con las MAC de tus esclavos
const char* slave1MAC = "CC:DB:A7:33:0E:0E"; // ESP1Slave
//const char* slave2MAC = "YY:YY:YY:YY:YY:YY"; // ESP2Slave

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // UART2

  SerialBT1.begin("Master1", true); // true = cliente
  //SerialBT2.begin("Master2", true);

  Serial.println("Conectando a esclavos...");

  if (SerialBT1.connect(slave1MAC)) {
    Serial.println("Conectado a Slave 1");
  } else {
    Serial.println("Error al conectar a Slave 1");
  }

  /*if (SerialBT2.connect(slave2MAC)) {
    Serial.println("Conectado a Slave 2");
  } else {
    Serial.println("Error al conectar a Slave 2");
  }*/
}

void loop() {
  if (SerialBT1.connected() && SerialBT1.available()) {
    String data1 = SerialBT1.readStringUntil('\n');
    data1.trim();
    Serial.println("Slave1: " + data1);
    MySerial.println(data1); // Reenvía a microcontrolador
  }
/*
  if (SerialBT2.connected() && SerialBT2.available()) {
    String data2 = SerialBT2.readStringUntil('\n');
    data2.trim();
    Serial.println("Slave2: " + data2);
    MySerial.println(data2); // Reenvía a microcontrolador
  }*/
}

