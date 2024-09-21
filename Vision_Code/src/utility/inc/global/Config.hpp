#ifndef __INLINE_VARIABLE_HPP__
#define __INLINE_VARIABLE_HPP__

#include <opencv2/opencv.hpp>
#include "json/ReJson.hpp"

inline ReJson J_CAPTURE("../config/json/capture.json");
inline ReJson J_DETECT("../config/json/detect.json");
inline ReJson J_SENSORS("../config/json/sensors.json");
inline ReJson J_TRACK("../config/json/track.json");
inline ReJson J_MOUSE("../config/json/mouse.json");

#endif