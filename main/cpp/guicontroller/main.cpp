#include "GuiController.h"

int main(int, char* [])
{
        Q_INIT_RESOURCE(controllerqml);

        boost::property_tree::ptree ptree; // TODO
        stride::GuiController       v(ptree);
}
