#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_XANH 3
#define LED_DO   4
#define COI      5

MFRC522 mfrc522(SS_PIN, RST_PIN);

// UID hợp lệ
byte dsUID[][4] = {
  {0x43, 0xBE, 0x7A, 0x0C},
  {0x63, 0xB0, 0x53, 0x05},
  {0x0D, 0x47, 0x56, 0x05},
  {0xEE, 0x81, 0x56, 0x05},
  {0xA1, 0xDA, 0x54, 0x05},
  {0xB1, 0x24, 0x55, 0x05}
};

const int soThe = sizeof(dsUID) / sizeof(dsUID[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_XANH, OUTPUT);
  pinMode(LED_DO, OUTPUT);
  pinMode(COI, OUTPUT);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  byte* uid = mfrc522.uid.uidByte;
  byte uidSize = mfrc522.uid.size;

  bool hopLe = false;
  for (int i = 0; i < soThe; i++) {
    bool match = true;
    for (int j = 0; j < uidSize; j++) {
      if (uid[j] != dsUID[i][j]) {
        match = false;
        break;
      }
    }
    if (match) {
      hopLe = true;
      break;
    }
  }

  if (hopLe) {
    // Gửi UID về PC (chuỗi HEX)
    for (byte i = 0; i < uidSize; i++) {
      if (uid[i] < 0x10) Serial.print("0");  // In đủ 2 ký tự HEX
      Serial.print(uid[i], HEX);
    }
    Serial.println(); // Xuống dòng để PC dễ đọc

    // Phản hồi đèn còi
    digitalWrite(LED_XANH, HIGH);
    digitalWrite(COI, HIGH);
    delay(300);
    digitalWrite(COI, LOW);
    digitalWrite(LED_XANH, LOW);
  } else {
    // Thẻ không hợp lệ
    digitalWrite(LED_DO, HIGH);
    for (int i = 0; i < 3; i++) {
      digitalWrite(COI, HIGH);
      delay(100);
      digitalWrite(COI, LOW);
      delay(100);
    }
    digitalWrite(LED_DO, LOW);
  }

  mfrc522.PICC_HaltA();
  delay(1000);
}
