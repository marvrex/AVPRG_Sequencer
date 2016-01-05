#ifndef NODE_H
#define NODE_H

#include "soundobject.h"


class Node
{
public:
    Node();

signals:

public slots:

    int xPos;
    int yPos;
    int colorId;
    int shapeId;
    int channel;

    bool cooldown;

    Node *next;

    //void addNode(int xPos, int yPos, int colorId, int shapeId, int channel, Node *actual);

    void play(Node *actual);

};

#endif // NODE_H
