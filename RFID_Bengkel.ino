#include <RFID.h>

//#include <LiquidCrystal_I2C.h>

//#include <LiquidCrystal.h>

/*
KONFIGURASI PIN RFID KE NODEMCU :

RST   ->  D3
SDA   ->  D4
SCL   ->  D5                          
MOSI  ->  D7                         
MISO  ->  D6                         

KONFIGURASI PIN LCD I2C KE NODEMCU :
SDA   ->  D2
SCL   ->  D1
VCC   ->  VV
GND   ->  GND

KONFIGURASI PIN RELAY KE NODEMCU :
VCC   ->  3V
GND   ->  GND
IN    ->  D0

KONFIGURASI PIN ACTIVE BUZZER KE NODEMCU :
+     ->  D0
-     ->  GND

catatan :
Pin buzzer diparalel dengan D0

*/


#include <Wire.h>                   // memasukkan library wire.h untuk pengaturan LCDI2C
#include <LiquidCrystal_I2C.h>      // memasukkan library LiquidCrystal_I2C.h

#include <RFID.h>                   // memasukkan library RFID.h
#include <SPI.h>                    // memasukkan library SPI.h
#define CAYENNE_PRINT Serial        // mendefinisikan komunikasi serial ke CAYENNE
#include <CayenneMQTTESP8266.h>     // memasukkan library CayenneMQTTESP8266.h
#define RELAY 16                    // mendefinisikan pin Relay di NodeMcu
#define SS_PIN D4                   // mendefinisikan pin RFID SDA
#define RST_PIN D3                  // mendefinisikan pin RFID RST


// WiFi network info.
// Memasukkan Nama Wifi yang digunakan beserta password wifi yang digunakan
char ssid[] = "Sina-IoT1";          // memasukkan nama wifi yang digunakan
char wifiPassword[] = "sina615i";   // memasukkan password wifi yang digunakan

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
// Memasukkan data username, password, dan clientID dari dashboard Cayenne yang dibuat
char username[] = "d72c5a30-7718-11ea-883c-638d8ce4c23d";
char password[] = "7c9ded1f039c1ce77903a10e94ced3717e87088b";
char clientID[] = "e7d3eed0-7942-11ea-b767-3f1a8f1211ba";

LiquidCrystal_I2C lcd(0x27,16,2);   // mendeklarasikan alamat LCD I2C
RFID rfid(SS_PIN,RST_PIN);          // mendeklarasikan alamat pin SDA dan RST dari RFID

int serNum[5];                      // mendeklarasikan serial number kartu RFID dalam bentuk array berjumlah 5

// data serial number dari kartu RFID yang memiliki hak akses
int cards[][5] =
{
  {7,76,125,98,84},
  {135,166,50,78,93},
  {23,64,50,78,43},
  {39,250,20,78,135},
  {71,69,224,77,175},
  {153,55,239,141,204},
  {7,111,217,77,252},
  {41,220,173,142,214}
};

bool access = false;                

void setup()
{
  Serial.begin(9600);                                               // inisialisasi untuk komunikasi serial ke serial monitor di arduinoIDE
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);  // inisialisasi komuunikasi serial cayenne
  lcd.init();                                                       // inisialisasi LCD dengan I2C
  lcd.backlight();                                                   // inisialisasi backlight LCD
  Serial.begin(9600);                                               // inisialisasi untuk NodeMCU
  SPI.begin();                                                      // inisialisasi SPI
  rfid.init();                                                      // inisialisasi RFID reader
  pinMode(RELAY, OUTPUT);                                           // inisialisasi relay sebagai output
  digitalWrite(RELAY, LOW);                                         // inisialisai relay dalam logika LOW
}

