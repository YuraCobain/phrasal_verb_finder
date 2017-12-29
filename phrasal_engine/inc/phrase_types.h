#ifndef __PHRASE_TYPES_H__
#define __PHRASE_TYPES_H__

/* enums of supported type of speech */
enum class part_of_speech_e {
	VERB_TYPE = 0,
	PREPOSITION_TYPE,
	PRONOUN_TYPE,
	ARTICLE_TYPE,
	PUNCTUATION_TYPE,
	UNDEFINED_TYPE
};

/* enums of supported type of speech */
enum class presence_in_phrasal_e {
	OPTIONAL = 0,
	MANDATORY,
	LAST
};

/* enums of supported type of speech */
enum parse_fsm_state_e {
	RESET,
	LOOKUP_FIRST_MATCH,
	MATCHING
};

#endif /* __PHRASE_TYPES__ */
