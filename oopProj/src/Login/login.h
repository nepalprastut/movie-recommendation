#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "ui_login.h"
#include "src/Database/database.h"
#include "QMessageBox"
#include "user_session.h"


namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(Database *db, QWidget *parent = nullptr);
    ~Login();


private slots:
    void on_loginBtn_clicked();

private:
    Ui::Login *ui;
    Database *db;
};

#endif // LOGIN_H

