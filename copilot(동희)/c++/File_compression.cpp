// ZIP 파일 압축 프로그램은 다음과 같은 기능을 제공해야 합니다.
// 1. ZIP 파일 압축 프로그램은 파일을 압축할 수 있어야 합니다.
// 2. ZIP 파일 압축 프로그램은 압축된 파일을 해제할 수 있어야 합니다.
// 3. ZIP 파일 압축 프로그램은 압축된 파일의 목록을 출력할 수 있어야 합니다.
// 4. ZIP 파일 압축 프로그램은 압축된 파일의 내용을 출력할 수 있어야 합니다.
// 5. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 이름을 변경할 수 있어야 합니다.
// 6. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 압축률을 설정할 수 있어야 합니다.
// 7. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 압축 방법을 설정할 수 있어야 합니다.
// 우분투 환경에서 C++ 언어로  ZIP 파일 압축 프로그램을 만들어줘
// ZIP 파일 압축 프로그램은 다음과 같은 함수를 구현해야 합니다.
// void compress_file(char *filename, char *zipname, int level);
// void decompress_file(char *zipname);
// void list_files(char *zipname);
// void read_file(char *zipname, char *filename);
//

//g++ File_compression.cpp -o FileCompression -lz
//./FileCompression
//1. Compress file
//2. Decompress file
//3. List files
//4. Read file
//5. Exit
//Select menu: 1
//Enter filename: test.txt
//Enter zipname: test.zip
//Enter level: 1

//level은 압축률이라고 하는데 1이 가장 빠르고 9가 가장 느린거라 합니다
// 근데 느려서 그런건지 실행이 안되는건지 1로만 압축이 되더라고요 다른 레벨로 하면 파일이 나오긴 하는데 zip 파일이아닌
// 누르고 나서 텍스트 파일에 이상한 문자들이 나오더라고요

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <zlib.h>
#include <dirent.h>
#include <sys/stat.h>


void compress_file(char *filename, char *zipname, int level);
void decompress_file(char *zipname);
void list_files(char *zipname);
void read_file(char *zipname, char *filename);

int main()
{
    char filename[256];
    char zipname[256];
    int level;
    int menu;

    while (1)
    {
        std::cout << "1. Compress file" << std::endl;
        std::cout << "2. Decompress file" << std::endl;
        std::cout << "3. List files" << std::endl;
        std::cout << "4. Read file" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Select menu: ";
        std::cin >> menu;

        switch (menu)
        {
        case 1:
            std::cout << "Enter filename: ";
            std::cin >> filename;
            std::cout << "Enter zipname: ";
            std::cin >> zipname;
            std::cout << "Enter level: ";
            std::cin >> level;
            compress_file(filename, zipname, level);
            break;
        case 2:
            std::cout << "Enter zipname: ";
            std::cin >> zipname;
            decompress_file(zipname);
            break;
        case 3:
            std::cout << "Enter zipname: ";
            std::cin >> zipname;
            list_files(zipname);
            break;
        case 4:
            std::cout << "Enter zipname: ";
            std::cin >> zipname;
            std::cout << "Enter filename: ";
            std::cin >> filename;
            read_file(zipname, filename);
            break;
        case 5:
            return 0;
        default:
            std::cout << "Invalid menu" << std::endl;
            break;
        }
    }

    return 0;
} 

void compress_file(char *filename, char *zipname, int level)
{
    std::ifstream
        input(filename, std::ios::binary);
    std::ofstream
        output(zipname, std::ios::binary);
    z_stream stream;
    unsigned char buffer[1024];
    int ret;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (deflateInit2(&stream, level, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        std::cerr << "deflateInit2 failed" << std::endl;
        return;
    }

    stream.next_in = buffer;
    stream.avail_in = 0;

    do
    {
        input.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
        stream.avail_in = input.gcount();
        stream.next_in = buffer;

        do
        {
            stream.avail_out = sizeof(buffer);
            stream.next_out = buffer;

            ret = deflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "deflate failed" << std::endl;
                return;
            }

            output.write(reinterpret_cast<char *>(buffer), sizeof(buffer) - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (input);

    deflateEnd(&stream);
}

void decompress_file(char *zipname)
{
    std::ifstream
        input(zipname, std::ios::binary);
    std::ofstream
        output("decompressed", std::ios::binary);
    z_stream stream;
    unsigned char buffer[1024];
    int ret;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (inflateInit2(&stream, 15) != Z_OK)
    {
        std::cerr << "inflateInit2 failed" << std::endl;
        return;
    }

    stream.next_in = buffer;
    stream.avail_in = 0;

    do
    {
        input.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
        stream.avail_in = input.gcount();
        stream.next_in = buffer;

        do
        {
            stream.avail_out = sizeof(buffer);
            stream.next_out = buffer;

            ret = inflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "inflate failed" << std::endl;
                return;
            }

            output.write(reinterpret_cast<char *>(buffer), sizeof(buffer) - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (input);

    inflateEnd(&stream);
}

void list_files(char *zipname)
{
    std::ifstream
        input(zipname, std::ios::binary);
    z_stream stream;
    unsigned char buffer[1024];
    int ret;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (inflateInit2(&stream, 15) != Z_OK)
    {
        std::cerr << "inflateInit2 failed" << std::endl;
        return;
    }

    stream.next_in = buffer;
    stream.avail_in = 0;

    do
    {
        input.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
        stream.avail_in = input.gcount();
        stream.next_in = buffer;

        do
        {
            stream.avail_out = sizeof(buffer);
            stream.next_out = buffer;

            ret = inflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "inflate failed" << std::endl;
                return;
            }

            std::cout.write(reinterpret_cast<char *>(buffer), sizeof(buffer) - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (input);

    inflateEnd(&stream);
}

void read_file(char *zipname, char *filename)
{
    std::ifstream
        input(zipname, std::ios::binary);
    z_stream stream;
    unsigned char buffer[1024];
    int ret;

    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (inflateInit2(&stream, 15) != Z_OK)
    {
        std::cerr << "inflateInit2 failed" << std::endl;
        return;
    }

    stream.next_in = buffer;
    stream.avail_in = 0;

    do
    {
        input.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
        stream.avail_in = input.gcount();
        stream.next_in = buffer;

        do
        {
            stream.avail_out = sizeof(buffer);
            stream.next_out = buffer;

            ret = inflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "inflate failed" << std::endl;
                return;
            }

            std::cout.write(reinterpret_cast<char *>(buffer), sizeof(buffer) - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (input);

    inflateEnd(&stream);
}


