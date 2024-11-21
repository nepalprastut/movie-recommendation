#include "register.h"
#include "ui_register.h"

Register::Register(Database *dbObj, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register),
    db(dbObj)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}



void Register::on_signupBtn_clicked()
{
    QString email = ui->lineEdit_email->text();
    QString username = ui->lineEdit_crusername->text();
    QString password = ui->lineEdit_crpassword->text();

    // sql query to insert data
    QSqlQuery query;

    query.prepare("INSERT INTO Users (email, username, password)"
                  "VALUES (:email, :username, :password)");

    query.bindValue(":email", email);
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if(query.exec()) {
        QMessageBox::information(this, "Success", "New User Created.");
    }
    else {
        QMessageBox::information(this, "Failed", "Failed to create user.");
    }
    close();

}

