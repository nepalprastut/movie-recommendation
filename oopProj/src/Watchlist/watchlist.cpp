#include <src/Watchlist/watchlist.h>
#include <QMessageBox>

Watchlist::Watchlist(Database *dbObj) : db(dbObj) {}

bool Watchlist::addMovie(int userId, int movieId, const QString &movieTitle) {
    QSqlQuery query;
    query.prepare("INSERT INTO Watchlist (user_id, movie_id, movie_title) VALUES (:userId, :movieId, :movieTitle)");
    query.bindValue(":userId", userId);
    query.bindValue(":movieId", movieId);
    query.bindValue(":movieTitle", movieTitle);

    if (!query.exec()) {
        qDebug() << "Error adding movie to watchlist:" << query.lastError().text();

        return false;
    }
    else  {
        qDebug() << "Movie added to your watchlist.";
        return true;

    }
}


bool Watchlist::removeMovie(int userId, int movieId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Watchlist WHERE user_id = :userId AND movie_id = :movieId");
    query.bindValue(":userId", userId);
    query.bindValue(":movieId", movieId);

    if (!query.exec()) {
        qDebug() << "Error removing movie from watchlist:" << query.lastError().text();
        return false;
    }
    return true;
}




bool Watchlist::movieExists(int userId, int movieId) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Watchlist WHERE user_id = :userId AND movie_id = :movieId");
    query.bindValue(":userId", userId);
    query.bindValue(":movieId", movieId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    } else {
        qDebug() << "Error checking movie existence:" << query.lastError().text();
        return false;
    }
}








