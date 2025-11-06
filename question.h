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
QVector<Question> getLevel2Questions();
QVector<Question> getLevel3Questions();
QVector<Question> getLevel4Questions();
QVector<Question> getLevel5Questions();
QVector<Question> getLevel6Questions();
QVector<Question> getLevel7Questions();
QVector<Question> getLevel8Questions();
QVector<Question> getLevel9Questions();
QVector<Question> getLevel10Questions();
QVector<Question> getLevel11Questions();
QVector<Question> getLevel12Questions();
QVector<Question> getLevel13Questions();
QVector<Question> getLevel14Questions();
QVector<Question> getLevel15Questions();
QVector<Question> getLevel16Questions();
QVector<Question> getLevel17Questions();
QVector<Question> getLevel18Questions();
QVector<Question> getLevel19Questions();
QVector<Question> getLevel20Questions();
QVector<Question> getLevel21Questions();
QVector<Question> getLevel22Questions();
QVector<Question> getLevel23Questions();
QVector<Question> getLevel24Questions();
QVector<Question> getLevel25Questions();
QVector<Question> getLevel26Questions();
QVector<Question> getLevel27Questions();
QVector<Question> getLevel28Questions();
QVector<Question> getLevel29Questions();
QVector<Question> getLevel30sQuestions();


#endif // QUESTION_H
