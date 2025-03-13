#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>

int main() {
    int length = 0;
    int firstNumber, secondNumber;

    std::ifstream file("example.txt");
    if (!file) {
        std::cerr << "Error: Unable to open file " << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        length++;
    }

    int *arr1 = new int[length];
    int *arr2 = new int[length];
    int *sim = new int[length];

    file.clear();
    file.seekg(0, std::ios::beg);

    int cont = 0;

    std::string line2;
    while (std::getline(file, line2)) {
        std::stringstream ss(line2);
        ss >> firstNumber;
        arr1[cont] = firstNumber;

        ss.ignore(3);

        ss >> secondNumber;
        arr2[cont] = secondNumber;
        cont++;
    }

    file.close();

    std::sort(arr1, arr1 + length);
    std::sort(arr2, arr2 + length);

    for (int i = 0; i < length; i++) {
        int num = std::count(arr2, arr2 + length, arr1[i]);
        sim[i] = arr1[i] * num;
    }

    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += abs(arr1[i] - arr2[i]);
    }
    int score = std::reduce(sim, sim + length);

    std::cout << "Total distance: " << sum << std::endl;
    std::cout << "Similarity score: " << score << std::endl;

    delete[] arr1;
    delete[] arr2;

    return 0;
}