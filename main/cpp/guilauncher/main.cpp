#include "GuiLauncher.h"

int main(int, char* [])
{
        bool crashed = true;
        while (crashed) {
                crashed = false;
                try {
                        GuiLauncher v;
                        v.StartSimulation();
                } catch (...) {
                        crashed = true;
                }
        }
}
