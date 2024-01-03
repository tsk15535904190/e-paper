import cv2
import numpy as np
from PIL import ImageFont, ImageDraw, Image

np.set_printoptions(threshold=np.inf)

def img_pil_draw(img,text,text_position,font_size):
    pil_image = Image.fromarray(img)

    font_path = "fonts/STXINWEI.TTF"
    font = ImageFont.truetype(font_path, font_size)

    draw = ImageDraw.Draw(pil_image)
    text_color = 255
    draw.text(text_position, text, font=font, fill=text_color)
    return np.array(pil_image)

def Convert_gray_to_8bit(img1,img2):
    binary_image_black = (img1 > 100).astype(np.uint8)
    binary_image_red = (img2 > 100).astype(np.uint8)

    binary_image_black = np.logical_not(binary_image_black)

    byte_array_black = np.packbits(binary_image_black, axis=1)
    byte_array_red = np.packbits(binary_image_red, axis=1)

    resized_array_black = np.resize(byte_array_black, 15000)
    resized_array_red = np.resize(byte_array_red, 15000)
    IMG_list = resized_array_black.tolist() + resized_array_red.tolist()
    return IMG_list

def display_img(img1,img2):
    cv2.imshow('image_black', img1)
    cv2.imshow('image_red', img2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()