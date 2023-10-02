#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>
using namespace std;

struct Job {
    int pid;
    string cmd;
    string status;
    // int sec;
};

void checkCompletedJobs(vector<Job>& jobs) {
    for (auto it = jobs.begin(); it != jobs.end();) {
        if (waitpid(it->pid, NULL, WNOHANG) > 0) {
            it = jobs.erase(it);
        } else {
            ++it;
        }
    }
}

int main() {
    string line;
    string token;

    vector<Job> jobs;

    while(true) {
        cout << "SHELL379: ";
        getline(cin, line);

        checkCompletedJobs(jobs);

        if (line == "exit") {
            break;
        } else if (line == "jobs") {
            cout << "PID\tS\tcommand" << endl;
        for (const Job& job : jobs) {
            cout << job.pid << "\t" << job.status << "\t" << job.cmd << endl;
            }
            continue;
        } else if (line.empty()){
            continue;
        }

        istringstream iss(line);
        vector<string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }

        bool is_background = false;
        if (tokens.back() == "&") {
            is_background = true;
            tokens.pop_back();
        }

        char* args[tokens.size() + 1];
        for (size_t i = 0; i < tokens.size(); i++) {
            args[i] = const_cast<char*>(tokens[i].c_str());
        }
        args[tokens.size()] = NULL;

        int rc = fork();
        if(rc < 0) {
            cout << "fork failed" << endl;
            return 1;
        } else if (rc == 0) {
            cout.flush();
            execvp(args[0], args);
            perror("Failed to execute command");
            exit(1);
        } else {
            if(is_background) {
                Job newjob = {rc, line, is_background ? "B" : "R"};
                jobs.push_back(newjob);
            }
            waitpid(rc, NULL, 0);
        }
        }
    return 0;
}