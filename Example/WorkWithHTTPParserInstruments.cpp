#include "../service/HTTPParseBase.hpp"

int example() {
    __RequestParseStated reqstated;
    std::vector<std::string> partOfHttpDocument = {
        "GET /index.html http/v1.1\r\n" 
        "Host: localhost:8080\r\n"
        "Content-Length: ","10\r\n"
        "h4: v1\r\n\r","\n",
        "1234567890"
    };
    for (auto send : partOfHttpDocument) {
        Girme::GirmeServer::HTTPParser::parse(reqstated, send);
    }
}