#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define BUFSIZE 512
#define TRUE 1
#define FALSE 0
typedef struct WordData {
	char* m_word;					// �ܾ�(���ڿ�)
	int m_freq;						// �󵵼�

	struct WordData* m_lf;			// ���� ��带 ����Ű�� ������
	struct WordData* m_rf;			// ������ ��带 ����Ű�� ������
} WordData;

void printResult(WordData* pHead);					// ����� ���
void SearchNInsert(WordData* pHead, char* word);	// Ʈ���� Ž���ϸ鼭 ����, Ȥ�� �󵵼� ����
char* readWord(FILE* fp, int* eofFlag);				// �ܾ� �Ľ�
int checkChar(char letter);							// ���� Ȯ��

void main() {
	// ============== �ʱ� �۾� ============================
	WordData* Head = NULL;
	char* word;
	int eofFlag = TRUE;
	FILE* fp = fopen("test.xml", "rt");
	// ================================================
	while(eofFlag == TRUE)
	{
		word = readWord(fp,&eofFlag);	// �ܾ� �Ľ�
		SearchNInsert(Head, word);		// ���� Ʈ���� ���� Ȥ�� Ž��
	}

	printResult(Head);					// ��� ���
	fclose(fp);
	return ;
}

void printResult(WordData* pHead) 		// ������ ���� ���̹Ƿ� ���� ��ȸ
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
void SearchNInsert(WordData* pHead, char* word)		// ���� Ʈ���� Ž���ϸ鼭 ����.
{
	WordData* tmpNode;
	WordData* p;
	if(pHead == NULL)								// ù ��� ��忡 ����
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
	else												// Ʈ�� ����
	{
		p = pHead;
		while (p->m_lf != NULL || p->m_rf != NULL) {	// ���� ����, �Ǵ� ���������� Ʈ�� Ž��
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
		if (strcmp(p->m_word, word) == 0) {				// Ž���� �Ϸ�ƴµ��� ��尡 ���� ��� Ʈ�� ��� ����
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
