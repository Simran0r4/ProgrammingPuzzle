#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
<<<<<<< HEAD
#include <QPixmap> // for static image
#include "question.h"  // ✅ added to use Question struct
=======
#include <QMovie>
#include <QVector>
#include "question.h"   // struct Question

>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e

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

    void on_Level1Button_clicked();
    void on_Level2Button_clicked();
    void on_Level3Button_clicked();
    void on_Level4Button_clicked();
    void on_Level5Button_clicked();
    void on_Level6Button_clicked();

    void on_BackButtonfromlvlpg_clicked();

    void on_Level7Button_clicked();
    void on_Level8Button_clicked();
    void on_Level9Button_clicked();
    void on_Level10Button_clicked();
    void on_Level11Button_clicked();
    void on_Level12Button_clicked();
    void on_Level13Button_clicked();
    void on_Level14Button_clicked();
    void on_Level15Button_clicked();
    void on_Level16Button_clicked();
    void on_Level17Button_clicked();
    void on_Level18Button_clicked();
    void on_Level19Button_clicked();
    void on_Level20Button_clicked();
    void on_Level21Button_clicked();
    void on_Level22Button_clicked();
    void on_Level23Button_clicked();
    void on_Level24Button_clicked();
    void on_Level25Button_clicked();
    void on_Level26Button_clicked();
    void on_Level27Button_clicked();
    void on_Level28Button_clicked();
    void on_Level29Button_clicked();
    void on_Level30Button_clicked();

    // New navigation buttons
    void on_nextButton_clicked();
    void on_skipButton_clicked();
    void on_submitButton_clicked();



private:

    void saveCurrentAnswer();
    void showQuestion(int Index);

    Ui::MainWindow *ui;

    QLabel *backgroundLabel;
    QPixmap backgroundImage; // store image here
    QWidget *overlay;

<<<<<<< HEAD
    QVector<Question> questions;  // ✅ store questions
=======
    QVector<Question> currentQuestions;  // Questions for current level
    int currentQuestionIndex;            // Current question index
    int score;                           // Score for current level
    QStringList userAnswers;             // User answers
>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e
};

#endif // MAINWINDOW_H
