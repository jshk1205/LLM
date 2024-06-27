// 수학적 프랙탈(예: Mandelbrot 집합, Julia 집합)을 생성하고 시각화하는 프로그램을 개발합니다.
// 프로그램은 사용자가 프랙탈의 종류와 세부 사항을 선택할 수 있어야 합니다.
// SFML 라이브러리를 사용하지 않고 개발해 주세요.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

// 프랙탈의 종류를 나타내는 열거형
enum FractalType
{
    MANDELBROT,
    JULIA
};

// 복소수 클래스
class Complex
{
public:
    Complex(double real, double imag) : real(real), imag(imag) {}
    Complex operator+(const Complex& c) const
    {
        return Complex(real + c.real, imag + c.imag);
    }
    Complex operator*(const Complex& c) const
    {
        return Complex(real * c.real - imag * c.imag, real * c.imag + imag * c.real);
    }
    double abs() const
    {
        return sqrt(real * real + imag * imag);
    }

    double real;
    double imag;
};

// 프랙탈 클래스
class Fractal
{
public:
    Fractal(FractalType type, int width, int height, int maxIter, double minReal, double maxReal, double minImag, double maxImag)
        : type(type), width(width), height(height), maxIter(maxIter), minReal(minReal), maxReal(maxReal), minImag(minImag), maxImag(maxImag)
    {
        data = new int[width * height];
    }

    ~Fractal()
    {
        delete[] data;
    }

    void generate()
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                double real = minReal + (maxReal - minReal) * x / width;
                double imag = minImag + (maxImag - minImag) * y / height;

                Complex c(real, imag);
                Complex z(0, 0);

                int iter = 0;
                while (z.abs() < 2 && iter < maxIter)
                {
                    if (type == MANDELBROT)
                    {
                        z = z * z + c;
                    }
                    else if (type == JULIA)
                    {
                        z = z * z + Complex(-0.8, 0.156);
                    }
                    iter++;
                }

                data[y * width + x] = iter;
            }
        }
    }

    void save(const string& filename)
    {
        ofstream file(filename);
        file << "P2" << endl;
        file << width << " " << height << endl;
        file << "255" << endl;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                file << data[y * width + x] * 255 / maxIter << " ";
            }
            file << endl;
        }

        file.close();
    }

private:
    FractalType type;
    int width;
    int height;
    int maxIter;
    double minReal;
    double maxReal;
    double minImag;
    double maxImag;
    int* data;
};

int main()
{
    Fractal fractal(JULIA, 800, 800, 100, -1.5, 1.5, -1.5, 1.5);
    fractal.generate();
    fractal.save("julia.pgm");

    return 0;
}
