# 수학적 프랙탈(예: Mandelbrot 집합, Julia 집합)을 생성하고 시각화하는 프로그램을 개발합니다.
# 이 프로그램은 사용자가 프랙탈의 종류와 다양한 매개변수를 선택할 수 있어야 합니다.
# 프로그램은 사용자가 선택한 매개변수를 사용하여 프랙탈을 생성하고 화면에 표시해야 합니다.
# 프로그램은 사용자가 프랙탈을 확대하고 이동할 수 있는 기능을 제공해야 합니다.
# 프로그램은 사용자가 프랙탈을 이미지 파일로 저장할 수 있는 기능을 제공해야 합니다.

import tkinter as tk

class FractalApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Fractal Generator")
        self.master.geometry("800x600")
        self.master.resizable(False, False)

        self.fractal_type = tk.StringVar()
        self.fractal_type.set("Mandelbrot")

        self.fractal_type_label = tk.Label(self.master, text="Fractal Type:")
        self.fractal_type_label.grid(row=0, column=0, padx=5, pady=5)

        self.fractal_type_menu = tk.OptionMenu(self.master, self.fractal_type, "Mandelbrot", "Julia")
        self.fractal_type_menu.grid(row=0, column=1, padx=5, pady=5)

        self.generate_button = tk.Button(self.master, text="Generate", command=self.generate_fractal)
        self.generate_button.grid(row=0, column=2, padx=5, pady=5)

        self.canvas = tk.Canvas(self.master, width=600, height=600)
        self.canvas.grid(row=1, column=0, columnspan=3, padx=5, pady=5)

        self.canvas.bind("<Button-1>", self.zoom_in)
        self.canvas.bind("<Button-3>", self.zoom_out)

        self.canvas.bind("<B1-Motion>", self.move)
        self.canvas.bind("<B3-Motion>", self.move)

        self.fractal = None
        self.fractal_image = None
        self.fractal_x = -2.0
        self.fractal_y = -2.0
        self.fractal_width = 4.0
        self.fractal_height = 4.0

    def generate_fractal(self):
        if self.fractal_type.get() == "Mandelbrot":
            self.fractal = Mandelbrot()
        elif self.fractal_type.get() == "Julia":
            self.fractal = Julia()

        self.fractal_image = self.fractal.generate(self.fractal_x, self.fractal_y, self.fractal_width, self.fractal_height, 600, 600)
        self.canvas.create_image(0, 0, image=self.fractal_image, anchor="nw")

    def zoom_in(self, event):
        self.fractal_width /= 2.0
        self.fractal_height /= 2.0
        self.fractal_x = self.fractal_x + (event.x / 600) * self.fractal_width
        self.fractal_y = self.fractal_y + (event.y / 600) * self.fractal_height
        self.generate_fractal()

    def zoom_out(self, event):
        self.fractal_width *= 2.0
        self.fractal_height *= 2.0
        self.fractal_x = self.fractal_x - (event.x / 600) * self.fractal_width
        self.fractal_y = self.fractal_y - (event.y / 600) * self.fractal_height
        self.generate_fractal()

    def move(self, event):
        self.fractal_x -= (event.x - event.x_root) / 600 * self.fractal_width
        self.fractal_y -= (event.y - event.y_root) / 600 * self.fractal_height
        self.generate_fractal()

class Fractal:
    def generate(self, x, y, width, height, image_width, image_height):
        pass

class Mandelbrot(Fractal):
    def generate(self, x, y, width, height, image_width, image_height):
        image = tk.PhotoImage(width=image_width, height=image_height)

        for i in range(image_width):
            for j in range(image_height):
                zx = x + (i / image_width) * width
                zy = y + (j / image_height) * height

                c = complex(zx, zy)
                z = 0

                for k in range(256):
                    z = z * z + c

                    if abs(z) > 2.0:
                        break

                image.put("#%02x%02x%02x" % (k, k, k), (i, j))

        return image

class Julia(Fractal):
    def generate(self, x, y, width, height, image_width, image_height):
        image = tk.PhotoImage(width=image_width, height=image_height)

        for i in range(image_width):
            for j in range(image_height):
                zx = x + (i / image_width) * width
                zy = y + (j / image_height) * height

                z = complex(zx, zy)
                c = complex(-0.7, 0.27015)

                for k in range(256):
                    z = z * z + c

                    if abs(z) > 2.0:
                        break

                image.put("#%02x%02x%02x" % (k, k, k), (i, j))

        return image

def main():
    root = tk.Tk()
    app = FractalApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
