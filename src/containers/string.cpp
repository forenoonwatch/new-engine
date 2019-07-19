#include "string.hpp"

#include <fstream>
#include <cctype>

void StringUtil::whitespaceSplit(Array<String>& elems, const String& s) {
	const char* cstr = s.c_str();
    unsigned int strLength = (unsigned int)s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while (end <= strLength) {
        while (end <= strLength) {
            if (std::isspace(cstr[end])) {
                break;
			}

            ++end;
        }
            
        elems.push_back(s.substr(start, end - start));

		start = end;

		while (start < strLength && std::isspace(cstr[start])) {
			++start;
		}

        end = start;
    }
}

void StringUtil::split(Array<String>& elems, const String& s, char delim) {
    const char* cstr = s.c_str();
    unsigned int strLength = (unsigned int)s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while (end <= strLength) {
        while (end <= strLength) {
            if (cstr[end] == delim) {
                break;
			}

            ++end;
        }
            
        elems.push_back(s.substr(start, end - start));

        start = end + 1;
        end = start;
    }
}

Array<String> StringUtil::split(const String& s, char delim) {
	Array<String> elems;
	StringUtil::split(elems, s, delim);

	return elems;
}

String StringUtil::getFilePath(const String& fileName) {
	const char* cstr = fileName.c_str();
	unsigned int strLength = (unsigned int)fileName.length();
	unsigned int end = strLength - 1;
	
	while (end != 0) {
		if (cstr[end] == '/' || cstr[end] == '\\') {
			break;
		}

		--end;
	}

	if (end == 0) {
		return fileName;
	}
	else {
		unsigned int start = 0;
		end = end + 1;
		return fileName.substr(start, end - start);
	}
}

String StringUtil::getFileExtension(const String& fileName) {
	const char* cstr = fileName.c_str();
	unsigned int strLength = (unsigned int)fileName.length();
	unsigned int end = strLength - 1;

	while (end > 0 && !std::isalnum(cstr[end])) {
		--end;
	}

	unsigned int start = end;

	while (start > 0) {
		if (cstr[start] == '.') {
			return fileName.substr(start + 1, end - start);
		}
		else if (cstr[start] == '/' || cstr[start] == '\\') {
			return "";
		}

		--start;
	}

	return "";
}

bool StringUtil::loadTextFileWithIncludes(StringStream& output, const String& fileName,
		const String& includeKeyword) {
	std::ifstream file;
	file.open(fileName.c_str());

	String filePath = getFilePath(fileName);
	String line;

	if (file.is_open()) {
		while (file.good()) {
			std::getline(file, line);
			
			if (line.find(includeKeyword) == String::npos) {
				output << line << "\n";
			}
			else {
				String includeFileName = StringUtil::split(line, ' ')[1];
				includeFileName =
					includeFileName.substr(1, includeFileName.length() - 2);

				loadTextFileWithIncludes(output, filePath + includeFileName,
						includeKeyword);
				output << "\n";
			}
		}
	}
	else {
		DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR, "Unable to load shader: %s",
				fileName.c_str());
		return false;
	}

	return true;
}

