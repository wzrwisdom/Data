
#include "core/function.hpp"


// 简单移动
inline std::vector<double> row_move(const std::vector<double>& series, int shift) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    for (size_t i = shift; i < series.size(); ++i) {
        result[i] = series[i-shift];
    }
    return result;
}

inline double move(const std::vector<double>& series, int shift) {
    if (series.size() < shift + 1) {
        return std::nan("");
    }
    return series.back() - series[series.size() - shift - 1];
}

// 简单差分
inline std::vector<double> row_delta(const std::vector<double>& series, int shift) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    for (size_t i = shift; i < series.size(); ++i) {
        result[i] = series[i] - series[i-shift];
    }
    return result;
}

inline double delta(const std::vector<double>& series, int shift) {
    if (series.size() < shift + 1) {
        return std::nan("");
    }
    return series.back() - series[series.size() - shift - 1];
}


// 收益率
inline std::vector<double> row_ret(const std::vector<double>& series, int shift) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    for (size_t i = shift; i < series.size(); ++i) {
        double prev = series[i - shift];
        result[i] = prev == 0 ? 0.0 : (series[i] - prev) / prev;
    }
    return result;
}

inline double ret(const std::vector<double>& series, int shift) {
    if (series.size() < shift + 1) {
        return std::nan("");
    }
    double prev = series[series.size() - shift - 1];
    return prev == 0 ? 0.0 : (series.back() - prev) / prev;
}

// 排名
inline double rank(const std::vector<double>& series, int window) {
    if (series.size() < std::floor(window/3)) {
        return std::nan("");
    }
    
    double value = series.back();
    std::vector<double> sorted = series;
    std::sort(sorted.begin(), sorted.end());
    auto lowerCount = std::count_if(sorted.begin(), sorted.end(), [value](double v) { return v < value; });
    auto equalCount = std::count_if(sorted.begin(), sorted.end(), [value](double v) { return v == value; });
    double percentileRank = (lowerCount + 0.5 * equalCount) / sorted.size();
    return percentileRank;
}

inline std::vector<double> ffill(const std::vector<double>& input) {
    std::vector<double> result(input.size(), std::numeric_limits<double>::quiet_NaN());
    double last_valid = std::numeric_limits<double>::quiet_NaN();

    for (size_t i = 0; i < input.size(); ++i) {
        if (!std::isnan(input[i])) {
            last_valid = input[i];
        }
        result[i] = last_valid;
    }

    return result;
}

inline std::vector<double> row_round(const std::vector<double>& series, int digits) {

    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    std::transform(series.begin(), series.end(), result.begin(), [digits](double value) {
        return std::round(value * std::pow(10, digits)) / std::pow(10, digits);
    });
    return result;
}

inline std::vector<double> ts_first(const std::vector<double>& series, int window, int min_periods) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    min_periods = min_periods == -1 ? std::floor(window/3) : min_periods;
    for (size_t i = 0; i < series.size(); ++i) {
        if (i >= min_periods-1 && i < window) {
            result[i] = series[0];
        } else if (i >= window) {
            result[i] = series[i-window+1];
        }
    }
    return result;
}

inline std::vector<double> ts_last(const std::vector<double>& series, int window, int min_periods) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    min_periods = min_periods == -1 ? std::floor(window/3) : min_periods;
    for (size_t i = 0; i < series.size(); ++i) {
        if (i >= min_periods-1) {
            result[i] = series[i];
        }
    }
    return result;
}

inline std::vector<double> ts_max(const std::vector<double>& series, int window, int min_periods) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    min_periods = min_periods == -1 ? std::floor(window/3) : min_periods;
    for (size_t i = 0; i < series.size(); ++i) {
        if (i < window && i >= min_periods-1) {
            result[i] = *std::max_element(series.begin(), series.begin() + i + 1);
        } else if (i >= window) {
            result[i] = *std::max_element(series.begin() + i - window + 1, series.begin() + i + 1);
        }
    }
    return result;
}

inline std::vector<double> ts_min(const std::vector<double>& series, int window, int min_periods) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    min_periods = min_periods == -1 ? std::floor(window/3) : min_periods;
    for (size_t i = 0; i < series.size(); ++i) {
        if (i < window && i >= min_periods-1) {
            result[i] = *std::min_element(series.begin(), series.begin() + i + 1);
        } else if (i >= window) {
            result[i] = *std::min_element(series.begin() + i - window + 1, series.begin() + i + 1);
        }
    }
    return result;
}

