#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QString getUsername() const { return username; }

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString username;

    bool validateUser(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password);
};

#endif // LOGINDIALOG_H
