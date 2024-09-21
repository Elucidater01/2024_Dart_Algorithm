#include "filter/OneEuroFilter.hpp"

#include <cmath>
OneEuroFilter::OneEuroFilter(double x0,
                             double d_x0,
                             double min_cut_off,
                             double beta,
                             double d_cut_off) :
        x_pre_(x0),
        d_x_pre_(d_x0),
        min_cut_off_(min_cut_off),
        beta_(beta),
        d_cut_off_(d_cut_off) {}

void OneEuroFilter::setParameter(double x0,
                                 double d_x0,
                                 double min_cut_off,
                                 double beta,
                                 double d_cut_off) {
    this->x_pre_ = x0;
    this->d_x_pre_ = d_x0;
    this->min_cut_off_ = min_cut_off;
    this->beta_ = beta;
    this->d_cut_off_ = d_cut_off;
}

double OneEuroFilter::smoothingFactor(double t_e, double cut_off) {
    double r = 2 * M_PI * cut_off * t_e;
    return r / (r + 1.);
}

double OneEuroFilter::exponentialSmoothing(double x, double prev_x, double alpha) {
    return alpha * x + (1. - alpha) * prev_x;
}

double OneEuroFilter::filter(double x, double dt) {
    double a_d = OneEuroFilter::smoothingFactor(dt, this->d_cut_off_);
    double dx = (x - this->x_pre_) / dt;
    double dx_hat = OneEuroFilter::exponentialSmoothing(dx, this->d_x_pre_, a_d);

    double cut_off = this->min_cut_off_ + this->beta_ * std::fabs(dx_hat);
    double alpha = OneEuroFilter::smoothingFactor(dt, cut_off);
    double x_hat = OneEuroFilter::exponentialSmoothing(x, this->x_pre_, alpha);

    this->x_pre_ = x_hat;
    this->d_x_pre_ = dx_hat;

    return x_hat;
}