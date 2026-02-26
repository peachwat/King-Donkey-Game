#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SPRITE_SIZE 20
#define MAN_HEIGHT 16
#define BARREL_HEIGHT 10

typedef struct {
    double hDistance;
    double vDistance;
    double hSpeed;
    double vSpeed;
} ItemsData;

typedef struct {
    double value;
    double animationSpeed;
    double itemSpeed;
} AnimationData;

typedef struct {
    SDL_Surface* first;
    SDL_Surface* second;
} Sprite;
 
int InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screen, SDL_Texture** scrtex) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, window, renderer);

    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(*window, "King Donkey Evelina Rylova 2024");

    *screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    *scrtex = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);

    return 0;
}

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
    SDL_Surface* charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    };
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
    SDL_Rect dest;
    dest.x = x - sprite->w / 2;
    dest.y = y - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32*)p = color;
};

// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
    for (int i = 0; i < l; i++) {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};

// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
    Uint32 outlineColor, Uint32 fillColor) {
    int i;
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void DrawOutline(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor) {
    DrawLine(screen, x, y, k - 2, 0, 1, outlineColor); // left
    DrawLine(screen, x + l - 1, y, k - 2, 0, 1, outlineColor); // right
    DrawLine(screen, x, y, l, 1, 0, outlineColor); // top
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor); // bottom
}

void Lives(SDL_Surface* screen, SDL_Surface* charset, int lives) {
    if (lives < 0) {
        lives = 0;
    }
    char hearts[4];
    for (int i = 0; i < lives; ++i) {
        hearts[i] = '\x03';
    }
    hearts[lives] = '\0';

    DrawString(screen, 6, 46, hearts, charset);
}

void DrawLevel1(const char* level, SDL_Surface* screen, SDL_Surface** sprites, SDL_Surface* charset, int lives) {

    //printf("%i\n", strlen(level));
    int x = 0, y = 0;
    for (int i = 0; i < strlen(level); ++i) {
        if (level[i] == '0' || level[i] == '3') {
            DrawSurface(screen, sprites[0], x + SPRITE_SIZE / 2, y + SPRITE_SIZE / 2);
        }
        else if (level[i] == '1') {
            DrawSurface(screen, sprites[1], x + SPRITE_SIZE / 2, y + SPRITE_SIZE / 2);
        }
        else if (level[i] == '2') {
            DrawSurface(screen, sprites[2], x + SPRITE_SIZE / 2, y + SPRITE_SIZE / 2);
        }

        x += SPRITE_SIZE;
        if (x >= SCREEN_WIDTH) {
            x = 0;
            y += SPRITE_SIZE;
        }
    }
    Lives(screen, charset, lives);
}

const char* Level1() {
    return "                                "
        "                                "
        "                                "
        "                                "//5
        "         2        4             "
        "00000000000000000030            "
        "                  1             "
        "            4     1             "
        "     000000030000000            "//10
        "            1                   "
        "        4   1                   "
        "00000000300000        00000000  "
        "        1                       "
        "        1            4          "
        "    0000000000000000030         "//15
        "                     1          "
        "    4                1          "
        "  0030000000000000000000        "
        "    1                           "
        "    1       4                   "//20
        "   000000000300000000           "
        "            1                   "
        "            1                   "
        "00000000000000000000000000000000";
}

const char* Level2() {
    return "                                "
        "                                "
        "                                "
        "                                "//5
        "        4      2                "
        "00000000300  00000              "
        "        1                       "
        "        1                   4   "
        "     0000000000 00000000000030  "//10
        "                            1   "
        "                            1   "
        "000                4   000000000"
        "                000300          "
        "      4            1            "//15
        "    00300  000000000000         "
        "      1                         "
        "      1         4               "
        "  00000000000000300             "
        "                1               "
        "                1 4             "//20
        "                00300           "
        "                  1             "
        "                  1             "
        "00000000000000000000000000000000";
}

