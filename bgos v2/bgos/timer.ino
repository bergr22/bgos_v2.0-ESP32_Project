// TIMER APP

/****************************************
this project made by @bergrfpv 
****************************************/

#include <Arduino.h>
#include <U8g2lib.h>


extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


static int timerMin = 0;
static int timerSec = 0;
static bool isRunning = false;
static bool selectingMin = true; // true: dakika, false: saniye
static unsigned long previousMillis = 0;
static bool timerSetupDone = false;

void runTimerApp() {
    if (!timerSetupDone) {
        timerMin = 0; timerSec = 0;
        isRunning = false;
        timerSetupDone = true;
    }

  
    if (!isRunning) {
        
        if (!digitalRead(btnRight) || !digitalRead(btnLeft)) {
            selectingMin = !selectingMin;
            delay(200);
        }

       
        if (!digitalRead(btnUp)) {
            if (selectingMin) timerMin++; else timerSec++;
            if (timerSec > 59) timerSec = 0;
            delay(150);
        }
        if (!digitalRead(btnDown)) {
            if (selectingMin) timerMin--; else timerSec--;
            if (timerMin < 0) timerMin = 0;
            if (timerSec < 0) timerSec = 59;
            delay(150);
        }

        
        if (!digitalRead(btnSelect)) {
            if (timerMin > 0 || timerSec > 0) isRunning = true;
            delay(200);
        }
    } else {
       // Geri Sayım Mantığı
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            if (timerSec > 0) {
                timerSec--;
            } else if (timerMin > 0) {
                timerMin--;
                timerSec = 59;
            } else {
                isRunning = false; // Süre doldu
                
                if (sesAcik) {
                    for (int i = 0; i < 5; i++) {
                        tone(BUZZER_PIN, 2500, 600); // 
                        // Alarm çalarken butona basılırsa sustur 
                        for(int j = 0; j < 60; j++) { // 600ms = 60 * 10ms
                            if (!digitalRead(btnSelect) || !digitalRead(btnUp) || !digitalRead(btnDown) || !digitalRead(btnLeft) || !digitalRead(btnRight)) {
                                noTone(BUZZER_PIN); return; 
                            }
                            delay(10);
                        }
                        noTone(BUZZER_PIN);
                        
                        // Bekleme süresinde de kontrol et (900ms)
                        for(int j = 0; j < 90; j++) {
                            if (!digitalRead(btnSelect) || !digitalRead(btnUp) || !digitalRead(btnDown) || !digitalRead(btnLeft) || !digitalRead(btnRight)) {
                                return;
                            }
                            delay(10);
                        }
                    }
                }
            }
        }
    }
        
        if (!digitalRead(btnSelect)) {
            isRunning = false;
            delay(50); // 
            while(!digitalRead(btnSelect)); 
    // --- EKRAN ÇİZİMİ ---
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso24_tr);
    
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", timerMin, timerSec);
    
    // 
    if (selectingMin) {
        u8g2.drawStr(10, 40, timeStr);
        u8g2.drawHLine(10, 45, 50); 
    } else {
        u8g2.drawStr(10, 40, timeStr);
        u8g2.drawHLine(70, 45, 50); 
    }

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(20, 60);
    u8g2.print(isRunning ? "Counting..." : "Minute - Second");
    
    u8g2.sendBuffer();
}
}