#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

struct Node {
    char data;
    Node* next;
    Node(char c) : data(c), next(nullptr) {}
};

class Stack {
private:
    Node* top;  

public:
    Stack() : top(nullptr) {}
    bool isEmpty() {
        return top == nullptr;
    }


    void push(char c) {
        Node* newNode = new Node(c);
        newNode->next = top;
        top = newNode;
    }


    char pop() {
        if (isEmpty()) {
            return '\0';
        }
        Node* temp = top;
        char c = top->data;
        top = top->next;
        delete temp;
        return c;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

std::string generateRandomString(int length) {
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    const int charsetSize = sizeof(charset) - 1;

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += charset[rand() % charsetSize];
    }
    return result;
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    int length;
    std::cout << "Введите длину строки: ";
    std::cin >> length;

    if (length <= 0) {
        std::cout << "Некорректная длина!" << std::endl;
        return 1;
    }

    std::string original = generateRandomString(length);
    std::cout << "\nСгенерированная строка: " << original << std::endl;

    Stack charStack;
    for (char c : original) {
        charStack.push(c);
    }

    std::string reversed;
    while (!charStack.isEmpty()) {
        reversed += charStack.pop();
    }

    std::cout << "Реверсированная строка: " << reversed << std::endl;

    return 0;
}