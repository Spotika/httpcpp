#pragma once

#include <json/reader.h>
#include <json/value.h>
#include<map>
#include<string>
#include<json/json.h>
#include "Exception.h"


class BaseModel {
public:
    void ParseBody(const std::string& body) {
        try {
            Json::Value root;
            Json::Reader reader;

            bool parsingSuccessful = reader.parse(body, root);

            if (!parsingSuccessful) {
                throw HTTPException("Unprocessable Entity: invalid body", HTTP_STATUS_CODE::UNPROCESSABLE_ENTITY);
            }

            bool result = ParseBody_(root);

            if (!result) {
                throw HTTPException("Unprocessable Entity: Invalid fields", HTTP_STATUS_CODE::UNPROCESSABLE_ENTITY);
            }
        } catch (HTTPException e) {
            throw;
        } catch (...) {
            throw HTTPException("Unprocessable Entity: invalid model", HTTP_STATUS_CODE::UNPROCESSABLE_ENTITY);
        }
    }
protected:
    virtual bool ParseBody_(const Json::Value body) {throw;}
};
