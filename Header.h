#include <iostream>
#include <string>
#include <fstream>
#include <windows.h> //for audio
#include <thread> //for flexible time delays
#include <chrono> //for flexible time delays

#pragma comment(lib, "Winmm.lib") //for audio

using namespace std;

const int WIDTH = 474;
const int HEIGHT = 126;
const int FRAME_COUNT = 131;

ofstream Log("Log.txt");

class Player {
protected:
    void clear() {
        cout << "\x1b[H";
    }

    void displayASCII(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Could not open the file!" << endl;
            return;
        }

        unsigned char pixel;
        const char symbols[] = " .:-=+*%@#"; // Light to dark
        const int numSymbols = sizeof(symbols) - 1;

        string picture;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                file.read((char*)&pixel, 1);
                int idx = (pixel * numSymbols) / 255;
                picture += symbols[idx];
            }
            picture += '\n';
        }
        cout << picture << flush;

        file.close();
    }

    void displayImage(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Could not open the file!" << endl;
            return;
        }

        Log << "The image was displayed" << endl;

        unsigned char pixel;
        const char symbols[] = " .:-=+*%@#"; // Light to dark
        const int numSymbols = sizeof(symbols) - 1;

        string picture;

        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                file.read((char*)&pixel, 1);
                int idx = (pixel * numSymbols) / 256;
                picture += symbols[idx];
            }
            picture += '\n';
        }
        cout << picture << flush;

        file.close();
    }

public:
    void PlayVideo() {
        PlaySound(TEXT("track.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOWAIT | SND_NOSTOP);

        for (int frame = 0; frame < FRAME_COUNT; ++frame) {
            clear();
            string filename = "ezgif-frame" + to_string(frame) + ".raw";
            displayASCII(filename);
            this_thread::sleep_for(chrono::milliseconds(73));  // Delay between frames
        }
        Log << "The video was played" << endl;
        Log.close();
    }
};

class ImageViewer :public Player {
public:
    void imgOpener() {
        displayImage("apple.raw");
    }
};

class Navigator {
private:
    Player vid1;
    ImageViewer img1;
public:
    Navigator() {
        int tmp;
        char answer;

        for (;;) {
            cout << "Welcome to Image and Video player. Please choose one of these options:\n";
            cout << "1. Photo Viewer (input 1)" << endl;
            cout << "2. Video player (input 2)" << endl;
            cin >> tmp;

            if (tmp == 1) {
                cout << "The Apple's logo" << endl;
                img1.imgOpener();
                cout << "Would you like to watch a video? Y or N" << endl;
                cout << "Zoom out 10 times" << endl;
                cin >> answer;
                if (answer == 'Y') {
                    system("cls");
                    vid1.PlayVideo();
                    break;
                }
            }
            else {
                cout << "Zoom out 10 times" << endl;
                cout << "if you zoomed out 10 times, input Y" << endl;
                cin >> answer;
                if (answer == 'Y') {
                    vid1.PlayVideo();
                }
                cout << "Would you like to open a photo? Y or N" << endl;
                cin >> answer;
                if (answer == 'Y') {
                    img1.imgOpener();
                    break;
                }
            }
        }
    }

};