const char* Level3() {
    return "                                "
        "                                "
        "                                "
        "                                "//5
        "    2        4                  "
        "000000 000  030                 "
        "             1                  "
        "        4    1                  "
        "     000300000000000            "//10
        "        1                       "
        "        1           4           "
        "000000000000000 0 0030000       "
        "                    1           "
        "     4              1           "//15
        "    0300 0 0000000000000        "
        "     1                          "
        "     1           4              "
        "  0000000000 0 003000000        "
        "                 1              "
        "                 1              "//20
        "                 1              "
        "            000000000           "
        "                                "
        "00000000000000000000000000000000";
}

void DrawDragon(SDL_Surface* screen, Sprite dragon, AnimationData dragonAnimation, int x, int y) {
    if (dragonAnimation.value >= 0 && dragonAnimation.value < 50) {
        DrawSurface(screen, dragon.first, x, y);
    }
    else DrawSurface(screen, dragon.second, x, y);
}

void DrawBarrels(SDL_Surface* screen, Sprite barrel, AnimationData barrelAnimation, int x, int y) {

    if (barrelAnimation.value >= 0 && barrelAnimation.value < 50) {
        DrawSurface(screen, barrel.first, x, y);
    }
    else {
        DrawSurface(screen, barrel.second, x, y);
    }
}

void DrawAntagonista(SDL_Surface* screen, Sprite antagonista, AnimationData antagonistaAnimation, int x, int y) {
    if (antagonistaAnimation.value >= 0 && antagonistaAnimation.value < 50) {
        DrawSurface(screen, antagonista.second, x, y);
    }
    else {
        DrawSurface(screen, antagonista.first, x, y);
    }
}

void InitializeGame(double* worldTime, ItemsData* dragon)
{
    *worldTime = 0;
    dragon->hDistance = SPRITE_SIZE / 2;
    dragon->vDistance = SCREEN_HEIGHT - (SPRITE_SIZE + SPRITE_SIZE / 2);
    dragon->hSpeed = 0;
    dragon->vSpeed = 0;
}

void loadSprite(SDL_Surface** sprites) {

    sprites[0] = SDL_LoadBMP("./bmp/platform2.bmp");
    sprites[1] = SDL_LoadBMP("./bmp/ledder.bmp");
    sprites[2] = SDL_LoadBMP("./bmp/door.bmp");
    sprites[3] = SDL_LoadBMP("./bmp/platform2.bmp");
}

void InitBarrels(ItemsData* barrels, int initialPositions[][2]) {

    for (int i = 0; i < 6; i++) {
        barrels[i].hDistance = initialPositions[i][0];
        barrels[i].vDistance = initialPositions[i][1];
        barrels[i].hSpeed = 50;
        barrels[i].vSpeed = 50;
    }
}

void ShowMenu(SDL_Surface* screen, SDL_Surface* charset, int MenuItem) {
    SDL_FillRect(screen, NULL, 0x000000);

    DrawString(screen, 250, 150, "King Donkey", charset);
    DrawString(screen, 250, 200, "1. Start Game", charset);
    DrawString(screen, 250, 220, "2. Show score", charset);
    DrawString(screen, 250, 240, "3. Choose level", charset);
    DrawString(screen, 250, 260, "4. Exit", charset);
    DrawString(screen, 230, 200 + MenuItem * 20, "->", charset);
}

void case1(SDL_Surface* screen, SDL_Surface* charset) {
                 
    SDL_FillRect(screen, NULL, 0x000000);

    DrawString(screen, 250, 150, "Score:", charset);
    DrawString(screen, 250, 200, "not available", charset);
    DrawString(screen, 250, 450, "b - back to menu", charset);
}

void case2(SDL_Surface* screen, SDL_Surface* charset) {
    SDL_FillRect(screen, NULL, 0x000000);

    DrawString(screen, 230, 150, "Choose level:", charset);
    DrawString(screen, 230, 180, "Press 1 - level 1", charset);
    DrawString(screen, 230, 200, "Press 2 - level 2", charset);
    DrawString(screen, 230, 220, "Press 3 - level 3", charset);
    DrawString(screen, 230, 450, "b - back to menu", charset);
}

