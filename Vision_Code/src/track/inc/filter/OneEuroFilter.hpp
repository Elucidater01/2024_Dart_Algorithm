#ifndef __ONE_EURO_FILTER_HPP__
#define __ONE_EURO_FILTER_HPP__
class OneEuroFilter {
public:
    OneEuroFilter(double x0,
                  double d_x0,
                  double min_cut_off = 0.0,
                  double beta  = 0.0,
                  double d_cut_off = 0.0);
    OneEuroFilter() = default;
    ~OneEuroFilter() = default;

    void setParameter(double x0,
                      double d_x0,
                      double min_cut_off = 0.0,
                      double beta  = 0.0,
                      double d_cut_off = 0.0);
    double filter(double x, double dt);
private:
    static double smoothingFactor(double t_e, double cut_off);
    static double exponentialSmoothing(double x, double prev_x, double alpha);
    double x_pre_;
    double d_x_pre_;
    double min_cut_off_;
    double beta_;
    double d_cut_off_;
//    double x_prev_;
//    double d_x_prev;
};
#endif