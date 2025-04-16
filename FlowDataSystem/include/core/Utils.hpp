#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace Utils {

class Timestamp {
public:
    // 默认构造函数（当前时间）
    Timestamp();
    
    Timestamp(const char* time_str);  
    // 从 std::chrono::system_clock::time_point 构造
    Timestamp(const std::chrono::system_clock::time_point& tp);
    
    // 从字符串构造（如 "2023-01-01" 或 "2023-01-01 12:00:00"）
    Timestamp(const std::string& time_str);
    
    // 从时分秒毫秒构造
    Timestamp(int hour, int min, int sec, int ms = 0);
    
    // 转换为 std::chrono::system_clock::time_point
    operator std::chrono::system_clock::time_point() const;
    
    // 对齐时间
    Timestamp align_time(int interval_ms, const Timestamp& base_time="2023-01-01 09:30:00") const;
    
    // 转换为字符串
    std::string to_string() const;
    
    // 获取内部 time_point
    std::chrono::system_clock::time_point get_time_point() const;

    inline bool operator< (const Timestamp& other) const { return m_time_point < other.m_time_point; }
    inline bool operator> (const Timestamp& other) const { return other < *this; }
    inline bool operator<=(const Timestamp& other) { return !(other < *this);}
    inline bool operator>=(const Timestamp& other) { return !(*this < other);}
    inline bool operator==(const Timestamp& other) const { return m_time_point == other.m_time_point; }
    inline bool operator!=(const Timestamp& other) const { return !(*this == other);}

    std::chrono::duration<double> operator-(const Timestamp& other) const;
private:
    std::chrono::system_clock::time_point m_time_point;
};

} // namespace Utils

#endif // UTILS_HPP