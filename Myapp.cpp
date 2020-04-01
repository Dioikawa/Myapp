#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define OK     1
#define ERROR  -1

struct node1//分数栈结点
{
	int dm;//分母
	int ds;//分子
	struct node1 *next;
};
typedef struct node1 Node1;

struct node2 //运算符栈结点
{
	int data;//运算符ASCII值
	struct node2 *next;
};
typedef struct node2 Node2;

struct stack1//分数栈
{
	Node1 *top;
	int count;
};
typedef struct stack1 Stack1;

 struct stack2//运算符栈
{
	Node2 *top;
	int count;
};
typedef struct stack2 Stack2;

int InitStack1(Stack1 *l)//创建分数栈
{
	l->top = NULL;
	l->count = 0;

	return OK;
}

int InitStack2(Stack2 *l)//创建运算符栈
{
	l->top = NULL;
	l->count = 0;

	return OK;
}

int EmptyStack(Stack2 *s)//对运算符栈判空
{
	return (s->count == 0) ? OK : ERROR;
}

int Push1(Stack1 *s, int mom,int son)//分数入栈
{
	Node1 *p = (Node1 *)malloc(sizeof(Node1));
	if(p == NULL)
	{
		return ERROR;
	}
	p->dm = mom;
	p->ds = son;
	p->next = s->top;
	s->top = p;
	s->count++;

	return OK;
}

int Push2(Stack2 *s, int e)//运算符入栈
{
	Node2 *p = (Node2 *)malloc(sizeof(Node2));
	if(p == NULL)
	{
		return ERROR;
	}
	p->data = e;
	p->next = s->top;
	s->top = p;
	s->count++;

	return OK;
}

int GetTop(Stack2 *s)//取运算符栈栈顶
{
	if(NULL == s->top)
	{
		return ERROR;
	}

	return (s->top->data);
}

int Priority(char s)//检测运算符优先级
{
	switch(s)
	{
		case '(':
			return 4;
		case '/'://分数线
			return 3;
		case 'x':
		case -10://除号
			return 2;
		case '+':
		case '-':
			return 1;
		default:
			return 0;
	}
}

int Pop2(Stack1 *s)//取分子且不消栈
{

	if(NULL == s->top)
	{
		return ERROR;
	}

	return s->top->ds;
}

int Pop1(Stack1 *s)//取分母且消栈
{
	int e;

	if(NULL == s->top)
	{
		return ERROR;
	}

	Node1 *p = s->top;
	e = p->dm;
	s->top = p->next;
	free(p);
	s->count--;

	return e;
}

int Popt(Stack2 *s)//取运算符且消栈
{
	int e;

	if(NULL == s->top)
	{
		return ERROR;
	}

	Node2 *p = s->top;
	e = p->data;
	s->top = p->next;
	free(p);
	s->count--;

	return e;
}

void getfenzifenmu(int *s,int *m,char str[])  //求分数str的分子及其分母
{
	int i,t,tag;//找到'/'时，tag=1
	for(i=0,tag=0,t=0;str[i]!='\0';i++)
	{
		if(str[i]=='/')
		{
			tag=1;
			*s=t;
			t=0;
		}
		else if(str[i] >= '0' && str[i] <= '9')
		{
			t=t*10+str[i]-'0';
			if(tag==1)
				*m=t;
		}
	}
}

int gcd(int a,int b) //返回a和b的最大公约数
{
 if(b==0) return a;
 return gcd(b,a%b);
}

void Simplify(int s,int m,char str[])//分数化简
{
 int d,g,s1;   //d为带分数的整数部分或者整数,g为s和m的最大公约数
 char son[20]={'\0'};
 char mom[20]={'\0'};
 char n[20]={'\0'};
 if(s%m==0){
  d=s/m;
  itoa(d,str,10);
 }
 else if(s>m){
  d=s/m;
  s1=s-d*m;
  g=gcd(s1,m);
  itoa(d,n,10);
  itoa(s1/g,son,10);
  itoa(m/g,mom,10);
  strcat(n,"'");
  strcat(n,son);
  strcat(n,"/");
  strcat(n,mom);
  strcpy(str,n);
 }
 else{
  g=gcd(s,m);
  itoa(s/g,son,10);
  itoa(m/g,mom,10);
  strcat(son,"/");
  strcat(son,mom);
  strcpy(str,son);
 }
}

