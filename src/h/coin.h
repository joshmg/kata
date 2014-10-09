#ifndef COIN_H
#define COIN_H

#include <string>

class coin {
    private:
        enum coin_type {
            INVALID, PENNY, NICKEL, DIME, QUARTER, HALF_DOLLAR, DOLLAR
        };
        static unsigned int _compute_value(const coin_type);

        coin(const coin_type type);

        coin_type _type;
        const unsigned int _value;

    public:
        // Description: Creates a coin object.
        // Params:      coin_name -- The name of a US coin. Case insensitive.
        // Note:        If the name is invalid, an INVALID coin is created; worth zero cents.
        static coin* parse(const std::string& coin_name);

        // Description: Return the value of the coin in cents.
        unsigned int get_value() const;

        // Description: Return the name of the coin.
        std::string get_name() const;

        // Math functions...
        unsigned int operator+(const unsigned int amt) const;
        unsigned int operator+(const coin& c) const;
        operator unsigned int() const;
};

#endif
