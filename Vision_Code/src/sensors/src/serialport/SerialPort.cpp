
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>

#include "serialport/CRC_Check.h"
#include "serialport/SerialPort.h"
#include "global/Config.hpp"
SerialPort::SerialPort() {
    this->init();
}

SerialPort::~SerialPort() {
    this->closePort();
}

bool SerialPort::init() {
    this->fd_ = open(((std::string)J_SENSORS.config_["serialport"]["port"]).c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    J_SENSORS.config_["serialport"]["baudrate"] >> this->baudrate_;
    J_SENSORS.config_["serialport"]["databits"] >> this->databits_;
    J_SENSORS.config_["serialport"]["stopbits"] >> this->stopbits_;
    this->parity_ =  'N';

    if (this->fd_ == -1) {
        return false;
    }

    this->setBrate();

    if (this->setBit() == false) {
        exit(0);
    }

    printf("SerialPort inits successed.\n");

    return true;
}

bool SerialPort::receive(CarData &cd) {
    int bytes;
    int result = ioctl(this->fd_, FIONREAD, &bytes);

    if (result == -1) {
        printf("ioctl: %s\n", strerror(errno));
        return false;
    }

    if (bytes == 0) {
        return false;
    }

    bytes = read(this->fd_, this->r_data_, bytes);
//    std::cout << "Receive Bytes: " << bytes << std::endl;
    int first_index = -1;
    for (int i = 0 ; i < bytes ; i++) {
        if (this->r_data_[i] == 0xA5 &&
            first_index == -1 &&
            Verify_CRC8_Check_Sum(&this->r_data_[i], 3) &&
            Verify_CRC16_Check_Sum(&this->r_data_[i], sizeof(CarData))) {
            first_index = i;
            break;
        }
    }

    int max_mul = (bytes - first_index) / sizeof(CarData);

    if (first_index != -1) {
        int index = first_index + (max_mul - 1) *sizeof(CarData);
        if (this->r_data_[index] == 0xA5 &&
            Verify_CRC8_Check_Sum(&this->r_data_[index], 3)) {
            memcpy(&cd, &this->r_data_[index], sizeof(CarData));
            return true;
        } else {
            std::cout << "crc check failed." << std::endl;
            return false;
        }
    } else {
        std::cout << "cannot find header," << std::endl;
        return false;
    }
}

void SerialPort::send(VisionData &vd) {
    memcpy(this->t_data_, &vd, sizeof(VisionData));
    Append_CRC8_Check_Sum(this->t_data_, 3);
    Append_CRC16_Check_Sum(this->t_data_, sizeof(VisionData));
    int result = write(this->fd_, this->t_data_, sizeof(VisionData));
    if (result == -1) {
//        std::cout << "send failed." << std::endl;
        this->closePort();
        this->init();
    }

}

void SerialPort::closePort() {
    close(this->fd_);
}

void SerialPort::setBrate() {
    int speed_arr[] = {B921600, B460800, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
                       B921600, B460800, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
    };
    int name_arr[] = {921600, 460800, 115200, 38400, 19200, 9600, 4800, 2400, 1200,  300,
                      921600, 460800, 115200, 38400, 19200, 9600, 4800, 2400, 1200,  300,
    };

    unsigned int i;
    int status;
    struct termios Opt;     //用于存储获得的终端参数信息
    tcgetattr(this->fd_, &Opt);
    for (i = 0;  i < sizeof(speed_arr) / sizeof(int);  i++) {
        if (this->baudrate_ == name_arr[i]) {
            tcflush(this->fd_, TCIOFLUSH);                 //清空缓冲区的内容
            cfsetispeed(&Opt, speed_arr[i]);        //设置接受和发送的波特率
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(this->fd_, TCSANOW, &Opt);  //使设置立即生效

            if (status != 0) {
                return;
            }

            tcflush(this->fd_, TCIOFLUSH);

        }
    }
}

int SerialPort::setBit() {
    // 获取串口相关参数
    struct termios termios_p;
    tcgetattr(this->fd_, &termios_p);
    // CLOACL：保证程序不会占用串口
    // CREAD：使得能够从串口中读取输入数据
    termios_p.c_cflag |= (CLOCAL | CREAD);

    // 设置数据位
    termios_p.c_cflag &= ~CSIZE;
    switch (this->databits_) {
        case 7:
            termios_p.c_cflag |= CS7;
            break;

        case 8:
            termios_p.c_cflag |= CS8;
            break;

        default:
            fprintf(stderr, "Unsupported data size\n");
            return false;
    }

    // 设置奇偶校验位
    switch (this->parity_) {
        case 'n':
        case 'N':
            termios_p.c_cflag &= ~PARENB;       //PARENB:启用奇偶校验码的生成和检测功能。
            termios_p.c_iflag &= ~INPCK;        //INPCK：对接收到的字符执行奇偶校检。
            break;

        case 'o':
        case 'O':
            termios_p.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            termios_p.c_iflag |= INPCK;             /* Disnable parity checking */
            break;

        case 'e':
        case 'E':
            termios_p.c_cflag |= PARENB;        /* Enable parity */
            termios_p.c_cflag &= ~PARODD;       /* 转换为偶效验*/
            termios_p.c_iflag |= INPCK;         /* Disnable parity checking */
            break;

        case 'S':
        case 's':  /*as no parity*/
            termios_p.c_cflag &= ~PARENB;
            termios_p.c_cflag &= ~CSTOPB;
            break;

        default:
            fprintf(stderr, "Unsupported parity\n");
            return false;

    }

    // 设置停止位
    switch (this->stopbits_) {
        case 1:
            termios_p.c_cflag &= ~CSTOPB;
            break;

        case 2:
            termios_p.c_cflag |= CSTOPB;
            break;

        default:
            fprintf(stderr, "Unsupported stop bits\n");
            return false;

    }

    if (this->parity_ != 'n') {
        termios_p.c_iflag |= INPCK;
    }

    tcflush(this->fd_, TCIFLUSH);                                  //清除输入缓存区
    termios_p.c_cc[VTIME] = 1;                              //设置超时
    termios_p.c_cc[VMIN] = 0;                 //最小接收字符
    termios_p.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);   //Input原始输入
    // 写死了，不管前面写了什么，都一定是无奇偶校验
    termios_p.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    termios_p.c_iflag &= ~(ICRNL | IGNCR);
    termios_p.c_oflag &= ~OPOST;                            //Output禁用输出处理

    if (tcsetattr(this->fd_, TCSANOW, &termios_p) != 0) {
        return false;
    }

    return true;
}