#파이썬으로 간단한 그림판으 만들려고해
#그림판의 기능은 선을 그릴수 있어
#선의 색깔을 바꿀수 있어
#선의 굵기를 바꿀수 있어
#그림판을 종료할수 있어
#그림판을 실행하면 선을 그리는 화면이 나와야해
#선을 그리는 화면에서는 마우스 왼쪽 버튼을 누른 상태로 마우스를 움직이면 선이 그려져야해
#선의 색깔을 바꾸려면 키보드의 r,g,b키를 누르면 선의 색깔이 빨강,초록,파랑으로 바뀌어야해
#선의 굵기를 바꾸려면 키보드의 +,-키를 누르면 선의 굵기가 두껍고 얇아져야해
#그림판을 종료하려면 키보드의 q키를 누르면 그림판이 종료되어야해
#그림을 png나 jpg로 저장이 가능해야돼
#그림판의 크기는 800x600이야
#그림판의 이름은 그림판이야
#그림판의 배경색은 흰색이야
#사진을 불러와서 그리기도 가능해야해
#사진을 불러오려면 키보드의 i키를 누르면 파일을 선택할수 있어
#선택한 파일은 그림판의 크기에 맞게 출력이 되어야해
#선을 그릴때 사진위에 그리면 사진이 보이지 않아야해
#선을 그릴때 사진아래에 그리면 사진이 보이야해

#---------------------------------------------
#pip install opencv-python를 다운 받아야 합니다.
# pip install numpy를 다운 받아야 합니다.
# 선 굵기를 할때 +버튼 눌러도 안되면 쉬프트키랑 같이 해보세요
import cv2
import numpy as np
import os

def draw_line(event, x, y, flags, param):
    global drawing, mode, ix, iy, color, thickness, img, img2
    if event == cv2.EVENT_LBUTTONDOWN:
        drawing = True
        ix, iy = x, y
    elif event == cv2.EVENT_MOUSEMOVE:
        if drawing == True:
            if mode == True:
                cv2.line(img, (ix, iy), (x, y), color, thickness)
                cv2.line(img2, (ix, iy), (x, y), color, thickness)
                ix, iy = x, y
            else:
                cv2.line(img, (ix, iy), (x, y), color, thickness)
                ix, iy = x, y
    elif event == cv2.EVENT_LBUTTONUP:
        drawing = False
        if mode == True:
            cv2.line(img, (ix, iy), (x, y), color, thickness)
            cv2.line(img2, (ix, iy), (x, y), color, thickness)

def main():
    global drawing, mode, ix, iy, color, thickness, img, img2
    drawing = False
    mode = True
    ix, iy = -1, -1
    color = (0, 0, 0)
    thickness = 1
    img = np.full((600, 800, 3), 255, np.uint8)
    img2 = np.full((600, 800, 3), 255, np.uint8)
    cv2.namedWindow('image')
    cv2.setMouseCallback('image', draw_line)
    while True:
        cv2.imshow('image', img)
        k = cv2.waitKey(1) & 0xFF
        if k == ord('r'):
            color = (0, 0, 255)
        elif k == ord('g'):
            color = (0, 255, 0)
        elif k == ord('b'):
            color = (255, 0, 0)
        elif k == ord('+'):
             thickness += 1
        elif k == ord('-') and thickness > 1:
            thickness -= 1
        elif k == ord('q'):
            cv2.destroyAllWindows()
            break
        elif k == ord('s'):
            filename = input("저장할 파일 이름을 입력하세요: ")
            if not any(filename.lower().endswith(ext) for ext in ['.jpg', '.png', '.bmp']):
               filename += '.jpg'  # 기본 확장자 추가
            cv2.imwrite(filename, img2)
        elif k == ord('i'):
            filename = input("불러올 파일 이름을 입력하세요: ")
            if os.path.exists(filename):
                img2 = cv2.imread(filename)
                img2 = cv2.resize(img2, (800, 600))
            else:
                print("파일을 찾을 수 없습니다.")
        elif k == ord('c'):
            img = np.full((600, 800, 3), 255, np.uint8)
            img2 = np.full((600, 800, 3), 255, np.uint8)
        elif k == ord('m'):
            mode = not mode

if __name__ == "__main__":
    main()
