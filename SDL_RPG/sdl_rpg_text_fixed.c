#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define WINDOW_W 640
#define WINDOW_H 480
#define TILE 32
#define MAP_W 20
#define MAP_H 15

// マップ：#=壁, .=道, N=村人
char map[MAP_H][MAP_W+1] = {
    "####################",
    "#.................#",
    "#..####....N......#",
    "#.................#",
    "#.....#######.....#",
    "#.................#",
    "#.............N...#",
    "#.................#",
    "#.................#",
    "#.................#",
    "#.....#######.....#",
    "#.................#",
    "#.................#",
    "#.................#",
    "####################"
};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL 初期化失敗: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("TTF 初期化失敗: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL RPG Text",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        printf("フォント読み込み失敗: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color white = {255, 255, 255, 255};
    char message[128] = "";

    int px = TILE, py = TILE;
    int speed = 4;
    int prev_tile_x = -1, prev_tile_y = -1;

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        int new_px = px;
        int new_py = py;

        if (state[SDL_SCANCODE_W] && py > 0) new_py -= speed;
        if (state[SDL_SCANCODE_S] && py < WINDOW_H - TILE) new_py += speed;
        if (state[SDL_SCANCODE_A] && px > 0) new_px -= speed;
        if (state[SDL_SCANCODE_D] && px < WINDOW_W - TILE) new_px += speed;

        int tile_x = new_px / TILE;
        int tile_y = new_py / TILE;

        // 壁チェック
        if (map[tile_y][tile_x] != '#') {
            px = new_px;
            py = new_py;
        }

        // 村人接触判定（重なり判定）
        int player_left = px;
        int player_right = px + TILE;
        int player_top = py;
        int player_bottom = py + TILE;

        int villager_left = tile_x * TILE;
        int villager_right = villager_left + TILE;
        int villager_top = tile_y * TILE;
        int villager_bottom = villager_top + TILE;

        if (map[tile_y][tile_x] == 'N' &&
            !(player_right <= villager_left || player_left >= villager_right ||
              player_bottom <= villager_top || player_top >= villager_bottom) &&
            (tile_x != prev_tile_x || tile_y != prev_tile_y)) {
            snprintf(message, sizeof(message), "村人: こんにちは！ようこそ村へ！");
            prev_tile_x = tile_x;
            prev_tile_y = tile_y;
        }

        // 描画
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 背景青
        SDL_RenderClear(renderer);

        for (int y = 0; y < MAP_H; y++) {
            for (int x = 0; x < MAP_W; x++) {
                SDL_Rect tile = { x*TILE, y*TILE, TILE, TILE };
                if (map[y][x] == '#') SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                else if (map[y][x] == 'N') SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &tile);
            }
        }

        SDL_Rect player = { px, py, TILE, TILE };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 赤キャラクター
        SDL_RenderFillRect(renderer, &player);

        // メッセージ描画
        if (message[0] != '\0') {
            SDL_Surface* surf = TTF_RenderUTF8_Solid(font, message, white);
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect dst = { 20, WINDOW_H - 50, surf->w, surf->h };
            SDL_FreeSurface(surf);
            SDL_RenderCopy(renderer, textTex, NULL, &dst);
            SDL_DestroyTexture(textTex);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
