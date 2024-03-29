#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Predicate.h"

using namespace std;

class Rule
{
private:
    Predicate headPredicate;
    vector<Predicate> bodyPredicates;

public:
    Rule() { }

    void setHead(Predicate head) {
        headPredicate = head;
    }

    Predicate getHead() {
        return headPredicate;
    }

    void addBodyPredicate(Predicate predicate) {
        bodyPredicates.push_back(predicate);
    }

    vector<Predicate> getPredicateList() {
        return bodyPredicates;
    }

    string toString() {
        stringstream out;
        out << headPredicate.toString() << " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); i++)
        {
            out << bodyPredicates.at(i).toString();
            if (i < bodyPredicates.size() - 1)
            {
                out << ',';
            }
        }
        return out.str();
    }
};
