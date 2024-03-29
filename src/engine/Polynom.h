#pragma once

#include <string>
#include "IWritable.h"
#include "LinkedList.h"

class Polynom : IWritable {
public:

    Polynom();
    Polynom(std::string string);

    static bool CheckPolynomString(std::string string) {
        return true;
    }
    
private:
    struct Term : IWritable {
        int factor = 0;
        int powers[26] = {};
    };

    LinkedList<Term> polynom_terms_;
};