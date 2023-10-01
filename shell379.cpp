#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>
using namespace std;

int main() {
    string line;
    string token;
    while(true) {
        cout << "SHELL379: ";
        getline(cin, line);

        if (line == "exit") {
            break;
        }

        istringstream iss(line);
        vector<string> tokens;

        while (iss) {
            iss >> token;
            tokens.push_back(token);
        }

        char* args[tokens.size() + 1];
        for (size_t i = 0; i < tokens.size(); i++) {
            args[i] = const_cast<char*>(tokens[i].c_str());
        }
        args[tokens.size()] = NULL;

        int rc = fork();
        if(rc = (-1)) {
            cout << "fork failed" << endl;
            return 1;
        } else if (rc == 0) {
            cout << "running baby program";
            execvp(args[0], args);
            perror("Failed to execute command");
            exit(1);
            }
        }
    return 0;
}