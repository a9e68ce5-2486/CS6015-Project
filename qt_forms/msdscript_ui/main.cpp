#include <QApplication>
#include "msdWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MsdWidget widget;
    widget.show();
    return app.exec();
}
