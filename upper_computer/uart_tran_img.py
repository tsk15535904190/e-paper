import serial
import sys
import time
import numpy as np
import cv2
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
    image_black = img1
    image_red = img2
    ##绘制每日一言
    word = get_word_of_day()
    if word :
        word_str = word['payload'] + '---' + word['from']
        print(word_str)
    else:
        word_str = "每日一言获取错误!"
    word_srt_len = len(word_str)
    image_black = img_pil_draw(image_black,word_str,(max(0,400 - 14 * word_srt_len), 280),14)

    ##绘制天气信息
    weather = get_weather_info()
    image_red = img_pil_draw(image_red,weather['location'],(0,0),48)

    image_red = img_pil_draw(image_red,weather['today_daily_forecasts']['date'],(0,50),18)
    image_black = img_pil_draw(image_black,weather['today_daily_forecasts']['text_day'],(0,68),18)
    image_black = img_pil_draw(image_black,'温度:'+ weather['today_daily_forecasts']['low'] + '~' + weather['today_daily_forecasts']['high'] ,(0,86),18)
    image_black = img_pil_draw(image_black,'降雨概率:'+weather['today_daily_forecasts']['rainfall'] ,(0,104),18)
    ##绘制备忘录
    tips = get_memo_info()
    if tips :

        tip_top_left = (100, 0)
        tip_bottom_right = (400, 40)

        cv2.rectangle(image_red, tip_top_left, tip_bottom_right, 255, -1)
        image_red = img_pil_draw(image_red,"Task",(100,0),40,text_color = 0)
        tip_x = 100
        tip_y = 50
        for tip in tips:
            if tip.startswith('*'):
                image_red = img_pil_draw(image_red,tip[1:],(tip_x,tip_y),16) #星标事件为红色
            elif tip.startswith('-'):
                image_black = img_pil_draw(image_black,tip[1:],(tip_x,tip_y),16) #完成事件为黑色
                cv2.line(image_red, (tip_x,tip_y + 8), (tip_x + len(tip) * 16,tip_y + 8), 255, 1) #完成事件画线使用红色
            else :
                image_black = img_pil_draw(image_black,tip,(tip_x,tip_y),16) #普通事件使用黑色
            tip_y = tip_y + 16
    else :
        print("error")

    
    ##绘制边框
    cv2.line(image_red, (0,0), (400,0), 255, 2)
    cv2.line(image_red, (0,0), (0,300), 255, 2)
    cv2.line(image_red, (400,0), (400,300), 255, 2)
    cv2.line(image_red, (0,300), (400,300), 255, 2)



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
# print(get_weather_info())
# print(get_word_of_day())
send_list = Convert_gray_to_8bit(image_black,image_red)
uart_send(send_list)
print('over.')