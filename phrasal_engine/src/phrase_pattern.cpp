#include "phrase_pattern.h"

phrase_pattern::const_phrase_it phrase_pattern::cbegin() const {
	return std::cbegin(_pattern);
}

phrase_pattern::const_phrase_it phrase_pattern::cend() const {
	return std::cend(_pattern);
}

void phrase_pattern::add_token(phrase_token_type token) {
	_pattern.emplace_back(std::move(token));
}

