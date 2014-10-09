#ifndef VENDING_MACHINE_H
#define VENDING_MACHINE_H

#include "coin.h"
#include "vending_item.h"

#include <string>
#include <vector>
#include <map>
#include <list>

class vending_machine {
    private:
        enum coin_type {
            INVALID_COIN, PENNY, NICKEL, DIME, QUARTER
        };
        enum item_type {
            INVALID_ITEM, COLA, CHIPS, CANDY
        };

        static const unsigned int INITIAL_COINAGE_COUNT = 10;
        static const unsigned int INITIAL_ITEM_COUNT = 10;

        static coin_type _coin_to_type(const coin& c);

        static std::string _item_type_to_string(const item_type);
        static item_type _string_to_item_type(const std::string&);

        void _calculate_change(const unsigned int cents, unsigned int* quarters, unsigned int* dimes, unsigned int* nickels, unsigned int* pennies) const;
        bool _can_make_change(const item_type) const;

        unsigned int _count_change_in_payment_storage() const;
        void _dispense_coins(const unsigned int cents);

        std::map< coin_type, std::vector<coin*> > _coin_inventory;
        std::map< item_type, std::vector<vending_item*> > _item_inventory;

        std::map< item_type, unsigned int> _item_prices;

        std::list<coin*> _payment_storage;

        bool _display_last_item_selected_price;
        bool _display_out_of_stock;
        item_type* _last_item_type_selected;

        void _init();

    public:
        vending_machine();
        vending_machine(const unsigned int quarters, const unsigned int dimes, const unsigned int nickels, const unsigned int pennies);
        ~vending_machine();

        std::string get_display();
        std::map<std::string, unsigned int> get_item_inventory() const;
        unsigned int get_item_price(const std::string& item) const;

        void insert_coin(coin*& c);
        void return_coins();

        void select_item(const std::string& item);

        std::list<coin*> withdraw_change();
        void withdraw_change(std::list<coin*>* wallet);

        std::list<vending_item*> withdraw_items();
        void withdraw_items(std::list<vending_item*>* bag);

        std::list<coin*> change_bin;
        std::list<vending_item*> item_bin;
};

#endif
