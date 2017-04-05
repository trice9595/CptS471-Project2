#pragma once
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>

//#include "suffixTree.h"

using namespace std;

class node
{
public:
	int label[2];
	vector<node*> children;
	char value;
	int id;
	int strDepth;
	node *parent;
	node *suffixLink;

	node();

	node(int labelinfo[2], char newValue, int depth, node *newParent)
	{
		label[0] = labelinfo[0];
		label[1] = labelinfo[1];

		strDepth = depth + 1;

		value = newValue;

		parent = newParent;

		suffixLink = nullptr;
	}

	//returns node created, or nullptr if requested node already exists
	node* addChild(int labelinfo[2], char newValue)
	{
		int newDepth = this->strDepth + (labelinfo[1] - labelinfo[0]);
		node *newNode = new node(labelinfo, newValue, newDepth, this);

		addChild(newNode);
		return newNode;
	}

	bool addChild(node *nodeToAdd)
	{
		int i = 0;

		while (nodeToAdd->value >= children.at(i)->value) { i++; }

		vector<node*>::iterator it = children.begin() + i;

		if (it == children.end)
			cout << "Error: Child already exists\n";

		nodeToAdd->parent = this;
		children.insert(it, nodeToAdd);

		return true;
	}

	node* getChild(char val)
	{
		for (auto child : children)
		{
			if (val == child->value)
			{
				return child;
			}
		}
		return nullptr;
	}

	bool hasEdge(char val)
	{
		for (auto child : children)
		{
			if (child->value == val)
			{
				return true;
			}
		}
		return false;
	}

	bool removeChild(node *toRemove)
	{
		vector<node*>::iterator it;
		
		for (it = this->children.begin(); it != this->children.end();)
		{
			if ((*it) == toRemove)
			{
				it = this->children.erase(it);
			}
			else
			{
				it++;
			}
			
		}
		return true;
	}


};

#endif // !NODE_H
