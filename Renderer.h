#pragma once

#include <windows.h>  //for audio

#include <chrono>  //for flexible time delays
#include <fstream>
#include <iostream>
#include <string>
#include <thread>  //for flexible time delays

class Renderer {
   public:
    void clear () { std::cout << "\x1b[H\x1b[J"; }

    void displayASCII ( const unsigned char* pixels, int width, int height, int linesize ) {
        const char symbols[] = "  ..::--==++**%%@@##";  // Light to dark
        const int numSymbols = sizeof ( symbols ) - 1;

        std::string picture;
        picture.reserve ( width * height + height );

        for ( int i = 0; i < height; i++ ) {
            for ( int j = 0; j < width; j++ ) {
                unsigned char pixel = pixels[ i * linesize + j ];
                int idx = ( pixel * numSymbols ) / 255;
                picture += symbols[ idx ];
            }
            picture += '\n';
        }

        std::cout << picture << std::flush;
    }

    void getTerminalSize ( int& width, int& height ) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo ( GetStdHandle ( STD_OUTPUT_HANDLE ), &csbi );
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
};
