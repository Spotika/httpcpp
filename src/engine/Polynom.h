#pragma once

#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "LinkedList.h"
#include "IWriteble.h"


class Polynom : public IWriteble {
public:

    struct Term : public IWriteble {
        int powers[26] = {};
        double weight = 0;

        Term() = default;

        ~Term() = default;

        std::string write_to_string() override {
            const double EPS = 0.001;
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
                    if (std::fabs((int)weight - weight) < EPS) {
                        result += std::to_string((int)weight);
                    } else {
                        result += std::to_string(weight);
                    }
                }
            } else {
                if (std::fabs((int)weight - weight) < EPS) {
                    result += std::to_string((int)weight);
                } else {
                    result += std::to_string(weight);
                }
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

        bool operator==(const Term& other) const {
            for (int i = 0; i < 26; ++i) {
                if (powers[i] != other.powers[i]) {
                    return false;
                }
            }
            if (weight != other.weight) {
                return false;
            }
            return true;
        }

        Term GetTheNthDerivative(int n, char target) const {
            Term result;

            int targetIndex = target - 'a';

            for (int i = 0; i < 26; ++i) {
                result.powers[i] = powers[i];                
            }

            result.weight = weight;

            while (n --> 0) {
                if (powers[targetIndex] == 0) {
                    result.weight = 0;
                    return result;
                }

                result.weight *= result.powers[targetIndex];
                result.powers[targetIndex] = result.powers[targetIndex] - 1;
            }

            return result;
        }

        Term operator*(const Term& other) const {
            Term result;

            result.weight = other.weight * weight;

            for (int i = 0; i < 26; ++i) {
                result.powers[i] = powers[i] + other.powers[i];
            }

            return result;
        }

        bool operator<=(const Term& other) const {
            return static_cast<int>(SumOfPowers()) - static_cast<int>(other.SumOfPowers()) >= 0;
        }

        Term operator/(const Term& other) const {
            Term result;

            for (int i = 0; i < 26; ++i) {
                result.powers[i] = std::max(0, powers[i] - other.powers[i]);
            }
            result.weight = weight / other.weight;
            return result;
        }
    };

    LinkedList<Term> polynom;

    Polynom() {
        read_from_string("");
    }

    Polynom(std::string polynomstring) {
        read_from_string(polynomstring);
    }

    Polynom GetTheNthDerivative(int n, char target) const {
        Polynom result;

        result.polynom = polynom;

        for (int i = 0; i < polynom.getSize(); ++i) {
            result.polynom[i] = polynom[i].GetTheNthDerivative(n, target);
        }

        result.Normalize_();
        return result;
    }

    std::string write_to_string() override {
        std::string result;
        for (int i = 0; i < polynom.getSize(); ++i) {
            std::string term = polynom[i].write_to_string();
            if (!term.empty() && term[0] == '-' && !result.empty()) {
                result.pop_back();
            }
            result += term;
            if (i != polynom.getSize() - 1) {
                result += "+";
            }
        }
        return result;
    }

    std::vector<int> FindRoots() const {
        int variable = 0;

        std::vector<int> roots;

        for (int i = 0; i < polynom.getSize(); ++i) {
            for (int j = 0; j < 26; ++j) {
                if (polynom[i].powers[j] != 0) {
                    variable = j;
                    goto end;        
                }
            }
        }

        end:

        int minWeight = 0;
        int currnetSumOfPowers = 1e9;
        for (int i = 0; i < polynom.getSize(); ++i) {
            if (polynom[i].SumOfPowers() < currnetSumOfPowers) {
                minWeight = static_cast<int>(polynom[i].weight);
                currnetSumOfPowers = static_cast<int>(polynom[i].SumOfPowers());
            }
        }

        std::vector<int> possible_roots = {0, 1};

        minWeight = abs(minWeight);

        for (int i = 2; i <= minWeight; ++i) {
            if (minWeight % i == 0) {
                possible_roots.push_back(i);
            }
        }


        for (int i = 0; i < possible_roots.size(); ++i) {
            double point[26] = {};
            point[variable] = possible_roots[i];
            double value = CalculateValueInPoint(point);

            if (std::fabs(value) < 0.001) {
                roots.push_back(possible_roots[i]);
            }

            point[variable] = -possible_roots[i];
            value = CalculateValueInPoint(point);

            if (std::fabs(value) < 0.001 && possible_roots[i]!= 0) {
                roots.push_back(-possible_roots[i]);
            }
        }
        return roots;
    }