inline std::vector<double> avg_price(const std::vector<double>& price, const std::vector<double>& volume, int window) {
    std::vector<double> result(price.size(), std::numeric_limits<double>::quiet_NaN());
    result = ts_sum(price*volume, window) / ts_sum(volume, window);
    return result;
}

inline double sum(const std::vector<double>& series, int window) {
    if (series.size() < std::floor(window/3)) {
        return std::nan("");
    }
    return std::accumulate(series.begin(), series.end(), 0.0);
}

inline std::vector<double> ts_sum(const std::vector<double>& series, int window) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    int min_periods = std::floor(window / 3);

    double window_sum = 0.0;
    int valid_count = 0;
    for (size_t i = 0; i < series.size(); i++) {
        ++valid_count;
        // 添加新元素到窗口
        if (!std::isnan(series[i])) {
            window_sum += series[i];
        }

        // 移除窗口外的元素
        if (i >= window) {
            if (!std::isnan(series[i - window])) {
                window_sum -= series[i - window];
                --valid_count;
            }
        }

        // 记录结果
        if (valid_count >= min_periods) {
            result[i] = window_sum;
        }
    }
    return result;
}

// 滑动均值
inline double mean(const std::vector<double>& series, int window) {
    if (series.size() < std::floor(window/3)) {
        return std::nan("");
    }

    double mean = std::accumulate(series.begin(), series.end(), 0.0) / series.size();
    return mean;
}

inline std::vector<double> ts_mean(const std::vector<double>& series, int window) {
    std::vector<double> result = ts_sum(series, window);
    int min_periods = std::floor(window / 3);
    for (int i = 0; i < result.size(); ++i) {
        if ((i+1) >= min_periods && !std::isnan(result[i])) {
            result[i] = result[i] / std::min(i+1, window);
        }
    }
    return result;
}

inline std::vector<double> row_fillna(const std::vector<double>& series, double default_value) {
    std::vector<double> result(series.size(), default_value);
    for (size_t i = 0; i < series.size(); ++i) {
        if (!std::isnan(series[i])) {
            result[i] = series[i];
        }
    }
    return result;
}

inline std::vector<double> operator/(double val, const std::vector<double>& series1) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), result.begin(), [val](double a) {
        return val == 0 ? std::numeric_limits<double>::quiet_NaN() : val/a;
    });
    return result;
}

inline std::vector<double> operator/(const std::vector<double>& series1, double val) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), result.begin(), [val](double a) {
        return val == 0 ? std::numeric_limits<double>::quiet_NaN() : a / val;
    });
    return result;
}

inline std::vector<double> operator*(const std::vector<double>& series1, double val) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), result.begin(), [val](double a) {
        return a*val;
    });
    return result;
}

inline std::vector<double> operator+(const std::vector<double>& series1, double val) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), result.begin(), [val](double a) {
        return a + val;
    });
    return result;
}

inline std::vector<double> operator-(const std::vector<double>& series1, double val) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), result.begin(), [val](double a) {
        return a - val;
    });
    return result;
}

inline std::vector<double> operator*(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), series2.begin(), result.begin(), std::multiplies<double>());
    return result;
}

inline std::vector<double> operator/(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), series2.begin(), result.begin(), [](double a, double b) {
        return b == 0 ? std::numeric_limits<double>::quiet_NaN() : a / b;
    });
    return result;
}
inline std::vector<double> operator+(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), series2.begin(), result.begin(), [](double a, double b) {
        return a+b;
    });
    return result;
}
inline std::vector<double> operator-(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<double> result(series1.size());
    std::transform(series1.begin(), series1.end(), series2.begin(), result.begin(), [](double a, double b) {
        return a - b;
    });
    return result;
}

inline std::vector<bool> operator==(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] == series2[i];
    }
    return result;
}

inline std::vector<bool> operator!=(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] != series2[i];
    }
    return result;
}

inline std::vector<bool> operator<(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] < series2[i];
    }
    return result;
}

inline std::vector<bool> operator>(const std::vector<double>& series1, const std::vector<double>& series2) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] > series2[i];
    }
    return result;
}

inline std::vector<bool> operator>(const std::vector<double>& series1, double val) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] > val;
    }
    return result;
}

inline std::vector<bool> operator<(const std::vector<double>& series1, double val) {
    std::vector<bool> result(series1.size());
    for (size_t i = 0; i < series1.size(); ++i) {
        result[i] = series1[i] < val;
    }
    return result;
}

