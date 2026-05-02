// KEYBOARD APP

/****************************************
this project made by @bergrfpv 
****************************************/

extern const int btnRight;
extern const int btnLeft;
extern const int btnUp;
extern const int btnDown;
extern const int btnSelect;
extern const int hPin;
extern const int ePin;
extern bool inApp;
extern bool sesAcik;
extern const int BUZZER_PIN;
extern const int macro3;
extern const int macro4;
extern const int PeakBtn;
extern const int NorthEast;

void runGamepadApp() {
  if(bleKeyboard.isConnected()) {
    u8g2.clearBuffer();         

    // BAŞLIK BÖLÜMÜ
    u8g2.setFont(u8g2_font_6x10_tf); 
    u8g2.drawStr(38, 10, "Keyboard");  // Başlığı biraz yukarı çektim (y=10)
    u8g2.drawLine(0, 13, 128, 13);     // Çizgiyi başlığın hemen altına aldım

    // BUTONLAR (Çizginin altından başlıyor)
    // Sol Sütun (x=0)
    u8g2.drawStr(0, 28, "1 = Alt+F4"); 
    u8g2.drawStr(0, 45, "2 = Menu");
    u8g2.drawStr(0, 62, "3 = Enter");

    // Sağ Sütun (x=68)
    u8g2.drawStr(68, 28, "4 = TAB");
    u8g2.drawStr(68, 45, "5 = E"); 
    u8g2.drawStr(68, 62, "6 = ESC");

    // Ayırıcı dikey çizgi (Opsiyonel, daha profesyonel durur)
    u8g2.drawLine(64, 18, 64, 64);

    u8g2.sendBuffer();

    // --- YÖN TUŞLARI (WASD) ---
    if (!digitalRead(btnUp)) bleKeyboard.press('w'); else bleKeyboard.release('w');
    if (!digitalRead(btnDown)) bleKeyboard.press('s'); else bleKeyboard.release('s');
    if (!digitalRead(btnLeft)) bleKeyboard.press('a'); else bleKeyboard.release('a');
    if (!digitalRead(btnRight)) bleKeyboard.press('d'); else bleKeyboard.release('d');
    if (!digitalRead(btnSelect)) bleKeyboard.press(' '); else bleKeyboard.release(' ');
    if (!digitalRead(NorthEast)) bleKeyboard.press('e'); else bleKeyboard.release('e');
    // Macro3: Enter
    if (!digitalRead(macro3)) {
       bleKeyboard.press(KEY_RETURN); 
     } else {
       bleKeyboard.release(KEY_RETURN);
    }

    // Macro4: Tab
    if (!digitalRead(macro4)) {
      bleKeyboard.press(KEY_TAB);
    } else {
      bleKeyboard.release(KEY_TAB);
    }

    // ePin: Alt + F4
    if (!digitalRead(ePin)) {
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press(KEY_F4);
    } else {
      bleKeyboard.release(KEY_LEFT_ALT);
      bleKeyboard.release(KEY_F4);
    }

    // PeakBtn: ESC (Escape)
    if (!digitalRead(PeakBtn)) {
      bleKeyboard.press(KEY_ESC);
    } else {
      bleKeyboard.release(KEY_ESC);
    }
//ÇIKIŞ İÇİN HPİN
   if (digitalRead(hPin) == LOW) { 
      bleKeyboard.releaseAll(); 
      if(sesAcik) tone(15, 800, 100); 
      inApp = false; 
      delay(500); 
  } 
}
//BAĞLANTI BEKLEME
else if (!bleKeyboard.isConnected()) {
    // Bağlantı yoksa ve hPin'e basılmıyorsa burası çalışır
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(10, 35, "BAGLANTI BEKLENIYOR");
    u8g2.sendBuffer();
  
  }
}
