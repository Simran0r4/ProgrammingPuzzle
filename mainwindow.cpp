#include "ui_mainwindow.h"  // MUST BE INCLUDED FIRST
#include "mainwindow.h"
#include <QMovie>
#include <QLabel>
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // === Apply button style to all buttons ===
    QString buttonStyle = R"(
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 12px 24px;
            font-size: 16px;
            border-radius: 8px;
        }

        QPushButton:hover {
            background-color: #45a049;
        }

        QPushButton:pressed {
            background-color: #388E3C;
            padding-left: 14px;
            padding-top: 14px;
        }
    )";

    ui->startButton->setStyleSheet(buttonStyle);
    ui->instructionsButton->setStyleSheet(buttonStyle);
    ui->backButton->setStyleSheet(buttonStyle);
    ui->exitButton->setStyleSheet(buttonStyle);

    // === Add background GIF ===
    QLabel *backgroundLabel = new QLabel(this);
    QMovie *movie = new QMovie("1_kGUl6EWTSViaPpGjaQ_kUw.gif");

    backgroundLabel->setMovie(movie);
    backgroundLabel->setScaledContents(true);            // Stretch to full size
    backgroundLabel->setGeometry(this->rect());          // Initially set to full window size
    backgroundLabel->lower();                            // Send it to the back
    movie->start();

    // === Set opacity effect ===
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.3);                      // 0.0 = invisible, 1.0 = full opaque
    backgroundLabel->setGraphicsEffect(opacityEffect);

    // === Resize behavior: keep GIF fullscreen ===
    connect(this, &MainWindow::resizeEvent, [=](QResizeEvent *) {
        backgroundLabel->setGeometry(this->rect());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    ui->label->setText("Let's begin! Get ready for your first puzzle.");
}

void MainWindow::on_instructionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);  // switch to instructions page
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
        "<p>Answer all questions correctly and sharpen your programming skills. This app is designed to boost your coding logic, problem-solving, and understanding of key programming concepts.</p>"

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
    ui->stackedWidget->setCurrentIndex(0);  // Go back to the main/home page
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

