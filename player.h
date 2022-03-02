#ifndef __PLAYER_H__
#define __PLAYER_H__

#define CMD_CONFIRM 0x0B //选曲确认
#define CMD_VOLUME 0x0C  //设置音量
#define CMD_MODE 0x0E //设置播放模式
class Player
{
public:
    Player(uint8_t data, uint8_t busy)
    {
        _dataPin = data;
        _busyPin = busy;
    }

    void begin()
    {
        pinMode(_dataPin, OUTPUT);

        volume(30); // 设置音量等级
        delay(200);
    }

    // 播放第n首歌曲，n: 1 - 99
    void play(int n)
    {
        select(n, CMD_CONFIRM);
    }

    void volume(int n)
    {
        select(n, CMD_VOLUME);
    }

    // Mode 播放模式，上电默认02-单曲停止；模式有：
    // 00-全盘循环：顺序播放全盘曲目，播放完后循环播放；
    // 01-单曲循环：循环播放当前曲目；
    // 02-单曲停止：播放完当前曲目一次，停止播放；
    // 03-全盘随机：随机播放盘符内曲目；
    // 04-目录循环：顺序播放当前目录文件夹内曲目，播放完后循环播放，目录不包含子目录；
    // 05-目录随机：在当前目录文件夹内随机播放，目录不包含子目录；
    // 06-目录顺序：顺序播放当前目录文件夹内曲目，播放完后停止播放，目录不包含子目录；
    // 07-全盘顺序：顺序播放全盘曲目，播放完后停止。

    void mode(int n)
    {
        select(n, CMD_MODE);
    }

private:
    void select(int n, int c)
    {
        if (n < 1 || n >= 100)
        {
            n = 1;
        }
        if (n < 10)
        {
            cmd(2, n, c);
        }

        if (n >= 10 && n < 100)
        {
            cmd(3, int(n / 10), int(n % 10), c);
        }
    }
    void sendData(int data)
    {
        digitalWrite(_dataPin, HIGH);
        delay(1); // >1ms
        digitalWrite(_dataPin, LOW);
        delay(3); // >3ms
        for (int i = 0; i < 8; i++)
        {
            digitalWrite(_dataPin, HIGH);
            if (data & 1)
            {
                delayMicroseconds(1200); // >1200us
                digitalWrite(_dataPin, LOW);
                delayMicroseconds(400); // >400us
            }
            else
            {
                delayMicroseconds(400); // >400us
                digitalWrite(_dataPin, LOW);
                delayMicroseconds(1200); // >1200us
            }
            data >>= 1;
        }
        digitalWrite(_dataPin, HIGH);
    }

    /*
       多参数功能函数
        参数：n - 参数数量
             args - 参数值
    */
    void cmd(uint8_t n, ...)
    {
        va_list ap;
        unsigned int args;
        va_start(ap, n);
        while (n > 0)
        {
            args = va_arg(ap, unsigned int);
            sendData(args);
            n--;
        }
        va_end(ap);
    }

    uint8_t _dataPin;
    uint8_t _busyPin;
};
#endif
