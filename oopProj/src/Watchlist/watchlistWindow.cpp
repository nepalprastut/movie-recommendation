#include "watchlistWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <user_session.h>

WatchlistWindow::WatchlistWindow(Database *dbObj, QWidget *parent)
    : QMainWindow(parent), db(dbObj) {
    setupUI();
    loadWatchlist();
}

void WatchlistWindow::setupUI() {
    // Set the fixed size for the window
    setFixedSize(1920, 1080);

    // Navbar widget
    QWidget *navbarWidget = new QWidget(this);
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbarWidget);

    QPushButton *homeButton = new QPushButton("Home", this);
    homeButton->setStyleSheet("padding: 5px 10px; background-color: #007bff; color: white; border-radius: 5px;");
    connect(homeButton, &QPushButton::clicked, this, &WatchlistWindow::goToHome);
    navbarLayout->addWidget(homeButton, 0, Qt::AlignLeft);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    navbarLayout->addWidget(spacer);

    QPushButton *logoutButton = new QPushButton("Logout", this);
    logoutButton->setStyleSheet("padding: 5px 10px; background-color: #dc3545; color: white; border-radius: 5px;");
    connect(logoutButton, &QPushButton::clicked, this, &WatchlistWindow::logout);
    navbarLayout->addWidget(logoutButton, 0, Qt::AlignRight);

    navbarWidget->setStyleSheet("background-color: #2E3A47; padding: 10px;");
    navbarWidget->setFixedHeight(60);
    setMenuWidget(navbarWidget);

    // Title label
    QLabel *title = new QLabel("Your Watchlist", this);
    title->setAlignment(Qt::AlignLeft);
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin: 10px 0;");

    // Main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(title);

    // Table widget setup
    watchlistTable = new QTableWidget(this);
    watchlistTable->setColumnCount(2);  // Movie Title and Action
    watchlistTable->setHorizontalHeaderLabels({"Movie Title", "Action"});
    watchlistTable->horizontalHeader()->setStretchLastSection(false);
    watchlistTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);  // Stretch movie title column
    watchlistTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);    // Fixed size for action column
    watchlistTable->setColumnWidth(1, 120);  // Width for the "Remove" button column

    // Styling changes
    watchlistTable->setStyleSheet(
        "QTableWidget { "
        "    background-color: #f5f5f5; "
        "    border: none; "  // Remove borders
        "} "
        "QTableWidget::item { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #2E2E2E;"
        "    border: none;"        // Dark font for movie titles
        "} "
        "QHeaderView::section { "
        "    background-color: #2E3A47; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: none; "
        "} "
        "QTableWidget::item:selected { "
        "    background-color: blue; "
        "    border: none;"
        "}"
        );

    watchlistTable->setSelectionMode(QAbstractItemView::NoSelection);
    watchlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    watchlistTable->verticalHeader()->setVisible(false);  // Hide row numbers

    mainLayout->addWidget(watchlistTable);
    setCentralWidget(centralWidget);
}

void WatchlistWindow::loadWatchlist() {
    QString currentUser = UserSession::currentUser;
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "Error", "No user is logged in.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT user_id FROM Users WHERE username = :username");
    query.bindValue(":username", currentUser);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch user ID: " + query.lastError().text());
        return;
    }

    int userId = query.value(0).toInt();
    UserSession::userId = userId;

    query.prepare("SELECT movie_id, movie_title FROM Watchlist WHERE user_id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch watchlist: " + query.lastError().text());
        return;
    }

    watchlistTable->setRowCount(0);

    while (query.next()) {
        int movieId = query.value(0).toInt();
        QString movieTitle = query.value(1).toString();

        int currentRow = watchlistTable->rowCount();
        watchlistTable->insertRow(currentRow);

        // Movie title item
        QTableWidgetItem *titleItem = new QTableWidgetItem(movieTitle);
        titleItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        watchlistTable->setItem(currentRow, 0, titleItem);

        // Remove button setup
        QPushButton *removeButton = new QPushButton("Remove", this);
        removeButton->setStyleSheet(
            "padding: 5px 10px; background-color: #dc3545; color: white; border-radius: 5px; font-weight: bold;"
            );
        removeButton->setProperty("movieId", movieId);
        connect(removeButton, &QPushButton::clicked, this, &WatchlistWindow::on_removeButton_clicked);
        QWidget *buttonWidget = new QWidget();
        QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
        buttonLayout->addStretch();  // Right align the button
        buttonLayout->addWidget(removeButton);
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        watchlistTable->setCellWidget(currentRow, 1, buttonWidget);
    }
}


void WatchlistWindow::goToHome() {
    close();
}

void WatchlistWindow::logout() {
    UserSession::currentUser.clear();
    QMessageBox::information(this, "Logout", "You have been logged out.");
    this->close();
    if (!loginWindow) {
        loginWindow = new Login(db, this);
    }
    loginWindow->show();
}


void WatchlistWindow::on_removeButton_clicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) {
        qDebug() << "Error: Could not cast sender to QPushButton";
        return;
    }

    // Retrieve the movieId from the button's property
    int movieId = button->property("movieId").toInt();

    // Get the current user's userId from the session
    int userId = UserSession::userId;

    // Create an instance of the Watchlist class
    Watchlist watchlist(db);

    // Call the removeMovie method
    if (watchlist.removeMovie(userId, movieId)) {
        QMessageBox::information(this, "Success", "Movie removed from watchlist.");

        // Optionally, refresh the table to reflect changes
        loadWatchlist();
    } else {
        QMessageBox::critical(this, "Error", "Failed to remove movie from watchlist.");
    }
}

