#pragma once

struct Connection {
    int fromNodeID;
    int toNodeID;

    Connection(int from, int to)
        : fromNodeID(from), toNodeID(to) {
    }
};
