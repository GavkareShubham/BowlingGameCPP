#include <iostream>
#include <vector>
#include <limits>

class Frame {
public:
    int roll1 = 0;
    int roll2 = 0;
    int bonus = 0; // Only for 10th frame
    bool isStrike = false;
    bool isSpare = false;

    void inputRolls(int frameNumber) {
        std::cout << "Frame " << frameNumber << ":\n";

        roll1 = getValidRoll("  Roll1: ");

        if (frameNumber < 10) {
            if (roll1 == 10) {
                isStrike = true;
                return;
            }

            roll2 = getValidRoll("  Roll2: ", roll1, frameNumber);
            if (roll1 + roll2 == 10) {
                isSpare = true;
            }
        }
        else {
            // Frame 10 logic
            if (roll1 == 10) {
                isStrike = true;
                roll2 = getValidRoll("  Roll2: ");
                bonus = getValidRoll("  Bonus Roll 3: ");
            } else {
                roll2 = getValidRoll("  Roll2: ", roll1, frameNumber);
                if (roll1 + roll2 == 10) {
                    isSpare = true;
                    bonus = getValidRoll("  Bonus Roll 3: ");
                }
            }
        }
    }

    static int getValidRoll(const std::string& prompt, int firstRoll = 0, int frameNumber = 0) {
        int pins;
        while (true) {
            std::cout << prompt;
            if (!(std::cin >> pins) || pins < 0 || pins > 10 || (frameNumber < 10 && firstRoll + pins > 10)) {
                std::cout << "Invalid input. Please enter a number between 0 and 10.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        return pins;
    }
};

class BowlingGame {
private:
    std::vector<Frame> frames;
    std::vector<int> rolls;

public:
    void playGame() {
        std::cout << "Bowling Scores:\n";
        for (int i = 1; i <= 10; ++i) {
            Frame frame;
            frame.inputRolls(i);
            rolls.push_back(frame.roll1);
            if (i < 10) {
                if (!frame.isStrike) {
                    rolls.push_back(frame.roll2);
                }
            } else {
                // Frame 10: always add roll2 and maybe bonus
                rolls.push_back(frame.roll2);
                if (frame.isStrike || frame.isSpare) {
                    rolls.push_back(frame.bonus);
                }
            }
            frames.push_back(frame);
        }
    }

    void calculateScore() {
        int score = 0;
        int rollIndex = 0;

        std::cout << "\n--- Frame-by-Frame Score ---\n";
        for (int i = 0; i < 10; ++i) {
            if (rolls[rollIndex] == 10) {
                // Strike
                score += 10 + rolls[rollIndex + 1] + rolls[rollIndex + 2];
                std::cout << "Frame " << (i + 1) << ": Strike! Total Score = " << score << "\n";
                rollIndex += 1;
            }
            else if (rolls[rollIndex] + rolls[rollIndex + 1] == 10) {
                // Spare
                score += 10 + rolls[rollIndex + 2];
                std::cout << "Frame " << (i + 1) << ": Spare! Total Score = " << score << "\n";
                rollIndex += 2;
            }
            else {
                // Open
                score += rolls[rollIndex] + rolls[rollIndex + 1];
                std::cout << "Frame " << (i + 1) << ": Total Score = " << score << "\n";
                rollIndex += 2;
            }
        }

        std::cout << "\nFinal Total Score: " << score << std::endl;
    }
};

int main() {
    BowlingGame game;
    game.playGame();
    game.calculateScore();
    return 0;
}
