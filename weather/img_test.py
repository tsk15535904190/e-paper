import cv2
import numpy as np
from PIL import ImageFont, ImageDraw, Image

# 创建一个单色图像
image = np.zeros((300, 400), dtype=np.uint8)

# 转换为PIL Image
pil_image = Image.fromarray(image)

# 加载中文字体文件，这里使用SimHei.ttf，你可以替换为其他中文字体文件
font_path = "fonts/STXINWEI.TTF"
font_size = 40
font = ImageFont.truetype(font_path, font_size)

# 在图像上绘制汉字
draw = ImageDraw.Draw(pil_image)
text = "你好"
text_position = (50, 150)
text_color = 255  # 单通道图像只需设置一个灰度值
draw.text(text_position, text, font=font, fill=text_color)

# 将PIL Image转换回OpenCV格式
image_with_text = np.array(pil_image)

# 显示图像
cv2.imshow('Image with Chinese Text', image_with_text)
cv2.waitKey(0)
cv2.destroyAllWindows()
