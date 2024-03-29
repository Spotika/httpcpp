#include <exception>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <random>
#include <stdexcept>
#include <string>
#include "DataStash.h"
#include "httplib.h"
#include "helpers.h"

#include "engine/LinkedList.h"
#include "engine/Polynom.h"


int main() {

    httplib::Server svr;

    svr.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
    });


    svr.Get("/ping", [](const httplib::Request &, httplib::Response &res) {
        Json::Value json;
        json["status"] = "OK";
        res.set_content(JSON_RESPONSE(json));
    });


    svr.set_exception_handler([](auto& req, auto& res, std::exception_ptr ex) {
        Json::Value json;

        json["status"] = "Internal server error";

        try {
            if (ex) {
                std::rethrow_exception(ex);
            }
        } catch (const std::exception& e) {
            json["detail"] = e.what();
        }

        res.set_content(JSON_RESPONSE(json));
        res.status = 500;
    });

    svr.listen("127.0.0.1", 4242);

    return 0;
}
