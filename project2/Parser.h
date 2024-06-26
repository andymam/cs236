#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "Token.h"
#include "DatalogProgram.h"
using namespace std;

class Parser {
private:
    vector<Token> tokens;
    DatalogProgram datalog;
    
public:
    Parser(const vector<Token>& tokens) : tokens(tokens) { }

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }
    
    void throwError(Token token) {
        cout << "Failure!" << endl;
        cout << "  " << token.toString() << endl;
    }

    void match(TokenType t) {
        if (tokenType() == t) {
            advanceToken();
        } else {
            throw tokens.at(0);
        }

    }

    void parse() {
        try {
            parseDatalog();
            printSuccess();
        } catch (Token badToken) {
            throwError(badToken);
        }
    }

    void printSuccess() {
        cout << "Success!" << endl;
        cout << datalog.printSchemes();
        cout << datalog.printFacts();
        cout << datalog.printRules();
        cout << datalog.printQueries();
        cout << datalog.printDomain();
    }

    void parseDatalog() {
        if (tokenType() == SCHEMES) {
            match(SCHEMES);
            match(COLON);
            scheme();
            schemeList();
        } else {
            throw tokens.at(0);
        }

        if (tokenType() == FACTS) {
            match(FACTS);
            match(COLON);
            factList();
        } else {
            throw tokens.at(0);
        }

        if (tokenType() == RULES) {
            match(RULES);
            match(COLON);
            ruleList();
        } else {
            throw tokens.at(0);
        }

        if (tokenType() == QUERIES) {
            match(QUERIES);
            match(COLON);
            query();
            queryList();
        } else {
            throw tokens.at(0);
        }

        if (tokenType() == EOFILE) {
        // don't do anything for now
        } else {
            throw tokens.at(0);
        }
    }

    void schemeList() {
        if (tokenType() == ID) {
            scheme();
            schemeList();
        } else {
            // lambda
        }
    }

    void factList() {
        if (tokenType() == ID) {
            fact();
            factList();
        } else {
            // lambda
        }
    }

    void ruleList() {
        if (tokenType() == ID) {
            rule();
            ruleList();
        } else {
            // lambda
        }
    }
    
    void queryList() {
        if (tokenType() == ID) {
            query();
            queryList();
        } else {
            // lambda
        }
    }

    void scheme() {
        if (tokenType() == ID) {
            Predicate pred(tokens.at(0).getValue());

            match(ID);
            match(LEFT_PAREN);

            Parameter param(tokens.at(0).getValue());
            pred.addParameter(param);

            match(ID);
            idList(pred);
            match(RIGHT_PAREN);

            datalog.addScheme(pred);

        } else {
            throw tokens.at(0);
        }
    }

    void fact() {
        if (tokenType() == ID) {
            Predicate pred(tokens.at(0).getValue());
            match(ID);
            match(LEFT_PAREN);

            Parameter param(tokens.at(0).getValue());
            pred.addParameter(param);
            datalog.addDomain(param);

            match(STRING);
            stringList(pred);
            match(RIGHT_PAREN);
            match(PERIOD);

            datalog.addFact(pred);
        } else {
            throw tokens.at(0);
        }
    }

    void rule() {
        if (tokenType() == ID) {
            Rule rule;
            headPredicate(rule);

            match(COLON_DASH);

            Predicate pred;
            predicateBody(rule);
            predicateList(rule);

            match(PERIOD);

            datalog.addRule(rule);
        } else {
            throw tokens.at(0);
        }
    }

    void query() {
        if (tokenType() == ID) {
            predicate();
            match(Q_MARK);
        } else {
            throw tokens.at(0);
        }
    }

    void headPredicate(Rule &rule) {
        if (tokenType() == ID) {
            Predicate pred(tokens.at(0).getValue());

            match(ID);
            match(LEFT_PAREN);

            Parameter param(tokens.at(0).getValue());
            pred.addParameter(param);

            parameter();
            parameterList(pred);

            match(RIGHT_PAREN);

            rule.setHead(pred);
        } else {
            throw tokens.at(0);
        }
    }

    void predicate() {
        if (tokenType() == ID) {
            Predicate pred(tokens.at(0).getValue());

            match(ID);
            match(LEFT_PAREN);

            Parameter param(tokens.at(0).getValue());
            pred.addParameter(param);

            parameter();
            parameterList(pred);
            match(RIGHT_PAREN);

            datalog.addQuery(pred);
        } else {
            throw tokens.at(0);
        }
    }

    void predicateBody(Rule &rule) {
        if (tokenType() == ID) {
            Predicate pred(tokens.at(0).getValue());

			match(ID);
			match(LEFT_PAREN);

			Parameter param(tokens.at(0).getValue());
			pred.addParameter(param);

			parameter();
			parameterList(pred);
			match(RIGHT_PAREN);
            
			rule.addBodyPreds(pred);
        } else {
            throw tokens.at(0);
        }
    }

    void predicateList(Rule &rule) {
        if (tokenType() == COMMA) {
            match(COMMA);
            predicateBody(rule);
            predicateList(rule);
        } else {
            // lambda
        }
    }

    void stringList(Predicate &predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);

            Parameter parameter(tokens.at(0).getValue());
            predicate.addParameter(parameter);
            datalog.addDomain(parameter);

            match(STRING);
            stringList(predicate);
        } else {
            // lambda
        }
    }

    void idList(Predicate &predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);

            Parameter parameter(tokens.at(0).getValue());
            predicate.addParameter(parameter);

            match(ID);
            idList(predicate);
        } else {
            // lambda
        }
    }

    void parameter() {
        if (tokenType() == STRING) {
            match(STRING);
        } else if (tokenType() == ID ) {
            match(ID);
        } else {
            throw tokens.at(0);
        }
    }

    void parameterList(Predicate &predicate) {
        if (tokenType() == COMMA) {
            match(COMMA);

            Parameter param(tokens.at(0).getValue());
            predicate.addParameter(param);

            parameter();
            parameterList(predicate);
        } else {
            // lambda
        }
    }
};