void Calculator(int son1,int mom1,int son2,int mom2,int opt,char string[])//计算中间结果
{
 char temps[20]="\0";
 char son[20]="\0";
 char mom[20]="\0";
 int s,m;
 switch(opt)
 {
  case '+':
   s=son1*mom2+son2*mom1;
   m=mom1*mom2;
   break;
  case '-':
   s=son2*mom1-son1*mom2;
   m=mom1*mom2;
   break;
  case 'x':
   s=son1*son2;
   m=mom1*mom2;
   break;
  case -10:
  case '/':
   m=son1*mom2;
   s=mom1*son2;
   break;
 }
 itoa(s,son,10);
 itoa(m,mom,10);
 strcat(son,"/");
 strcat(son,mom);
 strcpy(string,son);
}

int Answer(char str[],int y)//答案函数，str为运算式子,y为序号
{
	Stack1 num;//分数数栈
	Stack2 opt;//运算符栈
	int i = 0, tmp = 0;
	int s,m;
	int a1,a2,a3,a4,a5;
	char temps[20]="\0";//存放中间运算结果
	char fans[20]="\0";//答案字符串
	FILE *fp;
	fp=fopen("Answers.txt","a");

	if(InitStack1(&num) != OK || InitStack2(&opt) != OK)
	{
		printf("Init Failure!\n");
		exit(1);
	}

	while(str[i] != '\0' || EmptyStack(&opt) != OK)
	{
		if(str[i]==32)
		{
			i++;
			continue;
		}
		if(str[i] >= '0' && str[i] <= '9')
		{
			tmp = tmp * 10 + str[i] - '0';
			i++;
			if(str[i] < '0' || str[i] > '9')
			{
				if(Push1(&num,1,tmp)==ERROR)
			    	printf("Push1 ERROR!");//数字tmp/1入栈
				tmp = 0;
			}
		}
		else
		{
			if((EmptyStack(&opt) == OK) || (GetTop(&opt) == '(' && str[i] != ')') || Priority(str[i]) > Priority(GetTop(&opt)))//进栈不参与运算
			{
				if(Push2(&opt,str[i])==ERROR)
					printf("Push2 ERROR!");//运算符入栈
				i++;
				continue;
			}
			if(GetTop(&opt) == '(' && str[i] == ')')//出栈不参与运算
			{
				Popt(&opt);
				i++;
				continue;
			}
			if((str[i] == '\0' && EmptyStack(&opt) != OK) || (str[i] == ')' && GetTop(&opt) != '(') || Priority(str[i]) <= Priority(GetTop(&opt)))//出栈并参与运算
			{   a1=Pop2(&num);
			    a2=Pop1(&num);
			    a3=Pop2(&num);
			    a4=Pop1(&num);
			    a5=Popt(&opt);
				Calculator(a1,a2,a3,a4,a5,temps);//计算中间结果，得到字符串temps
			//	getfenzifenmu(&s,&m,temps);
				if(temps[0]=='-')
				 return -1;//判断中间结果是否为负

            	getfenzifenmu(&s,&m,temps);
				 if(Push1(&num,m,s)==ERROR)
					printf("Push3 ERROR!");//非负则中间结果入栈
				continue;
			}
		}
	}

    Simplify(s,m,fans);//化简分数，得到字符串fans
	//printf("%d.是%s\n",y,fans);
    fprintf(fp,"%d.%s\n",y,fans);//将答案写进文件
	fclose(fp);
	return 1;
}

char getopt()
{
   // srand(time(NULL));
    int a;
    a=rand()%4+1;
    switch(a)
    {
    	case 1:
    		return 43;
    	case 2:
    		return 45;
    	case 3:
		    return 120;
		case 4:
			return 246;
	}
}

void Truescore(int i,char string[])//获得一随机真分数
{
 int son,mom; //son为分子，mom为分母
 char str1[20]={'\0'};
 char str2[20]={'\0'};
 //srand(time(NULL));//我吃柠檬
 mom=rand()%(i-2)+2;  //分母的范围为2~i-1
 son=rand()%(mom-1)+1; //分母的范围为1~mom-1
 itoa(son,str1,10);
 itoa(mom,str2,10);
 strcat(str1,"/");
 strcat(str1,str2);
 strcpy(string,str1);
}

