// SONG APP

#include <Arduino.h>
#include <U8g2lib.h>

bool şarkıDurdur = false;
extern const int btnUp, btnDown, btnSelect, btnLeft, BUZZER_PIN;
extern bool sesAcik, inApp;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

//NOTALAR
const int e4 = 329;  const int a4 = 440;  const int b4 = 494;  const int c5 = 523;
const int gd4 = 415; const int d5 = 587;  const int e5 = 659;  const int f5 = 698;
const int cd5 = 554; const int dd5 = 622; const int g5 = 784;  const int a5 = 880;
const int g4 = 392;  const int fd4 = 370; const int dd4 = 311; double s4;
const int ad4 = 466; const int ad5 = 932; const int gd5 = 830; const int bb3 = 233; 
const int eb4 = 311; const int gb4 = 370; const int ab4 = 415; const int bb4 = 466;
const int db4 = 277; const int f4 = 349; const int fd5 = 740; const int c6 = 1046; 
const int b5 = 988; const int f6 = 1397; const int c4 = 261; const int cd4 = 277; 
const int g3 = 196; const int a3 = 220; const int b3 = 247; const int d6 = 1175; 
const int e6 = 1319; const int d4 = 294; const int d3 = 147; const int cd6 = 1109;
const int fd6 = 1480;   


double bpm(int tempo) {
  return 60.0 / tempo * 1000.0;
}

