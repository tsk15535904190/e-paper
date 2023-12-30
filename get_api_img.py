import requests,json
import json
import cv2
import numpy as np


pic_url = "https://api.mwm.moe/ai/"

def floyd_steinberg_dithering(image):
    # 将图像转换为灰度图
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # Floyd-Steinberg抖动方法
    for y in range(1, gray_image.shape[0] - 1):
        for x in range(1, gray_image.shape[1] - 1):
            old_pixel = gray_image[y, x]
            new_pixel = 255 if old_pixel > 115 else 0
            gray_image[y, x] = new_pixel
            error = old_pixel - new_pixel

            # 传播误差
            gray_image[y, x + 1] += error * 7 / 23
            gray_image[y + 1, x - 1] += error * 5 / 23
            gray_image[y + 1, x] += error * 3 / 23
            gray_image[y + 1, x + 1] += error * 1 / 23

    return gray_image

def fetch_display_dithered_image(api_url = "https://api.mwm.moe/ai/"):
    # 发送GET请求获取图片数据
    response = requests.get(api_url)
    
    if response.status_code == 200:
        # 从响应中获取图片数据
        image_data = response.content
        
        # 将图片数据转换为numpy数组
        nparr = np.frombuffer(image_data, np.uint8)
        
        # 使用OpenCV解码图像
        original_image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

        return original_image
    else:
        print(f"Failed to fetch image. Status code: {response.status_code}")
        exit()