#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QInputDialog>
#include <QAbstractButton>
#include <QScrollArea>
#include <algorithm> // <-- added for std::max
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "database.h"


    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)
, backgroundLabel(nullptr)
, overlay(nullptr)
, currentLevel(0)
, currentQuestionIndex(0)
, optionGroup(nullptr)
, hintLabel(nullptr)
, hintsRemainingCurrentLevel(0)
{
    ui->setupUi(this);
    this->setStyleSheet(R"(
    /* Main Window Styles */

    QMainWindow, QWidget {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                   stop:0 #000000,
                                   stop:0.3 #001100,
                                   stop:0.7 #002200,
                                   stop:1 #000000);
        color: #00FF00;
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
    }
/* ===== HINT BUTTON STYLES (Use Hint) ===== */
    QPushButton#hintButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                    stop:0 rgba(80, 80, 0, 0.9),
                                    stop:0.5 rgba(120, 120, 0, 1),
                                    stop:1 rgba(60, 60, 0, 0.9));
        color: #FFFF00; /* Yellow text */
        font-size: 16px;
        font-weight: bold;
        border: 3px solid #FFFF00;
        border-radius: 8px;
        padding: 10px 15px;
        margin: 10px;
        text-shadow: 0 0 8px rgba(255, 255, 0, 0.8);
        min-width: 120px;
        min-height: 40px;
    }
    QPushButton#hintButton:hover {
        border: 3px solid #FFFFFF;
        box-shadow: 0 0 15px rgba(255, 255, 255, 0.7);
    }

    /* ===== HINT COUNT LABEL STYLES (Hints Left: X) ===== */
    QLabel#hintCountLabel {
        background-color: rgba(0, 50, 0, 0.9); /* Dark green background */
        color: #00FFFF; /* Cyan text for contrast */
        font-size: 18px;
        font-weight: bold;
        border: 2px solid #00FFFF;
        border-radius: 5px;
        padding: 8px;
        margin: 5px 10px 20px 10px;
        qproperty-alignment: AlignCenter;
        text-shadow: 0 0 5px rgba(0, 255, 255, 0.8);
        min-height: 25px;
    }

    /* ===== HINT CONTENT BOX STYLES (Appears below question) ===== */
    QLabel#hintLabel {
        font-size: 18px;
        color: #FFD700; /* Gold/Yellow text */
        font-style: italic;
        padding: 15px;
        margin: 5px 30px 10px 30px;
        background-color: rgba(50, 0, 50, 0.8); /* Dark purple background */
        border: 2px solid #FF00FF; /* Magenta border */
        border-radius: 10px;
        text-shadow: 0 0 8px rgba(255, 0, 255, 0.5);
        min-height: 40px;
        qproperty-wordWrap: true;
    }

    /* Enhanced Title Styles with Matrix Effect */
    #codeQuestTitleLabel {
        font-size: 88px;
        color: #00FF00;
        qproperty-alignment: AlignCenter;
        text-shadow: 0 0 20px rgba(0, 255, 0, 0.9),
                     0 0 40px rgba(0, 255, 0, 0.7),
                     0 0 60px rgba(0, 255, 0, 0.5),
                     0 0 80px rgba(0, 255, 0, 0.3);
        background-color: rgba(0, 30, 0, 0.6);
        border: 4px solid #00FF00;
        border-radius: 20px;
        padding: 25px;
        margin: 30px 50px 20px 50px;
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 rgba(0, 40, 0, 0.8),
                                   stop:0.5 rgba(0, 60, 0, 0.9),
                                   stop:1 rgba(0, 40, 0, 0.8));
    }

    /* Enhanced Tagline */
    #taglineLabel {
        font-size: 20px;
        color: #00FFFF;
        qproperty-alignment: AlignCenter;
        padding: 15px;
        text-shadow: 0 0 15px rgba(0, 255, 255, 0.8),
                     0 0 25px rgba(0, 255, 255, 0.6);
        font-style: italic;
        margin-bottom: 40px;
        background-color: rgba(0, 40, 40, 0.4);
        border: 2px solid #00FFFF;
        border-radius: 10px;
        margin-left: 100px;
        margin-right: 100px;
    }

    /* Subtle Pulse Animation */
    #welcomeMessageLabel {
        font-size: 24px;
        color: #00FF88;
        qproperty-alignment: AlignCenter;
        text-shadow: 0 0 10px rgba(0, 255, 136, 0.8),
                     0 0 20px rgba(0, 255, 136, 0.6);
        background: transparent;
        border: none;
        padding: 30px;
        margin: 55px 0;
        font-style: italic;
        /* Creates a subtle pulsing effect */
        animation: pulse 3s infinite;
    }

    /* Enhanced Main Button Container */
    QWidget#centralWidget {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                   stop:0 #000000,
                                   stop:0.2 #001100,
                                   stop:0.8 #002200,
                                   stop:1 #000000);
        border: none;
    }

/* ===== LEVEL SCREEN HEADING ===== */
    #levelsTitleLabel {
        font-size: 60px; /* INCREASED from 48px to 60px */
        color: #00FF00;
        qproperty-alignment: AlignCenter;
        text-shadow: 0 0 15px rgba(0, 255, 0, 0.9),
                     0 0 30px rgba(0, 255, 0, 0.7),
                     0 0 45px rgba(0, 255, 0, 0.5);
        background: rgba(0, 30, 0, 0.6);
        border: 3px solid #00FF00;
        border-radius: 15px;
        padding: 20px;
        margin: 20px 80px 30px 80px;
        font-weight: bold;
        letter-spacing: 2px;
    }

    /* ===== QUESTION TEXT STYLING - FIXED FOR LONG QUESTIONS ===== */
    #questionLabel {
        font-size: 24px;
        color: #00FF00;
        qproperty-alignment: AlignLeft;
        text-shadow: 0 0 8px rgba(0, 255, 0, 0.8),
                     0 0 16px rgba(0, 255, 0, 0.6);
        background: rgba(0, 20, 0, 0.4);
        border: 2px solid #00FF00;
        border-radius: 10px;
        padding: 20px;
        margin: 10px 30px;
        font-weight: bold;
        line-height: 1.4;
        min-height: 120px;
        qproperty-wordWrap: true;
    }
/* ===== HOME PAGE BUTTONS - UNIFIED SIZE AND SPACING ===== */

    /* Define common properties for size, spacing, and font */
    #startButton, #instructionsButton, #exitButton {
        font-size: 30px;
        padding: 25px 45px; /* Ensures consistent height and width */
        margin: 20px 0;    /* Consistent vertical spacing */
        border-radius: 12px;
        font-weight: bold;
        min-width: 320px; /* Ensure buttons are wide enough */
    }

    #startButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                    stop:0 rgba(0, 80, 0, 0.9),
                                    stop:0.5 rgba(0, 120, 0, 1),
                                    stop:1 rgba(0, 60, 0, 0.9));
        border: 4px solid #00FF00;
        margin-top: 50px; /* Increased top margin for separation from title */
    }

    #instructionsButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                    stop:0 rgba(0, 60, 60, 0.9),
                                    stop:0.5 rgba(0, 100, 100, 1),
                                    stop:1 rgba(0, 40, 40, 0.9));
        border: 3px solid #00FFFF;
        font-size: 28px; /* Slightly smaller for balance, but still big */
    }

    #exitButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                    stop:0 rgba(60, 0, 0, 0.9),
                                    stop:0.5 rgba(100, 0, 0, 1),
                                    stop:1 rgba(40, 0, 0, 0.9));
        border: 3px solid #FF0000;
    }
    #exitButton:hover {
        border: 3px solid #FF4444;
        box-shadow: 0 0 25px rgba(255, 0, 0, 0.8);
    }
    /* ===== LEADERBOARD BUTTON STYLES ===== */
    #leaderboardButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(80, 60, 0, 0.9),
                                   stop:0.5 rgba(120, 90, 0, 1),
                                   stop:1 rgba(60, 45, 0, 0.9));
        color: #FFD700;
        font-size: 16px;
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 2px solid #FFD700;
        border-radius: 8px;
        padding: 8px 12px;
        margin: 10px;
        text-align: center;
        text-shadow: 0 0 5px rgba(255, 215, 0, 0.8);
        min-width: 180px;
        max-width: 200px;
        min-height: 40px;
        max-height: 50px;
    }
    #leaderboardButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(120, 90, 0, 1),
                                   stop:0.5 rgba(160, 120, 0, 1),
                                   stop:1 rgba(100, 75, 0, 1));
        color: #FFFFFF;
        border: 2px solid #FFFF00;
        text-shadow: 0 0 10px #FFFFFF;
        box-shadow: 0 0 15px rgba(255, 255, 0, 0.7);
    }
    /* ===== LEVEL BUTTONS - LARGER FONT SIZE ===== */
    #level1Button, #level2Button, #level3Button, #level4Button {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 60, 0, 0.9),
                                   stop:0.5 rgba(0, 100, 0, 1),
                                   stop:1 rgba(0, 40, 0, 0.9));
        color: #00FF00;
        font-size: 28px; /* Increased font size for level buttons */
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 3px solid #00FF00;
        border-radius: 12px;
        padding: 20px 30px; /* Increased padding */
        margin: 15px 0;
        text-align: center;
        text-shadow: 0 0 8px rgba(0, 255, 0, 0.8);
        min-width: 300px;
        min-height: 35px;
    }

    #level1Button:hover, #level2Button:hover, #level3Button:hover, #level4Button:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 100, 0, 1),
                                   stop:0.5 rgba(0, 140, 0, 1),
                                   stop:1 rgba(0, 80, 0, 1));
        color: #FFFFFF;
        border: 3px solid #00FFFF;
        text-shadow: 0 0 15px #FFFFFF;
        box-shadow: 0 0 25px rgba(0, 255, 255, 0.9),
                    inset 0 0 15px rgba(255, 255, 255, 0.3);
        transform: translateY(-3px);
    }

    /* ===== OPTION BUTTONS - LARGER FONT SIZE ===== */
    #option1Button, #option2Button, #option3Button, #option4Button {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 40, 0, 0.9),
                                   stop:0.5 rgba(0, 60, 0, 1),
                                   stop:1 rgba(0, 30, 0, 0.9));
        color: #00FF00;
        font-size: 20px; /* Increased from default */
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 2px solid #00FF00;
        border-radius: 8px;
        padding: 15px 20px; /* Increased padding */
        margin: 10px 0;
        text-align: left;
        text-shadow: 0 0 5px rgba(0, 255, 0, 0.7);
        min-width: 400px;
        min-height: 30px;
    }

    #option1Button:hover, #option2Button:hover, #option3Button:hover, #option4Button:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 80, 0, 1),
                                   stop:0.5 rgba(0, 100, 0, 1),
                                   stop:1 rgba(0, 60, 0, 1));
        color: #FFFFFF;
        border: 2px solid #00FFFF;
        text-shadow: 0 0 10px #FFFFFF;
        box-shadow: 0 0 15px rgba(0, 255, 255, 0.7);
    }

    #option1Button:checked, #option2Button:checked, #option3Button:checked, #option4Button:checked {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 100, 0, 1),
                                   stop:0.5 rgba(0, 140, 0, 1),
                                   stop:1 rgba(0, 80, 0, 1));
        border: 2px solid #00FF00;
        color: #FFFFFF;
        box-shadow: 0 0 20px rgba(0, 255, 0, 0.9);
    }

    /* ===== LANGUAGE SELECTION PAGE ENHANCEMENTS ===== */

    /* Page Container */
    #languagePage {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                   stop:0 #000000,
                                   stop:0.3 #001100,
                                   stop:0.7 #002200,
                                   stop:1 #000000);
        border: none;
    }

    /* Language Title */
    #languageTitleLabel {
        font-size: 56px;
        color: #00FF00;
        qproperty-alignment: AlignCenter;
        text-shadow: 0 0 20px rgba(0, 255, 0, 0.9),
                     0 0 40px rgba(0, 255, 0, 0.7),
                     0 0 60px rgba(0, 255, 0, 0.5);
        padding: 30px;
        margin: 20px 0 40px 0;
        background: transparent;
        border: none;
        font-weight: bold;
        letter-spacing: 2px;
    }

    /* Language Buttons Container */
    QWidget#languageButtonsContainer {
        background: rgba(0, 20, 0, 0.3);
        border: 3px solid #00FF00;
        border-radius: 20px;
        margin: 0 80px;
        padding: 40px;
    }

