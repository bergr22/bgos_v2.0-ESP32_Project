// TETRIS APP

// --- tetris.ino ---
// bgOS Tetris Entegrasyonu - Düzenlenmiş ve Stabil Versiyon

// 1. GLOBAL DEĞİŞKENLER VE TANIMLAMALAR
const char pieces_S_l[2][2][4] = {{{0, 0, 1, 1}, {0, 1, 1, 2}}, {{0, 1, 1, 2}, {1, 1, 0, 0}}};
const char pieces_S_r[2][2][4] = {{{1, 1, 0, 0}, {0, 1, 1, 2}}, {{0, 1, 1, 2}, {0, 0, 1, 1}}};
const char pieces_L_l[4][2][4] = {{{0, 0, 0, 1}, {0, 1, 2, 2}}, {{0, 1, 2, 2}, {1, 1, 1, 0}}, {{0, 1, 1, 1}, {0, 0, 1, 2}}, {{0, 0, 1, 2}, {1, 0, 0, 0}}};
const char pieces_Sq[1][2][4] = {{{0, 1, 0, 1}, {0, 0, 1, 1}}};
const char pieces_T[4][2][4] = {{{0, 0, 1, 0}, {0, 1, 1, 2}}, {{0, 1, 1, 2}, {1, 0, 1, 1}}, {{1, 0, 1, 1}, {0, 1, 1, 2}}, {{0, 1, 1, 2}, {0, 0, 1, 0}}};
const char pieces_l[2][2][4] = {{{0, 1, 2, 3}, {0, 0, 0, 0}}, {{0, 0, 0, 0}, {0, 1, 2, 3}}};

const short MARGIN_TOP = 19;
const short MARGIN_LEFT = 3;
const short SIZE = 5;
const short TYPES = 6;

word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];
int interval = 400, score;
unsigned long tetrisTimer;
boolean grid[10][18];
// song.ino ile çakışmaması için t1, t2, t3 olarak adlandırıldı
boolean t1 = true, t2 = true, t3 = true; 

// 2. YARDIMCI FONKSİYONLAR
void copyPiece(short targetPiece[2][4], short type, short rot) {
  switch (type) {
    case 0: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_L_l[rot][0][i]; targetPiece[1][i] = pieces_L_l[rot][1][i]; } break;
    case 1: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_S_l[rot][0][i]; targetPiece[1][i] = pieces_S_l[rot][1][i]; } break;
    case 2: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_S_r[rot][0][i]; targetPiece[1][i] = pieces_S_r[rot][1][i]; } break;
    case 3: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_Sq[0][0][i]; targetPiece[1][i] = pieces_Sq[0][1][i]; } break;
    case 4: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_T[rot][0][i]; targetPiece[1][i] = pieces_T[rot][1][i]; } break;
    case 5: for (int i = 0; i < 4; i++) { targetPiece[0][i] = pieces_l[rot][0][i]; targetPiece[1][i] = pieces_l[rot][1][i]; } break;
  }
}

boolean nextHorizontalCollision(short p[2][4], int amount) {
  for (short i = 0; i < 4; i++) {
    short newX = pieceX + p[0][i] + amount;
    if (newX > 9 || newX < 0 || grid[newX][pieceY + p[1][i]]) return true;
  }
  return false;
}

boolean nextCollision() {
  for (short i = 0; i < 4; i++) {
    short y = pieceY + piece[1][i] + 1;
    short x = pieceX + piece[0][i];
    if (y > 17 || grid[x][y]) return true;
  }
  return false;
}

void generate() {
  currentType = nextType;
  nextType = random(TYPES);
  pieceX = (currentType != 5) ? random(9) : random(7);
  pieceY = 0;
  rotation = 0;
  copyPiece(piece, currentType, rotation);
}

void drawGrid() {
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 18; y++)
      if (grid[x][y]) u8g2.drawBox(MARGIN_LEFT + (SIZE + 1) * x, MARGIN_TOP + (SIZE + 1) * y, SIZE, SIZE);
}

