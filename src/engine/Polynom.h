#pragma once

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "LinkedList.h"
#include "IWriteble.h"


class Polynom : public IWriteble {
public:

    Polynom() {
        read_from_string("");
    }

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
                    } else if (string[i] == '+') {
                        current_term.weight = 1;
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

        Normalize_();
    }

    double CalculateValueInPoint(
        double point[]
    ) {
        double result = 0;

        for (int i = 0; i < polynom_.getSize(); ++i) {
            double sum = 1;
            for (int j = 0; j < 26; ++j) {
                sum *= std::pow(point[j], polynom_[i].powers[j]);
            }
            sum *= polynom_[i].weight;

            result += sum;
        }

        return result;
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
                if (SumOfPowers() == 0) {
                    result += '1';
                }
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

        size_t SumOfPowers() const {
            size_t result = 0;

            for (int i = 0; i < 26; ++i) {
                result += powers[i];
            }
            
            return result;
        }

        bool operator<(const Term& other) const {
            int sum = static_cast<int>(SumOfPowers()) - static_cast<int>(other.SumOfPowers());        
            if (sum == 0) {
                return ComparePowerLiterals(other);
            }

            return sum > 0;
        }

        bool EqualPowers(const Term& other) const {
            for (int i = 0; i < 26; ++i) {
                if (powers[i] != other.powers[i]) {
                    return false;
                }
            }
            return true;
        }

        bool ComparePowerLiterals(const Term& other) const {
            std::string first;
            std::string second;

            for (int i = 0; i < 26; ++i ) {
                if (powers[i] != 0) {
                    first += static_cast<char>(i + 'a');
                }
                if (other.powers[i] != 0) {
                    second += static_cast<char>(i + 'a');
                }
            }
            return first < second;
        }

        Term operator+(const Term& other) const {
            if (!EqualPowers(other)) {
                throw std::invalid_argument(
                    "Polynoms with different powers cannot be added"
                );
            }

            Term result(other);
            result.weight += weight;

            if (result.weight == 0) {
                for (int i = 0; i < 26; ++i) {
                    result.powers[i] = 0;
                }
            }

            return result;
        }

        Term& operator+=(const Term& other) {
            if (!EqualPowers(other)) {
                throw std::invalid_argument(
                    "Polynoms with different powers cannot be added"
                );
            }

            weight += other.weight;

            if (weight == 0) {
                for (int i = 0; i < 26; ++i) {
                    powers[i] = 0;
                }
            }

            return *this;
        }
    };

    void Normalize_() {
        polynom_.Sort();

        LinkedList<Term> sp;

        Term current_term = polynom_[0];

        for (int i = 1; i < polynom_.getSize(); ++i) {
            if (current_term.EqualPowers(polynom_[i])) {
                current_term += polynom_[i];
            } else {
                sp.Push(current_term);
                current_term = polynom_[i];
            }
        }

        sp.Push(current_term);
        
        polynom_.Clear();        
        
        polynom_ = sp;

        for (int i = polynom_.getSize() - 1; i >= 0; --i) {
            if (polynom_.getSize() != 1) {
                if (polynom_[i].weight == 0) {
                    polynom_.Pop(i);
                }
            }
        }
    }

    LinkedList<Term> polynom_;
};
