#include"cJSON.h"
#include<stdio.h>

char * makeJson() {
	cJSON *pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot) {
		return NULL;
	}
	cJSON_AddStringToObject(pJsonRoot, "hello", "hello world");
	cJSON_AddNumberToObject(pJsonRoot, "number", 10010);
	cJSON_AddBoolToObject(pJsonRoot, "bool", 1);
	cJSON *pSubJson = NULL;
	pSubJson = cJSON_CreateObject();
	if (NULL == pSubJson) {
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_AddStringToObject(pSubJson, "subJsonObj", "a sub json string");
	cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);
	char *p = cJSON_Print(pJsonRoot);
	if (NULL == p) {
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
}

void parseJson(char * pMsg) {
	if (NULL == pMsg) {
		return;
	}
	cJSON * pJson = cJSON_Parse(pMsg);
	if (NULL == pJson) {
		return;
	}
	cJSON *pSub = cJSON_GetObjectItem(pJson, "hello");
	if (NULL != pSub) {
		printf("obj_1£º%s\n", pSub->valuestring);
	}
	pSub = cJSON_GetObjectItem(pJson, "number");
	if (NULL != pSub) {
		printf("obj_2£º%d\n", pSub->valueint);
	}
	pSub = cJSON_GetObjectItem(pJson, "bool");
	if (NULL != pSub) {
		printf("obj_3£º%d\n", pSub->valueint);
	}
	pSub = cJSON_GetObjectItem(pJson, "subobj");
	cJSON * pSubSub = cJSON_GetObjectItem(pSub, "subJsonObj");
	if (NULL != pSubSub) {
		printf("obj_4£º%s\n", pSubSub->valuestring);
	}
	cJSON_Delete(pJson);
}

int main() {
	char *p = makeJson();
	if (NULL != p) {
		printf("%s\n", p);
	}
	parseJson(p);
	delete(p);
	getchar();
	return 0;
}