    void read_from_string(std::string string) override {
        polynom.Clear();

        if (string.empty()) {
            polynom.Push({});
            Normalize_();
            return;
        }
        
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
            if (string[i] == ' ' || string[i] == '{' || string[i] == '}') continue;
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
                            polynom.Push(current_term);
                            current_term = Term();
                            if (current == '-') {
                                current_term.weight = -1;
                            } else {
                                current_term.weight = 1;
                            }
                            state = WAIT_WEIGHT;
                        } else if (current == '\0') {
                            polynom.Push(current_term);
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

    double CalculateValueInPoint(double point[]) const {
        double result = 0;

        for (int i = 0; i < polynom.getSize(); ++i) {
            double sum = 1;
            for (int j = 0; j < 26; ++j) {
                sum *= std::pow(point[j], polynom[i].powers[j]);
            }
            sum *= polynom[i].weight;

            result += sum;
        }

        return result;
    }

    std::pair<Polynom, Polynom> operator/(const Polynom& other) const {
        Polynom div;

        Polynom lhs = *this;
        Polynom rhs = other;

        while (lhs.polynom[0] <= rhs.polynom[0]) {
            auto local_ans = lhs.polynom[0] / rhs.polynom[0];

            div.polynom.Push(local_ans);

            auto decr = rhs * local_ans * Polynom("-1");
            lhs = lhs + decr;
        }

        div.Normalize_();
        lhs.Normalize_();

        return {div, lhs};
    }

    bool operator==(const Polynom& other) const {
        if (polynom.getSize() != other.polynom.getSize()) {
            return false;
        }
        for (int i = 0; i < polynom.getSize(); ++i) {
            if (polynom[i] != other.polynom[i]) {
                return false;
            }
        }
        return true;
    }

    Polynom operator+(const Polynom& other) const {
        Polynom result;

        for (int i = 0; i < polynom.getSize(); ++i) {
            result.polynom.Push(polynom[i]);
        }

        for (int i = 0; i < other.polynom.getSize(); ++i) {
            result.polynom.Push(other.polynom[i]);
        }

        result.Normalize_();
        return result;
    }

    Polynom operator*(const Polynom& other) const {
        Polynom result;

        // result.polynom ;

        for (int i = 0; i < other.polynom.getSize(); ++i) {
            result = result + operator*(other.polynom[i]);
        }
        result.Normalize_();
        return result;
    }

    Polynom operator*(const Term& other) const {
        Polynom result;

        for (int i = 0; i < polynom.getSize(); ++i) {
            result.polynom.Push(polynom[i] * other);
        }

        result.Normalize_();
        return result;
    }

private:
    void Normalize_() {
        polynom.Sort();

        LinkedList<Term> sp;

        Term current_term = polynom[0];

        for (int i = 1; i < polynom.getSize(); ++i) {
            if (current_term.EqualPowers(polynom[i])) {
                current_term += polynom[i];
            } else {
                sp.Push(current_term);
                current_term = polynom[i];
            }
        }

        sp.Push(current_term);
        
        polynom.Clear();        
        
        polynom = sp;

        for (int i = polynom.getSize() - 1; i >= 0; --i) {
            if (polynom.getSize() != 1) {
                if (polynom[i].weight == 0) {
                    polynom.Pop(i);
                }
            }
        }
    }

};
