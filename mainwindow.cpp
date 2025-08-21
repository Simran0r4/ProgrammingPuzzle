#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QLabel>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundLabel(nullptr)
    , overlay(nullptr)
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
    overlay->stackUnder(ui->centralwidget);

    // === Show Home Page first ===
    ui->stackedWidget->setCurrentIndex(0);
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
    ui->stackedWidget->setCurrentIndex(2);
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

