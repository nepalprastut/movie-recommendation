#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include "src/Recommender/recommender.h"
#include "user_session.h"
#include "src/Login/login.h"
#include "src/Watchlist/watchlistWindow.h"
#include "ui_home.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <src/Watchlist/watchlist.h>


using namespace std;

class Home : public QMainWindow {
    Q_OBJECT

public:
    explicit Home(const QString &username, QWidget *parent = nullptr);
    void setUserId(int userId);
    int getUserId();


signals:
    void logout();
    void openWatchList();

private slots:
    void onLogoutClicked();
    void onWatchListClicked();
    void onGetRecommendationsClicked();
    void onWatchlistButtonClicked();

private:
    QString currentUser;
    QLabel *userLabel;
    QLineEdit *searchBar;
    QPushButton *logoutButton;
    QPushButton *watchlistButton;
    QPushButton *getRecommendationsButton;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *navBarLayout;
    Database *db;
    Login *loginWindow;
    Recommendation recommender;
    int userId;
    void fetchAndDisplayMoviePoster(const QString& movieName, int x, int y);  // Method to fetch and display movie posters
};

#endif // HOME_H
