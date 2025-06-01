#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <esp_now.h>
#include <stdio.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465
#define sender_name ""
#define sender_email ""
#define sender_password ""
#define Recipient_email ""
#define Recipient_name ""

#define buzzerPin 25

volatile bool flag_sendMail = false;

typedef struct struct_message {
  int buttonPressed;
} struct_message;

struct_message inMsg = {0};

SMTPSession smtp;

void esp_now_awake(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("ESP-NOW Ready");
  return;
}

void sendMail() {
  smtp.debug(1); 

  ESP_Mail_Session session;
  session.server.host_name = SMTP_server ;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = sender_name;
  message.sender.email = sender_email;
  message.subject = "インターホンが押されました";
  message.addRecipient(Recipient_name, Recipient_email);

  String textMsg = "お客様がいらっしゃいました。";
  message.text.content = textMsg.c_str();
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session)) {
    Serial.println("Failed to connect SMTP server");
    return;
  }
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
  else
    Serial.println("Email sent successfully");
}

void onDataRecv(const esp_now_recv_info_t * info, const uint8_t * incomingData, int len) {
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
    flag_sendMail = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  esp_now_awake();
}

void loop() {
  if (flag_sendMail) {
    flag_sendMail = false;  // フラグリセット
    
    // ESP-NOW停止 & Wi-Fi接続 & メール送信 & ESP-NOW再開
    esp_now_deinit();
    WiFi.disconnect(true);  
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected. Sending email...");
    sendMail();
    WiFi.disconnect(); 
    WiFi.mode(WIFI_OFF);
    delay(100);
    esp_now_awake();
  } 
}
