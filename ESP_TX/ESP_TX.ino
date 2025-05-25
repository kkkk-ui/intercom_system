#include <esp_now.h>
#include <WiFi.h>
#include <stdio.h>

#define buttonPin 32

uint8_t receiverMAC[] = {0x24, 0x62, 0xab, 0xe2, 0x55, 0xc8};

typedef struct struct_message{
  int buttonPressed;
} struct_message;

struct_message msg = {0};

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // ← STAモード

  delay(100);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW){
    Serial.println("ボタンON\n");

    msg.buttonPressed = 1;
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&msg, sizeof(msg)); 
    if (result == ESP_OK) {
      Serial.println("送信成功");
    } else {
      Serial.println("送信失敗");
    }
    msg.buttonPressed = 0;

    delay(1000);
  }

}
