//
//  aihelper.hpp
//  dominion
//
//  Created by Kyle Hess on 8/27/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#ifndef aihelper_hpp
#define aihelper_hpp

#include <set>
#include <string>
using namespace std;

struct BoardState;

// UNFINISHED
// This is what each level of recommendation means:
//   0: Don't consider
//   0.5: Decide for yourself
//   1: Choose automatically
// Maybe make the above system more complicated in the future, but for now let's stick to simplicity
set<pair<double, string>> getWeightedChoices(BoardState* game);

#endif /* aihelper_hpp */
