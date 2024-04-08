#pragma once

#include "httplib.h"
#include "engine/BaseModel.h"

#define JSON_CONTENT "application/json"
#define JSON_RESPONSE(json) res.set_content(json.toStyledString(), "application/json")

template<typename T>
concept DerivedFromBaseModel = std::derived_from<T, BaseModel>; // Определение концепта

template<DerivedFromBaseModel T>
std::function<void(const httplib::Request&, httplib::Response&)> ParseBody(
    std::function<void(const httplib::Request&, httplib::Response&, T)> inner
) {
    return [inner](const httplib::Request& req, httplib::Response& res) {
        T parsedBody;
        parsedBody.ParseBody(req.body);
        inner(req, res, parsedBody);
    }; // wrapper function
}
