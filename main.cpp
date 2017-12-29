#include <iostream>
#include <string>
#include <fstream>

#include "cxxopts.hpp" 
#include "logger.h"
#include "phrasal_engine.h"

int main(int argc, char** argv) {
	cxxopts::Options options("phrasal_verb_finder", "Utility to find phrasal verb according to dictonary match");

	options
		.show_positional_help();

	/* define cli options */
	options.add_options()
		("f,file", "target file to search in", cxxopts::value<std::string>())
		("d,dict", "path to dictonary", cxxopts::value<std::string>())
		("v, verbose", "debug ")
		("h,help", "print help");

	try {
		/* parse inputoption */
		auto result = options.parse(argc, argv);

		/* initialize mandatory options to empty/zero */
		std::string dict_db_path("");
		std::string file_to_parse_path("");

		if (result.count("help"))
		{
			logger::get_instance().user(options.help({"", "Group"}));
			exit(0);
		}

		/* verify mandatory parsed arguments */
		if (result.count("dict")) {
			dict_db_path = result["dict"].as<std::string>();
		} else {
			throw cxxopts::OptionException("dict option is mandatory");
		}

		if (result.count("file")) {
			file_to_parse_path = result["file"].as<std::string>();
		} else {
			throw cxxopts::OptionException("file option is mandatory");
		}

		if (result.count("v")) {
			logger::get_instance().set_level(log_level_e::LOG_LEVEL_DEBUG);
		}
		 
		/* create engine and pass predifined dictonary */
		phrasal_engine engine(dict_db_path);

		/* prepare pattern for phrasal verb */	
	 	phrase_pattern pattern;
		pattern.add_token(phrase_token_type{part_of_speech_e::VERB_TYPE, presence_in_phrasal_e::MANDATORY});
		pattern.add_token(phrase_token_type{part_of_speech_e::UNDEFINED_TYPE, presence_in_phrasal_e::OPTIONAL});
		pattern.add_token(phrase_token_type{part_of_speech_e::PREPOSITION_TYPE, presence_in_phrasal_e::MANDATORY});

		/* register user callback to report found matches */
		engine.load_phrasal_pattern(pattern);
		engine.register_found_callback([](auto phrasal_pos_pair) {
				logger::get_instance().user("found phrasal verb: \'" + 
						phrasal_pos_pair.first + "\' at position " + phrasal_pos_pair.second);
				});

		/* let's party begin */
		engine.parse_file(file_to_parse_path);

	} catch (const cxxopts::OptionException& e) {
		logger::get_instance().error(std::string("parsing options: ") + e.what());
		logger::get_instance().user(options.help({"", "Group"}));
		exit(1);
	} catch (const std::runtime_error& e) {
		logger::get_instance().error(std::string("runtime : ") + e.what());
		exit(1);
	}

	return 0;
}
