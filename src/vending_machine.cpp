#include "h/vending_machine.h"

#include "h/coin.h"
#include "h/vending_item.h"
#include "h/util.h"

#include <string>
#include <vector>
#include <map>
#include <list>

using namespace std;


vending_machine::coin_type vending_machine::_coin_to_type(const coin& c) {
    if (to_upper_case(c.get_name()) == "PENNY") {
        return PENNY;
    }
    else if (to_upper_case(c.get_name()) == "NICKEL") {
        return NICKEL;
    }
    else if (to_upper_case(c.get_name()) == "DIME") {
        return DIME;
    }
    else if (to_upper_case(c.get_name()) == "QUARTER") {
        return QUARTER;
    }
    else {
        return INVALID_COIN;
    }
}
string vending_machine::_item_type_to_string(const item_type type) {
    switch (type) {
        case COLA:  return "Cola";
        case CHIPS: return "Chips";
        case CANDY: return "Candy";
    }
}
vending_machine::item_type vending_machine::_string_to_item_type(const string& str) {
    if (to_upper_case(str) == "COLA") {
        return COLA;
    }
    else if (to_upper_case(str) == "CHIPS") {
        return CHIPS;
    }
    else if (to_upper_case(str) == "CANDY") {
        return CANDY;
    }
    else {
        return INVALID_ITEM;
    }
}

unsigned int vending_machine::_count_change_in_payment_storage() const {
    unsigned int sum = 0;
    for (list<coin*>::const_iterator it = _payment_storage.begin(); it != _payment_storage.end(); it++) {
        sum += *(*it);
    }
    return sum;
}

void vending_machine::_dispense_coins(const unsigned int cents) {
    unsigned int remaining_cents = cents;

    unsigned int quarter_count = remaining_cents / 25;
    remaining_cents -= quarter_count * 25;

    unsigned int dime_count = remaining_cents / 10;
    remaining_cents -= dime_count * 10;

    unsigned int nickel_count = remaining_cents / 5;
    remaining_cents -= nickel_count * 5;

    unsigned int penny_count = remaining_cents;
    remaining_cents -= penny_count;

    while (quarter_count > 0) {
        if (_coin_inventory[QUARTER].size() == 0) break;
        this->change_bin.push_back(_coin_inventory[QUARTER].back());
        _coin_inventory[QUARTER].pop_back();
        --quarter_count;
    }
    // If we ran out of quarters, supplement with 2 dimes and 1 nickel.
    dime_count += quarter_count * 2;
    nickel_count += quarter_count;

    while (dime_count > 0) {
        if (_coin_inventory[DIME].size() == 0) break;
        this->change_bin.push_back(_coin_inventory[DIME].back());
        _coin_inventory[DIME].pop_back();
        --dime_count;
    }
    // If we run out of dimes, supplement with 2 nickels.
    nickel_count += dime_count * 2;


    while (nickel_count > 0) {
        if (_coin_inventory[NICKEL].size() == 0) break;
        this->change_bin.push_back(_coin_inventory[NICKEL].back());
        _coin_inventory[NICKEL].pop_back();
        --nickel_count;
    }
    // If we run out of nickels, supplement with 5 pennies.
    penny_count += nickel_count * 5;

    while (penny_count > 0) {
        if (_coin_inventory[PENNY].size() == 0) break;
        this->change_bin.push_back(_coin_inventory[PENNY].back());
        _coin_inventory[PENNY].pop_back();
        --penny_count;
    }
}

vending_machine::vending_machine() {
    _display_last_item_selected_price = false;
    _display_out_of_stock = false;
    _last_item_type_selected = 0;

    // Fund the machine
    for (unsigned int i=0; i<vending_machine::INITIAL_COINAGE_COUNT; ++i) {
        _coin_inventory[vending_machine::PENNY].push_back(coin::parse("PENNY"));
        _coin_inventory[vending_machine::NICKEL].push_back(coin::parse("NICKEL"));
        _coin_inventory[vending_machine::DIME].push_back(coin::parse("DIME"));
        _coin_inventory[vending_machine::QUARTER].push_back(coin::parse("QUARTER"));
    }

    // Stock the machine
    for (unsigned int i=0; i<vending_machine::INITIAL_COINAGE_COUNT; ++i) {
        _item_inventory[vending_machine::COLA].push_back(new vending_item());
        _item_inventory[vending_machine::CHIPS].push_back(new vending_item());
        _item_inventory[vending_machine::CANDY].push_back(new vending_item());
    }

    // Set the item prices
    _item_prices[vending_machine::COLA] = 100;
    _item_prices[vending_machine::CHIPS] = 50;
    _item_prices[vending_machine::CANDY] = 65;
}

