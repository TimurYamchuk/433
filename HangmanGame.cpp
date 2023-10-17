#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <windows.h>
#include "HangmanGame.h"
using namespace std;

HangmanGame::HangmanGame(const vector<string>& loadedWords) : words(loadedWords), maxTime(120), maxAttempts(5) {
    // Конструктор класса HangmanGame, инициализация параметров.
}

void HangmanGame::Run() {
    wordToGuess = ChooseWord();  // Выбираем слово для угадывания.
    startTime = time(NULL);
    time_t currentTime;
    int timeLeft;
    int attemptsLeft = maxAttempts;  // Оставшееся количество попыток.
    int tone = 1000;  // Начальная тональность для Beep.

    while (attemptsLeft > 0) {
        currentTime = time(NULL);
        timeLeft = maxTime - difftime(currentTime, startTime);

        if (timeLeft <= 0) {
            Beep(tone, 300);  // Звуковой сигнал при завершении времени.
            DisplayGameResultTimeout();  // Вывод сообщения о завершении времени.
            if (!PlayAgain()) {
                return;
            }
            wordToGuess = ChooseWord();
            guessedLetters.clear();
            startTime = time(NULL);
            attemptsLeft = maxAttempts;
            tone = 1000;  // Сброс тональности.
        }

        DisplayWord();  // Отображение текущего состояния слова.
        cout << "Оставшееся время: " << timeLeft << " секунд." << endl;
        cout << "Попыток осталось: " << attemptsLeft << endl;
        cout << "Введите букву: ";
        string input;
        cin >> input;

        if (input == "hint" && timeLeft > maxTime / 2) {
            cout << "Подсказка: " << GetHint() << endl;
        }
        else if (input.length() == 1 && isalpha(input[0])) {
            char guess = tolower(input[0]);

            if (IsGuessed(guess)) {
                Beep(tone, 300);  // Звуковой сигнал при вводе уже угаданной буквы.
                cout << "Вы уже вводили эту букву." << endl;
            }
            else {
                guessedLetters.push_back(guess);
                if (IsWinner()) {
                    Beep(3000, 300);
                    Beep(100, 300);
                    Beep(3000, 300);  // Звуковой сигнал при победе.
                    DisplayGameResultWin();  // Вывод сообщения о победе.
                    if (!PlayAgain()) {
                        return;
                    }
                    wordToGuess = ChooseWord();
                    guessedLetters.clear();
                    startTime = time(NULL);
                    attemptsLeft = maxAttempts;
                    tone = 1000;  // Сброс тональности.
                }
                else if (!IsLetterInWord(guess)) {
                    Beep(tone, 300);
                    cout << "Этой буквы нет в слове." << endl;
                    attemptsLeft--;
                    tone -= 300;  // Уменьшение тональности с каждой ошибкой.
                }
            }
        }
        else {
            Beep(tone, 300);
            cout << "Введите одну букву." << endl;
        }
    }

    DisplayGameResultLose();  // Вывод сообщения о проигрыше.
    if (PlayAgain()) {
        wordToGuess = ChooseWord();
        guessedLetters.clear();
        startTime = time(NULL);
        attemptsLeft = maxAttempts;
    }
}

string HangmanGame::ChooseWord() const {
    srand(static_cast<unsigned>(time(NULL)));
    return words[rand() % words.size()];  // Выбор случайного слова.
}

bool HangmanGame::IsGuessed(char guess) const {
    for (char letter : guessedLetters) {
        if (guess == letter) {
            return true;
        }
    }
    return false;
}

bool HangmanGame::IsWinner() const {
    for (char letter : wordToGuess) {
        bool found = false;
        for (char guess : guessedLetters) {
            if (letter == guess) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool HangmanGame::IsLetterInWord(char letter) const {
    for (char l : wordToGuess) {
        if (l == letter) {
            return true;
        }
    }
    return false;
}

string HangmanGame::GetHint() const {
    string hint;
    for (int i = 0; i <= guessedLetters.size(); ++i) {
        hint += wordToGuess[i];
    }
    return hint;
}

void HangmanGame::DisplayWord() const {
    for (char letter : wordToGuess) {
        bool found = false;
        for (char guess : guessedLetters) {
            if (letter == guess) {
                found = true;
                break;
            }
        }
        if (found) {
            cout << letter << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

void HangmanGame::DisplayGameResultWin() const {
    cout << "Поздравляем, вы выиграли! Загаданное слово: " << wordToGuess << endl;
    cout << "Желаете сыграть ещё раз? (да/нет): ";
}

void HangmanGame::DisplayGameResultTimeout() const {
    cout << "Время вышло! Загаданное слово: " << wordToGuess << endl;
    cout << "Желаете сыграть ещё раз? (да/нет): ";
}

void HangmanGame::DisplayGameResultLose() const {
    cout << "У вас закончились попытки! Загаданное слово: " << wordToGuess << endl;
    cout << "Желаете сыграть ещё раз? (да/нет): ";
}

bool HangmanGame::PlayAgain() const {
    string playAgain;
    cin >> playAgain;
    return playAgain == "да";
}

vector<string> HangmanGame::LoadWords(const string& filename) {
    vector<string> loadedWords;
    ifstream file(filename);

    if (!file.is open()) {
        Beep(400, 300);  // Звуковой сигнал при ошибке открытия файла.
        cout << "Ошибка: Не удается открыть файл с загруженными словами." << endl;
    }

    string word;
    while (getline(file, word)) {
        loadedWords.push_back(word);
    }

    return loadedWords;  // Возвращаем вектор загруженных слов.
}

