#include "Mouse.hpp"
Mouse::Mouse() {
    cv::namedWindow("mouse", cv::WINDOW_NORMAL);
    cv::moveWindow("mouse", 0, 0);
    cv::resizeWindow("mouse", cv::Size(1040, 768));
    J_MOUSE.config_["keep"]["path"] >> this->path_;
    this->config_.open(this->path_, cv::FileStorage::READ);

    this->m_i_ = std::make_shared<MouseInfo>();
    this->m_i_->ltx = this->config_["ltx"];
    this->m_i_->lty = this->config_["lty"];
    this->m_i_->rbx = this->config_["rbx"];
    this->m_i_->rby = this->config_["rby"];
    this->m_i_->mark_x = this->config_["mark_x"];
    this->m_i_->mark_y = this->config_["mark_y"];
    this->m_i_->reset_rect = false;
    cv::setMouseCallback("mouse", onMouse, (void*)this->m_i_.get());
    this->config_.release();
}

void Mouse::mouseControl(LfStack<ImageInfo> &mouse_info_stack) {
    auto opt_mouse_info = mouse_info_stack.pop();
    while (!opt_mouse_info.has_value()) {
        opt_mouse_info = mouse_info_stack.pop();
    }

    this->m_i_->ori = opt_mouse_info.value().ori_mat.clone();
    this->draw();
    cv::imshow("mouse", this->m_i_->canvas);
    this->markControl(cv::waitKey(20));
}

void Mouse::markControl(const int &key) {
    switch (key) {
        case LEFT:
            this->m_i_->mark_x -= 1;
            break;
        case UP:
            this->m_i_->mark_y -= 1;
            break;
        case RIGHT:
            this->m_i_->mark_x += 1;
            break;
        case DOWN:
            this->m_i_->mark_y += 1;
            break;
        default:
            break;
    }
}


void Mouse::onMouse(int event, int x, int y, int flags, void *param) {
    auto *m_i = (MouseInfo *)param;
    double delta_x = x - m_i->x;
    double delta_y = y - m_i->y;
    double width = std::fabs(m_i->rbx - m_i->ltx);
    double height = std::fabs(m_i->rby - m_i->lty);

    double min_w = m_i->ori.cols * (double)J_MOUSE.config_["wheel"]["min"];
    double min_h = m_i->ori.rows * (double)J_MOUSE.config_["wheel"]["min"];

    auto [can_x, can_y] = Mouse::letterbox2Canvas(m_i, std::make_tuple(x, y));

    switch (event) {
        case cv::EVENT_LBUTTONDOWN:
            m_i->mark_x = can_x;
            m_i->mark_y = can_y;
            break;
        case cv::EVENT_RBUTTONDOWN:
            if (!m_i->reset_rect) {
                m_i->is_dragging_rect = true;
                m_i->is_moving_rect = false;

                m_i->ltx = x;
                m_i->lty = y;
                m_i->rbx = x;
                m_i->rby = y;
            } else {
                m_i->ltx = 0;
                m_i->lty = 0;
                m_i->rbx = m_i->ori.cols;
                m_i->rby = m_i->ori.rows;
            }

            break;
        case cv::EVENT_RBUTTONUP:
            if (!m_i->reset_rect) {
                m_i->reset_rect = true;
                m_i->is_dragging_rect = false;
                m_i->rbx = x;
                m_i->rby = y;

                if (m_i->ltx > m_i->rbx) {
                    std::swap(m_i->ltx, m_i->rbx);
                }

                if (m_i->lty > m_i->rby) {
                    std::swap(m_i->lty, m_i->rby);
                }


                if (width < min_w) {
                    m_i->ltx = m_i->ltx - (min_w - width) / 2;
                    m_i->rbx = m_i->rbx + (min_w - width) / 2;

                    if (m_i->ltx < 0) {
                        m_i->ltx = 0;
                        m_i->rbx = min_w;
                    } else if (m_i->rbx > m_i->ori.cols) {
                        m_i->rbx = m_i->ori.cols;
                        m_i->ltx = m_i->ori.cols - min_w;
                    }
                }

                if (height < min_h) {
                    m_i->lty = m_i->lty - (min_h - height) / 2;
                    m_i->rby = m_i->rby + (min_h - height) / 2;

                    if (m_i->lty < 0) {
                        m_i->lty = 0;
                        m_i->rby = min_h;
                    } else if (m_i->rby > m_i->ori.rows) {
                        m_i->rby = m_i->ori.rows;
                        m_i->lty = m_i->ori.rows - min_h;
                    }
                }

//                std::cout << "ltx: " << m_i->ltx << " lty: " << m_i->lty << " rbx: " << m_i->rbx << " rby: " << m_i->rby << std::endl;
            } else {
                m_i->reset_rect = false;
            }
            break;
        case cv::EVENT_MBUTTONDOWN:
            m_i->is_dragging_rect = false;
            m_i->is_moving_rect = true;
            break;
        case cv::EVENT_MBUTTONUP:
            m_i->is_moving_rect = false;
            break;
        case cv::EVENT_MOUSEMOVE:

            m_i->x = x;
            m_i->y = y;
            if (m_i->is_dragging_rect) {
                m_i->rbx = x;
                m_i->rby = y;

//                std::cout << "ltx: " << m_i->ltx << " lty: " << m_i->lty << " rbx: " << m_i->rbx << " rby: " << m_i->rby << std::endl;
            } else if (m_i->is_moving_rect) {
                double originX = m_i->ltx + width / 2 + delta_x;
                double originY = m_i->lty + height / 2 + delta_y;

                auto ltx = originX - width / 2;
                auto lty = originY - height / 2;
                auto rbx = originX + width / 2;
                auto rby = originY + height / 2;

                if (ltx < 0) {
                    ltx = 0;
                    rbx = width;
                } else if (rbx > m_i->ori.cols) {
                    rbx = m_i->ori.cols;
                    ltx = m_i->ori.cols - width;
                }

                if (lty < 0) {
                    lty = 0;
                    rby = height;
                } else if (rby > m_i->ori.rows) {
                    rby = m_i->ori.rows;
                    lty = m_i->ori.rows - height;
                }

                m_i->ltx = ltx;
                m_i->lty = lty;
                m_i->rbx = rbx;
                m_i->rby = rby;
            }

            break;
        case cv::EVENT_MOUSEWHEEL:
            m_i->reset_rect = true;
            double scale = J_MOUSE.config_["wheel"]["scale"];

            if (flags > 0) {
                scale = 1 / scale;
            }


            double originX = m_i->ltx + width / 2;
            double originY = m_i->lty + height / 2;

            auto ltx = (originX - width / 2. * scale) < 0 ? 0 : (originX- width / 2 * scale);
            auto lty = (originY - height / 2. * scale) < 0 ? 0 : (originY - height / 2 * scale);

            auto rbx = (originX + width / 2. * scale) > m_i->ori.cols ? m_i->ori.cols : (originX + width / 2 * scale);
            auto rby = (originY + height / 2. * scale) > m_i->ori.rows ? m_i->ori.rows : (originY + height / 2 * scale);


            if ((rbx - ltx) > min_w) {
                m_i->ltx = ltx;
                m_i->rbx = rbx;
            }

            if ((rby - lty) > min_h) {
                m_i->lty = lty;
                m_i->rby = rby;
            }
            break;
    }
}

