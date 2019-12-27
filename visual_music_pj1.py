# coding=gbk
import warnings
warnings.simplefilter("ignore", DeprecationWarning)#��ֹ������
import numpy as np
import pygame
import pyaudio
import wave

#������������
CHUNK = 1024
#������ɫ
black = (0,0,0)
white = (255,255,255)
red = (255,0,0)
green = (0,255,0)

#������С��
def draw_man(screen,x,y):
    pygame.draw.ellipse(screen,green,[x,y-5,10,10])
    pygame.draw.line(screen,red,[5+x,5+y],[5+x,15+y],2)
    pygame.draw.line(screen,red,[5+x,5+y],[x-5,15+y],2)
    pygame.draw.line(screen,red,[5+x,5+y],[15+x,15+y],2)
    pygame.draw.line(screen,green,[5+x,15+y],[x-5,25+y],2)
    pygame.draw.line(screen,green,[5+x,15+y],[15+x,25+y],2)

'''
Visual����˵��
1.���� wav�����ļ�
2.����pyaudio������
3.��������
4.����ȡ��������

'''
def visual_music(wave_file,count_freq):
    wf=wave_file
    player = pyaudio.PyAudio()
    stream = player.open(format=player.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),#����������
                    rate=wf.getframerate(),#��������Ƶ��
                    output=True)

    data = wf.readframes(CHUNK)
    pygame.init()#pygame��ʼ��
    pygame.display.set_caption('���ӻ�Ƶ��')
    screen = pygame.display.set_mode((850, 400), 0, 32)#���ڴ�СΪ(850,400)
    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)#����data
        #����Ҷ�任��Ƶ����в���
        numpydata = np.fromstring(data, dtype=np.int16)
        transforamed=np.real(np.fft.fft(numpydata))
        screen.fill((0, 0, 0))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
        screen.fill((255, 255, 255))
        #�������ݵ� ���ƾ���
        for n in range(0,transforamed.size,count_freq):
            hight=abs(int(transforamed[n]/10000))
            pygame.draw.rect(screen,(0,0,0), ((20*n/count_freq,400),(20,-hight)))
            if hight>0:
                draw_man(screen,20*n/count_freq,370-hight)
        pygame.display.update()
    stream.stop_stream()
    stream.close()
    player.terminate()

if __name__ == '__main__':
    wf = wave.open("dave.wav", 'rb')  # ��ֻ���ķ�ʽ��"1qom8-vi8uq.wav"�ļ�
    visual_music(wf,count_freq=50)