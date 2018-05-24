#include "GuiLauncher.h"

int main(int, char* [])
{
        int crashed = 0;
        while (crashed >= 0 && crashed < 5) {
                try {
                        GuiLauncher::Start();
                        crashed = -1;
                } catch (...) {
                        crashed++;
                }
        }
}
