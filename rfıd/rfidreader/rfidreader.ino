#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
int successRead;

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID KART KAYIT UYGULAMASI");
  Serial.println("--------------------------");
  Serial.println("Lutfen 1 numarali karti okutun");
  Serial.println();
  do {
    //okuma başarılı olana kadar getID fonksiyonunu çağır
    successRead = getID(); 
  }
  while (!successRead);
  for ( int i = 0; i < mfrc522.uid.size; i++ )
  {
    //kartın UID'sini EEPROM'a kaydet
    EEPROM.write(i, readCard[i] ); 
  }
  Serial.println("Kart EEPROM'a kaydedildi.");
  Serial.println();
  Serial.println("Lutfen 2 numarali karti okutun.");
  Serial.println();
  do {
    successRead = getID();
  }
  while (!successRead);
  for ( int i = 0; i < mfrc522.uid.size; i++ )
  {
    EEPROM.write(i + 4, readCard[i] );
  }
  Serial.println("Kart EEPROM'a kaydedildi.");
  Serial.println();
  Serial.println("Kart kayit islemi basarili!");
}

void loop()
{
}

int getID() {
  //yeni bir kart okunmadıysa 0 döndür
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  Serial.print("Kart UID'si: ");
  //kartın UID'sini byte byte oku ve seri monitöre yaz
  for (int i = 0; i < mfrc522.uid.size; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  //kart okumayı durdur ve 1 döndür (okuma başarılı)
  mfrc522.PICC_HaltA();
  return 1;
}
