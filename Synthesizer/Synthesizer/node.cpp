#include "node.h"
#include "soundobject.h"
#include "midioutput.h"
#include <stdlib.h>
#include "windows.h"
//#include <unistd.h>


Node::Node()
{

}


void Node::wait(Node *actual){

    actual->cooldown = true;

    int wait = actual->xPos;

<<<<<<< HEAD
    usleep(100000*wait);
=======
    Sleep(10*wait);
>>>>>>> origin/master

    actual->cooldown = false;

}
