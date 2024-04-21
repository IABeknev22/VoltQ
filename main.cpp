#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "include/json.hpp"

using namespace std;
using json = nlohmann::json;

struct Question {
    string question;
    vector<string> options;
    string answer;
};

int main() {
    ifstream file("questions.json");
    json j;
    file >> j;

    map<string, vector<Question>> subjects;

    for (auto& element : j.items()) {
        vector<Question> questions;
        for (auto& question : element.value()) {
            questions.push_back({ question["question"], question["options"].get<vector<string>>(), question["answer"] });
        }
        subjects[element.key()] = questions;
    }

    cout << "Select subject:\n";
    int i = 1;
    for (const auto& subject : subjects) {
        cout << i++ << ") " << subject.first << "\n";
    }

    int selected_subject;
    cin >> selected_subject;

    auto it = subjects.begin();
    advance(it, selected_subject - 1);
    cout << "You selected " << it->first << "\n";

    int score = 0;
    for (const auto& question : it->second) {
        cout << question.question << "\n";
        for (int i = 0; i < question.options.size(); i++) {
            cout << i + 1 << ") " << question.options[i] << "\n";
        }
        int answer;
        cin >> answer;
        if (question.options[answer - 1] == question.answer) {
            score++;
        }
    }

    cout << "Your final score is " << score << "/" << it->second.size() << "\n";

    cout << "Press ENTER to exit...";
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    return 0;
}
