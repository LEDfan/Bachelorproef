#include "GuiController.h"

int main(int, char* [])
{
        boost::property_tree::ptree ptree; // TODO
        stride::GuiController       v(ptree);
        v.Join();
}
