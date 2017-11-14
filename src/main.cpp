#include <QApplication>
#include "mainwindow.h"
#include "string"
#include "math.h"

std::string hairstyle_file;
std::string headmodel_file;
float X_angle = 0.0;
float Y_angle = 0.0;
float Z_angle = 0.0;
std::string save_image;

int main(int argc, char *argv[])
{
    hairstyle_file.append(argv[1]);
    if(argc>2){
        headmodel_file.append(argv[2]);
        X_angle = atof(argv[3])/180*M_PI;
        Y_angle = atof(argv[4])/180*M_PI;
        Z_angle = atof(argv[5])/180*M_PI;
    }
    if(argc>6){
        save_image.append(argv[6]);
    }
    //hairstyle_file.append("./hairfiles/strands00001.data");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