string vending_machine::get_display() {
    if (_display_last_item_selected_price && _last_item_type_selected != 0) {
        _display_last_item_selected_price = false;

        return "PRICE: "+ to_string(_item_prices.find(*_last_item_type_selected)->second);
    }
    if (_display_out_of_stock && _last_item_type_selected != 0) {
        _display_out_of_stock = false;

        return "OUT OF STOCK: "+ _item_type_to_string(_item_prices.find(*_last_item_type_selected)->first);
    }
    else if (_payment_storage.size() > 0) {
        return "DEPOSITED: "+ to_string(_count_change_in_payment_storage());
    }
    else {
        return "INSERT COIN";
    }
}

map<string, unsigned int> vending_machine::get_item_inventory() const {
    map<string, unsigned int> item_inventory;

    // _item_inventory must have COLA, CHIPS, and CANDY defined
    item_inventory[vending_machine::_item_type_to_string(vending_machine::COLA)] = (_item_inventory.find(vending_machine::COLA)->second).size();
    item_inventory[vending_machine::_item_type_to_string(vending_machine::CHIPS)] = (_item_inventory.find(vending_machine::CHIPS)->second).size();
    item_inventory[vending_machine::_item_type_to_string(vending_machine::CANDY)] = (_item_inventory.find(vending_machine::CANDY)->second).size();

    return item_inventory;
}

unsigned int vending_machine::get_item_price(const std::string& item) const {
    item_type type = _string_to_item_type(item);
    switch (type) {
        case COLA: return _item_prices.find(COLA)->second;
        case CHIPS: return _item_prices.find(CHIPS)->second;
        case CANDY: return _item_prices.find(CANDY)->second;
        default: {
            return 0;
        }
    }
}

void vending_machine::insert_coin(coin*& c) {
    if (c == 0) return;

    coin_type type = vending_machine::_coin_to_type(*c);

    if (type == INVALID_COIN || type == PENNY) {
        this->change_bin.push_back(c);
    }
    else {
        _payment_storage.push_back(c);
    }

    c = 0;
}

void vending_machine::return_coins() {
    while (! _payment_storage.empty()) {
        this->change_bin.push_back(_payment_storage.front());
        _payment_storage.pop_front();
    }
}

void vending_machine::select_item(const std::string& item) {
    unsigned int payment_amount = _count_change_in_payment_storage();

    item_type type = _string_to_item_type(item);
    if (type == INVALID_ITEM) return;

    _last_item_type_selected = new item_type(type);

    if (_item_inventory[type].size() > 0) {
        if (_item_prices.find(type)->second <= payment_amount) {
            // Enough change has been given; dispense item
            this->item_bin.push_back(_item_inventory[type].back());
            _item_inventory[type].pop_back();
            
            // Put the payment_storage into permanent storage
            for (list<coin*>::iterator it = _payment_storage.begin(); it != _payment_storage.end(); it++) {
                coin* c = *it;
                _coin_inventory[_coin_to_type(*c)].push_back(c);
            }
            _payment_storage.clear();

            // Dispense change
            _dispense_coins(payment_amount - _item_prices.find(type)->second);
        }
        else {
            _display_last_item_selected_price = true;
            _display_out_of_stock = false;
        }
    }
    else {
        _display_out_of_stock = true;
        _display_last_item_selected_price = false;
    }
}

vending_machine::~vending_machine() {
    // Destroy the payment_storage
    while (! _payment_storage.empty()) {
        delete _payment_storage.front();
        _payment_storage.pop_front();
    }

    // Destroy the change_bin
    while (! this->change_bin.empty()) {
        delete this->change_bin.front();
        this->change_bin.pop_front();
    }

    // Destroy the coin_inventory
    map< coin_type, vector<coin*> >::iterator coin_inv_it = _coin_inventory.begin();
    while (coin_inv_it != _coin_inventory.end()) {
        vector<coin*>::iterator coin_it = coin_inv_it->second.begin();
        while (coin_it != coin_inv_it->second.end()) {
            delete *coin_it;
            coin_it++;
        }
        coin_inv_it->second.clear();
        coin_inv_it++;
    }
    _coin_inventory.clear();

    // Destroy the item_inventory
    map< item_type, vector<vending_item*> >::iterator item_inv_it = _item_inventory.begin();
    while (item_inv_it != _item_inventory.end()) {
        vector<vending_item*>::iterator item_it = item_inv_it->second.begin();
        while (item_it != item_inv_it->second.end()) {
            delete *item_it;
            item_it++;
        }
        item_inv_it->second.clear();
        item_inv_it++;
    }
    _item_inventory.clear();

    if (_last_item_type_selected != 0) {
        delete _last_item_type_selected;
        _last_item_type_selected = 0;
    }
}

list<coin*> vending_machine::withdraw_change() {
    list<coin*> change;
    for (list<coin*>::iterator it = this->change_bin.begin(); it != this->change_bin.end(); it++) {
        change.push_back(*it);
    }
    this->change_bin.clear();
    return change;
}

void vending_machine::withdraw_change(list<coin*>* wallet) {
    for (list<coin*>::iterator it = this->change_bin.begin(); it != this->change_bin.end(); it++) {
        wallet->push_back(*it);
    }
    this->change_bin.clear();
}
