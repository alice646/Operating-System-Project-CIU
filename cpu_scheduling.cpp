#include <algorithm>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

struct Process {
  int attributes[6];
};



void writeToFile(string output) {
  ofstream file;
  file.open(output);
  cout << "Enter your message: ";
  file << "Average: ms\n";
  file.close();
}

char *inputFileName = nullptr;
char *outputFileName = nullptr;

int main(int argc, char *argv[]) {
  int option;
  if (argc < 5) {
    cout << "You should introduce 2 arguments";
    exit(0);
  }
  while ((option = getopt(argc, argv, "f:o:")) != -1) {
    switch (option) {
    case 'f':
      inputFileName = optarg;
      break;
    case 'o':
      outputFileName = optarg;
      break;
    default:
      break;
    }
  }
  if (inputFileName == nullptr || outputFileName == nullptr) {
    cout << "Use the parameter -f for input and -o for output";
    exit(0);
  }
  int processCount = 0;
  vector<Process> tempArray{};
  ifstream file(inputFileName);
  string line;
  while (getline(file, line)) {
    vector<string> words{};
    string delimiter = ":";
    int pos;
    string text = line;
    while ((pos = text.find(delimiter)) != string::npos) {
      words.push_back(text.substr(0, pos));
      text.erase(0, pos + delimiter.length());
    }
    int attributes[6];
    int count = 0;
    for (const auto &word : words) {
      attributes[count] = stoi(word);
      count++;
    }
    attributes[2] = stoi(text);
    attributes[3] = 0;
    attributes[4] = 0;
    attributes[5] = 0;
    Process process = {{attributes[0], attributes[1], attributes[2],
                        attributes[3], attributes[4], attributes[5]}};
    tempArray.push_back(process);
    processCount++;
  }
  Process processes[processCount];
  int j = 0;
  for (const auto &element : tempArray) {
    processes[j] = element;
    j++;
  }
  int userChoice;
  bool isFinished = false;
  while (isFinished == false) {
    cout << "Choose Action: \n";
    cout << "1. Preemptive Scheduling\n";
    cout << "2. Non-Preemptive Scheduling\n";
    cout << "3. Show Result\n";
    cout << "4. End Program\n";
    cin >> userChoice;
    switch (userChoice) {
    case 1: {
      int preemptiveSchedulingType;
      cout << "Choose Between the five Preemptive Scheduling Types \n";
      cout << "1. None\n";
      cout << "2. First Come, First Served Scheduling\n";
      cout << "3. Shortest-Job-First Scheduling\n";
      cout << "4. Priority Scheduling\n";
      cout << "5. Round-Robin Scheduling\n";
      
    } break;
    case 2: {
      int nonPreemptiveSchedulingType;
      cout << "Choose Between the five Non-Preemptive Scheduling Types \n";
      cout << "1. None\n";
      cout << "2. First Come, First Served Scheduling\n";
      cout << "3. Shortest-Job-First Scheduling\n";
      cout << "4. Priority Scheduling\n";
      cin >> nonPreemptiveSchedulingType;
    } break;
    default:
      cout << "You have to choose between 1 - 4";
      break;
    }
  }
  return 0;
}
