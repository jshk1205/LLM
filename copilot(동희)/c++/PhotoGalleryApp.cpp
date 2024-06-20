// 프로젝트의 기본 설정과 파일 구조를 생성해줘
// PhotoGalleryAPP.cpp 파일과 필요한 헤더 파일들을 만들어줘
// SDL 라이브러리를 포함하고 초기화하는 코드를 작성해줘
// 사진을 추가할 수 있는 기능을 구현해줘
// 사진을 삭제할 수 있는 기능을 구현해줘
// 사진을 저장할 수 있는 기능을 구현해줘
// main 함수를 작성해줘
//GUI 라이브러리: Qt 또는 GTK+ 같은 C++ GUI 라이브러리
//이미지 처리 라이브러리: OpenCV 또는 libpng, libjpeg 같은 이미지 처리 라이브러리
//파일 시스템 관리: Boost Filesystem 또는 표준 라이브러리의 파일 입출력 기능
//이러한 정보를 바탕으로 사진갤러리 앱을 만들어줘
//init(), load_images(), draw() 함수의 구현을 완성하고, 필요에 따라 추가적인 이벤트 처리 로직을 handle_event() 함수에 추가해줘



//g++ -o PhotoGalleryApp PhotoGalleryApp.cpp `sdl2-config --cflags --libs` -lSDL2_image
//./PhotoGalleryApp
// 코드와 실행파일이 같은 디렉토리에 있어야 합니다.
// 같은 디렉토리에 images 디렉토리를 만들고 사진을 넣어주세요.
// 왼쪽 화살표 키를 누르면 이전 사진을 볼 수 있습니다.
// 오른쪽 화살표 키를 누르면 다음 사진을 볼 수 있습니다.
// S 키를 누르면 스크린샷을 찍을 수 있습니다.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;
SDL_Texture* texture = NULL;
SDL_Surface* image = NULL;
std::vector<std::string> imagePaths;
int currentImage = 0;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    window = SDL_CreateWindow("Photo Gallery App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void load_images_from_directory(const std::string& directory) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string file_name = ent->d_name;
            if (file_name != "." && file_name != "..") {
                imagePaths.push_back(directory + "/" + file_name);
            }
        }
        closedir(dir);
    } else {
        perror("");
    }
}

void load_image() {
    if (!imagePaths.empty()) {
        image = IMG_Load(imagePaths[currentImage].c_str());
        if (texture) {
            SDL_DestroyTexture(texture); // Avoid memory leak by destroying the old texture
        }
        texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
    }
}

void draw() {
    SDL_RenderClear(renderer);
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
    }
    SDL_RenderPresent(renderer);
}

void handle_event() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentImage = (currentImage - 1 + imagePaths.size()) % imagePaths.size();
                        load_image();
                        draw();
                        break;
                    case SDLK_RIGHT:
                        currentImage = (currentImage + 1) % imagePaths.size();
                        load_image();
                        draw();
                        break;
                    case SDLK_s:
                        SDL_Surface* surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
                        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                        IMG_SavePNG(surface, "screenshot.png");
                        SDL_FreeSurface(surface);
                        break;
                }
                break;
        }
    }
}

int main() {
    init();
    load_images_from_directory("images"); // Assuming there's a directory named "images" with photos
    load_image();
    draw();

    while (true) {
        handle_event();
    }

    return 0;
}