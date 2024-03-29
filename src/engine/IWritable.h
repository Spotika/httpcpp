#pragma once

#include <string>

class IWritable {
public:
    virtual std::string write() {throw;};
    virtual void read(std::string) {throw;};
};