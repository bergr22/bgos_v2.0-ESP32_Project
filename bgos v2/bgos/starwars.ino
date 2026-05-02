// STARWARS APP

/****************************************
this project made by @bergrfpv 
****************************************/

#include <Arduino.h>
#include <U8g2lib.h>

// Ana koddan gelen u8g2 objesini kullanıyoruz
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// --- SES NOTALARI ---
#define NOTE_A 440
#define NOTE_F 349
#define NOTE_CH 523

// --- BITMAPS (u8g2 uyumlu formatta) ---
const unsigned char PROGMEM dioda16[] = {
  0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x3F, 0xF0, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0x7F, 0xFF,
  0x7F, 0xFF, 0xFF, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x1F, 0xF0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00
};

// --- OYUN DEĞİŞKENLERİ ---
static int metx = 0, mety = 0;
static int postoji = 0, nep = 8, smjer = 0, go = 0;
static int rx = 95, ry = 0, rx2 = 95, ry2 = 0, rx3 = 95, ry3 = 0, rx4 = 95, ry4 = 0;
static int bodovi = 0, zivoti = 5, nivo = 1;
static int brzina = 3, bkugle = 1, promjer = 10;
static int ispaljeno = 0, poz = 30;
static unsigned long pocetno = 0, odabrano = 0, trenutno = 0, nivovrije = 0;
static bool swSetupDone = false;

// --- YARDIMCI FONKSİYONLAR ---
void swBeep(int note, int duration) {
  if (sesAcik) {
    tone(BUZZER_PIN, note, duration);
    delay(duration);
    noTone(BUZZER_PIN);
  }
}

void resetStarWars() {
  metx = 0; mety = 0; postoji = 0; nep = 8; smjer = 0; go = 0;
  rx = 95; ry = 0; rx2 = 95; ry2 = 0; rx3 = 95; ry3 = 0; rx4 = 95; ry4 = 0;
  bodovi = 0; brzina = 3; bkugle = 1; promjer = 12; zivoti = 5;
  ispaljeno = 0; nivo = 1; pocetno = 0; odabrano = 0; nivovrije = 0;
}

void runStarwarsApp() {
  // İlk girişte açılış müziği ve ayarlar
  if (!swSetupDone) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(30, 30, "STAR WARS");
    u8g2.drawStr(25, 45, "Loading...");
    u8g2.sendBuffer();
    
    swBeep(NOTE_A, 400); swBeep(NOTE_A, 400); swBeep(NOTE_A, 400);
    swBeep(NOTE_F, 300); swBeep(NOTE_CH, 150); swBeep(NOTE_A, 400);
    
    resetStarWars();
    swSetupDone = true;
  }

  trenutno = millis();

  if (go == 0) {
    u8g2.clearBuffer();
    
    // Yıldızlar (Arka plan)
    u8g2.drawPixel(50, 30); u8g2.drawPixel(30, 17); u8g2.drawPixel(80, 57); u8g2.drawPixel(10, 56);

    // Seviye Atlama Mantığı
    if ((trenutno - nivovrije) > 50000) {
      nivovrije = trenutno;
      nivo++;
      brzina++;
      if (nivo % 2 == 0) { bkugle++; if (promjer > 5) promjer--; }
    }

    // Düşman Ateşleme Zamanlaması
    if (pocetno == 0) {
      pocetno = trenutno;
      odabrano = random(400, 1200);
    }

    if ((odabrano + pocetno) < trenutno) {
      pocetno = 0;
      ispaljeno++;
      if (ispaljeno == 1) { rx = 95; ry = nep; }
      if (ispaljeno == 2) { rx2 = 95; ry2 = nep; }
      if (ispaljeno == 3) { rx3 = 95; ry3 = nep; }
      if (ispaljeno == 4) { rx4 = 95; ry4 = nep; }
    }

    // Mermileri Hareket Ettir
    if (ispaljeno > 0) { u8g2.drawCircle(rx, ry, 2); rx -= brzina; }
    if (ispaljeno > 1) { u8g2.drawCircle(rx2, ry2, 1); rx2 -= brzina; }
    if (ispaljeno > 2) { u8g2.drawCircle(rx3, ry3, 4); rx3 -= brzina; }
    if (ispaljeno > 3) { u8g2.drawCircle(rx4, ry4, 2); rx4 -= brzina; }

    // Kontroller (Girişler)
    if (!digitalRead(btnUp) && poz >= 2) poz -= 3;
    if (!digitalRead(btnDown) && poz <= 46) poz += 3;
    if (!digitalRead(btnSelect) && postoji == 0) {
      postoji = 1; metx = 16; mety = poz + 8;
      if (sesAcik) tone(BUZZER_PIN, 1200, 20);
    }

    // Oyuncunun Ateşi
    if (postoji == 1) {
      metx += 8;
      u8g2.drawLine(metx, mety, metx + 4, mety);
      if (metx > 128) postoji = 0;
    }

    // Çizimler
    u8g2.drawBitmap(4, poz, 2, 16, dioda16); // X-Wing
    u8g2.drawDisc(95, nep, promjer);         // Death Star
    u8g2.setDrawColor(0);
    u8g2.drawDisc(97, nep + 3, promjer / 3); // Çukur
    u8g2.setDrawColor(1);

    // Skor ve Can Bilgisi
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.setCursor(33, 62); u8g2.print("Score: "); u8g2.print(bodovi);
    u8g2.setCursor(33, 6);  u8g2.print("Lives: "); u8g2.print(zivoti);
    u8g2.setCursor(110, 6); u8g2.print("L:"); u8g2.print(nivo);

    // Hareket Mantığı (Death Star)
    if (smjer == 0) nep += bkugle; else nep -= bkugle;
    if (nep >= (64 - promjer)) smjer = 1;
    if (nep <= promjer) smjer = 0;

    // Çarpışma Kontrolü (Ateş -> Death Star)
    if (mety >= nep - promjer && mety <= nep + promjer) {
      if (metx > (95 - promjer) && metx < (95 + promjer)) {
        metx = -20;
        if (sesAcik) tone(BUZZER_PIN, 500, 20);
        bodovi++;
        postoji = 0;
      }
    }

    // Çarpışma Kontrolü (Düşman Mermisi -> X-Wing)
    int pozicija = poz + 8;
    if ((rx < 16 && rx > 4 && abs(ry - pozicija) < 8) || 
        (rx2 < 16 && rx2 > 4 && abs(ry2 - pozicija) < 8) || 
        (rx3 < 16 && rx3 > 4 && abs(ry3 - pozicija) < 8) || 
        (rx4 < 16 && rx4 > 4 && abs(ry4 - pozicija) < 8)) {
          zivoti--;
          if (sesAcik) tone(BUZZER_PIN, 100, 100);
          rx = rx2 = rx3 = rx4 = -50; // Mermileri temizle
          ispaljeno = 0;
          delay(200);
    }

    if (rx4 < 1) { ispaljeno = 0; rx4 = 200; }
    if (zivoti <= 0) go = 1;

  } else {
    // GAME OVER EKRANI
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(35, 20, "GAME OVER");
    u8g2.setCursor(40, 35); u8g2.print("Score: "); u8g2.print(bodovi);
    u8g2.drawStr(20, 55, "Press Select");
    
    if (!digitalRead(btnSelect)) {
      resetStarWars();
      swBeep(NOTE_CH, 200);
    }
  }
}