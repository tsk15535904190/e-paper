import serial
import sys
import time
import numpy as np
import cv2
from datetime import datetime
import os
import time
# from upper_computer.get_api_img import floyd_steinberg_dithering , fetch_display_dithered_image
from paint import img_pil_draw,Convert_gray_to_8bit,display_img
from get_weather import get_weather_info ,get_word_of_day
from get_memo import get_memo_info , get_memo_path

E_PAPER_WIDTH = 400
E_PAPER_HIGH = 300

daytime_start = 6
daytime_end = 18

COM = sys.argv[1]
ser = serial.Serial()
np.set_printoptions(threshold=np.inf)


#打开串口
if(COM[:3].upper() == "COM"):
    ser = serial.Serial(COM.upper(), 115200 , timeout=1)
    ser.setRTS(True)
else:
    print("COMx ERROR! exit")
    exit()

def img_process(img1,img2):
    current_time = datetime.now()
    print(current_time)
    current_hour = current_time.hour
    night = 0
    if daytime_start <= current_hour < daytime_end:
        night = 0
    else:
        night = 1

    image_black = img1
    image_red = img2

    sidebar_thickness = 2
    ##绘制每日一言
    word = get_word_of_day()
    if word :
        word_str = word['payload'] + '---' + word['from']
    else:
        word_str = "每日一言获取错误!"
    word_str_len = len(word_str)
    word_str_font_size = 16
    image_black = img_pil_draw(image_black,word_str,(max(0,400 - word_str_font_size * word_str_len), 300 - word_str_font_size - sidebar_thickness),word_str_font_size)

    ##绘制天气信息
    weather = get_weather_info()
    # print(weather)
    image_red = img_pil_draw(image_red,weather['location'],(0,0),44)

    if night :
        icon_path = './icon/' + weather['today_daily_forecasts']['code_night'] + '@1x.png'
    else :
        icon_path = './icon/' + weather['today_daily_forecasts']['code_day'] + '@1x.png'
    icon_image = cv2.imread(icon_path, cv2.IMREAD_UNCHANGED)
    icon_image = cv2.resize(icon_image,(32,32))
    icon_height, icon_width = icon_image.shape[:2]
    icon_x_pos = 90
    icon_y_pos = 10
    for c in range(0, 3):
        # 将图标中的 alpha 通道应用到灰度图上
        image_red[icon_y_pos:icon_y_pos + icon_height, icon_x_pos:icon_x_pos + icon_width] = \
            image_red[icon_y_pos:icon_y_pos + icon_height, icon_x_pos:icon_x_pos + icon_width] * (1 - icon_image[:, :, 3] / 255.0) + \
            icon_image[:, :, c] * (icon_image[:, :, 3] / 255.0)
        
    image_red = img_pil_draw(image_red,weather['today_daily_forecasts']['date'],(10,50),22)

    if night :
        image_black = img_pil_draw(image_black,'天气:' + weather['today_daily_forecasts']['text_night'],(10,68),18)
    else :
        image_black = img_pil_draw(image_black,'天气:' + weather['today_daily_forecasts']['text_day'],(10,68),18)
    image_black = img_pil_draw(image_black,'温度:'+ weather['today_daily_forecasts']['low'] + '~' + weather['today_daily_forecasts']['high'] + '℃',(10,86),18)
    image_black = img_pil_draw(image_black,'降水:'+weather['today_daily_forecasts']['rainfall'] + '%' ,(10,104),18)
    image_black = img_pil_draw(image_black,'风向:'+weather['today_daily_forecasts']['wind_direction'] ,(10,122),18)
    image_black = img_pil_draw(image_black,'风速:'+weather['today_daily_forecasts']['wind_speed'] + 'm/s',(10,140),18)
    image_black = img_pil_draw(image_black,'湿度:'+weather['today_daily_forecasts']['humidity']  + '%',(10,158),18)

    image_red = img_pil_draw(image_red,weather['tomorrow_daily_forecasts']['date'],(10,180),20)

    image_black = img_pil_draw(image_black,'天气:' + weather['tomorrow_daily_forecasts']['text_day'],(10,198),18)
    image_black = img_pil_draw(image_black,'温度:'+ weather['tomorrow_daily_forecasts']['low'] + '~' + weather['today_daily_forecasts']['high'] + '℃',(10,216),18)
    image_black = img_pil_draw(image_black,'湿度:'+weather['tomorrow_daily_forecasts']['humidity'] + '%',(10,234),18)

    ##绘制备忘录
    tips = get_memo_info()
    if tips :

        tip_top_left = (130, 0)
        tip_bottom_right = (400, 40)

        cv2.rectangle(image_red, tip_top_left, tip_bottom_right, 255, -1)
        image_red = img_pil_draw(image_red,"Task",(140,0),40,text_color = 0)
        tip_x = 130
        tip_y = 50
        for tip in tips:
            if tip.startswith('*'):
                image_red = img_pil_draw(image_red,tip[1:],(tip_x,tip_y),20) #星标事件为红色
            elif tip.startswith('-'):
                image_black = img_pil_draw(image_black,tip[1:],(tip_x,tip_y),20) #完成事件为黑色
                cv2.line(image_red, (tip_x,tip_y + 10), (tip_x + len(tip) * 20,tip_y + 10), 255, 2) #完成事件画线使用红色
            else :
                image_black = img_pil_draw(image_black,tip,(tip_x,tip_y),20) #普通事件使用黑色
            tip_y = tip_y + 20
    else :
        print("error")

    
    ##绘制边框
    cv2.line(image_red, (0,0), (400,0), 255, sidebar_thickness)
    cv2.line(image_red, (0,0), (0,300), 255, sidebar_thickness)
    cv2.line(image_red, (400,0), (400,300), 255, sidebar_thickness)
    cv2.line(image_red, (0,300), (400,300), 255, sidebar_thickness)

    cv2.line(image_red, (130,0), (130,300 - 20), 255, sidebar_thickness - 1)
    cv2.line(image_red, (0,300 - 20), (400,300 - 20), 255, sidebar_thickness - 1)


    return image_black,image_red



def uart_send(send_list):
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

def draw_task():
    ##创建两个显存，分别对应红黑像素
    image_black = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)
    image_red = np.zeros((E_PAPER_HIGH, E_PAPER_WIDTH), dtype=np.uint8)
    ##图像处理
    image_black,image_red = img_process(image_black,image_red)
    ##图像拼接并显示图像
    # display_img(image_black,image_red)
    ##数据整理并发送
    send_list = Convert_gray_to_8bit(image_black,image_red)
    uart_send(send_list)
print('over.')



file_path = get_memo_path()
# 获取文件的初始修改时间
initial_modification_time = os.path.getmtime(file_path)
# 脚本开始运行后，先进行一次屏幕刷新
draw_task()
last_run_time = time.time()
while True:
    # 获取当前文件的最新修改时间
    current_modification_time = os.path.getmtime(file_path)

    # 检查文件是否被修改，如果被修改，就
    if current_modification_time != initial_modification_time:
        print("文件修改运行一次")
        draw_task()
        last_run_time = time.time()
        initial_modification_time = current_modification_time

    if time.time() - last_run_time >= 3600:  # 3600秒 = 1小时
        print("一个小时运行一次")
        draw_task()

        # 更新上次运行任务的时间
        last_run_time = time.time()

    # 每个十分钟检查一次文件是否被修改
    time.sleep(60*10)