QPushButton#cppButton, QPushButton#pythonButton, QPushButton#cButton, QPushButton#javaButton {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 rgba(0, 128, 0, 1),
        stop:0.5 rgba(0, 200, 0, 1),
        stop:1 rgba(0, 100, 0, 0.9)
    );
    color: #00FF00;
    border: 3px solid #00FF00;
    border-radius: 15px;
    padding: 20px 30px;
    margin: 15px 0;
    text-shadow: 0 0 10px rgba(0, 255, 0, 0.8);
    min-width: 300px;
    min-height: 30px;
}
QPushButton#cppButton:hover, QPushButton#pythonButton:hover, QPushButton#cButton:hover, QPushButton#javaButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 rgba(0, 200, 0, 1),
        stop:0.5 rgba(0, 255, 0, 1),
        stop:1 rgba(0, 180, 0, 1)
    );
    color: #FFFFFF;
    border: 3px solid #00FFFF;
    text-shadow: 0 0 15px #FFFFFF;
    box-shadow: 0 0 25px rgba(0, 255, 255, 0.9), inset 0 0 20px rgba(255, 255, 255, 0.2);
    transform: translateY(-3px);
}

    /* Back Button - Green Theme */
    QPushButton#backButtonLevels {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 60, 0, 0.9),
                                   stop:0.5 rgba(0, 100, 0, 1),
                                   stop:1 rgba(0, 40, 0, 0.9));
        color: #00FF00;
        font-size: 18px;
        border: 3px solid #00FF00;
        border-radius: 10px;
        padding: 15px 25px;
        margin: 30px 0 20px 0;
        text-shadow: 0 0 5px rgba(0, 255, 0, 0.7);
        min-width: 200px;
    }

    QPushButton#backButtonLevels:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 100, 0, 1),
                                   stop:0.5 rgba(0, 140, 0, 1),
                                   stop:1 rgba(0, 80, 0, 1));
        border: 3px solid #00FFFF;
        box-shadow: 0 0 20px rgba(0, 255, 255, 0.7);
        color: #FFFFFF;
    }

    /* Language Icons (Optional - if you add them later) */
    QPushButton#cppButton::before {
        content: "⚡";
        margin-right: 10px;
    }

    QPushButton#pythonButton::before {
        content: "🐍";
        margin-right: 10px;
    }

    QPushButton#cButton::before {
        content: "🔧";
        margin-right: 10px;
    }

    QPushButton#javaButton::before {
        content: "☕";
        margin-right: 10px;
    }

    /* Cyberpunk Button Styles */
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 60, 0, 0.9),
                                   stop:0.5 rgba(0, 80, 0, 1),
                                   stop:1 rgba(0, 40, 0, 0.9));
        color: #00FF00;
        font-size: 24px;
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 3px solid #00FF00;
        border-radius: 12px;
        padding: 18px 35px;
        margin: 12px 0;
        text-align: center;
        text-shadow: 0 0 8px rgba(0, 255, 0, 0.8);
        min-width: 280px;
        min-height: 25px;
    }

    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 100, 0, 1),
                                   stop:0.5 rgba(0, 120, 0, 1),
                                   stop:1 rgba(0, 80, 0, 1));
        color: #FFFFFF;
        border: 3px solid #00FFFF;
        text-shadow: 0 0 15px #FFFFFF;
        box-shadow: 0 0 25px rgba(0, 255, 255, 0.9),
                    inset 0 0 15px rgba(255, 255, 255, 0.3);
        transform: translateY(-3px);
    }

    QPushButton:pressed {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 120, 0, 1),
                                   stop:0.5 rgba(0, 140, 0, 1),
                                   stop:1 rgba(0, 100, 0, 1));
        border: 3px solid #00FF00;
        box-shadow: 0 0 35px rgba(0, 255, 0, 1),
                    inset 0 0 20px rgba(255, 255, 255, 0.4);
        transform: translateY(-1px);
    }

    QPushButton:focus {
        border: 3px solid #00FFFF;
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 80, 0, 1),
                                   stop:0.5 rgba(0, 100, 0, 1),
                                   stop:1 rgba(0, 60, 0, 1));
        box-shadow: 0 0 30px rgba(0, 255, 255, 1);
        outline: none;
    }

    /* Button Container Styling */
    QFrame {
        background-color: rgba(0, 20, 0, 0.3);
        border: 2px solid #00FF00;
        border-radius: 15px;
        margin: 20px 100px;
        padding: 30px;
    }

    /* Scroll Area Enhancement (if used) */
    QScrollArea {
        border: 2px solid #00FF00;
        border-radius: 10px;
        background-color: rgba(0, 30, 0, 0.5);
    }

    QScrollBar:vertical {
        background: rgba(0, 40, 0, 0.8);
        width: 15px;
        margin: 0px;
        border-radius: 7px;
    }

    QScrollBar::handle:vertical {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 #00FF00, stop:0.5 #00FF88, stop:1 #00FF00);
        border-radius: 7px;
        min-height: 20px;
    }

    QScrollBar::handle:vertical:hover {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                   stop:0 #00FF88, stop:0.5 #00FFFF, stop:1 #00FF88);
    }
/* ===== FIX SCREEN MARGIN ISSUES ===== */
    QMainWindow {
        padding: 0px;
    }

    QWidget#centralWidget {
        padding: 0px;
        margin: 0px;
    }

    QStackedWidget {
        padding: 0px;
        margin: 0px;
    }

    QWidget#languagePage {
        padding: 10px;
        margin: 0px;
    }

    /* Fix for leaderboard page margins */
    #leaderboardPage {
        padding: 10px;
        margin: 0px;
    }

    #leaderboardTable {
        margin: 5px;
        padding: 5px;
    }
    /* ===== FIX LANGUAGE BUTTONS ONLY ===== */
    #cppButton, #pythonButton, #cButton, #javaButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 60, 0, 0.9),
                                   stop:0.5 rgba(0, 100, 0, 1),
                                   stop:1 rgba(0, 40, 0, 0.9));
        color: #00FF00;
        border: 3px solid #00FF00;
    }

    #cppButton:hover, #pythonButton:hover, #cButton:hover, #javaButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(0, 100, 0, 1),
                                   stop:0.5 rgba(0, 140, 0, 1),
                                   stop:1 rgba(0, 80, 0, 1));
        color: #FFFFFF;
        border: 3px solid #00FFFF;
    /* ===== LEADERBOARD BUTTON STYLES ===== */
    #leaderboardButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(80, 60, 0, 0.9),
                                   stop:0.5 rgba(120, 90, 0, 1),
                                   stop:1 rgba(60, 45, 0, 0.9));
        color: #FFD700;
        font-size: 22px;
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 3px solid #FFD700;
        border-radius: 12px;
        padding: 15px 25px;
        margin: 10px 0;
        text-align: center;
        text-shadow: 0 0 8px rgba(255, 215, 0, 0.8);
        min-height: 25px;
    }
    #leaderboardButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(120, 90, 0, 1),
                                   stop:0.5 rgba(160, 120, 0, 1),
                                   stop:1 rgba(100, 75, 0, 1));
        color: #FFFFFF;
        border: 3px solid #FFFF00;
        text-shadow: 0 0 15px #FFFFFF;
        box-shadow: 0 0 25px rgba(255, 255, 0, 0.9);
    }
