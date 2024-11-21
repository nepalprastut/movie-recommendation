#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include "ui_register.h"
#include "src/Database/database.h"
#include "QMessageBox"


namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

public:
    explicit Register(Database *db, QWidget *parent = nullptr);
    ~Register();


private slots:


    void on_signupBtn_clicked();

private:
    Ui::Register *ui;
    Database *db;
};

#endif // REGISTER_H
