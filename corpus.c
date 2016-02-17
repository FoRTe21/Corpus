#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define BUFSIZE 512
#define TRUE 1
#define FALSE 0
typedef struct WordData {
	char* m_word;					// 단어(문자열)
	int m_freq;						// 빈도수

	struct WordData* m_lf;			// 왼쪽 노드를 가리키는 포인터
	struct WordData* m_rf;			// 오른쪽 노드를 가리키는 포인터
} WordData;

void printResult(WordData* pHead);					// 결과값 출력
void SearchNInsert(WordData* pHead, char* word);	// 트리를 탐색하면서 저장, 혹은 빈도수 증가
char* readWord(FILE* fp, int* eofFlag);				// 단어 파싱
int checkChar(char letter);							// 문자 확인

void main() {
	// ============== 초기 작업 ============================
	WordData* Head = NULL;
	char* word;
	int eofFlag = TRUE;
	FILE* fp = fopen("test.xml", "rt");
	// ================================================
	while(eofFlag == TRUE)
	{
		word = readWord(fp,&eofFlag);	// 단어 파싱
		SearchNInsert(Head, word);		// 이진 트리의 삽입 혹은 탐색
	}

	printResult(Head);					// 결과 출력
	fclose(fp);
	return ;
}

void printResult(WordData* pHead) 		// 왼쪽이 작은 수이므로 중위 순회
{
	if(pHead->m_lf != NULL)
	{
		printResult(pHead->m_lf);
	}
	printf("%s : %d\n", pHead->m_word, pHead->m_freq);
	if(pHead->m_rf != NULL)
	{
		printResult(pHead->m_rf);
	}
}
void SearchNInsert(WordData* pHead, char* word)		// 이진 트리를 탐색하면서 삽입.
{
	WordData* tmpNode;
	WordData* p;
	if(pHead == NULL)								// 첫 노드 헤드에 삽입
	{
		printf("first Node");
		p = (WordData*)malloc(sizeof(WordData));
		strcpy(p->m_word, word);
		//p->m_word = word;
		p->m_freq = 1;
		p->m_lf = NULL;
		p->m_rf = NULL;
		pHead = p;
		return ;
	}
	else												// 트리 구성
	{
		p = pHead;
		while (p->m_lf != NULL || p->m_rf != NULL) {	// 먼저 왼쪽, 또는 오른쪽으로 트리 탐색
			if (strcmp(p->m_word, word) == 0) {
				p->m_freq++;
				free(word);
				return ;
			}
			else if(strcmp(p->m_word, word) < 0)
			{
				p = p->m_lf;
			}
			else
			{
				p = p->m_rf;
			}
		}
		if (strcmp(p->m_word, word) == 0) {				// 탐색이 완료됐는데도 노드가 없는 경우 트리 노드 삽입
			p->m_freq++;
			free(word);
			return;
		} else if (strcmp(p->m_word, word) < 0) {
			tmpNode = (WordData*)malloc(sizeof(WordData*));
			tmpNode->m_word = word;
			tmpNode->m_freq = 1;
			tmpNode->m_lf = NULL;
			tmpNode->m_rf = NULL;
			p->m_lf = tmpNode;
			return;
		} else {
			tmpNode = (WordData*) malloc(sizeof(WordData*));
			tmpNode->m_word = word;
			tmpNode->m_freq = 1;
			tmpNode->m_lf = NULL;
			tmpNode->m_rf = NULL;
			p->m_rf = tmpNode;
			return;
		}
	}
}


char* readWord(FILE* fp, int* eofFlag)
{
	int tagFlag = FALSE;// true : tag
							// false : not tag
	char* tmpBuf = NULL;
	char buffer[BUFSIZE] = {0,};
	char letter;
	int flag = TRUE;
	int what = 0;
	int index = 0;
	while (flag) {
		letter = fgetc(fp);
		if(letter == EOF)
		{
			*eofFlag = FALSE;
			break;
		}
		what = checkChar(letter);
		switch (what) {
			case -1:
			{
				tagFlag = !tagFlag;
				continue;
			}
			case 1:
			{
				continue;
			}
			case 2:
			{
					continue;
			}
			case 0:
			{
				if (tagFlag == TRUE) {
					continue;
				} else {
					if(letter >= 'A' && letter <= 'Z')
					{
						letter = letter - 'A' + 'a';
					}
					buffer[index++] = letter;
				}
			}
		}
	}

	tmpBuf = (char*)malloc(sizeof(char) * strlen(buffer));
	memcpy(tmpBuf,buffer, sizeof(char)* strlen(buffer));
	return tmpBuf;
}

int checkChar(char letter) {
	char tmpLetter = letter;
	if ((tmpLetter >= 'a' && tmpLetter <= 'z') || (tmpLetter >= 'A' && tmpLetter <= 'Z'))
	{
		return 0;
	}
	switch (tmpLetter) {
	case '<':
	case '>': {
		return -1;
	}
	case ' ':
	case ':':
	case '.':
	case '\n': {
		return 2;
	}
	default:
	{
		return 1;
	}
	}
}