#leaderboardPage {
        margin: 10px;
        padding: 10px;
    }

    #leaderboardTable {
        margin: 10px;
        padding: 10px;
    }

    /* ===== LEADERBOARD BUTTON STYLES ===== */
    #leaderboardButton {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(80, 60, 0, 0.9),
                                   stop:0.5 rgba(120, 90, 0, 1),
                                   stop:1 rgba(60, 45, 0, 0.9));
        color: #FFD700;
        font-size: 18px;
        font-family: 'Courier New', 'Monospace', monospace;
        font-weight: bold;
        border: 2px solid #FFD700;
        border-radius: 8px;
        padding: 10px 15px;
        margin: 5px;
        text-align: center;
        text-shadow: 0 0 5px rgba(255, 215, 0, 0.8);
        min-height: 20px;
        max-width: 200px;
    }
    #leaderboardButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                   stop:0 rgba(120, 90, 0, 1),
                                   stop:0.5 rgba(160, 120, 0, 1),
                                   stop:1 rgba(100, 75, 0, 1));
        color: #FFFFFF;
        border: 2px solid #FFFF00;
        text-shadow: 0 0 10px #FFFFFF;
        box-shadow: 0 0 15px rgba(255, 255, 0, 0.7);
    }
)"); // This is your existing closing parenthesis - don't change it



    ui->option1Button->setCheckable(true);
    ui->option2Button->setCheckable(true);
    ui->option3Button->setCheckable(true);
    ui->option4Button->setCheckable(true);

    optionGroup = new QButtonGroup(this);
    optionGroup->setExclusive(true);
    optionGroup->addButton(ui->option1Button, 0);
    optionGroup->addButton(ui->option2Button, 1);
    optionGroup->addButton(ui->option3Button, 2);
    optionGroup->addButton(ui->option4Button, 3);
    connect(optionGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* btn) {
        int id = optionGroup->id(btn);
        onOptionGroupClicked(id);
    });

    connect(ui->nextQuestionButton, &QPushButton::clicked, this, &MainWindow::onNextQuestionClicked);
    connect(ui->backQuestionButton, &QPushButton::clicked, this, &MainWindow::onBackQuestionClicked);
    connect(ui->hintButton, &QPushButton::clicked, this, &MainWindow::onHintButtonClicked);

    backgroundLabel = new QLabel(this);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect());
    backgroundLabel->lower();

    overlay = new QWidget(this);
    overlay->setStyleSheet("background-color: transparent;");
    overlay->setGeometry(this->rect());
    overlay->lower();
    hintLabel = new QLabel(ui->questionPage);
    hintLabel->setObjectName("hintLabel");
    hintLabel->setWordWrap(true);

    if (ui->questionLabel) {
        QRect qRect = ui->questionLabel->geometry();
        // Positioned 6 pixels below the question label
        //hintLabel->setGeometry(qRect.x(), qRect.y() + qRect.height() + 6, qRect.width(), 60);
    } else {
        //hintLabel->setGeometry(20, 300, 600, 60);
    }
    hintLabel->hide();

    languageProgress["C++"] = 1;
    languageProgress["Python"] = 1;
    languageProgress["C"] = 1;
    languageProgress["Java"] = 1;

    languageLevels["C++"] = {"Level 1: Basics of C++", "Level 2: Loops and Functions", "Level 3: OOP Concepts", "Level 4: File Handling"};
    languageLevels["Python"] = {"Level 1: Basics of Python", "Level 2: Loops and Lists", "Level 3: Functions & Modules", "Level 4: File I/O"};
    languageLevels["C"] = {"Level 1: Basics of C", "Level 2: Control Statements", "Level 3: Arrays & Pointers", "Level 4: Structures"};
    languageLevels["Java"] = {"Level 1: Java Fundamentals", "Level 2: Classes & Objects", "Level 3: Inheritance", "Level 4: Exception Handling"};

    hintsAvailable["C++"][1] = 5;
    hintsAvailable["C++"][2] = 5;
    hintsAvailable["C++"][3] = 5;
    hintsAvailable["C++"][4] = 0;

    hintsAvailable["Python"][1] = 5;
    hintsAvailable["Python"][2] = 5;
    hintsAvailable["Python"][3] = 5;
    hintsAvailable["Python"][4] = 0;

    hintsAvailable["C"][1] = 5;
    hintsAvailable["C"][2] = 5;
    hintsAvailable["C"][3] = 5;
    hintsAvailable["C"][4] = 0;

    hintsAvailable["Java"][1] = 5;
    hintsAvailable["Java"][2] = 5;
    hintsAvailable["Java"][3] = 5;
    hintsAvailable["Java"][4] = 0;

    QList<Question> cxxLevel1;
    cxxLevel1.append({"What is the correct way to declare a variable 'x' of type int?",
                      {"int x;", "x int;", "int: x;", "var x;"}, 0, true, "", "Use the C++ type first, then name."});
    cxxLevel1.append({"Which of the following is the correct increment operator?",
                      {"++", "--", "**", "=="}, 0, true, "", "One plus on the variable increments it."});
    cxxLevel1.append({"Which header is needed for std::cout?",
                      {"<iostream>", "<stdio.h>", "<string>", "<vector>"}, 0, true, "", "It's part of the iostream library."});
    cxxLevel1.append({"What will '5 / 2' evaluate to in integer arithmetic?",
                      {"2", "2.5", "3", "Undefined"}, 0, true, "", "Integer division drops the decimal."});
    cxxLevel1.append({"Which keyword creates a class in C++?",
                      {"struct", "class", "object", "module"}, 1, true, "", "C++ uses 'class' for classes."});
    cxxLevel1.append({"Which of these is used to allocate memory dynamically?",
                      {"new", "malloc", "alloc", "dyn"}, 0, true, "", "C++ uses new; malloc is C-style."});
    cxxLevel1.append({"Which operator is used for member access via pointer?",
                      {"->", ".", "::", "->>"}, 0, true, "", "Use arrow operator for pointer to object."});
    cxxLevel1.append({"Which loop guarantees execution at least once?",
                      {"for", "while", "do-while", "foreach"}, 2, true, "", "do-while executes before checking condition."});
    cxxLevel1.append({"What does 'const' mean when placed before a variable?",
                      {"Mutable", "Read-only", "Volatile", "Static"}, 1, true, "", "const prevents modification."});
    cxxLevel1.append({"Which is the correct main function signature in C++?",
                      {"int main()", "void main()", "main()", "int main(void) (C only)"}, 0, true, "", "int main() is standard."});
    cxxLevel1.append({"Which container is best for fast random access?",
                      {"std::list", "std::vector", "std::map", "std::set"}, 1, true, "", "vector gives O(1) index access."});

    cxxLevel1.append({"Write a single line that declares an integer named 'count' initialized to 10.",
                      {}, -1, false, "int count = 10;", "Declare with the type, variable name and initializer."});
    cxxLevel1.append({"Write the header include for input-output stream (one line).",
                      {}, -1, false, "#include <iostream>", "Include the iostream header."});
    cxxLevel1.append({"Write a line that prints 'Hello' using std::cout (without extra lines).",
                      {}, -1, false, "std::cout << \"Hello\";", "Use std::cout with the insertion operator."});
    cxxLevel1.append({"Write a line that declares a function prototype 'int add(int a, int b);' (exact signature).",
                      {}, -1, false, "int add(int a, int b);", "Prototype uses the return type then name and parameter list."});

    questions["C++"][1] = cxxLevel1;

    QList<Question> cxxLevel2;

    cxxLevel2.append({"Which of the following is not a loop in C++?",
                      {"for", "repeat", "while", "do-while"}, 1, true, "", "C++ has no 'repeat' loop."});
    cxxLevel2.append({"Which loop checks the condition at the end?",
                      {"for", "while", "do-while", "if-else"}, 2, true, "", "do-while checks after executing body."});
    cxxLevel2.append({"How many times does a while loop run if its condition is false initially?",
                      {"0", "1", "2", "Infinite"}, 0, true, "", "It won't run even once."});
    cxxLevel2.append({"Which keyword immediately exits a loop?",
                      {"skip", "end", "break", "Exit"}, 2, true, "", "'break' exits immediately."});
    cxxLevel2.append({"Which loop is best when number of iterations is known?",
                      {"while", "do-while", "for", "switch"}, 2, true, "", "Use 'for' for known counts."});
    cxxLevel2.append({"What happens if loop variable is never updated in a while loop?",
                      {"Error", "Infinite loop", "Executes once", "Undefined"}, 1, true, "", "Condition stays true = infinite."});
    cxxLevel2.append({"What does the 'continue' statement do in a loop?",
                      {"Ends the program", "Skips to next iteration", "Restarts the loop", "Exits the loop"}, 1, true, "", "It skips to the next iteration."});
    cxxLevel2.append({"Which of these loops guarantees execution at least once?",
                      {"for", "while", "do-while", "if"}, 2, true, "", "do-while executes before testing."});
    cxxLevel2.append({"What is the output?  for(int i=0;i<3;i++) cout<<i;",
                      {"012", "123", "0 1 2", "1 2 3"}, 0, true, "", "Loops from 0, prints 012."});
    cxxLevel2.append({"Which control statement lets you jump to a labelled part of code?",
                      {"goto", "switch", "exit", "break"}, 0, true, "", "'goto' performs unconditional jumps."});
    cxxLevel2.append({"Which of the following affects flow inside a loop?",
                      {"continue", "break", "goto", "All of the above"}, 3, true, "", "All control flow statements do."});

    // --- 4 coding tasks ---
    cxxLevel2.append({"Write a 'for' loop to print numbers 1 to 10 on separate lines.", {}, -1, false,
                      "for(int i=1;i<=10;i++) cout<<i<<endl;",
                      "Use for(initialization; condition; increment)."});
    cxxLevel2.append({"Write a 'while' loop to sum numbers from 1 to 5 and print total.", {}, -1, false,
                      "int sum=0,i=1; while(i<=5){ sum+=i; i++; } cout<<sum;",
                      "Increment i and accumulate into sum."});
    cxxLevel2.append({"Write a loop that prints even numbers from 2 to 10.", {}, -1, false,
                      "for(int i=2;i<=10;i+=2) cout<<i<<\" \";",
                      "Start from 2, step by 2."});
    cxxLevel2.append({"Write a do-while loop that runs once and prints 'Hello Loop'.", {}, -1, false,
                      "int i=0; do{ cout<<\"Hello Loop\"; i++; }while(i<1);",
                      "Condition checked after body."});

    questions["C++"][2] = cxxLevel2;
    QList<Question> cxxLevel3;

    cxxLevel3.append({"Which keyword defines a class in C++?", {"class", "object", "structure", "define"}, 0, true, "", "'class' creates a class."});
    cxxLevel3.append({"What is encapsulation?", {"Binding data & functions", "Hiding data", "Reusing code", "Overriding"}, 0, true, "", "Encapsulation binds data+methods."});
    cxxLevel3.append({"Which access modifier makes members accessible only within class?", {"public", "protected", "private", "sealed"}, 2, true, "", "private restricts access."});
    cxxLevel3.append({"Which of these shows polymorphism?", {"Function overloading", "Data hiding", "Constructor only", "Operator precedence"}, 0, true, "", "Overloading = compile-time polymorphism."});
    cxxLevel3.append({"Which is a constructor?", {"void main()", "int sum()", "ClassName()", "returnClass()"}, 2, true, "", "Constructor = same name as class."});
    cxxLevel3.append({"Which function is used to destroy objects?", {"delete()", "~ClassName()", "destroy()", "free()"}, 1, true, "", "Destructor starts with '~'."});
    cxxLevel3.append({"What type of inheritance allows one class from many parents?", {"Single", "Multiple", "Multilevel", "Hybrid"}, 1, true, "", "Multiple = many base classes."});
    cxxLevel3.append({"Which of these supports function overriding?", {"Virtual functions", "Static methods", "Inline functions", "Templates"}, 0, true, "", "Virtual enables runtime polymorphism."});
    cxxLevel3.append({"What is abstraction?", {"Showing only essential features", "Binding data", "Overloading", "Inheritance"}, 0, true, "", "It hides details."});
    cxxLevel3.append({"Which operator is overloaded for object copy?", {"operator=", "operator+", "operator*", "operator=="}, 0, true, "", "Assignment operator = copy."});

    // minimal coding
    cxxLevel3.append({"Declare a class named Car with a public int member speed.", {}, -1, false,
                      "class Car { public: int speed; };", "Remember to mark access as public."});
    cxxLevel3.append({"Define a constructor that sets speed to 100.", {}, -1, false,
                      "Car() { speed = 100; }", "Constructors initialize members."});
    cxxLevel3.append({"Write a function display() that prints 'Speed'.", {}, -1, false,
                      "void display() { cout<<\"Speed\"; }", "Function in class prints text."});
    cxxLevel3.append({"Create an object c of class Car and call display().", {}, -1, false,
                      "Car c; c.display();", "Use dot operator for members."});

    questions["C++"][3] = cxxLevel3;
    QList<Question> cxxLevel4;
    cxxLevel4.append({"Which header enables file I/O in C++?", {"<fstream>", "<iostream>", "<stdio.h>", "<file>"}, 0, true, "", "<fstream> contains file streams."});
    cxxLevel4.append({"Which class is used for writing to files?", {"ifstream", "ofstream", "fstream", "istream"}, 1, true, "", "ofstream—output stream."});
    cxxLevel4.append({"Which mode empties existing file content?", {"ios::app", "ios::trunc", "ios::ate", "ios::in"}, 1, true, "", "'trunc' truncates file."});
    cxxLevel4.append({"Which function opens a file?", {"open()", "create()", "start()", "load()"}, 0, true, "", "open() is used with file streams."});
    cxxLevel4.append({"Which function closes a file?", {"shutdown()", "close()", "exit()", "stop()"}, 1, true, "", "close() releases file handle."});
    cxxLevel4.append({"Which class can perform both read/write operations?", {"ifstream", "ofstream", "fstream", "iostream"}, 2, true, "", "fstream supports both."});
    cxxLevel4.append({"Which flag opens file in append mode?", {"ios::in", "ios::app", "ios::out", "ios::trunc"}, 1, true, "", "ios::app appends data."});
    cxxLevel4.append({"Which operator writes data to a file?", {">>", "<<", "=", "~"}, 1, true, "", "Use insertion operator like cout."});
    cxxLevel4.append({"Which type of file stores binary data?", {".txt", ".bin", ".csv", ".doc"}, 1, true, "", "Binary uses .bin convention."});
    cxxLevel4.append({"What happens if file fails to open?", {"Exception", "Error flag set", "Crash", "All"}, 1, true, "", "fail() returns true if open failed."});

    // small code tasks
    cxxLevel4.append({"Write code to open a file named 'data.txt' for writing.", {}, -1, false,
                      "ofstream fout(\"data.txt\");", "Use ofstream constructor."});
    cxxLevel4.append({"Write a statement that writes 'Hello File' to a file stream fout.", {}, -1, false,
                      "fout << \"Hello File\";", "Same syntax as cout."});
    cxxLevel4.append({"Write code to close file fout.", {}, -1, false,
                      "fout.close();", "Always close when done."});
    cxxLevel4.append({"Write code to read from file 'input.txt' and print to console.", {}, -1, false,
                      "ifstream fin(\"input.txt\"); string s; while(getline(fin,s)) cout<<s<<endl;",
                      "Use getline in loop to read lines."});

    questions["C++"][4] = cxxLevel4;

    // === Python Level 1 Questions (15) ===
    QList<Question> pythonLevel1;

    // --- MCQs (11) ---
    pythonLevel1.append({"Which of the following is the correct way to print 'Hello' in Python?",
                         {"print('Hello')", "echo 'Hello'", "cout << 'Hello'", "printf('Hello')"}, 0, true, "", "Use print() in Python."});

    pythonLevel1.append({"Which of the following is a list in Python?",
                         {"[1,2,3]", "{1,2,3}", "(1,2,3)", "<1,2,3>"}, 0, true, "", "Lists use square brackets."});

    pythonLevel1.append({"Which symbol is used for comments in Python?",
                         {"#", "//", "/* */", "--"}, 0, true, "", "Use # for comments."});

    pythonLevel1.append({"What is the output of '2 ** 3' in Python?",
                         {"8", "6", "9", "5"}, 0, true, "", "** is exponentiation in Python."});

    pythonLevel1.append({"Which of these is used to define a function in Python?",
                         {"def", "function", "func", "lambda"}, 0, true, "", "Use def keyword."});

    pythonLevel1.append({"Which of the following is mutable in Python?",
                         {"list", "tuple", "string", "int"}, 0, true, "", "Lists are mutable."});

    pythonLevel1.append({"What is the output of type([1,2,3])?",
                         {"<class 'list'>", "<class 'tuple'>", "<class 'array'>", "<class 'dict'>"}, 0, true, "", "Lists are <class 'list'>."});

    pythonLevel1.append({"Which of the following is used to handle exceptions in Python?",
                         {"try-except", "catch", "handle", "error"}, 0, true, "", "Python uses try-except blocks."});

    pythonLevel1.append({"Which of these is a valid variable name in Python?",
                         {"my_var", "2var", "my-var", "var!"}, 0, true, "", "Variable names cannot start with numbers or have hyphens."});

    pythonLevel1.append({"How do you start a while loop in Python?",
                         {"while condition:", "while (condition)", "while {condition}", "loop while"}, 0, true, "", "Python uses colon to start blocks."});

    pythonLevel1.append({"Which of these converts a string '123' to integer?",
                         {"int('123')", "str(123)", "float('123')", "toInt('123')"}, 0, true, "", "Use int() to convert string to integer."});

    // --- Coding Tasks (4) ---
    pythonLevel1.append({"Write a line to assign 10 to a variable x.", {}, -1, false, "x = 10", "Python uses = to assign values."});

    pythonLevel1.append({"Write a function named greet that prints 'Hello World'.", {}, -1, false,
                         "def greet():\n    print('Hello World')", "Use def and indentation."});

    pythonLevel1.append({"Write code to create a list of numbers 1 to 5.", {}, -1, false,
                         "numbers = [1,2,3,4,5]", "Use square brackets for list."});

    pythonLevel1.append({"Write code to print all elements of a list 'nums'.", {}, -1, false,
                         "for n in nums:\n    print(n)", "Use a for loop with print."});

    questions["Python"][1] = pythonLevel1;

    QList<Question> pythonLevel2;

    // --- MCQs (11) ---
    pythonLevel2.append({"Which loop runs at least once regardless of condition?",
                         {"for", "while", "do-while", "Python has no do-while"}, 3, true, "", "Python does not have do-while; trick question."});

    pythonLevel2.append({"What is the output of: sum([1,2,3,4])?",
                         {"10", "24", "1234", "None"}, 0, true, "", "sum() returns 10."});

    pythonLevel2.append({"Which keyword exits a loop immediately?",
                         {"stop", "break", "exit", "continue"}, 1, true, "", "Use break to exit a loop."});

    pythonLevel2.append({"How many times does a while loop execute if its condition is False?",
                         {"0", "1", "Infinite", "Error"}, 0, true, "", "It does not execute at all."});

    pythonLevel2.append({"Which statement skips the current iteration of a loop?",
                         {"skip", "continue", "pass", "break"}, 1, true, "", "Use continue to skip iteration."});

    pythonLevel2.append({"Which is correct to iterate over a list 'lst'?",
                         {"for x in lst:", "for x of lst:", "foreach lst as x:", "for x lst"}, 0, true, "", "Use 'for x in lst:'."});

    pythonLevel2.append({"Which of these creates an empty list?",
                         {"[]", "()", "{}", "list()"}, 0, true, "", "[] creates empty list; list() also works."});

    pythonLevel2.append({"What is the index of first element in a list?",
                         {"0", "1", "-1", "None"}, 0, true, "", "Python uses 0-based indexing."});

    pythonLevel2.append({"Which method adds an element to the end of a list?",
                         {"append()", "insert()", "add()", "push()"}, 0, true, "", "Use append() for adding at end."});

    pythonLevel2.append({"What is the output of: len([1,2,3,4])?",
                         {"4", "3", "5", "Error"}, 0, true, "", "len() returns number of elements."});

    pythonLevel2.append({"Which function converts a string '123' to integer?",
                         {"int('123')", "str(123)", "float('123')", "number('123')"}, 0, true, "", "int() converts string to integer."});

    // --- Coding tasks (4) ---
    pythonLevel2.append({"Write a for loop to print numbers 1 to 5.", {}, -1, false,
                         "for i in range(1,6):\n    print(i)", "Use range(1,6) in for loop."});

    pythonLevel2.append({"Write a while loop to sum numbers 1 to 5 and print the result.", {}, -1, false,
                         "i=1\nsum=0\nwhile i<=5:\n    sum+=i\n    i+=1\nprint(sum)", "Increment i and accumulate sum."});

    pythonLevel2.append({"Write code to create a list of squares of numbers 1 to 5.", {}, -1, false,
                         "squares = [i**2 for i in range(1,6)]", "Use list comprehension."});

    pythonLevel2.append({"Write code to print only even numbers from list nums.", {}, -1, false,
                         "for n in nums:\n    if n%2==0:\n        print(n)", "Use modulus to check even numbers."});

    questions["Python"][2] = pythonLevel2;

    QList<Question> pythonLevel3;

    // --- MCQs (11) ---
    pythonLevel3.append({"Which keyword is used to define a function?",
                         {"def", "func", "function", "define"}, 0, true, "", "Use def to define a function."});

    pythonLevel3.append({"What is returned by a function that has no return statement?",
                         {"None", "0", "''", "Error"}, 0, true, "", "Functions return None by default."});

    pythonLevel3.append({"How do you import a module named math?",
                         {"import math", "include math", "#import math", "using math"}, 0, true, "", "Use 'import module'."});

    pythonLevel3.append({"Which of these defines a lambda function?",
                         {"lambda x: x+1", "def x():", "fun x():", "function x()"}, 0, true, "", "Use lambda keyword."});

    pythonLevel3.append({"What does *args do in a function?",
                         {"Takes variable number of positional arguments", "Named arguments only", "Keyword-only arguments", "Returns a list"}, 0, true, "", "Use *args for variable args."});

    pythonLevel3.append({"Which of these raises an exception intentionally?",
                         {"raise ValueError", "throw Error", "error()", "except"}, 0, true, "", "Use raise to trigger exceptions."});

    pythonLevel3.append({"How do you check the type of variable x?",
                         {"type(x)", "typeof x", "x.type()", "class(x)"}, 0, true, "", "Use type() function."});

    pythonLevel3.append({"Which statement is used to handle exceptions?",
                         {"try-except", "catch", "handle", "error"}, 0, true, "", "Python uses try-except."});

    pythonLevel3.append({"Which of these correctly opens a file 'data.txt' for reading?",
                         {"open('data.txt','r')", "file('data.txt')", "open('data.txt','w')", "read('data.txt')"}, 0, true, "", "Use open(filename,'r')."});

    pythonLevel3.append({"Which keyword is used to create a class?",
                         {"class", "struct", "object", "define"}, 0, true, "", "Use class keyword."});

    pythonLevel3.append({"What is the output of: bool('')?",
                         {"False", "True", "0", "''"}, 0, true, "", "Empty string is False."});

    // --- Coding tasks (4) ---
    pythonLevel3.append({"Write a function add(a,b) that returns their sum.", {}, -1, false,
                         "def add(a,b):\n    return a+b", "Define function with return statement."});

    pythonLevel3.append({"Write code to read a file 'input.txt' line by line.", {}, -1, false,
                         "with open('input.txt') as f:\n    for line in f:\n        print(line.strip())", "Use with statement and loop."});

    pythonLevel3.append({"Write a class Car with attribute speed initialized to 100.", {}, -1, false,
                         "class Car:\n    def __init__(self):\n        self.speed=100", "Use __init__ constructor."});

    pythonLevel3.append({"Write code to call greet() function and print 'Hello'.", {}, -1, false,
                         "def greet():\n    print('Hello')\ngreet()", "Define function and call it."});

    questions["Python"][3] = pythonLevel3;

    QList<Question> pythonLevel4;

    // --- MCQs (11) ---
    pythonLevel4.append({"Which mode opens a file for writing (overwrite)?",
                         {"w", "r", "a", "x"}, 0, true, "", "Use 'w' to write (overwrite)."});
    pythonLevel4.append({"Which mode opens a file for appending?",
                         {"a", "w", "r", "x"}, 0, true, "", "Use 'a' to append."});
    pythonLevel4.append({"Which function reads entire file as string?",
                         {"read()", "readline()", "readlines()", "open()"}, 0, true, "", "Use read() method."});
    pythonLevel4.append({"Which reads file line by line into a list?",
                         {"readlines()", "read()", "readline()", "split()"}, 0, true, "", "Use readlines() method."});
    pythonLevel4.append({"Which method closes a file?",
                         {"close()", "end()", "stop()", "finish()"}, 0, true, "", "Use close() to close a file."});
    pythonLevel4.append({"Which operator writes to a file object?",
                         {">>", "<<", "=", "+="}, 1, true, "", "Use write() method; trick question for operator analogy."});
    pythonLevel4.append({"Which module is required for file handling?",
                         {"os", "sys", "file", "math"}, 0, true, "", "os module provides file operations."});
    pythonLevel4.append({"What happens if a file does not exist in read mode?",
                         {"Raises exception", "Creates new file", "Returns None", "Error ignored"}, 0, true, "", "Raises FileNotFoundError."});
    pythonLevel4.append({"Which flag opens a binary file for writing?",
                         {"wb", "w", "rb", "ab"}, 0, true, "", "Use 'wb' for binary writing."});
    pythonLevel4.append({"Which function checks if a file exists?",
                         {"os.path.exists()", "file.exists()", "check_file()", "exists()"}, 0, true, "", "Use os.path.exists(path)."});
    pythonLevel4.append({"Which method reads one line from a file?",
                         {"readline()", "readlines()", "read()", "line()"}, 0, true, "", "Use readline() to read single line."});

    // --- Coding tasks (4) ---
    pythonLevel4.append({"Write code to open 'data.txt' for writing.", {}, -1, false,
                         "with open('data.txt','w') as f:\n    pass", "Use with open(filename,'w'):"});

    pythonLevel4.append({"Write 'Hello File' to file object f.", {}, -1, false,
                         "f.write('Hello File')", "Use write() method."});

    pythonLevel4.append({"Close the file f.", {}, -1, false,
                         "f.close()", "Call close() method."});

    pythonLevel4.append({"Read all lines from 'input.txt' and print them.", {}, -1, false,
                         "with open('input.txt') as f:\n    for line in f:\n        print(line.strip())", "Use with and loop."});

    questions["Python"][4] = pythonLevel4;
    QList<Question> cLevel1;

    // --- MCQs (11) ---
    cLevel1.append({"Which header file is required for printf?",
                    {"stdio.h", "stdlib.h", "string.h", "math.h"}, 0, true, "", "Use stdio.h for standard I/O."});

    cLevel1.append({"Which is the correct main function declaration?",
                    {"int main()", "void main()", "main()", "All of these"}, 0, true, "", "int main() is standard."});

    cLevel1.append({"What is the output of: printf(\"%d\", 5+3); ?",
                    {"8", "53", "Error", "None"}, 0, true, "", "5+3 = 8."});

    cLevel1.append({"Which symbol is used for single-line comments?",
                    {"//", "/* */", "#", "--"}, 0, true, "", "Use // for single-line comment."});

    cLevel1.append({"Which is correct way to declare integer x?",
                    {"int x;", "x int;", "integer x;", "var x;"}, 0, true, "", "int x; declares an integer."});

    cLevel1.append({"Which function reads input from user?",
                    {"scanf()", "printf()", "gets()", "puts()"}, 0, true, "", "Use scanf() to read input."});

    cLevel1.append({"Which of these is a valid character literal?",
                    {"'a'", "\"a\"", "a", "`a`"}, 0, true, "", "Use single quotes for char."});

    cLevel1.append({"Which operator is for assignment?",
                    {"=", "==", ":", "+="}, 0, true, "", "Use = for assignment."});

    cLevel1.append({"Which data type is used for floating point numbers?",
                    {"float", "int", "double", "both float and double"}, 3, true, "", "float and double both work."});

    cLevel1.append({"Which function prints a string?",
                    {"printf()", "scanf()", "gets()", "putchar()"}, 0, true, "", "Use printf() to print."});

    cLevel1.append({"What is the default return type of main if not specified?",
                    {"int", "void", "char", "float"}, 0, true, "", "int is default return type."});

    // --- Coding tasks (4) ---
    cLevel1.append({"Write code to print 'Hello World'.", {}, -1, false,
                    "#include <stdio.h>\nint main() {\n    printf(\"Hello World\");\n    return 0;\n}", "Use printf inside main."});

    cLevel1.append({"Declare an integer variable x and initialize to 10.", {}, -1, false,
                    "int x = 10;", "Simple integer declaration."});

    cLevel1.append({"Write code to read integer n from user.", {}, -1, false,
                    "int n;\nscanf(\"%d\", &n);", "Use scanf with &."});

    cLevel1.append({"Write code to print numbers 1 to 5 using for loop.", {}, -1, false,
                    "for(int i=1;i<=5;i++) {\n    printf(\"%d\\n\", i);\n}", "Use for loop from 1 to 5."});

    questions["C"][1] = cLevel1;
    QList<Question> cLevel2;

    // --- MCQs (11) ---
    cLevel2.append({"Which operator is modulus?",
                    {"%", "&", "$", "mod"}, 0, true, "", "Use % for remainder."});

    cLevel2.append({"Which loop executes at least once?",
                    {"do-while", "while", "for", "if"}, 0, true, "", "do-while executes at least once."});

    cLevel2.append({"Which keyword exits a loop immediately?",
                    {"break", "continue", "exit", "stop"}, 0, true, "", "Use break to exit loop."});

    cLevel2.append({"Which skips current iteration of a loop?",
                    {"continue", "break", "skip", "pass"}, 0, true, "", "Use continue to skip iteration."});

    cLevel2.append({"What is the output of: 5/2 ?",
                    {"2", "2.5", "3", "Error"}, 0, true, "", "Integer division truncates."});

    cLevel2.append({"Which is correct if statement?",
                    {"if(x>0) {}", "if x>0 {}", "if x>0 then {}", "if (x>0)"}, 0, true, "", "Use if(condition) {} syntax."});

    cLevel2.append({"Which is correct switch syntax?",
                    {"switch(x){case 1: ... break;}", "switch x case 1:", "switch(x) case 1:", "switch x {1: ...}"}, 0, true, "", "Use switch(x){case...}."});

    cLevel2.append({"Which operator is logical AND?",
                    {"&&", "||", "!", "&"}, 0, true, "", "Use && for logical AND."});

    cLevel2.append({"Which is correct ternary operator usage?",
                    {"x>0?1:0", "if x>0 then 1 else 0", "x>0:1:0", "x?1:0"}, 0, true, "", "Use condition ? true : false."});

    cLevel2.append({"What is the output of: ++x ?",
                    {"x incremented before use", "x incremented after use", "Error", "x unchanged"}, 0, true, "", "Prefix increment increases before use."});

    cLevel2.append({"Which operator compares equality?",
                    {"==", "=", "!=", "==="}, 0, true, "", "Use == for comparison."});

    // --- Coding tasks (4) ---
    cLevel2.append({"Write code to print even numbers from 1 to 10.", {}, -1, false,
                    "for(int i=1;i<=10;i++) {\n    if(i%2==0) printf(\"%d\\n\", i);\n}", "Use modulus and for loop."});

    cLevel2.append({"Write code to calculate factorial of n using while loop.", {}, -1, false,
                    "int fact=1,i=1;\nwhile(i<=n){\n fact*=i;\n i++;\n}\nprintf(\"%d\",fact);", "Use accumulation inside while loop."});

    cLevel2.append({"Write code to use switch to print 'One' for 1, 'Two' for 2.", {}, -1, false,
                    "switch(n){\n case 1: printf(\"One\"); break;\n case 2: printf(\"Two\"); break;\n}", "Use switch and cases."});

    cLevel2.append({"Use ternary operator to assign max of a and b to m.", {}, -1, false,
                    "m = (a>b)?a:b;", "Condition ? value_if_true : value_if_false."});

    questions["C"][2] = cLevel2;
    QList<Question> cLevel3;

    // --- MCQs (11) ---
    cLevel3.append({"Which keyword defines a function?",
                    {"return_type name()", "func name()", "function name()", "void name()"}, 0, true, "", "Use return_type function_name() syntax."});

    cLevel3.append({"What is returned by function with no return?",
                    {"undefined", "0", "None", "Garbage"}, 3, true, "", "Function may return garbage in C."});

    cLevel3.append({"Which is correct array declaration?",
                    {"int arr[5];", "arr int[5];", "int[5] arr;", "array arr[5];"}, 0, true, "", "Use int arr[5];"});

    cLevel3.append({"How do you access 3rd element of array arr?",
                    {"arr[2]", "arr[3]", "arr(3)", "arr{3}"}, 0, true, "", "C uses 0-based indexing."});

    cLevel3.append({"Which passes variable by reference?",
                    {"Use pointers", "Use & operator in declaration", "Both", "None"}, 2, true, "", "Use pointers or &."});

    cLevel3.append({"Which is correct function call?",
                    {"func(a,b);", "call func(a,b);", "func a,b;", "func->(a,b);"}, 0, true, "", "Call function by name with arguments."});

    cLevel3.append({"What is the output of: sizeof(int)?",
                    {"4", "Depends on system", "2", "1"}, 1, true, "", "Size depends on system (commonly 4 bytes)."});

    cLevel3.append({"Which is correct to pass array to function?",
                    {"func(arr, size);", "func(&arr, size);", "func arr[];", "func arr;"}, 0, true, "", "Pass array and its size."});

    cLevel3.append({"Which header is needed for malloc?",
                    {"stdlib.h", "stdio.h", "malloc.h", "string.h"}, 0, true, "", "Use stdlib.h for malloc."});

    cLevel3.append({"Which operator accesses pointer value?",
                    {"*", "&", "->", "."}, 0, true, "", "Use * to dereference pointer."});

    cLevel3.append({"Which operator accesses structure member via pointer?",
                    {"->", ".", "*", "&"}, 0, true, "", "Use -> for pointer to struct."});

    // --- Coding tasks (4) ---
    cLevel3.append({"Write a function add(a,b) that returns sum.", {}, -1, false,
                    "int add(int a,int b){ return a+b; }", "Return a+b inside function."});

    cLevel3.append({"Write code to declare int arr[5] and initialize to 1,2,3,4,5.", {}, -1, false,
                    "int arr[5]={1,2,3,4,5};", "Initialize array with braces."});

    cLevel3.append({"Write function to print array of size n.", {}, -1, false,
                    "void printArr(int arr[],int n){\n for(int i=0;i<n;i++) printf(\"%d \",arr[i]);\n}", "Use for loop to print array."});

    cLevel3.append({"Write code to sum elements of array arr of size n.", {}, -1, false,
                    "int sum=0;\nfor(int i=0;i<n;i++) sum+=arr[i];", "Accumulate sum in loop."});

    questions["C"][3] = cLevel3;
    QList<Question> cLevel4;

    // --- MCQs (11) ---
    cLevel4.append({"Which operator gives address of variable?",
                    {"&", "*", "->", "%"}, 0, true, "", "Use & to get address."});

    cLevel4.append({"Which operator dereferences a pointer?",
                    {"*", "&", "->", "."}, 0, true, "", "Use * to access value at pointer."});

    cLevel4.append({"Which library is required for string functions?",
                    {"string.h", "stdlib.h", "stdio.h", "ctype.h"}, 0, true, "", "Use string.h for strcpy, strlen, etc."});

    cLevel4.append({"Which function copies string src to dest?",
                    {"strcpy(dest,src)", "strcopy(dest,src)", "strcpy(src,dest)", "copy(dest,src)"}, 0, true, "", "Use strcpy(dest,src)."});

    cLevel4.append({"Which function gets length of string str?",
                    {"strlen(str)", "size(str)", "length(str)", "strsize(str)"}, 0, true, "", "Use strlen(str)."});

    cLevel4.append({"Which function concatenates two strings?",
                    {"strcat(dest,src)", "concat(dest,src)", "strappend(dest,src)", "append(dest,src)"}, 0, true, "", "Use strcat(dest,src)."});

    cLevel4.append({"Which reads a string from input safely?",
                    {"fgets(str,size,stdin)", "gets(str)", "scanf(%s,str)", "read(str)"}, 0, true, "", "Use fgets to avoid buffer overflow."});

    cLevel4.append({"Which compares two strings?",
                    {"strcmp(str1,str2)", "strcomp(str1,str2)", "str==str2", "compare(str1,str2)"}, 0, true, "", "Use strcmp for comparison."});

    cLevel4.append({"Which converts string to integer?",
                    {"atoi(str)", "strtoi(str)", "int(str)", "toint(str)"}, 0, true, "", "Use atoi(str)."});

    cLevel4.append({"Which allocates memory dynamically?",
                    {"malloc()", "alloc()", "new", "calloc"}, 0, true, "", "Use malloc() or calloc()."});

    cLevel4.append({"Which frees allocated memory?",
                    {"free(ptr)", "delete ptr", "release(ptr)", "memfree(ptr)"}, 0, true, "", "Use free(ptr)."});

    // --- Coding tasks (4) ---
    cLevel4.append({"Write code to declare int *p and assign address of x.", {}, -1, false,
                    "int *p = &x;", "Use & to assign address to pointer."});

    cLevel4.append({"Write code to print value pointed by p.", {}, -1, false,
                    "printf(\"%d\", *p);", "Use * to dereference pointer."});

    cLevel4.append({"Write code to copy string src to dest.", {}, -1, false,
                    "strcpy(dest, src);", "Use strcpy(dest,src)."});

    cLevel4.append({"Write code to allocate int array of size n dynamically.", {}, -1, false,
                    "int *arr = (int*)malloc(n*sizeof(int));", "Use malloc and cast to int*."});

    questions["C"][4] = cLevel4;

    QList<Question> javaLevel1;

    // --- MCQs (11) ---
    javaLevel1.append({"Which is correct main method declaration in Java?",
                       {"public static void main(String[] args)", "void main(String[] args)", "public void main()", "static main(String args[])"},
                       0, true, "", "Use public static void main(String[] args)."});

    javaLevel1.append({"Which keyword is used to define a class?",
                       {"class", "struct", "function", "object"}, 0, true, "", "Use class keyword."});

    javaLevel1.append({"Which is used for single-line comments?",
                       {"//", "/* */", "#", "--"}, 0, true, "", "Use // for single-line comment."});

    javaLevel1.append({"Which prints output to console?",
                       {"System.out.println()", "printf()", "cout", "echo"}, 0, true, "", "Use System.out.println() in Java."});

    javaLevel1.append({"Which is a valid integer declaration?",
                       {"int x = 10;", "integer x = 10;", "var x = 10;", "num x=10;"}, 0, true, "", "Use int x=10;"});

    javaLevel1.append({"Which data type is for decimal numbers?",
                       {"double", "int", "char", "boolean"}, 0, true, "", "Use double or float for decimals."});

    javaLevel1.append({"Which operator is assignment?",
                       {"=", "==", "+=", "!="}, 0, true, "", "Use = to assign values."});

    javaLevel1.append({"Which keyword makes a variable constant?",
                       {"final", "const", "static", "immutable"}, 0, true, "", "Use final keyword."});

    javaLevel1.append({"Which operator compares equality?",
                       {"==", "=", "!=", "==="}, 0, true, "", "Use == for comparison."});

    javaLevel1.append({"Which function converts string to int?",
                       {"Integer.parseInt(str)", "str.toInt()", "int(str)", "parseInt(str)"}, 0, true, "", "Use Integer.parseInt(str)."});

    javaLevel1.append({"Which is the default value of int in Java?",
                       {"0", "null", "undefined", "1"}, 0, true, "", "int variables default to 0."});

    // --- Coding tasks (4) ---
    javaLevel1.append({"Write code to print 'Hello World'.", {}, -1, false,
                       "public class Main {\n public static void main(String[] args) {\n  System.out.println(\"Hello World\");\n }\n}", "Print using System.out.println inside main."});

    javaLevel1.append({"Declare an integer x and assign 10.", {}, -1, false,
                       "int x = 10;", "Simple int declaration."});

    javaLevel1.append({"Read integer input from user.", {}, -1, false,
                       "Scanner sc = new Scanner(System.in);\nint n = sc.nextInt();", "Use Scanner to read integer."});

    javaLevel1.append({"Print numbers 1 to 5 using for loop.", {}, -1, false,
                       "for(int i=1;i<=5;i++) {\n System.out.println(i);\n}", "Use for loop."});

    questions["Java"][1] = javaLevel1;
    QList<Question> javaLevel2;

    // --- MCQs (11) ---
    javaLevel2.append({"Which operator is modulus?",
                       {"%", "/", "*", "mod"}, 0, true, "", "Use % for remainder."});

    javaLevel2.append({"Which loop executes at least once?",
                       {"do-while", "while", "for", "if"}, 0, true, "", "Use do-while."});

    javaLevel2.append({"Which keyword exits loop immediately?",
                       {"break", "continue", "stop", "exit"}, 0, true, "", "Use break."});

    javaLevel2.append({"Which skips current iteration?",
                       {"continue", "break", "skip", "pass"}, 0, true, "", "Use continue."});

    javaLevel2.append({"Output of 5/2 in Java?", {"2", "2.5", "3", "Error"}, 0, true, "", "Integer division truncates to 2."});

    javaLevel2.append({"Which is correct if statement?", {"if(x>0) {}", "if x>0 {}", "if x>0 then {}", "if (x>0)"}, 0, true, "", "Use if(condition) { }"});

    javaLevel2.append({"Which is correct switch syntax?", {"switch(x){ case 1: ... break; }", "switch x case 1:", "switch(x) case 1:", "switch x {1: ... }"}, 0, true, "", "Use switch(x){case ...}."});

    javaLevel2.append({"Logical AND operator?", {"&&", "||", "!", "&"}, 0, true, "", "Use && for logical AND."});

    javaLevel2.append({"Ternary operator usage?", {"x>0?1:0", "if x>0 then 1 else 0", "x>0:1:0", "x?1:0"}, 0, true, "", "Use condition ? true : false."});

    javaLevel2.append({"Prefix increment ++x does what?", {"increments before use", "increments after use", "error", "unchanged"}, 0, true, "", "Prefix increases before use."});

    javaLevel2.append({"Equality operator?", {"==", "=", "!=", "==="}, 0, true, "", "Use == to compare."});

    // --- Coding tasks (4) ---
    javaLevel2.append({"Print even numbers 1-10.", {}, -1, false,
                       "for(int i=1;i<=10;i++) {\n if(i%2==0) System.out.println(i);\n}", "Use modulus and for loop."});

    javaLevel2.append({"Factorial of n using while loop.", {}, -1, false,
                       "int fact=1,i=1;\nwhile(i<=n){ fact*=i; i++; }\nSystem.out.println(fact);", "Accumulate factorial inside while loop."});

    javaLevel2.append({"Switch to print One for 1, Two for 2.", {}, -1, false,
                       "switch(n){ case 1: System.out.println(\"One\"); break;\n case 2: System.out.println(\"Two\"); break;}", "Use switch cases."});

    javaLevel2.append({"Ternary operator to assign max of a,b to m.", {}, -1, false,
                       "int m = (a>b)?a:b;", "Use ternary operator."});

    questions["Java"][2] = javaLevel2;
    QList<Question> javaLevel3;

    // --- MCQs (11) ---
    javaLevel3.append({"Keyword to define function in Java?", {"void", "func", "function", "def"}, 0, true, "", "Use return_type function_name()"});

    javaLevel3.append({"Return type of method with no return?", {"void", "null", "undefined", "int"}, 0, true, "", "Use void."});

    javaLevel3.append({"Array declaration for 5 ints?", {"int arr[] = new int[5];", "int[5] arr;", "arr int[5];", "array arr[5];"}, 0, true, "", "Use int arr[] = new int[5];"});

    javaLevel3.append({"Access 3rd element?", {"arr[2]", "arr[3]", "arr(3)", "arr{3}"}, 0, true, "", "Arrays are 0-indexed."});

    javaLevel3.append({"Pass by reference in Java?", {"Use objects", "Use primitive", "Both", "None"}, 0, true, "", "Objects are passed by reference."});

    javaLevel3.append({"Correct function call?", {"func(a,b);", "call func(a,b);", "func a,b;", "func->(a,b);"}, 0, true, "", "Call by name with args."});

    javaLevel3.append({"Length of int array arr?", {"arr.length", "length(arr)", "size(arr)", "arr.size()"}, 0, true, "", "Use arr.length."});

    javaLevel3.append({"Correct way to pass array to function?", {"func(arr);", "func(&arr);", "func arr[];", "func arr;"}, 0, true, "", "Just pass array reference."});

    javaLevel3.append({"Which header needed for arrays?", {"No header needed", "java.util.*", "java.array.*", "stdio.h"}, 0, true, "", "Arrays are native in Java."});

    javaLevel3.append({"Operator to access object member?", {".", "->", "*", "&"}, 0, true, "", "Use dot operator."});

    javaLevel3.append({"Which operator for instanceof?", {"instanceof", "isa", "typeof", "is"}, 0, true, "", "Use instanceof to check type."});

    // --- Coding tasks (4) ---
    javaLevel3.append({"Function add(a,b) returns sum.", {}, -1, false,
                       "int add(int a,int b){ return a+b; }", "Return sum."});

    javaLevel3.append({"Declare int array[5] initialized to 1-5.", {}, -1, false,
                       "int arr[] = {1,2,3,4,5};", "Use braces."});

    javaLevel3.append({"Print array of size n.", {}, -1, false,
                       "for(int i=0;i<n;i++) System.out.print(arr[i]+\" \");", "Use for loop."});

    javaLevel3.append({"Sum elements of array arr.", {}, -1, false,
                       "int sum=0;\nfor(int i=0;i<n;i++) sum+=arr[i];", "Accumulate sum."});

    questions["Java"][3] = javaLevel3;
    QList<Question> javaLevel4;

    // --- MCQs (11) ---
    javaLevel4.append({"Keyword for inheritance?", {"extends", "implements", "inherits", "super"}, 0, true, "", "Use extends for class inheritance."});

    javaLevel4.append({"Keyword for interface implementation?", {"implements", "extends", "interface", "super"}, 0, true, "", "Use implements."});

    javaLevel4.append({"Which method compares strings?", {"equals()", "==", "compare()", "strcmp()"}, 0, true, "", "Use equals() for string comparison."});

    javaLevel4.append({"Length of string str?", {"str.length()", "str.size()", "str.len()", "str.length"}, 0, true, "", "Use str.length()."});

    javaLevel4.append({"Concatenate two strings?", {"str1 + str2", "concat(str1,str2)", "str1.append(str2)", "append(str1,str2)"}, 0, true, "", "Use + operator or concat() method."});

    javaLevel4.append({"Which reads string from console?", {"Scanner sc = new Scanner(System.in); sc.nextLine();", "gets()", "readLine()", "System.read()"}, 0, true, "", "Use Scanner."});

    javaLevel4.append({"Convert string to integer?", {"Integer.parseInt(str)", "str.toInt()", "int(str)", "parseInt(str)"}, 0, true, "", "Use Integer.parseInt."});

    javaLevel4.append({"Allocate array of n integers?", {"int[] arr = new int[n];", "int arr[n];", "int arr[] = new int(n);", "array arr = new int[n];"}, 0, true, "", "Use new int[n]."});

    javaLevel4.append({"Free allocated memory?", {"Garbage collector handles it", "free(arr);", "delete arr;", "release(arr);"}, 0, true, "", "Java GC handles memory."});

    javaLevel4.append({"Which keyword accesses parent class method?", {"super", "parent", "base", "this"}, 0, true, "", "Use super."});

    javaLevel4.append({"Keyword to define constant?", {"final", "const", "immutable", "static"}, 0, true, "", "Use final."});

    // --- Coding tasks (4) ---
    javaLevel4.append({"Declare String s = 'Hello' and print it.", {}, -1, false,
                       "String s = \"Hello\";\nSystem.out.println(s);", "Simple String declaration."});

    javaLevel4.append({"Create class Person with name and age.", {}, -1, false,
                       "class Person {\n String name;\n int age;\n}", "Use class with member variables."});

    javaLevel4.append({"Write method greet() that prints 'Hi'.", {}, -1, false,
                       "void greet(){ System.out.println(\"Hi\"); }", "Method prints Hi."});

    javaLevel4.append({"Concatenate two strings s1 and s2.", {}, -1, false,
                       "String s3 = s1 + s2;", "Use + operator."});

    questions["Java"][4] = javaLevel4;

    // prepare storage vectors
    int totalQ = questions["C++"][1].size(); // should be 15
    userMCQAnswers = QVector<int>(totalQ, -1);
    userCodeAnswers = QVector<QString>(totalQ, QString());

    // === Language Button Connections (same as before) ===
    connect(ui->cppButton, &QPushButton::clicked, this, [this]() {
        selectedLanguage = "C++";
        ui->stackedWidget->setCurrentIndex(2);
        updateLevelLockStatusUI();
    });
    connect(ui->cButton, &QPushButton::clicked, this, [this]() {
        selectedLanguage = "C";
        ui->stackedWidget->setCurrentIndex(2);
        updateLevelLockStatusUI();
    });
    connect(ui->pythonButton, &QPushButton::clicked, this, [this]() {
        selectedLanguage = "Python";
        ui->stackedWidget->setCurrentIndex(2);
        updateLevelLockStatusUI();
    });
    connect(ui->javaButton, &QPushButton::clicked, this, [this]() {
        selectedLanguage = "Java";
        ui->stackedWidget->setCurrentIndex(2);
        updateLevelLockStatusUI();
    });

    // ------------------------
    // Dynamic Level Button Connections (works for all languages)
    // ------------------------
    connect(ui->level1Button, &QPushButton::clicked, this, [this]() {
        qDebug() << "Level 1 button clicked";
        openLevel(1);
    });
    connect(ui->level2Button, &QPushButton::clicked, this, [this]() {
        qDebug() << "Level 2 button clicked";
        openLevel(2);
    });
    connect(ui->level3Button, &QPushButton::clicked, this, [this]() {
        qDebug() << "Level 3 button clicked";
        openLevel(3);
    });
    connect(ui->level4Button, &QPushButton::clicked, this, [this]() {
        qDebug() << "Level 4 button clicked";
        openLevel(4);
    });


    // Back to language page
    connect(ui->backButtonLevels, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    // other nav
    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    // Start with home
    ui->stackedWidget->setCurrentIndex(0);

    // ensure hint UI is up-to-date
    updateHintCountUI();
    setupInstructionsPage();

    // ===== SIMPLE CONNECTION FOR UI-BASED BUTTON =====
    connect(ui->leaderboardButton, &QPushButton::clicked, this, &MainWindow::showLeaderboard);
    setupLeaderboardPage();

    // open sqlite DB and init tables
    if (!Database::instance().open()) {
        qDebug() << "Failed to open database file";
    } else {
        Database::instance().createLeaderboardTable();
    }
}
void MainWindow::setupInstructionsPage()
{
    // Clear any existing layout
    if (ui->instructionsPage->layout()) {
        delete ui->instructionsPage->layout();
    }

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->instructionsPage);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title with glow effect
    QLabel *titleLabel = new QLabel("🚀 CODE QUEST ");
    titleLabel->setStyleSheet(
        "color: #00FF00; "
        "font-size: 36px; "
        "font-family: 'Courier New'; "
        "font-weight: bold; "
        "text-align: center; "
        "padding: 20px; "
        "background-color: #001a00; "
        "border: 2px solid #00FF00; "
        "text-shadow: 0 0 10px #00FF00; "
        "margin-bottom: 10px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    QLabel *subtitleLabel = new QLabel(">>> SYSTEM INSTRUCTIONS INITIALIZED <<<");
    subtitleLabel->setStyleSheet(
        "color: #00FFFF; "
        "font-size: 18px; "
        "font-family: 'Courier New'; "
        "text-align: center; "
        "padding: 10px; "
        "margin-bottom: 20px;"
        );
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Scroll area for content
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "   border: 2px solid #00FF00;"
        "   background-color: #000000;"
        "}"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: #001100;"
        "   width: 15px;"
        "   margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #00FF00;"
        "   min-height: 20px;"
        "   border-radius: 7px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: #00CC00;"
        "}"
        );

    // Content widget
    QWidget *contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #000000;");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setAlignment(Qt::AlignTop);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(25, 25, 25, 25);

    // Instructions content with HTML formatting
    QString instructionsHTML = R"(
        <div style="color: #00FF00; font-family: 'Courier New'; font-size: 15px; line-height: 1.6;">

        <div style="color: #00FFFF; font-size: 20px; font-weight: bold; text-align: center; margin-bottom: 25px; text-shadow: 0 0 8px #00FFFF;">
        ╔═══════════════════════════════╗<br>
        ║        MISSION PROTOCOL       ║<br>
        ╚═══════════════════════════════╝
        </div>

        <table style="width: 100%; border-collapse: collapse;">
        <tr>
            <td style="color: #FFFF00; font-weight: bold; vertical-align: top; width: 200px;">▶ PROGRAM SELECT</td>
            <td style="color: #00FF00;">Choose from C++, Python, C, or Java programming languages</td>
        </tr>
        <tr><td colspan="2" style="height: 10px;"></td></tr>
        <tr>
            <td style="color: #FFFF00; font-weight: bold; vertical-align: top;">▶ LEVEL ACCESS</td>
            <td style="color: #00FF00;">Begin with Level 1. Unlock Level 2 with score ≥ 10/15</td>
        </tr>
        <tr><td colspan="2" style="height: 10px;"></td></tr>
        <tr>
            <td style="color: #FFFF00; font-weight: bold; vertical-align: top;">▶ QUESTION TYPES</td>
            <td style="color: #00FF00;">
                <span style="color: #00FFFF;">[MCQ]</span> Click correct option<br>
                <span style="color: #00FFFF;">[CODE]</span> Type solution in terminal box
            </td>
        </tr>
        </table>

        <div style="margin: 25px 0; border-top: 1px dashed #00FF00; padding-top: 20px;">
            <div style="color: #FF00FF; font-size: 18px; font-weight: bold; margin-bottom: 15px;">
            💡 HINT SUBROUTINE
            </div>
            <table style="width: 100%;">
            <tr>
                <td style="color: #00FF00; padding: 5px;">• Initial allocation:</td>
                <td style="color: #FFFF00;">5 hints/level</td>
            </tr>
            <tr>
                <td style="color: #00FF00; padding: 5px;">• Bonus matrix:</td>
                <td style="color: #FFFF00;">
                    Score 10-11: +1 hint<br>
                    Score 12: +2 hints<br>
                    Score 13-14: +3 hints<br>
                    Score 15: +5 hints
                </td>
            </tr>
            <tr>
                <td style="color: #00FF00; padding: 5px;">• Carryover:</td>
                <td style="color: #FFFF00;">Unused hints transfer to next level</td>
            </tr>
            </table>
        </div>

        <div style="margin: 25px 0; border-top: 1px dashed #00FF00; padding-top: 20px;">
            <div style="color: #00FFFF; font-size: 18px; font-weight: bold; margin-bottom: 15px;">
            🏆 SCORING MATRIX
            </div>
            <div style="background: #001100; padding: 15px; border: 1px solid #00FF00; border-radius: 5px;">
            <table style="width: 100%;">
            <tr>
                <td style="color: #00FF00; padding: 8px;">• Correct answer:</td>
                <td style="color: #FFFF00;">+1 point</td>
            </tr>
            <tr>
                <td style="color: #00FF00; padding: 8px;">• Level completion:</td>
                <td style="color: #FFFF00;">10+ points required</td>
            </tr>
            <tr>
                <td style="color: #00FF00; padding: 8px;">• Questions/level:</td>
                <td style="color: #FFFF00;">15 (11 MCQs + 4 Code)</td>
            </tr>
            <tr>
                <td style="color: #00FF00; padding: 8px;">• Code evaluation:</td>
                <td style="color: #FFFF00;">Case & space insensitive</td>
            </tr>
            </table>
            </div>
        </div>

        <div style="margin: 25px 0; border-top: 1px dashed #00FF00; padding-top: 20px;">
            <div style="color: #FF00FF; font-size: 18px; font-weight: bold; margin-bottom: 15px;">
            🎮 CONTROL SCHEMA
            </div>
            <table style="width: 100%; background: #001100; padding: 10px; border: 1px solid #00FF00;">
            <tr>
                <td style="color: #FFFF00; padding: 10px; width: 120px;">[START]</td>
                <td style="color: #00FF00;">Initialize mission sequence</td>
            </tr>
            <tr>
                <td style="color: #FFFF00; padding: 10px;">[NEXT/BACK]</td>
                <td style="color: #00FF00;">Navigate question queue</td>
            </tr>
            <tr>
                <td style="color: #FFFF00; padding: 10px;">[HINT]</td>
                <td style="color: #00FF00;">Request assistance protocol</td>
            </tr>
            <tr>
                <td style="color: #FFFF00; padding: 10px;">[FINISH]</td>
                <td style="color: #00FF00;">Execute scoring subroutine</td>
            </tr>
            </table>
        </div>

        <div style="text-align: center; margin: 30px 0; padding: 20px; background: #001a00; border: 2px solid #00FF00; border-radius: 8px;">
            <div style="color: #FFFF00; font-size: 16px; font-weight: bold; margin-bottom: 10px;">
            ⚠ SYSTEM READY
            </div>
            <div style="color: #00FF00; font-size: 14px;">
            Terminal initialized. Awaiting user command...
            </div>
        </div>

        </div>
    )";

    QLabel *instructionsLabel = new QLabel(instructionsHTML);
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setAlignment(Qt::AlignTop);
    instructionsLabel->setTextFormat(Qt::RichText);

    contentLayout->addWidget(instructionsLabel);
    scrollArea->setWidget(contentWidget);

    // Back button with terminal style
    QPushButton *backButton = new QPushButton(">>> EXIT TO MAIN TERMINAL ");
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #000;"
        "   color: #00FF00;"
        "   font-family: 'Courier New';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: 2px solid #00FF00;"
        "   padding: 12px 25px;"
        "   margin: 15px;"
        "   text-align: center;"
        "   text-shadow: 0 0 8px #00FF00, 0 0 12px #00FF00;"  // Added glowing text effect
        "}"
        "QPushButton:hover {"
        "   background-color: #004400;"
        "   color: #FFFFFF;"
        "   border: 2px solid #00FFFF;"
        "   text-shadow: 0 0 10px #FFFFFF;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #006600;"
        "   border: 2px solid #00FF00;"
        "}"
        );
    backButton->setFixedSize(300, 50);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    // Add everything to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
}

