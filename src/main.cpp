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
#include "engine/Polynom.h"

int main() {
    httplib::Server svr;

    LinkedList<Polynom> database;

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

        std::string pol = req.get_param_value("polynom");
        // std::cout << pol << std::endl;

        // req.getQueryParam("id");
        Polynom test;
        try {
            test.read_from_string(pol);
        } catch (const std::exception& e) {
            throw HTTPException(e.what(), HTTP_STATUS_CODE::BAD_REQUEST);
        }

        json["polynom"] = test.write_to_string();

        JSON_RESPONSE(json);
    });

    svr.Post("/insert", [&database](const auto& req, auto& res) {
        Json::Value json;

        std::string pol = req.get_param_value("polynom");
        // std::cout << pol << std::endl;

        // req.getQueryParam("id");
        Polynom p;
        try {
            p.read_from_string(pol);
        } catch (const std::exception& e) {
            throw HTTPException(e.what(), HTTP_STATUS_CODE::BAD_REQUEST);
        }

        database.Push(p);

        json["response"] = p.write_to_string();

        JSON_RESPONSE(json);
    });

    svr.Get("/list", [&database](const auto& req, auto& res) {
        Json::Value json;
        auto &response = json["response"];
        for (int i = 0; i < database.getSize(); i++) {
            response[i] = database[i].write_to_string();
        }

        JSON_RESPONSE(json);
    });

    svr.Get("/calculate", 
        [&database](const httplib::Request &req, httplib::Response &res) {    
            Json::Reader reader;    

            Json::Value inputArray;

            bool parsingSuccessful = reader.parse(req.get_param_value("point"), inputArray);

            double variables[26] = {};
            for (int i = 0; i < 26; i++) {
                variables[i] = inputArray[i].asDouble();
            }

            Polynom p(req.get_param_value("polynom"));
            

            Json::Value json;
            json["response"] = p.CalculateValueInPoint(variables);

            JSON_RESPONSE(json);
        }
    );

    svr.Get("/add", [&database](const auto& req, auto& res) {
        Json::Value json;
        
        Polynom lhs(req.get_param_value("lhs"));
        Polynom rhs(req.get_param_value("rhs"));

        json["response"] = (lhs + rhs).write_to_string();

        JSON_RESPONSE(json);
    });

    svr.Get("/multiply", [&database](const auto& req, auto& res) {
        Json::Value json;
        
        Polynom lhs(req.get_param_value("lhs"));
        Polynom rhs(req.get_param_value("rhs"));

        json["response"] = (lhs * rhs).write_to_string();

        JSON_RESPONSE(json);
    });

    svr.Get("/getTheNthDerivative", [](const auto& req, auto& res){
        Json::Value json;

        Polynom pol(req.get_param_value("polynom"));
        char target = req.get_param_value("target")[0];
        int nth = std::stoi(req.get_param_value("nth"));


        json["response"] = pol.GetTheNthDerivative(nth, target).write_to_string();

        JSON_RESPONSE(json);
    });

    svr.Get("/compare", [&database](const auto& req, auto& res) {
        Json::Value json;

        Polynom lhs(req.get_param_value("lhs"));
        Polynom rhs(req.get_param_value("rhs"));


        json["response"] = lhs == rhs;

        JSON_RESPONSE(json);
    });

    svr.Get("/findRoots", [&database](const auto& req, auto& res) {
        Json::Value json;

        Polynom pol(req.get_param_value("polynom"));


        std::vector<int> findRoots = pol.FindRoots();

        json["response"];

        for (int i = 0; i < findRoots.size(); i++) {
            json["response"][i] = findRoots[i];
        }

        JSON_RESPONSE(json);
    });

    svr.Get("/divide", [&database](const auto& req, auto& res) {
        Json::Value json;

        Polynom lhs(req.get_param_value("lhs"));
        Polynom rhs(req.get_param_value("rhs"));

        auto result = lhs / rhs;

        json["div"] = result.first.write_to_string();
        json["mod"] = result.second.write_to_string();

        JSON_RESPONSE(json);
    });

    svr.set_exception_handler([](auto& req, auto& res, std::exception_ptr ex) {
        Json::Value json;

        json["status"] = "error";
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
