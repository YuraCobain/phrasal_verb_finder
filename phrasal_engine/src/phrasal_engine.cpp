#include "phrasal_engine.h"
#include "hash_dictionary.h"
#include "logger.h"
#include <fstream>
#include <regex>
#include <iostream>

/* helper funtions */
static std::string word_type2str(const part_of_speech_e type) {
	switch(type) {
		case part_of_speech_e::ARTICLE_TYPE:
			return "article";
		case part_of_speech_e::PRONOUN_TYPE:
			return "pronoun";
		case part_of_speech_e::VERB_TYPE:
			return "verb";
		case part_of_speech_e::UNDEFINED_TYPE:
			return "undefined";
		case part_of_speech_e::PREPOSITION_TYPE:
			return "preposition";
		case part_of_speech_e::PUNCTUATION_TYPE:
		   return "punctuation";	
	}

	return "";
}

/* helper funtions */
static std::string presence2str(const presence_in_phrasal_e type) {
	switch(type) {
		case presence_in_phrasal_e::MANDATORY:
			return "mandatory";
		case presence_in_phrasal_e::OPTIONAL:
			return "optional";
		case presence_in_phrasal_e::LAST:
			return "last";
	}
	return "";
}
class phrasal_engine::engine_impl {
	public:
		explicit engine_impl(const std::string& path_to_dict):
			_dict(path_to_dict),
			_state(parse_fsm_state_e::RESET) {
			}

		void load_phrasal_pattern(const phrase_pattern& pattern) {
			_pattern = pattern;
		}

		void register_found_callback(found_pattern_callback found_cb) {
			_user_found_callback = std::move(found_cb);
		}

		void parse_file(const std::string& path_to_dict) {
			std::ifstream ifs(path_to_dict, std::ios::binary | std::ios::in);
			/* open dictoary db file*/
			if (ifs.good())
				logger::get_instance().debug("input file: " + path_to_dict + " is opened");
			else
				throw std::runtime_error("failed to open file: " + path_to_dict);

			/* split line by string and punctionations regex */
			std::regex re("[\\w']+|[.,!?;]");
			std::smatch m;
			uint32_t curr_line_num = 0;
			std::string curr_line;

			while (std::getline(ifs, curr_line)) {
				logger::get_instance().debug(curr_line);
				/* split line into words */
				std::sregex_iterator it(curr_line.begin(), curr_line.end(), re);
				std::sregex_iterator end;

				for (; it != end; ++it)
				{
					/* Skip ahead unsuitable word */
					if (_dict.is_article(it->str())) {
						logger::get_instance().debug("skip article \'" + it->str() + '\'');
						continue;
					}

					/* convert to smatch to have position property */
					std::smatch m = *it;

					/* run fsm word by word */	
					step_fsm(m, curr_line_num);

					/* little ugly check each iteration */
					report_if_phrasal_found(curr_line_num);
				}
				++curr_line_num;
			}
		}
	private:

		std::pair<std::string, bool> match_word_type(std::string word) {
			logger::get_instance().debug("analyzing \'" + word + '\'');
			logger::get_instance().debug("current pos in pattern: " + word_type2str(_curr_it->_type) + " is " 
					+ presence2str(_curr_it->_place));

			/* convert word to lower case due to dict supports only lower case words */
			std::transform(word.begin(), word.end(), word.begin(),::tolower);
		
			if (_dict.is_punctuation(word))	
				return std::make_pair("", false);

			/* in case of mismatch check for mandatory option */
			if (!_dict.is_part_of(_curr_it->_type, word)) {
				/* if mandatopy optional token return failure */
				if (_curr_it->_place == presence_in_phrasal_e::MANDATORY)
					return std::make_pair("", false);

				/* otherwise two options:
					1. current position is optional but phrasal does not have optional token check with the next mandatory
					2. it's optional word so it could be scipped */

				auto next_mandatory_it = std::find_if(_curr_it, _pattern.cend(), [] (const phrase_token_type &t) {
						return t._place == presence_in_phrasal_e::MANDATORY ; });
				if (next_mandatory_it == _pattern.cend())
					return std::make_pair("", true);
				
				/* ok give up and try the next word */
				if (!_dict.is_part_of(next_mandatory_it->_type, word))
					return std::make_pair("", true);

				_curr_it = next_mandatory_it;
				logger::get_instance().debug("next pos in pattern: " + word_type2str(_curr_it->_type) + " is " 
					+ presence2str(_curr_it->_place));
			}
	
			++_curr_it;
		
			return std::make_pair(word, true);
		}

