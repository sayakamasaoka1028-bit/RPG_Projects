#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_W 640
#define WINDOW_H 480
#define TILE 32

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL 初期化失敗: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL RPG Simple",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int px = TILE, py = TILE; // キャラクター初期位置
    int speed = 4;

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W] && py > 0) py -= speed;
        if (state[SDL_SCANCODE_S] && py < WINDOW_H - TILE) py += speed;
        if (state[SDL_SCANCODE_A] && px > 0) px -= speed;
        if (state[SDL_SCANCODE_D] && px < WINDOW_W - TILE) px += speed;

        // 描画
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 背景青
        SDL_RenderClear(renderer);

        SDL_Rect player = { px, py, TILE, TILE };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 赤四角
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
