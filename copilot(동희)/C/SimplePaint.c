// C언어로 간단한 그림판을 만들어줘
// 해당 코드는 우분투 환경에서 실행되어야 합니다.
// 그림판은 다음과 같은 기능을 제공해야 합니다.
// 1. 그림판은 800x600 크기의 화면을 가지고 있어야 합니다.
// 2. 그림판은 마우스로 선을 그릴 수 있어야 합니다.
// 3. 선의 색상은 사용자가 변경할 수 있어야 합니다.
// 4. 선의 굵기는 사용자가 변경할 수 있어야 합니다.
// 5. 그림판은 저장 기능을 제공해야 합니다. 저장 기능은 사용자가 그린 그림을 파일로 저장할 수 있어야 합니다.
// 6. 그림판은 불러오기 기능을 제공해야 합니다. 불러오기 기능은 사용자가 파일로 저장한 그림을 불러올 수 있어야 합니다.
// 7. sdl 라이브러리를 사용하여 그림판을 구현해야 합니다.
// 8. 그림판은 다음과 같은 함수를 구현해야 합니다.
// void init() : 그림판을 초기화합니다.
// void draw() : 그림판을 그립니다.
// void handle_event() : 사용자의 입력을 처리합니다.
// void save() : 그림판을 파일로 저장합니다.
// void load() : 파일로 저장한 그림을 불러옵니다.
// void cleanup() : 그림판을 정리합니다.
// 9. 그림판은 다음과 같은 제약을 가집니다.
// 그림판은 선을 그릴 때, 마우스의 왼쪽 버튼을 누르고 있는 동안 선을 그릴 수 있어야 합니다.

// 비주얼 스튜디오에서 헤더추가하는게 어려워서 우분투 환경에서 실행했습니다.
// sudo apt-get update
// sudo apt-get install libsdl2-dev
// gcc SimplePaint.c -o SimplePaint -lSDL2
//./SimplePaint

// 색 바꾸는 키 R , G , B
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
bool isDrawing = false;
int startX, startY, endX, endY;
Uint8 r = 0, g = 0, b = 0;

void init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Simple Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    SDL_RenderPresent(renderer);
}

void handle_event()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_Quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                isDrawing = true;
                startX = event.button.x;
                startY = event.button.y;
            }
            break;
        case SDL_MOUSEMOTION:
            if (isDrawing)
            {
                endX = event.motion.x;
                endY = event.motion.y;
                draw();
                startX = endX;
                startY = endY;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                isDrawing = false;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_r:
                r = 255;
                g = 0;
                b = 0;
                break;
            case SDLK_g:
                r = 0;
                g = 255;
                b = 0;
                break;
            case SDLK_b:
                r = 0;
                g = 0;
                b = 255;
                break;
            }
            break;
        }
    }
}

void save()
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, "paint.bmp");
    SDL_FreeSurface(surface);
}

void load()
{
    SDL_Surface *surface = SDL_LoadBMP("paint.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    init();
    while (1)
    {
        handle_event();
    }
    cleanup();
    return 0;
}
