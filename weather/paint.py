import cv2
import numpy as np
from PIL import ImageFont, ImageDraw, Image

E_PAPER_WIDTH = 400
E_PAPER_HIGH = 300

np.set_printoptions(threshold=np.inf)

def img_pil_draw(img,text,text_position,font_size):
    pil_image = Image.fromarray(img)

    font_path = "fonts/STXINWEI.TTF"
    font = ImageFont.truetype(font_path, font_size)

    draw = ImageDraw.Draw(pil_image)
    text_color = 255
    draw.text(text_position, text, font=font, fill=text_color)
    return np.array(pil_image)

#建立一张灰度图
image_black = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)
image_red = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)

image_black = img_pil_draw(image_black,"你好，世界",(50,150),40)

image_red = img_pil_draw(image_red,"你好，华夏",(50,150),28)

# 显示图像
cv2.imshow('image_black', image_black)
cv2.imshow('image_red', image_red)
cv2.waitKey(0)
cv2.destroyAllWindows()