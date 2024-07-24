#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct Voter {
    int id;
    string name;
    string password;
    bool hasVoted;
};

struct Candidate {
    int id;
    string name;
    int voteCount;
};

class EVotingSystem {
private:
    vector<Voter> voters;
    vector<Candidate> candidates;
    map<int, int> votes; // voter ID -> candidate ID

    bool isVoterIdUnique(int id) {
        return find_if(voters.begin(), voters.end(), [id](Voter &v) { return v.id == id; }) == voters.end();
    }

    bool isCandidateIdUnique(int id) {
        return find_if(candidates.begin(), candidates.end(), [id](Candidate &c) { return c.id == id; }) == candidates.end();
    }

public:
    void registerVoter(int id, string name, string password) {
        if (isVoterIdUnique(id)) {
            voters.push_back({id, name, password, false});
            cout << "Voter " << name << " registered successfully." << endl;
        } else {
            cout << "Voter ID already exists." << endl;
        }
    }

    void addCandidate(int id, string name) {
        if (isCandidateIdUnique(id)) {
            candidates.push_back({id, name, 0});
            cout << "Candidate " << name << " added successfully." << endl;
        } else {
            cout << "Candidate ID already exists." << endl;
        }
    }

    void castVote(int voterId, string password, int candidateId) {
        for (auto &voter : voters) {
            if (voter.id == voterId) {
                if (voter.password != password) {
                    cout << "Incorrect password." << endl;
                    return;
                }
                if (voter.hasVoted) {
                    cout << "Voter has already voted." << endl;
                    return;
                } else {
                    voter.hasVoted = true;
                    votes[voterId] = candidateId;
                    for (auto &candidate : candidates) {
                        if (candidate.id == candidateId) {
                            candidate.voteCount++;
                            cout << "Vote cast successfully." << endl;
                            return;
                        }
                    }
                    cout << "Candidate not found." << endl;
                    return;
                }
            }
        }
        cout << "Voter not found." << endl;
    }

    void displayResults() {
        cout << "Election Results:" << endl;
        for (const auto &candidate : candidates) {
            cout << candidate.name << " received " << candidate.voteCount << " votes." << endl;
        } 
    }

    void displayCandidates() {
        cout << "Candidates:" << endl;
        for (const auto &candidate : candidates) {
            cout << candidate.id << ": " << candidate.name << endl;
        }
    }

    void saveData() {
        ofstream outFile("voting_data.txt");
        if (outFile.is_open()) {
            outFile << voters.size() << endl;
            for (const auto &voter : voters) {
                outFile << voter.id << " " << voter.name << " " << voter.password << " " << voter.hasVoted << endl;
            }
            outFile << candidates.size() << endl;
            for (const auto &candidate : candidates) {
                outFile << candidate.id << " " << candidate.name << " " << candidate.voteCount << endl;
            }
            outFile.close();
            cout << "Data saved successfully." << endl;
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    void loadData() {
        ifstream inFile("voting_data.txt");
        if (inFile.is_open()) {
            size_t voterCount, candidateCount;
            inFile >> voterCount;
            voters.clear();
            for (size_t i = 0; i < voterCount; ++i) {
                Voter voter;
                inFile >> voter.id >> voter.name >> voter.password >> voter.hasVoted;
                voters.push_back(voter);
            }
            inFile >> candidateCount;
            candidates.clear();
            for (size_t i = 0; i < candidateCount; ++i) {
                Candidate candidate;
                inFile >> candidate.id >> candidate.name >> candidate.voteCount;
                candidates.push_back(candidate);
            }
            inFile.close();
            cout << "Data loaded successfully." << endl;
        } else {
            cout << "Unable to open file for loading." << endl;
        }
    }
};

int main() {
    EVotingSystem system;
    system.loadData();

    int choice;
    do {
        cout << "\nE-Voting System Menu:\n";
        cout << "1. Register Voter\n";
        cout << "2. Add Candidate\n";
        cout << "3. Cast Vote\n";
        cout << "4. Display Candidates\n";
        cout << "5. Display Results\n";
        cout << "6. Save Data\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            int id;
            string name, password;
            cout << "Enter Voter ID: ";
            cin >> id;
            cout << "Enter Voter Name: ";
            cin >> name;
            cout << "Enter Password: ";
            cin >> password;
            system.registerVoter(id, name, password);
            break;
        }
        case 2: {
            int id;
            string name;
            cout << "Enter Candidate ID: ";
            cin >> id;
            cout << "Enter Candidate Name: ";
            cin >> name;
            system.addCandidate(id, name);
            break;
        }
        case 3: {
            int voterId, candidateId;
            string password;
            cout << "Enter Voter ID: ";
            cin >> voterId;
            cout << "Enter Password: ";
            cin >> password;
            system.displayCandidates();
            cout << "Enter Candidate ID: ";
            cin >> candidateId;
            system.castVote(voterId, password, candidateId);
            break;
        }
        case 4:
            system.displayCandidates();
            break;
        case 5:
            system.displayResults();
            break;
        case 6:
            system.saveData();
            break;
        case 7:
            cout << "Exiting..." << endl;
            system.saveData();
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
