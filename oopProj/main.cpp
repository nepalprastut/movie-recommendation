#include <QApplication>
#include "mainwindow.h"
#include <src/Database/database.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Database db("localhost", "movieRecommendation", "admin", "Password@123");     // instantiating Database class to establish connection
    db.connect();
    return a.exec();
}