void MenuChoice(int* quit, int menuItem, int* gameOn, const char** level, double* worldTime, ItemsData* dragon, ItemsData* barrels, AnimationData* antagonistaAnim, int* choosecase1, int* choosecase2, int* inMenu, const char* level1, int initialPositions1[][2]) {
    switch (menuItem) {
    case 0:
        *gameOn = 1;
        *level = level1;
        InitializeGame(worldTime, dragon);
        InitBarrels(barrels, initialPositions1);
        antagonistaAnim->value = 0;
        *inMenu = 0;
        break;
    case 1:
        *choosecase1 = 1;
        *inMenu = 0;
        break;
    case 2:
        *choosecase2 = 1;
        *inMenu = 0;
        break;
    case 3:
        *quit = 1;
        break;
    }
}

const char* ChangeLevel1(const char* currentLevel, const char** level, int initialPositions1[][2], int initialPositions2[][2], int initialPositions3[][2], const char* level1, const char* level2, const char* level3, double* worldTime, ItemsData* dragon, ItemsData* barrels) {
    if (strcmp(*level, level1) == 0) {
        *level = level2;
        InitBarrels(barrels, initialPositions2);
    }
    else if (strcmp(*level, level2) == 0) {
        *level = level3;
        InitBarrels(barrels, initialPositions3);
    }
    else if (strcmp(*level, level3) == 0) {
        *level = level1;
        InitBarrels(barrels, initialPositions1);
    }
    InitializeGame(worldTime, dragon);
    return *level;
}

void RestartGame(int* gameOn, double* worldTime, ItemsData* dragon, ItemsData* barrels, const char** level, int initialPositions1[][2], int initialPositions2[][2], int initialPositions3[][2], const char* level1, const char* level2, const char* level3) {
    InitializeGame(worldTime, dragon);

    if (*level == level1) {
        InitBarrels(barrels, initialPositions1);
        *level = level1;
    }
    else if (*level == level2) {
        InitBarrels(barrels, initialPositions2);
        *level = level2;
    }
    else if (*level == level3) {
        InitBarrels(barrels, initialPositions3);
        *level = level3;
    }
}

int ScreenBounds(double new_h_position, double new_v_position) {
    return (new_h_position < MAN_HEIGHT / 2 || new_h_position >= SCREEN_WIDTH - (MAN_HEIGHT / 2) ||
        new_v_position < MAN_HEIGHT / 2 || new_v_position >= SCREEN_HEIGHT - (MAN_HEIGHT / 2));
}

int ManIndex(double new_h_position, double new_v_position, double v_speed) {// koordynaty bohatera
    int tile_x = (int)(new_h_position / SPRITE_SIZE);
    int tile_y;
    if (v_speed < 0) {
        tile_y = (int)(((new_v_position + MAN_HEIGHT / 2) + 1) / SPRITE_SIZE);
    }
    else {
        tile_y = (int)((new_v_position + MAN_HEIGHT / 2) / SPRITE_SIZE);
    }
    return tile_y * (SCREEN_WIDTH / SPRITE_SIZE) + tile_x;
}

int TopIndex(double new_h_position, double new_v_position) {
    int tile_x = (int)(new_h_position / SPRITE_SIZE);
    int tile_y = (int)((new_v_position - MAN_HEIGHT / 2 - 3) / SPRITE_SIZE);
    return tile_y * (SCREEN_WIDTH / SPRITE_SIZE) + tile_x;
}

int Index(double new_h_position, double new_v_position) {
    int tile_x = (int)(new_h_position / SPRITE_SIZE);
    int tile_y = (int)((new_v_position + BARREL_HEIGHT / 2 + 1) / SPRITE_SIZE);
    return tile_y * (SCREEN_WIDTH / SPRITE_SIZE) + tile_x;
}

