#pragma once
#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include "mynode.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//class node;

class suffixTree
{
public:
	node *root;
	std::vector<char> alphabet;
	std::string input;
	int inputSize;
	vector<node*> nodeList;
	vector<node*> leafList;
	vector<node*> internalList;


	suffixTree(std::string alpha, std::string str)
	{
		this->root = nullptr;
		for (int i = 0; i < alpha.length(); i++)
		{
			this->alphabet.push_back(alpha[i]);
		}

		this->input = str;
		inputSize = input.size() - 1;
	}

	//this returns the id
	vector<int> addEdge(std::string str, int sufNum, int len, int lastIndex, vector<int> slHelper)
	{
		int edgelabel[2];
		node *curNode = this->root;
		node *parent = this->root;
		node *temp;
		vector<int> toRet;
		int j = 0;
		int SL = 0;

		if (root == nullptr)
		{
			edgelabel[0] = 0;
			edgelabel[1] = 0;
			root = new node(edgelabel, 'R', -1, nullptr);
			//add root to list of nodes to make accessable for later if necessary as well as set id number
			nodeList.push_back(root);
			root->id = nodeList.size() - 1;
			root->suffixLink = root;

			edgelabel[0] = sufNum;
			edgelabel[1] = lastIndex;
			temp = root->addChild(edgelabel, str[0]);

			//add new node to list of nodes to make accessable for later if necessary as well as set id number
			nodeList.push_back(temp);
			leafList.push_back(temp);
			temp->id = nodeList.size() - 1;
		}
		else
		{
			for (int i = 0; i < len; i++)
			{
				//if (slHelper[1] > 0)//if the last iteration told us that a suffix link exists jump straight to it
				//{
				//	curNode = nodeList[slHelper[1]];
				//	i = curNode->strDepth;
				//}
				//means we dont need to make a new path yet
				if (curNode->hasEdge(str[i]))
				{
					parent = curNode;
					curNode = curNode->getChild(str[i]);
					j = curNode->label[0];

					//here we find where the two strings differ between the new edge and the node being traveled to
					while (input[j] == str[i] && i < len && j < curNode->label[1])
					{
						j++;
						i++;
					}

					//this means we hit an internal node
					if (j == curNode->label[1])
					{
						//we do nothing here because we want to re-start the loop
						//if (curNode->suffixLink != nullptr)
						//{
						//	SL = curNode->suffixLink->id;
						//}
					}
					else if (input[j] != str[i])
					{
						//this means that the two paths differ here, so we need to make a new internal node
						edgelabel[0] = curNode->label[0];
						edgelabel[1] = j - 1;
						int newDepth = parent->strDepth + (edgelabel[1] - edgelabel[0]);
						node *newinternal = new node(edgelabel, curNode->value, newDepth, parent);

						//add the new internal to the appropriate lists as well as set node's id number
						internalList.push_back(newinternal);
						nodeList.push_back(newinternal);
						newinternal->id = nodeList.size() - 1;

						//toRet.push_back(newinternal->id);

						//this means that the previous iteration is telling us to make this internal node the destination
						//of a suffix link
						//if (slHelper[0])
						//{
						//	temp = nodeList[slHelper[0]];
						//	temp->suffixLink = newinternal;
						//}

						//add the original node to the child node of the internal node
						curNode->label[0] = j;
						curNode->value = input[j];
						newinternal->addChild(curNode);

						//add the node of the new edge created
						edgelabel[0] = sufNum + i;
						edgelabel[1] = lastIndex;
						temp = newinternal->addChild(edgelabel, str[i]);

						//add the new node created to appropriate lists as well as set new leaf node's id
						leafList.push_back(temp);
						nodeList.push_back(temp);
						temp->id = nodeList.size() - 1;


						parent->removeChild(curNode);
						parent->addChild(newinternal);
						break;
					}
					else
					{
						//this means that the new edge to be added hit the end before reaching the end of a label aka i == len
						edgelabel[0] = curNode->label[0];
						edgelabel[1] = j - 1;

						int newDepth = parent->strDepth + (edgelabel[1] - edgelabel[0]);
						node *newinternal = new node(edgelabel, curNode->value, newDepth, parent);

						//add the new internal to the appropriate lists as well as set node's id number
						internalList.push_back(newinternal);
						nodeList.push_back(newinternal);
						newinternal->id = nodeList.size() - 1;

						//toRet.push_back(newinternal->id);

						//this means that the previous iteration is telling us to make this internal node the destination
						//of a suffix link
						//if (slHelper[0])
						//{
						//	temp = nodeList[slHelper[0]];
						//	temp->suffixLink = newinternal;
						//}

						//add the original node to the child node of the internal node
						curNode->label[0] = j;
						curNode->value = input[j];
						newinternal->addChild(curNode);

						//add the node of the new edge created
						edgelabel[0] = i;
						edgelabel[1] = len - 1;
						temp = newinternal->addChild(edgelabel, str[i]);

						//add the new node created to appropriate lists as well as set new leaf node's id
						leafList.push_back(temp);
						nodeList.push_back(temp);
						temp->id = nodeList.size() - 1;

						parent->removeChild(curNode);
						parent->addChild(newinternal);
						break;
					}
					

				}
				else
				{
					edgelabel[0] = sufNum + i;
					edgelabel[1] = lastIndex;
					temp = curNode->addChild(edgelabel, str[i]);

					//add the new node created to appropriate lists as well as set new leaf node's id
					nodeList.push_back(temp);
					leafList.push_back(temp);
					temp->id = nodeList.size() - 1;
					break;
				}
			}
		}

		if (toRet.size() == 0)
		{
			toRet.push_back(0);
		}
		toRet.push_back(SL);
		return toRet;
	}

