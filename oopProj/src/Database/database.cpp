#include "database.h"


// constructor to establish database connection
Database::Database(const QString &host, const QString &dbName, const QString &user, const QString &password)
    : hostName(host), databaseName(dbName), userName(user), password(password) {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
}


// destructor to disconnect from the database
Database::~Database() {
    disconnect();
}


// member function to show database connectivity status
bool Database::connect() {
    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully!";
    return true;
}


// member function to disconnect from the database
void Database::disconnect() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database disconnected.";
    }
}