char GetCellTypeAtPosition(const char* level, int x, int y) {
    int tile_x = x / SPRITE_SIZE;
    int tile_y = (y - SPRITE_SIZE / 2 - 1) / SPRITE_SIZE;
    return level[tile_y * (SCREEN_WIDTH / SPRITE_SIZE) + tile_x];
}

void MoveDragonRestrictions(ItemsData* dragon, const char** level, double delta, int jump, int climb, int inJump) {
    double new_h_position = dragon->hDistance + dragon->hSpeed * delta;
    double new_v_position = dragon->vDistance + dragon->vSpeed * delta;

    if (ScreenBounds(new_h_position, new_v_position)) {
        if (new_h_position < 10) {
            new_h_position = 10;
        }
        else if (new_h_position > SCREEN_WIDTH - 10) {
            new_h_position = SCREEN_WIDTH - 10;
        }
        if (new_v_position < 10) {
            new_v_position = 10;
        }
    }//return;

    int manindex = ManIndex(new_h_position, new_v_position, dragon->vSpeed);
    if (manindex < 0 || manindex > strlen(*level)) {
        return;
    }

    if ((*level)[manindex] == ' ' || (*level)[manindex] == '1' || (*level)[manindex] == '4') {
        dragon->hDistance = new_h_position;
    }
    if ((*level)[manindex] == '1' || ((*level)[manindex] == '3' && dragon->vSpeed < 0)) {
        dragon->vDistance = new_v_position;
        climb = 1;
    }
    if (((*level)[manindex] == '3' || (*level)[manindex] == '4') && dragon->vSpeed > 0) {
        dragon->vDistance = new_v_position;
        climb = 1;
    }

    if ((*level)[manindex] == ' ' && dragon->vSpeed > 0) {
        dragon->vDistance = new_v_position;
    }

    if (jump) {
        inJump = 1;
        dragon->vDistance = new_v_position;
    }
}

void JumpRestrictions(ItemsData* dragon, const char* level, double new_h_position, double new_v_position, int* jump, double* a, double delta) {
    char cellTypeUnderPlayer = GetCellTypeAtPosition(level, dragon->hDistance, dragon->vDistance + SPRITE_SIZE);
    char cellTypeInPlayer = GetCellTypeAtPosition(level, new_h_position, new_v_position - 2);

    if (cellTypeUnderPlayer == '0' && dragon->vSpeed > 0) {
        *jump = 0;
        *a = 0;
        dragon->vSpeed = 0;
        dragon->hSpeed = 0;
    }

    if (cellTypeUnderPlayer == ' ') {
        *a = 1;
    }

    if (cellTypeInPlayer == '0' && dragon->vSpeed <= 0 && *jump) {
        dragon->vSpeed = *a;
        dragon->vDistance += dragon->vSpeed * delta;
    }

    if (cellTypeUnderPlayer == '0' && dragon->vSpeed > 0 && *jump) {
        dragon->vSpeed = -1;
        dragon->vDistance += dragon->vSpeed * delta;
        dragon->vSpeed = 0;
        dragon->hSpeed = 0;
    }

    dragon->vSpeed = dragon->vSpeed + *a;
}           
                                                                                         
void MoveBarrels(ItemsData* barrels, const char** level, double delta, int initialPositions[][2]) { 
    for (int i = 0; i < 6; i++) { 
        int index = Index(barrels[i].hDistance, barrels[i].vDistance);         
        if (index < 0 || index > strlen(*level)) {
            return;
        }
        if (barrels[i].hDistance >= SCREEN_WIDTH) {  
            barrels[i].hDistance = initialPositions[i][0]; 
            barrels[i].vDistance = initialPositions[i][1];
        }
        if ((*level)[index] == '0' || (*level)[index] == '3') {
            barrels[i].hDistance += barrels[i].hSpeed * delta; 
            barrels[i].vSpeed = 0;
            barrels[i].hSpeed = 30;
        }                                                            
        if ((*level)[index] == ' ')
            barrels[i].vSpeed = 200;
        barrels[i].hSpeed = 30;
        barrels[i].vDistance += barrels[i].vSpeed *  delta;
        barrels[i].hDistance += barrels[i].hSpeed *  delta;
    }
}                                                                                 
 