void drawPiece() {
  for (short i = 0; i < 4; i++)
    u8g2.drawBox(MARGIN_LEFT + (SIZE + 1) * (pieceX + piece[0][i]), MARGIN_TOP + (SIZE + 1) * (pieceY + piece[1][i]), SIZE, SIZE);
}

void drawLayout() {
  u8g2.drawLine(0, 15, 64, 15);
  u8g2.drawFrame(0, 0, 64, 128);
  short nPiece[2][4];
  copyPiece(nPiece, nextType, 0);
  for (short i = 0; i < 4; i++) u8g2.drawBox(45 + 3 * nPiece[0][i], 4 + 3 * nPiece[1][i], 2, 2);
  u8g2.setFont(u8g2_font_04b_03_tr);
  u8g2.setCursor(5, 10);
  u8g2.print("SCORE: "); u8g2.print(score);
}

void breakLine(short line) {
  if(sesAcik) tone(BUZZER_PIN, 800, 50);
  for (short y = line; y >= 1; y--)
    for (short x = 0; x < 10; x++) grid[x][y] = grid[x][y - 1];
  for (short x = 0; x < 10; x++) grid[x][0] = 0;
  score += 10;
}

// 3. ANA UYGULAMA DÖNGÜSÜ
void runTetrisApp() {
  inApp = true;
  u8g2.setDisplayRotation(U8G2_R1); // Ekranı dikey yap
  
  // Resetleme işlemleri
  score = 0;
  for (int x = 0; x < 10; x++) for (int y = 0; y < 18; y++) grid[x][y] = 0;
  randomSeed(analogRead(0));
  nextType = random(TYPES);
  generate();
  tetrisTimer = millis();

  while(inApp) {
    u8g2.clearBuffer();
    drawLayout();
    drawGrid();
    drawPiece();
    u8g2.sendBuffer();

    // Oyun Mantığı
    if (millis() - tetrisTimer > interval) {
      for (short y = 17; y >= 0; y--) {
        bool full = true;
        for (short x = 0; x < 10; x++) full = full && grid[x][y];
        if (full) { breakLine(y); y++; }
      }

      if (nextCollision()) {
        for (short i = 0; i < 4; i++) grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
        if (pieceY <= 1) { // GAME OVER
           if(sesAcik) tone(BUZZER_PIN, 200, 500);
           delay(1000);
           inApp = false;
        }
        generate();
      } else {
        pieceY++;
      }
      tetrisTimer = millis();
    }

    // --- BUTON KONTROLLERİ --- //ekran yan olduğu için butonların doğruları yanında yazıyo
    if (!digitalRead(btnUp)) {  //btnLeft
      if (t1 && !nextHorizontalCollision(piece, -1)) { pieceX--; if(sesAcik) tone(BUZZER_PIN, 1200, 10); }
      t1 = false;
    } else t1 = true;

    if (!digitalRead(btnDown)) { // btnRight
      if (t2 && !nextHorizontalCollision(piece, 1)) { pieceX++; if(sesAcik) tone(BUZZER_PIN, 1200, 10); }
      t2 = false;
    } else t2 = true;

    if (!digitalRead(btnRight)) {  //çevirme  btnUp
      if (t3) {
        short maxRot = (currentType == 1 || currentType == 2 || currentType == 5) ? 2 : (currentType == 3 ? 1 : 4);
        short nextRot = (rotation + 1) % maxRot;
        short testPiece[2][4];
        copyPiece(testPiece, currentType, nextRot);
        if (!nextHorizontalCollision(testPiece, 0)) { rotation = nextRot; copyPiece(piece, currentType, rotation); }
        if(sesAcik) tone(BUZZER_PIN, 1500, 10);
      }
      t3 = false;
    } else t3 = true;

    interval = (!digitalRead(btnLeft)) ? 40 : 400; //btnDown

    // --- ÇIKIŞ KONTROLÜ (hPin / Pin 40) ---
    if (!digitalRead(hPin)) { 
      inApp = false;
      delay(300); 
    }
  }

  // Oyundan Çıkış ve Ekranı Eski Haline Getirme
  u8g2.setDisplayRotation(U8G2_R0); 
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  if(sesAcik) tone(BUZZER_PIN, 800, 100);
}