// Call this in your constructor:
// setupInstructionsPage();

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (backgroundLabel) backgroundLabel->setGeometry(this->rect());
    if (overlay) overlay->setGeometry(this->rect());
}

void MainWindow::on_startButton_clicked() {
    ui->stackedWidget->setCurrentIndex(1); // proceed to next page
}


void MainWindow::on_instructionsButton_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_backButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_exitButton_clicked() { close(); }

// When an option in the group is clicked, store the answer (no popups)
void MainWindow::onOptionGroupClicked(int id)
{
    const QList<Question> &qList = questions[selectedLanguage][currentLevel];
    if (currentQuestionIndex < 0 || currentQuestionIndex >= qList.size()) return;
    const Question &q = qList[currentQuestionIndex];
    if (!q.isMCQ) return;

    userMCQAnswers[currentQuestionIndex] = id;
    refreshOptionButtonsUI();
}

// Refresh visual check states for options
void MainWindow::refreshOptionButtonsUI()
{
    int sel = -1;
    if (currentQuestionIndex >= 0 && currentQuestionIndex < userMCQAnswers.size())
        sel = userMCQAnswers[currentQuestionIndex];

    // set checked states (buttonGroup will enforce exclusivity)
    QAbstractButton *b0 = optionGroup->button(0);
    QAbstractButton *b1 = optionGroup->button(1);
    QAbstractButton *b2 = optionGroup->button(2);
    QAbstractButton *b3 = optionGroup->button(3);

    if (b0) b0->setChecked(sel == 0);
    if (b1) b1->setChecked(sel == 1);
    if (b2) b2->setChecked(sel == 2);
    if (b3) b3->setChecked(sel == 3);
}

