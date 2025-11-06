#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    bool connectDatabase();
    bool verifyLogin(const QString &username, const QString &password);
    void showError(const QString &message);

    Ui::LoginWindow *ui;
    QSqlDatabase db;
};

#endif // LOGINWINDOW_H