int BarrelCollision(double hPosition, double vPosition, ItemsData* barrels, int& lives) {
    for (int i = 0; i < 6; i++) {
        double barrelsLeft = barrels[i].hDistance - 5;
        double barrelsRight = barrels[i].hDistance + 5;
        double barrelsTop = barrels[i].vDistance - 5;
        double barrelsBottom = barrels[i].vDistance + 5;

        double manLeft = hPosition - 8;                         
        double manRight = hPosition + 8;
        double manTop = vPosition - 8;
        double manBottom = vPosition + 8;        

        if (manRight >= barrelsLeft && manLeft <= barrelsRight &&
            manBottom >= barrelsTop && manTop <= barrelsBottom) {

            if (lives > 0) {
                lives--;
            }
            return 1;
        }
    }
    return 0;
}

void animation(AnimationData* Anim, double delta) {
    if (Anim->itemSpeed == 0) return;
    if (Anim->value < 100) {
        Anim->value += Anim->animationSpeed * delta;
    }
    else {
        Anim->value = 0;
    }
}         

void ContinueBox(SDL_Surface* screen, SDL_Surface* charset, int czarny, int light_green) {
    DrawRectangle(screen, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 40, 240, 80, light_green, czarny);

    char text[256];
    sprintf(text, "Do you want to continue?");
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2 - 15, text, charset);

    sprintf(text, "Yes - Y, No - Esc");
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2 + 10, text, charset);
}

