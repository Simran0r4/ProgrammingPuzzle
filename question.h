#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QStringList>

struct Question {
    QString type;            // "MCQ", "CODE_FIX", "OUTPUT", etc.
    QString questionText;    // The question itself
    QStringList options;     // Options for MCQ (empty for code/output)
    QString correctAnswer;   // Correct answer text
};
QVector<Question> getLevel1Questions();
#endif // QUESTION_H
