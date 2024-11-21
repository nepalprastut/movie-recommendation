#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QPixmap>
#include <src/Login/login.h>
#include <src/Register/register.h>
#include <user_session.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUserId(int id);
    int getUserId();

private slots:
    void on_check_clicked();
    void readData();
    void finishReading();
    void setUserImage();
    void sendRequest(int);
    void triggerNextRequest();
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onWatchlistButtonClicked();
    // void on_addToWatchlistButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;
    QByteArray dataBuffer;
    QPixmap *img;
    Login *loginWindow;
    Register *registerWindow;
    Database *db;
    int userId;
    QSqlDatabase database;
    QString currentUser;
    // QByteArray movieData;

};
#endif // MAINWINDOW_H
































