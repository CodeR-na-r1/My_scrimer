from PIL import Image, ImageTk
import winsound, tkinter

def make_sound():
    filename = "scrimer_sound.wav"
    winsound.PlaySound(filename, winsound.SND_FILENAME)

root = tkinter.Tk() # Создём окно

width_img = int(root.winfo_screenwidth())   # Вытаскиваем размеры монитора
height_img = int(root.winfo_screenheight())
root.geometry(str(width_img) + "x" + str(height_img))   # Подгоняем размеры приложения

img = Image.open("scrimer.jpg")   # Открываем картинку и подгоняем под размеры монитора
ratio = (width_img / float(img.size[0]))
height_img = int((float(img.size[1]) * float(ratio)))
imag = img.resize((width_img, height_img), Image.ANTIALIAS)   # Применяем

image = ImageTk.PhotoImage(imag)
panel = tkinter.Label(root, image=image)
panel.pack(side="top", fill="both", expand="no")
root.attributes('-fullscreen', True)   # Полноэкранный режим

root.after(80, make_sound)   # Подрубаем звук
root.mainloop()
