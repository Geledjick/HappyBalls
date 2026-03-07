#pragma once

#include "Ball.cpp"
#include "Game.cpp"

class FiedlChecker {
public:
    static const bool check(Game *game) {

        Matrix2<Ball> &data = game->getField().getData();
        ScoreBar &scoreBar = game->getScoreBar();
        
        bool checked = false;
        for (unsigned int r = 0; r < data.getRows(); r++) {
            for (unsigned int c = 0; c < data.getCols(); c++) {
                Ball &targetBall = data.at(r, c);
                if (targetBall.type == BALL_NONE_TYPE) {
                    continue;
                }
                
                if (
                    data.check(r + 1, c) &&
                    data.check(r + 2, c) &&
                    targetBall.type == data.at(r + 1, c).type &&
                    targetBall.type == data.at(r + 2, c).type
                ) {
                    targetBall.type = BALL_NONE_TYPE;
                    data.at(r + 1, c).type = BALL_NONE_TYPE;
                    data.at(r + 2, c).type = BALL_NONE_TYPE;

                    int len = 3;
                    while (
                        data.check(r + len, c) &&
                        targetBall.type == data.at(r + len, c).type
                    ) {
                        data.at(r + len, c).type = BALL_NONE_TYPE;
                        len++;
                    }

                    scoreBar.addScore(len);
                    scoreBar.incCombo();
                }
                
                if (
                    data.check(r, c + 1) &&
                    data.check(r, c + 2) &&
                    targetBall.type == data.at(r, c + 1).type &&
                    targetBall.type == data.at(r, c + 2).type
                ) {
                    targetBall.type = BALL_NONE_TYPE;
                    data.at(r, c + 1).type = BALL_NONE_TYPE;
                    data.at(r, c + 2).type = BALL_NONE_TYPE;

                    int len = 3;
                    while (
                        data.check(r, c + len) &&
                        targetBall.type == data.at(r, c + len).type
                    ) {
                        data.at(r, c + len).type = BALL_NONE_TYPE;
                        len++;
                    }

                    scoreBar.addScore(len);
                    scoreBar.incCombo();
                }
                
                if (
                    data.check(r + 1, c + 1) &&
                    data.check(r + 2, c + 2) &&
                    targetBall.type == data.at(r + 1, c + 1).type &&
                    targetBall.type == data.at(r + 2, c + 2).type
                ) {
                    targetBall.type = BALL_NONE_TYPE;
                    data.at(r + 1, c + 1).type = BALL_NONE_TYPE;
                    data.at(r + 2, c + 2).type = BALL_NONE_TYPE;

                    int len = 3;
                    while (
                        data.check(r + len, c + len) &&
                        targetBall.type == data.at(r + len, c + len).type
                    ) {
                        data.at(r + len, c + len).type = BALL_NONE_TYPE;
                        len++;
                    }

                    scoreBar.addScore(len);
                    scoreBar.incCombo();
                }
                
                if (
                    data.check(r - 1, c - 1) &&
                    data.check(r - 2, c - 2) &&
                    targetBall.type == data.at(r - 1, c - 1).type &&
                    targetBall.type == data.at(r - 2, c - 2).type
                ) {
                    targetBall.type = BALL_NONE_TYPE;
                    data.at(r - 1, c - 1).type = BALL_NONE_TYPE;
                    data.at(r - 2, c - 2).type = BALL_NONE_TYPE;

                    int len = 3;
                    while (
                        data.check(r - len, c - len) &&
                        targetBall.type == data.at(r - len, c - len).type
                    ) {
                        data.at(r - len, c - len).type = BALL_NONE_TYPE;
                        len++;
                    }

                    scoreBar.addScore(len);
                    scoreBar.incCombo();
                }
            }
        }

        return checked;
    }
};