int question(int y,int r)//生成题目
{
    FILE *fp;
	fp=fopen("Exercises.txt","a");
	//time_t t;
	//srand(time(NULL));
    int num,num1,num2,num3,num4,ans;
    char opt1,opt2,opt3;
    char str[25]={};//用于传进答案函数
    char strf[25]={};//用于写进文件
    char strt[5]={" ÷ "};
    opt1=getopt();
    opt2=getopt();
    opt3=getopt();

    num1=rand()%r;
    num2=rand()%r;
    num3=rand()%r;
    num4=rand()%r;

    if(r<3) num=rand()%4+1;//r小于3时，不产生分数
    else num=rand()%6+1;

    if(num==1)//a+b
     {
     	while(r==1&&opt1==-10)
     	 opt1=getopt();//r是1则除号不参与运算

     	while(opt1==-10&&num2==0)
    	 num2=rand()%r;//0不可作为除数

     	char str1[5]={};
     	itoa(num1,str1,10);
     	strcat(str,str1);
     	strcat(strf,str1);

     	char str2[5]={' ',opt1,' '};
     	strcat(str,str2);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str2);

     	char str3[5]={};
     	itoa(num2,str3,10);
     	strcat(str,str3);
     	strcat(strf,str3);
	 }
	 if(num==2)//a+b+c
     {  while(r==1&&(opt1==-10||opt2==-10))
         {
          opt1=getopt();
          opt2=getopt();
		 }//r是1则除号不参与运算

     	while(opt1==-10&&num2==0)
    	 num2=rand()%r;//0不可作为除数
		while(opt2==-10&&num3==0)
    	 num2=rand()%r;//0不可作为除数

     	char str1[5]={};
     	itoa(num1,str1,10);
     	strcat(str,str1);
     	strcat(strf,str1);

     	char str2[5]={' ',opt1,' '};
     	strcat(str,str2);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str2);

     	char str3[5]={};
     	itoa(num2,str3,10);
     	strcat(str,str3);
     	strcat(strf,str3);

     	char str4[5]={' ',opt2,' '};
     	strcat(str,str4);
     	if(opt2==-10) strcat(strf,strt);
     	else strcat(strf,str4);

     	char str5[5]={};
     	itoa(num3,str5,10);
     	strcat(str,str5);
     	strcat(strf,str5);
	 }
	  if(num==3)//a+b+c+d
     {
         while(r==1&&(opt1==-10||opt2==-10||opt3==-10))
         {
          opt1=getopt();
          opt2=getopt();
          opt3=getopt();
		 }//r是1则除号不参与运算

     	while(opt1==-10&&num2==0)
    	 num2=rand()%r;//0不可作为除数
		while(opt2==-10&&num3==0)
    	 num3=rand()%r;//0不可作为除数
		while(opt3==-10&&num4==0)
    	 num4=rand()%r;//0不可作为除数

     	char str1[5]={};
     	itoa(num1,str1,10);
     	strcat(str,str1);
     	strcat(strf,str1);

     	char str2[5]={' ',opt1,' '};
     	strcat(str,str2);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str2);

     	char str3[5]={};
     	itoa(num2,str3,10);
     	strcat(str,str3);
     	strcat(strf,str3);

     	char str4[5]={' ',opt2,' '};
     	strcat(str,str4);
     	if(opt2==-10) strcat(strf,strt);
     	else strcat(strf,str4);

     	char str5[5]={};
     	itoa(num3,str5,10);
     	strcat(str,str5);
     	strcat(strf,str5);

     	char str6[5]={' ',opt3,' '};
     	strcat(str,str6);
     	if(opt3==-10) strcat(strf,strt);
     	else strcat(strf,str6);

     	char str7[5]={};
     	itoa(num4,str7,10);
     	strcat(str,str7);
     	strcat(strf,str7);
	 }

	  if(num==5)//a/b+c
	  {
	  	while(opt1==-10&&num1==0)
    	 num1=rand()%r;//0不可作为除数

	  	Truescore(r,str);
	  	strcpy(strf,str);

	  	char str1[5]={' ',opt1,' '};
     	strcat(str,str1);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str1);

     	char str2[5]={};
     	itoa(num1,str2,10);
     	strcat(str,str2);
     	strcat(strf,str2);
	  }

	  if(num==6)//a+b/c+d
	  {
	  	while(opt2==-10&&num2==0)
    	 num2=rand()%r;//0不可作为除数

     	char str1[5]={};
     	itoa(num1,str1,10);
     	strcat(str,str1);
     	strcat(strf,str1);

     	char str2[5]={' ',opt1,' '};
     	strcat(str,str2);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str2);

     	char str3[8]={};
	  	Truescore(r,str3);
     	strcat(str,str3);
     	strcat(strf,str3);

     	char str4[5]={' ',opt2,' '};
     	strcat(str,str4);
     	if(opt2==-10) strcat(strf,strt);
     	else strcat(strf,str4);

     	char str5[5]={};
     	itoa(num2,str5,10);
     	strcat(str,str5);
     	strcat(strf,str5);
	  }

	  if(num==4)//a+(b+c)
	  {
	   while(r==1&&(opt1==-10||opt2==-10))
         {
          opt1=getopt();
          opt2=getopt();
		 }//r是1则除号不参与运算

     	while(opt1==-10&&num2==0)
    	 num2=rand()%r;//0不可作为除数
		while(opt2==-10&&num3==0)
    	 num3=rand()%r;//0不可作为除数

     	char str1[5]={};
     	itoa(num1,str1,10);
     	strcat(str,str1);
     	strcat(strf,str1);

     	char str2[5]={' ',opt1,' '};
     	strcat(str,str2);
     	if(opt1==-10) strcat(strf,strt);
     	else strcat(strf,str2);

     	char left[3]={'('};
     	strcat(str,left);
     	strcat(strf,left);

     	char str3[5]={};
     	itoa(num2,str3,10);
     	strcat(str,str3);
     	strcat(strf,str3);

     	char str4[5]={' ',opt2,' '};
     	strcat(str,str4);
     	if(opt2==-10) strcat(strf,strt);
     	else strcat(strf,str4);

     	char str5[5]={};
     	itoa(num3,str5,10);
     	strcat(str,str5);
     	strcat(strf,str5);

     	char right[3]={')'};
     	strcat(str,right);
     	strcat(strf,right);
	 }

	// ans=Answer(str,y);//将题目字符串str传进生成答案函数Answer
	 if(Answer(str,y)<0)
	 {  fclose(fp);
	    return -1;
	 }//若计算答案过程中产生负数，则返回-1给main()处理
	 else//否则将题目写进Exercises.txt文件，并返回1给main()处理
	 {
	    //printf("%d.%s",y,str) ;
	    fprintf(fp,"%d.%s = \n",y,strf);
	    fclose(fp);
	    return 1;
     }
}

