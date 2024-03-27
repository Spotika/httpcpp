#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include "httplib.h"

int main() {

    httplib::Server svr;

    svr.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
    });

    svr.Get("/ping", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "application/json");
    });

    svr.listen("127.0.0.1", 4242);

    return 0;
}