auto Mouse::letterbox2Canvas(MouseInfo *m_i, std::tuple<double, double> letterbox) -> std::tuple<double, double> {
    auto [x, y] = letterbox;
    double fix_x = (x - (m_i->canvas.cols - m_i->new_w) / 2) / m_i->new_w * (m_i->rbx - m_i->ltx) + m_i->ltx;
    double fix_y = (y - (m_i->canvas.rows - m_i->new_h) / 2) / m_i->new_h * (m_i->rby - m_i->lty) + m_i->lty;
    return std::make_tuple(fix_x, fix_y);
}
auto Mouse::canvas2ROI(MouseInfo *m_i, std::tuple<double, double> canvas) -> std::tuple<double, double> {
    auto[x, y] = canvas;
    double roi_x = m_i->canvas.cols - m_i->roi.cols + x / m_i->canvas.cols * m_i->roi.cols;
    double roi_y = m_i->canvas.rows - m_i->roi.rows + y / m_i->canvas.rows * m_i->roi.rows;
    return std::make_tuple(roi_x, roi_y);
}

void Mouse::draw() {
    this->m_i_->canvas = cv::Mat::zeros(this->m_i_->ori.size(), this->m_i_->ori.type());
    //draw mark line
    if (this->m_i_->mark_x != 0 || this->m_i_->mark_y != 0) {
        cv::line(this->m_i_->ori, cv::Point(this->m_i_->mark_x, 0), cv::Point(this->m_i_->mark_x, this->m_i_->canvas.rows), cv::Scalar(255, 0, 255), 1);
        cv::line(this->m_i_->ori, cv::Point(0, this->m_i_->mark_y), cv::Point(this->m_i_->canvas.cols, this->m_i_->mark_y), cv::Scalar(255, 0, 255), 1);
    }
    this->drawLetterBox();
    this->drawRoi();
    this->drawRect();
    this->drawMouse();
//    this->updateJson();
}

void Mouse::drawLetterBox() {
    if (this->m_i_->ltx == this->m_i_->rbx ||
        this->m_i_->lty == this->m_i_->rby ||
        this->m_i_->is_dragging_rect) {
        this->m_i_->letter_box.copyTo(this->m_i_->canvas(cv::Rect(this->m_i_->dx, this->m_i_->dy, this->m_i_->new_w, this->m_i_->new_h)));
        this->drawLetterBoxLine();
        return;
    }

    int src_width = this->m_i_->rbx - this->m_i_->ltx;
    int src_height = this->m_i_->rby - this->m_i_->lty;

    this->m_i_->rect = this->m_i_->ori(cv::Rect(this->m_i_->ltx, this->m_i_->lty, src_width, src_height));

    int dst_width = this->m_i_->canvas.cols;
    int dst_height = this->m_i_->canvas.rows;

    double r_w = (double)dst_width / src_width;
    double r_h = (double)dst_height / src_height;

    double r = std::min(r_w, r_h);

    this->m_i_->new_w = (int)(src_width * r);
    this->m_i_->new_h = (int)(src_height * r);

    cv::resize(this->m_i_->rect, this->m_i_->letter_box, cv::Size(this->m_i_->new_w, this->m_i_->new_h), cv::INTER_CUBIC);

    this->m_i_->dx = (int)((dst_width - this->m_i_->new_w) / 2.);
    this->m_i_->dy = (int)((dst_height - this->m_i_->new_h) / 2.);

    this->m_i_->letter_box.copyTo(this->m_i_->canvas(cv::Rect(this->m_i_->dx, this->m_i_->dy, this->m_i_->new_w, this->m_i_->new_h)));

    this->drawLetterBoxLine();
}

