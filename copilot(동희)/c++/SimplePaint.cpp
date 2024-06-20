// C++언어로 간단한 그림판을 만들어줘
// 그림판의 기능은 아래와 같아
// 1. 그림판은 800x600 크기의 화면을 가지고 있어야 해
// 2. 그림판은 마우스로 선을 그릴 수 있어야 해
// 3. 선의 색상은 사용자가 변경할 수 있어야 해
// 4. sdl 라이브러리를 사용하여 그림판을 구현해야 해
// 5. 그림판은 다음과 같은 함수를 구현해야 해

//g++ SimplePaint.cpp -o SimplePaint -lSDL2
//./SimplePaint

// R, G, B 키를 누르면 선의 색상이 변경됩니다.
// S 키를 누르면 그림을 저장합니다.
// 마우스 왼쪽 버튼을 누른 상태로 마우스를 움직이면 선을 그릴 수 있습니다.


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
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

void saveDrawing();

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
            case SDLK_s:
                saveDrawing();
                break;            
            }
            break;
        }
    }
}

void saveDrawing() {
    SDL_Surface* saveSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0); // saveSurface 생성
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, saveSurface->pixels, saveSurface->pitch);
    IMG_SavePNG(saveSurface, "saved_drawing.png"); // 'saved_drawing.png'는 저장될 파일 이름
    SDL_FreeSurface(saveSurface);
}

int main(int argc, char *argv[])
{
    init();
    while (1)
    {
        handle_event();
    }
    IMG_Quit(); 
    return 0;
}

