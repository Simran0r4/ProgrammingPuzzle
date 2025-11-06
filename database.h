#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVector>
#include <QVariant>

class Database {
public:
    static Database& instance() {
        static Database dbInstance;
        return dbInstance;
    }

    bool open();
    void close();
    bool saveUserScore(const QString &username, const QString &language, int level, int score);

    // New methods for user authentication
    bool createUserTable();
    bool userExists(const QString &username);
    bool addUser(const QString &username, const QString &password);
    bool verifyUser(const QString &username, const QString &password);

    // LEADERBOARD METHODS - ADD THESE
    bool createLeaderboardTable();
    bool updateLeaderboard(const QString &username, const QString &language, int score);
    QVector<QVector<QVariant>> getLeaderboard(const QString &language = "");

private:
    Database(); // private constructor
    QSqlDatabase db;
};

#endif // DATABASE_H
