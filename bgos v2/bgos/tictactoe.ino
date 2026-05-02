// TICTACTOE APP 

/****************************************
this project made by @bergrfpv 
****************************************/

#include <Arduino.h>
#include <U8g2lib.h>

// Ana dosyadan (bgos.ino) gelecek donanım nesneleri, pinler ve değişkenler
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern const int btnRight;
extern const int btnLeft;
extern const int btnUp;
extern const int btnDown;
extern const int btnSelect;
extern const int hPin;
extern bool inApp; // 

// Global Değişkenler
static int board_ttt[9];       // 0: Empty, 1: O (P1), 2: X (P2/CPU)
static int gameMode_ttt = 1;   // 1: 1 Player (vs CPU), 2: 2 Players
static int currentPlayer_ttt;  // 1: P1, 2: P2/CPU
static int gameState_ttt;      // 0: Mode Menu, 1: Playing, 2: Game Over
static int winner_ttt;         // 0: None, 1: P1, 2: P2/CPU, 3: Draw

// İmleç (Kürsör) Koordinatları (0, 1, 2)
static int cursorX_ttt = 0;
static int cursorY_ttt = 0;

// --- Yardımcı Fonksiyonlar ---

void resetGame_ttt() {
  for (int i = 0; i < 9; i++) board_ttt[i] = 0;
  currentPlayer_ttt = 1; // Her zaman P1 (O) başlar
  winner_ttt = 0;
  cursorX_ttt = 0;
  cursorY_ttt = 0;
}

void drawMenu_ttt() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(30, 15, "TIC TAC TOE");
  u8g2.drawLine(20, 18, 108, 18);

  // Seçili olan menüyü ters renk ile (kutu içinde) göster
  if (gameMode_ttt == 1) {
    u8g2.drawBox(10, 25, 108, 12);
    u8g2.setDrawColor(0);
    u8g2.drawStr(15, 34, "1 PLAYER (vs CPU)");
    u8g2.setDrawColor(1);
    u8g2.drawStr(15, 49, "2 PLAYERS");
  } else {
    u8g2.drawStr(15, 34, "1 PLAYER (vs CPU)");
    u8g2.drawBox(10, 40, 108, 12);
    u8g2.setDrawColor(0);
    u8g2.drawStr(15, 49, "2 PLAYERS");
    u8g2.setDrawColor(1);
  }
  
  u8g2.drawStr(25, 63, "Select: START");
  u8g2.sendBuffer();
}

void drawBoard_ttt() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  
  // Izgara Çizimi (Sol taraf 64x64 kare)
  u8g2.drawHLine(0, 21, 64);
  u8g2.drawHLine(0, 42, 64);
  u8g2.drawVLine(21, 0, 64);
  u8g2.drawVLine(42, 0, 64);

  // Hücre İçerikleri (X ve O)
  u8g2.setFont(u8g2_font_ncenB14_tr);
  for (int i = 0; i < 9; i++) {
    int cx = i % 3;
    int cy = i / 3;
    if (board_ttt[i] == 1) u8g2.drawStr(4 + cx * 21, 16 + cy * 21, "O");
    if (board_ttt[i] == 2) u8g2.drawStr(4 + cx * 21, 16 + cy * 21, "X");
  }

  
  u8g2.drawFrame(cursorX_ttt * 21, cursorY_ttt * 21, 22, 22);

  // Sağ Panel - Sıra Bilgisi
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(72, 15, "TURN:");
  
  if (currentPlayer_ttt == 1) {
    u8g2.drawStr(72, 30, "PLAYER 1");
    u8g2.drawStr(85, 45, "( O )");
  } else {
    if (gameMode_ttt == 1) {
      u8g2.drawStr(72, 30, "CPU");
      u8g2.drawStr(85, 45, "( X )");
    } else {
      u8g2.drawStr(72, 30, "PLAYER 2");
      u8g2.drawStr(85, 45, "( X )");
    }
  }

  u8g2.drawStr(68, 62, "EXIT: <");
  u8g2.sendBuffer();
}

void drawGameOver_ttt() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB14_tr);
  
  // Kazanan Mesajı
  if (winner_ttt == 1) {
    u8g2.drawStr(25, 30, "P1 WINS!");
  } else if (winner_ttt == 2) {
    if (gameMode_ttt == 1) u8g2.drawStr(15, 30, "CPU WINS!");
    else u8g2.drawStr(25, 30, "P2 WINS!");
  } else {
    u8g2.drawStr(35, 30, "DRAW!");
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(10, 50, "Select: RESTART");
  u8g2.drawStr(10, 62, "2: MENU");
  u8g2.sendBuffer();
}

