#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "HangmanGame.h"
#include <windows.h> // Подключение библиотеки для работы с Windows и Beep.

using namespace std;

int main() {
    HangmanGame words = words.LoadWords("words.txt");  // Загрузка слов из файла.
    HangmanGame game(words);  // Создание объекта игры.
    game.Run();  // Запуск игры.
    return 0;
}
