#include <iostream>
#include <vector>
#include <algorithm>

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    LinkedList() : head(nullptr) {}

    LinkedList(const std::vector<int>& values) : head(nullptr) {
        for (auto it = values.rbegin(); it != values.rend(); ++it) {
            push_front(*it);
        }
    }

    void push_front(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }

    void mergeSorted(LinkedList& other) {
        Node dummy(0);
        Node* tail = &dummy;
        Node* l1 = head;
        Node* l2 = other.head;

        while (l1 && l2) {
            if (l1->data <= l2->data) {
                tail->next = l1;
                l1 = l1->next;
            }
            else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        tail->next = l1 ? l1 : l2;
        head = dummy.next;

        other.head = nullptr;
    }

    // Вывод списка
    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};

int main() {
    std::vector<int> arr1, arr2;
    int n, value;
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите размер первого массива: ";
    std::cin >> n;
    std::cout << "Введите " << n << " чисел через пробел: ";
    for (int i = 0; i < n; i++) {
        std::cin >> value;
        arr1.push_back(value);
    }

    std::cout << "Введите размер второго массива: ";
    std::cin >> n;
    std::cout << "Введите " << n << " чисел через пробел: ";
    for (int i = 0; i < n; i++) {
        std::cin >> value;
        arr2.push_back(value);
    }

    std::sort(arr1.begin(), arr1.end());
    std::sort(arr2.begin(), arr2.end());


    LinkedList list1(arr1);
    LinkedList list2(arr2);

    std::cout << "\nПервый список: ";
    list1.print();

    std::cout << "Второй список: ";
    list2.print();

  
    list1.mergeSorted(list2);

    std::cout << "Результат слияния: ";
    list1.print();

    return 0;
}