void playTone(int frekans, int deger) {
    if (şarkıDurdur) return;

    double s1 = s4 * 4.0;
    double s2 = s4 * 2.0;
    double s8 = s4 / 2.0;
    double s16 = s4 / 4.0;
    double s32 = s4 / 8.0;
    double s64 = s4 / 16.0;
    
    double sure = 0;
    double es = 0;

   switch (deger) {
        // --- Standart Vuruşlar (Notalar arası %25 boşluk bırakır, ses netleşir) ---
        case 1:   sure = s1 - s4; es = s4; break;
        case 2:   sure = s2 - s8; es = s8; break;
        case 4:   sure = s4 - s16; es = s16; break;
        case 8:   sure = s8 - s32; es = s32; break;
        case 16:  sure = s16 * 0.75; es = s16 * 0.25; break; 
        case 32:  sure = s32 * 0.75; es = s32 * 0.25; break; 
        case 64:  sure = s64; es = 0; break;

        // --- Noktalı ve Özel Vuruşlar (İstiklal Marşı ve Diğerleri) ---
        case 45:  sure = (s4 + s8) * 0.75; es = (s4 + s8) * 0.25; break;
        case 455: sure = ((s4 + s8) + s16) * 0.75; es = ((s4 + s8) + s16) * 0.25; break; 
        case 25:  sure = (s4 + s2) * 0.75; es = (s4 + s2) * 0.25; break;
        case 85:  sure = (s8 + s16) * 0.75; es = (s8 + s16) * 0.25; break;
        case 88:  sure = s8; es = 0; break; 
        case 44:  sure = s4; es = 0; break;
        
        // --- Sessizlik (Es) Notaları ---
        case 40:  sure = 0; es = s4; break;
        case 80:  sure = 0; es = s8; break;
        case 20:  sure = 0; es = s2; break;
        case 250: sure = 0; es = (s4 + s2); break;

        // --- The à la Menthe & Çok Hızlı Parçalar (Triole ve Keskin Geçişler) ---
        case 169:  sure = s32 * 0.8; es = s32 * 0.2; break; 
        case 329:  sure = s64 * 0.9; es = s64 * 0.1; break;
        case 163:  sure = (s16 * 2.0 / 3.0) * 0.9; es = (s16 * 2.0 / 3.0) * 0.1; break; 
        case 1630: sure = 0; es = s16 * 2.0 / 3.0; break;
        case 89:   sure = s16 * 0.8; es = s16 * 0.2; break;
        case 83:   sure = (s8 * 2.0 / 3.0) * 0.9; es = (s8 * 2.0 / 3.0) * 0.1; break;
        case 161:  sure = s16 * 0.7; es = s16 * 0.3; break; 
        case 163118: sure = (s8 / 3.0 + s8) * 0.75; es = (s8 / 3.0 + s8) * 0.25; break;
        case 118:  sure = s8; es = 0; break;

        default:   sure = s16 * 0.75; es = s16 * 0.25; break;
    }

// --- ÇALMA VE DURDURMA KONTROLÜ (GÜNCELLENDİ) ---
if (frekans > 0 && sesAcik) {
    tone(BUZZER_PIN, frekans);
}

unsigned long baslangicZamani = millis();
while (millis() - baslangicZamani < sure) {
    if (digitalRead(btnSelect) == LOW) {
        şarkıDurdur = true;
        noTone(BUZZER_PIN);
        
        // ELİNİ BUTONDAN ÇEKENE KADAR BEKLE (Kritik Düzeltme)
        while(digitalRead(btnSelect) == LOW) { delay(10); } 
        delay(200); // Küçük bir debouncing payı
        
        return; 
    }
    delay(1);
}
noTone(BUZZER_PIN);

unsigned long esBaslangic = millis();
while (millis() - esBaslangic < es) {
    if (digitalRead(btnSelect) == LOW) {
        şarkıDurdur = true;
        
        // ELİNİ BUTONDAN ÇEKENE KADAR BEKLE
        while(digitalRead(btnSelect) == LOW) { delay(10); }
        delay(200);
        
        return;
    }
    delay(1);
}
}
void istiklalMarsi() {

   s4 = bpm(101); // Tempo ayarı
for (int i = 0; i < 2; i++) {
    // 1. Satır
    playTone(e4, 4);   playTone(a4, 4);   playTone(b4, 4);   playTone(c5, 4);
    // 2. Satır
    playTone(gd4, 85); playTone(b4, 16);  playTone(a4, 25);  playTone(a4, 4);
    // 3. Satır
    playTone(d5, 4);   playTone(e5, 4);   playTone(f5, 85);  playTone(e5, 16);
    // 4. Satır
    playTone(cd5, 85); playTone(e5, 16);  playTone(d5, 25);
    // 5. Satır
    playTone(e5, 83);  playTone(dd5, 83); playTone(e5, 83);
    playTone(b4, 4);   playTone(b4, 455); playTone(d5, 16);
    // 6. Satır
    playTone(c5, 85);  playTone(gd4, 16); playTone(a4, 85);  playTone(b4, 16);
    playTone(c5, 85);  playTone(d5, 16);  playTone(e5, 85);  playTone(f5, 16);
    playTone(g5, 85);  playTone(a5, 16);
    // 7. Satır
    playTone(g5, 4);   playTone(g4, 83);  playTone(fd4, 83); playTone(g4, 83);
    // 8. Satır
    playTone(e5, 4);   playTone(d5, 4);   playTone(c5, 25);
    // 9. Satır
    playTone(e4, 83);  playTone(dd4, 83); playTone(e4, 83);
    playTone(b4, 4);   playTone(e4, 4);   playTone(e5, 85);  playTone(d5, 16);
    // 10. Satır
    playTone(c5, 83);  playTone(b4, 83);  playTone(c5, 83);  playTone(a4, 4);
    // 11. Satır
    playTone(a5, 455); playTone(g5, 16);  playTone(f5, 85);  playTone(e5, 16);
    playTone(d5, 85);  playTone(c5, 16);  playTone(b4, 85);  playTone(a4, 16);
    // 12. Satır
    playTone(e5, 4);   playTone(e4, 4);   playTone(a4, 25);
    
    delay(500); // İki kıta arası bekleme
  }
}
void runSongApp() {    //Ana Fonksiyon
    static int songIndex = 0;
    const int totalSongs = 9; // 
    const char* songs[totalSongs] = {"1. Istiklal Marsi", "2. Egyptian Music", "3. Snap Manifest", "4. Donsen Bile", "5. The a la Menthe", "6. Anlamazdin", "7. Ben Tabii Ki" , "8. Fikrimin Ince Gulu" , "9. Bir Derdim Var"};

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(25, 12, "Music Player"); 
    u8g2.drawLine(0, 15, 128, 15);

    // Menü Kontrolleri
    if (!digitalRead(btnDown)) { songIndex++; if(songIndex >= totalSongs) songIndex = 0; delay(200); }
    if (!digitalRead(btnUp)) { songIndex--; if(songIndex < 0) songIndex = totalSongs - 1; delay(200); }
    
    if (!digitalRead(btnSelect)) {
        while(digitalRead(btnSelect) == LOW); 
        delay(100);
        şarkıDurdur = false; 

        if (songIndex == 0) istiklalMarsi(); 
        else if (songIndex == 1) egyptianTrend();
        else if (songIndex == 2) snapmanifest(); 
        else if (songIndex == 3) donsenBile(); 
        else if (songIndex == 4) theALaMenthe();
        else if (songIndex == 5) anlamazdin();
        else if (songIndex == 6) benTabiiKi();
        else if (songIndex == 7) fikriminInceGulu();
        else if (songIndex == 8) birDerdimVar();
        şarkıDurdur = false; 
    }

    // --- SCROLL (KAYDIRMA) MANTIĞI ---
    int startItem = 0;
    // Eğer seçili şarkı 3. sıradan (index 2) aşağıdaysa listeyi kaydır
    if (songIndex >= 3) {
        startItem = songIndex - 2;
    }

    // Ekranda aynı anda en fazla 3-4 şarkı gösterir
    for (int i = 0; i < 4; i++) {
        int currentItem = startItem + i;
        if (currentItem >= totalSongs) break; // Liste bittiyse çizme

        int yPos = 30 + (i * 12); // Satır aralığı (scroll için biraz daralttım)

        if (currentItem == songIndex) {
            u8g2.drawBox(0, yPos - 9, 128, 11); 
            u8g2.setDrawColor(0); 
            u8g2.drawStr(5, yPos, songs[currentItem]);
            u8g2.setDrawColor(1); 
        } else {
            u8g2.drawStr(5, yPos, songs[currentItem]); 
        }
    }
}

