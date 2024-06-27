// ������ ����Ż(��: Mandelbrot ����, Julia ����)�� �����ϰ� �ð�ȭ�ϴ� ���α׷��� c���� �����մϴ�.
// �� ���α׷��� ����ڰ� ����Ż�� ������ ���� ������ ������ �� �ִ� �� �������̽��� �����մϴ�.
// ����ڰ� ������ �ɼǿ� ���� ����Ż�� �����ϰ� �̹����� �����Ͽ� ����ڿ��� �����մϴ�.
// �� ���α׷��� Flask �� �����ӿ�ũ�� ����Ͽ� �� �������̽��� �����ϰ�, C ���α׷��� ȣ���Ͽ� ����Ż�� �����ϰ� �̹����� �����մϴ�.
// �� ���α׷��� ����ڰ� ������ ����Ż�� ������ ���� ������ �����ϰ�, ����ڰ� �̹����� �ٿ�ε��� �� �ֵ��� �մϴ�.
// �� ���α׷��� ����ڰ� ������ ����Ż �̹����� �����ϰ�, ����ڰ� ������ �̹����� �ٿ�ε��� �� �ֵ��� �մϴ�.
// ������ ���α׷��� �ֿ� ����Դϴ�.
// 1. ����ڰ� ����Ż�� ������ ������ �� �ֽ��ϴ�.
// 2. ����ڰ� ����Ż�� ���� ������ ������ �� �ֽ��ϴ�.
// 3. ����ڰ� ����Ż �̹����� �����ϰ� �ٿ�ε��� �� �ֽ��ϴ�.
// 4. ����ڰ� ������ ����Ż �̹����� �����ϰ�, ����ڰ� ������ �̹����� �ٿ�ε��� �� �ֽ��ϴ�.
// ������ �ּ���.

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