// 滑动标准差
inline double stdev(const std::vector<double>& series, int window) {
    if (series.size() < std::floor(window/3)) {
        return std::nan("");
    }

    double mean = std::accumulate(series.begin(), series.end(), 0.0) / series.size();
    double sq_sum = std::inner_product(series.begin(), series.end(), series.begin(), 0.0,
                                        std::plus<double>(),
                                        [mean](double a, double b) { return (a - mean) * (b - mean); });
    double stdev = std::sqrt(sq_sum / (series.size() - 1));
    return stdev;
}

inline std::vector<double> ts_std(const std::vector<double>& series, int window) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    int min_periods = std::floor(window / 3.0);

    for (size_t i = min_periods-1; i < series.size(); ++i) {
        int start_i = 0;
        int end_i = 0;
        if ((i+1) <= window) {
            end_i = i+1;
        } else {
            start_i = i - window + 1;
            end_i = i+1;
        }
        std::vector<double> window_data(series.begin() + start_i, series.begin() + end_i);

        // 移除 NaN 值
        window_data.erase(std::remove_if(window_data.begin(), window_data.end(),
                                         [](double v) { return std::isnan(v); }),
                          window_data.end());

        double mean = std::accumulate(window_data.begin(), window_data.end(), 0.0) / window_data.size();
        double sq_sum = std::inner_product(window_data.begin(), window_data.end(), window_data.begin(), 0.0,
                                            std::plus<double>(),
                                            [mean](double a, double b) { return (a - mean) * (b - mean); });
        double stdev = std::sqrt(sq_sum / (window_data.size() - 1));
        result[i] = stdev; // 将结果放在窗口的最后一个位置
    }

    return result;
}



// 滑动相关系数
inline double corr(const std::vector<double>& x, const std::vector<double>& y, int window) {
    if (x.size() < std::floor(window/3)) {
        return std::nan("");
    }
    
    double mean_x = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
    double mean_y = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
    double num = std::inner_product(x.begin(), x.end(), y.begin(), 0.0,
                                    std::plus<double>(),
                                    [mean_x, mean_y](double a, double b) { return (a - mean_x) * (b - mean_y); });
    double den = std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), 0.0,
                                                std::plus<double>(),
                                                [mean_x](double a, double b) { return (a - mean_x) * (b - mean_x); }) *
                            std::inner_product(y.begin(), y.end(), y.begin(), 0.0,
                                                std::plus<double>(),
                                                [mean_y](double a, double b) { return (a - mean_y) * (b - mean_y); }));
    double result = den != 0 ? num / den : 0.0;
    return result;
}

inline std::vector<double> ts_corr(const std::vector<double>& x, const std::vector<double>& y, int window) {
    std::vector<double> result(x.size(), std::numeric_limits<double>::quiet_NaN());
    int min_periods = std::floor(window / 3.0);
    for (size_t i = min_periods-1; i < x.size(); ++i) {
        int start_i = 0; 
        int end_i = 0;
        if ((i+1) <= window) {
            end_i = i+1;
        } else {
            start_i = i - window + 1;
            end_i = i+1;
        }
        std::vector<double> x_window_data(x.begin() + start_i, x.begin() + end_i);
        std::vector<double> y_window_data(y.begin() + start_i, y.begin() + end_i);

        // // 移除 NaN 值 感觉可以去掉 因为一遍会进行向后填充操作
        // x_window_data.erase(std::remove_if(x_window_data.begin(), x_window_data.end(),
        //                                  [](double v) { return std::isnan(v); }),
        //                   x_window_data.end());
        // y_window_data.erase(std::remove_if(y_window_data.begin(), y_window_data.end(),
        //                                  [](double v) { return std::isnan(v); }),
        //                   y_window_data.end());

        double mean_x = std::accumulate(x_window_data.begin(), x_window_data.end(), 0.0) / x_window_data.size();
        double mean_y = std::accumulate(y_window_data.begin(), y_window_data.end(), 0.0) / y_window_data.size();
        double num = std::inner_product(x_window_data.begin(), x_window_data.end(), y_window_data.begin(), 0.0,
                                        std::plus<double>(),
                                        [mean_x, mean_y](double a, double b) { return (a - mean_x) * (b - mean_y); });
        double den = std::sqrt(std::inner_product(x_window_data.begin(), x_window_data.end(), x_window_data.begin(), 0.0,
                                                    std::plus<double>(),
                                                    [mean_x](double a, double b) { return (a - mean_x) * (b - mean_x); }) *
                                std::inner_product(y_window_data.begin(), y_window_data.end(), y_window_data.begin(), 0.0,
                                                    std::plus<double>(),
                                                    [mean_y](double a, double b) { return (a - mean_y) * (b - mean_y); }));
        result[i ] = den != 0 ? num / den : 0.0; // 将结果放在窗口的最后一个位置
    }

    return result;
}


