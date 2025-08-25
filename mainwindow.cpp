#include "ui_mainwindow.h"
#include "mainwindow.h"
<<<<<<< HEAD
#include "question.h"   // ✅ include question system
=======
#include "question.h"      // ✅ add this include
#include <QMovie>
>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e
#include <QLabel>
#include <QResizeEvent>
<<<<<<< HEAD
#include <QDebug>       // optional for console check
=======
#include <QMessageBox>      // ✅ add this include



void MainWindow::on_Level1Button_clicked()
{
    currentQuestions = getLevel1Questions();
    currentQuestionIndex = 0;
    score = 0;
    userAnswers.clear();

    showQuestion(currentQuestionIndex);
    ui->stackedWidget->setCurrentIndex(3);  // 3 = Level1Page
}


>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundLabel(nullptr)
    , overlay(nullptr)
    , currentQuestionIndex(0)
    , score(0)
{
    ui->setupUi(this);

    // --- Make central area transparent ---
    ui->centralwidget->setAttribute(Qt::WA_StyledBackground, true);
    ui->centralwidget->setStyleSheet("background: transparent;");

    // === Apply modern button style to all buttons ===
    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #4CAF50, stop:1 #2E7D32);
            color: white;
            font-size: 18px;
            font-weight: bold;
            border-radius: 12px;
            padding: 12px 24px;
            border: none;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #66BB6A, stop:1 #388E3C);
        }
        QPushButton:pressed {
            background-color: #1B5E20;
            padding-left: 14px;
            padding-top: 14px;
        }
    )";

    ui->startButton->setStyleSheet(buttonStyle);
    ui->instructionsButton->setStyleSheet(buttonStyle);
    ui->backButton->setStyleSheet(buttonStyle);
    ui->exitButton->setStyleSheet(buttonStyle);

    // === Background Image (from resources) ===
    backgroundLabel = new QLabel(this);
    backgroundLabel->setObjectName("bgLabel");
<<<<<<< HEAD
=======
    backgroundMovie = new QMovie("1_kGUl6EWTSViaPpGjaQ_kUw.gif");
    backgroundLabel->setMovie(backgroundMovie);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect()); // cover whole window
    backgroundLabel->lower();                   // send to back
    backgroundMovie->start();
>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e

    // ✅ Make sure this matches the file path inside resources.qrc
    backgroundImage = QPixmap(":/images/images/download.jpg");

    backgroundLabel->setPixmap(backgroundImage);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect()); // cover whole window
    backgroundLabel->lower();

    // === Dark overlay ===
    overlay = new QWidget(this);
    overlay->setObjectName("overlay");
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.35);");
    overlay->setGeometry(this->rect());
    overlay->lower();
<<<<<<< HEAD
    overlay->stackUnder(ui->centralwidget);

    // === Load questions at startup ===
    questions = getLevel1Questions();
    qDebug() << "Loaded" << questions.size() << "questions";  // console check
=======
    overlay->stackUnder(ui->centralwidget); // ensure overlay is under UI content
>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e

    // === Show Home Page first ===
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::showQuestion(int index) {
    if (index < 0 || index >= currentQuestions.size()) return;

    Question q = currentQuestions[index];
    ui->questionLabel->setText(q.questionText);

    if (q.type == "MCQ") {
        // Show options, hide code box
        ui->optionsGroup->show();
        ui->codeAnswerBox->hide();

        ui->option1->setText(q.options[0]);
        ui->option2->setText(q.options[1]);
        ui->option3->setText(q.options[2]);
        ui->option4->setText(q.options[3]);
    }
    else if (q.type == "CODE") {
        // Show code box, hide options
        ui->optionsGroup->hide();
        ui->codeAnswerBox->show();
        ui->codeAnswerBox->clear();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (backgroundLabel)
        backgroundLabel->setGeometry(this->rect());
    if (overlay)
        overlay->setGeometry(this->rect());
}

void MainWindow::on_startButton_clicked()
{
<<<<<<< HEAD
    // ✅ Show Quiz Page
    ui->stackedWidget->setCurrentIndex(2);

    // ✅ Example: display first question in QTextBrowser if it exists
    if (!questions.isEmpty()) {
        ui->questionLable->setText(questions[0].questionText);

    }
=======

    ui->stackedWidget->setCurrentIndex(2);  // Go to levels page
>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e
}


void MainWindow::on_Level2Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);  // switch to Level2Page
}

void MainWindow::on_Level3Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);  // switch to Level3Page
}
void MainWindow::on_Level4Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);  // switch to Level1Page
}

void MainWindow::on_Level5Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);  // switch to Level2Page
}

