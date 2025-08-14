#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMovie>
#include <QGraphicsOpacityEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_instructionsButton_clicked();
    void on_exitButton_clicked();
    void on_backButton_clicked();

    void goToQuizPage();

private:
    Ui::MainWindow *ui;

    // === Added for background GIF ===
    QLabel *backgroundLabel;
    QMovie *backgroundMovie;
};

#endif // MAINWINDOW_H
