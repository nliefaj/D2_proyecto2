// ESP32 SLAVE
#include "BluetoothSerial.h"
#include "esp_bt_device.h"
BluetoothSerial SerialBT;
HardwareSerial MySerial(2); // UART2 (RX2: 16, TX2: 17)

void setup() {
  Serial.begin(115200);
  MySerial.begin(9600, SERIAL_8N1, 16, 17);
  SerialBT.begin("ESP1Slave"); // Cambia a "ESP2Slave" en el otro slave

  Serial.println("ESP Slave listo");
  // Mostrar MAC para emparejar desde el master
  const uint8_t* mac = esp_bt_dev_get_address();
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("MAC Bluetooth: ");
  Serial.println(macStr);
}

void loop() {
  if (MySerial.available()) {
    String msg = MySerial.readStringUntil('\n');
    msg.trim();
    Serial.println("Enviado: " + msg);
    SerialBT.println(msg);
  }
}

