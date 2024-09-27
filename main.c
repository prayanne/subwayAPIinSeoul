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

// curl 관련 설정
CURL* curl;
CURLcode res;

// API 관련 변수 선언, 이후 함수 내로 이동 필
char key[50] = "465a73644d6b7968313032624d434c41";
char url[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/";
//http://swopenAPI.seoul.go.kr/api/subway/sample/xml/realtimeStationArrival/0/5/서울

char sample[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/이수";



//함수 선언 파트
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

	fscanf(stream, "%s", string); // EOF를 확인하기 위해, 먼저 한번 읽는다. 이 과정을 생략하면, ^z가 포함된 문자열이 NULL까지 읽혀져, 파싱에 큰 어려움이 있다.
	//fprintf(stdout, "%s", string);

	while (!feof(stream))
	{

		for (int i = 0; i <= strlen(string); i++) {
			if (string[i] == "<" || string[i + 1] == '/') {
				printf("\n");
				// key 입력 시퀀스 시작
				// '/'가 들어가는 곳은 끝 지점으로 확인할 것
			}
			printf("%c", string[i]);
			//printf("%s", string);


			if (string[i] == '>')
			{
				printf("\n");
				// key 입력 시퀀스 중간 점검 및, 다음 값 체크
			}

		}

		memset(string, 0, sizeof(string));
		fscanf(stream, "%s", string);

	}
	return 0;
}
