#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QLabel>
#include <src/Home/home.h>
#include <src/Watchlist/watchlist.h>
#include <src/Login/login.h>
#include <QPainter>
#include <QGraphicsBlurEffect>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    netManager = new QNetworkAccessManager(this);
    netReply = nullptr;
    img = new QPixmap();
    setFixedSize(1920, 1080);
    loginWindow = new Login(db, this);
    registerWindow = nullptr;


    QList<QPushButton*> buttons = {
        ui->pushButton_1,
        ui->pushButton_2,
        ui->pushButton_3,
        ui->pushButton_4,
        ui->pushButton_5,
        ui->pushButton_6,
        ui->pushButton_7,
        ui->pushButton_8,
        ui->pushButton_9,
        ui->pushButton_10
    };


    for(QPushButton *button : buttons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onWatchlistButtonClicked);
    }

    // new code
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/bgImg.jpg").scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));

    this->setPalette(palette);
    this->setAutoFillBackground(true);
    // creating navbar components programatically

    QWidget *navbarWidget = new QWidget(this);
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbarWidget);

    QLabel *popularMoviesLabel = new QLabel("Popular Movies", navbarWidget);


    //Login and Register buttons
    QPushButton *loginButton = new QPushButton("Login", navbarWidget);
    QPushButton *registerButton = new QPushButton("Register", navbarWidget);

    navbarLayout->addWidget(popularMoviesLabel);  // add label on the left
    navbarLayout->addStretch(1);  // push search bar to the center
    // navbarLayout->addWidget(searchBar);  // Add the search bar
    navbarLayout->addStretch(1);  // Add stretch on the right side
    navbarLayout->addWidget(loginButton);  // Add login button
    navbarLayout->addWidget(registerButton);  // Add register button

    // fill the window width
    navbarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Set the navbar widget as the central widget's layout
    setMenuWidget(navbarWidget);

    // connecting signals to slots
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);


    // styling navbar using stylesheet
    navbarWidget->setStyleSheet("background-color: #2E3A47; padding: 10px;");
    popularMoviesLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    // searchBar->setStyleSheet("padding: 5px; border-radius: 15px; border: 1px solid #ccc; background-color: #f0f0f0;");
    loginButton->setStyleSheet("padding: 5px 10px; background-color: #1E90FF; color: white; border-radius: 5px; font-size: 14px;");
    registerButton->setStyleSheet("padding: 5px 10px; background-color: #32CD32; color: white; border-radius: 5px; font-size: 14px;");
}


MainWindow::~MainWindow()
{
    delete ui;
    if (loginWindow) {
        delete loginWindow;
    }
    if (registerWindow) {
        delete registerWindow;
    }
    delete db;
}


const QString API_KEY = "bf3ce5fa7c0e31adaa82cb21e0c6886e";          // TMDB API key
static int currentLabelIndex = 0;

void MainWindow::sendRequest(int movieId)
{
    QNetworkRequest req{QUrl(QString("https://api.themoviedb.org/3/movie/%1?api_key=%2").arg(movieId).arg(API_KEY))};
    netReply = netManager->get(req);
    connect(netReply, &QNetworkReply::readyRead, this, &MainWindow::readData);
    connect(netReply, &QNetworkReply::finished, this, &MainWindow::finishReading);
}

void MainWindow::readData()
{
    dataBuffer.append(netReply->readAll());
}

void MainWindow::finishReading()
{
    if (netReply->error() != QNetworkReply::NoError) {
        qDebug() << "Error: " << netReply->errorString();
        QMessageBox::warning(this, "Error", QString("Request [Error]: %1").arg(netReply->errorString()));
    } else {
        // Storing the JSON response for use in other methods
        // dataBuffer = netReply->readAll();

        // Convert the data from a JSON doc to a JSON object
        QJsonObject movieJsonInfo = QJsonDocument::fromJson(dataBuffer).object();

        // Extract the poster path
        QString picLink = movieJsonInfo.value("poster_path").toString();
        int movieId = movieJsonInfo.value("id").toInt();
        QString movieTitle = movieJsonInfo.value("title").toString();

        if (picLink.isEmpty()) {
            qDebug() << "Poster path not found!";
            dataBuffer.clear(); // Clear the buffer
            currentLabelIndex++; // Increment index even if poster is not found
            // Trigger the next request if available
            triggerNextRequest();
            return; // Exit if there's no poster
        }

        // Construct the full URL for the poster image
        QNetworkRequest link{QUrl(QString("https://image.tmdb.org/t/p/w500%1").arg(picLink))};
        netReply = netManager->get(link);
        connect(netReply, &QNetworkReply::finished, this, &MainWindow::setUserImage);



        // set id and title as button properties
        QString buttonName = QString("pushButton_%1").arg(currentLabelIndex + 1);
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            button->setProperty("movieId", movieId);
            button->setProperty("movieTitle", movieTitle);
        } else {
            qDebug() << "Button not found: " << buttonName;
        }

        dataBuffer.clear();
    }
}

void MainWindow::setUserImage() {
    if (netReply->error() != QNetworkReply::NoError) {
        qDebug() << "Error fetching image: " << netReply->errorString();
        return; // Exit if there was an error
    }

    img->loadFromData(netReply->readAll());

    // Determine which label to set the pixmap to
    QString labelName = QString("picLabel_%1").arg(currentLabelIndex + 1); // Create label name dynamically
    QLabel* label = findChild<QLabel*>(labelName); // Find the label by name

    if (label) {
        // Scale and set the pixmap to the label
        QPixmap temp = img->scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(temp);

    } else {
        qDebug() << "Label not found: " << labelName;
    }

    currentLabelIndex++; // Increment the index for next image
    // Trigger next request if available
    triggerNextRequest();
}




// function to trigger the next movie request
void MainWindow::triggerNextRequest() {
    QList<int> movieIds = {238, 769, 680, 157336, 807, 27205, 16869, 694, 550, 389}; // List of movie IDs
    if (currentLabelIndex < movieIds.size()) {
        sendRequest(movieIds[currentLabelIndex]); // Send request for the next movie ID
    }
}


void MainWindow::on_check_clicked() {
    // Reset the label index before starting
    currentLabelIndex = 0;
    triggerNextRequest(); // Start the first request
}


void MainWindow::onLoginButtonClicked() {
    if (!loginWindow) {
        loginWindow = new Login(db, this);
    }
    loginWindow->show();
}



void MainWindow::onRegisterButtonClicked()
{
    if (!registerWindow) {
        registerWindow = new Register(db, this);
    }
    registerWindow->show();
}



void MainWindow::setUserId(int userId){
    this->userId = userId;
}

int MainWindow::getUserId() {
    return userId;
}



void MainWindow::onWatchlistButtonClicked() {
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

        if (movieId != 0 && !movieTitle.isEmpty()) {
            // Get the userId from MainWindow
            int currentUserId = getUserId();

            if (currentUserId == 0) {
                // Handle the case where the user is not logged in
                qDebug() << "User is not logged in!";
                return;
            }

            // Pass the database object and userId to Watchlist
            Watchlist wlObj(db);

            // Store the movie in the user's watchlist
            if (wlObj.addMovie(currentUserId, movieId, movieTitle)) {
                qDebug() << "Movie added to watchlist!";
            }
        } else {
            qDebug() << "Movie ID or Title is invalid!";
        }
    }
}