void Answer1(char str[],char answer[])//答案函数，str为运算式子,answer为结果
{
	Stack1 num;//分数数栈
	Stack2 opt;//运算符栈
	int i = 0, tmp = 0;
	int s,m;
	int a1,a2,a3,a4,a5;
	char temps[20]="\0";//存放中间运算结果
	char fans[20]="\0";//答案字符串

	if(InitStack1(&num) != OK || InitStack2(&opt) != OK)
	{
		printf("Init Failure!\n");
		exit(1);
	}

	while(str[i] != '\0' || EmptyStack(&opt) != OK)
	{
		if(str[i]==32)
		{
			i++;
			continue;
		}
		if(str[i] >= '0' && str[i] <= '9')
		{
			tmp = tmp * 10 + str[i] - '0';
			i++;
			if(str[i] < '0' || str[i] > '9')
			{
				if(Push1(&num,1,tmp)==ERROR)
			    	printf("Push1 ERROR!");//数字tmp/1入栈
				tmp = 0;
			}
		}
		else
		{
			if((EmptyStack(&opt) == OK) || (GetTop(&opt) == '(' && str[i] != ')') || Priority(str[i]) > Priority(GetTop(&opt)))//进栈不参与运算
			{
				if(Push2(&opt,str[i])==ERROR)
					printf("Push2 ERROR!");//运算符入栈
				i++;
				continue;
			}
			if(GetTop(&opt) == '(' && str[i] == ')')//出栈不参与运算
			{
				Popt(&opt);
				i++;
				continue;
			}
			if((str[i] == '\0' && EmptyStack(&opt) != OK) || (str[i] == ')' && GetTop(&opt) != '(') || Priority(str[i]) <= Priority(GetTop(&opt)))//出栈并参与运算
			{   a1=Pop2(&num);
			    a2=Pop1(&num);
			    a3=Pop2(&num);
			    a4=Pop1(&num);
			    a5=Popt(&opt);
				Calculator(a1,a2,a3,a4,a5,temps);//计算中间结果，得到字符串temps
				getfenzifenmu(&s,&m,temps);

				if(Push1(&num,m,s)==ERROR)
					printf("Push3 ERROR!");//非负则中间结果入栈
				continue;
			}
		}
	}

    Simplify(s,m,fans);//化简分数，得到字符串fans
    strcpy(answer,fans);

}