void Mouse::drawLetterBoxLine() {
    cv::line(this->m_i_->canvas, cv::Point(0, this->m_i_->y), cv::Point(this->m_i_->canvas.cols, this->m_i_->y), cv::Scalar(255, 255, 255), 1);
    cv::line(this->m_i_->canvas, cv::Point(this->m_i_->x, 0), cv::Point(this->m_i_->x, this->m_i_->canvas.rows), cv::Scalar(255, 255, 255), 1);


}

void Mouse::drawRoi() {


    cv::resize(
            this->m_i_->ori,
            this->m_i_->roi,
            cv::Size(this->m_i_->canvas.cols * (double)J_MOUSE.config_["roi"]["scale"],
                     this->m_i_->canvas.rows * (double)J_MOUSE.config_["roi"]["scale"]));
    //draw roi at the right bottom corner of canvas
    cv::Rect canvas_rect(
            this->m_i_->canvas.cols - this->m_i_->roi.cols,
            this->m_i_->canvas.rows - this->m_i_->roi.rows,
            this->m_i_->roi.cols,
            this->m_i_->roi.rows);
    this->m_i_->roi.copyTo(this->m_i_->canvas(canvas_rect));

    auto[roi_x, roi_y] = this->canvas2ROI(this->m_i_.get(), this->letterbox2Canvas(this->m_i_.get(), std::make_tuple(this->m_i_->x, this->m_i_->y)));
    cv::line(this->m_i_->canvas, cv::Point(this->m_i_->canvas.cols - this->m_i_->roi.cols, roi_y), cv::Point(this->m_i_->canvas.cols, roi_y), cv::Scalar(128, 128, 128), 1);
    cv::line(this->m_i_->canvas, cv::Point(roi_x, this->m_i_->canvas.rows - this->m_i_->roi.rows), cv::Point(roi_x, this->m_i_->canvas.rows), cv::Scalar(128, 128, 128), 1);

}


void Mouse::drawRect() {
    if (this->m_i_->is_dragging_rect) {
        cv::rectangle(this->m_i_->canvas, cv::Point(this->m_i_->ltx, this->m_i_->lty), cv::Point(this->m_i_->rbx, this->m_i_->rby), cv::Scalar(255, 255, 255), 2);
    }
    auto [roi_ltx, roi_lty] = this->canvas2ROI(this->m_i_.get(), std::make_tuple(this->m_i_->ltx, this->m_i_->lty));
    auto [roi_rbx, roi_rby] = this->canvas2ROI(this->m_i_.get(), std::make_tuple(this->m_i_->rbx, this->m_i_->rby));

    cv::rectangle(this->m_i_->canvas, cv::Point(roi_ltx, roi_lty), cv::Point(roi_rbx, roi_rby), cv::Scalar(255, 255, 255), 1);
}

void Mouse::drawMouse() {
    cv::circle(this->m_i_->canvas, cv::Point(this->m_i_->x, this->m_i_->y), 5, cv::Scalar(255, 255, 255), 1);

    auto[roi_x, roi_y] = this->canvas2ROI(this->m_i_.get(), this->letterbox2Canvas(this->m_i_.get(), std::make_tuple(this->m_i_->x, this->m_i_->y)));

    cv::circle(this->m_i_->canvas, cv::Point(roi_x, roi_y), 3, cv::Scalar(255, 255, 255), -1);
}

void Mouse::updateJson() {
    if (this->m_i_->ltx != (double)this->config_["ltx"] ||
        this->m_i_->lty != (double)this->config_["lty"] ||
        this->m_i_->rbx != (double)this->config_["rbx"] ||
        this->m_i_->rby != (double)this->config_["rby"] ||
        this->m_i_->mark_x != (double)this->config_["mark_x"] ||
        this->m_i_->mark_y != (double)this->config_["mark_y"]) {
        this->config_.release();
        this->config_.open(this->path_, cv::FileStorage::WRITE);

        this->config_ << "ltx" << this->m_i_->ltx;
        this->config_ << "lty" << this->m_i_->lty;
        this->config_ << "rbx" << this->m_i_->rbx;
        this->config_ << "rby" << this->m_i_->rby;
        this->config_ << "mark_x" << this->m_i_->mark_x;
        this->config_ << "mark_y" << this->m_i_->mark_y;

        this->config_.release();
    }
}

