#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QMovie>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundLabel(nullptr)
    , backgroundMovie(nullptr)
    , overlay(nullptr)
{
    ui->setupUi(this);

    // --- Make central area transparent so the lowered GIF shows through ---
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

    // === Background GIF (child of MainWindow, not centralwidget) ===
    backgroundLabel = new QLabel(this);
    backgroundLabel->setObjectName("bgLabel");
    backgroundMovie = new QMovie("1_kGUl6EWTSViaPpGjaQ_kUw.gif");
    backgroundLabel->setMovie(backgroundMovie);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect()); // cover whole window client area
    backgroundLabel->lower();                    // go to back
    backgroundMovie->start();

    // === Dark overlay to reduce GIF intensity (sits above GIF, below UI) ===
    overlay = new QWidget(this);
    overlay->setObjectName("overlay");
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.35);");
    overlay->setGeometry(this->rect());
    overlay->lower();                // place just above background
    overlay->stackUnder(ui->centralwidget); // ensure overlay is under all UI content

    // === Show Home Page first ===
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Make background & overlay follow window size
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (backgroundLabel)
        backgroundLabel->setGeometry(this->rect());
    if (overlay)
        overlay->setGeometry(this->rect());
}

// === Button handlers ===
void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // Go to quiz page
}

void MainWindow::goToQuizPage()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_instructionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->instructionsBrowser->setHtml(
        "<h2>🧠 INSTRUCTIONS:</h2>"
        "<ul>"
        "<li><b>Welcome</b> to the Programming Puzzle App!</li>"
        "<li>You’ll be given a series of programming questions.</li>"
        "<li>Each question may include code, logic, or theory.</li>"
        "<li>There is no time limit — focus on accuracy!</li>"
        "<li>Feel free to retry questions and improve over time.</li>"
        "</ul>"
        "<h3>✅ HOW TO PLAY:</h3>"
        "<ul>"
        "<li>Read each question carefully.</li>"
        "<li>Choose the correct option from the answers shown.</li>"
        "<li>Click 'Next' to move to the following puzzle.</li>"
        "<li>After completing all questions, you'll see your score.</li>"
        "<li>You can review your answers after submission.</li>"
        "</ul>"
        "<h3>🎯 GOAL:</h3>"
        "<p>Answer all questions correctly and sharpen your programming skills. "
        "This app is designed to boost your coding logic, problem-solving, and understanding of key programming concepts.</p>"
        "<h3>📌 TIPS:</h3>"
        "<ul>"
        "<li>Think logically — don’t rush your answers.</li>"
        "<li>Understand the question fully before selecting.</li>"
        "<li>Use pen and paper if needed to solve logic problems.</li>"
        "<li>Don't worry about mistakes — each one is a learning opportunity!</li>"
        "</ul>"
        "<h3>💡 EXAMPLE TOPICS COVERED:</h3>"
        "<ul>"
        "<li>Loops and Conditionals</li>"
        "<li>Arrays and Strings</li>"
        "<li>Basic Algorithms</li>"
        "<li>Debugging simple C++ code</li>"
        "</ul>"
        "<h3>🚀 Ready? Click 'Start Game' to begin your challenge!</h3>"
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

