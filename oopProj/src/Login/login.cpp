#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "src/Home/home.h"

Login::Login(Database *dbObj, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login),
    db(dbObj)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}




void Login::on_loginBtn_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            int userId = query.value("user_id").toInt(); // Assuming the userId is stored in the column

            UserSession::currentUser = username; // new code
            UserSession::userId = userId;
            // Find MainWindow instance and set userId
            MainWindow *mainWindow = qobject_cast<MainWindow *>(parentWidget());
            if (mainWindow) {
                mainWindow->setUserId(userId);
                // Set the user ID in MainWindow
            }

            QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
            // UserSession::currentUser = username;
            // Creating an object for home class
            Home *homeWindow1 = new Home(username);
            if(homeWindow1) {
                homeWindow1->setUserId(userId);
                // qDebug() << "User ID set to: " << userId;
                homeWindow1->show();
            }
            close();  // Close the login window after successful login
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        }
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}
