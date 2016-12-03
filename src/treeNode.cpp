/*
 * treeNode.cpp
 *
 *  Created on: May 18, 2016
 *      Author: andy
 */

#include "treeNode.h"

treeNode::treeNode(treeNode* parent, bool rootFlag, graph& graf, int state, Mat myVisibleMap){
	this->parent = parent;
	this->value = 0;
	this->nPulls = 0;
	this->rootFlag = rootFlag;
	this->myState = state;
	this->myVisibleMap = myVisibleMap;
}

treeNode::getChildren(graph& graf){
	// for all available actions at state
	for(int i=0; i<graf[this->myState].nbrs.size(); i++){
		treeNode a(&this, false);
		children.push_back(a);
	}
}

treeNode::exploitTree(vector<int>& myPath){
	if(this->rootFlag){
		myPath.clear();
	}
	myPath.push_back(this->myState);
	int s = this->greedyChildSelect();
	this->children[s].exploitTree(myPath);
}

treeNode::searchTree(float& passedValue){
	this->nPulls++;
	if(this->nPulls > 1){ // am I getting tried for the 1st time?
		int s = this->UCBChildSelect();
		this->children[s].searchTree(passedValue);
		this->updateMyValue(passedValue);
	}
	else{ // my 1st time!
		int s = this->greedyChildSelect(); // choose the best child to represent me for now
		passedValue = this->children[s].value; // set as passed value
	}
}

treeNode::updateMyValue(float passedValue){
	// greedy
	if(this->value < passedValue){
		this->value = passedValue;
	}
	// average
	//this->value = (this->value * this->nPulls + passedValue) / (this->nPulls + 1);
}


int treeNode::UCBChildSelect(){
	int s = -1; // argmax_(each child) [value(each child) + sqrt(2*ln n(all children) / n(each child]
	float v = -1;
	for(size_t i=0; i<this->children.size(); i++){
		float tv = children[i].value + sqrt( log(this->pulls) / children[i].pulls);
		if(tv > v){
			v = tv;
			s = i;
		}
	}
	return s;
}

int treeNode::greedyChildSelect(){
	int s = -1;
	float v = -1;
	for(size_t i=0; i<this->children.size(); i++){
		float tv = children[i].value;
		if(tv > v){
			v = tv;
			s = i;
		}
	}
	return s;
}

int treeNode::eGreedyChildSelect(float epsilon = 0.5){
	int s = -1;
	if( (rand() % 10000)/10000 > epsilon){
		float v = -1;
		for(size_t i=0; i<this->children.size(); i++){
			float tv = children[i].value;
			if(tv > v){
				v = tv;
				s = i;
			}
		}
	}
	else{
		s = (rand() % children.size()) + 1;
	}
	return s;
}

int treeNode::randChildSelect(){
	int s = (rand() % children.size()) + 1;
	return s;
}


int treeNode::simAnnealingChildSelect(float& temp, float cooling = 0.99){
	if(this->rootFlag){
		temp = temp * cooling;
	}


	int s = -1;
	float v = -1;
	vector<float> vc;


	for(size_t i=0; i<this->children.size(); i++){
		vc.push_back(children[i].value);
		if(vc[i] > v){
			v = vc[i];
			s = i;
		}
	}

	vector<float> p;
	for(size_t i=0; i<vc.size(); i++){
		p.push_back(exp(-(v-vc[i])/temp)); // do sim annealing probability
	}



	return s;
}


treeNode::~treeNode() {
	// TODO Auto-generated destructor stub
}

