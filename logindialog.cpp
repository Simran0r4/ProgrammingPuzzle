#include "logindialog.h"
#include "ui_logindialog.h"
#include "database.h"
#include <QSqlQuery>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    username("")
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);

    this->setStyleSheet(R"(
/* Code Quest - Cyberpunk Theme with Background Image */
QDialog {
    background: transparent;
    color: #00FF00;
    font-family: 'Courier New', monospace;
    border: none;
}

LoginDialog {
    background: url(:/images/Screenshot 2025-11-03 at 2.53.53 PM.png) center center no-repeat;
    background-size: cover;
}

/* Main Title - Strong Neon Glow */
QLabel#titleLabel {
    color: #00FF00;
    font-size: 52px;
    font-weight: bold;
    text-align: center;
    padding: 40px 20px 30px 20px;
    text-shadow:
        0 0 10px #00FF00,
        0 0 20px #00FF00,
        0 0 30px #00FF00,
        0 0 40px #00FF00;
    background: transparent;
    border: none;
    margin: 20px 100px 10px 100px;
    font-family: 'Courier New', monospace;
    letter-spacing: 4px;
}

/* Subtitle with Neon Effect */
QLabel#subtitleLabel {
    color: #00FF00;
    font-size: 18px;
    text-align: center;
    padding: 15px 25px;
    text-shadow:
        0 0 8px #00FF00,
        0 0 16px #00FF00;
    background: transparent;
    border: none;
    margin: 0 150px 20px 150px;
    font-weight: bold;
    opacity: 0.9;
}

/* Instruction Label - Larger and Bolder */
QLabel#instructionLabel {
    color: #00FF00;
    font-size: 18px;
    font-weight: bold;
    text-align: center;
    padding: 15px 25px;
    text-shadow:
        0 0 8px #00FF00,
        0 0 16px #00FF00,
        0 0 24px #00FF00;
    background: rgba(0, 255, 0, 0.08);
    border: 1px solid #00FF00;
    border-radius: 10px;
    margin: 10px 150px 20px 150px;
}

/* Input Field Labels */
QLabel#usernameLabel, QLabel#passwordLabel {
    color: #00FF00;
    font-size: 16px;
    font-weight: bold;
    padding: 10px 5px 5px 5px;
    text-shadow:
        0 0 6px #00FF00,
        0 0 12px #00FF00;
    margin: 10px 150px 0 150px;
    background: transparent;
    border: none;
    border-bottom: 1px solid #00FF00;
}

/* Neon Input Fields - Shorter Width */
QLineEdit {
    background-color: rgba(0, 0, 0, 0.8);
    border: 2px solid #00FF00;
    border-radius: 8px;
    color: #00FF00;
    font-size: 14px;
    font-family: 'Courier New', monospace;
    padding: 12px 15px;
    margin: 5px 150px 15px 150px;
    selection-background-color: #00FF00;
    selection-color: #000000;
    box-shadow:
        0 0 10px rgba(0, 255, 0, 0.3),
        inset 0 0 5px rgba(0, 255, 0, 0.1);
    min-width: 300px;
    max-width: 400px;
}

QLineEdit:focus {
    border: 2px solid #00FF00;
    background-color: rgba(0, 0, 0, 0.9);
    box-shadow:
        0 0 20px #00FF00,
        0 0 30px rgba(0, 255, 0, 0.8),
        inset 0 0 10px rgba(0, 255, 0, 0.2);
    color: #00FF00;
}

QLineEdit::placeholder {
    color: #008800;
    font-style: italic;
    font-size: 13px;
}

/* New User Label - Larger and Bolder */
QLabel#newUserLabel {
    color: #00FF00;
    font-size: 16px;
    font-weight: bold;
    text-align: center;
    padding: 18px 25px;
    text-shadow:
        0 0 8px #00FF00,
        0 0 16px #00FF00,
        0 0 24px #00FF00;
    background: rgba(0, 255, 0, 0.08);
    border: 1px solid #00FF00;
    border-radius: 10px;
    margin: 20px 150px 15px 150px;
    font-style: italic;
}