	node* findPath(node *startNode, int *label)
	{
		node *curNode = startNode;
		node *parent, *temp;
		string str = input;
		int i = label[0];
		int j, len = label[1];
		int edgelabel[2];

		if (curNode->hasEdge(str[i]))
		{
			parent = curNode;
			curNode = curNode->getChild(str[i]);
			j = curNode->label[0];

			//here we find where the two strings differ between the new edge and the node being traveled to
			while (input[j] == str[i] && i < len && j < curNode->label[1])
			{
				j++;
				i++;
			}

			//this means we hit an internal node
			if (j == curNode->label[1])
			{
				temp = nullptr;
				//we do nothing here because we want to re-start the loop
			}
			else if (input[j] != str[i])
			{
				//this means that the two paths differ here, so we need to make a new internal node
				edgelabel[0] = curNode->label[0];
				edgelabel[1] = j - 1;
				int newDepth = parent->strDepth + (edgelabel[1] - edgelabel[0]);
				node *newinternal = new node(edgelabel, curNode->value, newDepth, parent);

				//add the new internal to the appropriate lists as well as set node's id number
				internalList.push_back(newinternal);
				nodeList.push_back(newinternal);
				newinternal->id = nodeList.size() - 1;

				//add the original node to the child node of the internal node
				curNode->label[0] = j;
				curNode->value = input[j];
				newinternal->addChild(curNode);

				//add the node of the new edge created
				edgelabel[0] = i;
				edgelabel[1] = this->inputSize;
				temp = newinternal->addChild(edgelabel, str[i]);

				//add the new node created to appropriate lists as well as set new leaf node's id
				leafList.push_back(temp);
				nodeList.push_back(temp);
				temp->id = nodeList.size() - 1;


				parent->removeChild(curNode);
				parent->addChild(newinternal);
			}
		}
		else
		{
			edgelabel[0] = label[0];
			edgelabel[1] = this->inputSize;
			temp = curNode->addChild(edgelabel, str[i]);

			//add the new node created to appropriate lists as well as set new leaf node's id
			nodeList.push_back(temp);
			leafList.push_back(temp);
			temp->id = nodeList.size() - 1;
		}

		//this returns the most recent leaf added
		return temp;
	}

	bool buildTree()
	{
		std::string subs = input;
		int i = 0;
		int lasti = subs.length() - 1;
		vector<int> SLHelper;

		SLHelper.push_back(0);
		SLHelper.push_back(0);

		while (subs != "")
		{
			SLHelper = addEdge(subs, i, subs.length(), lasti, SLHelper);
			subs = subs.substr(1);
			i++;
		}
		return true;
	}

	bool setUpRoot()
	{
		int edgelabel[2];

		edgelabel[0] = 0;
		edgelabel[1] = 0;

		root = new node(edgelabel, 'R', -1, nullptr);

		//add root to list of nodes to make accessable for later if necessary as well as set id number
		nodeList.push_back(root);
		root->id = nodeList.size() - 1;
		root->suffixLink = root;

		return true;
	}

