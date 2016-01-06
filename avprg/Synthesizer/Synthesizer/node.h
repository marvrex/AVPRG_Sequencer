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

    void wait(Node *actual);

};

#endif // NODE_H
