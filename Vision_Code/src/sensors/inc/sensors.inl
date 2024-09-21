#include "sensors.hpp"

template<class ...Args>
void sensors::BaseCamera::refreshImage(Args &&...args) {
    this->getImage();
    this->unpackArgs(args...);
}

template<typename T, class ...Args>
void sensors::BaseCamera::unpackArgs(T &&t, Args &&...args) {
    if constexpr (std::is_same_v<T, LfStack<FromData>&>) {
        t.pop();
        t.push(this->message_);
    } else {
        std::cout << "error type in sensors" << std::endl;
    }
    this->unpackArgs(args...);
}