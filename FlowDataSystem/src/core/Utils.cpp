#include "core/Utils.hpp"
#include <stdexcept>

namespace Utils {

Timestamp::Timestamp() 
    : m_time_point(std::chrono::system_clock::now()) {}

Timestamp::Timestamp(const std::chrono::system_clock::time_point& tp) 
    : m_time_point(tp) {}

Timestamp::Timestamp(const char* time_str) 
    : Timestamp(std::string(time_str)) {} 

Timestamp::Timestamp(const std::string& time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    
    // 尝试解析日期时间（支持 "YYYY-MM-DD" 或 "YYYY-MM-DD HH:MM:SS"）
    if (time_str.find(':') == std::string::npos) {
        ss >> std::get_time(&tm, "%Y-%m-%d");
    } else {
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    }
    
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp: " + time_str);
    }
    
    m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

Timestamp::Timestamp(int hour, int min, int sec, int ms) {
    auto now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    tm now_tm = *std::localtime(&now_time);
    
    now_tm.tm_hour = hour;
    now_tm.tm_min = min;
    now_tm.tm_sec = sec;
    
    m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&now_tm)) + 
                  std::chrono::milliseconds(ms);
}

Timestamp::operator std::chrono::system_clock::time_point() const {
    return m_time_point;
}

Timestamp Timestamp::align_time(int interval_ms, const Timestamp& base_time) const {
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_time_point - base_time.m_time_point);
    long long elapsed_ms = elapsed.count();
    long long aligned_ms = ((elapsed_ms / interval_ms) + (elapsed_ms % interval_ms > 0 ? 1 : 0)) * interval_ms;
    return Timestamp(base_time.m_time_point + std::chrono::milliseconds(aligned_ms));
}


std::string Timestamp::to_string() const {
    auto in_time_t = std::chrono::system_clock::to_time_t(m_time_point);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_time_point.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::chrono::system_clock::time_point Timestamp::get_time_point() const {
    return m_time_point;
}

std::chrono::duration<double> Timestamp::operator-(const Timestamp &other) const
{
    return m_time_point - other.m_time_point;
}
} // namespace Utils