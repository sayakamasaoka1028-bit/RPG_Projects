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
    "#.................#",
    "#.................#",
    "#.................#",
    "#.................#",
    "#.....#######.....#",
    "#.................#",
    "#.................#",
    "#.................#",
    "####################"
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 初期化失敗: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL RPG Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("ウィンドウ作成失敗: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("レンダラー作成失敗: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int px = TILE;  // キャラクター初期位置
    int py = TILE;
    int speed = 4;

    SDL_Event e;
    int quit = 0;

    // 前回のタイル座標（村人との接触判定用）
    int prev_tile_x = -1;
    int prev_tile_y = -1;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        int new_px = px;
        int new_py = py;

        if (state[SDL_SCANCODE_W]) new_py -= speed;
        if (state[SDL_SCANCODE_S]) new_py += speed;
        if (state[SDL_SCANCODE_A]) new_px -= speed;
        if (state[SDL_SCANCODE_D]) new_px += speed;

        int tile_x = new_px / TILE;
        int tile_y = new_py / TILE;

        if (tile_x >= 0 && tile_x < MAP_W && tile_y >= 0 && tile_y < MAP_H) {
            // 壁チェック
            if (map[tile_y][tile_x] == '.') {
                px = new_px;
                py = new_py;
            }
            // 村人接触判定
            if (map[tile_y][tile_x] == 'N' && (tile_x != prev_tile_x || tile_y != prev_tile_y)) {
                printf("村人: 「こんにちは！ようこそ村へ！」\n");
                prev_tile_x = tile_x;
                prev_tile_y = tile_y;
            }
        }

        // マップ描画
        for (int y = 0; y < MAP_H; y++) {
            for (int x = 0; x < MAP_W; x++) {
                SDL_Rect tile = { x*TILE, y*TILE, TILE, TILE };
                if (map[y][x] == '#') {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // 壁
                } else if (map[y][x] == 'N') {
                    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // 村人
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 道
                }
                SDL_RenderFillRect(renderer, &tile);
            }
        }

        // キャラクター描画（赤四角）
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
