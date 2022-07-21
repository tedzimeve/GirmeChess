#pragma once
#include <utility>
#include <cctype>
#include <string>
/**
 * @brief 
 * write 
 * <code>using namespace ParserBaseFunctions;</code>
 * if u want use this commands
 * 
 */
namespace ParserBaseFunctions {
    static const std::string doLowerCase(const std::string& input) {
        std::string newString = "";
        const size_t len = input.length();
        for(int i = 0; i < len; ++i) {
            newString += std::tolower(input[i]);
        }
        return newString;
    }

    class HTTPDocStrDropper {
    public:
        enum eGetLineReturnType { FullLine, EndOfStr };
        const std::string& strref;
        unsigned int cursor = 0;

        HTTPDocStrDropper(const std::string& strref_) : strref(strref_) {}

        typedef std::pair<eGetLineReturnType, std::string> getLineReturn;
        HTTPDocStrDropper::getLineReturn getLine() {
            const size_t len = strref.length();
            std::string ret = "";
            ret.reserve(30);
            for(cursor = 0; cursor < len; ++cursor) {
                // new line is "/r/n"
                switch (strref[cursor]) {
                case '\r'://skip
                    break;
                case '\n':
                    return std::make_pair(FullLine, std::move(ret));
                    break;
                default:
                    ret += strref[cursor];
                    break;
                }
            }
            return std::make_pair(EndOfStr, std::move(ret));
        }

        std::string drop() {
            const size_t len = strref.length();
            std::string ret = "";
            ret.reserve(50);
            for(cursor = 0; cursor < len; ++cursor) {
                ret += strref[cursor];
            }
            return ret;
        }
    };
    class StrCopyDrpper {};
}