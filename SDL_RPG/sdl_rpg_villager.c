#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_W 640
#define WINDOW_H 480
#define TILE 32
#define MAP_W 20
#define MAP_H 15

// # = 壁, . = 道, N = 村人
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

    SDL_Window* window = SDL_CreateWindow("SDL RPG Villager",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int px = TILE, py = TILE; // キャラクター初期位置
    int speed = 4;

    SDL_Event e;
    int quit = 0;

    // 前回接触した村人マス
    int prev_tile_x = -1, prev_tile_y = -1;

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

        // 村人接触判定（前回と違うマスなら会話）
        if (map[tile_y][tile_x] == 'N' && (tile_x != prev_tile_x || tile_y != prev_tile_y)) {
            printf("村人: 「こんにちは！ようこそ村へ！」\n");
            prev_tile_x = tile_x;
            prev_tile_y = tile_y;
        }

        // 描画
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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
