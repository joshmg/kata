#include "h/coin.h"

#include <iostream>
using namespace std;

int main() {
    coin* nickel = coin::parse("nickel");
    coin* quarter = coin::parse("quarter");

    if (nickel != 0 && quarter != 0) {
        cout << "Coin Name: " << nickel->get_name() << " ("<< nickel->get_value() << ")" << endl;
        cout << "Coin Name: " << quarter->get_name() << " ("<< quarter->get_value() << ")" << endl;
        cout << "Total Value: " << *quarter + *nickel << endl;
        cout << "Total Value + 21: " << *quarter + *nickel  + 21 << endl;
    }
    else {
        cout << "Invalid coinage." << endl;
    }

    delete nickel;
    delete quarter;

    return 0;
}
