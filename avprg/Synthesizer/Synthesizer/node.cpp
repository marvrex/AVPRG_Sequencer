#include "node.h"
#include "soundobject.h"
#include "midioutput.h"
#include <stdlib.h>
#include <unistd.h>


Node::Node()
{

}


void Node::wait(Node *actual){

    actual->cooldown = true;

    int wait = actual->xPos;

    usleep(100000*wait);

    actual->cooldown = false;

}
