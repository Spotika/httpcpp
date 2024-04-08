#pragma once
#include "BaseModel.h"
#include "Exception.h"
#include <iostream>


class TestModel : public BaseModel {
private:
    bool ParseBody_(const Json::Value body) final {
        return true;
    }
};