// 排名（归一化）
inline std::vector<double> ts_rank(const std::vector<double>& series, int window, int min_periods) {
    std::vector<double> result(series.size(), std::numeric_limits<double>::quiet_NaN());
    min_periods = min_periods == -1 ? std::floor(window / 3.0) : min_periods;

    for (size_t i = min_periods-1; i < series.size(); ++i) {
        int start_i = 0;
        int end_i = 0;
        if ((i+1) <= window) {
            end_i = i+1;
        } else {
            start_i = i - window + 1;
            end_i = i+1;
        }
        std::vector<double> window_data(series.begin() + start_i, series.begin() + end_i);

        // // 移除 NaN 值
        // window_data.erase(std::remove_if(window_data.begin(), window_data.end(),
        //                                  [](double v) { return std::isnan(v); }),
        //                   window_data.end());

        double value = window_data.back();
        std::vector<double> orderedData = window_data;
        std::sort(orderedData.begin(), orderedData.end());
        auto lowerCount = std::count_if(orderedData.begin(), orderedData.end(), [value](double v) { return v < value; });
        auto equalCount = std::count_if(orderedData.begin(), orderedData.end(), [value](double v) { return v == value; });
        double percentileRank = (lowerCount + 0.5 * equalCount) / orderedData.size();
        result[i] = percentileRank;
    }

    return result;
}

// 计算偏度 (Skewness)
inline double skewness(const std::vector<double>& series, int window) {
    if (series.size() < std::min(3, static_cast<int>(std::floor(window/3)))) {
        return std::numeric_limits<double>::quiet_NaN(); // 偏度需要至少 3 个数据点
    }

    double mean_value = mean(series, window);
    double std_dev = stdev(series, window);

    if (std::isnan(std_dev) || std_dev == 0) {
        return std::numeric_limits<double>::quiet_NaN(); // 标准差为 0 时，偏度未定义
    }

    double skew_sum = std::accumulate(series.begin(), series.end(), 0.0, [mean_value, std_dev](double acc, double x) {
        return acc + std::pow((x - mean_value) / std_dev, 3);
    });

    size_t n = series.size();
    return (n * skew_sum) / ((n - 1) * (n - 2));
}

 // 计算峰度 (Kurtosis)
 inline double kurtosis(const std::vector<double>& series, int window) {
    if (series.size() < std::min(4, static_cast<int>(std::floor(window/3)))) {
        return std::numeric_limits<double>::quiet_NaN(); // 峰度需要至少 4 个数据点
    }

    double mean_value = mean(series, window);
    double std_dev = stdev(series, window);

    if (std::isnan(std_dev) || std_dev == 0) {
        return std::numeric_limits<double>::quiet_NaN(); // 标准差为 0 时，峰度未定义
    }

    double kurt_sum = std::accumulate(series.begin(), series.end(), 0.0, [mean_value, std_dev](double acc, double x) {
        return acc + std::pow((x - mean_value) / std_dev, 4);
    });

    size_t n = series.size();
    double kurt = (n * (n + 1) * kurt_sum) / ((n - 1) * (n - 2) * (n - 3)) -
                  (3.0 * (n - 1) * (n - 1)) / ((n - 2) * (n - 3));
    return kurt;
}


inline std::vector<double> ts_detrend(const std::vector<double>& data) {
    size_t n = data.size();
    if (n < 2) {
        std::cerr << "Error: Data size must be at least 2 for detrending." << std::endl;
        return data;
    }

    // 计算 x 坐标（索引）
    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 0); // x = {0, 1, 2, ..., n-1}

    // 计算均值
    double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / n;
    double y_mean = std::accumulate(data.begin(), data.end(), 0.0) / n;

    // 计算斜率 (slope)
    double numerator = 0.0, denominator = 0.0;
    for (size_t i = 0; i < n; ++i) {
        numerator += (x[i] - x_mean) * (data[i] - y_mean);
        denominator += (x[i] - x_mean) * (x[i] - x_mean);
    }
    double slope = numerator / denominator;

    // 计算截距 (intercept)
    double intercept = y_mean - slope * x_mean;

    // 移除线性趋势
    std::vector<double> detrended_data(n);
    for (size_t i = 0; i < n; ++i) {
        detrended_data[i] = data[i] - (slope * x[i] + intercept);
    }

    return detrended_data;
}
