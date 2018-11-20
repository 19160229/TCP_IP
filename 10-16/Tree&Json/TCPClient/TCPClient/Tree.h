#pragma once
#include"cJSON.h"

typedef struct CSNode {
	int data;
	struct CSNode *lchild, *rbro;
}CSNode;

void PrintTree(CSNode* root);
char * makeJson(CSNode *root);
cJSON* CreateChildJson(CSNode *p);
void InitTree(CSNode* root);
CSNode* parseTree(char * Json);
CSNode* parseArray(cJSON *pArrayJson);
void freeTree(CSNode* p);