void MainWindow::on_Level6Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);  // switch to Level3Page
}

void MainWindow::saveCurrentAnswer()
{
    if (currentQuestions.isEmpty()) return;

    QString userAnswer;
    if (currentQuestions[currentQuestionIndex].type == "MCQ") {
        if (ui->option1->isChecked()) userAnswer = ui->option1->text();
        else if (ui->option2->isChecked()) userAnswer = ui->option2->text();
        else if (ui->option3->isChecked()) userAnswer = ui->option3->text();
        else if (ui->option4->isChecked()) userAnswer = ui->option4->text();
        else userAnswer = ""; // nothing selected
    } else {
        userAnswer = ui->codeAnswerBox->toPlainText();
    }

    if (userAnswers.size() > currentQuestionIndex)
        userAnswers[currentQuestionIndex] = userAnswer;
    else
        userAnswers.append(userAnswer);
}

void MainWindow::on_nextButton_clicked()
{
    saveCurrentAnswer();
    currentQuestionIndex++;
    if (currentQuestionIndex < currentQuestions.size()) {
        showQuestion(currentQuestionIndex);
    } else {
        on_submitButton_clicked(); // reached end, trigger submit
    }
}

void MainWindow::on_skipButton_clicked()
{
    if (userAnswers.size() > currentQuestionIndex)
        userAnswers[currentQuestionIndex] = ""; // mark as skipped
    else
        userAnswers.append("");

    currentQuestionIndex++;
    if (currentQuestionIndex < currentQuestions.size()) {
        showQuestion(currentQuestionIndex);
    } else {
        on_submitButton_clicked(); // reached end, trigger submit
    }
}




void MainWindow::on_instructionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->instructionsBrowser->setHtml(
        "<h2>🧠 INSTRUCTIONS:</h2>"
        "<p>1. Read each question carefully.</p>"
        "<p>2. Choose the best answer.</p>"
        "<p>3. Click 'Back' to return to the main menu.</p>"
        "<p>4. Click 'Exit' to close the application.</p>"
        );
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_exitButton_clicked()
{
    close();
}
<<<<<<< HEAD
=======


void MainWindow::on_BackButtonfromlvlpg_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_Level7Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}


void MainWindow::on_Level8Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}


void MainWindow::on_Level9Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}


void MainWindow::on_Level10Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}


void MainWindow::on_Level11Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}


void MainWindow::on_Level12Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
}


void MainWindow::on_Level13Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
}


void MainWindow::on_Level14Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
}


void MainWindow::on_Level15Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
}


void MainWindow::on_Level16Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(18);
}


void MainWindow::on_Level17Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
}


void MainWindow::on_Level18Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
}


void MainWindow::on_Level19Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(21);
}


void MainWindow::on_Level20Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);
}


void MainWindow::on_Level21Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(23);
}


void MainWindow::on_Level22Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
}


void MainWindow::on_Level23Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(25);
}


void MainWindow::on_Level24Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);
}


void MainWindow::on_Level25Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(27);
}


void MainWindow::on_Level26Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(28);
}


void MainWindow::on_Level27Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(29);
}


void MainWindow::on_Level28Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
}


void MainWindow::on_Level29Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(31);
}


void MainWindow::on_Level30Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(32);
}





void MainWindow::on_submitButton_clicked()
{
    saveCurrentAnswer(); // always save before evaluation

    if (currentQuestionIndex == currentQuestions.size() - 1) {
        // === Last question ===
        QMessageBox msgBox;
        msgBox.setWindowTitle("Level Completed!");
        msgBox.setText("🎯 You’ve answered all questions.\nDo you want to revisit or submit?");
        QPushButton *revisitButton = msgBox.addButton("Revisit", QMessageBox::ActionRole);
        QPushButton *submitButton  = msgBox.addButton("Submit", QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == revisitButton) {
            currentQuestionIndex = 0;
            showQuestion(currentQuestionIndex);
            userAnswers.clear(); // clear old answers if retrying
        }
        else if (msgBox.clickedButton() == submitButton) {
            int correct = 0;
            for (int i = 0; i < currentQuestions.size(); ++i) {
                if (userAnswers[i].trimmed() == currentQuestions[i].correctAnswer.trimmed()) {
                    correct++;
                }
            }
            QMessageBox::information(
                this,
                "Results",
                QString("✅ You scored %1 / %2").arg(correct).arg(currentQuestions.size())
                );
        }
    }
    else {
        // not last question → move forward
        currentQuestionIndex++;
        showQuestion(currentQuestionIndex);
    }
}

>>>>>>> faee4f033fc27b391351c6c4d115714fa16cfe1e