int getquestion(FILE *f,int *n,char str[]) //取文件题目
{
	char q[30]="\0";
	char no[10]="\0";
	int i=0,j=0,tag=0;  //tag为1时找到顿号.
	if(fgets(q,30,f)==NULL)  //读取失败
		return 1;
	while(q[i]!='\n')
	{
		if(tag==0){
			if(q[i]=='.')
				tag=1;
			else
				no[i]=q[i];
		}
		else{
			if(q[i]==-95)
			{
				str[j]=-62;
				j++;
				i++;
			}
			else if(q[i]!='='&&q[i]!=' '){
				str[j]=q[i];
				j++;
			}
			else if(q[i]=='='){
				str[j]='\0';
				break;
			}
		}
		i++;
	}
	*n=atoi(no);
	return 0;
}

void getanswer(FILE *f,int *n,char str[])
{
	char a[30]="\0";
	char no[10]="\0";
	int i=0,j=0,tag=0;  //tag为1时找到顿号.
	fgets(a,30,f);
	while(a[i]!='\n')
	{
		if(tag==0){
			if(a[i]=='.')
				tag=1;
			else
				no[i]=a[i];
		}
		else{
			if(a[i]!=' '){
				str[j]=a[i];
				j++;
			}
		}
		i++;
	}
	str[j]='\0';
	*n=atoi(no);
}

void check(char exercisefile[],char answerfile[])
{
	FILE *f1,*f2,*f3;
	int correct=0,wrong=0; //分别为对和错的题目数量
	int n1=0,n2=0;//题目序号
	int i;
	int correctno[10000]={0};
	int wrongno[10000]={0};
	char question[25]="\0";
	char answer[25]="\0";
	char canswer[25]="\0";  //正确答案
	f1=fopen(exercisefile,"r");
	f2=fopen(answerfile,"r");
	f3=fopen("Grade.txt","w");
	while(!feof(f1))
	{
		if(getquestion(f1,&n1,question))  //读取题目失败
			break;
		getanswer(f2,&n2,answer);
		Answer1(question,canswer);
		if(!strcmp(canswer,answer))  //答案正确
		{
			correctno[correct]=n1;
			correct++;
		}
		else{
			wrongno[wrong]=n2;
			wrong++;
		}
	}
	fprintf(f3,"Correct: %d (",correct);
	for(i=0;i<correct;i++)
	{
		fprintf(f3,"%d",correctno[i]);
		if(i!=correct-1)
			fprintf(f3,", ");
	}
	fprintf(f3,")\nWrong: %d (",wrong);
	for(i=0;i<wrong;i++)
	{
		fprintf(f3,"%d",wrongno[i]);
		if(i!=wrong-1)
			fprintf(f3,", ");
	}
	fprintf(f3,")");
	fclose(f1);
	fclose(f2);
	fclose(f3);
}
int main(int argc, char *argv[])
{
 if(argc<2)
    {
        printf("you must input args!");
        return 0;
    }//用户无参数输入
 int y=1;
 int n,r,flag;
 time_t t;
 srand((unsigned) time(NULL));
 n=atoi(argv[2]);
 r=atoi(argv[4]);//将char转换为int
 if(!strcmp(argv[1],"-n")&&!strcmp(argv[3],"-r"))
    {
        while(n>0)
        {
            //flag=question(y,r);//question()的返回值为生成题目的答案
            if(question(y,r)<0) continue;
            y++;
            n--;
        }
    }
  return 0;
}
