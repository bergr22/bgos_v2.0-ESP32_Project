// PONG APP

// game.ino - Pong Uygulaması
#include <Arduino.h>

// Ana dosyada 'const int' olduğu için burada da 'const int' olmalı
extern const int btnUp;
extern const int btnDown;
extern const int btnSelect;
extern const int btnRight;
extern const int btnLeft;
extern bool sesAcik;

// BUZZER_PIN bir macro (#define) olduğu için extern edilmez, 
// zaten her yerde görünürdür. O satırı sil.

void runPongApp() {
  // ... (Geri kalan pong kodun aynı kalsın)

  static float raketY = 16;
  static float topX = 64, topY = 32;
  static float topHizX = 2, topHizY = 2;
  static int skor = 0;
  static float zorlukHizi = 2;
  static int oyunDurumu = 0; // 0:Menu, 1:Oyun, 2:GameOver
  static int secilenMod = 0; // 1:Normal, 2:Hizli, 3:OC

  u8g2.setFont(u8g2_font_6x10_tf);

  // --- DURUM 0: ZORLUK SEÇİMİ ---
  if (oyunDurumu == 0) {
    u8g2.drawStr(35, 12, "Ping-Pong");
    u8g2.drawLine(0, 15, 128, 15);
    u8g2.drawStr(10, 28, "[UP]  -> Normal");
    u8g2.drawStr(10, 42, "[DWN] -> HIZLI");
    u8g2.drawStr(10, 56, "[SEL] -> OC MODU");

    if (!digitalRead(btnUp)) {
      secilenMod = 1; zorlukHizi = 1.5;
      topHizX = zorlukHizi; topHizY = zorlukHizi;
      skor = 0; topX = 64; topY = 32; oyunDurumu = 1; delay(200);
    }
    if (!digitalRead(btnDown)) {
      secilenMod = 2; zorlukHizi = 2.0;
      topHizX = zorlukHizi; topHizY = zorlukHizi;
      skor = 0; topX = 64; topY = 32; oyunDurumu = 1; delay(200);
    }
    if (!digitalRead(btnSelect)) {
      secilenMod = 3; zorlukHizi = 2.0; // OÇ Modu 2.0 ile başlar
      topHizX = zorlukHizi; topHizY = zorlukHizi;
      skor = 0; topX = 64; topY = 32; oyunDurumu = 1; delay(200);
    }
    return;
  }

  // --- DURUM 2: GAME OVER ---
  if (oyunDurumu == 2) {
    u8g2.setFont(u8g2_font_7x14_tf);
    int mesajX = (128 - u8g2.getStrWidth("kaybettun aminagom")) / 2;
    u8g2.drawStr(mesajX, 28, "kaybettun aminagom");
    
    u8g2.setFont(u8g2_font_6x10_tf);
    char finalSkor[15];
    sprintf(finalSkor, "Skorun: %d", skor);
    int skorX = (128 - u8g2.getStrWidth(finalSkor)) / 2;
    u8g2.drawStr(skorX, 45, finalSkor);
    
    int deneX = (128 - u8g2.getStrWidth("[sel] gel dene bida")) / 2;
    u8g2.drawStr(deneX, 60, "[sel] gel dene bida");

    if (!digitalRead(btnSelect)) {
      oyunDurumu = 0;
      delay(300);
    }
    return;
  }

  // --- DURUM 1: OYUN ---
  if (oyunDurumu == 1) {
    if (!digitalRead(btnUp) && raketY > 2) raketY -= 3;
    if (!digitalRead(btnDown) && raketY < 42) raketY += 4;

    topX += topHizX;
    topY += topHizY;

    if (topY <= 2 || topY >= 61) topHizY *= -1;
    if (topX >= 125) { topHizX *= -1; topX = 124; }

    // RAKET FİZİĞİ
    if (topX <= 10 && topX >= 5) { 
      if (topY >= raketY - 2 && topY <= raketY + 22) {
        topHizX = abs(topHizX);
        topX = 11;
        if(sesAcik) tone(BUZZER_PIN, 1500, 20);
        skor++;
        
        // ZORLUK ARTIŞ MANTIĞI
        if (secilenMod == 3) { // OÇ MODU
          if (skor % 3 == 0) {
            topHizX += 1.0 ; // Her 4 skorda bir 1.5 artış
            topHizY = (topHizY > 0) ? topHizY + 1.5 : topHizY - 1.5;
          }
        } else { // NORMAL VE HIZLI MOD
          if (skor % 5 == 0) {
            topHizX += 0.5;
            topHizY = (topHizY > 0) ? topHizY + 0.5 : topHizY - 0.5;
          }
        }
      }
    }

    if (topX < 0) {
      oyunDurumu = 2;
      if(sesAcik) tone(BUZZER_PIN, 500, 500);
    }

    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawBox(2, (int)raketY, 4, 20);
    u8g2.drawDisc((int)topX, (int)topY, 2);
    
    char skorStr[10];
    sprintf(skorStr, "Skor: %d", skor);
    u8g2.drawStr(80, 10, skorStr);
  }
}