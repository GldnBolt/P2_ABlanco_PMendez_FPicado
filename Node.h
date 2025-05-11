//
// Created by Xpc on 10/5/2025.
//

#ifndef NODE_H
#define NODE_H

struct Node {
    int x, y;
    int gCost, hCost;
    Node* parent;

    Node(int x, int y, int gCost, int hCost, Node* parent = nullptr);
    int fCost() const;
    bool operator>(const Node& other) const;
};

#endif

