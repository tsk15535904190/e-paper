import serial
import sys
import time
import numpy as np
# from upper_computer.get_api_img import floyd_steinberg_dithering , fetch_display_dithered_image
from paint import img_pil_draw,Convert_gray_to_8bit,display_img
from get_weather import get_weather_info ,get_word_of_day
from get_memo import get_memo_info

E_PAPER_WIDTH = 400
E_PAPER_HIGH = 300

COM = sys.argv[1]
ser = serial.Serial()
np.set_printoptions(threshold=np.inf)


#打开串口
if(COM[:3].upper() == "COM"):
    ser = serial.Serial(COM.upper(), 115200 , timeout=1)
    ser.setRTS(True)
else:
    print("COMx ERROR!")


##创建两个显存，分别对应红黑像素
image_black = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)
image_red = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)



def img_process(img1,img2):
    image_black = img_pil_draw(img1,"你好，世界",(50,150),40)
    image_red = img_pil_draw(img2,"你好，华夏",(50,150),28)
    return image_black,image_red



def uart_send(send_list):
    return 1
    IMG_index = 0 
    PACKET_LEN = 100
    print(ser.readall().decode())
    time.sleep(2)

    #发送开始命令
    ser.write('S'.encode('utf-8'))
    data = ser.read(1).decode()
    if data == 'S' :
        print("start tran!")
        #开启传输咯
        while(IMG_index < len(send_list)):
            packet_array = send_list[IMG_index:IMG_index + PACKET_LEN]
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

print('start.')
image_black,image_red = img_process(image_black,image_red)
display_img(image_black,image_red)
print(get_weather_info())
print(get_word_of_day())
send_list = Convert_gray_to_8bit(image_black,image_red)
uart_send(send_list)
print('over.')