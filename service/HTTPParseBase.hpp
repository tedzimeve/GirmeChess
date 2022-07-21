#pragma once
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <winsock2.h>
#include <string>
#include <map>
#include "../helpers/ParserBaseFunctions.hpp"

using namespace ParserBaseFunctions;
#define STANDART_HTTPPATH_LEN 20

namespace Girme {
namespace GirmeServer {

    /**
     * @brief 
     * Экземпляр запроса хранящий в себе данные запроса
     */
    struct Request {
    public:
        enum eHTTPMethod { GET, UNSUPPORTED }; // add support to OPRION is necessarily
        const eHTTPMethod method = UNSUPPORTED;
        const std::string path = "/";
        const std::string version = "";

        const std::string body; 
    };
    /**
     * @brief 
     * Собираемый пакет запроса со стейт машинной процесса анализа HTTP документа
     */
    struct __RequestParseStated {
    public:
        typedef Request::eHTTPMethod eHTTPMethod; // add support to OPRION is necessarily
        eHTTPMethod method = eHTTPMethod::UNSUPPORTED;
        std::string path = "/";
        std::string version = "";
        std::map<const std::string, const std::string> headers;
        /**
         * @brief 
         * состояния стейт-машины анализа пакета
         */
        enum eRequestParseState { 
            ///< чтение Заглавия
            TitleParse=0, 
            ///< чтение Заголовка
            HeadersParse=1, 
            ///< чтение тела документа
            MaybeBodyState=2, 
            ///< чтение Заглавия прекратилось на середине строки
            TitleParted=3, 
            ///< чтение Заголовка прекратилось на середине строки
            HeadersParted=4, 
            ///< ???
            BodyParted=5, 
            ///< чтение пакета завершено
            Complete=200 };
        ///< текущее состояние анализа пакета
        eRequestParseState state = TitleParse;

        /**
         * @brief 
         * проАлиасил состояния в частичные состояния - это надо, 
         * если строка закончилась на середине, и парсер переходит в состояние ожидания,
         * чтобы продолжить с нужной точки и без потери данных
         */
        static constexpr eRequestParseState PStatedAllias[] {
            eRequestParseState::TitleParted, eRequestParseState::HeadersParted, eRequestParseState::BodyParted,
            eRequestParseState::TitleParted, eRequestParseState::HeadersParted, eRequestParseState::BodyParted,
        };
        /**
         * @brief 
         * проАлиасил состояния в полные состояния - это надо, 
         * если мы восстановили строку полностью и хотим расчитать её значение
         */
        static constexpr eRequestParseState FullStatedAllias[] {
            eRequestParseState::TitleParse, eRequestParseState::HeadersParse, eRequestParseState::MaybeBodyState,
            eRequestParseState::TitleParse, eRequestParseState::HeadersParse, eRequestParseState::MaybeBodyState,
        };
        /**
         * @brief 
         * нужен лин чтобы определить, что текущее состояние при целой строке или частичной
         */
        static constexpr bool isPStated[] {
            false, false, false, 
            true, true, true
        };
        /**
         * @brief 
         * нужен лин чтобы определить, что текущее состояние при целой строке или частичной
         */
        static constexpr bool isBodyParseStated[] {
            false, false, true, 
            false, false, true
        };


