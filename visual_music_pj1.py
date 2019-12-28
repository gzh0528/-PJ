# coding=gbk
import warnings
warnings.simplefilter("ignore", DeprecationWarning)#防止报警告
import numpy as np
import pygame
import pyaudio
import wave

#定义数据流块
CHUNK = 1024
#定义颜色
black = (0,0,0)
white = (255,255,255)
red = (255,0,0)
green = (0,255,0)
seashell=(255,245,238)
tomato=(255,99,71)
gold=(255,215,0)

#画跳动小人
def draw_man(screen,x,y):
    pygame.draw.ellipse(screen,seashell,[x,y-5,10,10])
    pygame.draw.line(screen,red,[5+x,5+y],[5+x,15+y],2)
    pygame.draw.line(screen,red,[5+x,5+y],[x-5,15+y],2)
    pygame.draw.line(screen,red,[5+x,5+y],[15+x,15+y],2)
    pygame.draw.line(screen,gold,[5+x,15+y],[x-5,25+y],2)
    pygame.draw.line(screen,gold,[5+x,15+y],[15+x,25+y],2)

'''
Visual函数说明
1.读入 wav音乐文件
2.创建pyaudio播放器
3.打开数据流
4.逐块读取音乐数据

'''
def visual_music(wave_file,count_freq):
    wf=wave_file
    player = pyaudio.PyAudio()
    stream = player.open(format=player.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),#设置声道数
                    rate=wf.getframerate(),#设置流的频率
                    output=True)

    data = wf.readframes(CHUNK)
    pygame.init()#pygame初始化
    pygame.display.set_caption('可视化音乐频域')
    screen = pygame.display.set_mode((850, 400), 0, 32)#窗口大小为(850,400)
    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)
        #傅里叶变换对频域进行操作
        np_data = np.fromstring(data, dtype=np.int16)
        transforamed=np.real(np.fft.fft(np_data))
        screen.fill((0, 0, 0))
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                pygame.quit()
        screen.fill((255, 99, 71))
        points_list=[]
        for n in range(0,transforamed.size,count_freq):
            hight=abs(int(transforamed[n]/10000))
            pygame.draw.rect(screen,(255,0,255), ((20*n/count_freq,400),(20,-hight)))
            if hight>0:
                draw_man(screen,20*n/count_freq,370-hight)
            points_list.append((20*n/count_freq,330-hight))
        pygame.draw.lines(screen,(255,192,203),False,points_list,8)
        pygame.display.update()
    stream.stop_stream()
    stream.close()
    player.terminate()

if __name__ == '__main__':
    wf = wave.open("dave.wav", 'rb')  # 以只读的方式打开".wav"文件
    visual_music(wf,count_freq=50)