void egyptianTrend() {
  s4 = bpm(120); // Orijinal kodundaki tempo
  şarkıDurdur = false;
    for (int i = 0; i < 2; i++) {
    if (şarkıDurdur) break;

  // Giriş Bölümü
  playTone(g5, 89); playTone(g5, 89); playTone(g5, 89);
  playTone(g5, 8);  playTone(gd5, 8); 
  playTone(g5, 163); playTone(gd5, 163); playTone(g5, 163);
  playTone(d5, 8);
  
  // 2. Bölüm
  playTone(g5, 163); playTone(gd5, 163); playTone(g5, 163);
  playTone(f5, 8);
  playTone(f5, 163); playTone(g5, 163); playTone(f5, 163);
  playTone(dd5, 8);
  playTone(dd5, 163); playTone(f5, 163); playTone(dd5, 163);
  playTone(cd5, 8);
  playTone(cd5, 16); playTone(f5, 16); playTone(dd5, 16); playTone(cd5, 16);

  // Ana Döngü (2 Tekrar)

    // 3. Kısım
    playTone(c5, 8);  playTone(cd5, 16); playTone(c5, 16); playTone(e5, 16);
    playTone(cd5, 16); playTone(f5, 16);  playTone(c5, 16); playTone(e5, 16);
    playTone(c5, 16);  playTone(cd5, 16); playTone(c5, 16); playTone(ad4, 16);
    playTone(c5, 16);  playTone(cd5, 16); playTone(ad4, 16);
    
    // 4. Kısım
    playTone(c5, 8);  playTone(cd5, 16); playTone(c5, 16); playTone(g4, 16);
    playTone(ad4, 16); playTone(c5, 16);  playTone(cd5, 16); playTone(e5, 16);
    playTone(f5, 16);  playTone(e5, 16);  playTone(c5, 16); playTone(ad4, 4);
    
    // 5. Kısım
    playTone(c5, 8);   playTone(cd5, 16); playTone(c5, 16); playTone(e5, 16);
    playTone(c5, 16);   playTone(f5, 16);  playTone(c5, 16); playTone(e5, 16);
    playTone(c5, 16);   playTone(f5, 16);  playTone(c5, 16); playTone(g5, 16);
    playTone(c5, 16);   playTone(f5, 16);  playTone(c5, 16);
    
    // 6. Kısım (Final)
    playTone(e5, 16);  playTone(f5, 16);  playTone(g5, 16);  playTone(gd5, 16);
    playTone(g5, 16);  playTone(gd5, 32); playTone(ad5, 32); playTone(gd5, 32);
    playTone(ad5, 32); playTone(g5, 16);  playTone(f5, 16);  playTone(e5, 16);
    playTone(f5, 16);  playTone(e5, 16);  playTone(cd5, 16); playTone(c5, 4);
 }
}

void snapmanifest() {
    s4 = bpm(146); // Şarkının temposu
    şarkıDurdur = false;

    // Şarkı Başlıyor
    for (int j = 0; j < 2; j++) {
        if (şarkıDurdur) break;
        playTone(0, 20); // Es
        playTone(gb4, 8); playTone(gb4, 8); playTone(gb4, 8); playTone(ab4, 8);
        playTone(f4, 4);  playTone(db4, 4); playTone(f4, 4);  playTone(db4, 4);
        
        if (şarkıDurdur) break;
        playTone(0, 40); 
        playTone(bb3, 8); playTone(bb3, 8); playTone(gb4, 8); playTone(gb4, 8); playTone(gb4, 8); playTone(ab4, 8);
        playTone(f4, 4);  playTone(db4, 8); playTone(db4, 8); playTone(f4, 4);  playTone(gb4, 8); playTone(gb4, 8);
        playTone(eb4, 4);
        
        if (şarkıDurdur) break;
        playTone(0, 40);
        playTone(gb4, 8); playTone(gb4, 8); playTone(gb4, 8); playTone(ab4, 8);
        playTone(f4, 4);  playTone(db4, 8); playTone(db4, 8); playTone(f4, 4);  playTone(db4, 4);
        playTone(eb4, 2); playTone(f4, 2);  playTone(gb4, 4);
        
        if (şarkıDurdur) break;
        playTone(eb4, 8); playTone(f4, 8);  playTone(gb4, 8); playTone(f4, 8);  playTone(eb4, 4);

        // Nakarat (2 Tekrar)
        for (int i = 0; i < 2; i++) {
            if (şarkıDurdur) break;
            playTone(bb4, 4); playTone(eb4, 8); playTone(eb4, 8); playTone(eb4, 8); playTone(bb4, 8); playTone(ab4, 4);
            playTone(0, 40);
            playTone(f4, 8);  playTone(f4, 8);  playTone(gb4, 8); playTone(f4, 8);  playTone(eb4, 8); playTone(db4, 8);
            playTone(eb4, 4); playTone(0, 40);  playTone(eb4, 4); playTone(0, 40);  playTone(eb4, 4); playTone(0, 40);
            playTone(f4, 4);  playTone(gb4, 4);
            
            if (şarkıDurdur) break;
            playTone(bb4, 4); playTone(eb4, 8); playTone(eb4, 8); playTone(eb4, 8); playTone(bb4, 8); playTone(ab4, 4);
            playTone(0, 40);
            playTone(f4, 8);  playTone(f4, 8);  playTone(gb4, 8); playTone(f4, 8);  playTone(eb4, 8); playTone(db4, 8);
            playTone(eb4, 4); playTone(eb4, 4); playTone(eb4, 4); playTone(gb4, 4);
        }
    }
}

