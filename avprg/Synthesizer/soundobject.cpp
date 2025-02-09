#include "soundobject.h"
#include "midiinput.h"
#include "midioutput.h"
#include <stdlib.h>
//#include <unistd.h>
#include "node.h"
#include <iostream>
#include "windows.h"


using namespace std;


SoundObject::SoundObject(){

    QStringList connections = midiOutput.connections(true);

    midiOutput.open("Microsoft GS Wavetable Synth");

    this->start = new Node();
    start->next = NULL;
    this->actual = start;


}

SoundObject::~SoundObject(){
}


void SoundObject::addNode(int xPos, int yPos, int colorId, int shapeId, int channel){
    while(this->actual->next!=NULL){
        this->actual = this->actual->next;
    }
    Node *node = new Node;
    node->xPos = xPos;
    node->yPos = yPos;
    node->colorId = colorId;
    node->shapeId = shapeId;
    node->channel = channel;
    node->next = NULL;

    this->actual->next = node;
}

void SoundObject::deleteNode(int channel){
	Node *node = NULL;
	this->actual = this->start;
	while (this->actual->next != NULL){
		node = this->actual->next;
		if (node->channel == channel){
			if (node->next != NULL){
				actual->next = node->next;
				break;
			}
			else{
				actual->next = NULL;
				break;
			}
		}
		this->actual = this->actual->next;
	}
}

void SoundObject::play(){
	while (this->actual != NULL){
		midiOutput.sendProgram(this->actual->channel, this->actual->shapeId);
		midiOutput.sendNoteOn(this->actual->channel, this->actual->yPos + 60, this->actual->colorId);
		//this->actual->wait(this->actual);
		//Sleep(10 * this->actual->xPos);
		this->actual = this->actual->next;
	}
	this->actual = this->start;
}



void SoundObject::show(){
    this->actual = this->start->next;
    while(this->actual->next != NULL){
        cout << "Ton Nummer " << this->actual->channel <<", ";
        this->actual = this->actual->next;
    }
    cout << "Ton Nummer " << this->actual->channel;
}

void SoundObject::deleteAll(){
	this->actual = this->start;
	while (this->actual->next != NULL){
		midiOutput.sendNoteOff(this->actual->channel, this->actual->yPos + 60, this->actual->colorId);
		this->actual = this->actual->next;
	}
	midiOutput.sendNoteOff(this->actual->channel, this->actual->yPos + 60, this->actual->colorId);
	this->start->next = NULL;
	this->actual = this->start;
}
