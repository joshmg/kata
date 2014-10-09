#include "h/coin.h"
#include "h/vending_machine.h"

#include <iostream>
#include <map>
#include <list>
using namespace std;

int main() {
    coin* nickel = coin::parse("nickel");
    coin* quarter = coin::parse("quarter");

    /*
        if (nickel != 0 && quarter != 0) {
            cout << "Coin Name: " << nickel->get_name() << " ("<< nickel->get_value() << ")" << endl;
            cout << "Coin Name: " << quarter->get_name() << " ("<< quarter->get_value() << ")" << endl;
            cout << "Total Value: " << *quarter + *nickel << endl;
            cout << "Total Value + 21: " << *quarter + *nickel  + 21 << endl;
        }
        else {
            cout << "Invalid coinage." << endl;
        }
    */

    vending_machine v;

    cout << "Available Items: " << endl;
    map<string, unsigned int> inventory = v.get_item_inventory();
    for (map<string, unsigned int>::iterator it = inventory.begin(); it != inventory.end(); it++) {
        cout << it->first << ":\t" << it->second << " (" << v.get_item_price(it->first) << " cents each)" << endl;
    }

    cout << "Display: " << v.get_display() << endl;

    cout << "Inserting a nickel." << endl;
    v.insert_coin(nickel);
    v.insert_coin(nickel); // Shouldn't do anything...
    cout << "Display: " << v.get_display() << endl;
    cout << "Returning coins." << endl;
    v.return_coins();

    cout << "Display: " << v.get_display() << endl;

    cout << "Coins in change bin: " << endl;
    for (list<coin*>::iterator it = v.change_bin.begin(); it != v.change_bin.end(); it++) {
        cout << (*it)->get_name() << " ";
    }
    cout << endl;

    cout << "Display: " << v.get_display() << endl;

    list<coin*> wallet;
    for (unsigned int i=0;i<15; ++i) {
        // wallet.push_back(coin::parse("nickel"));
        wallet.push_back(coin::parse("quarter"));
        // wallet.push_back(coin::parse("dime"));
        // wallet.push_back(coin::parse("penny"));
    }
    v.withdraw_change(&wallet);

    cout << "Selecting cola." << endl;
    v.select_item("cola");

    cout << "Display: " << v.get_display() << endl;

    while (! wallet.empty()) {
        v.insert_coin(wallet.back());
        wallet.pop_back();
    }

    cout << "Display: " << v.get_display() << endl;

    cout << "Selecting Cola" << endl;
    v.select_item("cola");

    cout << "Display: " << v.get_display() << endl;

    cout << "Coins in change bin: " << endl;
    for (list<coin*>::iterator it = v.change_bin.begin(); it != v.change_bin.end(); it++) {
        cout << (*it)->get_name() << " ";
    }
    cout << endl;

    cout << "Items count in bin: " << v.item_bin.size() << endl;

    cout << "Display: " << v.get_display() << endl;

    // delete nickel;
    delete quarter;

    return 0;
}
