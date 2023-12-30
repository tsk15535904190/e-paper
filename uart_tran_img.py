import serial
import sys
import time
import cv2
import numpy as np
from get_api_img import floyd_steinberg_dithering , fetch_display_dithered_image

COM = sys.argv[1]
ser = serial.Serial()
np.set_printoptions(threshold=np.inf)


#打开串口
if(COM[:3].upper() == "COM"):
    ser = serial.Serial(COM.upper(), 115200 , timeout=1)
    ser.setRTS(True)
else:
    print("COMx ERROR!")

#读取图片
color_image = cv2.imread('test.jpg')
    
# pic_url1 = " "
# pic_url2 = "https://api.likepoems.com/img/pc/"
# color_image = fetch_display_dithered_image(api_url=pic_url2)

#将图像缩放为屏幕大小
color_image = cv2.resize(color_image, (400, 300),interpolation=cv2.INTER_AREA)  # 替换为你点阵屏幕的宽度和高度
#将图像转换为散点图
gay_image = floyd_steinberg_dithering(color_image)
# 调整灰度图像的大小，以适应点阵屏幕
# cv2.imshow('Dithered Image from API', gay_image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
# exit()
# 将灰度图像二值化为0或1
binary_image = (gay_image > 128).astype(np.uint8)
# 将每8个像素组合成一个字节
byte_array = np.packbits(binary_image, axis=1)
resized_array = np.resize(byte_array, 15000)
IMG_list = resized_array.tolist()

IMG_index = 0 
PACKET_LEN = 100
print(ser.readall().decode())
time.sleep(2)
print('start.')
#发送开始命令
ser.write('S'.encode('utf-8'))
data = ser.read(1).decode()
if data == 'S' :
    print("start tran!")
    #开启传输咯
    while(IMG_index < len(IMG_list)):
        packet_array = IMG_list[IMG_index:IMG_index + PACKET_LEN]
        ser.write(bytes(packet_array))
        IMG_index = IMG_index + PACKET_LEN
        read_data = ser.readline().decode()
        if 'T' in read_data :
            print(read_data)
        else:
            print('TIME OUT!')
            exit()
else :
    print("ERROR",data)
#发送结束命令
ser.write('E'.encode('utf-8'))
if 'E' == ser.read(1).decode() :
    print('end.')
print('over.')