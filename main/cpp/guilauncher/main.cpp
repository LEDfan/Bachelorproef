#include "GuiLauncher.h"

int main(int, char* [])
{
        bool crashed = true;
        while (crashed) {
                crashed = false;
                try {
                        GuiLauncher v;
                        v.Start();
                } catch (...) {
                        crashed = true;
                }
        }
}