void donsenBile() {
    s4 = bpm(121); // Şarkı temposu
    şarkıDurdur = false;

    // --- Intro / Enstrümantal (2 Tekrar) ---
    for(int i = 0; i < 2; i++){
        if (şarkıDurdur) break;
        playTone(e5, 4); playTone(g5, 4); playTone(c6, 2);
        playTone(b5, 16); playTone(c6, 16); playTone(b5, 45);
        playTone(a5, 16); playTone(b5, 16); playTone(a5, 45);
        playTone(d5, 4); playTone(fd5, 45); playTone(b5, 16);
        playTone(a5, 16); playTone(b5, 4);
        playTone(a5, 16); playTone(g5, 16); playTone(a5, 45);
        playTone(g5, 16); playTone(fd5, 16); playTone(g5, 45);
        playTone(c5, 4); playTone(e5, 4); playTone(a5, 2);
        playTone(g5, 16); playTone(fd5, 16); playTone(g5, 45);
        playTone(fd5, 16); playTone(e5, 16); playTone(fd5, 45);
        playTone(0, 80); 
        playTone(g5, 16); playTone(a5, 16); playTone(g5, 8);
        playTone(fd5, 16); playTone(g5, 16); playTone(fd5, 8);
        playTone(e5, 16); playTone(fd5, 16); playTone(e5, 8);
        playTone(d5, 16); playTone(e5, 16); playTone(d5, 8);
        playTone(c5, 16); playTone(d5, 16); playTone(c5, 8);
        playTone(b4, 16); playTone(c5, 16); playTone(b4, 2);
    }

    // --- Sözlü Bölüm 1 (2 Tekrar) ---
    for(int i = 0; i < 2; i++){
        if (şarkıDurdur) break;
        playTone(0, 80);
        playTone(e5, 4); playTone(g5, 4); playTone(c6, 45);
        playTone(b5, 8); playTone(b5, 4); playTone(a5, 4);
        playTone(a5, 4); playTone(0, 80); playTone(0, 80);
        playTone(d5, 4); playTone(fd5, 8); playTone(b5, 45);
        playTone(a5, 8); playTone(a5, 4); playTone(g5, 8);
        playTone(g5, 2); playTone(0, 80);
        playTone(c5, 4); playTone(e5, 8); playTone(a5, 4);
        playTone(g5, 4); playTone(g5, 45); playTone(fd5, 8);
        playTone(fd5, 2); playTone(0, 80);
        playTone(g5, 4); playTone(fd5, 8); playTone(fd5, 8);
        playTone(e5, 8); playTone(e5, 4); playTone(d5, 8);
        playTone(c5, 8); playTone(c5, 4); playTone(b4, 2);
    }

    // --- Nakarat (2 Tekrar) ---
    for(int i = 0; i < 2; i++){
        if (şarkıDurdur) break;
        playTone(0, 40);
        playTone(e5, 8); playTone(e5, 4); playTone(e5, 8); playTone(e5, 4);
        playTone(g5, 4); playTone(g5, 8); playTone(fd5, 4);
        playTone(e5, 8); playTone(fd5, 8); playTone(e5, 8);
        playTone(d5, 4); playTone(d5, 8); playTone(d5, 8);
        playTone(d5, 4); playTone(d5, 4); playTone(fd5, 4);
        playTone(fd5, 8); playTone(e5, 8); playTone(e5, 8);
        playTone(d5, 8); playTone(e5, 8); playTone(d5, 8);
        playTone(c5, 4); playTone(c5, 8); playTone(c5, 8);
        playTone(c5, 8); playTone(c5, 8); playTone(e5, 4);
        playTone(e5, 8); playTone(e5, 8); playTone(fd5, 4);
        playTone(d5, 4); playTone(c5, 4); playTone(a5, 4);
        playTone(fd5, 4); playTone(d5, 8); playTone(d5, 8);
        playTone(c5, 4); playTone(e5, 4); playTone(c5, 8);
        playTone(b4, 8); playTone(b4, 2);
    }
}

