#include "mainwindow.h"
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


    midiOutput.open("Microsoft GS Wavetable Synth");

    void addNode(int xPos, int yPos, int colorId, int shapeId, int channel, Node *actual);
    void deleteNode(int channel, Node *actual);
    void sound(Node *actual);

    Node *start = new Node;
    start->next = NULL;
    Node *actual = start;

    addNode(9,7,127,1,0,start);
    addNode(5,7,127,10,1,start);
    addNode(7,7,127,15,2,start);

    deleteNode(1, start);
    deleteNode(2, start);

    actual = start;



  //  SoundObject *o = new SoundObject(4,7,127,1,0);


   // SoundObject *o1 = new SoundObject(10,7,127,10,1);


    sound(actual);







   /* while(true){
        //o->play(o->xPos,o->yPos,o->colorId,o->shapeId,o->channel);
        midiOutput.sendProgram(o->channel, o->shapeId);
        midiOutput.sendNoteOn(o->channel, o->yPos+60, o->colorId);


        for(int i = 0; i<o->xPos;i++){

            usleep(100000);
        }
        midiOutput.sendProgram(o1->channel, o1->shapeId);
        midiOutput.sendNoteOn(o1->channel, o1->yPos+60, o1->colorId);

        for(int i = 0; i<o1->xPos;i++){

            usleep(100000);
        }

        //t1.join();

    }*/


}


void addNode(int xPos, int yPos, int colorId, int shapeId, int channel, Node *actual){
    while(actual->next!=NULL){
        actual = actual->next;
    }
    Node *node = new Node;
    node->xPos = xPos;
    node->yPos = yPos;
    node->colorId = colorId;
    node->shapeId = shapeId;
    node->channel = channel;
    node->next = NULL;

    actual->next = node;
}

void deleteNode(int channel, Node *actual){

    Node *node = NULL;

    while(actual->next!=NULL){
        node = actual->next;

        if(node->channel == channel){
            if(node->next!=NULL){

                actual->next = node->next;
                break;

            }else{
                actual->next = NULL;
                break;

            }
        }


        actual = actual->next;
    }
}

void sound(Node *actual){

    while(actual != NULL){

        int wait = actual->xPos;
        if(actual->cooldown == false){

            midiOutput.sendProgram(actual->channel, actual->shapeId);
            midiOutput.sendNoteOn(actual->channel, actual->yPos+60, actual->colorId);
            //actual->play(actual);


        }


        usleep(100000*wait);



        actual = actual->next;
    }



}
