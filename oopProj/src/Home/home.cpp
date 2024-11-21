#include "home.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QMessageBox>
#include <QDebug>
#include <user_session.h>
#include <ui_home.h>
#include <QLabel>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>

Home::Home(const QString &username, QWidget *parent)
    : QMainWindow(parent) {
    UserSession::currentUser = username;
    this->setWindowTitle("Home - Movie Recommendation System");
    setFixedSize(1920, 1080);

    // Set background image for the window
    QPalette palette;
    palette.setBrush(this->backgroundRole(),
                     QBrush(QPixmap(":/images/dbg.jpg").scaled(this->size(),
                                                               Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // Navbar Setup
    QWidget *navbarWidget = new QWidget(this);
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbarWidget);
    QLabel *popularMoviesLabel = new QLabel("Recommended Movies", navbarWidget);
    popularMoviesLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    searchBar = new QLineEdit(navbarWidget);
    searchBar->setPlaceholderText("Movies similar to...");
    searchBar->setStyleSheet("padding: 5px; border-radius: 15px; border: 1px solid #ccc; color: #000000; background-color: #f0f0f0;");
    searchBar->setMinimumWidth(300);
    getRecommendationsButton = new QPushButton("Get Recommendations", navbarWidget);
    getRecommendationsButton->setStyleSheet("QPushButton {"
                                            "background-color: #32CD32;" // Initial green color
                                            "color: black;"
                                            "border-radius: 5px;"
                                            "}"
                                            "QPushButton:hover {"
                                            "background-color: #006400;" // Darker green on hover
                                            "cursor: pointer;"
                                            "}");
    watchlistButton = new QPushButton("Watchlist", navbarWidget);
    logoutButton = new QPushButton("Logout", navbarWidget);
    userLabel = new QLabel(this);
    userLabel->setText("Welcome, " + UserSession::currentUser + "!");
    userLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    navbarLayout->addWidget(userLabel);
    navbarLayout->addWidget(popularMoviesLabel);
    navbarLayout->addStretch(1);
    navbarLayout->addWidget(searchBar);
    navbarLayout->addWidget(getRecommendationsButton);
    navbarLayout->addStretch(1);
    navbarLayout->addWidget(watchlistButton);
    navbarLayout->addWidget(logoutButton);
    navbarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    navbarWidget->setStyleSheet("background-color: #2E3A47; padding: 10px;");
    setMenuWidget(navbarWidget);

    connect(logoutButton, &QPushButton::clicked, this, &Home::onLogoutClicked);
    connect(watchlistButton, &QPushButton::clicked, this, &Home::onWatchListClicked);
    connect(getRecommendationsButton, &QPushButton::clicked, this, &Home::onGetRecommendationsClicked);

    // Main Layout Setup
    mainLayout = new QVBoxLayout();
    centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    mainLayout->addStretch();
    setCentralWidget(centralWidget);

    // Initialize Recommender
    recommender.vectorizer("/home/prastut/oopProj/src/Recommender/final_updated_project_ready_dataset.csv");
}

void Home::onGetRecommendationsClicked() {
    if (UserSession::currentUser.isEmpty()) {
        QMessageBox::warning(this, "Access Denied", "You must log in.");
        loginWindow->show();
        return;
    }

    QString movieTitle = searchBar->text();

    if (movieTitle.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a movie name in the search bar.");
        return;
    }

    QVector<QPair<float, QString>> recommendations = recommender.recommendMovies(movieTitle);

    for (const auto& recommendation : recommendations) {
        float similarityScore = recommendation.first; // Access the similarity score
        QString movieName = recommendation.second;    // Access the movie title
        qDebug() << "Movie:" << movieName << "Similarity Score:" << similarityScore;
    }


    if (recommendations.isEmpty()) {
        QMessageBox::information(this, "No Recommendations", "No similar movies found.");
        return;
    }

    // Clear previous widgets
    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int baseX = 60;
    int baseY = 110;
    int posterWidth = 271;
    int posterHeight = 311;
    int buttonWidth = 100;
    int buttonHeight = 26;

    int xGap = 390;
    int yGap = 400;

    int rowCount = 2;
    int colCount = 5;
    int displayedItems = 0;

    for (int row = 0; row < rowCount && displayedItems < recommendations.size(); ++row) {
        for (int col = 0; col < colCount && displayedItems < recommendations.size(); ++col) {
            QString recommendedMovieTitle = recommendations[displayedItems].second;

            // Fetch movie details from the API
            QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
            QNetworkRequest movieRequest{QUrl(QString("https://api.themoviedb.org/3/search/movie?api_key=bf3ce5fa7c0e31adaa82cb21e0c6886e&query=%1").arg(recommendedMovieTitle))};
            QNetworkReply* reply = netManager->get(movieRequest);

            connect(reply, &QNetworkReply::finished, this, [this, reply, recommendedMovieTitle, baseX, baseY, xGap, yGap, col, row, posterWidth, posterHeight, buttonWidth, buttonHeight]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QJsonObject movieJsonInfo = QJsonDocument::fromJson(reply->readAll()).object();
                    QJsonArray results = movieJsonInfo.value("results").toArray();

                    if (!results.isEmpty()) {
                        QJsonObject movieDetails = results.at(0).toObject();
                        int movieId = movieDetails.value("id").toInt();
                        QString movieTitle = movieDetails.value("title").toString();

                        // Calculate position for poster and button
                        int currentX = baseX + (col * xGap);
                        int currentY = baseY + (row * yGap);

                        // Fetch and display the movie poster
                        fetchAndDisplayMoviePoster(movieTitle, currentX, currentY);

                        // Create the "Add to Watchlist" button
                        QPushButton* watchlistBtn = new QPushButton("Watchlist", this);
                        watchlistBtn->setFixedSize(buttonWidth, buttonHeight);
                        watchlistBtn->move(currentX + 108, currentY + posterHeight + 20);
                        watchlistBtn->setStyleSheet("QPushButton {"
                                                    "background-color: #32CD32;"
                                                    "color: black;"
                                                    "border-radius: 5px;"
                                                    "}"
                                                    "QPushButton:hover {"
                                                    "background-color: #006400;" // darker green on hover
                                                    "cursor: pointer;"
                                                    "}");
                        watchlistBtn->show();

                        // Set the actual movie ID and title as properties
                        watchlistBtn->setProperty("movieId", movieId);
                        watchlistBtn->setProperty("movieTitle", movieTitle);

                        connect(watchlistBtn, &QPushButton::clicked, this, &Home::onWatchlistButtonClicked);
                    }
                }
                reply->deleteLater();
            });

            displayedItems++;
        }
    }
}


