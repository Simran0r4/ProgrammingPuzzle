#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); // Show Home Page first
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // Go to quiz page
}
void MainWindow::goToQuizPage() {
    ui->stackedWidget->setCurrentIndex(2);  // or whatever index your quiz page is
}

void MainWindow::on_instructionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);  // Show instructions page
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
    ui->stackedWidget->setCurrentIndex(0);  // Back to home
}

void MainWindow::on_exitButton_clicked()
{
    close(); // Close app
}
