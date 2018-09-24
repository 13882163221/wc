#include <stdio.h>

//统计字符数
void CharCount()
{
    FILE *fp;
    int charCount = 0;
    char ch;//读取文件返回的字节
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("文件打开失败.");
    }
    ch = fgetc(fp);
    while(ch != EOF)
    {
            charCount++;
            ch = fgetc(fp);
    }
    printf("字符数为：%d个.\n",charCount);
    fclose(fp);
}


//统计单词数
void WordCount() 
{
    FILE *fp;
    int wordCount = 0,flag=0;
    char ch;//读取文件返回的字节
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("文件打开失败.");
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
    printf("单词数为：%d个.\n",wordCount);
    fclose(fp);
}

//统计行数
void LineCount()
{
    FILE *fp;
    int lineCount = 0;
    char ch;//读取文件返回的字节
    if((fp = fopen("test.txt","r")) == NULL)
    {
        printf("文件打开失败.");
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
    printf("行数为：%d行.\n",lineCount);
    fclose(fp);
}

//统计字符数，单词数，行数
void Passage()
{
    FILE *fp1;
	FILE *fp2;
	int lineCount=0,wordCount=0,charCount=0;
	int flag=0;
    char ch;//读取文件返回的字节
    if((fp1 = fopen("test.txt","r")) == NULL)
    {
        printf("文件打开失败.");
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
    printf("字符数为：%d个.\n",charCount);
    printf("单词数为：%d个.\n",wordCount);
    printf("行数为：%d行.\n",lineCount);
    fclose(fp1);
	fp2=fopen("result.txt","w");
    fprintf(fp2,"字符数为：%d个.\n",charCount);
    fprintf(fp2,"单词数为：%d个.\n",wordCount);
    fprintf(fp2,"行数为：%d行.\n",lineCount);
    fclose(fp2);  
}


int main(int argc,char *argv[])
{
	//统计单词数
    if ((strcmp(argv[1], "-w") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        WordCount();
    }
	//统计字符数
    if ((strcmp(argv[1], "-c") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        CharCount();
    }
    //统计行数
    if ((strcmp(argv[1], "-l") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        LineCount();
    }
	//统计字符数，单词数，行数
	if ((strcmp(argv[1], "-p") == 0) && (strcmp(argv[2], "test.txt") == 0))
    {
        Passage();
    }
    return 0;
}



