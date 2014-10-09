#include "h/util.h"

#include <string>
#include <locale>
#include <cstdio>

using namespace std;

string to_upper_case(const string& str) {
    string return_string;

    locale loc;
    for (unsigned int i=0; i<str.length(); ++i) {
        return_string += toupper(str[i], loc);
    }

    return return_string;
}

#define TO_STRING_BUF_SIZE 256
string to_string(const unsigned int i) {
    char buf[TO_STRING_BUF_SIZE];
    snprintf(buf, TO_STRING_BUF_SIZE, "%u", i);
    return string(buf);
}
