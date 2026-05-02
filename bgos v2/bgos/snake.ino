// SNAKE APP

#define DIR_RIGHT 0
#define DIR_UP    1
#define DIR_LEFT  2
#define DIR_DOWN  3

// Oyun Değişkenleri
int box_x = 0;
int box_y = 0;
int box_x_length = 98;
int box_y_length = 62;
int snake_max_length = 100;
int snake_x[100];
int snake_y[100];
int snake_body_width = 3;
int food_x;
int food_y;
int snake_length = 3;
unsigned int game_speed;
int last_direction = DIR_RIGHT;
bool snakeGameOver = false;

// Yön Okuma (bgOS Butonlarıyla)
int read_key_snake() {
    if (!digitalRead(btnUp)) return DIR_UP;
    if (!digitalRead(btnDown)) return DIR_DOWN;
    if (!digitalRead(btnLeft)) return DIR_LEFT;
    if (!digitalRead(btnRight)) return DIR_RIGHT;
    return -1;
}

// Yemi Rastgele Oluşturma
void food() {
    bool flag = true;
    while (flag) {
        food_x = random(0, (box_x_length - 2) / 3);
        food_y = random(0, (box_y_length - 2) / 3);

        flag = false;
        for (int i = 0; i < snake_length; i++) {
            if ((food_x == snake_x[i]) && (food_y == snake_y[i])) {
                flag = true;
            }
        }
    }
}

// Çarpışma Kontrolü (Duvar)
int snake_knock_wall(int dir) {
    int x_temp = snake_x[0];
    int y_temp = snake_y[0];

    if (dir == DIR_UP) y_temp -= 1;
    if (dir == DIR_DOWN) y_temp += 1;
    if (dir == DIR_LEFT) x_temp -= 1;
    if (dir == DIR_RIGHT) x_temp += 1;

    return (x_temp < 0 || x_temp > 31 || y_temp < 0 || y_temp > 19) ? 1 : 0;
}

// Yem Yeme Kontrolü
int snake_eat_food(int dir) {
    int x_temp = snake_x[0];
    int y_temp = snake_y[0];

    if (dir == DIR_UP) y_temp -= 1;
    if (dir == DIR_DOWN) y_temp += 1;
    if (dir == DIR_LEFT) x_temp -= 1;
    if (dir == DIR_RIGHT) x_temp += 1;

    if ((x_temp == food_x) && (y_temp == food_y)) {
        snake_length += 1;
        snake_x[snake_length - 1] = food_x;
        snake_y[snake_length - 1] = food_y;
        return 1;
    }
    return 0;
}

// Yılanın Pozisyonunu Güncelleme
void change_con(int DIR) {
    int temp_x[100];
    int temp_y[100];

    for (int i = 0; i < snake_length; i++) {
        temp_x[i] = snake_x[i];
        temp_y[i] = snake_y[i];
    }

    if (DIR == DIR_RIGHT) snake_x[0] += 1;
    if (DIR == DIR_LEFT) snake_x[0] -= 1;
    if (DIR == DIR_UP) snake_y[0] -= 1;
    if (DIR == DIR_DOWN) snake_y[0] += 1;

    for (int i = 1; i < snake_length; i++) {
        snake_x[i] = temp_x[i - 1];
        snake_y[i] = temp_y[i - 1];
    }
}

// Oyunu Ekrana Çizme (U8g2 Formatı)
void snake_frame(int s) {
    u8g2.clearBuffer();
    
    // Oyun Çerçevesi
    u8g2.drawFrame(box_x, box_y, box_x_length, box_y_length);
    
    // Skor ve Hız Bilgileri (Sağ Taraf)
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(box_x_length + 2, 10);
    u8g2.print("Score");
    u8g2.setCursor(box_x_length + 10, 20);
    u8g2.print((snake_length - 3) * 5);
    
    u8g2.setCursor(box_x_length + 2, 35);
    u8g2.print("Speed");
    u8g2.setCursor(box_x_length + 6, 45);
    u8g2.print(s);

    // Yemi Çiz
    u8g2.drawFrame(food_x * snake_body_width + 1, food_y * snake_body_width + 1, snake_body_width, snake_body_width);

    // Yılanı Çiz
    for (int i = 0; i < snake_length; i++) {
        if (i == 0) {
            u8g2.drawBox(snake_x[i] * snake_body_width + 1, snake_y[i] * snake_body_width + 1, snake_body_width, snake_body_width);
        } else {
            u8g2.drawFrame(snake_x[i] * snake_body_width + 1, snake_y[i] * snake_body_width + 1, snake_body_width, snake_body_width);
        }
    }
    u8g2.sendBuffer();
}

