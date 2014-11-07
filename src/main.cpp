#include "h/util.h"
#include "h/coin.h"
#include "h/vending_machine.h"
#include "h/vending_item.h"

#include <string>
#include <iostream>
#include <map>
#include <list>
using namespace std;

enum coin_type {
    INVALID_COIN, PENNY, NICKEL, DIME, QUARTER
};
enum item_type {
    INVALID_ITEM, COLA, CHIPS, CANDY
};

typedef map< coin_type, vector<coin*> > wallet;

coin_type coin_to_type(const coin& c) {
    if (util::to_upper_case(c.get_name()) == "PENNY") {
        return PENNY;
    }
    else if (util::to_upper_case(c.get_name()) == "NICKEL") {
        return NICKEL;
    }
    else if (util::to_upper_case(c.get_name()) == "DIME") {
        return DIME;
    }
    else if (util::to_upper_case(c.get_name()) == "QUARTER") {
        return QUARTER;
    }
    else {
        return INVALID_COIN;
    }
}
coin_type string_to_coin_type(const string& str) {
    if (util::to_upper_case(str) == "PENNY") {
        return PENNY;
    }
    else if (util::to_upper_case(str) == "NICKEL") {
        return NICKEL;
    }
    else if (util::to_upper_case(str) == "DIME") {
        return DIME;
    }
    else if (util::to_upper_case(str) == "QUARTER") {
        return QUARTER;
    }
    else {
        return INVALID_COIN;
    }
}
item_type string_to_item_type(const string& str) {
    if (util::to_upper_case(str) == "COLA") {
        return COLA;
    }
    else if (util::to_upper_case(str) == "CHIPS") {
        return CHIPS;
    }
    else if (util::to_upper_case(str) == "CANDY") {
        return CANDY;
    }
    else {
        return INVALID_ITEM;
    }
}

unsigned int sum_wallet(const wallet& w) {
    unsigned int sum = 0;
    for (wallet::const_iterator it = w.begin(); it != w.end(); it++) {
        sum += it->second.size() * it->second.front()->get_value();
    }
    return sum;
}

void print_wallet(const wallet& w) {
    unsigned int q = w.find(QUARTER)->second.size();
    unsigned int d = w.find(DIME)->second.size();
    unsigned int n = w.find(NICKEL)->second.size();
    unsigned int p = w.find(PENNY)->second.size();

    cout << "TOTAL: " << q * 25 + d * 10 + n * 5 + p << " [";
    cout << "Q: " << q << " ";
    cout << "D: " << d << " ";
    cout << "N: " << n << " ";
    cout << "P: " << p << "]";
    cout << endl;
}
void print_wallet(const list<coin*>& w) {
    unsigned int q = 0;
    unsigned int d = 0;
    unsigned int n = 0;
    unsigned int p = 0;

    for (list<coin*>::const_iterator it = w.begin(); it != w.end(); it++) {
        switch ((*it)->get_value()) {
            case 1:     p++; break;
            case 5:     n++; break;
            case 10:    d++; break;
            case 25:    q++; break;
        }
    }

    cout << "TOTAL: " << q * 25 + d * 10 + n * 5 + p << " [";
    cout << "Q: " << q << " ";
    cout << "D: " << d << " ";
    cout << "N: " << n << " ";
    cout << "P: " << p << "]";
    cout << endl;
}

map< coin_type, vector<coin*> > WALLET;
vending_machine VENDING_MACHINE;
list<vending_item*> BAG;

void print_help() {
    cout << "Commands: " << endl;
    cout << "  " << "check_wallet" << endl;
    cout << "  " << "inspect_machine" << endl;
    cout << "  " << "check_display" << endl;
    cout << "  " << "select_item" << endl;
    cout << "  " << "insert_coin" << endl;
    cout << "  " << "return_coins" << endl;
    cout << "  " << "collect_change" << endl;
    cout << "  " << "collect_item" << endl;
    cout << endl;
}

