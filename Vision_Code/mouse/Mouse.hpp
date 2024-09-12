#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "lockfree/LockFree.hpp"
#include "global/Config.hpp"
#include "global/Struct.hpp"

class Mouse {
public:
    explicit Mouse();
    ~Mouse() = default;

    void mouseControl(LfStack<ImageInfo> &mouse_info_stack);
private:
    enum BOARD {
        LEFT = 81,
        UP,
        RIGHT,
        DOWN,
    };
    typedef struct {
        cv::Mat ori;
        cv::Mat roi;
        cv::Mat rect;
        cv::Mat letter_box;
        cv::Mat canvas;
        double x;
        double y;
        double dx;
        double dy;
        double new_w;
        double new_h;
        double mark_x;
        double mark_y;
        double ltx;
        double lty;
        double rbx;
        double rby;
        bool is_dragging_rect;
        bool is_moving_rect;
        bool reset_rect;
    }MouseInfo;

    static auto letterbox2Canvas(MouseInfo* m_i,  std::tuple<double, double> letterbox) -> std::tuple<double, double>;
    static auto canvas2ROI(MouseInfo*, std::tuple<double, double> canvas) -> std::tuple<double, double>;
    void draw();
    void drawLetterBox();
    void drawLetterBoxLine();
    void drawRoi();
    void drawRect();
    void drawMouse();
    void markControl(const int &key);
    void updateJson();
    static void onMouse(int event, int x, int y, int flags, void *param);

    std::shared_ptr<MouseInfo> m_i_;
    cv::FileStorage config_;
    std::string path_;
};
#endif