        void addHeaderLine(const std::string HeaderName, const std::string HeaderValue) {
            headers.emplace(doLowerCase(HeaderName), HeaderValue);
        }
        const size_t getContentLength() {
            auto iter = headers.find("content-length");
            try{
                return (iter != headers.end()) ? std::stoi((*iter).second) : 0;
            } catch (...) {
                return 0;
            }
        }
        bool isContainsBody() {
            return getContentLength() != 0;
        }
        ///< частичная строка
        std::string _partedline = "";
        void savePartedLine(const std::string partedline) {
            _partedline = partedline;
        }
        std::string popPartedLine() {
            return _partedline;
        }
        ///< тело запроса
        std::string _body = "";
        unsigned int contentlength = 0;
        unsigned int appendBody(const std::string body) {
            _body += body;
            contentlength = body.size();
            return contentlength;
        }
    };
    using eHTTPMethod = Request::eHTTPMethod;
    class HTTPParser {
    public:
        typedef Request::eHTTPMethod eHTTPMethod; 
        /**
         * @brief 
         * Метод аналаза HTTP formatted документа с возможностью полного анализа 
         * или частичного. 
         * @param req Собираемый пакет запроса со стейт машинной процесса анализа
         * @param input полностью HTTP formatted документ или его доля
         * @return возвращает состояние анализа зхапроса
         * @todo Refactor state-machine!!!
         */
        static __RequestParseStated::eRequestParseState parse(__RequestParseStated& req, std::string input) {
            using eParseState = __RequestParseStated::eRequestParseState;


            ///< Dropper of lines by line from the HTTP Doc or its part
            HTTPDocStrDropper sdr(input);
            while (true) {
                if(__RequestParseStated::isBodyParseStated[req.state]) {
                    const size_t ContentLen = req.getContentLength();
                    unsigned int parsedlen = req.contentlength;

                    const std::string strl = sdr.drop();
                    const size_t inputlen = strl.length();
                    std::string parsed = "";//нужно переименовать в reader
                    parsed.reserve(50);

                    for(int i = 0; parsedlen < ContentLen && i < inputlen; ++parsedlen, ++i) {
                        parsed += strl[i];
                    }
                    req.appendBody(parsed);
                    if (req.contentlength >= ContentLen) {
                        req.state = eParseState::Complete;
                    }
                    return req.state;
                } else if(req.state == eParseState::Complete) {
                    return req.state;
                } else {
                    ///< one line from the HTTP Doc
                    const HTTPDocStrDropper::getLineReturn line = sdr.getLine();

                    // Is current line Parted or Full
                    if(line.first == HTTPDocStrDropper::eGetLineReturnType::EndOfStr) { // while parted
                        req.savePartedLine(
                            __RequestParseStated::isPStated[req.state] // если сейчас Parted состояние
                            ? (req.popPartedLine() + line.second) // то расширяем уже сохраннённую строку
                            : line.second // иначе просто сохраняем только текущую строку
                        );
                        req.state = __RequestParseStated::PStatedAllias[req.state];
                        return req.state;
                    } else { // if full
                        std::string strline = line.second;
                        if(__RequestParseStated::isPStated[req.state]) {
                            strline = req.popPartedLine() + line.second;
                            req.state = __RequestParseStated::FullStatedAllias[req.state];
                        }
                        switch (req.state) {
                            case eParseState::TitleParse:
                                doTitleParse(req, strline);
                                req.state = eParseState::HeadersParse;
                                break;
                            case eParseState::HeadersParse:
                                if(strline.length() == 0) { // detected twice line break 
                                    // this is lucky situaton of detecte twice break
                                    // but by my analyze in my mind at this moment i hasn't found
                                    //       an other situation where we can find thre twice line breaks..
                                    //       maybe because we don't analyze headers in other places
                                    //       only parses line if they if full
                                    req.state = eParseState::Complete;
                                    if(req.isContainsBody()) {
                                        req.state = eParseState::MaybeBodyState;
                                    }
                                } else 
                                    doHeaderStringParse(req, strline);
                                break;
                            case eParseState::MaybeBodyState:
                                break;
                        }
                    }
                }
            }
            return req.state;
        }

