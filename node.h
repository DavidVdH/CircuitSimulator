#ifndef NODE_H
#define NODE_H

#include "component.h"

using namespace std;

class Node : public Component
{
public:
    int nodeId;
    static int numberOfNodes;

    Node();
    Node(string);
    ~Node();

    // Must be overlaoded because the general implementation
    // is based on a component with two connection points
    void updateConnectionPoints();
};

#endif // NODE_H
