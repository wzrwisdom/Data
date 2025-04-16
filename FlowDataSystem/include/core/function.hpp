#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <limits>
#include <iostream>



// 函数声明
std::vector<double> row_move(const std::vector<double>& series, int shift);
double move(const std::vector<double>& series, int shift);
std::vector<double> row_delta(const std::vector<double>& series, int shift = 1);
double delta(const std::vector<double>& series, int shift = 1);
std::vector<double> row_ret(const std::vector<double>& series, int shift = 1);
double ret(const std::vector<double>& series, int shift = 1);
double rank(const std::vector<double>& series, int window);
std::vector<double> ffill(const std::vector<double>& input);
std::vector<double> row_round(const std::vector<double>& series, int digits = 0);
std::vector<double> ts_first(const std::vector<double>& series, int window, int min_periods = -1);
std::vector<double> ts_last(const std::vector<double>& series, int window, int min_periods = -1);
std::vector<double> ts_max(const std::vector<double>& series, int window, int min_periods = -1);
std::vector<double> ts_min(const std::vector<double>& series, int window, int min_periods = -1);
std::vector<double> avg_price(const std::vector<double>& price, const std::vector<double>& volume, int window);
double sum(const std::vector<double>& series, int window);
std::vector<double> ts_sum(const std::vector<double>& series, int window);
double mean(const std::vector<double>& series, int window);
std::vector<double> ts_mean(const std::vector<double>& series, int window);
std::vector<double> row_fillna(const std::vector<double>& series, double default_value);
std::vector<double> operator/(double val, const std::vector<double>& series1);
std::vector<double> operator/(const std::vector<double>& series1, double val);
std::vector<double> operator*(const std::vector<double>& series1, double val);
std::vector<double> operator+(const std::vector<double>& series1, double val);
std::vector<double> operator-(const std::vector<double>& series1, double val);
std::vector<double> operator*(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<double> operator/(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<double> operator+(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<double> operator-(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<bool> operator==(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<bool> operator!=(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<bool> operator<(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<bool> operator>(const std::vector<double>& series1, const std::vector<double>& series2);
std::vector<bool> operator>(const std::vector<double>& series1, double val);
std::vector<bool> operator<(const std::vector<double>& series1, double val);
double stdev(const std::vector<double>& series, int window);
std::vector<double> ts_std(const std::vector<double>& series, int window);
double corr(const std::vector<double>& x, const std::vector<double>& y, int window);
std::vector<double> ts_corr(const std::vector<double>& x, const std::vector<double>& y, int window);
std::vector<double> ts_rank(const std::vector<double>& series, int window, int min_periods = -1);
double skewness(const std::vector<double>& series, int window);
double kurtosis(const std::vector<double>& series, int window);
std::vector<double> ts_detrend(const std::vector<double>& data);

#endif // FUNCTION_HPP