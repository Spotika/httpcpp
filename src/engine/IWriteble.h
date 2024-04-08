#pragma once

#include "string"


class IWriteble { 
public:
    virtual void read_from_string(std::string) {};
    virtual std::string write_to_string() {return "";};
};