int main() {
    cout << "Filling your wallet." << endl;
    for (unsigned int i=0; i<10; ++i) {
        WALLET[PENNY].push_back(coin::parse("penny"));
        WALLET[NICKEL].push_back(coin::parse("nickel"));
        WALLET[DIME].push_back(coin::parse("dime"));
        WALLET[QUARTER].push_back(coin::parse("quarter"));
    }

    cout << "You arrive at a vending machine." << endl;
    print_help();

    string line;
    while (getline(cin, line)) {
        if (line == "check_wallet") {
            cout << "You reach in your pocket and inspect your wallet." << endl;
            print_wallet(WALLET);
        }
        else if (line == "inspect_machine") {
            cout << "You look inside the vending machine. Behind the glass you see:" << endl;
            map<string, unsigned int> machine_inventory = VENDING_MACHINE.get_item_inventory();
            for (map<string, unsigned int>::iterator it = machine_inventory.begin(); it != machine_inventory.end(); it++) {
                cout << "  " << it->second << " " << it->first << " for " << VENDING_MACHINE.get_item_price(it->first) << " cents each" << endl;
            }
        }
        else if (line == "check_display") {
            cout << "You check the display. It says:" << endl;
            cout << "  " << VENDING_MACHINE.get_display() << endl;
        }
        else if (line == "select_item") {
            cout << "Which item would you like to select from the dial pad?" << endl;
            
            string item_name;
            getline(cin, item_name);
            item_type type = string_to_item_type(item_name);
            if (type == INVALID_ITEM) {
                cout << "Invalid item." << endl;
            }
            else {
                unsigned int item_bin_size = VENDING_MACHINE.item_bin.size();
                cout << "You select the item." << endl;
                VENDING_MACHINE.select_item(item_name);
                if (item_bin_size != VENDING_MACHINE.item_bin.size()) {
                    cout << "You hear an item fall from inside the machine." << endl;
                }
            }
        }
        else if (line == "insert_coin") {
            cout << "You reach inside your wallet. Which coin to you grab?" << endl;
            string coin_name;
            getline(cin, coin_name);
            coin_type type = string_to_coin_type(coin_name);
            if (type == INVALID_COIN) {
                cout << "Invalid coin." << endl;
            }
            else {
                if (WALLET[type].size() > 0) {
                    VENDING_MACHINE.insert_coin(WALLET[type].back());
                    WALLET[type].pop_back();
                    cout << "You insert a coin." << endl;
                }
                else {
                    cout << "You do not have any of those." << endl;
                }
            }
        }
        else if (line == "return_coins") {
            unsigned int change_bin_size = VENDING_MACHINE.change_bin.size();
            cout << "You press the \"Return Change\" button." << endl;
            VENDING_MACHINE.return_coins();
            if (change_bin_size != VENDING_MACHINE.change_bin.size()) {
                cout << "You hear change drop into the bin." << endl;
            }
        }
        else if (line == "collect_change") {
            cout << "You collected " << VENDING_MACHINE.change_bin.size() << " coins from the change bin." << endl;
            list<coin*> temp_wallet;
            VENDING_MACHINE.withdraw_change(&temp_wallet);
            print_wallet(temp_wallet);

            for (list<coin*>::iterator it = temp_wallet.begin(); it != temp_wallet.end(); it++) {
                WALLET[coin_to_type(*(*it))].push_back(*it);
            }
        }
        else if (line == "collect_item") {
            cout << "You collected " << VENDING_MACHINE.item_bin.size() << " items from the bin." << endl;

            list<vending_item*> temp_bag;
            VENDING_MACHINE.withdraw_items(&temp_bag);

            for (list<vending_item*>::iterator it = temp_bag.begin(); it != temp_bag.end(); it++) {
                BAG.push_back(*it);
            }
        }
        else {
            cout << "You stand there mumbling jibberish." << endl;
        }
    }

    return 0;
}
