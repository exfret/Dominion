//
//  event.hpp
//  dominion
//
//  Created by Kyle Hess on 8/29/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#ifndef event_hpp
#define event_hpp

#include <functional>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct BoardState;

struct Event {
    string eventTypeName;
    vector<void*> data;
    set<string> triggerBuckets;
    // TODO: Figure out how to correctly const-qualify this expression
    function<bool(BoardState*, vector<string>)> checkIfTriggered;
    // Always assume that events are called by commands of the form "resolve [params]"
    // Also, return a string of what to output
    function<string(BoardState*, vector<string>)> onCall;
    function<string(BoardState*)> getEventString;
};

#endif /* event_hpp */




