// store code answer if any before navigating away
void MainWindow::checkAndStoreCurrentAnswerBeforeNavigate()
{
    const QList<Question> &qList = questions[selectedLanguage][currentLevel];
    if (currentQuestionIndex < 0 || currentQuestionIndex >= qList.size()) return;

    const Question &q = qList[currentQuestionIndex];
    if (!q.isMCQ) {
        QString typed = ui->codeAnswerBox->toPlainText().trimmed();
        userCodeAnswers[currentQuestionIndex] = typed;
    }
}
void MainWindow::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/programming_puzzle.db");


    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        return;
    }

    // Create table for user scores if not exists
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS user_scores (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            language TEXT,
            level INTEGER,
            score INTEGER
        )
    )");
}
void MainWindow::setupLeaderboardPage() {
    // Create leaderboard page
    QWidget *leaderboardPage = new QWidget();
    leaderboardPage->setObjectName("leaderboardPage");
    QVBoxLayout *layout = new QVBoxLayout(leaderboardPage);

    // Title
    QLabel *titleLabel = new QLabel("🏆 CODE QUEST LEADERBOARD 🏆");
    titleLabel->setObjectName("leaderboardTitle");
    titleLabel->setStyleSheet(R"(
        #leaderboardTitle {
            color: #FFD700;
            font-size: 48px;
            font-family: 'Courier New';
            font-weight: bold;
            text-align: center;
            text-shadow: 0 0 20px rgba(255, 215, 0, 0.9);
            padding: 20px;
            margin: 20px 0;
        }
    )");

    // Filter buttons
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QPushButton *allButton = new QPushButton("ALL LANGUAGES");
    QPushButton *cppButton = new QPushButton("C++");
    QPushButton *pythonButton = new QPushButton("PYTHON");
    QPushButton *cButton = new QPushButton("C");
    QPushButton *javaButton = new QPushButton("JAVA");

    // Reusing the filter style template from the QSS block (good practice)
    QString filterButtonStyle = R"(
        QPushButton {
            background: rgba(0, 40, 0, 0.7);
            color: #00FF00;
            border: 2px solid #00FF00;
            padding: 10px 15px;
            margin: 5px;
            font-family: 'Courier New';
            font-weight: bold;
            font-size: 16px;
            min-width: 100px;
        }
        QPushButton:hover {
            background: rgba(0, 80, 0, 0.9);
            border: 2px solid #00FFFF;
        }
        QPushButton:checked {
            background: rgba(0, 120, 0, 1);
            border: 2px solid #FFD700;
            color: #FFD700;
        }
    )";

    allButton->setStyleSheet(filterButtonStyle);
    cppButton->setStyleSheet(filterButtonStyle);
    pythonButton->setStyleSheet(filterButtonStyle);
    cButton->setStyleSheet(filterButtonStyle);
    javaButton->setStyleSheet(filterButtonStyle);

    allButton->setCheckable(true);
    cppButton->setCheckable(true);
    pythonButton->setCheckable(true);
    cButton->setCheckable(true);
    javaButton->setCheckable(true);
    allButton->setChecked(true);

    filterLayout->addWidget(allButton);
    filterLayout->addWidget(cppButton);
    filterLayout->addWidget(pythonButton);
    filterLayout->addWidget(cButton);
    filterLayout->addWidget(javaButton);
    filterLayout->addStretch();

    // Leaderboard table setup
    QTableWidget *table = new QTableWidget();
    table->setObjectName("leaderboardTable");
    table->setShowGrid(true);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->verticalHeader()->setVisible(false); // Hide row numbers

    // APPLY STRETCH MODE HERE TO FIX WIDTH CUTOFF
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    table->setStyleSheet(R"(
        #leaderboardTable {
            background: rgba(0, 20, 0, 0.8);
            border: 3px solid #00FF00;
            color: #00FF00;
            font-family: 'Courier New';
            gridline-color: #00FF00; /* Use the neon green for grid lines */
            border-radius: 10px;
            selection-background-color: rgba(0, 100, 0, 0.7);
        }

        /* Style for individual data cells (items) */
        #leaderboardTable::item {
            padding: 10px;
            border: 1px solid #00FF00; /* Define border around cells */
            font-size: 16px;
        }

        /* Style for the horizontal header (column titles) */
        QHeaderView::section {
            background: rgba(0, 50, 0, 1); /* Distinct background for headers */
            color: #FFD700; /* Gold color for text */
            padding: 15px 10px;
            border: 2px solid #00FF00; /* Stronger border for header separation */
            border-bottom-width: 3px;
            font-weight: bold;
            font-family: 'Courier New';
            font-size: 18px;
        }

        /* Hide the vertical (row) header QSS too, just in case */
        QHeaderView::vertical {
            border: none;
            width: 0px;
        }
    )");

    // Back button setup (using QSS from the global block)
    QPushButton *backButton = new QPushButton("← BACK TO MAIN MENU");
    // Explicitly using the red exit style for the back button on the leaderboard
    backButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                         stop:0 rgba(60, 0, 0, 0.9),
                                         stop:0.5 rgba(100, 0, 0, 1),
                                         stop:1 rgba(40, 0, 0, 0.9));
            color: #FF4444;
            font-size: 18px;
            font-family: 'Courier New';
            font-weight: bold;
            border: 3px solid #FF4444;
            border-radius: 10px;
            padding: 15px 25px;
            margin: 20px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                         stop:0 rgba(100, 0, 0, 1),
                                         stop:0.5 rgba(140, 0, 0, 1),
                                         stop:1 rgba(80, 0, 0, 1));
            border: 3px solid #FF8888;
        }
    )");

    layout->addWidget(titleLabel, 0, Qt::AlignCenter); // Centered title
    layout->addLayout(filterLayout);
    layout->addWidget(table);
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    // Add the new page to the stacked widget (assuming ui->stackedWidget is accessible)
    // IMPORTANT: Make sure this widget is correctly added to your UI!
    ui->stackedWidget->addWidget(leaderboardPage);

    // Connect filter buttons (connects remain the same)
    auto updateTable = [this, table](const QString &language) {
        table->clear();
        table->setRowCount(0);

        QVector<QVector<QVariant>> data = Database::instance().getLeaderboard(language);

        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({"Rank", "Player", "Language", "Total Score", "Levels Completed"});

        // This line needs to be repeated in updateTable to ensure stretching persists
        // after content updates, even though it's set above.
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for (int i = 0; i < data.size(); ++i) {
            table->insertRow(i);
            QString rankText;
            if (i == 0) rankText = "🥇 " + QString::number(i + 1);
            else if (i == 1) rankText = "🥈 " + QString::number(i + 1);
            else if (i == 2) rankText = "🥉 " + QString::number(i + 1);
            else rankText = QString::number(i + 1);

            table->setItem(i, 0, new QTableWidgetItem(rankText));
            table->setItem(i, 1, new QTableWidgetItem(data[i][0].toString()));
            table->setItem(i, 2, new QTableWidgetItem(data[i][1].toString()));
            table->setItem(i, 3, new QTableWidgetItem(data[i][2].toString()));
            table->setItem(i, 4, new QTableWidgetItem(data[i][3].toString()));
        }
    };

    // ... (Connections and initial updateTable call remain the same) ...
    connect(allButton, &QPushButton::clicked, [updateTable]() { updateTable(""); });
    connect(cppButton, &QPushButton::clicked, [updateTable]() { updateTable("C++"); });
    connect(pythonButton, &QPushButton::clicked, [updateTable]() { updateTable("Python"); });
    connect(cButton, &QPushButton::clicked, [updateTable]() { updateTable("C"); });
    connect(javaButton, &QPushButton::clicked, [updateTable]() { updateTable("Java"); });
    connect(backButton, &QPushButton::clicked, [this]() { ui->stackedWidget->setCurrentIndex(0); });

    updateTable(""); // Initial load
}
void MainWindow::showLeaderboard() {
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count() - 1); // Last added widget is leaderboard
}
void MainWindow::saveUserScore(int level, int score, const QString &language) {
    if (userName.isEmpty()) return; // safety check

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO user_scores (username, language, level, score)
        VALUES (:username, :language, :level, :score)
    )");
    query.bindValue(":username", userName);
    query.bindValue(":language", language);
    query.bindValue(":level", level);
    query.bindValue(":score", score);

    if (!query.exec()) {
        qDebug() << "Failed to save score:" << query.lastError().text();
    } else {
        qDebug() << "Score saved for" << userName << "Level:" << level << "Score:" << score;
    }
}

