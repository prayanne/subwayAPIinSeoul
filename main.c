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

char sample[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/�̼�";



//�Լ� ���� ��Ʈ
int praseAPI(FILE* stream);
char processingAPI(FILE* stream);



void main()
{
	SetConsoleOutputCP(CP_UTF8);

	FILE* stream;
	stream = fopen("ex.txt", "w+");

	praseAPI(stream);
	processingAPI(stream);

	fclose(stream);
	stream = NULL;

	system("notepad.exe ex.txt");
}

int praseAPI(FILE* stream) {
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
		
		fscanf(stream, "%s", curl);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return 0;
	}
	
}

char processingAPI(FILE* stream)
{
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
	return 0;
}
