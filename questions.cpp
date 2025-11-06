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
QVector<Question> getLevel2Questions() {
    QVector<Question> questions;

    // 1. MCQ
    questions.push_back({
        "MCQ",
        "Which operator is used to allocate memory dynamically in C++?",
        {"malloc", "alloc", "new", "create"},
        "new"
    });

    // 2. MCQ
    questions.push_back({
        "MCQ",
        "Which function is used to release dynamically allocated memory in C++?",
        {"free", "delete", "dispose", "remove"},
        "delete"
    });

    // 3. Output Prediction
    questions.push_back({
        "CODE",
        "What will this print?\n int x = 10; cout << ++x;",
        {},
        "11"
    });

    // 4. Code Fix
    questions.push_back({
        "CODE",
        "Fix the error:\n string s = Hello;",
        {},
        "string s = \"Hello\";"
    });

    // 5. MCQ
    questions.push_back({
        "MCQ",
        "Which loop is guaranteed to run at least once in C++?",
        {"for", "while", "do-while", "none"},
        "do-while"
    });

    return questions;
}
QVector<Question> getLevel3Questions() {
    QVector<Question> questions;
    questions.append({
        "MCQ",
        "Which keyword is used to define a class in C++?",
        {"struct", "define", "class", "object"},
        "class"
    });

    questions.append({
        "CODE",
        "Write a program to print 'Level 3 Started!' in C++.",
        {},
        "cout << \"Level 3 Started!\";"
    });

    return questions;
}
QVector<Question> getLevel4Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which keyword is used to define a class in C++?",
        {"struct", "define", "class", "object"},
        "class"
    });

    questions.append({
        "CODE",
        "Write a program to print 'Level 3 Started!' in C++.",
        {},
        "cout << \"Level 3 Started!\";"
    });

    questions.append({
        "MCQ",
        "Which operator is used to access members of a class through a pointer?",
        {".", "->", "::", "*"},
        "->"
    });

    questions.append({
        "CODE",
        "Write a C++ program to declare an integer variable 'x' with value 10 and print it.",
        {},
        "int x = 10;\ncout << x;"
    });

    questions.append({
        "MCQ",
        "What is the default return type of a function in C++ if not specified?",
        {"int", "void", "float", "double"},
        "int"
    });

    questions.append({
        "CODE",
        "Write a C++ program to take two integers as input and print their sum.",
        {},
        "int a, b;\ncin >> a >> b;\ncout << a + b;"
    });

    return questions;
}
QVector<Question> getLevel5Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which of the following is NOT a feature of C++?",
        {"Encapsulation", "Polymorphism", "Garbage Collection", "Inheritance"},
        "Garbage Collection"
    });

    questions.append({
        "MCQ",
        "What is the default return type of the main() function in C++?",
        {"void", "int", "float", "char"},
        "int"
    });

    questions.append({
        "MCQ",
        "Which operator cannot be overloaded in C++?",
        {"+", "=", "::", "[]"},
        "::"
    });

    questions.append({
        "CODE",
        "Write a C++ loop that prints numbers from 1 to 10.",
        {},
        "for(int i=1; i<=10; i++) {\n    cout << i << \" \";\n}"
    });

    questions.append({
        "CODE",
        "Write a C++ program to check whether a number is even or odd.",
        {},
        "int n; cin >> n;\nif(n % 2 == 0) cout << \"Even\";\nelse cout << \"Odd\";"
    });

    return questions;
}
QVector<Question> getLevel6Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which operator is overloaded by default in C++?",
        {"=", "()", "new", "+"},
        "="
    });

    questions.append({
        "MCQ",
        "Which of the following is not a feature of Object Oriented Programming?",
        {"Encapsulation", "Polymorphism", "Recursion", "Inheritance"},
        "Recursion"
    });

    questions.append({
        "MCQ",
        "Which concept allows using the same function name with different parameters in C++?",
        {"Inheritance", "Overloading", "Encapsulation", "Abstraction"},
        "Overloading"
    });

    questions.append({
        "CODE",
        "Write a function in C++ that takes two integers and returns their maximum.",
        {},
        "int getMax(int a, int b) { return (a > b) ? a : b; }"
    });

    questions.append({
        "CODE",
        "Write a program in C++ to print numbers from 1 to 5 using a for loop.",
        {},
        "for(int i=1; i<=5; i++) {\n    cout << i << \" \";\n}"
    });

    return questions;
}
QVector<Question> getLevel7Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which of the following is used to allocate memory dynamically in C++?",
        {"malloc", "alloc", "new", "calloc"},
        "new"
    });

    questions.append({
        "MCQ",
        "Which header file is required for using the 'string' class in C++?",
        {"<string.h>", "<cstring>", "<string>", "<str>"},
        "<string>"
    });

    questions.append({
        "MCQ",
        "What is the default return type of 'main()' in C++?",
        {"void", "int", "float", "char"},
        "int"
    });

    questions.append({
        "CODE",
        "Write a C++ program to swap two numbers using a temporary variable.",
        {},
        "int a=5, b=10, temp;\ntemp=a;\na=b;\nb=temp;\ncout<<a<<\" \"<<b;"
    });

    questions.append({
        "CODE",
        "Write a C++ program to calculate factorial of a number using recursion.",
        {},
        "int fact(int n){\n    if(n<=1) return 1;\n    return n*fact(n-1);\n}\ncout<<fact(5);"
    });

    return questions;
}
QVector<Question> getLevel8Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which concept in C++ provides data hiding?",
        {"Inheritance", "Polymorphism", "Encapsulation", "Overloading"},
        "Encapsulation"
    });

    questions.append({
        "MCQ",
        "Which of the following is an abstract class in C++?",
        {"Class with only constructors", "Class with a pure virtual function", "Class with only static members", "Class with private variables"},
        "Class with a pure virtual function"
    });

    questions.append({
        "MCQ",
        "Which of the following access specifiers is used by default for members of a class in C++?",
        {"private", "public", "protected", "friend"},
        "private"
    });

    questions.append({
        "CODE",
        "Write a class in C++ named 'Student' with attributes name and age, and a method to display them.",
        {},
        "class Student {\npublic:\n string name;\n int age;\n void display(){ cout<<name<<\" \"<<age; }\n};"
    });

    questions.append({
        "CODE",
        "Write a program in C++ to find the sum of array elements.",
        {},
        "int arr[5]={1,2,3,4,5};\nint sum=0;\nfor(int i=0;i<5;i++) sum+=arr[i];\ncout<<sum;"
    });

    return questions;
}
QVector<Question> getLevel9Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which of the following is not a C++ access specifier?",
        {"public", "private", "protected", "package"},
        "package"
    });

    questions.append({
        "MCQ",
        "Which feature of OOP allows the use of the same function name with different arguments?",
        {"Encapsulation", "Polymorphism", "Inheritance", "Abstraction"},
        "Polymorphism"
    });

    questions.append({
        "CODE",
        "Write a C++ program to print numbers from 1 to 5 using a while loop.",
        {},
        "int i = 1;\nwhile(i <= 5) {\n    cout << i << \" \";\n    i++;\n}"
    });

    questions.append({
        "MCQ",
        "What does the 'this' pointer in C++ represent?",
        {"Current object", "Base class object", "Pointer to class", "Function address"},
        "Current object"
    });

    questions.append({
        "CODE",
        "Write a C++ program that takes two numbers and prints their sum.",
        {},
        "int a, b;\ncin >> a >> b;\ncout << a + b;"
    });

    return questions;
}
QVector<Question> getLevel10Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which operator cannot be overloaded in C++?",
        {"+", "==", "::", "[]"},
        "::"
    });

    questions.append({
        "MCQ",
        "Which type of inheritance is not supported in C++?",
        {"Single", "Multiple", "Hybrid", "None of the above"},
        "None of the above"
    });

    questions.append({
        "CODE",
        "Write a C++ program to calculate the factorial of a number using recursion.",
        {},
        "int fact(int n){\n    if(n<=1) return 1;\n    return n * fact(n-1);\n}\nint main(){\n    int n; cin >> n;\n    cout << fact(n);\n}"
    });

    questions.append({
        "MCQ",
        "Which keyword is used to prevent a variable from being modified in C++?",
        {"immutable", "const", "static", "volatile"},
        "const"
    });

    questions.append({
        "CODE",
        "Write a C++ program that swaps two numbers using pointers.",
        {},
        "int a, b;\ncin >> a >> b;\nint *p1 = &a, *p2 = &b;\nint temp = *p1;\n*p1 = *p2;\n*p2 = temp;\ncout << a << \" \" << b;"
    });

    return questions;
}
QVector<Question> getLevel11Questions() {
    QVector<Question> questions;

    questions.append({
        "MCQ",
        "Which of the following correctly declares a pointer to an integer in C++?",
        {"int *ptr;", "int ptr*;", "pointer<int> ptr;", "int &ptr;"},
        "int *ptr;"
    });

    questions.append({
        "MCQ",
        "What is the output of the following code?\nint x = 5;\ncout << ++x << x++;",
        {"56", "65", "55", "66"},
        "65"
    });

    questions.append({
        "CODE",
        "Write a C++ program that prints all even numbers between 1 and 20 using a for loop.",
        {},
        "for(int i=2; i<=20; i+=2){\n    cout << i << \" \";\n}"
    });

    questions.append({
        "MCQ",
        "Which of the following keywords is used for exception handling in C++?",
        {"try", "catch", "throw", "All of the above"},
        "All of the above"
    });

    questions.append({
        "CODE",
        "Write a C++ function that swaps two integers using references.",
        {},
        "void swap(int &a, int &b){\n    int temp = a;\n    a = b;\n    b = temp;\n}"
    });

    return questions;
}