void theALaMenthe() {
  s4 = bpm(100); 
  şarkıDurdur = false;

  // --- ANA MELODİ (4 TEKRAR) ---
  for (int i = 0; i < 4; i++) {
    if (şarkıDurdur) break;
    playTone(cd5, 169); playTone(dd5, 169); playTone(e5, 169); playTone(fd5, 32);
    playTone(gd5, 329); playTone(fd5, 32);  playTone(e5, 32);  playTone(fd5, 323);
    playTone(e5, 323);  playTone(dd5, 323); playTone(e5, 169); playTone(fd5, 32);
    playTone(gd5, 32);  playTone(fd5, 169); playTone(e5, 32);  playTone(fd5, 32);
    playTone(e5, 16);   playTone(fd5, 32);  playTone(gd5, 329); playTone(fd5, 32);
    playTone(e5, 32);   playTone(fd5, 323); playTone(e5, 323); playTone(dd5, 323);
    playTone(e5, 169);  playTone(dd5, 32);  playTone(e5, 32);
    
    playTone(fd5, 32);  playTone(gd5, 32);  playTone(fd5, 32);  playTone(e5, 32);
    playTone(dd5, 32);  playTone(e5, 32);   playTone(dd5, 32);  playTone(cd5, 32);
    playTone(dd5, 32);  playTone(e5, 32);   playTone(cd5, 323); playTone(dd5, 323);
    playTone(e5, 323);  playTone(c5, 169);  playTone(cd5, 323); playTone(dd5, 323);
    playTone(cd5, 323); playTone(c5, 169);  playTone(cd5, 169); playTone(dd5, 169);
    playTone(e5, 32);   playTone(fd5, 329); playTone(e5, 32);   playTone(dd5, 32);
    playTone(e5, 64);   playTone(dd5, 64);  playTone(cd5, 64);  playTone(dd5, 64);
    playTone(c5, 169);  playTone(e5, 32);   playTone(f5, 329);
  }

  // --- ARA BÖLÜM (2 TEKRAR) ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) break;
    playTone(cd4, 89);  playTone(dd4, 89);  playTone(e4, 89);   playTone(fd4, 16);
    playTone(gd4, 169); playTone(0, 1630);  playTone(fd4, 163); playTone(e4, 163);
    playTone(fd4, 32);  playTone(e4, 32);   playTone(dd4, 32);  playTone(e4, 32);
    playTone(e4, 8);    playTone(fd4, 16);  playTone(gd4, 16);  playTone(fd4, 8);
    playTone(e4, 16);   playTone(fd4, 16);  playTone(e4, 169);  playTone(e4, 16);
    playTone(fd4, 16);  playTone(gd4, 169); playTone(0, 1630);  playTone(fd4, 163);
    playTone(e4, 163);  playTone(fd4, 32);  playTone(e4, 32);   playTone(dd4, 32);
    playTone(e4, 32);   playTone(e4, 8);    playTone(dd4, 16);  playTone(e4, 16);
    playTone(fd4, 16);  playTone(gd4, 16);  playTone(fd4, 16);  playTone(e4, 16);
    playTone(dd4, 16);  playTone(e4, 16);   playTone(dd4, 16);  playTone(cd4, 16);
    playTone(0, 1630);  playTone(dd4, 163); playTone(e4, 163);  playTone(dd4, 163);
    playTone(cd4, 163); playTone(dd4, 163); playTone(c4, 8);    playTone(cd4, 163);
    playTone(dd4, 163); playTone(cd4, 163118); playTone(cd4, 89); playTone(dd4, 89);
    playTone(e4, 16);   playTone(fd4, 16);  playTone(0, 1630);  playTone(e4, 163);
    playTone(dd4, 163); playTone(e4, 163);  playTone(dd4, 163); playTone(cd4, 163);
    playTone(dd4, 8);   playTone(e4, 16);   playTone(fd4, 16);
  }
}

void anlamazdin() {
    s4 = bpm(105); // Şarkı temposu
    şarkıDurdur = false;

 // --- Giriş Melodisi ---
  playTone(0, 40); // 1
  playTone(a4, 4); 
  playTone(b4, 4);
  playTone(c5, 4);
  playTone(d5, 45); // 2
  playTone(cd5, 8);
  playTone(d5, 8);
  playTone(e5, 8);
  playTone(d5, 8);
  playTone(cd5, 8);
  playTone(d5, 45); // 3
  playTone(cd5, 8);
  playTone(d5, 8);
  playTone(e5, 8);
  playTone(e5, 8);
  playTone(d5, 8);
  playTone(c5, 45); // 4
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(d5, 8);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(c5, 45); // 5
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(d5, 8);
  playTone(d5, 8);
  playTone(c5, 8);
  playTone(b4, 45); // 6
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(a4, 8);
  playTone(b4, 45); // 7
  playTone(a4, 8);
  playTone(gd4, 8);
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(a4, 1); // 8

  // --- Sözler Bölümü 1 ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) break;
    playTone(0, 40); // 9
    playTone(a4, 8);
    playTone(b4, 8);
    playTone(c5, 8);
    playTone(a4, 8);
    playTone(b4, 8);
    playTone(a4, 8);
    if(i == 0){
      playTone(e5, 4); // 10
      playTone(d5, 25);
      playTone(0, 40); // 11
      playTone(gd4, 8);
      playTone(a4, 8);
      playTone(b4, 8);
      playTone(gd4, 8);
      playTone(b4, 8);
      playTone(gd4, 8);
      playTone(d5, 4); // 12 
      playTone(c5, 25);
    }
  }

  playTone(b4, 2); // 14
  playTone(0, 80);
  playTone(e4, 8);
  playTone(c5, 8);
  playTone(a4, 8);
  playTone(b4, 25); // 15
  playTone(b4, 8);
  playTone(gd4, 8);
  playTone(b4, 4); // 16
  playTone(a4, 25);

  // --- Nakarat ---
  playTone(0, 40); // 17
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(a4, 8);
  playTone(e5, 4); // 18
  playTone(d5, 2); 
  playTone(0, 20); // 19
  playTone(gd4, 8);
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(gd4, 8);
  playTone(b4, 8);
  playTone(gd4, 8);
  playTone(d5, 4); // 20
  playTone(c5, 2); 
  playTone(0, 20); // 21
  playTone(a4, 8);
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(e4, 8);
  playTone(c5, 8);
  playTone(a4, 8);
  playTone(b4, 2); // 22
  playTone(0, 80);
  playTone(e4, 8);
  playTone(c5, 8);
  playTone(a4, 8);
  playTone(b4, 25); // 23
  playTone(b4, 8);
  playTone(gd4, 8);
  playTone(b4, 4); // 24
  playTone(a4, 2);
  playTone(0, 40);

  // --- Sözler Bölümü 2 / Final ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) break;
    playTone(0, 40); // 25
    playTone(a4, 4); 
    playTone(b4, 4);
    playTone(c5, 4);
    playTone(d5, 45); // 26
    playTone(cd5, 8);
    playTone(d5, 8);
    playTone(e5, 8);
    playTone(d5, 8);
    playTone(cd5, 8);
    playTone(d5, 45); // 27
    playTone(cd5, 8);
    playTone(d5, 8);
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(d5, 8);
    playTone(c5, 45); // 28
    playTone(b4, 8);
    playTone(c5, 8);
    playTone(d5, 8);
    playTone(c5, 8);
    playTone(b4, 8);
    playTone(c5, 45); // 29
    playTone(b4, 8);
    playTone(c5, 8);
    playTone(d5, 8);
    playTone(d5, 8);
    playTone(c5, 8);
    playTone(b4, 45); // 30
    playTone(a4, 8);
    playTone(b4, 8);
    playTone(c5, 8);
    playTone(b4, 8);
    playTone(a4, 8);
    playTone(b4, 45); // 31
    playTone(a4, 8);
    playTone(gd4, 8);
    playTone(a4, 8);
    playTone(b4, 8);
    playTone(c5, 8);
    playTone(a4, 1); // 32
  }
}

