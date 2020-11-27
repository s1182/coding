// ax2.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map> 
using namespace std;

struct treeNode
{
	string name;
	int value;
	treeNode* left;
	treeNode* right;
	treeNode(string nam): name(nam) , value(1), left(nullptr), right(nullptr)
	{}
	treeNode(int val) : name( to_string(val) ), value(val), left(nullptr), right(nullptr)
	{}
};

treeNode* search_tree(treeNode* node, const string& val)
{
	if (node->name == val)
	{
		return node;
	}
	else
	{
		if (node->left != nullptr)
		{
			treeNode* tempNode = search_tree(node->left, val);
			if (tempNode != nullptr)
			{
				return tempNode;
			}
		}
		if (node->right != nullptr)
		{
			treeNode* tempNode = search_tree(node->right, val);
			if (tempNode != nullptr)
			{
				return tempNode;
			}
		}
	}
	return nullptr;
}

treeNode* createNode(const string& item)
{
	treeNode* tempNode(nullptr);
	try
	{
		int val = std::stoi(item);
		tempNode = new treeNode(val);
	}
	catch (std::invalid_argument e)
	{
		tempNode = new treeNode(item);
	}
	return tempNode;
}

void printTree(const treeNode* const node)
{
	if (node)
	{
		cout << node->name << "__" << node->value << endl;

		printTree(node->left);
		printTree(node->right);
	}
}

void buildTree(treeNode** root, const vector<string>& items)
{
	if (*root == nullptr)
	{
		treeNode* tempRoot = createNode(items[0]);
		tempRoot->left = createNode(items[1]);
		tempRoot->right = createNode(items[2]);		
		*root = tempRoot;
	}
	else
	{		
		treeNode* s = search_tree(*root, items[0]);
		if (s != nullptr)
		{
			s->left = createNode(items[1]);
			s->right = createNode(items[2]);			
		}
		else
		{
			cout << "Invalid local root passed." << endl;
		}
	}	
}

void buildCompleteTree(treeNode** root, const vector<string>& inputRows)
{
	for (auto row : inputRows)
	{
		vector<string> rowItems;
		istringstream rowStream(row);
		string nodeItem;
		
		while (getline(rowStream, nodeItem, ','))
		{
			rowItems.push_back(nodeItem);	
		}

		buildTree(root, rowItems);
	}
}

int balanceTheTree(treeNode* node, unordered_map<string,int>& resMap)
{
	int lSum = 0;
	int rSum = 0;

	if (node == NULL)
		return 0;

	if (node->left == NULL && node->right == NULL)
		return node->value;

	lSum = balanceTheTree(node->left,resMap);
	rSum = balanceTheTree(node->right,resMap);
	if (lSum > rSum)
	{
		node->value = node->value + 2 * lSum;		
		resMap[node->name] = (lSum - rSum) * -1 ;
	}
	else if (rSum > lSum)
	{
		node->value = node->value + 2 * rSum;		
		resMap[node->name] = (rSum - lSum) * 1;
	}
	else
	{
		node->value = node->value  + 2 * rSum;

	}
	return node->value;
}

void readInputFile(vector<string>& vecInputLines)
{
	ifstream inputFile("System.in");
	if (inputFile.is_open())
	{
		string inputLine; 
		for (std::string line; getline(inputFile, inputLine); )
		{			
			if (inputLine.find_first_of("#", 0) != 0)
			{
				if (count(inputLine.begin(), inputLine.end(), ',') != 2)
				{
					std::cout << "Incorrect input line...skipping this line." << endl;
				}
				else
				{
					vecInputLines.push_back(inputLine);					
				}
			}
		}
		inputFile.close();
	}
	else
	{
		cout << "Unable to open input file - System.in" << endl;
	}	
}

void writeOutputFile(const vector<string>& uniqueNodes, unordered_map<string,int>& resultMap)
{
	ofstream outputFile("System.out");
	
	if (outputFile.is_open())
	{
		for (auto itr = uniqueNodes.begin(); itr < uniqueNodes.end(); ++itr)
		{				
			string str = itr->substr(0, itr->find_first_of(','));			
			if (resultMap[str] < 0)
			{
				outputFile << str << " , 0 , " << resultMap[str] * -1 << endl;
			}
			else
			{
				outputFile << str << " , "<< resultMap[str] << " , 0" << endl;
			}
		}		
		outputFile.close();
	}
	else
	{
		cout << "Unable to open output file - System.out" << endl;
	}

}

int main()
{	
	vector<string> inputRows;	
	readInputFile(inputRows);
	 
	treeNode* tempNode = nullptr;
	treeNode** treeRoot = &tempNode;	
	buildCompleteTree(treeRoot, inputRows);	
	printTree(*treeRoot);	

	unordered_map<string,int> resultMap;
	balanceTheTree(*treeRoot, resultMap);
		
	writeOutputFile(inputRows, resultMap);
}
