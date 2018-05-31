#include <QtWidgets>

#include "core/test.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMessageBox::information(nullptr, "kiwiDMG", core::test());

    QMainWindow window;
    window.setWindowTitle("kiwiDMG");
    window.setCentralWidget(new QLabel("Hello, World!"));
    window.show();

    return app.exec();
}