/* Neon Buttons */
QPushButton {
    background: rgba(0, 0, 0, 0.8);
    color: #00FF00;
    font-size: 14px;
    font-family: 'Courier New', monospace;
    font-weight: bold;
    border: 2px solid #00FF00;
    border-radius: 8px;
    padding: 12px 25px;
    margin: 8px;
    text-align: center;
    text-shadow:
        0 0 5px #00FF00,
        0 0 10px #00FF00;
    min-width: 140px;
    box-shadow:
        0 0 10px rgba(0, 255, 0, 0.4);
}

QPushButton:hover {
    background: #00FF00;
    color: #000000;
    border: 2px solid #00FF00;
    text-shadow: none;
    box-shadow:
        0 0 20px #00FF00,
        0 0 30px rgba(0, 255, 0, 0.8);
}

QPushButton:pressed {
    background: #00CC00;
    color: #000000;
    border: 2px solid #00CC00;
    box-shadow:
        0 0 25px #00CC00,
        inset 0 2px 5px rgba(0, 0, 0, 0.5);
}

QPushButton:focus {
    border: 2px solid #00FF00;
    box-shadow:
        0 0 15px #00FF00,
        0 0 25px rgba(0, 255, 0, 0.6);
    outline: none;
}

/* Specific Login Button Styles */
QPushButton#loginButton {
    background: rgba(0, 0, 0, 0.8);
    font-size: 15px;
    padding: 14px 30px;
    border: 2px solid #00FF00;
    box-shadow:
        0 0 15px rgba(0, 255, 0, 0.5);
}

QPushButton#loginButton:hover {
    background: #00FF00;
    color: #000000;
    border: 2px solid #00FF00;
    box-shadow:
        0 0 25px #00FF00,
        0 0 40px rgba(0, 255, 0, 0.9);
}

QPushButton#registerButton {
    background: rgba(0, 0, 0, 0.8);
    font-size: 15px;
    padding: 14px 30px;
    border: 2px solid #00FF00;
    box-shadow:
        0 0 15px rgba(0, 255, 0, 0.5);
}

QPushButton#registerButton:hover {
    background: #00FF00;
    color: #000000;
    border: 2px solid #00FF00;
    box-shadow:
        0 0 25px #00FF00,
        0 0 40px rgba(0, 255, 0, 0.9);
}

/* Container Styling */
QWidget {
    background: transparent;
}

/* Button Container */
QWidget#buttonWidget {
    background: transparent;
    border-top: 1px solid rgba(0, 255, 0, 0.3);
    border-radius: 0;
    margin: 15px 150px;
    padding: 15px;
}

/* Input Container */
QWidget#inputWidget {
    background: transparent;
    border: none;
    border-radius: 0;
    margin: 15px 150px;
    padding: 15px;
}

/* Main dialog styling for centering */
QDialog {
    background: rgba(0, 0, 0, 0.7);
    border: 2px solid #00FF00;
    border-radius: 15px;
    margin: auto;
    box-shadow:
        0 0 50px rgba(0, 255, 0, 0.7),
        inset 0 0 30px rgba(0, 255, 0, 0.1);
    min-width: 600px;
    max-width: 800px;
    min-height: 400px;
}
    )");

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::on_loginButton_clicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginDialog::on_registerButton_clicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password!");
        return;
    }

    if (Database::instance().verifyUser(username, password)) {
        this->username = username;
        accept();
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password!");
    }
}

void LoginDialog::on_registerButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password!");
        return;
    }

    if (username.length() < 3) {
        QMessageBox::warning(this, "Error", "Username must be at least 3 characters long!");
        return;
    }

    if (password.length() < 4) {
        QMessageBox::warning(this, "Error", "Password must be at least 4 characters long!");
        return;
    }

    if (Database::instance().userExists(username)) {
        QMessageBox::warning(this, "Error", "Username already exists!");
        return;
    }

    if (Database::instance().addUser(username, password)) {
        QMessageBox::information(this, "Success", "Registration successful! You can now login.");
        this->username = username;
        accept();
    } else {
        QMessageBox::warning(this, "Error", "Registration failed!");
    }
}

bool LoginDialog::validateUser(const QString &username, const QString &password)
{
    return Database::instance().verifyUser(username, password);
}

bool LoginDialog::registerUser(const QString &username, const QString &password)
{
    if (Database::instance().userExists(username)) {
        return false;
    }
    return Database::instance().addUser(username, password);
}
