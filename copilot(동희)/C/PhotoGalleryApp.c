// 우분투 환경에서 c언어 사진갤러리 앱을 만들어줘요
//해당 앱은 사진을 불러와서 화면에 보여주고 다음 사진으로 넘어가는 버튼을 만들어주시면 됩니다.
//사진은 5장정도 넣어주시면 됩니다.
//사진은 화면에 꽉차게 보여주시면 됩니다.
//사진은 화면에 가운데 정렬되어야 합니다.
//사진을 불러오는 방법은 다음과 같습니다.
//1. 사진을 불러오기 위해서는 SDL_image 라이브러리를 사용해야 합니다.
//2. 우분투 환경에서 SDL_image 라이브러리를 설치하기 위해서는 다음과 같은 명령어를 사용하면 됩니다.
//sudo apt-get update
//sudo apt-get install libsdl2-image-dev
//3. SDL_image 라이브러리를 사용하기 위해서는 다음과 같은 헤더파일을 추가해야 합니다.
//#include <SDL2/SDL_image.h>
//4. 사진을 불러오기 위해서는 다음과 같은 함수를 사용하면 됩니다.
//SDL_Surface* image = IMG_Load("image.jpg");
//5. 사진을 화면에 보여주기 위해서는 다음과 같은 함수를 사용하면 됩니다.
//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
//6. 사진을 화면에 가운데 정렬하기 위해서는 다음과 같은 방법을 사용하면 됩니다.
//int w, h;
//SDL_QueryTexture(texture, NULL, NULL, &w, &h);
//SDL_Rect dstrect = {WIDTH / 2 - w / 2, HEIGHT / 2 - h / 2, w, h};
//SDL_RenderCopy(renderer, texture, NULL, &dstrect);
//7. 사진을 화면에 꽉차게 보여주기 위해서는 다음과 같은 방법을 사용하면 됩니다.


//  gcc PhotoGalleryApp.c -o PhotoGalleryApp -lSDL2 -lSDL2_image
// ./PhotoGalleryApp
// 사진은 1.jpg, 2.jpg, 3.jpg, 4.jpg, 5.jpg를 사용하시면 됩니다.


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
SDL_Texture *texture = NULL;
SDL_Surface *image = NULL;
int currentImage = 0;
char *images[] = {"1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg"};

void init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Photo Gallery App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void load_image()
{
    image = IMG_Load(images[currentImage]);
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
}

void draw()
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void handle_event()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_RIGHT:
                currentImage = (currentImage + 1) % 5;
                load_image();
                draw();
                break;
            case SDLK_LEFT:
                currentImage = (currentImage + 4) % 5;
                load_image();
                draw();
                break;
            }
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    init();
    load_image();
    draw();
    while (1)
    {
        handle_event();
    }
    return 0;
}


