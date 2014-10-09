#include "h/util.h"

#include <string>
#include <locale>

using namespace std;

string to_upper_case(const string& str) {
    string return_string;

    locale loc;
    for (unsigned int i=0; i<str.length(); ++i) {
        return_string += toupper(str[i], loc);
    }

    return return_string;
}