void Home::fetchAndDisplayMoviePoster(const QString& movieName, int x, int y) {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QString apiKey = "bf3ce5fa7c0e31adaa82cb21e0c6886e"; // Replace with your actual TMDB API Key
    QString url = QString("https://api.themoviedb.org/3/search/movie?query=%1&api_key=%2").arg(movieName).arg(apiKey);
    QNetworkRequest request{QUrl(url)};

    // Send GET request to fetch movie details
    QNetworkReply* reply = manager->get(request);

    // Connect the finished signal for the reply
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj.contains("results") && obj["results"].isArray()) {
            QJsonArray results = obj["results"].toArray();
            if (!results.isEmpty()) {
                QString posterPath = results[0].toObject()["poster_path"].toString();

                if (posterPath.isEmpty()) {
                    return; // No poster found, exit the function
                }

                // Form the complete URL for the poster image
                QString posterUrl = "https://image.tmdb.org/t/p/w500" + posterPath;

                // Now create a request to download the poster image
                QNetworkRequest imgRequest{QUrl(posterUrl)};
                QNetworkReply* imgReply = manager->get(imgRequest);

                // Connect finished signal of the image request
                connect(imgReply, &QNetworkReply::finished, this, [=]() {
                    QByteArray imgData = imgReply->readAll();
                    QPixmap pixmap;
                    if (pixmap.loadFromData(imgData)) {
                        QLabel* posterLabel = new QLabel(this);
                        posterLabel->setPixmap(pixmap.scaled(311, 271, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        posterLabel->setAlignment(Qt::AlignCenter);
                        posterLabel->setFixedSize(311, 271);
                        posterLabel->move(x, y);
                        posterLabel->show();
                    } else {
                        qDebug() << "Failed to load image for " << movieName;
                    }
                    imgReply->deleteLater(); // Clean up the reply after usage
                });
            }
        }
        reply->deleteLater(); // Clean up the reply after processing
    });
}





void Home::onWatchListClicked() {
    if (UserSession::currentUser.isEmpty()) {
        QMessageBox::warning(this, "Access Denied", "You must log in.");
        loginWindow->show();
        return;
    }

    WatchlistWindow *watchlistWin = new WatchlistWindow(db, this);
    watchlistWin->show();
}

void Home::onLogoutClicked() {
    UserSession::currentUser.clear();
    QMessageBox::information(this, "Logout", "You have been logged out!");
    // loginWindow->show();
    this->close();
}



void Home::onWatchlistButtonClicked() {
    if (UserSession::currentUser.isEmpty()) {
        QMessageBox::warning(this, "Access Denied", "You must log in.");
        loginWindow->show();
        return;
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // Retrieve the properties
        int movieId = button->property("movieId").toInt();
        QString movieTitle = button->property("movieTitle").toString();
        // qDebug() << "Movie ID: " << movieId;
        // qDebug() << "Title: " << movieTitle;
        if (movieId != 0 && !movieTitle.isEmpty()) {
            int currentUserId = getUserId();
            // qDebug() << "Value of currentUserId = " << currentUserId;
            if (currentUserId == 0) {
                // Handle the case where the user is not logged in
                // qDebug() << "User is not logged in!";
                QMessageBox::information(this, "Access Denied", QString("User is not logged in!"));
                return;
            }

            // Pass the database object and userId to Watchlist
            Watchlist wlObj(db);

            // Store the movie in the user's watchlist
            if (wlObj.addMovie(currentUserId, movieId, movieTitle)) {
                // qDebug() << "Movie added to watchlist!";
                QMessageBox::information(this, "Watchlist", QString("Movie \"%1\" added to your watchlist!").arg(movieTitle));


            }
        } else {
            // qDebug() << "Movie ID or Title is invalid!";
            QMessageBox::information(this, "Watchlist Error", QString("Failed to add movie to your watchlist."));
        }
    }
}


void Home::setUserId(int userId){
    this->userId = userId;
}

int Home::getUserId() {
    return userId;
}
