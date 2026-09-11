#include <iostream>

int main() {
    int num1 = 0, num2 = 0;
    std::cout << "Enter two numbers separated by a space: ";
    std::cin >> num1;
    std::cin >> num2;
    // early exit
    if (num1 == num2) {
        std::cout << "LCM is " << num1;
        return 0;
    } else if (num1 < num2) {
        for (int i = num1; i <= num1 * num2; i++)
            if (i % num1 == 0 && i % num2 == 0) {
                std::cout << "LCM is " << i;
                break;
            }
    } else
        for (int i = num2; i <= num1 * num2; i++)
            if (i % num1 == 0 && i % num2 == 0) {
                std::cout << "LCM is " << i;
                break;
            }
    return 0;
}
