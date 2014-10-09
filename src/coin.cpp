#include "h/coin.h"

#include <string>
#include "h/util.h"

using namespace std;

coin* coin::parse(const string& str) {
    string coin_type = to_upper_case(str);

    coin* c = 0;

    if (coin_type == "PENNY") {
        c = new coin(PENNY);
    }
    else if (coin_type == "NICKEL") {
        c = new coin(NICKEL);
    }
    else if (coin_type == "DIME") {
        c = new coin(DIME);
    }
    else if (coin_type == "QUARTER") {
        c = new coin(QUARTER);
    }
    else if (coin_type == "HALF_DOLLAR") {
        c = new coin(HALF_DOLLAR);
    }
    else if (coin_type == "DOLLAR") {
        c = new coin(DOLLAR);
    }
    else {
        c = new coin(INVALID);
    }

    return c;
}

unsigned int coin::_compute_value(const coin_type type) {
    switch (type) {
        case PENNY:         return 1;
        case NICKEL:        return 5;
        case DIME:          return 10;
        case QUARTER:       return 25;
        case HALF_DOLLAR:   return 50;
        case DOLLAR:        return 100;
        default: {
            return 0;
        } break;
    }
}

coin::coin() : _value(coin::_compute_value(INVALID)), _type(INVALID) { }
coin::coin(const coin_type type) : _value(coin::_compute_value(type)), _type(type) { }

string coin::get_name() const {
    switch (_type) {
        case PENNY:         return "Penny";
        case NICKEL:        return "Nickel";
        case DIME:          return "Dime";
        case QUARTER:       return "Quarter";
        case HALF_DOLLAR:   return "Half Dollar";
        case DOLLAR:        return "Dollar";
        default: {
            return "";
        } break;
    }
}

unsigned int coin::get_value() const {
    return _value;
}

unsigned int coin::operator+(const unsigned int amt) const {
    return amt + _value;
}
unsigned int coin::operator+(const coin& c) const {
    return c._value + _value;
}
coin::operator unsigned int() const {
    return _value;
}
