#암호화 복호화 프로그램을 만들어줘
#사용자가 직접 파일을 입력하는게 아닌 파일을 열어야됨(Python의 tkinter 라이브러리와 filedialog 모듈을 사용) 암호화 복호화를 할 수 있어야해
#암호화 복호화 방식은 AES를 사용해줘
# 암호화 복호화 방식은 같은 방식을 사용해야해 
# 1.암호화
# 2.복호화
# 3.종료
# pip install pycryptodome 설치해야됨

#사용법
#test.txt에다가 원하는 문장을 입력하고 
#1. 암호화 를 선택하고 test.txt파일을 선택하고 비밀번호를 입력한다.
#2. 그러면 encrypted_test.txt 파일이 생성되고 그 파일을 선택하고 비밀번호를 입력하면 test.txt파일이 생성된다.
#3. test.txt에 원래 써있던 문장을 지우고 복호화를 하면 됩니다.
import os
from Crypto.Cipher import AES
from Crypto.Hash import SHA256
from Crypto import Random
from tkinter import *
from tkinter import filedialog
from tkinter import messagebox


def encrypt(key, filename):
    chunksize = 64*1024
    dir_name, file_name = os.path.split(filename)
    outputFile = os.path.join(dir_name, "encrypted_" + file_name)
    filesize = str(os.path.getsize(filename)).zfill(16)
    IV = Random.new().read(16)

    encryptor = AES.new(key, AES.MODE_CBC, IV)

    with open(filename, 'rb') as infile:
        with open(outputFile, 'wb') as outfile:
            outfile.write(filesize.encode('utf-8'))
            outfile.write(IV)

            while True:
                chunk = infile.read(chunksize)

                if len(chunk) == 0:
                    break
                elif len(chunk) % 16 != 0:
                    chunk += b' ' * (16 - (len(chunk) % 16))

                outfile.write(encryptor.encrypt(chunk))

            
def decrypt(key, filename):
    chunksize = 64*1024
    dir_name, file_name = os.path.split(filename)
    if file_name.startswith("encrypted_"):
        file_name = file_name[len("encrypted_"):]
    outputFile = os.path.join(dir_name, file_name)

    with open(filename, 'rb') as infile:
        filesize = int(infile.read(16))
        IV = infile.read(16)

        decryptor = AES.new(key, AES.MODE_CBC, IV)

        with open(outputFile, 'wb') as outfile:
            while True:
                chunk = infile.read(chunksize)

                if len(chunk) == 0:
                    break

                outfile.write(decryptor.decrypt(chunk))
            outfile.truncate(filesize)

def getKey(password):
    hasher = SHA256.new(password.encode('utf-8'))
    return hasher.digest()

def Main():
    root = Tk()
    root.withdraw()  # Hide the main window
    choice = input("Would you like to (E)ncrypt or (D)ecrypt?: ")

    if choice == 'E':
        filename = filedialog.askopenfilename()  # Show the "Open" dialog box and return the path to the selected file
        password = input("Password: ")
        encrypt(getKey(password), filename)
        print("Done.")
    elif choice == 'D':
        filename = filedialog.askopenfilename()  # Show the "Open" dialog box and return the path to the selected file
        password = input("Password: ")
        decrypt(getKey(password), filename)
        print("Done.")
    else:
        print("No Option selected, closing...")

if __name__ == '__main__':
    Main()
