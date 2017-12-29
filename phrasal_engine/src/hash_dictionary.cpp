#include "hash_dictionary.h"
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <fstream>
#include <regex>
#include "logger.h"

struct part_of_speech_hash
{
	template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
};

class hash_dictionary::dict_impl {
	public:
		dict_impl(const std::string& path_to_dict) {
			load_dictionary(path_to_dict);
		}

		void load_dictionary(const std::string& path_to_dict) {
			std::ifstream ifs(path_to_dict, std::ios::binary | std::ios::in);
			
			/* prefix used to sperate different type of speech in dictory db */
			std::map<std::string, part_of_speech_e> word_type_str2enum_map = {
				{std::string("verb"), part_of_speech_e::VERB_TYPE},
				{std::string("preposition"), part_of_speech_e::PREPOSITION_TYPE},
				{std::string("pronoun"), part_of_speech_e::PRONOUN_TYPE},
			};
			/* open dictoary db file*/
			if (ifs.good())
				logger::get_instance().debug("dictonary DB file: " + path_to_dict + " is opened");
			else
				throw std::runtime_error("failed to open file: " + path_to_dict);

			/* load dictonary into hash table */

			/* the file format of dictonary should be exactly:
			 * [string_type]:[string_word] 
			 *
			 * where [string_type] cloud be : "verb", "preposition", "pronoun"
			 * and   [string_word] specific word from specified part of speech
			 *
			 * each line that does not satify format will be skipped
			 * */

			std::string line;
			std::regex sep (":");

			logger::get_instance().debug("indexing user defined DB");
			while (std::getline(ifs, line)) {
				/* split the line by delimiter */
				std::sregex_token_iterator it(line.begin(), line.end(), sep, -1);
				std::sregex_token_iterator end;
				
				/* skip malfomed string */
				if (std::distance(it, end) != db_line_len) {
					logger::get_instance().debug("malformed line: [" + line + "] is skipped");
					continue;
				}
				
				/* fetch type of word and word itself */
				auto word_type_str = *it++; 
				auto word = *it; 
			
				/* in case of unknown word type skip it */ 
				if (!word_type_str2enum_map.count(word_type_str)) {
					continue;
				}

				/* other wise save the word in dictonary by its category */
				_dict[word_type_str2enum_map[word_type_str]].insert(word);
			}

			logger::get_instance().debug("dictonary DB is loaded");
			load_build_in_words();
		}

		void load_build_in_words() {
			logger::get_instance().debug("indexing built-in dictonary");
			_dict[part_of_speech_e::ARTICLE_TYPE].insert({"The", "the", "A", "a", "An", "an"});
			_dict[part_of_speech_e::PUNCTUATION_TYPE].insert({".", ",", "!", "?", ":", ";"});
		}

		inline void print() const {
			for(const auto& t : _dict) {
				for(const auto& w: t.second) {
					logger::get_instance().user(w);
				}
			}
		}

		/* check if a word belongs to concerete part of speech */
		inline bool is_verb(const std::string& word) const {
			return _dict[part_of_speech_e::VERB_TYPE].count(word);
		}

		inline bool is_preposition(const std::string& word) const {
			return _dict[part_of_speech_e::PREPOSITION_TYPE].count(word);
		}

		inline bool is_pronoun(const std::string& word) const {
			return _dict[part_of_speech_e::PRONOUN_TYPE].count(word);
		}

		inline bool is_article(const std::string& word) const {
			return _dict[part_of_speech_e::ARTICLE_TYPE].count(word);
		}

		inline bool is_punctuation(const std::string& word) const {
			return _dict[part_of_speech_e::PUNCTUATION_TYPE].count(word);
		}

		inline bool exists(part_of_speech_e type, const std::string& word) const {
			return _dict[type].count(word);
		}

	private:
		void insert_part_of_speech(const std::string& word, part_of_speech_e word_type) {

		}
		using part_of_speech_set = std::unordered_set<std::string>;
		using dict_map = std::unordered_map<part_of_speech_e, part_of_speech_set, part_of_speech_hash>; 
		const int db_line_len = 2; /* each line in db file shoudl consist exacly two words */
		
		mutable dict_map _dict;
};

hash_dictionary::~hash_dictionary() = default;

hash_dictionary::hash_dictionary(const std::string& path_to_dict) {
	_pimpl	= std::make_unique<dict_impl>(path_to_dict);
}

/* check if a word belongs to concerete part of speech */
bool hash_dictionary::is_verb(const std::string& word) const {
	return _pimpl->is_verb(word);
}

bool hash_dictionary::is_preposition(const std::string& word) const {
	return _pimpl->is_preposition(word);
}

bool hash_dictionary::is_pronoun(const std::string& word) const {
	return _pimpl->is_pronoun(word);
}

bool hash_dictionary::is_article(const std::string& word) const {
	return _pimpl->is_article(word);
}

bool hash_dictionary::is_punctuation(const std::string& word) const {
	return _pimpl->is_punctuation(word);
}


bool hash_dictionary::is_part_of(const part_of_speech_e type, const std::string& word) const {
	return _pimpl->exists(type, word);
}

void hash_dictionary::print() const {
	_pimpl->print();
}