// Next button handler (also handles Submit on last question)
void MainWindow::onNextQuestionClicked()
{
    const QList<Question> &qList = questions[selectedLanguage][currentLevel];
    int total = qList.size();
    if (total == 0) return;

    // Save current answer if code question
    checkAndStoreCurrentAnswerBeforeNavigate();

    if (currentQuestionIndex < total - 1) {
        currentQuestionIndex++;
        loadQuestion(selectedLanguage, currentLevel);
    } else {
        // Last question reached: show Revisit / Submit dialog
        QMessageBox msg;
        msg.setWindowTitle("Finish Level");
        msg.setText("You've reached the last question. Do you want to revisit the questions or submit?");
        QPushButton *revisitBtn = msg.addButton("Revisit", QMessageBox::ActionRole);
        QPushButton *submitBtn = msg.addButton("Submit", QMessageBox::AcceptRole);
        msg.exec();

        if (msg.clickedButton() == revisitBtn) {
            currentQuestionIndex = 0;
            loadQuestion(selectedLanguage, currentLevel);
            ui->stackedWidget->setCurrentWidget(ui->questionPage);
        } else if (msg.clickedButton() == submitBtn) {
            finalizeSubmission();
        }
    }
}

// Back button handler
void MainWindow::onBackQuestionClicked()
{
    // Save current code answer before moving back
    checkAndStoreCurrentAnswerBeforeNavigate();

    if (currentQuestionIndex > 0) {
        currentQuestionIndex--;
        loadQuestion(selectedLanguage, currentLevel);
    } else {
        // at first question, take user back to levels page
        ui->stackedWidget->setCurrentIndex(2);
    }
}

