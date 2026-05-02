// NETWORK APP

/****************************************
this project made by @bergrfpv 
****************************************/

#include <WiFi.h>

void runNetworkWiFi() {
    // Değişkeni 'static' yapıyoruz. Böylece menüden çıkıp başka uygulamaya 
    // girsen bile, Wi-Fi'ın açık olup olmadığını hafızasında tutar.
    static bool isApActive = false; 

    // Sekmeye girerken butondan elini çekme süresi (debounce)
    delay(200); 

    while (inApp) {
        u8g2.clearBuffer();

        // --- 1. BAŞLIK YAZISI ---
        u8g2.setFont(u8g2_font_ncenB14_tr); // Kalın ve büyük font
        u8g2.drawStr(12, 25, "Wi-Fi");

        // --- 2. WI-FI LOGOSU ÇİZİMİ ---
        // Yazının hemen sağına, merkezden yukarı doğru açılan dalgalar
        int cx = 80; // Merkez X koordinatı
        int cy = 25; // Merkez Y koordinatı
        
        // Merkezdeki küçük nokta
        u8g2.drawBox(cx - 1, cy - 1, 3, 3);
        
        // 3 kademeli kavisli radyo dalgaları
        u8g2.drawCircle(cx, cy, 6, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
        u8g2.drawCircle(cx, cy, 11, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
        u8g2.drawCircle(cx, cy, 16, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);

        // --- 3. DURUM VE BİLGİLENDİRME EKRANI ---
        u8g2.setFont(u8g2_font_6x10_tf);
        if (isApActive) {
            u8g2.drawStr(12, 45, "Status: ON");
            u8g2.drawStr(12, 60, "Press '3' to OFF");
        } else {
            u8g2.drawStr(12, 45, "Status: OFF");
            u8g2.drawStr(12, 60, "Press '3' to ON");
        }

        u8g2.sendBuffer();

        // --- 4. AÇMA/KAPAMA KONTROLÜ ---
       
        if (!digitalRead(macro3)) {
            if (sesAcik) tone(BUZZER_PIN, 1500, 100); // Tıklama sesi
            
            isApActive = !isApActive; // Durumu ON ise OFF, OFF ise ON yap

            if (isApActive) {
                // ESP32'yi Access Point moduna al ve ağı başlat
                WiFi.softAP("ESP32_Wi-Fi"); 
            } else {
                // agı uyut
                WiFi.softAPdisconnect(true); 
                WiFi.mode(WIFI_OFF);
            }

            // ek delay
            delay(2000); 
        }

        // --- 5. UYGULAMADAN ÇIKIŞ KONTROLÜ ---
        // Wi-Fi açıkken menüye dönsen bile 'ben kapatana kadar kapanmaz' mantığı geçerli kalır
        if (!digitalRead(btnLeft) || !digitalRead(hPin)) {
            inApp = false;
            if (sesAcik) tone(BUZZER_PIN, 800, 100);
            delay(250);
            return;
        }
    }
}