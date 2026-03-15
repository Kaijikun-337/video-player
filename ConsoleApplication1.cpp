#include <iostream>
#include <windows.h>
#include <string>


#include "Header.h"

using namespace std;

int main() {
    string tmp1;
    cout << "Welcome to our application, what would you like to launch now?" << endl;
    cout << "Video and photo viewer (input player)" << endl;
    
    try {
        cin >> tmp1;

        if (tmp1 == "player") {
            Navigator tmp;
        }
    }

    catch (string err) {
        cout << "Please try again, your input is invalid: " << err << endl;
    }

    cout << "Thanks for launching!" << endl;

    return 0;
}
