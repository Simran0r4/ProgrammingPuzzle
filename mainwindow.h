#ifndef MAINWINDOW_H
#define MAINWINDOW_H   // <-- this was missing

#include <QMainWindow>
#include <QLabel>
#include <QPixmap> // for static image

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_startButton_clicked();
    void on_instructionsButton_clicked();
    void on_exitButton_clicked();
    void on_backButton_clicked();
    void goToQuizPage();

private:
    Ui::MainWindow *ui;

    QLabel *backgroundLabel;
    QPixmap backgroundImage; // store image here
    QWidget *overlay;
};

#endif // MAINWINDOW_H
