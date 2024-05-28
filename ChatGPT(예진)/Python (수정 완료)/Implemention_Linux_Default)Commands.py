import os

def ls(directory=".", show_hidden=False, long_format=False, human_readable=False):
    """
    ls 명령어를 파이썬 코드로 구현
    """
    files = os.listdir(directory)
    if not show_hidden:
        files = [file for file in files if not file.startswith(".")]

    if long_format:
        for file in files:
            file_stat = os.stat(os.path.join(directory, file))
            if human_readable:
                size = _human_readable_size(file_stat.st_size)
            else:
                size = file_stat.st_size
            print(f"{file_stat.st_mode:10o} {file_stat.st_nlink} {file_stat.st_uid} {file_stat.st_gid} {size} {file_stat.st_mtime} {file}")
    else:
        for file in files:
            print(file)

def cat(filename, number=False, start=None, end=None):
    """
    cat 명령어를 파이썬 코드로 구현
    """
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()
            if start is None:
                start = 0
            if end is None:
                end = len(lines)
            for i in range(start, min(end, len(lines))):
                if number:
                    print(f"{i+1}: {lines[i]}", end='')
                else:
                    print(lines[i], end='')
    except FileNotFoundError:
        print(f"{filename}: No such file or directory")

def echo(text, file=None, append=False):
    """
    echo 명령어를 파이썬 코드로 구현
    """
    if file:
        mode = 'a' if append else 'w'
        with open(file, mode) as f:
            f.write(text)
    else:
        print(text)

def _human_readable_size(size):
    """
    파일 크기를 사람이 읽기 쉬운 형식으로 변환
    """
    units = ['B', 'KB', 'MB', 'GB', 'TB']
    index = 0
    while size >= 1024 and index < len(units)-1:
        size /= 1024
        index += 1
    return f"{size:.2f} {units[index]}"

# ls 명령어 테스트
print("ls 명령어 실행 결과:")
ls(".", show_hidden=True, long_format=True, human_readable=True)

# cat 명령어 테스트
print("\ncat 명령어 실행 결과:")
cat("example.txt", number=True, start=2, end=4)

# echo 명령어 테스트
print("\necho 명령어 실행 결과:")
echo("Hello, world!\nThis is a test.\n", file="output.txt", append=True)
echo("Another line", file="output.txt", append=True)
cat("output.txt")
