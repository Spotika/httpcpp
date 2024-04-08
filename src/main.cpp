#include <exception>
#include <functional>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <random>
#include <stdexcept>
#include <string>
#include "Exception.h"
#include "httplib.h"

#include "helpers.h"
#include "engine/Models.h"
#include "engine/LinkedList.h"
#include "engine/DataBase.h"
#include "engine/Polynom.h"

int main() {

    httplib::Server svr;

    svr.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
    });


    // svr.Post("/ping", ParseBody<TestModel>([](const httplib::Request &, httplib::Response &res, TestModel parsedBody) {
    //     Json::Value json;
    //     json["response"] = "OK";

    //     std::cout << "ping" << std::endl;
    //     JSON_RESPONSE(json);
    // }));

    svr.Get("/ping", [](const auto& req, auto& res) {
        Json::Value json;
        json["response"] = "OK";
        JSON_RESPONSE(json);
    });

    svr.Get("/test", [](const auto& req, auto& res) {
        Json::Value json;
        json["response"] = "OK";

        // req.getQueryParam("id");
        
        std::string pol = req.get_param_value("polynom");
        // std::cout << pol << std::endl;
        Polynom test(pol);

        json["polynom"] = test.write_to_string();

        JSON_RESPONSE(json);
    });

    svr.set_exception_handler([](auto& req, auto& res, std::exception_ptr ex) {
        Json::Value json;

        json["status"] = "Internal server error";
        try {
            if (ex) {
                std::rethrow_exception(ex);
            }
        } catch (HTTPException& e) {
            json["detail"] = e.what();
            res.status = e.status();
        } catch (const std::exception& e) {
            json["detail"] = e.what();
            res.status = 500;
        }

        JSON_RESPONSE(json);
    });


    svr.listen("127.0.0.1", 4242);


    // LinkedList<int> sp;

    // sp.Push(1);
    // sp.Push(2);
    // sp.Push(3);
    // sp.Push(4);
    // sp.Push(5);


    // for (int i = 0; i < sp.getSize(); i++) {
    //     std::cout << sp[i] << " ";
    // }

    // std::cout << std::endl;

    // sp.Pop(4);

    // for (int i = 0; i < sp.getSize(); i++) {
    //     std::cout << sp[i] << " ";
    // }

    // std::cout << std::endl;

    // sp.Pop(0);
    // sp.Pop(1);
    // for (int i = 0; i < sp.getSize(); i++) {
    //     std::cout << sp[i] << " ";
    // }

    // std::cout << std::endl;

   // for (int i = 0; i < 10; i++) {
    //     sp.Push(i);
    // }

    // for (int i = 0; i < 10; i++) {
    //     std::cout << sp[i] << std::endl;
    // }

    // std::cout << sp[0] << sp[1] << sp[9] << sp[1];
 
    return 0;
}
