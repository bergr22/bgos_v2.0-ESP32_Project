// COIN FLIP (Yazi Tura) "heads or tails"

/****************************************
this project made by @bergrfpv 
****************************************/

#include <Arduino.h>
#include <U8g2lib.h>


extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern const int btnSelect;
extern const int btnLeft;
extern const int hPin;
extern const int BUZZER_PIN;
extern bool inApp;
extern bool sesAcik;

// Uygulama Durumları
static int flipState = 0; 
static int resultValue = 0; // 0: Heads (Yazı), 1: Tails (Tura)
static unsigned long flipStartTime = 0;
static int animFrame = 0;

void drawCoin(int x, int y, int width, const char* label) {
  // Paranın dış dairesi
  u8g2.drawDisc(x, y, 20); // Dış gölge/kenar
  u8g2.setDrawColor(0);
  u8g2.drawDisc(x, y, 18); // İç boşluk
  u8g2.setDrawColor(1);
  
  // Paranın içindeki metin (Genişliğe göre ölçekli)
  if (width > 10) {
    u8g2.setFont(u8g2_font_7x14_tf);
    int labelW = u8g2.getStrWidth(label);
    u8g2.drawStr(x - (labelW / 2), y + 5, label);
  }
}

void runYaziTuraApp() {
  flipState = 0; // Başlangıçta bekleme modunda
  
  while (inApp) {
    u8g2.clearBuffer();
    
    // --- Çıkış Kontrolü ---
    if (digitalRead(hPin) == LOW || digitalRead(btnLeft) == LOW) {
      inApp = false;
      if(sesAcik) tone(BUZZER_PIN, 800, 100);
      while(digitalRead(hPin) == LOW || digitalRead(btnLeft) == LOW);
      return;
    }

    // --- Arayüz ve Mantık ---
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(35, 10, "COIN FLIP");
    u8g2.drawHLine(30, 12, 68);

    if (flipState == 0) { // IDLE - BEKLEME
      u8g2.drawStr(25, 35, "PRESS SELECT");
      u8g2.drawStr(38, 48, "TO FLIP");
      
      if (digitalRead(btnSelect) == LOW) {
        resultValue = random(0, 2); // 0 veya 1
        flipStartTime = millis();
        flipState = 1; // Animasyonu başlat
        if(sesAcik) tone(BUZZER_PIN, 1200, 50);
        while(digitalRead(btnSelect) == LOW);
      }
    } 
    
    else if (flipState == 1) { // FLIPPING - ANİMASYON
      unsigned long elapsed = millis() - flipStartTime;
      animFrame++;
      
      // Paranın dönme animasyonu (Elips genişliğini değiştirerek)
      int dynamicWidth = abs(20 * cos(animFrame * 0.5)); 
      u8g2.drawEllipse(64, 35, dynamicWidth, 20);
      
      if(sesAcik && animFrame % 4 == 0) tone(BUZZER_PIN, 1500 + dynamicWidth*10, 10);

      if (elapsed > 1500) { // 1.5 saniye sonra dur
        flipState = 2;
        if(sesAcik) {
            tone(BUZZER_PIN, 1000, 100); delay(100);
            tone(BUZZER_PIN, 1500, 200);
        }
      }
    } 
    
    else if (flipState == 2) { // RESULT - SONUÇ
      const char* resStr = (resultValue == 0) ? "YAZI" : "TURA";
      
      // Sonuç Parası
      u8g2.drawDisc(64, 38, 22);
      u8g2.setDrawColor(0);
      u8g2.drawDisc(64, 38, 20);
      u8g2.setDrawColor(1);
      
      u8g2.setFont(u8g2_font_7x14_tf);
      int w = u8g2.getStrWidth(resStr);
      u8g2.drawStr(64 - (w/2), 43, resStr);

      u8g2.setFont(u8g2_font_4x6_tf);
      u8g2.drawStr(32, 63, "PRESS SELECT TO RETRY");

      if (digitalRead(btnSelect) == LOW) {
        flipState = 0; // Tekrar başa dön
        while(digitalRead(btnSelect) == LOW);
        delay(50);
      }
    }

    u8g2.sendBuffer();
  }
}