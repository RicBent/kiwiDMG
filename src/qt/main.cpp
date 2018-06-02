#include <QtWidgets>

#include "core/dmg.h"

int main(int argc, char** argv)
{
    DMG dmg;
    dmg.reset();
    dmg.run();

    return 0;

    /*QApplication app(argc, argv);
    return app.exec();*/
}