// Hint button clicked
void MainWindow::onHintButtonClicked()
{
    if (hintsRemainingCurrentLevel <= 0) {
        hintLabel->setText("No hints left for this level.");
        hintLabel->show();
        return;
    }

    const QList<Question> &qList = questions[selectedLanguage][currentLevel];
    if (currentQuestionIndex < 0 || currentQuestionIndex >= qList.size()) {
        hintLabel->setText("No question to hint.");
        hintLabel->show();
        return;
    }

    const Question &q = qList[currentQuestionIndex];
    if (q.hint.isEmpty()) {
        hintLabel->setText("No hint available for this question.");
    } else {
        hintLabel->setText("Hint: " + q.hint);
    }
    hintLabel->show();

    // decrement hints
    hintsRemainingCurrentLevel = std::max(0, hintsRemainingCurrentLevel - 1);
    // write back to global map for this language & level
    hintsAvailable[selectedLanguage][currentLevel] = hintsRemainingCurrentLevel;
    updateHintCountUI();

}
void MainWindow::finalizeSubmission()
{
    const QList<Question> &qList = questions[selectedLanguage][currentLevel];
    int total = qList.size();
    int score = 0;

    for (int i = 0; i < total; ++i) {
        const Question &q = qList[i];
        if (q.isMCQ) {
            if (i < userMCQAnswers.size() && userMCQAnswers[i] == q.correctIndex)
                score++;
        } else {
            QString typed = (i < userCodeAnswers.size()) ? userCodeAnswers[i].trimmed() : QString();
            QString expect = q.expectedAnswer.trimmed();
            if (!typed.isEmpty() && !expect.isEmpty()) {
                QString typedLower = typed.toLower();
                QString expectLower = expect.toLower();
                if (typedLower.contains(expectLower) || expectLower.contains(typedLower)) {
                    score++;
                }
            }
        }
    }

    // Save into SQLite DB
    bool ok = Database::instance().saveUserScore(userName, selectedLanguage, currentLevel, score);

    // UPDATE LEADERBOARD - THIS LINE MUST BE PRESENT
    Database::instance().updateLeaderboard(userName, selectedLanguage, score);

    if (!ok) {
        qDebug() << "Failed to save score to DB";
    }

    // DEBUG: Before unlocking
    qDebug() << "=== BEFORE UNLOCKING ===";
    qDebug() << "Current Level:" << currentLevel;
    qDebug() << "Score:" << score;
    qDebug() << "Current Progress:" << languageProgress[selectedLanguage];

    // UNLOCK NEXT LEVEL IF SCORE IS 10 OR MORE
    if (score >= 10 && currentLevel < 4) {
        int nextLevel = currentLevel + 1;
        if (languageProgress[selectedLanguage] < nextLevel) {
            languageProgress[selectedLanguage] = nextLevel;
            qDebug() << "UNLOCKED LEVEL" << nextLevel << "FOR" << selectedLanguage;
            qDebug() << "New Progress:" << languageProgress[selectedLanguage];
        }
    }

    // DEBUG: After unlocking
    qDebug() << "=== AFTER UNLOCKING ===";
    qDebug() << "Final Progress:" << languageProgress[selectedLanguage];

    // Show results to user
    QString resultMessage = QString("Level %1 Completed!\n\n"
                                    "Your Score: %2/%3\n"
                                    "Percentage: %4%\n\n")
                                .arg(currentLevel)
                                .arg(score)
                                .arg(total)
                                .arg((score * 100) / total);

    // Add unlock message if applicable
    if (score >= 10 && currentLevel < 4) {
        resultMessage += QString("🎉 Level %1 unlocked!").arg(currentLevel + 1);
    } else if (score < 10) {
        resultMessage += "You need at least 10 points to unlock the next level.";
    }

    QMessageBox::information(this, "Level Results", resultMessage);

    // Go back to level selection
    ui->stackedWidget->setCurrentIndex(2);

    // Update the level lock status UI
    updateLevelLockStatusUI();
}
void MainWindow::loadQuestion(const QString &lang, int level)
{
    const QList<Question> &qList = questions[lang][level];
    if (qList.isEmpty()) {
        ui->questionLabel->setText("No questions for this level yet.");
        ui->option1Button->hide();
        ui->option2Button->hide();
        ui->option3Button->hide();
        ui->option4Button->hide();
        ui->codeAnswerBox->hide();
        hintLabel->hide();
        ui->nextQuestionButton->setText("Next");
        return;
    }

    if (currentQuestionIndex < 0) currentQuestionIndex = 0;
    if (currentQuestionIndex >= qList.size()) currentQuestionIndex = qList.size() - 1;

    const Question &q = qList[currentQuestionIndex];

    ui->questionLabel->setText(QString("Question %1: %2").arg(currentQuestionIndex + 1).arg(q.text));

    if (q.isMCQ) {
        ui->option1Button->show();
        ui->option2Button->show();
        ui->option3Button->show();
        ui->option4Button->show();
        ui->codeAnswerBox->hide();
        hintLabel->hide();

        QString o0 = (q.options.size() > 0) ? q.options[0] : "";
        QString o1 = (q.options.size() > 1) ? q.options[1] : "";
        QString o2 = (q.options.size() > 2) ? q.options[2] : "";
        QString o3 = (q.options.size() > 3) ? q.options[3] : "";

        ui->option1Button->setText(o0);
        ui->option2Button->setText(o1);
        ui->option3Button->setText(o2);
        ui->option4Button->setText(o3);

        refreshOptionButtonsUI();
    } else {
        ui->option1Button->hide();
        ui->option2Button->hide();
        ui->option3Button->hide();
        ui->option4Button->hide();
        ui->option1Button->setChecked(false);
        ui->option2Button->setChecked(false);
        ui->option3Button->setChecked(false);
        ui->option4Button->setChecked(false);

        ui->codeAnswerBox->show();
        QString prev = userCodeAnswers[currentQuestionIndex];
        ui->codeAnswerBox->setPlainText(prev);

        hintLabel->hide();
    }

    const int total = qList.size();
    if (currentQuestionIndex == total - 1) {
        ui->nextQuestionButton->setText("Finish");
    } else {
        ui->nextQuestionButton->setText("Next");
    }
}

