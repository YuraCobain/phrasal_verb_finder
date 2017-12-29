#ifndef __HASH_DICTONARY_H__
#define __HASH_DICTONARY_H__

#include <string>
#include <memory>
#include "phrase_types.h"

/* hash_dictionary is collection of hashes for specific part of speech */
class hash_dictionary {
	public:
		hash_dictionary(const hash_dictionary&) = delete;
		hash_dictionary(hash_dictionary&&) = default;
		hash_dictionary& operator=(const hash_dictionary&) = delete;
		hash_dictionary& operator=(hash_dictionary&&) = default;
		~hash_dictionary();

		explicit hash_dictionary(const std::string& path_to_dict);

		/* check if a word belongs to concerete part of speech */
		bool is_verb(const std::string& word) const;
		bool is_preposition(const std::string& word) const;
		bool is_pronoun(const std::string& word) const;
		bool is_article(const std::string& word) const; 
		bool is_punctuation(const std::string& word) const; 
		bool is_part_of(part_of_speech_e type, const std::string& word) const;
		void print() const;
	private:
		class dict_impl;
		std::unique_ptr<dict_impl> _pimpl;
};

#endif /* __DICTIONARY__ */
