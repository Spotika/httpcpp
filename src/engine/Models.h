#pragma once
#include "BaseModel.h"
#include "Exception.h"
#include <iostream>
#include <json/value.h>


class TestModel : public BaseModel {
private:
    bool ParseBody_(const Json::Value body) final {
        return true;
    }
};

// class PointInputScheme : public BaseModel {
// public:

//     long double point[26] = {};

// private:
//     bool ParseBody_(const Json::Value body) final {
//         // if (!body["point"].isArray()) {
//         //     return false;
//         // }

//         for (int i = 0; i < 26; ++i) {
//             std::cout << body["point"][i].asDouble() << std::endl;
//         }

//         return true;
//     }
// };