    private:
        /**
         * @brief 
         * часть функции parse(req, input) которая выполняет анализ отдельной строки или принимает тело документа
         * @param req 
         * @param req 
         */
        static void __parse_interv__Line_or_Body(__RequestParseStated& req, const std::string input) { }
        /**
         * @brief 
         * Очень доверчивый метод, считает что ему точно подадут HeaderSimpleFormatted строку. 
         * А ещё и простой. Не умеет в эти ваши навороченные форматты с 
         * <code>HeaderName: val1,val2,val3</code> и другие форматы
         * @param req Собираемый пакет запроса со стейт машинной процесса анализа
         * @param headerFormattedString строка в формате <code>$HeaderName: $Value</code>
         */
        static void doHeaderStringParse(__RequestParseStated& req, const std::string& headerFormattedString) {
            struct temp { enum eHeaderParseState { nHeaderName, nSpace, nValue }; };
            using eHeaderParseState = temp::eHeaderParseState;
            static const unsigned int STANDART_HEADERNAME_LEN = 20;
            static const unsigned int STANDART_HEADERVALUE_LEN = 20;
            const size_t len = headerFormattedString.length();
            std::string HeaderName = ""; HeaderName.reserve(STANDART_HEADERNAME_LEN);
            std::string HeaderValue = ""; HeaderValue.reserve(STANDART_HEADERVALUE_LEN);
            eHeaderParseState state = eHeaderParseState::nHeaderName;

            for(int cursor = 0; cursor < len; ++cursor) {
                const char ch = headerFormattedString[cursor];
                switch (state) {
                case eHeaderParseState::nHeaderName:
                    switch (ch) {
                        case ':': // if found ":" it's a sign of end of HeaderName
                            state = eHeaderParseState::nSpace;
                            break;
                        case ' ': //ignores spaces
                            break;
                        default: //else appends HeaderName
                            HeaderName += ch;
                            break;
                    }
                    break;
                case eHeaderParseState::nSpace:
                    switch (ch) {
                        case ' ': //ignores spaces until we handle other char
                            break;
                        default: //we handle
                            HeaderValue += ch;
                            state = eHeaderParseState::nValue;
                            break;
                    }
                    break;
                case eHeaderParseState::nValue:
                    HeaderValue += ch;
                    break;
                }
            }
            req.addHeaderLine(HeaderName, HeaderValue);
        }
        /**
         * @brief 
         * очень доверчивый метод, считает что ему точно подадут TitleFormatted строку
         * @param req Собираемый пакет запроса со стейт машинной процесса анализа
         * @param titleFormattedString строка в формате <code>$MethodName $Path $Version</code>
         */
        static void doTitleParse(__RequestParseStated& req, const std::string& titleFormattedString) {
            enum eTitleParseState { Method, Path, Version };
            static const unsigned int STANDART_METHODNAME_LEN = 5;
            static const unsigned int STANDART_HTTPVERSION_LEN = 8;
            const size_t len = titleFormattedString.length();
            std::string ret = ""; ret.reserve(STANDART_METHODNAME_LEN);
            eTitleParseState state = Method;

            for(int cursor = 0; cursor < len; ++cursor) {
                const char ch = titleFormattedString[cursor];
                if(ch != ' ') {
                    ret += ch;
                } else
                    switch (state) {
                    case Method:
                        if("GET" == ret) {
                            req.method = eHTTPMethod::GET;
                        } else {
                            req.method = eHTTPMethod::UNSUPPORTED;
                        }
                        ret = ""; ret.reserve(STANDART_HTTPVERSION_LEN);
                        state = Path;
                        break;
                    case Path:
                        req.path = ret;
                        ret = ""; ret.reserve(STANDART_HTTPPATH_LEN);
                        state = Version;
                        break;
                    case Version:
                        req.version = ret;
                        break;
                    }
            }
            req.version = ret;
        }
        HTTPParser() {}
    };
    /*
     * Unstable ContentReciever
     * 
     */
    class ContentReciever {
    public:
        std::string recieved_data;
        SOCKET sock;
        __RequestParseStated req;
        
        ContentReciever(SOCKET sock) : sock(sock) {}

        void recieve() {
            
        }
    };


} //GirmeServer
} //Girme