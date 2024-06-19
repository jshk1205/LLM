#파이썬으로 음악 플레이어를 만들어줘요.
#음악 파일을 재생하고 일시정지, 다음 곡, 이전 곡을 재생할 수 있어야 합니다.
#음악 파일을 불러오는 기능도 있으면 좋겠죠?
#음악 파일을 불러오고 재생하는 기능을 가진 음악 플레이어를 만들어주세요.
#음악 파일은 mp3 파일로 제한합니다.
#음악 파일을 불러오는 기능은 파일을 직접 선택하는 방식으로 구현해주세요.
#음악 파일을 재생하는 기능은 play, pause, stop, next, prev 기능을 구현해주세요.
#음악 파일을 재생할 때는 현재 재생 중인 음악 파일의 이름이 출력되어야 합니다.
#음악 파일을 불러올 때는 파일의 경로가 출력되어야 합니다.
#음악 플레이어의 기능을 선택할 수 있는 메뉴를 구성해주세요.


import pygame
from tkinter import *
from tkinter import filedialog
from pygame import mixer

def play():
    global paused
    if not playlist:  # 플레이리스트가 비어있는 경우
        print("파일을 먼저 선택하세요")
        return
    filename = playlist[current_track_index]  # 현재 재생할 파일
    try:
        pygame.mixer.init()  # mixer 초기화
        pygame.mixer.music.load(filename)  # 파일 불러오기
        pygame.mixer.music.play()  # 파일 재생
        print(f"{filename} 재생 중...")
        paused = False
    except Exception as e:
        print(f"재생 중 오류 발생: {e}")

def pause():
    global paused
    mixer.music.pause()
    paused = True

def stop():
    mixer.music.stop()

def load():
    global playlist
    root = Tk()
    root.withdraw()  # Tkinter 창을 숨깁니다.
    files = filedialog.askopenfilenames(filetypes=[("MP3 files", "*.mp3")])  # 다중 선택 가능
    playlist = list(files)  # 선택된 파일들을 플레이리스트에 추가
    if playlist:
        print(f"플레이리스트: {playlist}")
    else:
        print("파일이 선택되지 않았습니다.")

def next():
    global current_track_index
    if playlist:  # 플레이리스트가 비어있지 않은 경우
        current_track_index = (current_track_index + 1) % len(playlist)  # 다음 곡으로 이동
        filename = playlist[current_track_index]
        play()

def prev():
    global current_track_index
    if playlist:  # 플레이리스트가 비어있지 않은 경우
        current_track_index = (current_track_index - 1) % len(playlist)  # 이전 곡으로 이동
        filename = playlist[current_track_index]
        play()

def main():
    global filename, paused, playlist, current_track_index
    filename = ""
    paused = False
    playlist = []  # 플레이리스트 초기화
    current_track_index = 0  # 현재 재생 중인 곡의 인덱스


    # mixer 모듈 초기화
    pygame.mixer.init()

    root = Tk()
    root.title("음악 플레이어")
    root.geometry("300x200")

    play_button = Button(root, text="Play", command=play)
    play_button.pack()

    pause_button = Button(root, text="Pause", command=pause)
    pause_button.pack()

    stop_button = Button(root, text="Stop", command=stop)
    stop_button.pack()

    next_button = Button(root, text="Next", command=next)
    next_button.pack()

    prev_button = Button(root, text="Prev", command=prev)
    prev_button.pack()

    load_button = Button(root, text="Load", command=load)
    load_button.pack()

    root.mainloop()

    # 프로그램 종료 시 mixer 모듈 정리 (선택적)
    # pygame.mixer.quit()

if __name__ == "__main__":
    main()



