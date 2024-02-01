// CPPMathQuizzer.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <string>
#include <cstdio>
#include <chrono>

using namespace std;

struct QuizAttempt {
    string userName;
    int attemptNumber;
    int correctAnswers;
    int totalQuestions;
};

int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int additionQuestion(int num1, int num2) {
    return num1 + num2;
}

int subtractionQuestion(int num1, int num2) {
    return num1 - num2;
}

int multiplicationQuestion(int num1, int num2) {
    return num1 * num2;
}

int divisionQuestion(int num1, int num2) {
    return num1 / num2;
}

void saveAttempt(const QuizAttempt& attempt) {
    ofstream outFile("quiz_results.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    outFile << attempt.userName << ',' << attempt.attemptNumber << ',' << attempt.correctAnswers << ',' << attempt.totalQuestions << '\n';

    outFile.close();
}

void displayDashboard(const vector<QuizAttempt>& attempts) {
    cout << "Quiz Results Dashboard" << endl;
    cout << setw(15) << "Attempt" << setw(15) << "Correct" << setw(15) << "Total" << setw(15) << "Percentage" << endl;

    for (const auto& attempt : attempts) {
        cout << setw(15) << attempt.attemptNumber
            << setw(15) << attempt.correctAnswers
            << setw(15) << attempt.totalQuestions
            << setw(15) << fixed << setprecision(2) << (static_cast<double>(attempt.correctAnswers) / attempt.totalQuestions) * 100 << "%" << endl;
    }
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Load previous quiz attempts
    vector<QuizAttempt> quizAttempts;
    ifstream inFile("quiz_results.txt");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            QuizAttempt attempt;
            size_t pos = line.find(',');

            // Read the user name until the first comma
            if (pos != string::npos) {
                attempt.userName = line.substr(0, pos);
                line = line.substr(pos + 1); // Move past the first comma
            }

            // Read the remaining fields directly
            if (sscanf_s(line.c_str(), "%d,%d,%d", &attempt.attemptNumber, &attempt.correctAnswers, &attempt.totalQuestions) == 3) {
                // Check if attemptNumber is non-negative, else set it to 1
                attempt.attemptNumber = (attempt.attemptNumber >= 0) ? attempt.attemptNumber : 1;
                quizAttempts.push_back(attempt);
            }
            else {
                cerr << "Error reading attempt data." << endl;
            }
        }
        inFile.close();
    }

    // User information
    string userName;
    cout << "Enter your name: ";
    getline(cin, userName);

    // Increment attempt number
    int attemptNumber = quizAttempts.empty() ? 1 : quizAttempts.back().attemptNumber + 1;

    int choice;
    do {
        // Welcome message
        cout << "Welcome to the Math Quiz Game!" << endl;
        cout << "Test your math skills with addition, subtraction, multiplication, and division." << endl << endl;

        // Set the number of questions
        const int numQuestions = 5;
        int correctAnswers = 0;

        // Menu for the user to choose the type of operation
        cout << "Choose the type of operation:" << endl;
        cout << "1. Addition" << endl;
        cout << "2. Subtraction" << endl;
        cout << "3. Multiplication" << endl;
        cout << "4. Division" << endl;

        // Get the user's choice
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
        }
        // Consume the newline character left in the input buffer
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int i = 1; i <= numQuestions; ++i) {
            // Generate random numbers for the question
            int num1 = generateRandomNumber(1, 10);
            int num2 = generateRandomNumber(1, 10);

            // Determine the operation based on the user's choice
            int result, userAnswer;
            switch (choice) {
            case 1:
                cout << "Question " << i << ": What is " << num1 << " + " << num2 << "? ";
                result = additionQuestion(num1, num2);
                break;
            case 2:
                cout << "Question " << i << ": What is " << num1 << " - " << num2 << "? ";
                result = subtractionQuestion(num1, num2);
                break;
            case 3:
                cout << "Question " << i << ": What is " << num1 << " * " << num2 << "? ";
                result = multiplicationQuestion(num1, num2);
                break;
            case 4:
                // Avoid division by zero
                num2 = (num2 != 0) ? num2 : 1;
                cout << "Question " << i << ": What is " << num1 << " / " << num2 << "? ";
                result = divisionQuestion(num1, num2);
                break;
            default:
                cout << "Invalid choice. Exiting the program." << endl;
                return 1;
            }

            // Get the user's answer
            cin >> userAnswer;

            // Check the answer
            if (userAnswer == result) {
                cout << "Correct! Well done." << endl;
                correctAnswers++;
            }
            else {
                cout << "Incorrect. The correct answer is " << result << "." << endl;
            }

            // Add a newline for better readability
            cout << endl;
        }

        // Save the attempt
        QuizAttempt currentAttempt;
        currentAttempt.userName = userName;
        currentAttempt.attemptNumber = attemptNumber;
        currentAttempt.correctAnswers = correctAnswers;
        currentAttempt.totalQuestions = numQuestions;

        saveAttempt(currentAttempt);

        // Display the final score
        cout << "Quiz completed! You got " << correctAnswers << " out of " << numQuestions << " questions correct." << endl;

        // Display dashboard
        quizAttempts.push_back(currentAttempt);
        displayDashboard(quizAttempts);

        // Prompt the user to try another quiz
        cout << "Do you want to try another quiz? (1 for Yes, 0 for No): ";
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Consume the newline character

        attemptNumber++;
    } while (choice != 0);

    return 0;
}
