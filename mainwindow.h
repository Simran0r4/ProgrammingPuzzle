#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QVector>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Structure for each question
struct Question {
    QString text;
    QStringList options;   // used for MCQ
    int correctIndex;      // 0..3 for MCQ; -1 if not applicable
    bool isMCQ;            // true => MCQ, false => code/snippet
    QString expectedAnswer; // used for code question (simple simulated match)
    QString hint;          // short hint to display when user asks
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserName(const QString &name) { userName = name; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_startButton_clicked();
    void on_instructionsButton_clicked();
    void on_backButton_clicked();
    void on_exitButton_clicked();

    // navigation
    void onNextQuestionClicked();
    void onBackQuestionClicked();

    // option selects (no popups)
    void onOptionGroupClicked(int id);

    // hint
    void onHintButtonClicked();

    void on_pushButton_clicked();
    void setupInstructionsPage();

    // LEADERBOARD - ADD THESE
    void showLeaderboard();
    void setupLeaderboardPage();

private:
    Ui::MainWindow *ui;

    QLabel *backgroundLabel;
    QWidget *overlay;

    // existing
    QString selectedLanguage;
    QMap<QString, int> languageProgress;
    QMap<QString, QStringList> languageLevels;

    // question system
    QMap<QString, QMap<int, QList<Question>>> questions; // language -> level -> list
    int currentLevel;
    int currentQuestionIndex;
    int lastScore;
    int lastTotal;

    // user answers storage
    QVector<int> userMCQAnswers;        // -1 if unanswered; index for chosen option
    QVector<QString> userCodeAnswers;   // empty if unanswered

    // option selection group
    QButtonGroup *optionGroup;

    // hints
    QMap<QString, QMap<int, int>> hintsAvailable; // language -> level -> hints left
    int hintsRemainingCurrentLevel;               // mirror for quick access
    QLabel *hintLabel;                            // shows hint under question

    // helper functions
    void loadQuestion(const QString &lang, int level);
    void refreshOptionButtonsUI(); // mark checked states based on userMCQAnswers
    void checkAndStoreCurrentAnswerBeforeNavigate(); // stores code answer when moving
    void finalizeSubmission();

    // utilities
    int computeBonusHintsForScore(int score);
    void updateHintCountUI();
    void debugUserAnswers();
    void updateLevelLockStatusUI();
    void saveUserScore(int level, int score, const QString &language);
    QString userName;
    void initDatabase();
    void openLevel(int level);
    void setupLevel(int level);
};

#endif // MAINWINDOW_H
