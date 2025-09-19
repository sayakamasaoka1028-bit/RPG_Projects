#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "恐竜RPG",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        0
    );
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // 背景画像
    SDL_Surface *bgSurface = IMG_Load("background-7487151_640.jpg");
    if (!bgSurface) {
        printf("IMG_Load Error (background): %s\n", IMG_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(ren, bgSurface);
    SDL_FreeSurface(bgSurface);

    // 恐竜画像
    SDL_Surface *dinoSurface = IMG_Load("dino.png");
    if (!dinoSurface) {
        printf("IMG_Load Error (dino): %s\n", IMG_GetError());
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *dinoTexture = SDL_CreateTextureFromSurface(ren, dinoSurface);
    SDL_FreeSurface(dinoSurface);

    SDL_Rect dinoRect = {350, 300, 100, 100}; // 恐竜の初期位置とサイズ

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    dinoRect.y -= 10; break;
                    case SDLK_DOWN:  dinoRect.y += 10; break;
                    case SDLK_LEFT:  dinoRect.x -= 10; break;
                    case SDLK_RIGHT: dinoRect.x += 10; break;
                }
            }
        }

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bgTexture, NULL, NULL);        // 背景描画
        SDL_RenderCopy(ren, dinoTexture, NULL, &dinoRect); // 恐竜描画
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(dinoTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