int MainWindow::computeBonusHintsForScore(int score)
{
    if (score >= 15) return 5;
    if (score >= 13) return 3;
    if (score == 12) return 2;
    if (score >= 10) return 1;
    return 0;
}

void MainWindow::updateHintCountUI()
{
    if (selectedLanguage.isEmpty()) {
        if (ui->hintCountLabel) ui->hintCountLabel->setText("Hints: 0");
        return;
    }
    int level = currentLevel > 0 ? currentLevel : 1;
    int available = hintsAvailable[selectedLanguage].value(level, 0);
    hintsRemainingCurrentLevel = available;
    if (ui->hintCountLabel) ui->hintCountLabel->setText(QString("Hints left: %1").arg(available));
}

void MainWindow::openLevel(int level)
{
    if (selectedLanguage.isEmpty()) return;

    // DEBUG: Check current progress
    qDebug() << "=== OPEN LEVEL DEBUG ===";
    qDebug() << "Selected Language:" << selectedLanguage;
    qDebug() << "Requested Level:" << level;
    qDebug() << "Current Progress:" << languageProgress[selectedLanguage];
    qDebug() << "Condition:" << (languageProgress[selectedLanguage] < level);

    if (languageProgress[selectedLanguage] < level) {
        qDebug() << "LEVEL IS LOCKED - Showing popup";
        QMessageBox::information(this, "Level Locked 🔒",
                                 QString("Level %1 is locked!\n\n"
                                         "You need to score 10 or more points in Level %2 to unlock this level.\n\n"
                                         "Keep practicing! 💪")
                                     .arg(level)
                                     .arg(level-1));
        return;
    }

    qDebug() << "LEVEL IS UNLOCKED - Proceeding";

    currentLevel = level;
    currentQuestionIndex = 0;

    int totalQ = questions[selectedLanguage][level].size();
    userMCQAnswers = QVector<int>(totalQ, -1);
    userCodeAnswers = QVector<QString>(totalQ, QString());

    hintsRemainingCurrentLevel = hintsAvailable[selectedLanguage][level];
    updateHintCountUI();

    loadQuestion(selectedLanguage, level);

    ui->stackedWidget->setCurrentWidget(ui->questionPage);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::updateLevelLockStatusUI()
{
    if (selectedLanguage.isEmpty()) return;

    int currentProgress = languageProgress[selectedLanguage];

    // Update level buttons based on progress
    ui->level1Button->setEnabled(true); // Level 1 is always enabled

    ui->level2Button->setEnabled(currentProgress >= 2);
    ui->level3Button->setEnabled(currentProgress >= 3);
    ui->level4Button->setEnabled(currentProgress >= 4);

    // Update button texts to show locked/unlocked status
    ui->level1Button->setText(languageLevels[selectedLanguage][0]);

    if (currentProgress >= 2) {
        ui->level2Button->setText(languageLevels[selectedLanguage][1]);
    } else {
        ui->level2Button->setText(languageLevels[selectedLanguage][1] + " 🔒");
    }

    if (currentProgress >= 3) {
        ui->level3Button->setText(languageLevels[selectedLanguage][2]);
    } else {
        ui->level3Button->setText(languageLevels[selectedLanguage][2] + " 🔒");
    }

    if (currentProgress >= 4) {
        ui->level4Button->setText(languageLevels[selectedLanguage][3]);
    } else {
        ui->level4Button->setText(languageLevels[selectedLanguage][3] + " 🔒");
    }
}
