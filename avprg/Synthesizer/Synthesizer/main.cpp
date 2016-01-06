
#include <QApplication>
#include "soundobject.h"
#include "midioutput.h"
#include <stdlib.h>
#include <unistd.h>
#include "node.h"

#include "iostream"

using namespace std;

drumstick::rt::MIDIOutput midiOutput;

int main(int argc, char *argv[])
{



    SoundObject *o = new SoundObject();

    o->addNode(9,7,127,1,0);
    o->addNode(5,7,127,10,1);
    o->addNode(7,7,127,15,2);

    o->deleteNode(1);
    o->deleteNode(2);


    while(true){
        o->play();

    }


}

