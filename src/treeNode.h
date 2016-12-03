/*
 * treeNode.h
 *
 *  Created on: May 18, 2016
 *      Author: andy
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include<vector>
#include <math.h>
#include <cstdlib>

#include "graph.h";

using namespace std;

class treeNode {
public:
	treeNode(treeNode* parent, bool rootFlag, graph& graf, int state, Mat myVisibleMap);
	virtual ~treeNode();

	vector<treeNode> children;
	treeNode* parent;
	bool rootFlag;

	int myState; // link to physical graph representation
	float value; // my current value
	float nPulls; // number of times I've been pulled
	Mat myVisibleMap; // opencv mat of visible area



	void getChildren(graph& graf);
	// select children algorithms
	int UCBChildSelect();
	int eGreedyChildSelect(float epsilon);
	void greedyChildSelect();
	int simAnnealingChildSelect(float& temp, float cooling);
	int randChildSelect();


	void updateMyValue(float passedValue);
	void searchTree(float& passedValue);
	void exploitTree(vector<int>& myPath);

};

#endif /* TREENODE_H_ */