void handleHumanMove_ttt() {
  // Yön Butonları Kontrolü
  if (digitalRead(btnRight) == LOW) {
    cursorX_ttt = (cursorX_ttt + 1) % 3;
    while(digitalRead(btnRight) == LOW); delay(20);
  }
  if (digitalRead(btnLeft) == LOW) {
    cursorX_ttt = (cursorX_ttt + 2) % 3; 
    while(digitalRead(btnLeft) == LOW); delay(20);
  }
  if (digitalRead(btnDown) == LOW) {
    cursorY_ttt = (cursorY_ttt + 1) % 3;
    while(digitalRead(btnDown) == LOW); delay(20);
  }
  if (digitalRead(btnUp) == LOW) {
    cursorY_ttt = (cursorY_ttt + 2) % 3; 
    while(digitalRead(btnUp) == LOW); delay(20);
  }

  // Seçim (OK) Butonu Kontrolü
  if (digitalRead(btnSelect) == LOW) {
    int pos = cursorY_ttt * 3 + cursorX_ttt;
    if (board_ttt[pos] == 0) { // Seçilen yer boşsa hamleyi yap
      board_ttt[pos] = currentPlayer_ttt;
      currentPlayer_ttt = (currentPlayer_ttt == 1) ? 2 : 1; // Sırayı değiştir
    }
    while(digitalRead(btnSelect) == LOW); delay(20);
  }
}

int cpuBrain_ttt(int p) {
  const int winPatterns[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}
  };
  for (int i = 0; i < 8; i++) {
    int a = winPatterns[i][0];
    int b = winPatterns[i][1];
    int c = winPatterns[i][2];
    if (board_ttt[a] == p && board_ttt[b] == p && board_ttt[c] == 0) return c;
    if (board_ttt[a] == p && board_ttt[c] == p && board_ttt[b] == 0) return b;
    if (board_ttt[b] == p && board_ttt[c] == p && board_ttt[a] == 0) return a;
  }
  return -1;
}

void handleCPUMove_ttt() {
  delay(600); // CPU biraz düşünsün (gerçekçi hissiyat)

  int move = cpuBrain_ttt(2); // Kazanmaya çalış
  if (move == -1) move = cpuBrain_ttt(1); // Rakibi engelle
  
  if (move == -1) { // Rastgele hamle
    while (true) {
      int r = random(9);
      if (board_ttt[r] == 0) {
        move = r;
        break;
      }
    }
  }
  board_ttt[move] = 2;
  currentPlayer_ttt = 1; // Sırayı Human'a (P1) geri ver
}

void checkWinner_ttt() {
  const int winPatterns[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}
  };

  // Kazanan var mı kontrol et
  for (int i = 0; i < 8; i++) {
    if (board_ttt[winPatterns[i][0]] != 0 &&
        board_ttt[winPatterns[i][0]] == board_ttt[winPatterns[i][1]] &&
        board_ttt[winPatterns[i][0]] == board_ttt[winPatterns[i][2]]) {
      winner_ttt = board_ttt[winPatterns[i][0]];
      gameState_ttt = 2; // Oyun bitti
      return;
    }
  }

  // Beraberlik kontrolü
  bool draw = true;
  for (int i = 0; i < 9; i++) if (board_ttt[i] == 0) draw = false;
  
  if (draw) {
    winner_ttt = 3; // Beraberlik
    gameState_ttt = 2; // Oyun bitti
  }
}

// --- Ana Uygulama Döngüsü ---

void runTicTacToeApp() {
  gameState_ttt = 0; // Uygulama açıldığında menü ile başla
  gameMode_ttt = 1;  // Varsayılan: 1 Player

  // bgOS ana döngüsünde kalmak için inApp kontrolü
  while (inApp) {
    
    // Her an bgOS Ana Menüye Çıkış Kontrolü (hPin)
    if (digitalRead(hPin) == LOW) {
      inApp = false; // bgOS loop'una geri döner
      while(digitalRead(hPin) == LOW); 
      return; 
    }

    // STATE 0: OYUN MODU MENÜSÜ
    if (gameState_ttt == 0) {
      drawMenu_ttt();
      
      // Yukarı / Aşağı ile mod seç
      if (digitalRead(btnDown) == LOW || digitalRead(btnUp) == LOW) {
        gameMode_ttt = (gameMode_ttt == 1) ? 2 : 1;
        while(digitalRead(btnDown) == LOW || digitalRead(btnUp) == LOW); 
        delay(20);
      }
      
      // Select ile oyunu başlat
      if (digitalRead(btnSelect) == LOW) {
        resetGame_ttt();
        gameState_ttt = 1; // Oynanış state'ine geç
        while(digitalRead(btnSelect) == LOW); 
        delay(20);
      }
    } 
    
    // STATE 1: OYUN OYNANIYOR
    else if (gameState_ttt == 1) {
      drawBoard_ttt();
      
      if (currentPlayer_ttt == 2 && gameMode_ttt == 1) {
        // CPU Sırası
        handleCPUMove_ttt();
      } else {
        // Human Sırası (P1 veya P2)
        handleHumanMove_ttt();
      }
      
      checkWinner_ttt(); // Hamle sonrası kazananı kontrol et
    }
    
    // STATE 2: OYUN BİTTİ (KAZANAN EKRANI)
    else if (gameState_ttt == 2) {
      drawGameOver_ttt();
      
      // Select ile aynı modda tekrar başlat (RESTART)
      if (digitalRead(btnSelect) == LOW) {
        resetGame_ttt();
        gameState_ttt = 1; 
        while(digitalRead(btnSelect) == LOW); 
        delay(20);
      }
    }
    
  } 
}