void benTabiiKi() {
  s4 = bpm(140); // Parçanın orijinal temposu
  şarkıDurdur = false;

  // --- GİRİŞ / INTRO ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) break;
    playTone(0, 80);  // 1
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(a5, 16);
    playTone(b5, 16);
    playTone(a5, 16);
    playTone(g5, 16);
    playTone(a5, 4);
    playTone(0, 80); // 2
    playTone(d5, 8);
    playTone(d5, 8);
    playTone(d5, 8);
    playTone(g5, 16);
    playTone(a5, 16);
    playTone(g5, 16);
    playTone(fd5, 16);
    playTone(g5, 4);
    playTone(0, 80); // 3
    playTone(c5, 8);
    playTone(c5, 8);
    playTone(c5, 8);
    playTone(e5, 16);
    playTone(f5, 16);
    playTone(e5, 16);
    playTone(d5, 16);
    playTone(e5, 4);
    playTone(0, 80); // 4
    playTone(e5, 4);
    playTone(c5, 8);
    playTone(d5, 4);
    playTone(c5, 8);
    playTone(b4, 8);
  }
  playTone(b4, 8); // 5
  playTone(0, 250);
  playTone(0, 40); // 6
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(d5, 8);
  playTone(d5, 16);
  playTone(e5, 16);
  playTone(d5, 8);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(a4, 8); // 7

  // --- BÖLÜM 1 (Kaşları kara...) ---
  playTone(e5, 4);
  playTone(e5, 8);
  playTone(gd5, 4);
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(0, 80); // 8
  playTone(e5, 4);
  playTone(e5, 8);
  playTone(gd5, 4);
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(0, 80); // 9
  playTone(dd5, 8);
  playTone(e5, 4);
  playTone(dd5, 8);
  playTone(e5, 4);
  playTone(dd5, 8); // 10
  playTone(e5, 44);
  playTone(c5, 2);
  playTone(0, 40); // 11
  playTone(b4, 88);
  playTone(c5, 8);
  playTone(d5, 4);
  playTone(c5, 88);
  playTone(b4, 8);
  playTone(a4, 2); // 12
  playTone(0, 20);
  playTone(a4, 8); // 13

  // --- BÖLÜM 2 (Saçları belam...) ---
  playTone(e5, 4);
  playTone(e5, 8);
  playTone(gd5, 4);
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(0, 80); // 14
  playTone(e5, 4);
  playTone(e5, 8);
  playTone(gd5, 4);
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(0, 40); // 15
  playTone(dd5, 8);
  playTone(e5, 4);
  playTone(dd5, 8);
  playTone(e5, 4);
  playTone(0, 40); // 16
  playTone(fd5, 4);
  playTone(g5, 2);
  playTone(0, 40); // 17
  playTone(b4, 8);
  playTone(c5, 8);
  playTone(d5, 44);
  playTone(c5, 88);
  playTone(b4, 8);
  playTone(a4, 2); // 18
  playTone(0, 20);

  // --- ARA GEÇİŞ ---
  playTone(a4, 16); // 19
  playTone(b4, 16);
  playTone(a4, 16);
  playTone(g4, 16);
  playTone(a4, 16);
  playTone(b4, 16);
  playTone(c5, 16);
  playTone(d5, 16);
  playTone(c5, 16);
  playTone(d5, 16);
  playTone(c5, 16);
  playTone(b4, 16);
  playTone(c5, 16);
  playTone(d5, 16);
  playTone(e5, 16);
  playTone(f5, 161);

  // --- BÖLÜM 3 (Sensiz bu hayat...) ---
  playTone(f5, 4); // 20
  playTone(f5, 44);
  playTone(d5, 4);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(d5, 44); // 21
  playTone(e5, 25);
  playTone(f5, 4); // 22
  playTone(f5, 44);
  playTone(d5, 4);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(d5, 44); // 23
  playTone(e5, 25);
  playTone(f5, 4); // 24
  playTone(f5, 44);
  playTone(d5, 4);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(d5, 44); // 25
  playTone(e5, 45);
  playTone(a4, 8);
  playTone(a4, 4);
  playTone(a4, 4); // 26
  playTone(d5, 2);
  playTone(c5, 8);
  playTone(b4, 8);
  playTone(c5, 8); // 27
  playTone(a4, 2);
  playTone(0, 450);
  playTone(a5, 4); // 28
  playTone(f5, 8);
  playTone(a5, 2);
  playTone(gd5, 88);
  playTone(f5, 2); // 29
  playTone(0, 20);
  playTone(a5, 4); // 30
  playTone(f5, 8);
  playTone(a5, 4);
  playTone(gd5, 88);
  playTone(f5, 2); // 31
  playTone(0, 20);
  playTone(gd5, 88); // 32
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(gd5, 88);
  playTone(f5, 8);
  playTone(e5, 8);
  playTone(gd5, 88);
  playTone(f5, 8);
  playTone(e5, 4); // 33
  playTone(0, 250);

  // --- NAKARAT (Seni bu dünyada...) ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) break;
    playTone(0, 80); // 34
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(a5, 4);
    playTone(a5, 4);
    playTone(0, 80); // 35
    playTone(d5, 45);
    playTone(g5, 4);
    playTone(g5, 4);
    playTone(0, 80); // 36
    playTone(c5, 4);
    playTone(c5, 8);
    playTone(f5, 88);
    playTone(e5, 88);
    playTone(f5, 4);
    playTone(0, 80); // 37
    playTone(e5, 4);
    playTone(c5, 8);
    playTone(d5, 4);
    playTone(c5, 88);
    playTone(b4, 88);
    playTone(a4, 8); // 38
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(e5, 8);
    playTone(a5, 4);
    playTone(a5, 8);
    playTone(a5, 4); // 39
    playTone(d5, 4);
    playTone(d5, 8);
    playTone(g5, 4);
    playTone(g5, 4);
    playTone(c5, 4); // 40
    playTone(c5, 8);
    playTone(f5, 8);
    playTone(f5, 2);
    playTone(0, 80); // 41
    playTone(e5, 4);
    playTone(c5, 8);
    playTone(d5, 4);
    playTone(c5, 88);
    playTone(b4, 8);
  }

  // --- FİNAL ---
  playTone(0, 80);
  playTone(e5, 4);
  playTone(c5, 8);
  playTone(d5, 4);
  playTone(c5, 88);
  playTone(b4, 8);
}

