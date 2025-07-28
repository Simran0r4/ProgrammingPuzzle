#include "ui_mainwindow.h"  // MUST BE INCLUDED FIRST
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    if (ui && ui->instructionsBrowser) {
        ui->instructionsBrowser->setHtml(
            "<h2>🧠 INSTRUCTIONS:</h2>"
            "<ul>"
            "<li><b>Welcome</b> to the Programming Puzzle App!</li>"
            "<li>You’ll be given a series of programming questions.</li>"
            "<li>Each question may include code, logic, or theory.</li>"
            "</ul>"
            "<h3>✅ HOW TO PLAY:</h3>"
            "<ul>"
            "<li>Read the question carefully.</li>"
            "<li>Click the correct answer from the options.</li>"
            "<li>Get instant feedback and keep going!</li>"
            "</ul>"
            "<h3>🎯 GOAL:</h3>"
            "<p>Answer all questions correctly and sharpen your coding skills.</p>"
            "<h3>📌 TIPS:</h3>"
            "<ul>"
            "<li>Think logically — don’t rush.</li>"
            "<li>Understand the code before selecting.</li>"
            "<li>Review incorrect answers to learn.</li>"
            "</ul>"
            "<h3>🚀 Ready? Click 'Start Game'!</h3>"
            );
    }
}


void MainWindow::on_exitButton_clicked()
{
    close();
}
