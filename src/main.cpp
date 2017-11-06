#include <QApplication>
#include "mainwindow.h"
#include "string"

std::string hairstyle_file;
int main(int argc, char *argv[])
{
    hairstyle_file.append(argv[1]);
    //hairstyle_file.append("./hairfiles/strands00001.data");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
