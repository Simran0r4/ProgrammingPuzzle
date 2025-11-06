#include "mainwindow.h"
#include "logindialog.h"
#include "database.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize and open the database
    if (!Database::instance().open()) {
        qDebug() << "Failed to open database!";
        return -1;
    }

    // Show login dialog first
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        QString username = loginDialog.getUsername();

        // Pass username to MainWindow
        MainWindow w;
        w.setUserName(username);
        w.show();

        return a.exec();
    }

    // If login was cancelled, exit application
    return 0;
}
