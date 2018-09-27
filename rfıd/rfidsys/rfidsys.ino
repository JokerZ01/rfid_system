//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>

//LiquidCrystal_I2C lcd (0x3F, 16, 2);

#define RST_PIN 9
#define SS_PIN 10
#define buzzerpin 6
#define yledpin 5
#define role 4
#define ledPin 3

Servo sg90;


MFRC522 mfrc522(SS_PIN, RST_PIN);



String lastRfid = "";
String kart1 = "";
String kart2 = "";

MFRC522::MIFARE_Key key;


void setup()
{
  //lcd.home();
  //lcd.print("Dogukan");
  //lcd.setCursor(7,1);
  //lcd.print("Yuvanc");
  //lcd.begin();
  //lcd.backlight();
  Serial.begin(9600);
  SPI.begin();
  //lcd.setCursor(3,0);

  mfrc522.PCD_Init();
  pinMode(buzzerpin, OUTPUT);
  pinMode(yledpin, OUTPUT);
  pinMode(role, OUTPUT);
  pinMode(ledPin, OUTPUT);
  sg90.attach(7);
  Serial.println("Sistem Gecis V1.0");
  Serial.println("--------------------------");
  Serial.println();
  //EEPROM'dan kart bilgisini oku
  readEEPROM();
}

void loop()
{
  //yeni kart okununmadıkça devam etme
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //kartın UID'sini oku, rfid isimli string'e kaydet
  String rfid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    rfid += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    rfid += String(mfrc522.uid.uidByte[i], HEX);
  }
  //string'in boyutunu ayarla ve tamamını büyük harfe çevir
  rfid.trim();
  rfid.toUpperCase();
  
  if (rfid == lastRfid)
    return;
  lastRfid = rfid;

  Serial.print("Okunan Kart ID'si: ");
  Serial.println(rfid);
  Serial.print("Gecis Izni Verilen Kart ID'si: ");
  Serial.println(kart2);
  Serial.println();
  digitalWrite(ledPin,HIGH);
  //1 nolu kart okunduysa LED'i yak, 2 nolu kart okunduysa LED'i söndür
  if (rfid == kart1)
  
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(yledpin, HIGH);
    digitalWrite(role, LOW);
    tone(buzzerpin, 349);
    delay(500);
    noTone(buzzerpin);
    tone(buzzerpin, 349);
    delay(500);
    noTone(buzzerpin);
    //.home();
    //lcd.setCursor(0,0);
    //lcd.print("Gecerli Kart...");
    Serial.println("Dogrulanmis kart gecis verildi");
    Serial.println("Sistem 1 saniye sonra kitlenecektir ...");
    //lcd.setCursor(7,3);
    //lcd.print("Giris Yapildi.");
    delay(5000);
    sg90.write(360);
    digitalWrite(yledpin, LOW);
    Serial.println("Sistem kitlendi.");
  }
  else
  {
    //lcd.clear();
    //lcd.setCursor(0,0);
    //cd.print("Hatali Kart ...");
    //lcd.setCursor(7,3);
    //lcd.print("Sistem Kapatiliyor...");
    Serial.println("Hatali Kart !!");
    Serial.println("Lutfen Tekrar Deneyiniz !!");
    tone(buzzerpin, 349);
    delay(500);
    noTone(buzzerpin);
    Serial.println("Sistem deaktif oldu.");
    digitalWrite(role, HIGH);
    sg90.write(-10);
    delay(2000);
    
   }
  Serial.println();
  delay(200);

}

void readEEPROM()
{
  //EEPROM'dan ilk kartın UID'sini oku (ilk 4 byte)
  for (int i = 0 ; i < 4 ; i++)
  {
    kart1 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    kart1 += String(EEPROM.read(i), HEX);
  }
  //EEPROM'dan ikinci kartın UID'sini oku
  for (int i = 4 ; i < 8 ; i++)
  {
    kart2 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    kart2 += String(EEPROM.read(i), HEX);
  }
  //Okunan stringleri düzenle
  kart1.trim();
  kart1.toUpperCase();
  kart2.trim();
  kart2.toUpperCase();
}