void loop()
{  
  Cayenne.loop();                                                   // inisialisasi looping utuk cayenne

  lcd.setCursor(0,0);
  lcd.print(" DEPT.ENGINEER ");
  lcd.setCursor(0,1);
  lcd.print("<== SCAN HERE ");
  delay(1000);
  
  if(rfid.isCard())                                                 // kondisi jika ada kartu yang dideatkan ke RFID reader
  {      
    if(rfid.readCardSerial())                                       // kondisi pada saat kartu RFID dibaca oleh RFID reader
    {      
      // serial number RFID yang dibaca oleh RFID reader
      /*
         catatan : Jika ada kartu RFID yang belum memiliki hak akses,
                   kartu RFID tersebut bisa diketahui serial numbernya
                   dengan menscan kartu dan melihat serial numbernya di serial monitor pada arduino IDE.
                   Setelah itu serial number bisa ditambahkan pada script "int cards[][5] ="
      */
      
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println("");
      
      // kartu yang memiliki hak akses datanya akan dikirm ke cayennne 
      if(rfid.serNum[0]==135)
      {         
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NUR FATKAN ");
        Serial.println("NUR FATKAN");
        Cayenne.virtualWrite(1,"1");
      }
      else if(rfid.serNum[4]==84)
      {        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HUSEN");
        Serial.println("HUSEN");
        Cayenne.virtualWrite(1,"2");
      }
      else if (rfid.serNum[0]==23)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DWI NANANG ");
        Serial.println("DWI NANANG");
        Cayenne.virtualWrite(1,"3");
      }
      else if (rfid.serNum[0]==39)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MUZAKKI ");
        Serial.println("MUZAKKI");
        Cayenne.virtualWrite(1,"4");
      }
      else if (rfid.serNum[0]==71)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("JUNAIWAN ");
        Serial.println("JUNAIWAN");
        Cayenne.virtualWrite(1,"5");
      }
      else if (rfid.serNum[4]==204)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("YUSUF ");
        Serial.println("YUSUF");
        Cayenne.virtualWrite(1,"6");
      }
      else if (rfid.serNum[4]==252)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DEDI ");
        Serial.println("DEDI");
        Cayenne.virtualWrite(1,"7");
      }
      else if (rfid.serNum[0]==41)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SAGITA ");
        Serial.println("SAGITA");
        Cayenne.virtualWrite(1,"8");
      }
      /*
        Catatan : Tambahkan format script code di bawah ini jika ingin menambah anggota di atas fungsi else

        else if (rfid.serNum[array nomor serial yang dipilih]==nilai dari array yang dipilih)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NAMA ANGGOTA");
        Serial.println("NAMA ANGGOTA");
        Cayenne.virtualWrite(1,"URUTAN ANGKA ANGGOTA SELANJUTNYA");
      }
        
      */
      
      // jika kartu belum terdaftar, maka tidak mempunyai hak akses
      else
      {
        lcd.setCursor(0, 0);
        lcd.print("TIDAK TERDAFTAR");
        Serial.print("TIDAK TERDAFTAR");
        delay(2000);
      }
 
      for(int x = 0; x < sizeof(cards); x++)                          // for loop untuk membaca kartu RFID
      {        
        for(int i = 0; i < sizeof(rfid.serNum); i++ )                 // for loop untuk membaca serial number kartu RFID
        {
          
          if(rfid.serNum[i] != cards[x][i])                           // kondisi jika serial number tidak sama dengan kartu yang terdaftar maka tidak bisa mengakses
          {
            
            access = false;
            break;
          }
          else                                                        // kondisi jika serial number sama dengan kartu yang terdaftar maka bisa mengakses
          {
            
            access = true;
          }
        }
        if(access) break;                                             // eksekusi perintah untuk mengakses
      }           
    }
        
    if(access)                                                        // proses pengeksekusian akses yang diterima
    {      
      Serial.println("AKSES DITERIMA");
      lcd.setCursor(0,1);
      lcd.print("AKSES DITERIMA");   
      digitalWrite(RELAY,HIGH);
      delay(3000);
      digitalWrite(RELAY,LOW);      
    }   
    else                                                              // proses pengeksekusian jika ditolak
    {      
      Serial.println("AKSES DITOLAK"); 
      lcd.setCursor(0,1);
      lcd.print("AKSES DITOLAK");
    }        
  }
  rfid.halt();                                                        
  lcd.clear();                                                        // clear LCD seperti semula
}  
