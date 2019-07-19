#pragma once

#include <string>
#include <sstream>
#include <algorithm>

#include "core/common.hpp"
#include "array.hpp"

#define String std::string
#define StringStream std::stringstream

struct StringUtil {
	template <typename T>
	static inline String toString(T val) {
		return std::to_string(val);
	}

	static inline String toLower(const String& str) {
		String res = str;
		std::transform(res.begin(), res.end(), res.begin(), ::tolower);

		return res;
	}

	static inline bool startsWith(const String& s0, const String& s1) {
		return s0.compare(0, s1.length(), s1);
	}

	static void whitespaceSplit(Array<String>& elems, const String& s);
	static void split(Array<String>& elems, const String& s, char delim);
	static Array<String> split(const String& s, char delim);
	static String getFilePath(const String& fileName);
	static String getFileExtension(const String& fileName);
	
	static bool loadTextFileWithIncludes(StringStream& output, const String& fileName,
		const String& includeKeyword);
};

