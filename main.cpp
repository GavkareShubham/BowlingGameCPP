#include <iostream>
#include <vector>
using namespace std;

int main() {

    vector<int> rolls = {10, 7, 3, 9, 0, 10, 0, 8, 8, 2, 0, 6, 10, 10, 10, 8, 1};

    int score = 0;
    int rollIndex = 0;

    // Calculating score for frame by frame 
    for (int frame = 1; frame <= 10; ++frame) {
        if (frame < 10) {
            if (rolls[rollIndex] == 10) {
                score += 10 + rolls[rollIndex+1] + rolls[rollIndex+2];
                rollIndex += 1;
            }
            else if (rolls[rollIndex] + rolls[rollIndex+1] == 10) {
                score += 10 + rolls[rollIndex+2];
                rollIndex += 2;
            }
            else {
                score += rolls[rollIndex] + rolls[rollIndex+1];
                rollIndex += 2;
            }
        }
        else {
            score += rolls[rollIndex] + rolls[rollIndex+1];
            if (rolls[rollIndex] == 10 || rolls[rollIndex] + rolls[rollIndex+1] == 10) {
                score += rolls[rollIndex+2];
            }
        }
    }

    cout << "Total score: " << score << endl;  
    return 0;
}
