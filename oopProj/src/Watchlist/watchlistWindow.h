#ifndef WATCHLISTWINDOW_H
#define WATCHLISTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMainWindow>
#include <src/Login/login.h>
#include <src/Home/home.h>
#include <user_session.h>
#include <QTableWidget>
#include <QHeaderView>
#include <src/Watchlist/watchlist.h>

class WatchlistWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WatchlistWindow(Database *dbObj, QWidget *parent);

private slots:
    void goToHome();
    void logout();
    void on_removeButton_clicked();
private:

    QVBoxLayout *mainLayout;
    Login *loginWindow;
    QLabel *userLabel;
    QWidget *watchlistArea;
    QString currentUser;
    Database *db;
    QTableWidget *watchlistTable;
    // Home *hWindow;
    void setupUI();
    void loadWatchlist();
};

#endif // WATCHLISTWINDOW_H
