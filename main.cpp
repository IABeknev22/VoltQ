#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "include/json.hpp"

using namespace std;
using json = nlohmann::json;

// define question's structure 
struct Question {
    string question;
    vector<string> options;
    string answer;
};

int main() {
    // read json file with questions
    ifstream file("questions.json");
    json j;
    file >> j;

    // define map to hold subjects, subtests, and questions
    map<string, map<string, vector<Question>>> subjects;

    // copy json data into structured map
    for (auto& subject : j.items()) {

        map<string, vector<Question>> subtests;

        for (auto& subtest : subject.value().items()) {

            vector<Question> questions;

            // copy individual questions from json
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

    // show available subjects
    cout << "Select subject:\n";
    int i = 1;
    for (const auto& subject : subjects) {
        cout << i++ << ") " << subject.first << "\n";
    }

    // get user input for selected subject
    int selected_subject;
    cin >> selected_subject;

    // get the selected subject from the map
    auto it = subjects.begin();
    advance(it, selected_subject - 1);
    cout << "You selected " << it->first << "\n";

    // show available subtests for the selected subject
    cout << "Select subtest:\n";
    i = 1;
    for (const auto& subtest : it->second) {
        cout << i++ << ") " << subtest.first << "\n";
    }

    // get user input for selected subtest
    int selected_subtest;
    cin >> selected_subtest;

    // get the selected subtest from the map
    auto it_subtest = it->second.begin();
    advance(it_subtest, selected_subtest - 1);
    cout << "You selected " << it_subtest->first << "\n";

    // start the test and calculate the score
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

    // demonstrate final score
    cout << "Your final score is " << score << "/" << it_subtest->second.size() << "\n";

    // wait for user to press ENTER
    cout << "Press ENTER to exit...";
    // prevent program from closing
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get();
    return 0;
}