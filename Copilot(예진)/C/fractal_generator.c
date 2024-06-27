// 수학적 프랙탈(예: Mandelbrot 집합, Julia 집합)을 생성하고 시각화하는 프로그램을 c언어로 개발합니다.
// 이 프로그램은 사용자가 프랙탈의 종류와 세부 사항을 선택할 수 있는 웹 인터페이스를 제공합니다.
// 사용자가 선택한 옵션에 따라 프랙탈을 생성하고 이미지를 생성하여 사용자에게 제공합니다.
// 이 프로그램은 Flask 웹 프레임워크를 사용하여 웹 인터페이스를 제공하고, C 프로그램을 호출하여 프랙탈을 생성하고 이미지를 생성합니다.
// 이 프로그램은 사용자가 선택한 프랙탈의 종류와 세부 사항을 저장하고, 사용자가 이미지를 다운로드할 수 있도록 합니다.
// 이 프로그램은 사용자가 생성한 프랙탈 이미지를 저장하고, 사용자가 생성한 이미지를 다운로드할 수 있도록 합니다.
// 다음은 프로그램의 주요 기능입니다.
// 1. 사용자가 프랙탈의 종류를 선택할 수 있습니다.
// 2. 사용자가 프랙탈의 세부 사항을 선택할 수 있습니다.
// 3. 사용자가 프랙탈 이미지를 생성하고 다운로드할 수 있습니다.
// 4. 사용자가 생성한 프랙탈 이미지를 저장하고, 사용자가 생성한 이미지를 다운로드할 수 있습니다.
// 개발해 주세요.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to generate Mandelbrot set
void mandelbrot(int width, int height, int max_iter, double zoom, double x_offset, double y_offset, char* filename)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    unsigned char* img = (unsigned char*)malloc(width * height * 3);
    if (img == NULL)
    {
        fprintf(stderr, "Error allocating memory.\n");
        fclose(fp);
        return;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double zx = 0;
            double zy = 0;
            double cx = (x - width / 2.0) * 4.0 / width / zoom + x_offset;
            double cy = (y - height / 2.0) * 4.0 / width / zoom + y_offset;
            int i = 0;
            for (i = 0; i < max_iter; i++)
            {
                double zx_new = zx * zx - zy * zy + cx;
                double zy_new = 2 * zx * zy + cy;
                if (zx_new * zx_new + zy_new * zy_new > 4)
                {
                    break;
                }
                zx = zx_new;
                zy = zy_new;
            }
            double t = (double)i / max_iter;
            int r = (int)(9 * (1 - t) * t * t * t * 255);
            int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
            int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
            img[(y * width + x) * 3 + 0] = r;
            img[(y * width + x) * 3 + 1] = g;
            img[(y * width + x) * 3 + 2] = b;
        }
    }

    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(img, 1, width * height * 3, fp);

    fclose(fp);
    free(img);
}

// Function to generate Julia set
void julia(int width, int height, int max_iter, double zoom, double x_offset, double y_offset, double c_real, double c_imag, char* filename)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    unsigned char* img = (unsigned char*)malloc(width * height * 3);
    if (img == NULL)
    {
        fprintf(stderr, "Error allocating memory.\n");
        fclose(fp);
        return;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double zx = (x - width / 2.0) * 4.0 / width / zoom + x_offset;
            double zy = (y - height / 2.0) * 4.0 / width / zoom + y_offset;
            int i = 0;
            for (i = 0; i < max_iter; i++)
            {
                double zx_new = zx * zx - zy * zy + c_real;
                double zy_new = 2 * zx * zy + c_imag;
                if (zx_new * zx_new + zy_new * zy_new > 4)
                {
                    break;
                }
                zx = zx_new;
                zy = zy_new;
            }
            double t = (double)i / max_iter;
            int r = (int)(9 * (1 - t) * t * t * t * 255);
            int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
            int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
            img[(y * width + x) * 3 + 0] = r;
            img[(y * width + x) * 3 + 1] = g;
            img[(y * width + x) * 3 + 2] = b;
        }
    }

    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(img, 1, width * height * 3, fp);

    fclose(fp);
    free(img);
}

int main()
{
    // Generate Mandelbrot set
    mandelbrot(800, 800, 1000, 1, 0, 0, "mandelbrot.ppm");

    // Generate Julia set
    julia(800, 800, 1000, 1, 0, 0, -0.7, 0.27015, "julia.ppm");

    return 0;
}
