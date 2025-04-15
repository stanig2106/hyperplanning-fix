#include "crow_all.h"
#include <cpr/cpr.h>

static std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0;
    int valb = -8;
    for (unsigned char c: in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}


int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/get/<path>").methods("GET"_method)(
            [](const crow::request &req, std::string const &url) {

                std::string decoded_url = base64_decode(url);

                cpr::Response r = cpr::Get(cpr::Url{decoded_url});

                std::string content = r.text;

                const std::list<std::string> to_replace = {
                        "X-WR-CALNAME;LANGUAGE=fr:",
                        "X-WR-CALDESC;LANGUAGE=fr:"
                };

                for (const std::string &str: to_replace) {
                    size_t pos = content.find(str);
                    if (pos != std::string::npos) {
                        size_t endPos = content.find('\n', pos);
                        if (endPos != std::string::npos) {
                            content.replace(pos, endPos - pos, str + "Cours - Centrale\r");
                        }
                    }
                }

                crow::response res;
                res.body = content;

                res.code = (int) r.status_code;
                res.add_header("Content-Type", "text/calendar");
                res.add_header("Content-Disposition", "attachment; filename=data.ics");
                return res;
            });


    CROW_ROUTE(app, "/").methods("GET"_method)([] {
        std::ifstream in(CROW_STATIC_DIRECTORY "index.html",
                         std::ios::in | std::ios::binary);
        if (!in)
            return crow::response(404);
        std::stringstream buffer;
        buffer << in.rdbuf();
        std::string content(buffer.str());
        return crow::response(200, content);
    });


    app.port(80)
            .multithreaded().run();

    return 0;
}

