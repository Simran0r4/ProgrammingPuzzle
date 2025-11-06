#include "database.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");

    // Use application directory instead of current working directory
    QString dbPath = QCoreApplication::applicationDirPath() + "/programming_puzzle.db";
    db.setDatabaseName(dbPath);

    qDebug() << "Database path:" << dbPath;
}

bool Database::open() {
    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        qDebug() << "Database path attempted:" << db.databaseName();
        return false;
    }

    QSqlQuery query;

    // Create users table first
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE,
            password TEXT
        )
    )")) {
        qDebug() << "Failed to create users table:" << query.lastError().text();
        return false;
    }

    // Create user_scores table
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS user_scores (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            language TEXT,
            level INTEGER,
            score INTEGER
        )
    )")) {
        qDebug() << "Failed to create user_scores table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database tables created successfully!";
    return true;
}

void Database::close() {
    if (db.isOpen())
        db.close();
}
// Add these to your Database class implementation:

bool Database::createLeaderboardTable() {
    QSqlQuery query;
    return query.exec(R"(
        CREATE TABLE IF NOT EXISTS leaderboard (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            language TEXT NOT NULL,
            total_score INTEGER DEFAULT 0,
            levels_completed INTEGER DEFAULT 0,
            last_updated DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )");
}

bool Database::updateLeaderboard(const QString &username, const QString &language, int score) {
    QSqlQuery query;

    // Check if user exists in leaderboard
    query.prepare("SELECT total_score, levels_completed FROM leaderboard WHERE username = ? AND language = ?");
    query.addBindValue(username);
    query.addBindValue(language);

    if (query.exec() && query.next()) {
        // Update existing entry
        int currentScore = query.value(0).toInt();
        int levelsCompleted = query.value(1).toInt();

        query.prepare("UPDATE leaderboard SET total_score = ?, levels_completed = ?, last_updated = CURRENT_TIMESTAMP WHERE username = ? AND language = ?");
        query.addBindValue(currentScore + score);
        query.addBindValue(levelsCompleted + 1);
        query.addBindValue(username);
        query.addBindValue(language);
    } else {
        // Insert new entry
        query.prepare("INSERT INTO leaderboard (username, language, total_score, levels_completed) VALUES (?, ?, ?, 1)");
        query.addBindValue(username);
        query.addBindValue(language);
        query.addBindValue(score);
    }

    return query.exec();
}

QVector<QVector<QVariant>> Database::getLeaderboard(const QString &language) {
    QVector<QVector<QVariant>> results;

    QSqlQuery query;
    if (language.isEmpty()) {
        query.prepare("SELECT username, language, total_score, levels_completed FROM leaderboard ORDER BY total_score DESC, last_updated DESC LIMIT 10");
    } else {
        query.prepare("SELECT username, language, total_score, levels_completed FROM leaderboard WHERE language = ? ORDER BY total_score DESC, last_updated DESC LIMIT 10");
        query.addBindValue(language);
    }

    if (query.exec()) {
        while (query.next()) {
            QVector<QVariant> row;
            row << query.value(0) << query.value(1) << query.value(2) << query.value(3);
            results.append(row);
        }
    }

    return results;
}
bool Database::saveUserScore(const QString &username, const QString &language, int level, int score) {
    if (!db.isOpen() && !open()) {
        qDebug() << "DB not open, cannot save score";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO user_scores (username, language, level, score) "
                  "VALUES (:username, :language, :level, :score)");
    query.bindValue(":username", username);
    query.bindValue(":language", language);
    query.bindValue(":level", level);
    query.bindValue(":score", score);

    if (!query.exec()) {
        qDebug() << "Failed to save score:" << query.lastError().text();
        return false;
    }

    qDebug() << "Score saved successfully for user:" << username << "Level:" << level << "Score:" << score;
    return true;
}

bool Database::userExists(const QString &username) {
    if (!db.isOpen() && !open()) {
        qDebug() << "DB not open, cannot check user";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    bool exists = query.exec() && query.next();
    qDebug() << "User exists check for" << username << ":" << exists;
    return exists;
}

bool Database::addUser(const QString &username, const QString &password) {
    if (!db.isOpen() && !open()) {
        qDebug() << "DB not open, cannot add user";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Failed to add user:" << query.lastError().text();
        return false;
    }

    qDebug() << "User added successfully:" << username;
    return true;
}

bool Database::verifyUser(const QString &username, const QString &password) {
    if (!db.isOpen() && !open()) {
        qDebug() << "DB not open, cannot verify user";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString storedPassword = query.value(0).toString();
        bool verified = (storedPassword == password);
        qDebug() << "User verification for" << username << ":" << verified;
        return verified;
    }

    qDebug() << "User verification failed - user not found:" << username;
    return false;
}
