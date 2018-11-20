#include<stdio.h>
#include<queue>
#include"cJSON.h"

typedef struct CSNode {
	int data;
	struct CSNode *lchild, *rbro;
}CSNode;

void PrintTree(CSNode* root);
char * makeJson(CSNode *root);
cJSON* CreateChildJson(CSNode *p);
CSNode* parseTree(char * Json);
CSNode* parseArray(cJSON *pArrayJson);

int main() {
	CSNode*root = (CSNode *)malloc(sizeof(CSNode));
	CSNode*a1 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*a2 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*a3 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*b1 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*b2 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*b3 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*c1 = (CSNode *)malloc(sizeof(CSNode));
	CSNode*c2 = (CSNode *)malloc(sizeof(CSNode));
	root->data = 2;
	a1->data = 1, a2->data = 5, a3->data = 4;
	b1->data = 0, b2->data = 6, b3->data = 7;
	c1->data = 8, c2->data = 9;
	root->lchild = a1, root->rbro = NULL;
	a1->lchild = b1, a1->rbro = a2;
	a2->lchild = NULL, a2->rbro = a3;
	a3->lchild = c1, a3->rbro = NULL;
	b1->lchild = NULL, b1->rbro = b2;
	b2->lchild = NULL, b2->rbro = b3;
	b3->lchild = NULL, b3->rbro = NULL;
	c1->lchild = NULL, c1->rbro = c2;
	c2->lchild = NULL, c2->rbro = NULL;
	char*p = makeJson(root);
	CSNode * newroot = parseTree(p);
	PrintTree(newroot);
	delete(p);
	free(root), free(a1), free(a2), free(a3), free(b1), free(b2), free(b3), free(c1), free(c2);
	getchar();
	return 0;
}

void PrintTree(CSNode * root)
{
	std::queue<CSNode> q;
	q.push(*root);
	while (!q.empty()) {
		CSNode *p = &(q.front());
		printf("%d\n", p->data);
		q.pop();
		CSNode* child = p->lchild;
		while (child != NULL) {
			q.push(*child);
			child = child->rbro;
		}
	}
}

char * makeJson(CSNode * root)
{
	cJSON *pJson = NULL;
	pJson = cJSON_CreateArray();
	if (NULL == pJson) {
		return NULL;
	}
	cJSON_AddItemToArray(pJson,  CreateChildJson(root));
	char *p = cJSON_Print(pJson);
	if (NULL == p) {
		cJSON_Delete(pJson);
		return NULL;
	}
	cJSON_Delete(pJson);
	return p;
}

cJSON * CreateChildJson(CSNode * p)
{
	cJSON *pJson = NULL;
	pJson = cJSON_CreateObject();
	if (NULL == pJson) {
		return NULL;
	}
	cJSON_AddNumberToObject(pJson, "data", p->data);
	cJSON *pChildJson = NULL;
	pChildJson = cJSON_CreateArray();
	if (NULL == pChildJson) {
		cJSON_Delete(pJson);
		return NULL;
	}
	CSNode *child = p->lchild;
	while (child != NULL) {
		cJSON_AddItemToArray(pChildJson, CreateChildJson(child));
		child = child->rbro;
	}
	cJSON_AddItemToObject(pJson, "children", pChildJson);
	return pJson;
}

CSNode * parseTree(char * Json)
{
	if (NULL == Json) {
		return NULL;
	}
	cJSON *pArrayJson = cJSON_Parse(Json);
	CSNode* root = parseArray(pArrayJson);
	return root;
}

CSNode * parseArray(cJSON * pArrayJson)
{
	int size = cJSON_GetArraySize(pArrayJson);
	if (size == 0) {
		return NULL;
	}
	CSNode *p = (CSNode *)malloc(sizeof(CSNode));
	CSNode *q = p;
	for (int i = 0; i < size; i++) {
		cJSON *child = cJSON_GetArrayItem(pArrayJson, i);
		cJSON *obj = cJSON_GetObjectItem(child, "data");
		if (obj == NULL) {
			return NULL;
		}
		q->data = obj->valueint;
		q->lchild = parseArray(cJSON_GetObjectItem(child, "children"));
		if (i != size - 1) {
			q->rbro = (CSNode *)malloc(sizeof(CSNode));
			q = q->rbro;
		}
		else {
			q->rbro = NULL;
		}
	}
	return p;
}
