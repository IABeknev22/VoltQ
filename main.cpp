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

    map<string, map<string, vector<Question>>> subjects;

    for (auto& subject : j.items()) {

        map<string, vector<Question>> subtests;

        for (auto& subtest : subject.value().items()) {

            vector<Question> questions;

            for (auto& question : subtest.value()) {
                Question newQuestion;
                newQuestion.question = question["question"];
                newQuestion.options = question["options"].get<vector<string>>();
                newQuestion.answer = question["answer"];

                questions.push_back(newQuestion);
            }

            subtests[subtest.key()] = questions;
        }

        subjects[subject.key()] = subtests;
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

    cout << "Select subtest:\n";
    i = 1;
    for (const auto& subtest : it->second) {
        cout << i++ << ") " << subtest.first << "\n";
    }

    int selected_subtest;
    cin >> selected_subtest;

    auto it_subtest = it->second.begin();
    advance(it_subtest, selected_subtest - 1);
    cout << "You selected " << it_subtest->first << "\n";

    int score = 0;
    for (const auto& question : it_subtest->second) {
        cout << question.question << "\n";
        for (int i = 0; i < question.options.size(); i++) {
            cout << i+1 << ") " << question.options[i] << "\n";
        }
        int answer;
        cin >> answer;
        if (question.options[answer-1] == question.answer) {
            score++;
        }
    }

    cout << "Your final score is " << score << "/" << it_subtest->second.size() << "\n";

    cout << "Press ENTER to exit...";
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    return 0;
}