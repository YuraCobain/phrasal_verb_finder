#ifndef __PHRASAL_ENGINE_H__
#define __PHRASAL_ENGINE_H__

#include <functional>
#include <memory>
#include "phrase_pattern.h"

using found_pattern_callback = std::function<void(std::pair<std::string, std::string>)>;

class phrasal_engine {
	public:
		explicit phrasal_engine(const std::string& path_to_dict);
		~phrasal_engine();
		void load_phrasal_pattern(const phrase_pattern& pattern);
		void register_found_callback(found_pattern_callback);
		void parse_file(const std::string& path_to_dict);
	private:
		class engine_impl;
		std::unique_ptr<engine_impl> _pimpl;
};

#endif /*__PHRASAL_ENGINE_H__*/
