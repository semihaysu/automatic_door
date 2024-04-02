#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

Servo servoMotor;
int RST_PIN = 9;
int SS_PIN = 10;
int motor = 8;
int led=2;

MFRC522 rfid(SS_PIN, RST_PIN);

// Her kartın UID'sini ve isimlerini içeren bir dizi
byte authorizedIDs[][4] = {
  {3, 15, 87 ,168}, // Kart 1
  {35 ,245, 114 ,51},
  {115, 101,223 ,165},
  {139, 137, 116, 70},
  {123, 51, 175, 70 } // Kart 2 (örnek)
  // Diğer kartları burada tanımlayın
};

char* authorizedNames[] = {
  "Kart 1",
  "Kart 2",
  "Kart 3",
   "Kart 4",
   "Kart 5" // (örnek)
  // Diğer isimleri burada tanımlayın
};

void setup() {
  pinMode(motor, OUTPUT);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  servoMotor.attach(6); // Servo motorunun kontrol pini olarak 6. pini kullanıyoruz.
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  bool authorized = false;

  for (int i = 0; i < sizeof(authorizedIDs) / 4; i++) {
    if (compareIDs(rfid.uid.uidByte, authorizedIDs[i])) {
      authorized = true;
      Serial.print("Kapi acildi. Kart ismi: ");
      Serial.println(authorizedNames[i]);
      ekranaYazdir();
      digitalWrite(led, HIGH);
      
      
   
     //  digitalWrite(motor, HIGH);
    //  delay(1000);
    //  digitalWrite(motor, LOW);
    //  delay(1000);
       
      servoMotor.write(180);  // Servo motoru 180 derece konumuna getiriyoruz.
      delay(500);           // 1 saniye bekliyoruz.
      servoMotor.write(0);   // Servo motoru 0 derece konumuna getiriyoruz.
      delay(500);
      digitalWrite(led, LOW);
      break;
    }
  }

  if (!authorized) {
    Serial.println("Yetkisiz Kart");
    ekranaYazdir();
  }

  rfid.PICC_HaltA();
}

void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}

bool compareIDs(byte* id1, byte* id2) {
  for (int i = 0; i < 4; i++) {
    if (id1[i] != id2[i]) {
      return false;
    }
  }
  return true;
}