void fikriminInceGulu() {
  s4 = bpm(158); 
  şarkıDurdur = false;

  // --- GİRİŞ MELODİSİ 
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 2; j++){
      if (şarkıDurdur) return;
      playTone(d5, 4); playTone(e5, 4); playTone(f5, 4);
      playTone(e5, 25); 
    }
    if (şarkıDurdur) return;
    playTone(e5, 8); playTone(d5, 8); playTone(c5, 8); playTone(b4, 8);
    playTone(a4, 4); playTone(g4, 4); playTone(a4, 4); playTone(b4, 4);
    playTone(g4, 8); playTone(a4, 8); playTone(g4, 8); playTone(f4, 8);
    playTone(f4, 8); playTone(e4, 8); playTone(e4, 25);
  }

  // --- 1. SÖZ KISMI (Fikrimin ince gülü...) ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) return;
    playTone(d5, 2);   playTone(e5, 4);   playTone(e5, 4); 
    playTone(e5, 2);   playTone(d5, 4);   playTone(e5, 4); playTone(f5, 4);
    playTone(e5, 25);  playTone(d5, 4);   playTone(e5, 4); playTone(f5, 4);
    playTone(f5, 8);   playTone(e5, 8);   playTone(e5, 4); playTone(d5, 4);
    playTone(d5, 4);   playTone(c5, 4);   playTone(c5, 8); playTone(b4, 8);
    playTone(c5, 25);
  }

  // --- 2. SÖZ KISMI (O gün ki gördüm seni...) ---
  for(int i = 0; i < 2; i++){
    if (şarkıDurdur) return;
    playTone(b4, 4);   playTone(c5, 2);   playTone(c5, 8); 
    playTone(d5, 8);   playTone(b4, 4);   playTone(a4, 4);
    playTone(a4, 4);   playTone(g4, 4);   playTone(a4, 4);
    playTone(b4, 8);   playTone(a4, 8);   playTone(g4, 8);
    playTone(f4, 8);   playTone(e4, 8);   playTone(d4, 8);
    playTone(f4, 2);   playTone(f4, 4);   playTone(f4, 4); 
    playTone(f4, 8);   playTone(e4, 8);   playTone(d4, 4);
    playTone(g4, 4);   playTone(f4, 4);   playTone(f4, 8);
    playTone(e4, 8);   playTone(e4, 25);
  }

  // --- FİNAL (Tekrar giriş melodisiyle bitiş) ---
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 2; j++){
      if (şarkıDurdur) return;
      playTone(d5, 4); playTone(e5, 4); playTone(f5, 4);
      playTone(e5, 25); 
    }
    if (şarkıDurdur) return;
    playTone(e5, 8); playTone(d5, 8); playTone(c5, 8); playTone(b4, 8);
    playTone(a4, 4); playTone(g4, 4); playTone(a4, 4); playTone(b4, 4);
    playTone(g4, 8); playTone(a4, 8); playTone(g4, 8); playTone(f4, 8);
    playTone(f4, 8); playTone(e4, 8); playTone(e4, 25);
  }
}

