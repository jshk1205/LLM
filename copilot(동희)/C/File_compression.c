// 우분투 환경에서 C언어로  ZIP 파일 압축 프로그램을 만들어줘
// ZIP 파일 압축 프로그램은 다음과 같은 기능을 제공해야 합니다.
// 1. ZIP 파일 압축 프로그램은 파일을 압축할 수 있어야 합니다.
// 2. ZIP 파일 압축 프로그램은 압축된 파일을 해제할 수 있어야 합니다.
// 3. ZIP 파일 압축 프로그램은 압축된 파일의 목록을 출력할 수 있어야 합니다.
// 4. ZIP 파일 압축 프로그램은 압축된 파일의 내용을 출력할 수 있어야 합니다.
// 5. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 이름을 변경할 수 있어야 합니다.
// 6. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 압축률을 설정할 수 있어야 합니다.
// 7. ZIP 파일 압축 프로그램은 파일을 압축할 때, 파일의 압축 방법을 설정할 수 있어야 합니다.

// git clone https://github.com/Microsoft/vcpkg.git.\vcpkg\bootstrap-vcpkg.bat
//# zlib 소스 코드 다운로드
//wget https://zlib.net/zlib-1.2.11.tar.gz

//# 압축 해제
//tar -xzvf zlib-1.2.11.tar.gz

//# PowerShell을 사용한 zlib 소스 코드 다운로드
//Invoke-WebRequest -Uri https://zlib.net/zlib-1.2.11.tar.gz -OutFile zlib-1.2.11.tar.gz
//# 압축 해제는 수동으로 하거나 적절한 명령어/프로그램을 사용

// 프로그램 실행시 gcc File_compression.c -o File_compression -L/path/to/zlib/lib -lz
// ./File_compression
// compress [file_name] [out_file_name] : 파일을 압축합니다.
// decompress [file_name] [out_file_name] : 파일을 압축 해제합니다.
// list [file_name] : 압축된 파일의 목록을 출력합니다.
// show [file_name] : 압축된 파일의 내용을 출력합니다.
// exit : 프로그램을 종료합니다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define CHUNK 16384

void compress_file(const char *file_name, const char *out_file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", file_name);
        return;
    }

    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", out_file_name);
        fclose(file);
        return;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        fprintf(stderr, "Error: Failed to initialize zlib\n");
        fclose(file);
        fclose(out_file);
        return;
    }

    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    int ret;
    do
    {
        stream.avail_in = fread(in, 1, CHUNK, file);
        if (ferror(file))
        {
            fprintf(stderr, "Error: Failed to read file %s\n", file_name);
            deflateEnd(&stream);
            fclose(file);
            fclose(out_file);
            return;
        }
        stream.next_in = in;

        do
        {
            stream.avail_out = CHUNK;
            stream.next_out = out;
            ret = deflate(&stream, stream.avail_in == 0 ? Z_FINISH : Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR)
            {
                fprintf(stderr, "Error: Failed to compress file %s\n", file_name);
                deflateEnd(&stream);
                fclose(file);
                fclose(out_file);
                return;
            }
            fwrite(out, 1, CHUNK - stream.avail_out, out_file);
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    deflateEnd(&stream);
    fclose(file);
    fclose(out_file);
}

void decompress_file(const char *file_name, const char *out_file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", file_name);
        return;
    }

    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", out_file_name);
        fclose(file);
        return;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if (inflateInit(&stream) != Z_OK)
    {
        fprintf(stderr, "Error: Failed to initialize zlib\n");
        fclose(file);
        fclose(out_file);
        return;
    }

    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    int ret;
    do
    {
        stream.avail_in = fread(in, 1, CHUNK, file);
        if (ferror(file))
        {
            fprintf(stderr, "Error: Failed to read file %s\n", file_name);
            inflateEnd(&stream);
            fclose(file);
            fclose(out_file);
            return;
        }
        stream.next_in = in;

        do
        {
            stream.avail_out = CHUNK;
            stream.next_out = out;
            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR)
            {
                fprintf(stderr, "Error: Failed to decompress file %s\n", file_name);
                inflateEnd(&stream);
                fclose(file);
                fclose(out_file);
                return;
            }
            fwrite(out, 1, CHUNK - stream.avail_out, out_file);
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(file);
    fclose(out_file);
}

void list_files(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", file_name);
        return;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if (inflateInit(&stream) != Z_OK)
    {
        fprintf(stderr, "Error: Failed to initialize zlib\n");
        fclose(file);
        return;
    }

    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    int ret;
    do
    {
        stream.avail_in = fread(in, 1, CHUNK, file);
        if (ferror(file))
        {
            fprintf(stderr, "Error: Failed to read file %s\n", file_name);
            inflateEnd(&stream);
            fclose(file);
            return;
        }
        stream.next_in = in;

        do
        {
            stream.avail_out = CHUNK;
            stream.next_out = out;
            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR)
            {
                fprintf(stderr, "Error: Failed to decompress file %s\n", file_name);
                inflateEnd(&stream);
                fclose(file);
                return;
            }
            fwrite(out, 1, CHUNK - stream.avail_out, stdout);
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(file);
}

void show_file(const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Failed to open file %s\n", file_name);
        return;
    }

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    if (inflateInit(&stream) != Z_OK)
    {
        fprintf(stderr, "Error: Failed to initialize zlib\n");
        fclose(file);
        return;
    }

    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    int ret;
    do
    {
        stream.avail_in = fread(in, 1, CHUNK, file);
        if (ferror(file))
        {
            fprintf(stderr, "Error: Failed to read file %s\n", file_name);
            inflateEnd(&stream);
            fclose(file);
            return;
        }
        stream.next_in = in;

        do
        {
            stream.avail_out = CHUNK;
            stream.next_out = out;
            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR)
            {
                fprintf(stderr, "Error: Failed to decompress file %s\n", file_name);
                inflateEnd(&stream);
                fclose(file);
                return;
            }
            fwrite(out, 1, CHUNK - stream.avail_out, stdout);
        } while (stream.avail_out == 0);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(file);
}

int main()
{
    char command[256];
    while (1)
    {
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';

        char *token = strtok(command, " ");
        if (token == NULL)
        {
            continue;
        }

        if (strcmp(token, "compress") == 0)
        {
            char *file_name = strtok(NULL, " ");
            char *out_file_name = strtok(NULL, " ");
            if (file_name == NULL || out_file_name == NULL)
            {
                fprintf(stderr, "Error: Invalid arguments\n");
                continue;
            }
            compress_file(file_name, out_file_name);
        }
        else if (strcmp(token, "decompress") == 0)
        {
            char *file_name = strtok(NULL, " ");
            char *out_file_name = strtok(NULL, " ");
            if (file_name == NULL || out_file_name == NULL)
            {
                fprintf(stderr, "Error: Invalid arguments\n");
                continue;
            }
            decompress_file(file_name, out_file_name);
        }
        else if (strcmp(token, "list") == 0)
        {
            char *file_name = strtok(NULL, " ");
            if (file_name == NULL)
            {
                fprintf(stderr, "Error: Invalid arguments\n");
                continue;
            }
            list_files(file_name);
        }
        else if (strcmp(token, "show") == 0)
        {
            char *file_name = strtok(NULL, " ");
            if (file_name == NULL)
            {
                fprintf(stderr, "Error: Invalid arguments\n");
                continue;
            }
            show_file(file_name);
        }
        else if (strcmp(token, "exit") == 0)
        {
            break;
        }
        else
        {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }

    return 0;
}

