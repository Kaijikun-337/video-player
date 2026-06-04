#include <iostream>

#include "Player.h"

int main () {
    Player player;
    std::string fileName;

    std::cout << "Please enter the file name of your video\n";
    std::cin >> fileName;

    auto result = player.openVideo ( fileName );
    if ( !result ) {
        std::cerr << result.error () << '\n';
        return 1;
    }

    auto decoder = player.setupDecoder ();
    if ( !decoder ) {
        std::cerr << decoder.error () << '\n';
        return 1;
    }

    auto sizing = player.setupSizing ();
    if ( !sizing ) {
        std::cerr << sizing.error () << '\n';
        return 1;
    }

    player.PlayVideo ();
    return 0;
}