	//returns the leaf made
	node* buildFirstLeaf()
	{
		int edgelabel[2];
		node *temp;

		edgelabel[0] = 0;
		edgelabel[1] = this->inputSize;
		temp = this->root->addChild(edgelabel, this->input[0]);

		//add new node to list of nodes to make accessable for later if necessary as well as set id number
		nodeList.push_back(temp);
		leafList.push_back(temp);
		temp->id = nodeList.size() - 1;

		return temp;
	}

	node* handle1B(int iteration)
	{
		int label[2];
		node* toRet;

		label[0] = iteration;
		label[1] = this->inputSize;

		toRet = this->findPath(this->root, label);
		return toRet;
	}

	node* nodeHop(node *startNode, int betaLabel, int *strLabel, node *toSetUp)
	{
		node *temp, *toRet;

		int childLen;
		int edgelabel[2];

		temp = startNode->getChild(this->input[strLabel[0]]);

		childLen = (temp->label[1] - temp->label[0]) + 1;

		if (betaLabel == 0)//this means correct node was found
		{
			toRet = this->findPath(startNode, strLabel);
			toSetUp->suffixLink = toRet->parent;
			return toRet;
		}

		if (childLen > betaLabel)
		{
			edgelabel[0] = temp->label[0];
			edgelabel[1] = temp->label[0] + (betaLabel - 1);
			int newDepth = startNode->strDepth + (edgelabel[1] - edgelabel[0]);
			node *newinternal = new node(edgelabel, temp->value, newDepth, startNode);

			//add the new internal to the appropriate lists as well as set node's id number
			internalList.push_back(newinternal);
			nodeList.push_back(newinternal);
			newinternal->id = nodeList.size() - 1;

			//add the original node to the child node of the internal node
			temp->label[0] = newinternal->label[1] + 1;
			temp->value = this->input[temp->label[0]];
			newinternal->addChild(temp);

			startNode->removeChild(temp);

			//add the node of the new edge created
			edgelabel[0] = strLabel[0] + (newinternal->label[1] - newinternal->label[0]) + 1;
			edgelabel[1] = this->inputSize;
			toRet = this->findPath(newinternal, edgelabel);
			//temp = newinternal->addChild(edgelabel, this->input[edgelabel[0]]);

			//add the new node created to appropriate lists as well as set new leaf node's id
			leafList.push_back(temp);
			nodeList.push_back(temp);
			temp->id = nodeList.size() - 1;

			startNode->addChild(newinternal);
			//link up!
			toSetUp->suffixLink = newinternal;

			return toRet;
		}
		else
		{
			if (childLen == 0)
			{
				childLen = 1;
			}
			strLabel[0] = strLabel[0] + childLen;
			betaLabel -= childLen;
			return this->nodeHop(temp, betaLabel, strLabel, toSetUp);
		}
	}

	bool buildWithLinks()
	{
		node *leaf, *u, *uPrime = nullptr;
		string subs = input;
		int i = 1, betaLen, tempLabel[2];

		//pre processing steps
		this->setUpRoot();
		leaf = this->buildFirstLeaf();

		subs = subs.substr(1);
		while (subs != "")
		{
			u = leaf->parent;

			if (u == root)//this is the 1b case
			{
				leaf = this->handle1B(i);
				//break;
			}
			else if (u->suffixLink == nullptr && uPrime == this->root)
			{
				betaLen = u->label[1] - u->label[0];
				tempLabel[0] = i;
				tempLabel[1] = this->inputSize;
				leaf = this->nodeHop(this->root, betaLen, tempLabel, u);
			}
			else if (u->suffixLink == nullptr && uPrime != this->root)
			{
				betaLen = (u->label[1] - u->label[0]) + 1;
				tempLabel[0] = i + u->strDepth;
				tempLabel[1] = this->inputSize;
				leaf = this->nodeHop(uPrime->suffixLink, betaLen, tempLabel, u);
			}
			else if (u->suffixLink != nullptr)
			{
				tempLabel[0] = i + u->strDepth;
				tempLabel[1] = this->inputSize;
				leaf = this->findPath(u->suffixLink, tempLabel);
			}



			u = leaf->parent;
			if (u != root)
			{
				uPrime = u->parent;
			}
			subs = subs.substr(1);
			i++;
		}
		return true;

	}
};

#endif // !SUFFIXTREE_H