		inline void reset_state_handler() {
			_match_phrase.clear();
			_match_position = 0;
			_curr_it = _pattern.cbegin();
			_state = LOOKUP_FIRST_MATCH;
		}

		inline void lookup_first_match_handler(const std::smatch& m) {
			std::string matched_word;
			bool matched_ok;

			std::tie(matched_word, matched_ok) = match_word_type(m.str(0));

			/* in case of the first match save postion, matched word and step pattern iterator */
			if (matched_ok) {
				if (!matched_word.empty())
					_match_phrase.push_back(matched_word);
				_match_position = m.position(0);
				_state = MATCHING;
				logger::get_instance().debug("first matched: " + matched_word);
			} else {
				logger::get_instance().debug("not matched with : " + m.str(0));
			} 
		}

		inline void matching_handle(const std::smatch& m) {
			std::string matched_word;
			bool matched_ok;

			std::tie(matched_word, matched_ok) = match_word_type(m.str(0));

			/* in case of the first match save postion, matched word and step pattern iterator */
			if (matched_ok) {
				if (!matched_word.empty())
					_match_phrase.push_back(matched_word);
				logger::get_instance().debug("matching: " + matched_word);
			} else {
				logger::get_instance().debug("reset on: " + m.str(0));
				_state = RESET;
			} 
		}

		/* if match is found report and reset fsm */
		inline void report_if_phrasal_found(const uint32_t curr_line_num) {
			if (_curr_it != _pattern.cend())
				return;

			/* join phrasal into string */
			std::stringstream phrasal_str;
			const char* delim = " ";
			std::copy(std::begin(_match_phrase), std::end(_match_phrase),
					std::ostream_iterator<std::string>(phrasal_str, delim));

			/* position string */
			std::string phrasal_postion_str("(" + std::to_string(_match_position) + ", " +
					std::to_string(curr_line_num) + ")");

			/* notify user about match */
			_user_found_callback(std::make_pair(phrasal_str.str(), phrasal_postion_str));

			/* reset the fsm */
			_state = RESET;
		}

		void step_fsm(const std::smatch& m, const uint32_t curr_line_num) {
			/* process current word */
			switch(_state) 
			{
				case RESET:
					reset_state_handler();
				case LOOKUP_FIRST_MATCH:
					lookup_first_match_handler(m);
					break;
				case MATCHING:
					matching_handle(m);
					break;
			}
		}

		hash_dictionary _dict;
		phrase_pattern _pattern;
		parse_fsm_state_e _state;
		std::vector<std::string> _match_phrase;
		uint32_t _match_position;
		phrase_pattern::const_phrase_it _curr_it;
		found_pattern_callback _user_found_callback;
};

phrasal_engine::phrasal_engine(const std::string& path_to_dict) {
	_pimpl = std::make_unique<engine_impl>(path_to_dict);
}

phrasal_engine::~phrasal_engine() = default;

void phrasal_engine::load_phrasal_pattern(const phrase_pattern& pattern) {
	_pimpl->load_phrasal_pattern(pattern);
}

void phrasal_engine::register_found_callback(found_pattern_callback found_cb) {
	_pimpl->register_found_callback(found_cb);
}

void phrasal_engine::parse_file(const std::string& path_to_dict) {
	_pimpl->parse_file(path_to_dict);
}
