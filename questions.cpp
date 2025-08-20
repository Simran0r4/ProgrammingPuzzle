#include "question.h"

QVector<Question> getLevel1Questions() {
    QVector<Question> questions;

    // 1. MCQ
    questions.push_back({
        "MCQ",
        "Which of the following is a valid C++ comment?",
        {"/* comment */", "// comment", "# comment", "Both a & b"},
        "Both a & b"
    });

    // 2. MCQ
    questions.push_back({
        "MCQ",
        "Which data type is used to store a single character in C++?",
        {"string", "char", "int", "bool"},
        "char"
    });

    // 3. Syntax Fix
    questions.push_back({
        "CODE",
        "Fix the error: \n int main { cout << \"Hello\"; return 0; }",
        {},
        "int main() { cout << \"Hello\"; return 0; }"
    });

    // 4. Output Prediction
    questions.push_back({
        "CODE",
        "What is the output?\n int x = 5; cout << x++ << endl;",
        {},
        "5"
    });

    // 5. Output Prediction
    questions.push_back({
        "CODE",
        "What will be printed?\n printf(\"%d\", 3 + 2 * 2);",
        {},
        "7"
    });

    // 6. MCQ
    questions.push_back({
        "MCQ",
        "Which header is required for cout in C++?",
        {"<stdio.h>", "<iostream>", "<stdlib.h>", "<cstdio>"},
        "<iostream>"
    });

    // 7. Code Correction
    questions.push_back({
        "CODE",
        "Fix: \n for(i=0; i<5; i++) { cout << i << endl; }",
        {},
        "for(int i=0; i<5; i++) { cout << i << endl; }"
    });

    // 8. MCQ
    questions.push_back({
        "MCQ",
        "Which keyword is used to define a constant in C++?",
        {"const", "final", "#define", "Both const and #define"},
        "Both const and #define"
    });

    // 9. Output Prediction
    questions.push_back({
        "CODE",
        "Output?\n char c = 'A'; cout << c + 1;",
        {},
        "66"
    });

    // 10. MCQ
    questions.push_back({
        "MCQ",
        "Which of the following is not a loop in C++?",
        {"for", "while", "do-while", "foreach"},
        "foreach"
    });

    // 11. Code Correction
    questions.push_back({
        "CODE",
        "Fix the declaration: \n string name = 'Alice';",
        {},
        "string name = \"Alice\";"
    });

    // 12. Output
    questions.push_back({
        "CODE",
        "What is printed?\n int arr[3] = {1, 2, 3}; cout << arr[1];",
        {},
        "2"
    });

    // 13. MCQ
    questions.push_back({
        "MCQ",
        "What is the size of int (commonly) on a 64-bit system?",
        {"2 bytes", "4 bytes", "8 bytes", "Depends on compiler"},
        "4 bytes"
    });

    // 14. Code Fix
    questions.push_back({
        "CODE",
        "Fix: \n if(x=5) { cout << \"Yes\"; }",
        {},
        "if(x==5) { cout << \"Yes\"; }"
    });

    // 15. Output
    questions.push_back({
        "CODE",
        "What is output?\n int x=10; cout << (x > 5 ? 1 : 0);",
        {},
        "1"
    });

    return questions;
}
