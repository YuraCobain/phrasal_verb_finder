#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <memory>

enum log_level_e {
	LOG_LEVEL_USER,  // display the result of commands, alway on 
	LOG_LEVEL_ERROR, // display errors, on by default
	LOG_LEVEL_DEBUG  // display debug message, off by default
};

class logger {
	public:	
		logger& operator=(const logger&) = delete;
		logger(logger&&) = delete;
		logger& operator=(logger&&) = delete;
		static logger& get_instance();
		~logger();

		void set_level(log_level_e log_level);		
		void error(const std::string& msg) const;
		void debug(const std::string& msg) const;
		void user(const std::string& msg) const;

	private:
		logger(); 

		class logger_impl;
		std::unique_ptr<logger_impl> _pimpl;
};

#endif /* __LOGGER_H__ */