void InfoBar(SDL_Surface* screen, SDL_Surface* charset, double worldTime, double fps, int czarny, int light_green) {
    DrawRectangle(screen, 0, 0, SCREEN_WIDTH, 40, czarny, light_green);

    char text[256];
    sprintf(text, "czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

    sprintf(text, "Esc - wyjscie, n - new game, wykonano: A-E");
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

    DrawOutline(screen, 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, light_green);
}

int LoadImages(Sprite* runImage, Sprite* jumpImage, Sprite* climbImage, Sprite* barrelImage, Sprite* antagonistaImage) {
    runImage->first = SDL_LoadBMP("./bmp/run1.bmp");
    runImage->second = SDL_LoadBMP("./bmp/run2.bmp");

    jumpImage->first = SDL_LoadBMP("./bmp/jump1.bmp");
    jumpImage->second = SDL_LoadBMP("./bmp/jump2.bmp");

    climbImage->first = SDL_LoadBMP("./bmp/climb1.bmp");
    climbImage->second = SDL_LoadBMP("./bmp/climb2.bmp");

    barrelImage->first = SDL_LoadBMP("./bmp/barrel1.bmp");
    barrelImage->second = SDL_LoadBMP("./bmp/barrel2.bmp");

    antagonistaImage->first = SDL_LoadBMP("./bmp/antagonista1.bmp");
    antagonistaImage->second = SDL_LoadBMP("./bmp/antagonista2.bmp");

    return 0;
}

void InitAnimations(AnimationData* runAnim, AnimationData* jumpAnim, AnimationData* climbAnim, AnimationData* barrelAnim, AnimationData* antagonistaAnim) {
    runAnim->value = 0;
    runAnim->animationSpeed = 250;

    jumpAnim->value = 0;
    jumpAnim->animationSpeed = 300;

    climbAnim->value = 0;
    climbAnim->animationSpeed = 350;

    barrelAnim->value = 0;
    barrelAnim->animationSpeed = 300;

    antagonistaAnim->value = 0;
    antagonistaAnim->animationSpeed = 11;
}


void UpdateAnimations(double delta, AnimationData* Anim1, AnimationData* Anim2, double itemSpeed2, AnimationData* Anim3, double itemSpeed3, AnimationData* Anim4, double itemSpeed4, AnimationData* Anim5) {

    animation(Anim1, delta);

    animation(Anim2, delta);
    Anim2->itemSpeed = itemSpeed2;

    animation(Anim3, delta);
    Anim3->itemSpeed = itemSpeed3;                                   

    animation(Anim4, delta);
    Anim4->itemSpeed = itemSpeed4;

    animation(Anim5, delta);
}    

void FPS(double delta, int* frames, double* fpsTimer, double* fps) {
    *fpsTimer += delta;
    if (*fpsTimer > 0.5) {
        *fps = *frames * 2; 
        *frames = 0;
        *fpsTimer -= 0.5;
    } 
}

int KeyUp(const char* level,ItemsData* dragon, int jump, double* a, double delta) {
    double new_h_position = dragon->hDistance + dragon->hSpeed * delta;
    double new_v_position = dragon->vDistance + dragon->vSpeed * delta;
    char cellTypeUnderPlayer = GetCellTypeAtPosition(level, new_h_position, new_v_position + SPRITE_SIZE);

    if (cellTypeUnderPlayer != ' ' && !jump) {
        dragon->hSpeed = 0;
        dragon->vSpeed = 0;
        *a = 0;
    }

    if (jump || cellTypeUnderPlayer == ' ') {
        *a = 1;
    }

    return 0;
}

void KeyPad3Button(int* lives, int* gameOn, const char** level, double* worldTime, ItemsData* dragon, ItemsData* barrels, AnimationData* antagonistaAnim, int* choosecase2, const char* level3, int initialPositions3[][2]) {
    if (*lives == 0) {
        *lives = 3;
    }
    *gameOn = 1;
    *level = level3;
    InitializeGame(worldTime, dragon);
    InitBarrels(barrels, initialPositions3);
    antagonistaAnim->value = 0;
    *choosecase2 = 0;
}

void KeyPad2Button(int* lives, int* gameOn, const char** level, double* worldTime, ItemsData* dragon, ItemsData* barrels, AnimationData* antagonistaAnim, int* choosecase2, const char* level2, int initialPositions2[][2]) {
    if (*lives == 0) {
        *lives = 3;
    }
    *gameOn = 1;
    *level = level2;
    InitializeGame(worldTime, dragon);
    InitBarrels(barrels, initialPositions2);
    antagonistaAnim->value = 0;
    *choosecase2 = 0;
}

void KeyPad1Button(int* gameOn, const char** level, double* worldTime, ItemsData* dragon, ItemsData* barrels, AnimationData* antagonistaAnim, int* choosecase2, const char* level1, int initialPositions1[][2]) {
    *gameOn = 1;
    *level = level1;
    InitializeGame(worldTime, dragon);
    InitBarrels(barrels, initialPositions1);
    antagonistaAnim->value = 0;
    *choosecase2 = 0;
}

void spaceButton(ItemsData* dragon, int* jump, int* inJump, int* climb) {
    dragon->vSpeed = -400;
    *jump = 1;
    *inJump = 1;
    *climb = 0;
}

void yButton(int* gameOn, int* choosecase1, int* choosecase2, int* inMenu) {
    *gameOn = 1;
    *choosecase1 = 0;
    *choosecase2 = 0;
    *inMenu = 0;
}

void upButton(ItemsData* dragon, int* jump, int* climb, int inMenu, int* MenuItem) {
    if (inMenu == 1) {
        if (*MenuItem > 0) (*MenuItem)--;
    }
    else {
        dragon->vSpeed = -175;
        *jump = 0;
        *climb = 1;   
    }
}

void downButton(ItemsData* dragon, int* jump, int* climb, int inMenu, int* MenuItem) {
    if (inMenu == 1) {
        if (*MenuItem < 3) (*MenuItem)++;
    }
    else {
        dragon->vSpeed = 175;
        *jump = 0;
        *climb = 1;
    }
}

void LeftButtonPress(ItemsData* dragon, int* inJump, int* leftButton, int* climb) {
    dragon->hSpeed = -150;
    *inJump = 0;
    *leftButton = 1;
    *climb = 0;
}

void RightButtonPress(ItemsData* dragon, int* inJump, int* rightButton, int* climb) {
    dragon->hSpeed = 151;
    *inJump = 0;
    *rightButton = 1;
    *climb = 0;
}

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) { 
    int t1, t2, quit, frames, rc;
    double delta, worldTime, fpsTimer, fps;
    double a = 0;//przyspieszenie
    int jump = 0, inJump = 0, rightButton = 0, leftButton = 0, climb = 0, MenuItem = 0, lives = 3, inMenu = 1, choosecase1 = 0, choosecase2 = 0, continuegamechoice = 0, gameOn = 0; // 0- false, 1- true
    SDL_Event event;
    SDL_Surface* screen, * charset;
    SDL_Surface* sprites[4];
    SDL_Texture* scrtex;
    SDL_Window* window;
    SDL_Renderer* renderer;
    ItemsData barrels[6], dragon; 
    Sprite runImage, jumpImage, climbImage, barrelImage, antagonistaImage;
    AnimationData runAnim, jumpAnim, climbAnim, barrelAnim, antagonistaAnim;

    if (InitializeSDL(&window, &renderer, &screen, &scrtex) != 0) {
        return 1;
    }

    charset = SDL_LoadBMP("./bmp/cs8x8.bmp");

    SDL_SetColorKey(charset, true, 0x000000);

    if (LoadImages(&runImage, &jumpImage, &climbImage, &barrelImage, &antagonistaImage) != 0) {
        return 1;
    }

    loadSprite(sprites);

    const char* level1 = Level1();
    int initialPositions1[6][2] = {{30 , 94},{120, 154},{15, 214},{100, 274},{70, 334},{70, 454}};
    const char* level2 = Level2();
    int initialPositions2[6][2] = {{90, 91},{110, 151},{465, 211},{230, 271},{60, 331},{40, 451}};
    const char* level3 = Level3();  
    int initialPositions3[6][2] = {{1, 91},{110, 151},{10, 211},{230, 271},{80, 331},{60, 451}}; 
          
    const char* level = level1;
    const char* currentLevel = level;

    char text[128];
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    int light_green = SDL_MapRGB(screen->format, 0x53, 0x8F, 0x63);

    t1 = SDL_GetTicks();
    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;

    dragon.hDistance = SPRITE_SIZE / 2;
    dragon.vDistance = SCREEN_HEIGHT - (SPRITE_SIZE + SPRITE_SIZE / 2);

    dragon.hSpeed = 0, dragon.vSpeed = 0;
    barrelAnim.itemSpeed = barrels[5].vSpeed;
    InitAnimations(&runAnim, &jumpAnim, &climbAnim, &barrelAnim, &antagonistaAnim); 

    while (!quit) {
        t2 = SDL_GetTicks();
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        worldTime += delta;

        double new_h_position = dragon.hDistance + dragon.hSpeed * delta;       
        double new_v_position = dragon.vDistance + dragon.vSpeed * delta; 

        int index = Index(new_h_position, new_v_position);

        MoveDragonRestrictions(&dragon, &level, delta, jump, climb, inJump);
        MoveBarrels(barrels, &level, delta, initialPositions1);    
        UpdateAnimations(delta, &barrelAnim, &runAnim, dragon.hSpeed, &jumpAnim, dragon.vSpeed, &climbAnim, dragon.vSpeed, &antagonistaAnim);

        SDL_FillRect(screen, NULL, czarny);
                   
        if (inMenu && !choosecase1 && !choosecase2 && !gameOn) ShowMenu(screen, charset, MenuItem);
        else if (!inMenu && choosecase1 == 1 && !gameOn) case1(screen, charset);
        else if (!inMenu && choosecase2 == 1 && !gameOn) case2(screen, charset);
        else if (lives == 0) {
            inMenu = 1;
            lives = 3;
        }
        else if (!inMenu && (!choosecase1 || !choosecase2) && gameOn) {
            DrawLevel1(level, screen, sprites, charset, lives);
            for (int i = 0; i < 6; i++) {
                DrawBarrels(screen, barrelImage, barrelAnim, barrels[i].hDistance, barrels[i].vDistance);
            }
            if (BarrelCollision(dragon.hDistance, dragon.vDistance, barrels, lives)) {
                inMenu = 0;
                choosecase1 = 0;
                choosecase2 = 0;
                gameOn = 0;
            }
            DrawAntagonista(screen, antagonistaImage, antagonistaAnim, 23, 80);
            if ((inJump && ( rightButton || leftButton)) || inJump) {
                DrawDragon(screen, jumpImage, jumpAnim, dragon.hDistance, dragon.vDistance);
            }
            else if (climb) { 
                DrawDragon(screen, climbImage, climbAnim, dragon.hDistance, dragon.vDistance);
            }
            else {
                DrawDragon(screen, runImage, runAnim, dragon.hDistance, dragon.vDistance);
            }
            if (level[index] == '2') {
                level = ChangeLevel1(currentLevel, &level, initialPositions1, initialPositions2, initialPositions3, level1, level2, level3, &worldTime, &dragon, barrels);
                antagonistaAnim.value = 0;    
            }
            InfoBar(screen, charset, worldTime, fps, czarny, light_green);
        }
        else if (!gameOn) ContinueBox(screen, charset, czarny, light_green);
         
        FPS(delta, &frames, &fpsTimer, &fps);

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);


        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                else if (event.key.keysym.sym == SDLK_n) {
                    antagonistaAnim.value = 0;
                    lives = 3;
                    RestartGame(&gameOn, &worldTime, &dragon, barrels, &level, initialPositions1, initialPositions2, initialPositions3, level1, level2, level3);
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    LeftButtonPress(&dragon, &inJump, &leftButton, &climb);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    RightButtonPress(&dragon, &inJump, &rightButton, &climb);
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    upButton(&dragon, &jump, &climb, inMenu, &MenuItem);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    downButton(&dragon, &jump, &climb, inMenu, &MenuItem);
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!jump && dragon.vSpeed == 0) spaceButton(&dragon, &jump, &inJump, &climb);
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (inMenu == 1) {
                        MenuChoice(&quit,MenuItem, &gameOn, &level, &worldTime, &dragon, barrels, &antagonistaAnim, &choosecase1, &choosecase2, &inMenu, level1, initialPositions1 );
                    }
                }
                else if (event.key.keysym.sym == SDLK_b) {
                    inMenu = 1;
                    choosecase1 = 0;
                    choosecase2 = 0;
                }
                else if (event.key.keysym.sym == SDLK_y) {
                    if (!inMenu && !choosecase1 && !choosecase2) {
                        yButton(&gameOn, &choosecase1, &choosecase2, &inMenu);
                        RestartGame(&gameOn, &worldTime, &dragon, barrels, &level, initialPositions1, initialPositions2, initialPositions3, level1, level2, level3);
                        antagonistaAnim.value = 0;
                    }
                }
                else if (event.key.keysym.sym == SDLK_KP_1) {
                    if (!inMenu) KeyPad1Button(&gameOn, &level, &worldTime, &dragon, barrels, &antagonistaAnim, &choosecase2, level1, initialPositions1);
                }
                else if (event.key.keysym.sym == SDLK_KP_2) {
                    if (!inMenu) KeyPad2Button(&lives, &gameOn, &level, &worldTime, &dragon, barrels, &antagonistaAnim, &choosecase2, level2, initialPositions2);
                }
                else if (event.key.keysym.sym == SDLK_KP_3) {
                    if (!inMenu) KeyPad3Button(&lives, &gameOn, &level, &worldTime, &dragon, barrels, &antagonistaAnim, &choosecase2, level3, initialPositions3);
                }
                break;
            case SDL_KEYUP: {
                KeyUp(level, &dragon, jump, &a, delta);
                break; }
            case SDL_QUIT:
                quit = 1;
                break;
            };
        };
        JumpRestrictions(&dragon, level, new_h_position, new_v_position, &jump, &a, delta);
        frames++;
    };

    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
};
