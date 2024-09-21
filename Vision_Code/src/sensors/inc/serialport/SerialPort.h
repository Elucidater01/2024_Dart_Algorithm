#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#include "global/Struct.hpp"

class SerialPort {
public:
    explicit SerialPort();
    ~SerialPort();

    bool init();
    bool receive(CarData &cd);
    void send(VisionData &vd);
    void closePort();

private:
    void setBrate();
    int setBit();
    int fd_;
    int baudrate_;
    int databits_;
    int stopbits_;
    int parity_;
    unsigned char r_data_[4096];
    unsigned char t_data_[30];
};
#endif