#include <iostream>
#include <string>
#include <vector>

#include "mynode.h"
#include "suffixTree.h"

using namespace std;


int testInsertHelper(node* u) 
{
	int i = 0;
	for (i = 0; i < u->children.size() - 1; i++)
	{
		testInsertHelper(u->children.at(i));
		if (u->children.at(i) >= u->children.at(i + 1))
			return -1;
	}
	return 1;
}

int testInsert() 
{
	string alphabet = "acgt$", input = "acgtgtaccttaggccaaagggtt$";//"actg$", input = "actgagcactgag$";
	suffixTree myTree(alphabet, input);
	myTree.buildWithLinks();

	node* u = myTree.root;
	testInsertHelper(u);
}


int main()
{
	testInsert();

	string alphabet = "misp$", input = "mississippi$";//"actg$", input = "actgagcactgag$";
	suffixTree myTree(alphabet, input);

	myTree.buildWithLinks();

	string g = "hi";

	return 0;
}