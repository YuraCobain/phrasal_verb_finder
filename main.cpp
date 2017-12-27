#include <iostream>
#include <string>
#include <fstream>

#include "cxxopts.hpp" 

class logger {
	public:	
	    enum log_level_e {
			LOG_LEVEL_USER,
			LOG_LEVEL_ERROR,
			LOG_LEVEL_DEBUG
		};	
		logger& operator=(const logger&) = delete;
		logger(logger&&) = delete;
		logger& operator=(logger&&) = delete;
		static logger& get_instance() {
			static logger instance;

			return instance;
		}

		void set_log_level(log_level_e log_level) {
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
		
		void error(const std::string& msg) {
			auto notice_msg = "error: " + msg;
			_log_map[log_level_e::LOG_LEVEL_ERROR](notice_msg);
		}

		void debug(const std::string& msg) {
			auto debug_msg = "debug: " + msg;
			_log_map[log_level_e::LOG_LEVEL_DEBUG](debug_msg);
		}

		void user(const std::string& msg) {
			_log_map[log_level_e::LOG_LEVEL_USER](msg);
		}


	private:
		logger() {
			set_log_level(_log_level);
		}

		void cout_print(const std::string& msg) const {
			std::cout << msg << '\n'; 
		}

		void stub_print(const std::string& msg) const {
		}

		using log_func_type = std::function<void(const std::string&)>; 
		log_level_e _log_level{log_level_e::LOG_LEVEL_USER};
		std::unordered_map<logger::log_level_e, log_func_type, std::hash<int>> _log_map;
};

int main(int argc, char** argv) {
	try {
		cxxopts::Options options("Fresal_finder", "Utility to find fresal according to dictonary");

		options
			.show_positional_help();
		options.add_options()
			("file", "target file to search in", cxxopts::value<std::string>())
			("dict", "path to dictonary", cxxopts::value<std::string>())
			("d, debug", "debug ")
			("h,help", "usage: ./finder --file [path to input file] --dict [path to dictinary] ");

		auto result = options.parse(argc, argv);

		/* initialize mandatory options to empty/zero */
		std::string dict_db_path("");
		std::string file_to_parse_path("");
		auto debug{0};

		/* parse all options firstly */
		if (result.count("dict")) {
			dict_db_path = result["dict"].as<std::string>();
		} else {
			throw cxxopts::OptionException("dict option is mandatory");
		}

		std::ifstream ifs(dict_db_path, std::ios::binary | std::ios::in);
		if (ifs.good())
			std::cout << "Opened file!";
		else
			throw std::runtime_error("failed to open file: " + dict_db_path);

		std::string line;
		while (std::getline(ifs, line)) {
			logger::get_instance().user(line);
		}
	} catch (const cxxopts::OptionException& e) {
		logger::get_instance().error(std::string("parsing options: ") + e.what());
		exit(1);
	} catch (const std::runtime_error& e) {
		logger::get_instance().error(std::string("runtime : ") + e.what());
		exit(1);
	}

	return 0;
}
