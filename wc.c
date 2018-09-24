#include <stdio.h>

//ͳ���ַ���
void CharCount()
{
    FILE *fp;
    int charCount = 0;
    char ch;//��ȡ�ļ����ص��ֽ�
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("�ļ���ʧ��.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
            charCount++;
            ch = fgetc(fp);
    }
    printf("�ַ���Ϊ��%d��.\n",charCount);
    fclose(fp);
}


//ͳ�Ƶ�����
void WordCount() 
{
    FILE *fp;
    int wordCount = 0,flag=0;
    char ch;//��ȡ�ļ����ص��ֽ�
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("�ļ���ʧ��.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch==' ')
        {
			flag=0;
            ch = fgetc(fp);  
        }
        else 
        {   
			if(flag==0)
			{
				wordCount ++;	
				flag=1;
			}
            ch = fgetc(fp);
        }
    }
    printf("������Ϊ��%d��.\n",wordCount);
    fclose(fp);
}

//ͳ������
void LineCount()
{
    FILE *fp;
    int lineCount = 0;
    char ch;//��ȡ�ļ����ص��ֽ�
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("�ļ���ʧ��.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch == '\n')
        {
            lineCount++;
            ch = fgetc(fp);
        }	
		else 
		{   
			ch = fgetc(fp);
		}
    }
    printf("����Ϊ��%d��.\n",lineCount);
    fclose(fp);
}

//ͳ���ַ�����������������
void Passage()
{
    FILE *fp1;
	FILE *fp2;
	int lineCount=0,wordCount=0,charCount=0;
	int flag=0;
    char ch;//��ȡ�ļ����ص��ֽ�
    if((fp1 = fopen("test.txt","r")) == NULL)
    {
        printf("�ļ���ʧ��.");
    }
    ch = fgetc(fp1);
    while(ch != EOF)
    {
		charCount++;
        if (ch == '\n')
        {
            lineCount++;
            ch = fgetc(fp1);
        }
        else if(ch==' ')
        {
			flag=0;
            ch = fgetc(fp1);
        }		
		else 
		{   
			if(flag==0)
			{
				wordCount ++;	
				flag=1;
			}
			ch = fgetc(fp1);
		}
    }
    printf("�ַ���Ϊ��%d��.\n",charCount);
    printf("������Ϊ��%d��.\n",wordCount);
    printf("����Ϊ��%d��.\n",lineCount);
    fclose(fp1);
	fp2=fopen("result.txt","w");
    fprintf(fp2,"�ַ���Ϊ��%d��.\n",charCount);
    fprintf(fp2,"������Ϊ��%d��.\n",wordCount);
    fprintf(fp2,"����Ϊ��%d��.\n",lineCount);
    fclose(fp2);  
}


int main(int argc,char *argv[])
{
	//ͳ�Ƶ�����
    if ((strcmp(argv[1], "-w") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        WordCount();
    }
	//ͳ���ַ���
    if ((strcmp(argv[1], "-c") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        CharCount();
    }
    //ͳ������
    if ((strcmp(argv[1], "-l") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        LineCount();
    }
	//ͳ���ַ�����������������
	if ((strcmp(argv[1], "-p") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        Passage();
    }
    return 0;
}