// ANA UYGULAMA DÖNGÜSÜ (bgOS İçinden Çağrılır)
void runSnakeApp() {
    while (inApp) {
        // --- BAŞLANGIÇ AYARLARI ---
        snake_length = 3;
        snake_x[0] = 15; snake_y[0] = 15;
        snake_x[1] = 14; snake_y[1] = 15;
        snake_x[2] = 13; snake_y[2] = 15;
        last_direction = DIR_RIGHT;
        game_speed = 130;
        snakeGameOver = false;
        food(); // İlk yemi at

        // --- GİRİŞ EKRANI ---
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr(12, 25, "Snake Game");
        u8g2.setFont(u8g2_font_5x7_tf);
        u8g2.drawStr(18, 45, "OK = PLAY");
        u8g2.drawStr(22, 55, "2 = EXIT");
        u8g2.sendBuffer();

        // Kullanıcı giriş yapana kadar bekle
        while (digitalRead(btnSelect) && digitalRead(hPin)) { delay(10); }

        // Eğer HOME tuşuna bastıysa uygulamadan tamamen çık
        if (!digitalRead(hPin)) {
            inApp = false;
            if(sesAcik) tone(BUZZER_PIN, 800, 100);
            delay(250);
            return; 
        }

        delay(200); // SELECT tuşunun parmak çekilme süresi (debounce)
        if(sesAcik) tone(BUZZER_PIN, 1500, 100);

        // --- OYUN İÇİ DÖNGÜ ---
        bool isPlaying = true;
        unsigned long lastMoveTime = millis();
        int current_direction = last_direction;

        while (isPlaying && inApp) {
            // Oyun sırasında HOME'a basılırsa sistemi kilitlemeden çık
            if (!digitalRead(hPin)) {
                inApp = false;
                if(sesAcik) tone(BUZZER_PIN, 800, 100);
                delay(250);
                return;
            }

            // Butonları sürekli dinle (Responsive kontrol için)
            int pressed = read_key_snake();
            if (pressed != -1) {
                // Yılanın kendi içine girmesini engelle
                if ((pressed == DIR_RIGHT && last_direction != DIR_LEFT) ||
                    (pressed == DIR_LEFT && last_direction != DIR_RIGHT) ||
                    (pressed == DIR_UP && last_direction != DIR_DOWN) ||
                    (pressed == DIR_DOWN && last_direction != DIR_UP)) {
                    current_direction = pressed;
                }
            }

            // Sadece game_speed süresi dolduğunda yılanı hareket ettir
            if (millis() - lastMoveTime >= game_speed) {
                lastMoveTime = millis();
                last_direction = current_direction;

                if (snake_eat_food(last_direction)) {
                    food();

                    //HIZ
                    game_speed = (snake_length < 10) ? 130 : (snake_length < 30) ? 125 : (snake_length < 50) ? 120 : 120;
                    if(sesAcik) tone(BUZZER_PIN, 2000, 30); // Yem yeme sesi
                } 
                else if (snake_knock_wall(last_direction)) {
                    isPlaying = false; // Duvara çarptı, oyun bitti
                    snakeGameOver = true;
                    if(sesAcik) tone(BUZZER_PIN, 300, 500); // Ölüm sesi
                } 
                else {
                    change_con(last_direction);
                }

                // Ölmediysek yeni kareyi çiz
                if(isPlaying) {
                    snake_frame(game_speed);
                }
            }
        }

        // --- GAME OVER EKRANI ---
        if (snakeGameOver) {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_ncenB10_tr);
            u8g2.drawStr(15, 25, "Game Over");
            
            u8g2.setFont(u8g2_font_5x7_tf);
            u8g2.setCursor(45, 40);
            u8g2.print("Score: ");
            u8g2.print((snake_length - 3) * 5);
            
            u8g2.drawStr(12, 55, "OK: RETRY | 2: EXIT");
            u8g2.sendBuffer();

            // Yeniden denemek veya çıkmak için giriş bekle
            while (digitalRead(btnSelect) && digitalRead(hPin)) { delay(10); }

            if (!digitalRead(hPin)) {
                inApp = false;
                if(sesAcik) tone(BUZZER_PIN, 800, 100);
                delay(250);
                return;
            }
            // SELECT'e basıldıysa while(inApp) döngüsü başa saracak ve oyun sıfırlanacak.
            delay(200); 
        }
    }
}