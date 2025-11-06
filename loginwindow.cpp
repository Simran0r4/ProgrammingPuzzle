#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    if (!connectDatabase()) {
        showError("Database connection failed! Check your XAMPP settings.");
    }
}

LoginWindow::~LoginWindow()
{
    if (db.isOpen()) db.close();
    delete ui;
}

bool LoginWindow::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("CodeQuestDB");
    db.setUserName("phpadmin");
    db.setPassword("1234");

    if (!db.open()) {
        qDebug() << "DB connection error:" << db.lastError().text();
        return false;
    }

    return true;
}

bool LoginWindow::verifyLogin(const QString &username, const QString &password)
{
    if (!db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Login query error:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedHash = query.value(0).toString();
        QByteArray passHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
        return storedHash == passHash;
    }

    return false;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter username and password!");
        return;
    }

    if (verifyLogin(username, password)) {
        emit loginSuccess();
        this->hide();
    } else {
        showError("Invalid username or password!");
    }
}

void LoginWindow::on_registerButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        showError("Username and password cannot be empty!");
        return;
    }

    QByteArray passHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", passHash);

    if (!query.exec()) {
        qDebug() << "Registration error:" << query.lastError().text();
        showError("Registration failed. Username may already exist.");
    } else {
        QMessageBox::information(this, "Success", "User registered successfully!");
    }
}

void LoginWindow::showError(const QString &message)
{
    QMessageBox::critical(this, "Error", message);
}
