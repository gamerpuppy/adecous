//
// Created by Keegan Millard on 2019-05-12.
//

#ifndef AUDECOUS_MANAGE_H
#define AUDECOUS_MANAGE_H

#include "Solver.h"
#include <string>

uint64_t solve(uint64_t lb, uint64_t ub, int tc);

void generatePicure(const std::string &filepath, uint64_t lb, uint64_t ub, uint64_t width);

#endif //AUDECOUS_MANAGE_H
