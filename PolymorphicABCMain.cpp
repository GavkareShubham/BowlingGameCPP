#include <iostream>
#include <vector>
#include <memory>
#include <limits>

// Base abstract class for a Frame
class Frame {
public:
    virtual void inputRolls(int frameNumber) = 0;
    virtual int score(const std::vector<int>& rolls, int& rollIndex) = 0;
    virtual void addRollsToVec(std::vector<int>& rolls) const = 0;
    virtual ~Frame() = default;

protected:
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

// Frame for frames 1-9
class NormalFrame : public Frame {
public:
    int roll1 = 0;
    int roll2 = 0;
    bool isStrike = false;
    bool isSpare = false;

    void inputRolls(int frameNumber) override {
        std::cout << "Frame " << frameNumber << ":\n";
        roll1 = getValidRoll("  Roll1: ", 0, frameNumber);

        if (roll1 == 10) {
            isStrike = true;
            roll2 = 0;
        } else {
            roll2 = getValidRoll("  Roll2: ", roll1, frameNumber);
            if (roll1 + roll2 == 10)
                isSpare = true;
        }
    }

    int score(const std::vector<int>& rolls, int& rollIndex) override {
        int frameScore = 0;
        if (rolls[rollIndex] == 10) {
            // Strike
            frameScore = 10 + rolls[rollIndex + 1] + rolls[rollIndex + 2];
            rollIndex += 1;
        } else if (rolls[rollIndex] + rolls[rollIndex + 1] == 10) {
            // Spare
            frameScore = 10 + rolls[rollIndex + 2];
            rollIndex += 2;
        } else {
            // Open frame
            frameScore = rolls[rollIndex] + rolls[rollIndex + 1];
            rollIndex += 2;
        }
        return frameScore;
    }

    void addRollsToVec(std::vector<int>& rolls) const override {
        rolls.push_back(roll1);
        if (!isStrike)
            rolls.push_back(roll2);
    }
};

// Tenth Frame (frame 10) logic
class TenthFrame : public Frame {
public:
    int roll1 = 0;
    int roll2 = 0;
    int bonus = 0;
    bool isStrike = false;
    bool isSpare = false;

    void inputRolls(int frameNumber) override {
        std::cout << "Frame " << frameNumber << ":\n";
        roll1 = getValidRoll("  Roll1: ", 0, frameNumber);

        if (roll1 == 10) {
            isStrike = true;
            roll2 = getValidRoll("  Roll2: ", 0, frameNumber);
            bonus = getValidRoll("  Bonus Roll 3: ", 0, frameNumber);
        } else {
            roll2 = getValidRoll("  Roll2: ", roll1, frameNumber);
            if (roll1 + roll2 == 10) {
                isSpare = true;
                bonus = getValidRoll("  Bonus Roll 3: ", 0, frameNumber);
            } else {
                bonus = 0;
            }
        }
    }

    int score(const std::vector<int>& rolls, int& rollIndex) override {
        // Always three rolls in rolls vector for 10th frame (bonus may be zero)
        int frameScore = rolls[rollIndex] + rolls[rollIndex + 1] + rolls[rollIndex + 2];
        rollIndex += 3;
        return frameScore;
    }

    void addRollsToVec(std::vector<int>& rolls) const override {
        rolls.push_back(roll1);
        rolls.push_back(roll2);
        rolls.push_back(bonus);
    }
};

class BowlingGame {
private:
    std::vector<std::unique_ptr<Frame>> frames;
    std::vector<int> rolls;

public:
    void playGame() {
        std::cout << "Bowling Scores:\n";
        for (int i = 1; i <= 10; ++i) {
            if (i < 10)
                frames.push_back(std::make_unique<NormalFrame>());
            else
                frames.push_back(std::make_unique<TenthFrame>());
            frames.back()->inputRolls(i);
            frames.back()->addRollsToVec(rolls);
        }
    }

    void calculateScore() {
        int score = 0;
        int rollIndex = 0;

        std::cout << "\n--- Frame-by-Frame Score ---\n";
        for (int i = 0; i < 10; ++i) {
            int frameScore = frames[i]->score(rolls, rollIndex);
            score += frameScore;
            // Print info
            std::string frameType;
            if (i < 9) {
                auto* nf = dynamic_cast<NormalFrame*>(frames[i].get());
                if (nf->isStrike)
                    frameType = "Strike!";
                else if (nf->isSpare)
                    frameType = "Spare!";
            } else {
                auto* tf = dynamic_cast<TenthFrame*>(frames[i].get());
                if (tf->isStrike)
                    frameType = "Strike!";
                else if (tf->isSpare)
                    frameType = "Spare!";
            }
            std::cout << "Frame " << (i + 1) << ": " << frameType << " Total Score = " << score << "\n";
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
