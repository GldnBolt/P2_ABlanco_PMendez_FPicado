//
// Created by Xpc on 10/5/2025.
//

#include "Node.h"

Node::Node(int x, int y, int gCost, int hCost, Node* parent)
    : x(x), y(y), gCost(gCost), hCost(hCost), parent(parent) {}

int Node::fCost() const {
    return gCost + hCost;
}

bool Node::operator>(const Node& other) const {
    return fCost() > other.fCost();
}

