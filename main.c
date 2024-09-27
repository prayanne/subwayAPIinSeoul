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
#include <cJSON.h>

// curl 관련 설정
CURL* curl;
CURLcode res;

// API 관련 변수 선언, 이후 함수 내로 이동 필
char key[50] = "465a73644d6b7968313032624d434c41";
char url[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/xml/realtimeStationArrival/0/5/";
//http://swopenAPI.seoul.go.kr/api/subway/sample/xml/realtimeStationArrival/0/5/서울
//http://swopenapi.seoul.go.kr/api/subway/sample/xml/realtimeStationArrival/1/5/%EC%A0%95%EC%99%95
char sample[500] = "http://swopenapi.seoul.go.kr/api/subway/465a73644d6b7968313032624d434c41/json/realtimeStationArrival/0/5/%EC%A0%95%EC%99%95";


// 데이터 받을 때 사용할 버퍼 구조체
struct MemoryStruct {
	char* memory;
	size_t size;
};


//함수 선언 파트
int praseAPI();
void parseJSON(const char* filename);
char processingAPI();
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
void DEVTool();

void main()
{
	SetConsoleOutputCP(CP_UTF8);

	praseAPI();
	//processingAPI();
	parseJSON("output.json");
	DEVTool();
}

void DEVTool() {
	system("start output.json");
}

int praseAPI() {
	
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  // 메모리 초기화
	chunk.size = 0;            // 데이터 크기 초기화
	
	curl = curl_easy_init();
	if (curl) {
		// curl을 사용할 url 설정
		curl_easy_setopt(curl, CURLOPT_URL, sample);
		
		// FOLLOW LOCATION설정, 리다이렉트 관련설정
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 
		
		// 데이터를 받을 콜백 함수 설정
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		
		// 콜백 함수에 전달할 데이터 구조체 설정
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
		
		// http 요청 수행
		res = curl_easy_perform(curl);


		// 요청이 성공했는지 확인
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			// 받아온 데이터를 파일로 저장
			FILE* fp = fopen("output.json", "w");
			if (fp) {
				fwrite(chunk.memory, sizeof(char), chunk.size, fp);
				fclose(fp);
				printf("XML data saved to output.json\n");
			}
			else {
				printf("Failed to open file for writing.\n");
			}
		}

		curl_easy_cleanup(curl);

		// 메모리 해제
		free(chunk.memory);  

		return 0;
	}
	
}

// 콜백 함수: curl이 데이터를 받을 때 호출되는 함수
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = realloc(mem->memory, mem->size + realsize + 1); // 메모리 재할당
	if (ptr == NULL) {
		// 메모리 할당 실패
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize); // 데이터 복사
	mem->size += realsize;
	mem->memory[mem->size] = 0; // null-terminate

	return realsize;
}


void parseJSON(const char* filename) {
	// 파일을 열고 내용을 읽기
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Failed to open JSON file\n");
		return;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* jsonData = malloc(length + 1);
	fread(jsonData, 1, length, file);
	fclose(file);
	jsonData[length] = '\0'; // null-terminate the string

	// cJSON 객체로 파싱
	cJSON* json = cJSON_Parse(jsonData);
	free(jsonData); // 메모리 해제

	if (json == NULL) {
		printf("Failed to parse JSON\n");
		return;
	}

	// "stations" 배열을 가져오기
	cJSON* stations = cJSON_GetObjectItem(json, "stations");
	if (stations != NULL && cJSON_IsArray(stations)) {
		for (int i = 0; i < cJSON_GetArraySize(stations); i++) {
			cJSON* station = cJSON_GetArrayItem(stations, i);
			if (station != NULL) {
				// 각 항목의 데이터 출력
				cJSON* name = cJSON_GetObjectItem(station, "name");
				cJSON* line = cJSON_GetObjectItem(station, "line");
				cJSON* arrival = cJSON_GetObjectItem(station, "arrival");
				printf("Station Name: %s\n", name->valuestring);
				printf("Line: %s\n", line->valuestring);
				printf("Arrival: %s\n", arrival->valuestring);
				printf("\n");
			}
		}
	}

	// 메모리 해제
	cJSON_Delete(json);
}

char processingAPI()
{
	FILE* stream;

	stream = fopen("ouput.xml", "r");

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
	fclose(stream);
	stream = NULL;

	return 0;
}
