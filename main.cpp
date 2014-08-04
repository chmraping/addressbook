#include <QApplication>
#include "adressbook.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdressBook w;
    w.show();
    return a.exec();
}
