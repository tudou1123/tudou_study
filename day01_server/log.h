#ifndef DAY01_SERVER_LOG_H
#define DAY01_SERVER_LOG_H

#include <string>
#include <filesystem>  // 头文件
#include <fstream>

class Logger {
public:
    static Logger& instance();
    bool init(const std::string& filename = "app.log");
    void log(const std::string& level, const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
    void debug(const std::string& msg);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    struct Impl;
    Impl* pimpl;
};

#endif // DAY01_SERVER_LOG_H
