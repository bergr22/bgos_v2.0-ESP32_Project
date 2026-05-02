// BGOS (bgos) main file

/****************************************
this project made by @bergrfpv 
****************************************/


#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ㅤ", "user", 100);

// --- DONANIM AYARLARI ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);

// Buton ve Buzzer Pinleri
const int btnRight = 38;
const int btnLeft = 35;
const int btnUp = 37;
const int btnDown = 36;
const int btnSelect = 39;
const int BUZZER_PIN = 15;
const int macro3 = 2;
const int macro4 = 1;
const int hPin = 40; 
const int ePin = 41; 
const int PeakBtn = 10;
const int NorthEast = 17;

bool isLocked = true;        // Cihaz kilitli başlasın
String inputCode = "";       // Kullanıcının girdiği şifre
const String secretCode = "111333"; // Belirlenen şifre

// The Menu
int menuIndex = 0;
bool sesAcik = true; 
bool inApp = false; 
const int totalApps = 14;
const char* apps[totalApps] = {"1. WASD Keyboard", 
"2. Wi-Fi Scanner", 
"3. NetWork Wi-Fi", 
"4. Buzzer", 
"5. HID Macro", 
"6. HID Attacks", 
"7. Game", 
"8. Music", 
"9. Tetris", 
"10. Star Wars", 
"11. Timer",
"12. Snake",
"13. Tic Tac Toe",
"14. Yazi Tura"};


// Uygulama Fonksiyonları Prototipleri
void runGamepadApp();
void runWiFiApp();
void runHIDApp();
void runBuzzerApp();
void runPongApp();
void runSongApp();
void runTetrisApp();
void runStarwarsApp();
void runTimerApp();
void runSnakeApp();
void runNetworkWiFi();
void runTicTacToeApp();
void runYaziTuraApp();
void runHIDAttack();


