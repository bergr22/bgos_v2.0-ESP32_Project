// WiFi SCANNER

#include <WiFi.h>

void runWiFiApp() {
  static bool scanStarted = false;
  static int n = 0;

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "[ WiFi Scanner ]");
  u8g2.drawLine(0, 12, 128, 12);

 
  if (!scanStarted) {
    u8g2.drawStr(0, 35, "Aglar araniyor...");
    u8g2.sendBuffer(); 
    
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    
    n = WiFi.scanNetworks();
    scanStarted = true;
  }

 
  if (n == 0) {
    u8g2.drawStr(0, 35, "Ag bulunamadi!");
  } else {
    
    for (int i = 0; i < min(n, 4); i++) {
      int yPos = 25 + (i * 10);
      String ssid = WiFi.SSID(i);
      if(ssid.length() > 15) ssid = ssid.substring(0, 15) + ".."; 
      
      String networkInfo = String(i + 1) + ". " + ssid + " (" + String(WiFi.RSSI(i)) + ")";
      u8g2.drawStr(0, yPos, networkInfo.c_str());
    }
    u8g2.drawStr(0, 64, "[select] Scan Again");
  }

  // Seçim tuşuna basılırsa taramayı sıfırla (Yeniden tara)
  if (!digitalRead(btnSelect)) {
    scanStarted = false;
    delay(300);
  }
}