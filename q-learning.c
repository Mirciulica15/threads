//
// Created by mircea.talu on 9/10/2024.
//

#include "q-learning.h"

int env[2][2];
float q_table[4][4];

void update_q_table() {

}

void start_q_learning(int environment[2][2]) {
    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 2; j++) {
            env[i][j] = environment[i][j];
        }
    }
}