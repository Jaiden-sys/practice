#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;


vector<int> generateRingArray(int N) {
    int firstLen = N / 2;
    int secondLen = N - firstLen;


    vector<int> first;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist1(0, 10);
    int val = dist1(gen);
    for (int i = 0; i < firstLen; ++i) {
        val += dist1(gen); 
        first.push_back(val);
    }


    vector<int> second;
    uniform_int_distribution<> dist2(1, firstLen);
    int prev = 0;
    for (int i = 0; i < secondLen; ++i) {
        int inc = dist2(gen);
        prev += inc;
        second.push_back(prev);
    }

  
    vector<int> ringArray = first;
    ringArray.insert(ringArray.end(), second.begin(), second.end());

    return ringArray;
}


int findBreakIndex(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1])
            return i;
    }
    return 0; 
}

int countRingness(const vector<int>& arr, int breakIndex) {
    int count = 0;
    int firstVal = arr[0];
    for (size_t i = breakIndex; i < arr.size(); ++i) {
        if (arr[i] <= firstVal)
            ++count;
    }
    return count;
}

int main() {
    int N;
    cout << "Введите N: ";
    cin >> N;

    vector<int> arr = generateRingArray(N);

    cout << "Сгенерированный массив:\n";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    int breakIndex = findBreakIndex(arr);
    int ringness = countRingness(arr, breakIndex);

    cout << "Закольцованность: " << ringness << "\n";
    return 0;
}
