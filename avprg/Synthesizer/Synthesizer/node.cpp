#include "node.h"
#include "soundobject.h"
#include "midioutput.h"
#include <stdlib.h>
#include <unistd.h>


Node::Node()
{

}


void play(Node *actual){

    actual->cooldown = true;

    int wait = actual->xPos;

    //midiOutput.sendProgram(actual->channel, actual->shapeId);
    //midiOutput.sendNoteOn(actual->channel, actual->yPos+60, actual->colorId);


    usleep(100000*wait);

    actual->cooldown = false;

}
