#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>

class Database {
public:
    Database(const QString &host, const QString &dbName, const QString &user, const QString &password);
    ~Database();

    bool connect();
    void disconnect();


private:
    QSqlDatabase db;
    QString hostName;
    QString databaseName;
    QString userName;
    QString password;
};

#endif // DATABASE_H
