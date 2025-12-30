#include "log.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <cstdlib>

struct Logger::Impl {
	std::mutex mtx;
	std::ofstream ofs;
	std::string filepath;
	bool initialized = false;

	bool open_file(const std::string& filename) {
		const char* env_dir = std::getenv("LOG_DIR");
		std::filesystem::path dir;
		if (env_dir && env_dir[0] != '\0') {
			dir = std::filesystem::path(env_dir);
		} else {
			const char* home = std::getenv("HOME");
			if (!home) home = std::getenv("USERPROFILE");
			if (!home) home = ".";
			dir = std::filesystem::path(home) / "log";
		}
		std::error_code ec;
		std::filesystem::create_directories(dir, ec);
		std::filesystem::path fp = dir / filename;
		ofs.open(fp, std::ios::app);
		if (!ofs.is_open()) return false;
		filepath = fp.string();
		initialized = true;
		return true;
	}
};

Logger& Logger::instance() {
	static Logger inst;
	return inst;
}

Logger::Logger() : pimpl(new Impl()) {}

Logger::~Logger() {
	if (pimpl) {
		std::lock_guard<std::mutex> lk(pimpl->mtx);
		if (pimpl->ofs.is_open()) pimpl->ofs.close();
		delete pimpl;
		pimpl = nullptr;
	}
}

bool Logger::init(const std::string& filename) {
	std::lock_guard<std::mutex> lk(pimpl->mtx);
	return pimpl->open_file(filename);
}

void Logger::log(const std::string& level, const std::string& msg) {
	if (!pimpl->initialized) {
		// lazy init with default name
		init("app.log");
	}
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
#if defined(_WIN32)
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms.count();

	std::lock_guard<std::mutex> lk(pimpl->mtx);
	if (pimpl->ofs.is_open()) {
		pimpl->ofs << "[" << oss.str() << "] [" << level << "] " << msg << std::endl;
		pimpl->ofs.flush();
	} else {
		// fallback to stderr
		std::cerr << "[" << oss.str() << "] [" << level << "] " << msg << std::endl;
	}
}

void Logger::info(const std::string& msg) { log("INFO", msg); }
void Logger::warn(const std::string& msg) { log("WARN", msg); }
void Logger::error(const std::string& msg) { log("ERROR", msg); }
void Logger::debug(const std::string& msg) { log("DEBUG", msg); }
