#define _CRT_SECURE_NO_WARRINGS
#pragma warning(disable:4996)

//#pragma once
//#define _CURL_DEFINE_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>	

#include <curl.h>

// curl ���� ����
CURL* curl;
CURLcode res;

// API ���� ���� ����, ���� �Լ� ���� �̵� ��
char key[50] = "465a73644d6b7968313032624d434c41";
char url[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/";
//http://swopenAPI.seoul.go.kr/api/subway/sample/xml/realtimeStationArrival/0/5/����
//http://swopenapi.seoul.go.kr/api/subway/sample/xml/realtimeStationArrival/1/5/%EC%A0%95%EC%99%95
char sample[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/%EC%A0%95%EC%99%95";


// ������ ���� �� ����� ���� ����ü
struct MemoryStruct {
	char* memory;
	size_t size;
};


//�Լ� ���� ��Ʈ
int praseAPI();
char processingAPI();
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);


void main()
{
	SetConsoleOutputCP(CP_UTF8);

	

	//processingAPI();

	praseAPI();
	system("start output.xml");
}

int praseAPI() {
	
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  // �޸� �ʱ�ȭ
	chunk.size = 0;            // ������ ũ�� �ʱ�ȭ
	
	curl = curl_easy_init();
	if (curl) {
		// curl�� ����� url ����
		curl_easy_setopt(curl, CURLOPT_URL, sample);
		//curl_easy_setopt(curl, CURLOPT_URL, "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/json/realtimeStationArrival/0/5/�̼�");
		
		// FOLLOW LOCATION����, �����̷�Ʈ ���ü���
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 
		
		// �����͸� ���� �ݹ� �Լ� ����
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		
		// �ݹ� �Լ��� ������ ������ ����ü ����
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
		
		// http ��û ����
		res = curl_easy_perform(curl);


		// ��û�� �����ߴ��� Ȯ��
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			// �޾ƿ� �����͸� ���Ϸ� ����
			FILE* fp = fopen("output.xml", "w");
			if (fp) {
				fwrite(chunk.memory, sizeof(char), chunk.size, fp);
				fclose(fp);
				printf("XML data saved to output.xml\n");
			}
			else {
				printf("Failed to open file for writing.\n");
			}
		}

		curl_easy_cleanup(curl);

		// �޸� ����
		free(chunk.memory);  

		return 0;
	}
	
}

// �ݹ� �Լ�: curl�� �����͸� ���� �� ȣ��Ǵ� �Լ�
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = realloc(mem->memory, mem->size + realsize + 1); // �޸� ���Ҵ�
	if (ptr == NULL) {
		// �޸� �Ҵ� ����
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize); // ������ ����
	mem->size += realsize;
	mem->memory[mem->size] = 0; // null-terminate

	return realsize;
}

char processingAPI()
{
	FILE* stream;

	stream = fopen("ex.txt", "r");

	char string[700];
	char onePart = 0;



	memset(string, 0, sizeof(string));

	fscanf(stream, "%s", string); // EOF�� Ȯ���ϱ� ����, ���� �ѹ� �д´�. �� ������ �����ϸ�, ^z�� ���Ե� ���ڿ��� NULL���� ������, �Ľ̿� ū ������� �ִ�.
	//fprintf(stdout, "%s", string);

	while (!feof(stream))
	{

		for (int i = 0; i <= strlen(string); i++) {
			if (string[i] == "<" || string[i + 1] == '/') {
				printf("\n");
				// key �Է� ������ ����
				// '/'�� ���� ���� �� �������� Ȯ���� ��
			}
			printf("%c", string[i]);
			//printf("%s", string);


			if (string[i] == '>')
			{
				printf("\n");
				// key �Է� ������ �߰� ���� ��, ���� �� üũ
			}

		}

		memset(string, 0, sizeof(string));
		fscanf(stream, "%s", string);

	}
	fclose(stream);
	stream = NULL;

	return 0;
}
