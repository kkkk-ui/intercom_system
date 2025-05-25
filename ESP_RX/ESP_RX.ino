#include <esp_now.h>
#include <WiFi.h>
#include <stdio.h>

#define buzzerPin 25

typedef struct struct_message {
  int buttonPressed;
} struct_message;

struct_message inMsg = {0};

void onDataRecv(const esp_now_recv_info_t * info, const uint8_t * incomingData, int len){
  Serial.print("受信 from ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  memcpy(&inMsg, incomingData, sizeof(inMsg));
  Serial.println(inMsg.buttonPressed);

  if (inMsg.buttonPressed == 1) {
    Serial.println("Arduino GPIO ON");
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  WiFi.mode(WIFI_STA);  // STAモード（送信側と同じ）

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