void setup() {
  Serial.begin(115200);

  bleKeyboard.begin();
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(hPin, INPUT_PULLUP);
  pinMode(ePin, INPUT_PULLUP);
  pinMode(macro3,INPUT_PULLUP);
  pinMode(macro4,INPUT_PULLUP);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(PeakBtn, INPUT_PULLUP);
  pinMode(NorthEast, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Açılış Sesi
  tone(BUZZER_PIN, 1000); delay(100); noTone(BUZZER_PIN);

  // Ekran Başlatma
Wire.begin(5, 4); 
delay(200);

if (u8g2.begin()) {
    u8g2.setContrast(255);
}

u8g2.clearBuffer();

// Türk Bayrağı çizimi

u8g2.setDrawColor(1); 
u8g2.drawRBox(24, 4, 80, 50, 3); 


u8g2.setDrawColor(0); 
u8g2.drawDisc(56, 29, 15);       
u8g2.setDrawColor(1); 
u8g2.drawDisc(61, 29, 12);        
u8g2.setDrawColor(0); 


int sx = 82; 
int sy = 29; 

u8g2.setDrawColor(0); 


u8g2.drawTriangle(sx-3, sy, sx+3, sy, sx, sy-11);


u8g2.drawTriangle(sx-1, sy-4, sx-1, sy+2, sx-11, sy-2);


u8g2.drawTriangle(sx+1, sy-4, sx+1, sy+2, sx+11, sy-2);


u8g2.drawTriangle(sx-3, sy+1, sx-1, sy+4, sx-7, sy+11); 
u8g2.drawTriangle(sx+3, sy+1, sx+1, sy+4, sx+7, sy+11);
u8g2.drawTriangle(sx-1, sy, sx+1, sy, sx, sy+5); 
u8g2.drawDisc(sx-2, sy+1, 2); 
u8g2.drawDisc(sx+2, sy+1, 2); 
u8g2.drawDisc(sx, sy, 3);


u8g2.setDrawColor(1); 
u8g2.setFont(u8g2_font_4x6_tf); 
int signatureX = (128 - u8g2.getStrWidth("made by bergrfpv")) / 2;
u8g2.drawStr(signatureX, 62, "made by bergrfpv"); 

u8g2.sendBuffer();
delay(2200);
}
void loop() {

  u8g2.clearBuffer();
  if (isLocked) {
    drawLockScreen(); // Şifre girme ekranı
    handleLockInput(); 
    u8g2.sendBuffer();
  } else {
    
    
  int menuOffset = 0; 

  if (!inApp) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 12, "bgos - v2.0");
    u8g2.drawLine(0, 15, 128, 15);

    if (!digitalRead(btnDown)) { 
      menuIndex++; 
      if(menuIndex >= totalApps) menuIndex = 0; 
      if(sesAcik) tone(BUZZER_PIN, 2000, 20); 
      delay(200); 
    }
    if (!digitalRead(btnUp)) { 
      menuIndex--; 
      if(menuIndex < 0) menuIndex = totalApps - 1; 
      if(sesAcik) tone(BUZZER_PIN, 2000, 20); 
      delay(200); 
    }
    if (!digitalRead(btnSelect)) { 
      inApp = true; 
      if(sesAcik) tone(BUZZER_PIN, 1500, 100); 
      delay(250); 
    }

   // Kaydırma (Scroll) hesaplama
    if (menuIndex < menuOffset) {
        menuOffset = menuIndex; // 
    }
    if (menuIndex >= menuOffset + 4) { 
        menuOffset = menuIndex - 3; // Aşağı inerken (4 satır gösteriyorsak) ekranı aşağı kaydır
    }

    for (int i = 0; i < 4; i++) { // Sadece 4 satır çiz
        int appIdx = menuOffset + i;
        if (appIdx >= totalApps) break; 

        int yPos = 30 + (i * 11);
        if (appIdx == menuIndex) {
            u8g2.drawBox(0, yPos - 9, 128, 11);
            u8g2.setDrawColor(0); 
            u8g2.drawStr(10, yPos, apps[appIdx]);
            u8g2.setDrawColor(1);
        } else {
            u8g2.drawStr(10, yPos, apps[appIdx]);
        }
    }
    // Ekranın sağ tarafına küçük bir kaydırma çubuğu (scrollbar)
    int barHeight = (4 * 40) / totalApps;
    int barPos = 20 + (menuOffset * 40 / totalApps);
    u8g2.drawVLine(125, 20, 40);
    u8g2.drawBox(124, barPos, 3, barHeight);

  } 
  else {
      // --- GENEL GERİ DÖNÜŞ MANTIĞI ---
      if (menuIndex != 0) { 
        if (!digitalRead(btnLeft)) { 
          inApp = false; 
          if(sesAcik) tone(BUZZER_PIN, 800, 100); 
          delay(250); 
        }
      } 
      else {
        if (!digitalRead(hPin)) {
          bleKeyboard.releaseAll(); 
          inApp = false; 
          if(sesAcik) tone(BUZZER_PIN, 800, 100); 
          delay(250); 
        }
      }

      // --- UYGULAMALARI ÇALIŞTIRMA --- sıralaması
      switch (menuIndex) {
        case 0: runGamepadApp(); break; 
        case 1: runWiFiApp();    break;
        case 2: runNetworkWiFi();break;
        case 3: runBuzzerApp();  break;
        case 4: runHIDApp();     break; 
        case 5: runHIDAttack();  break;
        case 6: runPongApp();    break;
        case 7: runSongApp();    break;
        case 8: runTetrisApp();  break;
        case 9: runStarwarsApp();break;
        case 10: runTimerApp();   break;
        case 11: runSnakeApp();  break;
        case 12: runTicTacToeApp();break;
        case 13: runYaziTuraApp(); break;
      }
    }
  u8g2.sendBuffer();
  }
}

void drawLockScreen() {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(30, 20, "LOCKED");
  u8g2.drawStr(10, 40, "Password:");
  
  // Girilen şifre kadar yıldız göster
  String stars = "";
  for(int i=0; i < inputCode.length(); i++) stars += "* ";
  u8g2.drawStr(10, 55, stars.c_str());
}

void handleLockInput() {
  if (!digitalRead(btnUp))    { inputCode += "1"; delay(150); }
  if (!digitalRead(btnDown))  { inputCode += "2"; delay(150); }
  if (!digitalRead(btnRight)) { inputCode += "3"; delay(150); }
  if (!digitalRead(btnLeft))  { inputCode += "4"; delay(150); }

  if (inputCode.length() >= 6) { // 6 haneli şifre dolunca kontrol et
    if (inputCode == secretCode) {
      isLocked = false;
      if(sesAcik) tone(BUZZER_PIN, 1500, 200); // Başarılı sesi
    } else {
      inputCode = ""; // Yanlışsa sıfırla
      if(sesAcik) tone(BUZZER_PIN, 400, 300); // Hata sesi
    }
  }
}