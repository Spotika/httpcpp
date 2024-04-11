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