void birDerdimVar() {
    s4 = bpm(120); // Şarkı temposu
    şarkıDurdur = false;

    // --- Intro (2 Tekrar) ---
    for(int i = 0; i < 2; i++){
        if (şarkıDurdur) break;
        if(i == 1) playTone(b4, 8);
        playTone(fd5, 8); playTone(b5, 8); playTone(cd6, 8);
        playTone(d6, 8); playTone(cd6, 8); playTone(b5, 8); playTone(fd5, 8);
        playTone(e6, 8); playTone(d6, 8); playTone(cd6, 8); playTone(b5, 8);
        playTone(fd6, 8); playTone(e6, 8); playTone(d6, 8); playTone(cd6, 8);
        
        for(int j = 0; j < 2; j++){
            if (şarkıDurdur) break;
            playTone(d6, 8); playTone(g5, 8); playTone(fd5, 8); playTone(g5, 8);
        }
        
        playTone(cd6, 8); playTone(g5, 8); playTone(b5, 8); playTone(g5, 8);
        playTone(a5, 8); playTone(g5, 8); playTone(fd5, 8); playTone(g5, 8);
    }

    // --- 1. Söz Kısmı (Bir derdim var artık...) ---
    if (!şarkıDurdur) {
        playTone(fd4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(a4, 4); playTone(g4, 4);
        playTone(fd4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(cd5, 4); playTone(d5, 4);
        
        playTone(e4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(a4, 4); playTone(g4, 4);
        playTone(fd4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(cd5, 4);
        playTone(d5, 8); playTone(cd5, 8);
        playTone(d5, 2); playTone(d5, 2);
        
        playTone(cd5, 4); playTone(b4, 4); playTone(a4, 4);
        playTone(g4, 8); playTone(fd4, 8);
        playTone(g4, 2); playTone(d5, 2);
        
        playTone(cd5, 4);
        playTone(b4, 8); playTone(a4, 8); playTone(a4, 8);
        playTone(g4, 8); playTone(g4, 8); playTone(fd4, 8);
    }

    // --- Ara Geçiş (Arpejli Kısım) ---
    for(int i = 0; i < 3; i++){
        if (şarkıDurdur) break;
        playTone(b4, 8); playTone(fd5, 8); playTone(b5, 8); playTone(cd6, 8);
        playTone(d6, 8); playTone(cd6, 8); playTone(b5, 8); playTone(fd5, 8);
    }

    // --- 2. Söz Kısmı (Bak bu son perde...) ---
    if (!şarkıDurdur) {
        playTone(d5, 2); playTone(fd5, 2);
        playTone(e5, 4); playTone(d5, 4); playTone(cd5, 4); playTone(d5, 4);
        playTone(cd5, 2); playTone(fd5, 2);
        playTone(e5, 4); playTone(d5, 4); playTone(cd5, 4); playTone(d5, 4);
        playTone(cd5, 2); playTone(fd5, 2);
        playTone(e5, 4); playTone(d5, 4); playTone(cd5, 4); playTone(d5, 4);
        
        playTone(cd5, 2); playTone(cd5, 2);
        playTone(cd5, 2); playTone(cd5, 2);
    }

    // --- Nakarat ---
    if (!şarkıDurdur) {
        playTone(g4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(g4, 4); playTone(d5, 4); playTone(cd5, 4);
        playTone(b4, 1);
        playTone(0, 40);
        
        playTone(b4, 4); playTone(cd5, 4); playTone(d5, 4);
        playTone(cd5, 1);
        playTone(0, 40);
        
        playTone(cd5, 4); playTone(d5, 4); playTone(e5, 4);
        playTone(d5, 1);
        
        playTone(e5, 4); playTone(e5, 4); playTone(fd5, 4); playTone(g5, 4);
        playTone(fd5, 15);
        playTone(0, 20);
        
        playTone(g4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(a4, 4); playTone(g4, 4);
        playTone(g4, 2); playTone(d5, 2);
        playTone(cd5, 4); playTone(b4, 4); playTone(cd5, 4); playTone(d5, 4);
        
        playTone(cd5, 25);
        playTone(b4, 45);
    }
}