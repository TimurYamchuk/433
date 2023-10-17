#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

class HangmanGame {
private:
 vector<string> words;  // Вектор загруженных слов.
string wordToGuess;    // Слово, которое нужно угадать.
vector<char> guessedLetters;  // Вектор угаданных букв.
time_t startTime;  // Время начала игры.
int maxTime;  // Максимальное время игры в секундах.
int maxAttempts;  // Максимальное количество попыток (ошибок).


public:
    HangmanGame(const vector<string>& loadedWords);
    void Run();

    string ChooseWord() const;
    bool IsGuessed(char guess) const;
    bool IsWinner() const;
    bool IsLetterInWord(char letter) const;
    string GetHint() const;
    void DisplayWord() const;
    void DisplayGameResultWin() const;
    void DisplayGameResultTimeout() const;
    void DisplayGameResultLose() const;
    bool PlayAgain() const;

    vector<string> LoadWords(const string& filename);
};
