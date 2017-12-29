#include "logger.h"
#include <unordered_map>
#include <iostream>

void cout_print(const std::string& msg) {
	std::cout << msg << '\n'; 
}

void stub_print(const std::string& msg) {

}

class logger::logger_impl {
	public:
		logger_impl() {
			set_level(_log_level);
		};

		void set_level(log_level_e log_level) {
			_log_level = log_level;
			switch(log_level) {
				case log_level_e::LOG_LEVEL_USER:
				case log_level_e::LOG_LEVEL_ERROR:
					_log_map[log_level_e::LOG_LEVEL_USER] = [this] (const std::string& msg) {
						cout_print(msg);
					};

					_log_map[log_level_e::LOG_LEVEL_ERROR] = [this] (const std::string& msg) {
						cout_print(msg);
					};

					_log_map[log_level_e::LOG_LEVEL_DEBUG] = [this] (const std::string& msg) {
						stub_print(msg);
					};

					break;
				case log_level_e::LOG_LEVEL_DEBUG:
					_log_map[log_level_e::LOG_LEVEL_USER] = [this] (const std::string& msg) {
						cout_print(msg);
					};

					_log_map[log_level_e::LOG_LEVEL_DEBUG] = [this] (const std::string& msg) {
						cout_print(msg);
					};

					_log_map[log_level_e::LOG_LEVEL_DEBUG] = [this] (const std::string& msg) {
						cout_print(msg);
					};

					break;
			}
		}
		
		inline void error(const std::string& msg) const {
			auto notice_msg = "error: " + msg;
			_log_map[log_level_e::LOG_LEVEL_ERROR](notice_msg);
		}

		inline void debug(const std::string& msg) const {
			auto debug_msg = "debug: " + msg;
			_log_map[log_level_e::LOG_LEVEL_DEBUG](debug_msg);
		}

		inline void user(const std::string& msg) const {
			_log_map[log_level_e::LOG_LEVEL_USER](msg);
		}

	private:
		using log_func_type = std::function<void(const std::string&)>; 
		log_level_e _log_level{log_level_e::LOG_LEVEL_USER};
		mutable std::unordered_map<log_level_e, log_func_type, std::hash<int>> _log_map;
};

logger& logger::get_instance() {
	static logger instance;

	return instance;
}

logger::logger() {
	_pimpl = std::make_unique<logger_impl>();
}

logger::~logger() = default;

void logger::set_level(log_level_e log_level) {
	_pimpl->set_level(log_level);
}
		
void logger::error(const std::string& msg) const {
	_pimpl->error(msg);
}

void logger::debug(const std::string& msg) const {
	_pimpl->debug(msg);
}

void logger::user(const std::string& msg) const {
	_pimpl->user(msg);
}
