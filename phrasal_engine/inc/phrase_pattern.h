#ifndef __PHRASE_PATTERN_H__
#define __PHRASE_PATTERN_H__

#include "phrase_types.h"
#include <vector>

typedef struct {
	part_of_speech_e _type;
	presence_in_phrasal_e _place;
} phrase_token_type;

class phrase_pattern {
	using phrase_pattern_vec = std::vector<phrase_token_type>;
	public:
		using const_phrase_it = phrase_pattern_vec::const_iterator;

		phrase_pattern() = default;
		phrase_pattern(const phrase_pattern&) = default;
		phrase_pattern& operator=(const phrase_pattern&) = default;
		phrase_pattern(phrase_pattern&&) = default;
		phrase_pattern& operator=(phrase_pattern&&) = default;

		const_phrase_it cbegin() const;
		const_phrase_it cend() const;

		void add_token(phrase_token_type token);
			
	private:
		phrase_pattern_vec _pattern;
};

#endif /* __PHRASE_PATTERN_H__ */
