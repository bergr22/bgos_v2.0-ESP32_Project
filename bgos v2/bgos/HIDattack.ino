// HID ATTACK APP 

/****************************************
this project made by @bergrfpv 
****************************************/

#include <Arduino.h>
#include <BleKeyboard.h>


extern BleKeyboard bleKeyboard;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

extern bool inApp;
extern bool sesAcik;
extern const int BUZZER_PIN;
extern const int btnLeft;
extern const int btnSelect;
extern const int ePin; // Macro 1 Butonu
extern const int hPin; // Macro 2 Butonu


void safePrint(const char* text) {
  for (int i = 0; i < strlen(text); i++) {
    bleKeyboard.write(text[i]);
    delay(10); //buffer delay
  }
}

void runHIDAttack() {
  u8g2.setFont(u8g2_font_6x10_tf);
  
  while (inApp) {
    u8g2.clearBuffer();
    
    // --- exit
    if (digitalRead(btnLeft) == LOW) {
      inApp = false;
      if(sesAcik) tone(BUZZER_PIN, 800, 100);
      while(digitalRead(btnLeft) == LOW);
      return;
    }

    if (bleKeyboard.isConnected()) {
      u8g2.drawStr(30, 15, "HID ATTACK");
      u8g2.drawLine(0, 18, 128, 18);
      u8g2.drawStr(10, 35, "STATUS: READY");
      u8g2.drawStr(5, 50, "[1] -> Fake Update"); 
      u8g2.drawStr(5, 62, "[2] -> ATATURK");  

      // --- SALDIRI 1: FAKE UPDATE (Windows 10) ---
      if (digitalRead(ePin) == LOW) {
        if(sesAcik) tone(BUZZER_PIN, 1500, 100);
        
       
        bleKeyboard.press(KEY_LEFT_GUI);
        bleKeyboard.press('r');
        delay(250);
        bleKeyboard.releaseAll();
        delay(800); 

        
        safePrint("msedge -- fakeupdate.net/win10ue/");
        
        delay(300);
        bleKeyboard.write(KEY_RETURN);
        
       
        u8g2.clearBuffer();
        u8g2.drawStr(20, 35, "EXECUTING...");
        u8g2.sendBuffer();
        
        delay(1200); 
        
      
        bleKeyboard.write(KEY_F11);
        
        if(sesAcik) tone(BUZZER_PIN, 2000, 200);
        while(digitalRead(ePin) == LOW); 
      }

      // --- SALDIRI 2: ATATURK ---
      if (digitalRead(hPin) == LOW && inApp) {
        if(sesAcik) tone(BUZZER_PIN, 1500, 100);
        
        bleKeyboard.press(KEY_LEFT_GUI);
        bleKeyboard.press('r');
        delay(250);
        bleKeyboard.releaseAll();
        delay(800);

        safePrint("https://www.sochic.com/blog/wp-content/uploads/2025/05/Ataturkun-Ozellikleri-ve-Stili-Ataturkun-Sevdigi-Aksesuarlar.jpg");
    
        delay(300);
        bleKeyboard.write(KEY_RETURN);
        delay(1200);
        bleKeyboard.write(KEY_F11);
        while(digitalRead(hPin) == LOW);
      }
    } 
    else {
      u8g2.drawStr(25, 40, "WAITING FOR BT...");
    }
    
    u8g2.sendBuffer();
  }
}