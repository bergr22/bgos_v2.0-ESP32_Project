// APPS

// APPS.ino - Uygulama Fonksiyonları
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <BleKeyboard.h> 

void runHIDApp() {
  u8g2.setFont(u8g2_font_6x10_tf);

  if(bleKeyboard.isConnected()) { 
    u8g2.drawStr(30, 15, "BT HID MACRO");
    u8g2.drawLine(0, 18, 128, 18);
    u8g2.drawStr(25, 35, "Status: Connected");
    u8g2.drawStr(5, 50, "[2] -> Chrome");
    u8g2.drawStr(5, 62, "[1] -> Reset GPU"); 

    // --- MACRO 2: Chrome (2. Pin) ---
    if (digitalRead(hPin) == LOW) {
      bleKeyboard.press(KEY_LEFT_GUI);
      bleKeyboard.press('r');
      delay(150);
      bleKeyboard.releaseAll();
      delay(300); 
      
      bleKeyboard.print("chrome"); 
      delay(100);
      bleKeyboard.write(KEY_RETURN);
      
      if(sesAcik) tone(15, 1500, 100);
      delay(300); // Debounce
    }

    // --- MACRO 1: Reset GPU (1. Pin) ---
    if (digitalRead(ePin) == LOW) {
  // GPU Sürücüsünü Yenile (Win + Ctrl + Shift + B)
  bleKeyboard.press(KEY_LEFT_GUI);   // Windows Tuşu
  bleKeyboard.press(KEY_LEFT_CTRL);
  bleKeyboard.press(KEY_LEFT_SHIFT);
  bleKeyboard.press('b');
  
  delay(100);        
  bleKeyboard.releaseAll(); 
  
  delay(500);       
}
    
    bleKeyboard.releaseAll(); // Güvenlik için her döngü sonunda bırak
  } 
  else {
    // Bağlantı yoksa burası çalışır
    u8g2.drawStr(30, 15, "BT HID MACRO");
    u8g2.drawLine(0, 18, 128, 18);
    u8g2.drawStr(15, 40, "Waiting For Connection...");
    u8g2.drawStr(10, 55, "Device: nothing"); 
  }
}


void runBuzzerApp() {
  // --- DEĞİŞKENLER (STATİC OLARAK KALIYOR) ---
  static Adafruit_NeoPixel pixel(1, 48, NEO_GRB + NEO_KHZ800);
  static bool ledAcik = false;
  static unsigned long sonTiklamaZamani = 0;
  static bool ledInit = false;

  // LED Başlatma
  if (!ledInit) {
    pixel.begin();
    pixel.setBrightness(40); 
    ledInit = true;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(30, 15, "LED & BUZZER");
  u8g2.drawLine(0, 18, 128, 18);

  // --- SES KONTROLÜ (YUKARI TUŞUNA 1 KERE BASINCA) ---
  if (!digitalRead(btnUp)) {
    sesAcik = !sesAcik; // Ana dosyadaki değişkeni değiştirir
    if(sesAcik) { 
      tone(BUZZER_PIN, 1500, 50); // Ses açıldı onayı
    }
    delay(300); // Tek basışı korumak için kısa bekleme
  }

  // --- LED KONTROLÜ (SELECT TUŞUNA ÇİFT TIKLAMA) ---
  if (!digitalRead(btnSelect)) {
    unsigned long suAn = millis();
    if (suAn - sonTiklamaZamani > 50 && suAn - sonTiklamaZamani < 450) {
      ledAcik = !ledAcik;
      if (!ledAcik) {
        pixel.setPixelColor(0, pixel.Color(0, 0, 0)); 
        pixel.show();
      }
      // LED durum değişikliği sesi (Ses açıksa)
      if(sesAcik) { tone(BUZZER_PIN, 2000, 50); delay(50); tone(BUZZER_PIN, 2500, 50); }
      delay(400); 
    }
    sonTiklamaZamani = suAn;
    delay(150); 
  }

  // Durum Yazıları ve RGB Efekti
  if (ledAcik) {
    u8g2.drawStr(10, 35, "RGB LED: AKTIF");
    u8g2.drawStr(10, 48, "Mod: Yavas Gecis");
    
    float h = millis() / 20.0; 
    int r = sin(h * 0.02) * 127 + 128;
    int g = sin(h * 0.02 + 2) * 127 + 128;
    int b = sin(h * 0.02 + 4) * 127 + 128;
    pixel.setPixelColor(0, pixel.Color(r, g, b));
    pixel.show();
  } else {
    u8g2.drawStr(10, 35, "RGB LED: KAPALI");
    u8g2.drawStr(10, 48, "[SEL] x2 Bas -> LED");
  }
  
  // Alt Bilgi Satırı
  u8g2.drawStr(10, 62, sesAcik ? "Sound: ON [UP: OFF]" : "Sound: OFF [UP: ON]");
}