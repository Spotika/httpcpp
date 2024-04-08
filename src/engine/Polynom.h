#pragma once

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

#include "LinkedList.h"
#include "IWriteble.h"


class Polynom : public IWriteble {
public:

    Polynom(std::string polynom_string) {
        read_from_string(polynom_string);
    }

    std::string write_to_string() override {
        std::string result;
        for (int i = 0; i < polynom_.getSize(); ++i) {
            result += polynom_[i].write_to_string();
            if (i != polynom_.getSize() - 1) {
                result += " + ";
            }
        }
        return result;
    }

    void read_from_string(std::string string) override {
        polynom_.Clear();
        
        enum STATE { 
            BEGIN = 0,
            WAIT_WEIGHT = 1,
            WAIT_VARIABLE = 2,
            WAIT_POW = 3,
            END = 4
        };

        STATE state = BEGIN;

        string += '\0';

        Term current_term;
        std::string current_weight_string = "";
        std::string current_pow_string = "";
        char current;
        char current_pow_target = ' ';
        for (int i = 0; i < string.size(); ++i) {
            if (string[i] == ' ') continue;
            switch (state) {
                case BEGIN: // Начало парсинга
                    if (string[i] == '-') {
                        current_term.weight = -1;
                    } else {
                        current_term.weight = 1; --i;
                    }
                    state = WAIT_WEIGHT;
                    break;
                case WAIT_WEIGHT: // Ожидание веса полинома. Принимаются только цифры.
                    current = string[i];
                    if (current - '0' >= 0 && current - '0' <= 9) {
                        current_weight_string += string[i];                        
                    } else {
                        int current_weight = 1;
                        if (!current_weight_string.empty()) {
                            std::stringstream ss;
                            ss << current_weight_string;
                            ss >> current_weight;
                        }
                        current_term.weight *= current_weight;
                        current_weight_string.clear();
                        state = WAIT_VARIABLE; --i;
                    }
                    break;
                case WAIT_VARIABLE: // Ожидание переменной. Принимаются только буквы.
                    current = string[i];
                    if (current >= 'a' && current <= 'z') {
                        ++current_term.powers[current - 'a'];
                        current_pow_target = current;
                    } else {
                        if (current == '^') {
                            if (current_pow_target == ' ') {
                                throw std::invalid_argument(
                                    "Expected '^' after variable, got single '^'"
                                );
                            }
                            state = WAIT_POW;
                        } else if (current == '+' || current == '-') {
                            polynom_.Push(current_term);
                            current_term = Term();
                            if (current == '-') {
                                current_term.weight = -1;
                            } else {
                                current_term.weight = 1;
                            }
                            state = WAIT_WEIGHT;
                        } else if (current == '\0') {
                            polynom_.Push(current_term);
                            state = END;
                        } else {
                            throw std::invalid_argument(
                                "Unexpected character '"
                                + std::to_string(current)
                                + "' in polynom string at position "
                                + std::to_string(i) 
                                + "expected character from 'a' to 'z' or + or - or ^"
                            );
                        }
                    }
                    break;
                case WAIT_POW: // Ожидание '^'
                    current = string[i];
                    if (current - '0' >= 0 && current - '0' <= 9) {
                        current_pow_string += string[i];
                    } else {
                        if (!current_pow_string.empty()) {
                            std::stringstream ss;
                            ss << current_pow_string;
                            int pow;
                            ss >> pow;
                            current_term.powers[current_pow_target - 'a'] += pow - 1;
                            current_pow_target = ' ';
                            current_pow_string.clear();
                            state = WAIT_VARIABLE; --i;
                        } else {
                            throw std::invalid_argument(
                                "Unexpected character '"
                                + std::to_string(current)
                                + "' in polynom string at position "
                                + std::to_string(i) 
                                + "expected character from '0' to '9'"
                            );
                        }
                    }
                    break;
                case END:
                    break;
            }
        }
    }

private:
    struct Term : public IWriteble {
        int powers[26] = {};
        int weight = 0;

        Term() = default;

        ~Term() = default;

        std::string write_to_string() override {
            std::string result;

            if (weight == 0) {
                return result;
            }

            if (weight == 1) {

            } else if (weight == -1) {
                bool flag = false;
                for (int i = 0; i < 26; ++i) {
                    if (powers[i]!= 0) {
                        flag = true;
                        break;
                    }
                } 
                if (flag) {
                    result += "-";
                } else {
                    result += std::to_string(weight);
                }
            } else {
                result += std::to_string(weight);
            }
            
            for (int i = 0; i < 26; ++i) {
                if (powers[i] > 0) {
                    result += static_cast<char>(i + 'a');
                    if (powers[i] == 1) continue;
                    result += "^{";
                    result += std::to_string(powers[i]);
                    result += "}";
                }
            }
            return result;
        }
    };

    void Normalize_() {
        
    }

    LinkedList<Term> polynom_;
};
