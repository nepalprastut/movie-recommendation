#ifndef WATCHLIST_H
#define WATCHLIST_H


#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "src/Database/database.h"


class Watchlist {
public:
    Watchlist(Database *db);
    bool addMovie(int userId, int movieId, const QString &movieTitle);
    bool removeMovie(int userId, int movieId);
    bool movieExists(int userId, int movieId);

private:
    Database *db;
};

#endif


