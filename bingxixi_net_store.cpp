#include<stdio.h> 
#include<graphics.h> 
#include<string.h> 
#include<process.h> 
#include<dos.h> 
#include<conio.h> 
#include<stdlib.h> 
#include<malloc.h> 
#include<bios.h> 
#include<stddef.h> 
#include<time.h> 
#include<math.h> 
#include<dir.h> 
#include<direct.h> 
#include<bmp.h> 
 
#define Max_name_length 20 
#define Max_name_pinyin 40 
#define Max_miaoshu_length 50 
#define Key_Enter 0x1c0d 
#define Key_Backspace 0x0e08 
#define Key_Esc 0x011b 
typedef struct node1 //�û��ṹ�� 
{ 
 	char name[12]; // �� �� �� 
 	char mima[12]; // �� ��   
 	char zhifumima[12];//֧������ 
 	char phone[12];    //�ֻ����� 
 	char address[12]; //��ַ 
 	float money; // �� �� �� �� 
 	struct node1 *next; 
}yonghu; 
 
typedef struct node2 //����Ա�ṹ�� 
{ 
 	char name[12]; 
 	char mima[12]; 
 	struct node2 *next; 
}guanli; 
 
typedef struct node3 //��Ʒ�ṹ�� 
{ 
 
 	int life; // �� �� �� �� �� �� �� �� �� �� 
 	int flag;   
 	int cla;  //�����ж��û��Ƿ��Ѹ��� 
 	char bianhao[10]; 
 	int kucun; // �� �� �� 
 	int xiaoshou; // �� �� �� 
 	long t;             // �� �� �� �� �� Ʒ �� ʱ ��float chengben; // �� Ʒ �� �� 
 	float price; // �� �� 
 	float  profit;               // �� ��char name[Max_name_length]; 
 	char name_pinyin[Max_name_pinyin]; //��Ʒ����ƴ������������ 
 	char miaoshu[Max_miaoshu_length]; // �� Ʒ �� ��                
 	struct node3 *next; 
}shangpin; 
 
typedef struct BANK_CARD //���п��ṹ�� 
{ 
 	char count[12]; 
 	char mima[12]; 
 	float money; // �� �� �� �� 
 	struct BANK_CARD *next; 
}bank; 
 
typedef struct Liuyan //���Խṹ�� 
{ 
 	int flag; //0 ��ʾ�û������̵����� 
 	int life; //0 ��ʾ�Ѷ���1 ��ʾδ�� 
 	char yonghuname[12]; //�û��� 
 	char xinxi[80]; 
 	struct Liuyan *next; 
}liuyan; 
 
typedef struct Dingdan //�����ṹ�� 
{ 
 	int life; //����Ƿ񱻴���Ķ��� 
 	char bianhao[10];//��Ʒ��� 
 	char name[12]; //�û��� 
 	char phone[12]; //�û��ֻ��� 
 	char address[12];//�û���ַ 
 	struct Dingdan *next; 
}dingdan; 
 
 
 
/************************* main.c ****************************/ 
 
void main() 
{ 
   int driver=VGA; //����ͼ�ν����ʼ�� 
   int mode=VGAHI; 
   int mx,my,mbutt; // �� �� 
   int iden=0; // �� �� �� �� 
   char str_name[12],str_mima[12]; //�û��������� 
   yonghu *current; //ָ��ǰ�û� 
   yonghu *yh_head=yhcreat();  //�����û����� 
   guanli *gl_head=glcreat();  //��������Ա���� 
   shangpin *sp_head=spcreat();  //������Ʒ���� 
   liuyan*ly_head=lycreat();     //������������ 
   str_name[0]='\0'; 
   str_mima[0]='\0'; 
   initgraph(&driver,&mode,"c:\\borlandc\\bgi"); 
   init_mouse(0,639,0,479); //����ʼ�� 
   mbutt=0; 
   for(;;) 
   { 
      dljiemian(); // �� ½ �� �� �� �� 
 	  for(;;) 
 	  { 
 	     mbutt=0; 
 	     delay(20); 
 	 	 newxy(&mx,&my,&mbutt); 
 	 	 dl_xuanze(&mx,&my,&mbutt,&iden);       // ѡ �� �� ¼ �� �� 
 	 	 dl_input(&mx,&my,&mbutt,str_name,str_mima); //�����¼��Ϣ 
 	 	 zhuce(&mx,&my,&mbutt,yh_head,gl_head); // ע �� �� �� 
 	 	 if(dl_yanzheng(&mx,&my,&mbutt,str_name,str_mima,iden)==1) // �����¼��Ϣ�Ƿ���д��ȫ�� 
 	 	 { 
 	 	 	 if((xinxi_yz(iden,str_name,str_mima,yh_head,gl_head,&current,&mx,&my,&mbutt))== 1) 
 	 	 	 { // �� �� �� ¼ �� Ϣ �� �� �� ȷ 
 	 	 	  	str_name[0]='\0'; 
                str_mima[0]='\0'; 
                delay(500); 
 	 	 	  	break; 
 	 	 	 }       
 	 	 } 
 	  } 
 
    if(iden==1) //��¼���Ϊ 1 ʱ�������û�ģ�� 
 	      customer(&mx,&my,&mbutt,current,yh_head,sp_head,ly_head); 
    if(iden==2) // �� �� �� �� Ա ģ �� 
    { 
          dingdan*dd_head=ddcreat();        //������������ 
          manager(&mx,&my,&mbutt,ly_head,sp_head,dd_head); 
 	} 
  } 
} 
yonghu * yhcreat(void) // �� �� �� �� �� �� 
{ 
   FILE *fp; 
   int size; 
   char s[30]; 
   yonghu *head,*p1,*p2; //head ָ������ͷ�ڵ㣬p1 ָ��ǰ�ڵ㣬p2 ָ����һ�ڵ� 
   size=sizeof(yonghu); 
   head=(yonghu*)malloc(size); 
   if(head==NULL) 
   { 
 	  printf("yonghu creat:no enough memory"); 
 	  exit(1); 
   } 
   if((fp=fopen("txt\\yonghu.txt","rt"))==NULL) //ֻ��ģʽ���ļ� 
   { 
 	 printf("unable to open yonghu.txt"); 
 	 exit(0); 
   } 
   p1=p2=head; 
   rewind(fp); 
   
fscanf(fp,"%s %s %s %s %s %s\n",head->name,head->mima,head->zhifumima,head->p hone,head->address,s); //��ʽ����ʽ�����ļ���Ϣ 
   head->money=atof(s); //�����ַ�����ʽ������ת���ɸ����� 
   head->next=NULL; 
   while(!feof(fp)) 
   { 
 	  if( ( p1=(yonghu*)malloc(sizeof(yonghu))) ==NULL)//���䶯̬�ڴ沢��� 
 	  { 
 	 	printf("No enough memory"); 
 	 	exit(0); 
 	  } 
 	   
fscanf(fp,"%s %s %s %s %s %s\n",p1->name,p1->mima,p1->zhifumima,p1->phone,p1->a ddress,s); 
 
 	   p1->money=atof(s); 
 	   p2->next=p1; �� p1 �ĵ�ַ���� p2 ��ָ��Ľ������������Ͱ� p1 �� p2 ��ָ��Ľ������������ 
 	   p1->next=NULL; 
 	   p2=p1; 
   } 
   fclose(fp); 
   return head; 
} 
 
guanli *glcreat() //����Ա��������ԭ��ͬ�û�����Ĵ��� 
{ 
   FILE *fp; 
   guanli *p1,*p2,*head; 
   head=(guanli*) malloc(sizeof(guanli)); 
   if(head==NULL) 
   { 
 	 printf("guanli creat:no enough memory"); 
 	 exit(0); 
   } 
   p1=p2=head; 
   if((fp=fopen("txt\\guanli.txt","rt"))==NULL) 
   { 
 	  printf("unable to open guanli.txt"); 
 	  exit(0); 
   } 
   rewind(fp); 
   fscanf(fp,"%s %s\n",head->name,head->mima); 
   head->next=NULL; 
   while(!feof(fp)) 
   { 
 	 p1=(guanli*) malloc(sizeof(guanli)); 
 	 if(p1==NULL) 
 	 { 
 	   printf("No enough memory"); 
 	   exit(0); 
 	 } 
 	 fscanf(fp,"%s %s",p1->name,p1->mima); 
 	 p2->next=p1; 
 	 p1->next=NULL; 
 	 p2=p1; 
   } 
   fclose(fp); 
   return head; 
 
} 
 
shangpin* spcreat() 
{ 
    FILE *fp; 
    shangpin *sh,*s1,*s2;   
 	char str[20];                //sh ָ������ͷ�ڵ㣬s1 ָ��ǰ�ڵ㣬s2 ָ����һ�ڵ� 
    char str1[20]; 
    char str2[20]; 
    char str3[20]; 
    if((sh=(shangpin*)malloc(sizeof(shangpin)))==NULL) 
    { 
        printf("No enough memory when build shangpin list\n"); 
        exit(1); 
    } 
    s1=s2=sh; 
    if((fp=fopen("txt\\shangpin.txt","rt"))==NULL) 
    { 
        printf("unable to open shangpin.txt"); 
        exit(1); 
    } 
    rewind(fp); 
    fscanf(fp,"%d %d %d %s %d %d %s %s %s %s %s %s %s\n",&sh->life,&sh->flag,&s h->cla,sh->bianhao,&sh->kucun,&sh->xiaoshou,str,str1,str2,str3,sh->name,sh->nam e_pinyin,sh->miaoshu);   
    sh->t=0; 
    sh->chengben=atof(str1); 
    sh->price=atof(str2); 
    sh->profit=atof(str3); 
    sh->next=NULL; 
    while(!feof(fp)) 
    { 
       if((s1=(shangpin*)malloc(sizeof(shangpin)))==NULL) 
       { 
             printf("No enough memory when build shangpin list\n"); 
             exit(1); 
       }     
       
fscanf(fp,"%d %d %d %s %d %d %s %s %s %s %s %s %s\n",&s1->life,&s1->flag,&s1->c la,s1->bianhao,&s1->kucun,&s1->xiaoshou,str,str1,str2,str3,s1->name,s1->name_pi nyin,s1->miaoshu); 
       s2->next=s1; 
       s1->t=0; 
 
       s1->chengben=atof(str1); 
       s1->price=atof(str2); 
       s1->profit=atof(str3); 
       s1->next=NULL; 
       s2=s1; 
    } 
    fclose(fp); 
    return sh; 
} 
liuyan *lycreat(void) //�������������� 
{ 
   FILE *fp; 
   int size; 
   liuyan *head,*p1,*p2; 
   size=sizeof(liuyan); 
   head=(liuyan*)malloc(size); 
   if(head==NULL) 
   { 
    printf("liuyan creat:no enough memory"); 
    exit(1); 
   } 
   if((fp=fopen("txt\\liuyan.txt","rt"))==NULL) 
   { 
   printf("unable to open liyan.txt"); 
   exit(0); 
   } 
   p1=p2=head; 
   rewind(fp); 
   
fscanf(fp,"%d %d %s %s\n",&head->flag,&head->life,head->yonghuname,head->xinxi); 
   head->next=NULL; 
   while(!feof(fp)) 
   { 
    if( ( p1=(liuyan*)malloc(sizeof(liuyan))) ==NULL) 
    { 
    printf("No enough memory"); 
    exit(0); 
    } 
     fscanf(fp,"%d %d %s %s\n",&p1->flag,&p1->life,p1->yonghuname,p1->xinxi); 
     p2->next=p1; 
     p1->next=NULL; 
     p2=p1; 
   } 
   fclose(fp); 
 
   return head; 
} 
dingdan *ddcreat() //�������������� 
{ 
   FILE *fp; 
   int size; 
   dingdan *head,*p1,*p2; 
   size=sizeof(dingdan); 
   head=(dingdan*)malloc(size); 
   if(head==NULL) 
   { 
      printf("dingdan creat:no enough memory"); 
      exit(1); 
   } 
   if((fp=fopen("txt\\dingdan.txt","rt"))==NULL) 
   { 
     printf("unable to open dingdan.txt"); 
     exit(0); 
   } 
   p1=p2=head; 
   rewind(fp); 
fscanf(fp,"%d %s %s %s %s\n",&head->life,head->bianhao,head->name,head->phone
,head->address); 
   head->next=NULL; 
   while(!feof(fp)) 
   { 
      if( ( p1=(dingdan*)malloc(sizeof(dingdan))) ==NULL) 
      { 
        printf("No enough memory"); 
        exit(0); 
      } 
       
fscanf(fp,"%d %s %s %s %s\n",&p1->life,p1->bianhao,p1->name,p1->phone,p1->a ddress); 
       p2->next=p1; 
       p1->next=NULL; 
       p2=p1; 
   } 
   fclose(fp); 
   return head; 
} 
void tuichu(int *mx,int *my,int *mbutt) // �� �� �� �� �� 
{ 
 	  void *buffer; 
 
      unsigned size; 
      size=imagesize(170,140,470,340); //�����Ļ����Ĵ�С 
      if(((void*)buffer=malloc(size))==NULL)//�����ڴ�ռ䲢��� 
      { 
         printf("No enough memory"); 
         exit(1); // �� �� ʧ �� �� �� �� 
      } 
      getimage(170,140,470,340,buffer); //��ȡ��Ļ��ͼ 
      setfillstyle(1,WHITE); // �� �� �� �� �� 
      bar(170,140,470,340); 
      setfillstyle(1,DARKGRAY); 
      bar(170,140,470,160); 
      setfillstyle(1,LIGHTGRAY); 
      bar(190,310,230,330); 
      bar(410,310,450,330); 
      out_hz(260,230,"ȷ���˳���",24,0,BLACK); 
      out_hz(194,312,"ȡ��",16,16,BLACK); 
      out_hz(414,312,"ȷ��",16,16,BLACK); 
      for(;;) 
      { 
        newxy(mx,my,mbutt); 
 	    if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //���ȡ��ʱ 
 	    { 
 	       putimage(170,140,buffer,0); 
 	       cursor(*mx,*my); 
 	       free(buffer); 
 	       break; 
        } 
        if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1)     
 	       exit(0);       
      } 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
/********************************denglu.c*************************************/ 
 
void dl_xuanze(int *mx,int *my,int *mbutt,int *iden) //��¼���ѡ�� 
{  
   if(*mx>547&&*mx<585&&*my>427&&*my<449&&*mbutt==1) // �� �� �� �� 
      tuichu(mx,my,mbutt);    
   if(*mx>205&&*mx<215&&*my>185&&*my<195&&*mbutt==1) //ѡ��ͻ���¼ 
   { 
      yh_dl_xz();            
      *iden=1; 
   } 
   else if(*mx>310&&*mx<320&&*my>185&&*my<195&&*mbutt==1) //ѡ�����Ա��¼ 
   { 
      gl_dl_xz(); 
      *iden=2; 
   } 
} 
 
void dl_input(int *mx,int *my,int *mbutt,char *str_name,char *str_mima) 
{ 
   if(*mx>252&&*mx<425&&*my>210&&*my<230&&*mbutt==1) // �� �� �� �� 
   { 
      setfillstyle(1,WHITE); 
      bar(250,210,425,230);               
 	  input_name(str_name,254,212,12); 
   } 
   if(*mx>252&&*mx<425&&*my>250&&*my<270&&*mbutt==1) // �� �� �� �� 
   { 
      setfillstyle(1,WHITE); 
      bar(250,250,425,270); 
      input_mima(str_mima,254,252); 
   } 
} 
 
int dl_yanzheng(int *mx,int *my,int *mbutt,char*str_name,char*str_mima,int iden) 
//���������Ϣ�Ƿ����� 
{ 
 	if(*mx>320&&*mx<360&&*my>315&&*my<335&&*mbutt==1) // �� �� �� ¼ 
    { 
      if(!(iden==1||iden==2)) // û �� ѡ �� �� �� ʱ 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"��ѡ����ݣ�"); 
 	 	return 0; 
      } 
 
      else if(*str_name=='\0') //û�������ʺ�ʱ 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"�������ʺţ�"); 
 	 	return 0; 
      } 
      else if(*str_mima=='\0') //û����������ʱ 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"���������룡"); 
 	 	return 0; 
 	  } 
 	  else     
 	    return 1; //ȷ����Ϣ��д��������������ֵΪ 1������Ϊ 0.   
    } 
} 
 
int xinxi_yz(int iden,char *str_name,char*str_mima,yonghu*yh,guanli*gh,yonghu 
**cu,int *mx,int *my,int *mbutt) // �� ֤ �� ¼ �� Ϣ 
{ 
 	yonghu*p; 
 	guanli*q; 
 	switch(iden)                     
 	{ 
 	 	case 1: // �� �� �� �� ¼ ʱ 
 	 	 	if( (p=yh_name_yz(str_name,yh))==NULL) //��������û���ҵ��û���ʱ 
 	 	      { 
 	 	         	outinformation(mx,my,mbutt,200,200,"�ʺŲ����ڣ�"); 
 	 	         	return 0; 
 	 	      } 
 	 	 	else 
 	 	        { 
 	 	 	 	    if(yh_mima_yz(str_mima,p)==0) //��⵽�������ʱ 
 	 	         	{ 
 	 	         	 	outinformation(mx,my,mbutt,200,200,"�������"); 
 	 	         	 	return 0; 
 	 	         	} 
 	 	         	Else // �� ֤ ͨ �� 
 	 	         	{ 
                        *cu=p; �ѵ�ǰ��¼�û���ָ�븳�� cu 
                        return 1; 
 	 	 	 	 	} 
                } 
 	 	        break; 
 	 	case 2:  
 	 	        if((q=gl_name_yz(str_name,gh))==NULL)//��������û���ҵ�����Աʱ 
 
 	 	        { 
 	 	         	outinformation(mx,my,mbutt,200,200,"�ʺŲ����ڣ�"); 
 	 	         	return 0; 
 	 	        } 
 	 	 	 	else 
 	 	        { 
 	 	 	 	 	if(gl_mima_yz(str_mima,q)==0) //�����������ʱ 
 	 	         	{ 
 	 	         	 	outinformation(mx,my,mbutt,200,200,"�������"); 
 	 	         	 	return 0; 
 	 	         	} 
 	 	 	 	 	if(gl_mima_yz(str_mima,q)==1) //������Ϣͨ��ʱ 
                        return 1; 
 	 	        } 
 	 	 	 	break; 
 	} 
} 
 
void yh_dl_xz() //ѡ���û����ʱ���û�ȦȦͿ�ڣ��ѹ���ԱȦȦͿ�� 
{ 
 	setcolor(BLACK); 
    circle(210,190,3); 
    setfillstyle(1,BLACK); 
    floodfill(210,190,BLACK); 
    setcolor(LIGHTGRAY); 
    circle(315,190,3); 
    setfillstyle(1,LIGHTGRAY); 
    floodfill(315,190,LIGHTGRAY); 
} 
void gl_dl_xz() //ѡ�����Ա���ʱ���û�ȦȦͿ�ڣ����û�ȦȦͿ�� 
{ 
 	setcolor(BLACK); 
    circle(315,190,3); 
    setfillstyle(1,BLACK); 
    floodfill(315,190,BLACK); 
    setcolor(LIGHTGRAY); 
    circle(210,190,3); 
    setfillstyle(1,LIGHTGRAY); 
    floodfill(210,190,LIGHTGRAY); 
} 
 
yonghu* yh_name_yz(char *str1,yonghu *p) //�û���¼�ʺ���֤���� 
{ 
   while(p!=NULL) 
 
   { 
 	 if(strcmp(p->name,str1)==0) //��⵽��Ӧ�û����˳�ѭ�� 
 	  	break;  
 	 else 
 	  	p=p->next; 
   } 
   return p; //�ҵ���Ӧ�û������� yonghu ָ�� P�����򷵻ؿ�ָ�� 
} 
int yh_mima_yz(char*str2,yonghu *p) 
{ 
 	 	if(strcmp(p->mima,str2)==0) //���������֤�ɹ�,��������ֵΪ 1�����򷵻�0.   	 
 	 	   return 1; 	 	 
 	 	else 	 	 
 	 	  return 0;  	 
} 
  	 
guanli* gl_name_yz(char *str1,guanli *p) //����Ա�ʺ���֤ 
{ 
   while(p!=NULL) 
   { 
 	 if(strcmp(p->name,str1)==0) 
 	 	break;    
 	 else 
 	  	p=p->next; 
   } 
   return p; 
} 
int gl_mima_yz(char *str2,guanli *p) //����Ա������֤ 
{ 
 	 	if(strcmp(p->mima,str2)==0)  //���������֤�ɹ�,��������ֵΪ 1�����򷵻� 0. 	 	 
 	 	  return 1;  	 
 	 	else 
 	 	  return 0; 
}  
 
void dljiemian() //��¼������� 
{ 
  cleardevice(); 
  int triangle1[]={150,160,460,160,460,70}; //��������������� 
  int triangle2[]={160,430,160,175,80,175}; 
  int triangle3[]={175,350,175,440,485,350}; 
  int triangle4[]={470,335,550,335,470,70}; 
 
  setbkcolor(BLACK); 
  setcolor(LIGHTBLUE); 
  rectangle(0,0,640,480); 
  setfillstyle(1,LIGHTBLUE); 
  floodfill(320,240,LIGHTBLUE); 
  setfillstyle(1,YELLOW); 
  fillpoly(3,triangle1); 
  setfillstyle(1,GREEN); 
  fillpoly(3,triangle2); 
  setfillstyle(1,LIGHTRED); 
  fillpoly(3,triangle3); 
  setfillstyle(1,BROWN); 
  fillpoly(3,triangle4); 
  setcolor(LIGHTGRAY); 
  setfillstyle(1,LIGHTGRAY); 
  rectangle(172,172,450,340); 
  floodfill(320,240,LIGHTGRAY); 
  setfillstyle(1,WHITE); 
  bar(250,210,425,230); 
  bar(250,250,425,270); 
  setfillstyle(1,LIGHTBLUE); 
  bar(320,315,360,335); 
  bar(370,315,410,335); 
  setcolor(DARKGRAY); 
  rectangle(202,210,250,230); 
  rectangle(202,250,250,270); 
  setfillstyle(1,DARKGRAY); 
  floodfill(230,220,DARKGRAY); 
  floodfill(220,260,DARKGRAY); 
  out_hz(204,210,"�û���",16,16,BLACK); 
  out_hz(204,250,"����",16,26,BLACK); 
  out_hz(325,317,"��¼",16,16,WHITE); 
  out_hz(375,317,"ע��",16,16,WHITE); 
  out_hz(550,430,"�˳�",16,16,WHITE); 
  setcolor(BLACK); 
  circle(210,190,5); 
  outtextxy(220,187,"Customer"); 
  circle(315,190,5); 
  outtextxy(325,187,"Administrator"); 
  setlinestyle(0,0,3); 
  setcolor(WHITE); 
  rectangle(547,427,585,449); 
  timeoutput(420,20); 
 
/**********************************zhuce.c********************************/ void zhuce(int *mx,int *my,int *mbutt,yonghu *yh,guanli *gh)//ע�������� 
{ 
   if(*mx>370&&*mx<410&&*my>315&&*my<335&&*mbutt==1) //�����¼�����ϵĵ�ע�� 
   {                                     
 	  zhuce_menu(); // ע �� �� �� �� �� 
 	  for(;;) 
 	  { 
 	     newxy(mx,my,mbutt); 
 	     if(*mx>370&&*mx<450&&*my>335&&*my<355&&*mbutt==1)//����û�ע�� 
 	     { 
 	      	yh_zhuce_jm();                                        // �� �� ע�������� 
 	      	if(yh_zhuce(mx,my,mbutt,yh)==1) //�û�ע��ɹ�ʱ���ص�¼���� 
 	      	{ 
 	      	 	dljiemian(); 
 	      	 	break; 
 	      	} 
 	     } 
 	     if(*mx>370&&*mx<450&&*my>355&&*my<375&&*mbutt==1)//�������Աע�� 
 	     { 
 	      	gl_zhuce_jm(); 
 	      	if(gl_zhuce(mx,my,mbutt,gh)==1) //����Աע��ɹ����ص�¼���� 
 	      	{ 
 	      	 	dljiemian(); 
 	      	 	break; 
 	      	} 
 	 	 } 
 	  } 
   } 
} 
 
void zhuce_menu()  //ע��˵����� 
{ 
 	setcolor(BLACK); 
 	setfillstyle(1,WHITE); 
 	bar(370,335,450,355); 
 	bar(370,355,450,375); 
 	rectangle(370,335,450,355); 
 	rectangle(370,355,450,375); 
 	setcolor(BLACK); 
 	out_hz(370,336,"���û�ע��",16,16,BLACK); 
 	out_hz(370,356,"����Աע��",16,16,BLACK); 
 
void yh_zhuce_jm() //�û�ע�������� 
{ 
   cleardevice(); 
   int i,x=220,y=153; 
   setbkcolor(BLACK); 
   setfillstyle(1,LIGHTBLUE); 
   bar(0,0,640,480); 
   setfillstyle(1,LIGHTGRAY); 
   bar(200,100,440,380); 
   setfillstyle(1,BROWN); 
   bar(200,100,440,125); 
   for(i=0;i<5;i++) 
   { 
 	  setfillstyle(1,WHITE); 
 	  bar(x,y,x+200,y+20); 
 	  setfillstyle(1,BROWN); 
 	  bar(x,y,x+64,y+20); 
 	  y+=40; 
   } 
   bar(380,350,420,370); 
   y=153; 
   out_hz(280,105,"���û�ע��",16,20,BLACK); 
   out_hz(220,155,"�û���",16,25,BLACK); 
   out_hz(x+11,y+41,"����",16,26,BLACK); 
   out_hz(x+11,y+80,"�ֻ�",16,26,BLACK); 
   out_hz(x+11,y+120,"��ַ",16,26,BLACK); 
   out_hz(x,y+160,"֧������",16,16,BLACK); 
   out_hz(383,350,"ȷ��",16,20,BLACK); 
   setcolor(WHITE); 
   circle(30,30,12); 
   setlinestyle(0,0,3); 
   line(20,30,32,25); 
   line(20,30,32,35); 
   line(20,30,40,30); 
   setlinestyle(0,0,1); 
   timeoutput(420,20); 
} 
int yh_zhuce(int *mx,int *my,int *mbutt,yonghu *yh) //�û�ע�ắ�� 
{ 
 	int i,n=0,a[5]; //�������ڱ��ÿһ����Ϣ��д��״̬����д���ֵΪ 1 
 	char shuzu[5][12];//�ַ��������ڽ����û��������Ϣ 
 	for(i=0;i<5;i++) 
 	 	a[i]=0; // �� �� a[5] �� �� ʼ �� 
 	for(;;) 
 
 	{ 	
 	 	n=a[0]+a[1]+a[2]+a[3]+a[4]; //n Ϊ���� a ����Ԫ�صĺ� 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>18&&*mx<42&&*my>18&&*my<42&&*mbutt==1) //������ذ�ťʱ 
 	 	 	break; 
 	 	if(*mx>284&&*mx<420&&*my>153&&*my<173&&*mbutt==1)//����û�������� 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,153,420,173); 
 	 	 	input_name(shuzu[0],286,155,12); //�����û������洢������[0] 
 	 	 	if(shuzu[0][0]!='\0') //ȷ���û�����д��ɣ�a[0]��ֵ 1 
 	 	 	 	a[0]=1;  	     
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>193&&*my<213&&*mbutt==1) //������������ 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,193,420,213); 
 	 	 	input_name(shuzu[1],286,195,12); //����洢�� shuzu[1] 
 	 	 	if(shuzu[1][0]!='\0') //ȷ��������д��ɣ�a[1]��ֵ 1 
 	 	 	    a[1]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>233&&*my<253&&*mbutt==1) //��������ֻ��� 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,233,420,253); 
 	 	 	input_name(shuzu[2],286,235,12); //�ֻ��Ŵ洢�� shuzu[2] 
 	 	 	if(shuzu[2][0]!='\0') //ȷ���ֻ�����д��ɣ�a[2]��ֵ 1 
 	 	 	    a[2]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>273&&*my<293&&*mbutt==1) //�����ַ����� 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,273,420,293); 
 	 	 	input_name(shuzu[3],286,275,12); //��ַ�洢�� shuzu[3], 
 	 	 	if(shuzu[3][0]!='\0') //ȷ�ϵ�ַ��д��ɣ�a[3]��ֵ 1   	    
		 a[3]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>313&&*my<333&&*mbutt==1) //���֧����������� 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,313,420,333); 
 	 	 	input_name(shuzu[4],286,315,12);//֧������洢�� shuzu[4] 
 	 	 	if(shuzu[4][0]!='\0') //ȷ��֧��������д��ɣ�a[4]��ֵ 1 
 
 	 	 	    a[4]=1; 
 	 	} 
 	 	if(*mx>380&&*mx<420&&*my>350&&*my<370&&*mbutt==1) //���ȷ�� 
 	 	{ 
 	 	 	if(n!=5) //��Ϣ��д����ȫ 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"��Ϣ����ȫ��"); 
 	 	 	else 
 	 	 	{ 
 	 	 	 	if(yh_name_yz(shuzu[0],yh)!=NULL)//����û����Ƿ��Ѵ���  	 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"�ʻ��Ѵ��ڣ�");  
 	 	
 	 	 	 	else //��⵽�û�������ʱ�������û���Ϣд���ļ� 
 	 	 	 	{ 
 	 	 	 	 	yh_xieru(yh,shuzu);   
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"ע��ɹ���"); 
 	 	 	 	    break; //��֤��д��,����дһ��д�ļ��ĺ��� 
 	 	 	 	} 
 	 	 	} 
 	 	} 
 		
 	} 
 	return 1; // ע �� �� �� �� �� �� ֵ �� �� 1 
} 
void yh_xieru(yonghu*h,char c[][12]) //�����û�д���ļ��ĺ��� 
{ 
 	FILE *fp; 
 	yonghu *p,*q; 
    char s[30]="txt\\";//s ���ڱ�ʾ���û���ר�� txt �ļ�·�� 
 	p=h; 
 	if((q=(yonghu *)malloc(sizeof(yonghu)))==NULL)//����ռ䲢��� 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	strcpy(q->name,c[0]); //���û���Ϣд���¿ռ��� 
 	strcpy(q->mima,c[1]); 
 	strcpy(q->phone,c[2]); 
 	strcpy(q->address,c[3]); 
 	strcpy(q->zhifumima,c[4]); 
 	q->money=0; // �� �� �� �� �� �� 0 strcat(s,c[0]);  
 	strcat(s,".txt"); //�� s ���ӳ�"txt\\yonghuname.txt"����ʽ 
 	while(p->next!=NULL) //ʹ p ָ���û��������һ���ڵ� 
 	 	p=p->next; 
 
 	p->next=q; // ʹ p ָ �� q 
 	q->next=NULL; 
 	p=h; //���½��û�ͷָ�븳�� p 
 	if( (fp=fopen("txt\\yonghu.txt","wt"))==NULL ) //�� yonghu.txt 
    { 
     	printf("unable to open yonghu.txt"); 
     	exit(1); 
    } 
while(p!=NULL) // �� �� �� д �� �� �� 
{     
fprintf(fp,"%s %s %s %s %s %f\n",p->name,p->mima,p->zhifumima,p->phone, p->address,p->money); 
     	p=p->next; 
    } 
    fclose(fp); 
    if( (fp=fopen(s,"wt"))==NULL) //Ϊ�û��½�һ���ı��ļ� 
    { 
     	printf("unable to open txt when build Customer list"); 
     	exit(1); 
    } 
    fclose(fp); 
} 
void gl_zhuce_jm() //����Աע�������� 
{ 
   cleardevice(); 
   setbkcolor(BLACK); 
   setfillstyle(1,LIGHTBLUE); 
   bar(0,0,640,480); 
   setfillstyle(1,LIGHTGRAY); 
   bar(200,150,440,330); 
   setfillstyle(1,BROWN); 
   bar(200,150,440,175); 
   setfillstyle(1,WHITE); 
   bar(220,190,420,210); 
   bar(220,230,420,250); 
   bar(220,270,420,290); 
   setfillstyle(1,BROWN); 
   bar(220,190,270,210); 
   bar(220,230,270,250); 
   bar(220,270,270,290); 
   bar(383,305,423,325); 
   out_hz(280,153,"����Աע��",16,16,BLACK); 
   out_hz(231,191,"�˺�",16,20,BLACK); 
   out_hz(231,231,"����",16,20,BLACK); 
 
   out_hz(220,271,"���к�",16,16,BLACK); 
   out_hz(385,306,"ȷ��",16,20,BLACK); 
   setcolor(WHITE); 
   circle(30,30,12); 
   setlinestyle(0,0,3); 
   line(20,30,32,25); 
   line(20,30,32,35); 
   line(20,30,40,30); 
   setlinestyle(0,0,1); 
   timeoutput(420,20); 
} 
int gl_zhuce(int *mx,int *my,int *mbutt,guanli *gh) //����Աע�ắ�� 
{ 
 	char shuzu[3][12]; //�ַ����� shuzu ���ڽ���������Ϣ 
 	int n=0,a[3]; 
 	a[0]=a[1]=a[2]=0; 
 	for(;;) 
 	{ 
 	 	n=a[0]+a[1]+a[2]; 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>18&&*mx<42&&*my>18&&*my<42&&*mbutt==1) //������� 
 	 	 	break; 
 	 	if(*mx>270&&*mx<420&&*my>190&&*my<210&&*mbutt==1) //������������ 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,190,420,210); 
 	 	 	input_name(shuzu[0],272,192,12); 
 	 	 	if(shuzu[0][0]!='\0') 
 	 	 	    a[0]=1; 
 	 	}  
 	 	if(*mx>270&&*mx<420&&*my>230&&*my<250&&*mbutt==1) //������������ 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,230,420,250); 
 	 	 	input_name(shuzu[1],272,232,12); 
 	 	 	if(shuzu[1][0]!='\0') 
 	 	 	    a[1]=1; 
 	 	} 
 	 	if(*mx>270&&*mx<420&&*my>270&&*my<290&&*mbutt==1) //����������к� 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,270,420,290); 
            input_name(shuzu[2],272,272,12); 
 
            if(shuzu[2][0]!='\0') 
                a[2]=1; 
 	 	} 
 	 	if(*mx>383&&*mx<423&&*my>305&&*my<325&&*mbutt==1) //���ȷ�� 
 	 	{ 
 	 	 	if(n!=3) //n!=3, �� ʾ �� Ϣ �� д �� ȫ 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"��Ϣ����ȫ��"); 
 	 	 	else 
 	 	 	{ 
 	 	 	 	if(gl_name_yz(shuzu[0],gh)!=NULL)//��⵽�˻����Ѵ���ʱ 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"�ʻ��Ѵ��ڣ�"); 
 	 	 	 	else  
 	 	 	 	{ 
 	 	 	 	 	if(strcmp(shuzu[2],"zidonghua")!=0) //���к�ƥ��ʧ�� 
 	 	 	 	 	   outinformation(mx,my,mbutt,210,200,"���кŴ���"); 
 	 	 	 	 	else 
 	 	 	 	    { 
 	 	 	 	 	   gl_xieru(gh,shuzu); //���¹���Ա��Ϣд���ļ� 
 	 	 	 	 	   outinformation(mx,my,mbutt,210,200,"ע��ɹ���"); 
 	 	 	 	 	   break;				} 
 	 	 	 	} 
 	 	 	} 	
 	 	} 	 	         
 	} 	
 	return 1; 	
} 		
 		
void gl_xieru(guanli*h,char c[][12]) //����Աд�뺯�� 
{ 
 	FILE *fp; 
 	guanli *p,*q; 
 	p=h; 
 	if((q=(guanli *)malloc(sizeof(guanli)))==NULL) 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	strcpy(q->name,c[0]); 
 	strcpy(q->mima,c[1]); 
 	while(p->next!=NULL) 
 	{ 
 	 	p=p->next; 
 	} 
 	p->next=q; 
 
 	q->next=NULL; 
 	p=h; 
 	if( (fp=fopen("txt\\guanli.txt","wt"))==NULL ) 
    { 
     	printf("unable to open guanli.txt"); 
     	exit(1); 
    } 
    while(p!=NULL) 
    { 
 	    fprintf(fp,"%s %s\n",p->name,p->mima); 
     	p=p->next; 
    } 
    fclose(fp); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/*****************************customer.c*******************************/ void customer(int*mx,int*my,int*mbutt,yonghu*cu,yonghu*yh,shangpin*sh,liuyan*lh) 
{ // �� �� ģ �� �� �� �� 
   int n=7; 
   shangpin*th=truck_creat(cu); //���ﳵ����Ĵ��� 
   yonghu_jm();                    // �� �� �� �� �� �� 
   huanying();                     // �� ӭ �� �� �� ʾ 
   cursor(*mx,*my); 
   for(;;) 
   { 
       delay(20); 
       newxy(mx,my,mbutt); 
       if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) //����˳� 
              exit(0)�� 
       else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) //������Ϣ 
 	 	 	  n=personal_info(mx,my,mbutt,cu,yh); 
       else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) //���ﳵ 
              n=buy_truck(mx,my,mbutt,cu,yh,th);           
       else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt)//���Ѽ�¼              
              n=consume(mx,my,mbutt,th);              
       else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt)   // ���� 
              n=search(mx,my,mbutt,sh,cu,th);         
       else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt)   // ֧���� 
 	 	 	  n=zhifubao(mx,my,mbutt,cu,yh);             
       else if(*mx>=38 && *mx<87 && *my>=456 && *my<=479 && *mbutt)//���Թ���  
              n=gukeliuyan(mx,my,mbutt,cu,lh); 
       else if(*mx>=87 && *mx<125 && *my>=456 && *my<=490 && *mbutt //������� 
             n=cust_fun(0,mx,my,mbutt,cu,yh,sh,th,lh); 
       if(n==0) //����ֵ n Ϊ 0�����ص�¼���� 
       { 
          delay(100);      
          break; 
       } 
       else if( n>=1&&n<=6 ) 
          n=cust_fun(n,mx,my,mbutt,cu,yh,sh,th,lh);      
    } 
} 
int cust_fun(int n,int *mx,int *my,int *mbutt,yonghu *cu,yonghu *yh,shangpin 
*sh,shangpin *th,liuyan *lh) 
{ 
 
  switch (n) 
  { 
    case 1: 
 	      personal_info(mx,my,mbutt,cu,yh); //������Ϣ 
 	      return 1; 
    case 2: 
          buy_truck(mx,my,mbutt,cu,yh,th); //���ﳵ���� 
          return 2; 
    case 3: 
          consume(mx,my,mbutt,th); // �� �� �� ¼ 
          return 3; 
    case 4: 
 	      search(mx,my,mbutt,sh,cu,th); //������Ʒ 
          return 4; 
    case 5: 
 	      zhifubao(mx,my,mbutt,cu,yh); //֧������ֵ 
          return 5; 
    case 6: 
          gukeliuyan(mx,my,mbutt,cu,lh); //���Թ��� 
          return 6; 
    case 0: 
          return 0; 
  } 
} 
shangpin *truck_creat(yonghu *cu) //���ﳵ����Ĵ��� 
{ 
 	FILE *fp; 
    char ch; 
 	char c1[30]="txt\\"; 
 	char c2[10]=".txt";           //��������·��"txt\\yonghuname.txt" 
 	shangpin *sh,*s1,*s2;  
 	strcat(c1,cu->name); 
 	strcat(c1,c2);    
 	char str[20];                //sh ָ������ͷ�ڵ㣬s1 ָ��ǰ�ڵ㣬s2 ָ����һ�ڵ� 
 	char str1[20]; //�ļ���Ϣ���ַ�����ʽ���룬��ת���ɸ����� 
 	char str2[20]; 
 	char str3[20]; 
 	if((sh=(shangpin*)malloc(sizeof(shangpin)))==NULL) 
 	{ 
 	 	printf("No enough memory when build truck list\n"); 
 	 	exit(1); 
 	} 
  sh->next=NULL; //sh ͷָ�룬ָ����� 
 
  s1=s2=sh; if((fp=fopen(c1,"rt"))==NULL) 
{ 
 	 	printf("unable to open shangpin.txt"); 
 	 	exit(1); 
} 
  ch=fgetc(fp); 
  if(ch==EOF) //����ļ�Ϊ�գ������ٴ��ļ��ڶ������� 
  {} 
  else 
  { 
 	rewind(fp); 
    while(!feof(fp)) 
    { 
       if((s1=(shangpin*)malloc(sizeof(shangpin)))==NULL) 
 	     { 
 	 	     printf("No enough memory when build shangpin list\n"); 
 	 	     exit(1); 
 	   }   
fscanf(fp,"%d %d %d %s %d %d %s %s %s %s %s %s %s\n",&s1->life,&s1->flag, &s1->cla,s1->bianhao,&s1->kucun,&s1->xiaoshou,str,str1,str2,str3,s1->name
,s1->name_pinyin,s1->miaoshu); 
       s1->t=atol(str); 
       s1->chengben=atof(str1); 
       s1->price=atof(str2); 
       s1->profit=atof(str3); 
       s1->next=NULL; 
       s2->next=s1; 
       s2=s1; 
    } 
  } 
    fclose(fp); 
    return sh; 
} 
 
int menu_back(int *mx,int*my,int*mbutt) //�������˵����ܺ��� 
{ 
    if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) //����˳� 
      exit(0); 
    else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) //������Ϣ 
      return 1; 
    else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) //���ﳵ 
      return 2; 
    else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt) //���Ѽ�¼ 
 
      return 3; 
    else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt) //������Ʒ 
      return 4; 
    else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt) //֧���� 
      return 5; 
    else if(*mx>=38 && *mx<87 && *my>=456 && *my<=479 && *mbutt) //���� 
      return 6; 
    else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) //���� 
      return 0; 
} 
 
void yonghu_jm()  //�û������� 
{ 
   int i,y=0; 
   int a[5]; 
   cleardevice(); 
   setfillstyle(1,LIGHTBLUE); 
   bar(0,0,639,479); 
   setfillstyle(1,WHITE); 
   bar(0,0,125,640); 
   setfillstyle(1,LIGHTBLUE); 
   bar(0,456,38,480); 
   bar(87,456,125,480); 
   setcolor(LIGHTBLUE); 
   rectangle(38,456,87,479); 
   for(i=0;i<5;i++,y=y+75)   
       bar(12,y+25,112,y+69);   
   for(i=0;i<5;i++) 
       a[i]=39+75*i; 
   out_hz(27,a[0],"������Ϣ",16,17,WHITE); 
   out_hz(36,a[1],"���ﳵ",16,19,WHITE); 
   out_hz(27,a[2],"���Ѽ�¼",16,17,WHITE); 
   out_hz(27,a[3],"������Ʒ",16,17,WHITE); 
   out_hz(36,a[4],"֧����",16,19,WHITE); 
   out_hz(2,460,"�˳�",16,16,WHITE); 
   out_hz(88,460,"����",16,16,WHITE); 
   out_hz(47,460,"����",16,16,LIGHTBLUE); 
} 
void huanying() // �� ӭ �� �� 
{ 
 	int i,j; 
  setcolor(WHITE); 
  for(i=0;i<12;i++) 
  { 
 
    delay(200); 
    rectangle(130+20*i,10+19*i,630-20*i,470-18*i); 
  } 
  delay(400); 
  out_hz(356,221,"��",48,0,WHITE); 
  delay(300); 
  setfillstyle(1,LIGHTBLUE); 
  bar(350,219,410,272); 
  out_hz(356,221,"ӭ",48,0,WHITE); 
  delay(300); 
  bar(350,219,410,272); 
  out_hz(356,221,"��",48,0,WHITE); 
  delay(300); 
  bar(350,219,410,272); 
  out_hz(356,221,"��",48,0,WHITE); 
  delay(300); 
  bar(125,0,639,479); 
  int triangle1[6]={332+120,393,279+120,341,332+120,292}; 
  int triangle2[6]={238+120,384,387+120,238,238+120,238}; 
  int triangle3[6]={363+120,112,414+120,164,313+120,164}; 
  int triangle4[6]={238+120,150,238+120,297,90+120,150}; 
  int triangle5[6]={90+120,150,238+120,150,163+120,76}; 
  int xing[8]={164+120,297,164+120,193,90+120,150,90+120,224}; 
  setfillstyle(1,WHITE); 
  fillpoly(3,triangle1); 
  fillpoly(3,triangle2); 
  fillpoly(3,triangle3); 
  fillpoly(3,triangle4); 
  fillpoly(3,triangle5); 
  fillpoly(4,xing); 
  bar(313+120,164,387+120,238); 
  setcolor(BLACK); 
  line(313+120,164,414+120,164); 
  line(279+120,341,332+120,292); 
  line(90+120,150,238+120,150); 
  line(90+120,150,164+120,223); 
  line(238+120,238,238+120,297); 
  delay(400); 
  out_hz(285,160,"��",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(260,85,"��",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(238+150,250,"��",48,0,LIGHTBLUE); 
  delay(400); 
 
  out_hz(432,353,"��",24,0,LIGHTBLUE); 
  delay(400); 
  out_hz(363+110,125,"��",24,0,LIGHTBLUE); 
  delay(400); 
  out_hz(313+135,185,"��",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(210+30,210,"��",24,0,LIGHTBLUE); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/******************************customer_infor********************************/ int personal_info(int*mx,int*my,int*mbutt,yonghu*p,yonghu*h) //������Ϣģ�� 
{ 
    personal_info_jm(p); // �� �� �� Ϣ �� �� �� �� 
    delay(100); 
    for(;;) 
   { 
        newxy(mx,my,mbutt); 
 	    return xiugai(mx,my,mbutt,p,h); 
   } 
} 
int xiugai(int*mx,int*my,int*mbutt,yonghu*p,yonghu*h) //�޸��û���Ϣ�ĺ��� 
{ 
 	char str[4][12];  //��ά�������ڽ����û��������Ϣ 
 	int n=0; 
 	str[0][0]=str[1][0]=str[2][0]=str[3][0]='\0'; 
 	setfillstyle(1,WHITE); 
 	for(;;) 
 	{ 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>340&&*mx<482&&*my>193&&*my<213&&*mbutt) //�����޸Ŀ� 
 	    { 
 	 	     bar(340,195,450,211); 
 	 	     input_name(str[0],346,195,12);  
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>233&&*my<253&&*mbutt)//�ֻ����޸Ŀ� 
 	    { 	 	      
 	 	     bar(340,235,450,251); 
 	 	     input_name(str[1],346,235,12); 
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>273&&*my<293&&*mbutt)//��ַ�޸��� 
 	    { 
 	 	     bar(340,275,450,291); 
             input_name(str[2],346,275,12); 
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>313&&*my<333&&*mbutt)//֧�������޸��� 
 	    { 
 	 	     bar(340,315,450,331); 
 	 	     input_name(str[3],346,315,12); 
 	    } 
 	    else if(*mx>406&&*mx<470&&*my>385&&*my<405&&*mbutt)//���ȷ���޸�ʱ 
 	    { 
 
 	     	if(str[0][0]!='\0') //ȷ�����뱻�޸ģ����޸�д������ 
 	        { 
 	 	         strcpy(p->mima,str[0]); 
 	 	         n=1; 
 	        } 
 	        if(str[1][0]!='\0') //ȷ���ֻ��ű��޸� 
 	        { 
 	 	         strcpy(p->phone,str[1]); 
 	  	         n=1; 
 	        } 
 	        if(str[2][0]!='\0') //ȷ�ϵ�ַ���޸� 
 	        { 
 	 	         strcpy(p->address,str[2]); 
 	 	         n=1; 
 	        } 
 	        if(str[3][0]!='\0') //ȷ��֧�����뱻�޸� 
 	        { 
 	 	         strcpy(p->zhifumima,str[3]); 
 	 	         n=1; 
 	        } 
 	        if(n) //n ���㣬������Ϣ���޸�ʱ�����޸�д���ļ� 
 	        { 
                 xiugai_xieru(h); 
 	 	 	 	 outinformation(mx,my,mbutt,210,200,"�޸ĳɹ���"); 
 	        } 
 	         if(n==0) //û����Ϣ���޸�ʱ 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"������޸ķ�ʽ��"); 
 	    } 
 	    else if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )     
 	     	return menu_back(mx,my,mbutt); // ѡ�����˵�����             	                    
 	 } 
} 
 
void xiugai_xieru(yonghu*h) //�û���Ϣ�޸ĳɹ��󣬽��޸ĺ����Ϣд���ļ� 
{ 
 	FILE*fp; 
 	if((fp=fopen("txt\\yonghu.txt","wt"))==NULL) //�� yonghu.txt 
 	{ 
 	 	  printf("un able to open txt when xiugai_xieru"); 
 	 	  exit(1); 
 	} 
 	while(h!=NULL) //����������д���ļ� 
 	{ 
 	    
 
fprintf(fp,"%s %s %s %s %s %f\n",h->name,h->mima,h->zhifumima,h->phon e,h->address,h->money); 
          h=h->next; 
 	} 
 	fclose(fp); 
} 
 
void personal_info_jm(yonghu *p) //������Ϣ������� 
{ 
   int i,x=270,y=153;  
   char s[20]; 
   setfillstyle(1,LIGHTBLUE); 
   bar(125,0,640,480); 
   setfillstyle(1,WHITE); 
   bar(250,100,482,380); 
   bar(250,373,482,410); 
   setfillstyle(1,LIGHTGRAY); 
   bar(250,100,482,125); 
   for(i=0;i<6;i++) 
   { 
      setfillstyle(1,LIGHTGRAY); 
      bar(x,y,x+64,y+20); 
      setcolor(DARKGRAY); 
      rectangle(x,y,x+200,y+20); 
      y+=40; 
   } 
   y=153; 
   bar(406,385,470,405); 
   out_hz(406,387,"ȷ���޸�",16,16,DARKGRAY); 
   out_hz(334,104,"������Ϣ",16,16,DARKGRAY); 
   out_hz(270,155,"�û���",16,25,DARKGRAY); 
   out_hz(281,y+40,"����",16,26,DARKGRAY); 
   out_hz(281,y+80,"�ֻ�",16,26,DARKGRAY); 
   out_hz(281,y+120,"��ַ",16,26,DARKGRAY); 
   out_hz(270,y+160,"֧������",16,16,DARKGRAY); 
   out_hz(281,y+200,"���",16,26,DARKGRAY); 
   outtextxy(346,159,p->name); 
   outtextxy(346,199,p->mima); 
   outtextxy(346,239,p->phone); 
   outtextxy(346,279,p->address); 
   outtextxy(346,319,p->zhifumima); 
   sprintf(s,"%.2f",p->money); 
   outtextxy(346,359,s); 
   timeoutput(420,20); 
 
} 
/************************** customer_jilu.c *****************************/ int consume(int*mx,int*my,int*mbutt,shangpin*th) //���Ѽ�¼ 
{ 
 int n; //n, �� �� �� �� �� Ʒ �� �� 
   int i=1,k; //i, ҳ �� 
   shangpin *temp[30]; //ָ�����飬�洢 truck �������ѹ������Ʒ 
   n=truck_tongji(1,th,temp); //ͳ���ѹ������Ʒ�� 
   consume_jm(n,temp); //���Ѽ�¼���� 
   if(n%8==0) 
       k=n/8; 
   else 
       k=n/8+1; //k �� ҳ �� 
   jl_output(n,i,temp); 
   for(;;) 
   { 
         delay(50); 
         newxy(mx,my,mbutt); 
         if(*mx>603 && *mx<627 && *my>3 && *my<27 && *mbutt) //����Ҽ�ͷ 
         { 
             if(i<k) //ҳ��δ�������һҳ 
             { 
          	       i++; 
          	       consume_jm(n,temp); 
          	       jl_output(n-8*i+8,i,temp); //��ӡ�� i ҳ��Ʒ 
          	       cursor(*mx,*my); 
             } 
         } 
         if(*mx>540 && *mx<570 && *my>3 && *my<27 && *mbutt) //������ͷ 
         { 
      	     
 
 
if(i>1) //ҳ�벻�ڵ�һҳ 
      	    { 
      	        i--; 	 
      	 	    consume_jm(n,temp);                   
      	 	    jl_output(n-8*i+8,i,temp); //��ӡ�� i ҳ��¼ 
      	 	    cursor(*mx,*my); 
      	    } 
         } 
        if(0<= menu_back(mx,my,mbutt)&& menu_back(mx,my,mbutt)<=6)//ѡ�����˵����� 
      	     break; 
 
   } 
   return menu_back(mx,my,mbutt); 
} 
void jl_output(int n,int k,shangpin**temp) //��ӡ�� k ���Ѽ�¼ 
{ 
    int i=0,j; 
    char s[20];   
    if(n<=8) //n,�ӵ� k ҳ�����Ʒ���� 
    { 
        for(i=0;i<n;i++) //��ӡ�� k ҳ�� n ���¼ 
        { 
     	 	    j=i/2; //j �� i+1 �� �� Ʒ �� �� �� �� 
     	 	    out_hz(218+260*(i%2),42+120*j,temp[i+8*k-8]->name,16,16,BLACK); 
                sprintf(s,"%.2f",temp[i+8*k-8]->price); 
                setcolor(BLACK); 
                outtextxy(218+260*(i%2),70+120*j,s);//��ӡ�۸�   
                exchangetime(temp[i+8*k-8]->t,218+260*(i%2),94+120*j);// �� ӡʱ�� 
        } 
    } 
    else 
    { 
     	for(i=0;i<8;i++) 
        { 
     	 	    j=i/2; 
     	 	    out_hz(218+260*(i%2),42+120*j,temp[i+8*k-8]->name,16,16,BLACK); 
                sprintf(s,"%.2f",temp[i+8*k-8]->price); 
                setcolor(BLACK); 
                outtextxy(218+260*(i%2),70+120*j,s);   
                exchangetime(temp[i+8*k-8]->t,218+260*(i%2),94+120*j); 
        } 
    } 
   setcolor(WHITE); // �� �� �� ͷ 
   circle(15+550,15,12); 
   setlinestyle(0,0,3); 
   line(5+550,15,17+550,10); 
   line(5+550,15,17+550,20); 
   line(5+550,15,25+550,15); 
   setlinestyle(0,0,1); 
} 
void consume_jm(int n,shangpin**temp) //���Ѽ�¼���� 
{ 
  int i,j; 
  char s[20]; 
 
  float sum=0; //sum ��ǰ�����ܶ� 
  setfillstyle(1,LIGHTBLUE); 
  bar(125,0,639,479); 
  setcolor(WHITE); 
  for(i=0;i<4;i++) //�� 8 ����ʾ�� 
  { 
    for(j=0;j<2;j++) 
    { 
      rectangle(146+260*j,34+120*i,366+260*j,114+120*i); 
      setfillstyle(1,LIGHTGRAY); 
      bar(151-1+260*j,41+120*i,151+65+260*j,42+17+120*i); 
      bar(151-1+260*j,65+120*i,151+65+260*j,66+17+120*i); 
      bar(151-1+260*j,89+120*i,151+65+260*j,90+17+120*i); 
      setfillstyle(1,WHITE); 
      bar(151+65+260*j,41+120*i,360+260*j,42+17+120*i); 
      bar(151+65+260*j,65+120*i,360+260*j,66+17+120*i); 
      bar(151+65+260*j,89+120*i,360+260*j,90+17+120*i); 
      out_hz(151+260*j,42+120*i,"��Ʒ��",16,16,DARKGRAY); 
      out_hz(151+260*j,66+120*i,"�۸�",16,16,DARKGRAY); 
      out_hz(151+260*j,90+120*i,"����ʱ��",16,16,DARKGRAY); 
    } 
  } 
   for(i=0;i<n;i++) // �� �� n �� �� Ʒ �� �� 
        sum+=temp[i]->price; 
   sprintf(s,"%.2f",sum); //�������� sum ת�����ַ��� 
   setcolor(BLACK); 
   out_hz(130,10,"��ǰ�����Ѷ",12,12,BLACK); 
   outtextxy(220,12,s); //��ӡ�����ܶ� 
 
   setcolor(WHITE); // �� �� �� ͷ 
   circle(15+600,15,12); 
   setlinestyle(0,0,3); 
   line(25+600,15,13+600,10); 
   line(25+600,15,13+600,20); 
   line(5+600,15,25+600,15); 
   setlinestyle(0,0,1); 
} 
 
 
 
 
 
 
 
 
 
/*************************** customer_truck ******************************/ 
 
int buy_truck(int*mx,int*my,int*mbutt,yonghu*cu,yonghu*yh,shangpin*th)         
{ // �� �� �� �� �� �� 
    int n; // n �� �� �� �� �� �� Ʒ �� �� 
    shangpin *temp[20]; 
    n=truck_tongji(0,th,temp);          
    cursor(*mx,*my); 
    timeoutput(420,20); // �� �� �� ǰ ʱ �� 
    return sp_xuanze2(mx,my,mbutt,n,cu,yh,temp,th); 
} 
 
int truck_tongji(int k,shangpin*th,shangpin**temp) //���ﳵ��Ʒͳ�ƺ��� 
{ 
 	int i=0,n=0; // i ҳ �� 
 	shangpin *p=th->next; // p ָ���ﳵ�ڵ�һ����Ʒ 
    while(p!=NULL) 
    { 
     	if(p->flag==k) 
     	{ 
     	 	temp[i++]=p; 
     	 	n++; 
     	} 
     	p=p->next; 
    } 
    return n; // �� �� �� Ʒ �� �� 
} 
 
int sp_xuanze2(int *mx,int *my,int *mbutt,int n,yonghu*cu,yonghu *yh,shangpin 
**temp,shangpin *th) // �� Ʒ �� �� �� �� 
{ 
 	int i=1,k=1;  //i ��ǰ��ʾҳ��,k Ϊ��ҳ�� 
 	char str[10]; 
    if(n%4!=0) 
     	k=n/4+1; 
    else 
     	k=n/4; 
    if(n<=4) // �� ʾ �� һ ҳ �� Ʒ 
     	sp_output(n,2,temp);        
    else 
     	sp_output(4,2,temp); 
    for(;;) 
    {   
 
     	delay(30); 
     	newxy(mx,my,mbutt); 
     	if(*mx>587 && *mx<635 && *my>=451 && *my<=475 && *mbutt) //�����һҳ 
     	{ 
     	    if(i==k) 
     	     	outinformation(mx,my,mbutt,210,200,"�������һҳ��"); 
     	    if(i<k) 
     	 	    {	//��ӡ��һҳ��Ʒ 
     	 	        if((n-4*i)>=4) 
     	 	            sp_output(4,2,temp+4*i); 
     	 	        else 	     	 
     	 	     	    sp_output(n-4*i,2,temp+4*i); 
     	 	     	i++; 
     	 	    } 
     	 	     
     	} 
     	if(*mx>130 && *mx<178 && *my>=451 && *my<=475 && *mbutt) //�����һҳ 
     	{ 
     	 	if(i==1) 
     	 	 	outinformation(mx,my,mbutt,210,200,"��һҳ��"); 
     	 	else     	 	 
     	 	 	{ // �� ӡ �� һ ҳ �� Ʒ 
     	 	 	 	sp_output(4,2,temp+4*i-8); 
     	 	 	 	i--; 
     	 	 	} 
     	} 
 	    if(fukuan(mx,my,mbutt,i,n,temp,yh,cu,th)==1)//����ɹ����ع��ﳵ 
 	     	return 2; 
 	 	if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )     
 	     	 return menu_back(mx,my,mbutt); 
 	} 
} 
int fukuan(int *mx,int *my,int *mbutt,int i,int n,shangpin **temp,yonghu *yh,yonghu 
*cu,shangpin *th) 
{ 
    if(*mx>497 && *mx<597 && *my>80 && *my<100 && *mbutt)//ҳ���ϵ�һ����Ʒ���� 
 	{ 
 	 	if(4*i-3<=n) 
 	 	    if( zhuanzhang(mx,my,mbutt,i,0,cu,yh,temp,th)==1 ) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40 && *my<60 && *mbutt) 
 	{ 
        sp_cancel(i,0,n,cu,temp,&th); //ɾ��ҳ���ϵ�һ����Ʒ 
 
        return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+110 && *my<100+110 && *mbutt)//ҳ���ϵڶ�����Ʒ���� 
 	{ 
 	 	if(4*i-2<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,1,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+110 && *my<60+110 && *mbutt) 
 	{ 
         sp_cancel(i,1,n,cu,temp,&th); // ɾ �� ҳ �� �� �� �� �� �� Ʒ 
         return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+220 && *my<100+220 && *mbutt)//ҳ���ϵ�������Ʒ���� 
 	{ 
 	 	if(4*i-1<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,2,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+220 && *my<60+220 && *mbutt)//ɾ��ҳ���ϵ�������Ʒ 
 	{ 
        sp_cancel(i,2,n,cu,temp,&th); 
        return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+330 && *my<100+330 && *mbutt)//ҳ���ϵ��ļ���Ʒ���� 
 	{ 
 	 	if(4*i<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,3,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+330 && *my<60+330 && *mbutt)//ɾ��ҳ���ϵ��ļ���Ʒ 
 	{ 
        sp_cancel(i,3,n,cu,temp,&th); 
        return 1; 
 	} 
} 
void sp_cancel(int i,int j,int n,yonghu*cu,shangpin**temp,shangpin**th) 
{ // ɾ �� �� Ʒ �� �� 
 	shangpin *p; 
 
 	p=(*th)->next; // p �� �� ͷ �� �� 
 	if((4*i-3+j)==1) //���Ҫɾ������Ʒ�ǵ�һҳ�ĵ�һ����Ʒ 
 	{ 
 	 	if(temp[0]==p) //���ɾ���ľ���ͷ�ڵ�  
 	 	{ 
 	 	    (*th)->next=(*th)->next->next; 
 	 	    free(p); 
 	 	} 
 	 	else 
 	 	{ 
 	 	 	while(p->next!=temp[0]) //���������ҵ���ɾ����Ʒ����һ���ڵ� 	 p=p->next; 
 	 	    p->next=temp[0]->next; //��������ɾ���ڵ� 
 	 	    free(temp[0]); 
 	 	} 
 	} 
 	else 
 	{ 
 	 	while(p->next!=temp[4*i+j-4]) //���������ҵ���ɾ����Ʒ����һ���ڵ� 
 	 	 	p=p->next; 
 	 	p->next=temp[4*i+j-4]->next; 
 	 	free(temp[4*i+j-4]); 
 	} 
    xieru_yonghu(cu,*th); //���޸�д���û�ר�� txt �ļ� 
} 
 int zhuanzhang(int *mx,int *my,int *mbutt,int i,int j,yonghu *cu,yonghu 
*yh,shangpin **temp,shangpin *th) // ת �� �� �� 
{ 
 	int x=zhifu_tanchu(mx,my,mbutt,cu); //֧��������֤ͨ�� 
 	if(x==1) 
 	{ 
        if(cu->money<temp[4*i+j-4]->price) //�û����С����Ʒ�۸�ʱ 
        { 
         	outinformation(mx,my,mbutt,210,200,"�˻����㣡"); 
         	return 0; 
        } 
        else // �� �� �� �� �� �� �� �� �� Ʒ ��            
        { 
         	cu->money-=temp[4*i+j-4]->price; 
         	temp[4*i+j-4]->flag=1; //�������� 1 
         	temp[4*i+j-4]->t=time(NULL)-5*3600; //����ϵͳʱ�� 
         	xieru_yonghu(cu,th); //��������Ϣд���û����ļ� 
         	xiugai_xieru(yh); //���û������ٴ�д�� yonghu.txt 
         	xieru_dingdan(cu,temp[4*i+j-4]);//���û����µĶ���д�� dingdan.txt 
 
         	return 1; 
        } 
 	} // ת �� �� �� �� �� ֵ �� �� 1�� �� �� �� �� �� �� 0 
 	else 
 	 	return 0; 
} 
void xieru_dingdan(yonghu*cu,shangpin*p) //�û��Ķ���д�� dingdan.txt 
{ 
    FILE*fp; 
    if((fp=fopen("txt\\dingdan.txt","at"))==NULL) 
    { 
     	printf("No enough memory!"); 
 	 	exit(1); 
    } 
    fprintf(fp, "\n%d %s %s %s %s",1,p->bianhao,cu->name,cu->phone,cu->address); 
    fclose(fp); 
} 
void xieru_yonghu(yonghu*cu,shangpin*th) //�޸����û����������ı��ļ��ڵ���Ϣ 
{ 
 	FILE*fp; 
 	char c1[30]="txt\\"; 
 	char c2[10]=".txt"; 
 	char str1[20],str2[20],str3[20]; 
 	strcat(c1,cu->name); 
 	strcat(c1,c2);   
 	if((fp=fopen(c1,"wt"))==NULL) 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	th=th->next; 
 	rewind(fp); 
 	while(th!=NULL) 
 	{ 
 	 	sprintf(str1,"%f",th->chengben); 
 	 	sprintf(str2,"%f",th->price); 
 	 	sprintf(str3,"%f",th->profit); 
 	 	 
fprintf(fp,"%d %d %d %s %d %d %ld %s %s %s %s %s %s\n",th->life,th->fla g,th->cla,th->bianhao,th->kucun,th->xiaoshou,th->t,str1,str2,str3,th->n ame,th->name_pinyin,th->miaoshu); 
 	 	th=th->next; 
 	} 
    fclose(fp); 
 
} 
int zhifu_tanchu(int*mx,int*my,int*mbutt,yonghu*cu) //ȷ�Ϲ���ʱ֧�������� 
{ 
 	void *buffer; 
 	unsigned size; 
 	char mima[20]; 
 	size=imagesize(210,190,430,290); 	 
 	if(((void *)buffer=malloc(size))==NULL) 
 	{ 
 	 	printf("not enough buffer"); 
 	 	exit(1); 
 	} 
 	getimage(210,190,430,290,buffer);//ȡ��Ļ��ͼ�洢�� buffer �� 
    setfillstyle(1,LIGHTGRAY); //�������� 
    bar(210,190,430,290); 
    setfillstyle(1,BROWN); 
    bar(210,190,430,210); 
    bar(230,260,270,280); 
    bar(370,260,410,280); 
    setfillstyle(1,WHITE); 
    bar(260,230,390,250); 
 
 
 	out_hz(280,192,"֧��������",16,16,BLACK); 
out_hz(234,262,"ȷ��",16,16,BLACK); 
out_hz(374,262,"ȡ��",16,16,BLACK);          	
 	
 	
 	
 
 	while(1) 				
 	{ 				
 	  newxy(mx,my,mbutt); 				
 	  if(*mx>300 && *mx<400 && *my>230 && *my<250 && *mbutt)//������������ 
 	  { 
 	      bar(260,230,390,250); 
 	 	  input_mima(mima,265,232); 
 	  } 
 	  if(*mx>230 && *mx<270 && *my>260 && *my<280 && *mbutt)//���ȷ�� 
 	  { 
 	      if(strcmp(cu->zhifumima,mima)==0) //����ͨ��ʱ 
 	      { 
             putimage(210,190,buffer,0); 
             free(buffer);  
             outinformation(mx,my,mbutt,210,200,"������֤ͨ������"); 
 	         return 1; 
 	      } 
 	      else 
 	        outinformation(mx,my,mbutt,210,250,"�������"); 
 	  } 
 
 	  if(*mx>370 && *mx<410 && *my>260 && *my<280 && *mbutt)//���ȡ�� 
 	  { 
 	 	  putimage(210,190,buffer,0); 
 	 	  cursor(*mx,*my); 
          free(buffer);  
          return 0; 
 	  } 
 	} 	                   
 	delay(100); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/************************** customer_search.c ********************************/ 
 
int search(int *mx,int *my,int *mbutt,shangpin *sh,yonghu *cu,shangpin *th) 
{ 
   
   char mubiao[30]; 
   char price1[10]; 
   char price2[10]; 
   shangpin *temp[20]; 
   int n; //n Ϊ��Ӧ������ʽ�ѵ�����Ʒ���� 
   price1[0]=price2[0]='\0'; 
   search_jm(); 
   for(;;) 
   {   
       delay(20); 
       newxy(mx,my,mbutt); 
       if(*mx>278 && *mx<566 && *my>=375 && *my<=404 && *mbutt) //��Ʒ������ 
      	    input_name(mubiao,280,381,12); 
 	   if(*mx>278 && *mx<338 && *my>=426 && *my<=455 && *mbutt) //��ߵļ۸� 
 	  	    input_name(price1,280,433,12); 
 	   if(*mx>400 && *mx<460 && *my>=426 && *my<=455 && *mbutt) //�ұߵļ۸� 
 	  	    input_name(price2,402,433,12); 
 	   if(*mx>547 && *mx<585 && *my>=427 && *my<=449 && *mbutt) //���ȷ��ʱ 
 	   { 
            n=searching(mubiao,price1,price2,sh,temp); 
            sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=183 && *mx<320 && *my>=104 && *my<=193 && *mbutt==1)//�ѵ��Ӳ�Ʒ 
 	   { 
 	 	    n=sp_cla_search(1,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=437 && *mx<574 && *my>=104 && *my<=193 && *mbutt==1) //�����鼮 
 	   { 
 	 	    n=sp_cla_search(2,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=183 && *mx<320 && *my>=275 && *my<=364 && *mbutt==1) //�������� 
 	   { 
 	 	    n=sp_cla_search(3,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=437 && *mx<574 && *my>=275 && *my<=364 && *mbutt==1) //����ʳƷ 
 
 	   { 
 	 	    n=sp_cla_search(4,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 	 	 	 	 	 	 
 	   if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )//���˵�����     
 	     	 break; 
   } 
   return menu_back(mx,my,mbutt); 
} 
 
void search_jm() //�������ܽ������ 
{ 
   setfillstyle(1,LIGHTBLUE); 
   bar(125,0,639,479); 
   sp_cla_jm();    
   setfillstyle(1,WHITE); // �� �� �� �� �� 
   bar(278,375,566,404); 
   bar(278,426,338,455); 
   bar(400,426,460,455); 
   setcolor(WHITE); 
   rectangle(547,427,585,449); 
   out_hz(550,430,"ȷ��",16,16,WHITE); 
   out_hz(209,381,"��ͨ����",16,16,WHITE); 
   out_hz(209,432,"�۸��޶�",16,16,WHITE); 
   timeoutput(420,20); 
} 
void sp_cla_jm() // �� Ʒ �� �� �� �� 
{ 
   setfillstyle(1,LIGHTBLUE); 
   bar(125,0,639,479); 
   out_hz(349,44,"��Ʒ����",16,16,WHITE); 
   setfillstyle(1,WHITE); //�Ŀ���Ʒ�� 
   bar(183,104,320,193); 
   bar(183,275,320,364); 
   bar(437,104,574,193); 
   bar(437,275,574,364); 
   setcolor(LIGHTBLUE); 
   setfillstyle(1,LIGHTBLUE); //�����Ӳ�Ʒ����  
   bar(261,126,291,131); 
   bar(261,165,291,175); 
   rectangle(261,121,291,131); 
   rectangle(222,111,232,141); 
   rectangle(212,121,242,131); 
   circle(276,158,4); 
 
   circle(276,182,4); 
   bar(222,111,232,141); 
   line(233,184,240,177); 
   line(240,177,219,156); 
   line(219,156,212,163); 
   line(212,163,233,184); 
   floodfill(226,170,LIGHTBLUE); 
   line(233,156,212,177); 
   line(212,177,219,184); 
   line(219,184,240,163); 
   line(240,163,233,156); 
   floodfill(226,170,LIGHTBLUE); 
   line(467,119,467,178); // �� �� �� 
   line(467,178,505,188); 
   line(505,188,505,109); 
   line(505,109,467,119); 
   rectangle(505,115,540,182); 
   bar(514,120,536,125); 
   bar(514,132,536,137); 
   bar(514,145,536,150); 
   bar(514,157,536,162); 
   bar(514,169,536,174); 
   moveto(251,302);//������ 
   lineto(272,302); 
   lineto(292,324); 
   lineto(280,332); 
   lineto(270,321); 
   lineto(270,358); 
   lineto(232,358); 
   lineto(232,321); 
   lineto(222,332); 
   lineto(210,324); 
   lineto(230,302); 
   lineto(251,302); 
   circle(502,323,32); 
   bar(232,327,270,329); 
   bar(232,342,270,344); 
   bar(232,355,270,356); 
   setfillstyle(1,WHITE); // �� �� 
   bar(470,345,534,355); 
   bar(470,291,534,323); 
   line(470,323,534,323); 
   line(475,319,483,303); 
   line(488,319,496,303); 
 
   line(501,319,509,303); 
   line(514,319,522,303); 
   line(527,319,535,303); 
   setfillstyle(1,LIGHTBLUE); 
   bar(478,346,526,352); 
   bar(470,321,534,323); 
   out_hz(215,87,"���Ӳ�Ʒ",16,16,WHITE); 
   out_hz(489,87,"ͼ��",16,16,WHITE); 
   out_hz(235,258,"����",16,16,WHITE); 
   out_hz(489,258,"ʳƷ",16,16,WHITE); 
} 
 
int searching(char*mubiao,char*p1,char*p2,shangpin*sh,shangpin**temp)  //�����ж��ٸ�������������Ʒ 
{ 
 	shangpin *s=sh; 
 	int i=0,n=0,t=0; 
 	float f1,f2; 
 	if(p1[0]=='\0'&&p2[0]=='\0') //û���޶��۸����� 
 	{ 
 	    while(s!=NULL) 
 	    { 
           if(strstr(s->name_pinyin,mubiao)!=NULL&&s->life==1) 
 	 	   { 
 	 	         temp[i++]=s; //���ҵ�����Ʒ�浽ָ�������� 
 	 	 	     n++;                   
           } 	 
 	 	   s=s->next; 
 	     } 
 	} 
    else if(p1[0]!='\0'&&p2[0]=='\0') //ֻ�޶��۸�����ʱ 
    { 
     	f1=atof(p1); 
     	while(s!=NULL) 
 	    { 
           if((strstr(s->name_pinyin,mubiao)!=NULL)&&s->price>f1&&s->life==1 ) 
 	 	   { 
 	 	          temp[i++]=s; 
 	 	 	      n++;         
 	 	    }    
 	 	   s=s->next; 
 	    } 
} 
 	else if (p1[0]=='\0'&&p2[0]!='\0') // ֻ �� �� �� �� �� �� �� ʱ 
 
 		{ 	
 		 	f2=atof(p2); 
 		 	while(s!=NULL) 
 		 	{      
 	 	 	    if((strstr(s->name_pinyin,mubiao)!=NULL)&&s->price<f2&&s->life==1 ) 
 		 	 	{ 
 		 	 	     	temp[i++]=s; 
 		 	 	        n++; 
 		 	 	 } 
 		 	 	 s=s->next; 
 		 	} 
 		} 	
 	else if( p1[0]!='\0'&& p2[0]!='\0' ) //�۸������޶���ʱ 
 	{ 
 	 	f1=atof(p1); 
 	 	f2=atof(p2); 
     	while(s!=NULL) 
 	    { 
 	if( (strstr(s->name_pinyin,mubiao)!=NULL) && s->price>f1 && s->price<f2 && s->life==1 ) 
 	 	       { 
 	 	          temp[i++]=s; 
 	 	 	        n++;             
 	 	       } 
 	 	       s=s->next; 
 	 	} 
 	} 
 	return n; // �� �� �� �� �� �� �� �� Ʒ �� �� n  
} 
 	 
int sp_cla_search(int n,shangpin*sh,shangpin **temp) //��Ʒ�ķ������� 
{ 
 	int i=0,k=0; 
 	shangpin *s1=sh; 
 	while(s1!=NULL) 
 	{ 
 	 	if(s1->cla==n&&s1->life==1) 
 	 	{ 
 	 	 	temp[i++]=s1; 
 	 	 	k++; 
 	 	} 
 	 	s1=s1->next; 
 	} 
 	return k; //���ظ������Ʒ���� 
 
}    
 
void sp_huizhi(int flag,int j,shangpin*sp) //��ҳ��ָ��λ�û�����Ʒ 
{ 
 	  char s[20]; 
 	  setfillstyle(1,WHITE); 
 	  setcolor(WHITE); 
      rectangle(171,40+j*110,597,100+j*110); 
      bar(497,40+j*110,597,60+j*110); 
      bar(497,80+j*110,597,100+j*110); 
      if(flag==1) //flag=1�������������������Ʒ 
      { 
         out_hz(499,42+j*110,"�鿴��ϸ��Ϣ",16,16,LIGHTBLUE); 
         out_hz(507,82+j*110,"���빺�ﳵ",16,16,LIGHTBLUE); 
      } 
      if(flag==2) //flag=2�� �� �� �� �� �� �� �� �� Ʒ 
      { 
         out_hz(531,42+j*110,"ɾ��",16,16,LIGHTBLUE); 
         out_hz(515,82+j*110,"ȷ�Ϲ���",16,16,LIGHTBLUE); 
      } 
      out_hz(173,42+j*110,"���",16,16,WHITE); 
      out_hz(380,42+j*110,"�ۼ�",16,16,WHITE); 
      out_hz(173,62+j*110,"����",16,16,WHITE); 
      out_hz(380,62+j*110,"���",16,16,WHITE); 
      out_hz(173,82+j*110,"����",16,16,WHITE); 
      outtextxy(210,46+j*110,sp->bianhao); 
      sprintf(s,"%.2f",sp->price); 
      outtextxy(420,46+j*110,s); 
      out_hz(210,62+j*110,sp->name,16,16,WHITE); 
 	  sprintf(s,"%d",sp->kucun); 
      outtextxy(420,66+j*110,s); 
      out_hz(210,82+j*110,sp->miaoshu,16,16,WHITE); 
} 
 
void sp_xuanze1(int *mx,int *my,int *mbutt,int n,yonghu *cu,shangpin 
**temp,shangpin *th) 
{ 
 	int i=1,k=1; //i ����ǰ��ʾҳ��,k Ϊ�ܹ���ҳ�� 
 	char str[10]; 
    if(n%4!=0) 
     	k=n/4+1; 
    else 
     	k=n/4; 
    if(n<=4) 
 
     	sp_output(n,1,temp); 
    else 
     	sp_output(4,1,temp); //��ӡ��һҳ����Ʒ����  
 
    out_hz(145,15,"���ҵ�",12,12,WHITE); 
 	out_hz(200,15,"���������",12,12,WHITE); 
 	setcolor(WHITE); 
 	sprintf(str,"%d",n); 
 	outtextxy(186,17,str); 
    cursor(*mx,*my); // �� �� " �� �� �� n �� �� Ʒ " �� �� �� 
 
    for(;;) // �� �� Ʒ �� �� �� �� 
    { 
     	newxy(mx,my,mbutt); 
     	if(*mx>587 && *mx<635 && *my>=451 && *my<=475 && *mbutt) //�����һҳ 
     	{ 
     	    if(i==k) 
     	     	outinformation(mx,my,mbutt,210,200,"�������һҳ��"); 
     	    if(i<k) 
     	 	    { 	 
     	 	        if((n-4*i)>=4) 
     	 	          sp_output(4,1,temp+4*i); 
     	 	        else 	     	 
     	 	     	  sp_output(n-4*i,1,temp+4*i); 
     	 	     	i++; 
     	 	    } 
     	 	     
     	} 
     	if(*mx>130 && *mx<178 && *my>=451 && *my<=475 && *mbutt)//�����һҳ 
     	{ 
     	 	if(i==1) 
     	 	 	outinformation(mx,my,mbutt,210,200,"��һҳ��"); 
     	 	else     	 	 
     	 	 	{ 
     	 	 	 	sp_output(4,1,temp+4*i-8); 
     	 	 	 	i--; 
     	 	 	} 
     	} 
 	    goumai(mx,my,mbutt,i,n,temp,cu,th); // �� �� �� �� �� �� �� �� 
 	 	sp_xinxi(mx,my,mbutt,i,n,temp); //�鿴��Ʒ��ϸ��Ϣ�Ĳ��� 
 	 	if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=5 )     
 	     	 break; 
    } 
} 
 
 
void sp_output(int n,int flag,shangpin **temp) //����ĳһҳ�ϵ���Ʒ 
{ 
 	int i; 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,639,479);  	 
 	out_hz(587,451,"��ҳ",24,0,WHITE); 
    out_hz(130,451,"��ҳ",24,0,WHITE); 
 	for(i=0;i<n;i++) 	 	 
 	sp_huizhi(flag,i,temp[i]);   	 
} 
 
void goumai(int *mx,int *my,int *mbutt,int i,int n,shangpin **temp,yonghu 
*cu,shangpin *th) //i �� ʾ ҳ �� 
{ 
 	if(*mx>497 && *mx<597 && *my>80 && *my<100 && *mbutt) 
 	{ 
        if(4*i-3<=n) // �� ҳ �� �� �� һ �� �� Ʒ �� �� �� �� �� 
        { 
            to_gouwuche(cu,temp[4*i-4],th); 
            outinformation(mx,my,mbutt,210,200,"�ɹ����빺�ﳵ��"); 
        } 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+110 && *my<100+110 && *mbutt) 
 	{ 
 	 	if(4*i-2<=n) // �� ҳ �� �� �� �� �� �� Ʒ �� �� �� �� �� 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-3],th); 
 	 	    outinformation(mx,my,mbutt,210,200,"�ɹ����빺�ﳵ��"); 
 	 	} 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+220 && *my<100+220 && *mbutt) 
 	{ 
 	 	if(4*i-1<=n) // �� ҳ �� �� �� �� �� �� Ʒ �� �� �� �� �� 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-2],th); 
 	 	    outinformation(mx,my,mbutt,210,200,"�ɹ����빺�ﳵ��"); 
 	 	} 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+330 && *my<100+330 && *mbutt) 
 	{ 
 	 	if(4*i<=n) // �� ҳ �� �� �� �� �� �� Ʒ �� �� �� �� �� 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-1],th); 
 
 	 	    outinformation(mx,my,mbutt,210,200,"�ɹ����빺�ﳵ��"); 
 	    } 
 	} 
} 
void sp_xinxi(int*mx,int*my,int*mbutt,int i,int n,shangpin**temp)//�鿴��Ʒ��Ϣ 
{ 
    if(*mx>497 && *mx<597 && *my>40 && *my<60 && *mbutt) 
    { // �� �� ҳ �� �� �� һ �� �� Ʒ �� �� ϸ �� Ϣ 
        show_bmp(i,0,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+110 && *my<60+110 && *mbutt) 
    { // �� �� ҳ �� �� �� �� �� �� Ʒ �� �� ϸ �� Ϣ 
        show_bmp(i,1,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+220 && *my<60+220 && *mbutt) 
    { // �� �� ҳ �� �� �� �� �� �� Ʒ �� �� ϸ �� Ϣ 
        show_bmp(i,2,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+330 && *my<60+330 && *mbutt) 
    { // �� �� ҳ �� �� �� �� �� �� Ʒ �� �� ϸ �� Ϣ 
        show_bmp(i,3,n,temp);   
        cursor(*mx,*my); 
    } 
} 
 
void show_bmp(int i,int j,int n,shangpin**temp) // �� ʾ �� Ʒ �� ͼ Ƭ 
{ 
  char s1[20]="bmp\\"; 
  char s2[20]=".bmp"; 
  strcat(s1,temp[4*i+j-4]->bianhao); 
  strcat(s1,s2); 
  Read_BMP(s1); 
  yonghu_jm(); // �� �� �� �� Ʒ �� Ϣ �� �� ԭ �� �� 
  if(n-4*i+4<=4) 
      sp_output(n-4*i+4,1,temp+4*i-4); 
  else 
      sp_output(4,1,temp+4*i-4); 
} 
void to_gouwuche(yonghu*cu,shangpin*temp,shangpin*th) //���빺�ﳵ 
{ 
 	FILE*fp; 
 
 	char str1[30]="txt\\"; 
 	char str2[10]=".txt"; 
 	shangpin*p=th,*q; 
 	strcat(str1,cu->name); 
 	strcat(str1,str2);   
    if ( (q=(shangpin*)malloc(sizeof(shangpin)) )==NULL ) 
    { 
     	printf("No enough memory!"); 
     	exit(1); 
    } 
    if ((fp=fopen(str1,"wt"))==NULL) 
 	{ 
 	 	printf("cannot open yonghu_name txt"); 
 	 	exit(1); 
 	} 
    while(p->next!=NULL) 
     	p=p->next; 
    p->next=q; 
    *q=*temp; // �� �� Ӧ �� Ʒ �� Ϣ �� �� �� q 
    q->next=NULL; 
    p=th->next; 
    while(p!=NULL) 
    { 
        fprintf(fp,"%d %d %d %s %d %d %ld %f %f %f %s %s %s\n",p->life,p->flag, p->cla,p->bianhao,p->kucun,p->xiaoshou,p->t,p->chengben,p->price,p->pro fit,p->name,p->name_pinyin,p->miaoshu); 
        p=p->next; 
    } 
 	fclose(fp); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************customer_zhifubao.c*****************************/ int zhifubao(int*mx,int*my,int*mbutt,yonghu*p,yonghu*yh) // ֧ �� �� �� �� 
{ 
 	char nu[12];               // �� �� �� �� �� 
 	char mima[12];             // �� �� �� �� �� 
 	char money[10]; // �� ֵ �� �� 
 	bank *bh,*q; //bh �� �� �� �� �� ͷ ָ �� 
    zhifubao_jm(p); // ֧ �� �� �� �� �� �� 
    delay(100); 
    for(;;) 
    { 
      newxy(mx,my,mbutt); 
      if(*mx>346 && *mx<488 && *my>=195 && *my<=215 && *mbutt)  //�������п� 
      { 
         bar(346,195,488,215); 
         input_name(nu,348,197,12); 
      } 
      if(*mx>346 && *mx<488 && *my>=225 && *my<=245 && *mbutt) //�������� 
      { 
         bar(346,225,488,245); 
         input_mima(mima,348,227); 
      } 
      if(*mx>346 && *mx<488 && *my>=255 && *my<=275 && *mbutt) //������ 
      {  
         bar(346,255,488,275); 
         input_name(money,348,257,12); 
      } 
      if(*mx>436 && *mx<478 && *my>=305 && *my<=331 && *mbutt) //ȷ�ϳ�ֵ 
      { 
        if(nu[0]=='\0') 
 	 	 	outinformation(mx,my,mbutt,210,200,"���Ų���Ϊ�գ�"); 
        else if(money[0]=='\0')       	 
 	 	 	outinformation(mx,my,mbutt,210,200,"�������");       	 
        else  
        { 
 	 	 	bh=bank_creat(); // �� �� �� �� �� �� �� 
 	 	 	if((q=bank_yz(bh,nu,mima))==NULL) 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"��ֵʧ�ܣ�"); 
            else 
            { 
                if(chongzhi(p,q,atof(money))==0) 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"���п����㣡"); 
                else 
 
                {    
                    outinformation(mx,my,mbutt,210,200,"��ֵ�ɹ���");        
                 	xiugai_xieru(yh);    //�û�������Ϣд�� yonghu.txt 
 	 	 	 	 	bank_xieru(bh);      //���п�������Ϣд�� bank.txt 
 	 	 	 	 	bank_clear(bh);	//������п����� 
                 	zhifubao_jm(p);      //����֧�������� 
                 	cursor(*mx,*my); 
                 	money[0]='\0'; 
                 	nu[0]='\0'; 
                 	mima[0]='\0'; 
                } 
            } 
        } 
      } 
     if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 ) //�������˵�    
 	     	 break; 
    } 
    return menu_back(mx,my,mbutt); 
} 
 
int chongzhi(yonghu*p,bank*q,float x) //p �� ǰ �� �� ��q �� ǰ �� �� �� 
{ 
    if(x>q->money) 
     	return 0; // �� �� �� �� 
    else 
    { 
     	q->money-=x; 
     	p->money+=x; 
     	return 1; // �� ֵ �� �� 
    } 
} 
bank* bank_yz(bank*h,char*name,char *mima) //��֤���п����������Ƿ���ȷ 
{ 
 	while(h!=NULL) 
 	{ 
 	 	if(strcmp(name,h->count)==0) 
 	 	{ 
            if(strcmp(mima,h->mima)==0) 
            { 
             	return h; 
             	break; 
            } 
            else 
            { 
 
                return NULL;     
            }       
 	 	} 
 	 	else 
 	 	{ 
 	 	 	h=h->next; 
 	 	} 
 	} 
 	if(h==NULL) 
 	 	return NULL; //��֤�ɹ��������п�ָ�룬���򷵻� NULL 
} 
void bank_xieru(bank*h) //��ֵ�ɹ�����Ӧ���п�Ҫ�ۿ� 
{ 
 	FILE*fp; 
 	if((fp=fopen("txt\\bank.txt","wt"))==NULL) 
 	{ 
 	 	printf("un able to open txt when bank_xieru"); 
 	 	exit(1); 
 	} 
 	while(h!=NULL) 
 	{ 
          fprintf(fp,"%s %s %f\n",h->count,h->mima,h->money); 
          h=h->next; 
 	} 
 	fclose(fp); 
} 
bank *bank_creat() // �� �� �� �� �� �� �� �� 
{ 
 	FILE *fp; 
 	bank *h,*q1,*q2; 
 	char s[15]; 
 	if((h=(bank*)malloc(sizeof(bank)))==NULL) 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	q1=q2=h; 
 	if((fp=fopen("txt\\bank.txt","rt"))==NULL) 
 	{ 
 	 	printf("unable to open bank.txt!"); 
 	 	exit(1); 
 	} 
 	rewind(fp); 
 	fscanf(fp,"%s %s %s\n",h->count,h->mima,s); 
 
 	h->money=atof(s); 
 	h->next=NULL; 
 	while(!feof(fp)) 
 	{ 
 	 	if((q1=(bank*)malloc(sizeof(bank)))==NULL) 
 	    { 
 	 	   printf("No enough memory!"); 
 	 	   exit(1); 
 	    } 
 	    fscanf(fp,"%s %s %s\n",q1->count,q1->mima,s); 
 	    q1->money=atof(s); 
 	    q1->next=NULL; 
 	    q2->next=q1; 
 	    q2=q1; 
 	} 
 	fclose(fp); 
 	return h; 
} 
void bank_clear(bank*bh) //������������ 
{ 
 	bank *p=bh; 
    while(bh!=NULL) 
    { 
     	p=bh->next; 
     	free(bh); 
     	bh=p; 
    } 
} 
 
void zhifubao_jm(yonghu*p) // ֧ �� �� �� �� �� �� �� �� 
{ 
  char s[20]; 
  setfillstyle(1,LIGHTBLUE); 
  bar(125,0,639,479); 
  setcolor(WHITE); 
  rectangle(250,91,509,352); 
  line(250,133,509,133); 
  setfillstyle(1,WHITE); 
  bar(346,195,488,215); 
  bar(346,225,488,245); 
  bar(346,255,488,275); 
  bar(436,305,478,331);//ȷ�ϰ�ť 
  timeoutput(420,20);  
  out_hz(265,105,"���",16,16,WHITE); 
 
  out_hz(265,148,"��ֵ",16,16,WHITE); 
  out_hz(298,197,"���п�",16,16,WHITE); 
  out_hz(306,227,"����",16,16,WHITE); 
  out_hz(272,257,"������",16,16,WHITE); 
  out_hz(441,310,"ȷ��",16,16,LIGHTBLUE); 
  sprintf(s,"%f",p->money); 
  outtextxy(300,109,s); 
  delay(30); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/***************************customer_liuyan.c******************************/ int gukeliuyan(int*mx,int*my,int*mbutt,yonghu*cu,liuyan*lh) 
{ 
    int i; 
    message_jm(); 
    delay(100); 
    for(;;) 
    { 
        newxy(mx,my,mbutt); 
        if(*mx>=200 && *mx<306 && *my>=223 && *my<=257 && *mbutt) //δ������ 
        { 
            liuyan *lp1[20]; 
            for(i=7;i==7;) 
                i=weidu(mx,my,mbutt,1,1,1,lh,lp1,cu->name); 
            return i; 
        } 
        if(*mx>=459 && *mx<565 && *my>=223 && *my<=257 && *mbutt) //�Ѷ����� 
        { 
            liuyan *lp2[20]; 
            for(i=7;i==7;) 
                i=weidu(mx,my,mbutt,1,1,0,lh,lp2,cu->name); 
            return i; 
        } 
         if(*mx>=330 && *mx<436 && *my>=123 && *my<=157 && *mbutt)  //�ѷ���Ϣ  
         { 
             liuyan *lp3[20]; 
             for(i=7;i==7;) 
                 i=weidu(mx,my,mbutt,1,0,1,lh,lp3,cu->name); 
             return i; 
         } 
         if(*mx>=330 && *mx<436 && *my>=323 && *my<=357&&*mbutt) //��д���� 
         { 
             for(i=7;i==7;) 
             i=bianxieliuyan(mx,my,mbutt,1,lh,cu->name); 
             return i; 
         } 
         if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )     
                 return menu_back(mx,my,mbutt); 
    } 
} 
/****************************** guanli.c ******************************/ 
int manager(int*mx,int*my,int*mbutt,liuyan*lh,shangpin*sh,dingdan*dh) 
{ 
 
 	manager_jm(); // �� �� Ա �� �� �� 
 	cursor(*mx,*my); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt)//�˳� 
 	 	 	exit(0); 
 	 	if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt)//���� 
 	 	 	return 0; 
 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt) 
 	 	{ 
 	 	 	if(mana_fun(good_mana(mx,my,mbutt,sh),mx,my,mbutt,lh,sh,dh)==0) 
 	 	 	 	return 0; 
 	 	} 
 	 	if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
 	{	 if(mana_fun(order_mana(mx,my,mbutt,sh,dh),mx,my,mbutt,lh,sh,dh)==0) 
 	 	 	 	return 0; 
 	 	} 
 	 	else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && 
*mbutt) 
 	 	{ 
 	 	 	if(mana_fun(message(mx,my,mbutt,lh),mx,my,mbutt,lh,sh,dh)==0) 
 	 	 	 	return 0; 
 	 	} 
 	} 
} 
int mana_fun(int i,int *mx,int *my,int *mbutt,liuyan *lh,shangpin *sh,dingdan*dh) 
{ // �� �� �� ͬ �� �� �� �� �� �� �� ֵ �� �� �� һ �� �� 
 	for(;;) 
 	{ 
 	 	switch (i) 
 	 	{ 
 	 	  case 1: 
 	 	 	i=good_mana(mx,my,mbutt,sh); // �� Ʒ �� �� 
 	 	 	break; 
 	 	case 2: 
 	 	 	i=order_mana(mx,my,mbutt,sh,dh); // �� �� �� �� 
 	 	 	break; 
 	 	case 3: 
 	 	 	i=message(mx,my,mbutt,lh); // �� �� �� �� 
 	 	 	break; 
 	 	default: 
 	 	 	return 0; 
 
 	 	} 
    } 
} 
void manager_jm() // �� �� Ա �� �� �� 
{ 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(0,0,640,480); 
 	setcolor(WHITE); 
 	setfillstyle(1,WHITE); 
 	bar(0,0,125,480); 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(12,87,112,87+44); 
 	bar(12,87*2+44,112,87*2+44*2); 
 	bar(12,87*3+44*2,112,87*3+44*3); 
 	out_hz(27,87+14,"��Ʒ����",16,16,WHITE); 
 	out_hz(27,87*2+58,"��������",16,16,WHITE); 
 	out_hz(27,87*3+44*2+14,"�鿴����",16,16,WHITE); 
 	bar(0,456,38,480); 
 	bar(87,456,125,480); 
 	out_hz(2,460,"�˳�",16,16,WHITE); 
 	out_hz(88,460,"����",16,16,WHITE); 
} 
 
void kucunbuzu(int*mx,int*my,int*mbutt,int t) //��治������Ʒͣ������ 
{ 
 	char str[12]; 
 	str[0]='/0'; 
 	void *buffer; 
    unsigned size; 
    size=imagesize(170,120,470,340); 
    if(((void*)buffer=malloc(size))==NULL) 
    { 
 	 	printf("No enough memory"); 
 	 	exit(1); 
    } 
    if(*mx>170&&*mx<470&&*my>120&&*my<340) 
 	 	cursor(*mx,*my); 
    getimage(170,120,470,340,buffer); 
    setfillstyle(1,WHITE); 
    bar(170,140,470,340); 
    setfillstyle(1,DARKGRAY); 
    bar(170,120,470,140); 
    setfillstyle(1,LIGHTGRAY); 
    bar(300,310,340,330); 
 
    if(t==1) 
 	 	out_hz(272,230,"��治��",24,0,BLACK); 
 	if(t==2) 
 	 	out_hz(260,230,"��Ʒ��ͣ��",24,0,BLACK); 
    out_hz(304,312,"ȷ��",16,16,BLACK); 
    for(;;) 
    { 
        newxy(mx,my,mbutt); 
 
 	 
 	if(*mx>300&&*mx<340&&*my>310&&*my<330&&*mbutt==1) //���ȷ�� 
{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
    } 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************** manager_spgl.c *****************************/ int good_mana(int*mx,int*my,int*mbutt,shangpin*sh) //��Ʒ���������ܺ��� 
{ 
  int i=0,k=0,l,f=0; 
  shangpin*q,*p,*sp[20]; 
  p=sh; 
  while(p!=NULL) 
  {  	 
    if(p->life==1)      
       sp[k++]=p;    
    p=p->next; 
  } 
  l=(k-1)/5; //l �� ҳ �� 
  good_jm(sh,&i); // �� Ʒ �� �� �� �� 
  if(*mx>125&&*mx<=640) 
    cursor(*mx,*my); 
  delay(100); 
  for(;;) 
  { 
 	  newxy(mx,my,mbutt); 
 	  if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//���˵����� 
 	     return 1; 
 	  else if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
 	    return 2; 
      else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && *mbutt) 
        return 3; 
      else if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	    exit(0); 
      else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	    return 0; 
 	  else if(*mx>=528 && *mx<592 && *my>=70 && *my<=86 && *mbutt &&i*5+1<=k) 
 	  { // ɾ �� �� ҳ �� һ �� �� Ʒ 
 	 	  shanchusp(mx,my,mbutt,sp[i*5],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88 && *my<=88*2 && *mbutt &&i*5+2<=k) 
      { // ɾ �� �� ҳ �� �� �� �� Ʒ 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+1],&f); 
 	 	  if(f==1) 
 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*2 && *my<=88*3 && *mbutt &&i*5+3<=k)                
//ɾ����ҳ��������Ʒ 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+2],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*3 && *my<=88*4 && *mbutt &&i*5+4<=k)                
//ɾ����ҳ���ļ���Ʒ 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+3],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*4 && *my<=88*5 && *mbutt &&i*5+5<=k)                
//ɾ��ҳ���ϵĵ������Ʒ 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+4],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 
 	  else if(*mx>=400 && *mx<416 && *my>=31 && *my<=45 && *mbutt &&i*5+1<=k) 
 	  {                                // �� �� ҳ �� �� һ �� �� Ʒ �� �� �� 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88 && *my<=45+88 && *mbutt &&i*5+2<=k)
//�޸�ҳ��ڶ�����Ʒ�ļ۸� 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+1],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*2 && *my<=45+88*2 && *mbutt &&i*5+3<=k) // �� �� ҳ �� �� �� �� �� Ʒ �� �� �� 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+2],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*3 && *my<=45+88*3 && *mbutt &&i*5+4<=k)	//�޸�ҳ����ļ���Ʒ�ļ۸� 
 	  { 
 
 	 
 	  xiugai_jg(mx,my,mbutt,sp[i*5+3],&f); 
  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*4 && *my<=45+88*4 && *mbutt &&i*5+5<=k) // �� �� ҳ �� �� �� �� �� Ʒ �� �� �� 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+4],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 
 	  else if(*mx>=494 && *mx<510 && *my>=31 && *my<=45 && *mbutt&&i*5<k) 
 	  { // Ϊ ҳ �� �� �� һ �� �� Ʒ �� �� 
 
 	      buchong_kc(mx,my,mbutt,sp[i*5],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88 && *my<=45+88 && *mbutt&&i*5+1<k) 
 	  { // Ϊ ҳ �� �� �� �� �� �� Ʒ �� �� 
 	      buchong_kc(mx,my,mbutt,sp[i*5+1],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88*2 && *my<=45+88*2 && 
*mbutt&&i*5+2<k) // Ϊ ҳ �� �� �� �� �� �� Ʒ �� �� 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+2],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else  if(*mx>=494  &&  *mx<510  &&  *my>=31+88*3  &&  *my<=45+88*3  && 
*mbutt&&i*5+3<k)                            // Ϊ ҳ �� �� �� �� �� �� Ʒ �� �� 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+3],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88*4 && *my<=45+88*4 && 
*mbutt&&i*5+4<k) // Ϊ ҳ �� �� �� �� �� �� Ʒ �� �� 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+4],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 
 	      } 
 	  } 
 	  else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
 	  { // �� �� �� һ ҳ 
 	 	  i++; 
 	 	  good_jm(sh,&i); 
          cursor(*mx,*my); 
     	  delay(100); 
 	  } 
 	  else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
 	  { // �� �� �� һ ҳ 
 	      i--; 
 	      good_jm(sh,&i); 
 	      cursor(*mx,*my); 
 	      delay(100); 
 	  } 
 
  } 
} 
void shanchusp(int *mx,int *my,int *mbutt,shangpin*p,int*f)//ɾ����Ʒ������ 
{ 
 	void *buffer; 
 	unsigned size; 
 	size=imagesize(170,140,470,340); 
 	if(((void*)buffer=malloc(size))==NULL) 
 	{ 
 	 	printf("No enough memory"); 
 	 	exit(1); 
 	} 
 	getimage(170,140,470,340,buffer); 
 	setfillstyle(1,WHITE); // �� �� �� �� 
 	bar(170,140,470,340); 
 	setfillstyle(1,DARKGRAY); 
 	bar(170,140,470,160); 
 	setfillstyle(1,LIGHTGRAY); 
 	bar(190,310,230,330); 
 	bar(410,310,450,330); 
 	out_hz(260,230,"ȷ��ɾ����",24,0,BLACK); 
 	out_hz(194,312,"ȡ��",16,16,BLACK); 
 	out_hz(414,312,"ȷ��",16,16,BLACK); 
 	for(;;) 
 	{ 
        newxy(mx,my,mbutt); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //���ȡ�� 
 
 	 	{ 	
 	 	 	putimage(170,140,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
        if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //ȷ��ɾ��    
        {  
 	 	 	p->life=0;      
 	 	 	putimage(170,140,buffer,0); 
 	 	 	free(buffer); 
 	 	 	*f=1; 
 	 	 	break; 
 	 	} 
 	} 
} 
void xiugai_jg(int*mx,int*my,int*mbutt,shangpin*p,int*f) //�޸ļ۸��� 
{ 
 	char str[12]; 
 	str[0]='/0'; 
 	void *buffer; 
 	unsigned size; 
 	size=imagesize(170,120,470,340); 
 	if(((void*)buffer=malloc(size))==NULL) 
 	{ 
 	 	printf("No enough memory"); 
 	 	exit(1); 
 	} 
 	if(*mx>170&&*mx<470&&*my>120&&*my<340) 
 	 	cursor(*mx,*my); 
 	getimage(170,120,470,340,buffer); 
 	setfillstyle(1,WHITE); // �� �� �� �� �� 
 	bar(170,140,470,340); 
 	setfillstyle(1,DARKGRAY); 
 	bar(170,120,470,140); 
 	setfillstyle(1,LIGHTGRAY); 
 	bar(190,310,230,330); 
 	bar(410,310,450,330); 
 	out_hz(200,230,"����۸�",16,16,BLACK); 
 	out_hz(194,312,"ȡ��",16,16,BLACK); 
 	out_hz(414,312,"ȷ��",16,16,BLACK); 
 	setcolor(LIGHTGRAY); 
 	rectangle(266,228,440,248); 
 	if(*mx>170&&*mx<470&&*my>120&&*my<340) 
 
 	 	cursor(*mx,*my); 
 	for(;;) 
 	{ 
        newxy(mx,my,mbutt); 
        if(*mx>266&&*mx<440&&*my>232&&*my<252&&*mbutt==1) //����۸� 
 
 
 	 
 
 	 	input_name(str,267,230,12); 
if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //���ȡ�� 
{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
        if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //ȷ���޸�  
        {  
 	 	 	p->price=atof(str); 
 	 	 	*f=1; 
 	 	 	putimage(170,140,buffer,0); 
 	 	 	free(buffer); 
 	 	 	cursor(*mx,*my); 
 	 	 	break; 
 	 	} 
 	} 
} 
void buchong_kc(int*mx,int*my,int*mbutt,shangpin*p,int*f)//������Ʒ��� 
{ 
 	char str[12]; 
 	str[0]='/0'; 
 	void *buffer; 
    unsigned size; 
    size=imagesize(170,120,470,340); 
    if(((void*)buffer=malloc(size))==NULL) 
    { 
 	 	printf("No enough memory"); 
 	 	exit(1); 
    } 
    getimage(170,120,470,340,buffer); 
    setfillstyle(1,WHITE); // �� �� �� �� �� 
    bar(170,140,470,340); 
    setfillstyle(1,DARKGRAY); 
    bar(170,120,470,140); 
    setfillstyle(1,LIGHTGRAY); 
    bar(190,310,230,330); 
    bar(410,310,450,330); 
 
    out_hz(184,230,"���벹������",16,16,BLACK); 
    out_hz(194,312,"ȡ��",16,16,BLACK); 
    out_hz(414,312,"ȷ��",16,16,BLACK); 
    setcolor(LIGHTGRAY); 
    rectangle(282,228,440,248); 
     
    for(;;) 
    { 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>266&&*mx<440&&*my>232&&*my<252&&*mbutt==1) //���벹������ 
 	 	 	input_name(str,283,230,12); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //ȡ������ 
 	 	{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
 	 	} 
 	 	if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //ȷ�Ͻ��� 
 	 	{  
 	 	 	p->kucun=p->kucun+atoi(str); 
 	 	 	*f=1; 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	free(buffer); 
 	 	 	cursor(*mx,*my); 
 	 	 	break; 
 	 	} 
    } 
} 
void xiesp(shangpin *sh) // �� Ʒ �� Ϣ д �� shangpin.txt 
{ 
 	FILE*fp; 
 	shangpin*p; 
 	p=sh; 
 	if( (fp=fopen("txt\\shangpin.txt","wt"))==NULL) 
    { 
 	 	printf("unable to open shangpin.txt"); 
 	 	exit(1); 
    } 
    while(p!=NULL) 
    { 
 
 	fprintf(fp,"%d %d %d %s %d %d %ld %f %f %f %s %s %s\n",p->life,p->flag, p->cla,p->bianhao,p->kucun,p->xiaoshou,p->t,p->chengben,p->price,p->profit,
 
p->name,p->name_pinyin,p->miaoshu); 
 	 	p=p->next; 
    } 
    fclose(fp); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************** manager_dingdan.c *****************************/ int order_mana(int*mx,int*my,int*mbutt,shangpin*sh,dingdan*dh) //�������� 
{ 
    int i=0,l,k=0; 
    dingdan*p,*dp[20]; 
    shangpin*q,*sq[20]; 
    p=dh; 
    q=sh; 
    while(p!=NULL) 
    { 
      if(p->life==1) 
      { 
          dp[k]=p; 
          while(strcmp(q->bianhao,p->bianhao)!=0&&q!=NULL) 
             q=q->next; 
          sq[k++]=q;    
      } 
      q=sh; 
      p=p->next; 
    } 
    l=(k-1)/4; //l �� ҳ �� 
    order_jm(dh,sh,&i); 
    if(*mx>=125) 
        cursor(*mx,*my); 
    delay(100); 
    for(;;) 
    { 
        newxy(mx,my,mbutt); 
        if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//������Ҫ���� 
            return 1; 
        else if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
            return 2; 
        else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && *mbutt) 
            return 3; 
        else if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
            exit(0); 
        else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
            return 0; 
        else if(*mx>=513-48 && *mx<597 && *my>=70 && *my<=100 && *mbutt &&i*4+1<=k)                 
//��һ����Ʒ���� 
        { 
            if(sq[i*4]->life==0) // �� Ʒ ͣ �� ʱ �� �� 
            { 
 
                kucunbuzu(mx,my,mbutt,2); 
                return 2; 
            } 
            else if(sq[i*4]->kucun==0) // �� Ʒ �� �� �� �� �� 
            { 
                kucunbuzu(mx,my,mbutt,1); 
                return 2; 
            } 
            dp[i*4]->life=0; 
            sq[i*4]->xiaoshou++; 
            sq[i*4]->kucun--; 
            sq[i*4]->profit=sq[i*4]->profit+sq[i*4]->price-sq[i*4]->chengben; 
            xiesp(sh); 
            xiedd(dh);   
            delay(100); 
            return 2; 
        } 
        else if(*mx>=513-48 && *mx<597 && *my>=70+110 && *my<=100+110 && *mbutt &&i*4+2<=k) // �� �� �� �� Ʒ �� �� 
        { 
            if(sq[i*4+1]->life==0) 
            { 
                kucunbuzu(mx,my,mbutt,2); 
                return 2; 
            } 
            else if(sq[i*4+1]->kucun==0) 
            { 
                kucunbuzu(mx,my,mbutt,1); 
                return 2; 
            } 
            dp[i*4+1]->life=0; 
            sq[i*4+1]->xiaoshou++; 
            sq[i*4+1]->kucun--; 
            
sq[i*4+1]->profit=sq[i*4+1]->profit+sq[i*4+1]->price-sq[i*4+1]->chengben; 
            xiesp(sh); 
            xiedd(dh);   
            delay(100); 
            return 2; 
        } 
        else if(*mx>=513-48 && *mx<597 && *my>=70+110*2 && *my<=100+110*2 && *mbutt &&i*4+3<=k) // ҳ �� �� �� �� �� Ʒ �� �� 
        { 
            if(sq[i*4+2]->life==0) 
 
            { 
                kucunbuzu(mx,my,mbutt,2); 
                return 2; 
            } 
            else if(sq[i*4+2]->kucun==0) 
            { 
                kucunbuzu(mx,my,mbutt,1); 
                return 2; 
            } 
            dp[i*4+2]->life=0; 
            sq[i*4+2]->xiaoshou++; 
            sq[i*4+2]->kucun--; 
            
sq[i*4+2]->profit=sq[i*4+2]->profit+sq[i*4+2]->price-sq[i*4+2]->chengben; 
            xiesp(sh); 
            xiedd(dh);   
            delay(100); 
            return 2; 
        } 
        else if(*mx>=513-48 && *mx<597 && *my>=70+110*3 && *my<=100+110*3 && *mbutt &&i*4+4<=k) // ҳ �� �� �� �� �� Ʒ �� �� 
        { 
            if(sq[i*4+3]->life==0) 
            { 
                kucunbuzu(mx,my,mbutt,2); 
                return 2; 
            } 
            else if(sq[i*4+3]->kucun==0) 
            { 
                kucunbuzu(mx,my,mbutt,1); 
                return 2; 
            } 
            dp[i*4+3]->life=0; 
            sq[i*4+3]->xiaoshou++; 
            sq[i*4+3]->kucun--; 
            sq[i*4+3]->profit=sq[i*4]->profit+sq[i*4]->price-sq[i*4]->chengben; 
            xiesp(sh); 
            xiedd(dh);   
            delay(100); 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40 && *my<=56 && *mbutt &&i*4+1<=k) 
        { // ɾ �� ҳ �� �� һ �� �� �� 
            shanchudd(mx,my,mbutt,dp[i*4]); 
 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110 && *my<=56+110 && *mbutt &&i*4+2<=k) // ɾ �� ҳ �� �� �� �� �� �� 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+1]); 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110*2 && *my<=56+110*2 && *mbutt &&i*4+3<=k) // ɾ �� ҳ �� �� �� �� �� �� 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+2]); 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110*3 && *my<=56+110*3 && *mbutt &&i*4+4<=k) // ɾ �� ҳ �� �� �� �� �� �� �� 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+3]); 
            return 2; 
        } 
        else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
        { // �� �� �� һ ҳ 
            i++; 
            order_jm(dh,sh,&i); 
            cursor(*mx,*my); 
            delay(100); 
        } 
        else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
        { // �� �� �� һ ҳ 
            i--; 
            order_jm(dh,sh,&i); 
            cursor(*mx,*my); 
            delay(100); 
        } 
    } 
} 
void shanchudd(int *mx,int *my,int *mbutt,dingdan*p) //ɾ������ 
{ 
    void *buffer; 
    unsigned size; 
    size=imagesize(170,120,470,340); 
    if(((void*)buffer=malloc(size))==NULL) 
    { 
 	 	printf("No enough memory"); 
 
 	 	exit(1); 
    } 
    getimage(170,120,470,340,buffer); // �� �� �� �� �� 
    setfillstyle(1,WHITE); 
    bar(170,140,470,340); 
    setfillstyle(1,DARKGRAY); 
    bar(170,120,470,140); 
    setfillstyle(1,LIGHTGRAY); 
    bar(190,310,230,330); 
    bar(410,310,450,330); 
    out_hz(194,312,"ȡ��",16,16,BLACK); 
    out_hz(414,312,"ȷ��",16,16,BLACK); 
    out_hz(260,230,"ȷ��ɾ����",24,0,BLACK); 
    for(;;) 
    { 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //���ȡ�� 
 	 	{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
 	 	} 
 	 	if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //���ȷ�� 
 	 	{  
 	 	 	p->life=0; 
 	 	 	putimage(170,140,buffer,0); 
 	 	 	free(buffer); 
 	 	 	cursor(*mx,*my); 
 	 	 	break; 
 	 	} 
    } 
} 
 
void xiedd(dingdan*dh) // �� д �� �� �� �� txt 
{ 
 	FILE*fp; 
 	dingdan*p; 
 	p=dh; 
 	if( (fp=fopen("txt\\dingdan.txt","wt"))==NULL) 
    { 
        printf("unable to open dingdan.txt"); 
 	 	exit(1); 
    } 
 
    while(p!=NULL) 
    { 
 	   
fprintf(fp,"%d %s %s %s %s\n",p->life,p->bianhao,p->name,p->phone,p-> address); 
 	 	p=p->next; 
    } 
    fclose(fp); 
} 
void order_jm(dingdan*dh,shangpin*sh,int*i) // �� �� �� �� �� i+1 ҳ �� �� 
{ 
    int j=0,l=0; // l�� �� �� �� �� 
    char str[30]; 
    dingdan *p; // �� �� �� �� �� ͷ ָ �� 
    shangpin *q; 
    q=sh; 
    p=dh; 
    setfillstyle(1,LIGHTBLUE); 
    bar(125,0,640,480); 
    out_hz(587,451,"��ҳ",24,0,WHITE); 
    out_hz(130,451,"��ҳ",24,0,WHITE); 
    setcolor(WHITE); 
    setfillstyle(1,WHITE); 
    while(p!=NULL&&l<4+4*(*i)) // �� �� �� �� �� ɨ �� 
    {   
        if(l>=4*(*i)&&p->life==1) 
        { 
            while(strcmp(p->bianhao,q->bianhao)!=0&&q!=NULL) 
            { 
                q=q->next; 
            } 
            strcpy(str,q->name); 
            rectangle(171,40+j*110,597,100+110*j); // �� ӡ �� Ʒ �� Ϣ 
            out_hz(173,42+110*j,"�û�",16,16,WHITE); 
            out_hz(173,62+110*j,"��Ʒ",16,16,WHITE); 
            out_hz(173,82+110*j,"��ַ",16,16,WHITE); 
            out_hz(380,42+110*j,"�绰",16,16,WHITE); 
            outtextxy(210,44+110*j,p->name); 
            out_hz(210,62+110*j,str,16,16,WHITE); 
            outtextxy(210,84+110*j,p->address); 
            outtextxy(417,44+110*j,p->phone);   
            bar(513-48,70+110*j,597,100+110*j); 
            out_hz(531-48,73+110*j,"�������",24,0,LIGHTBLUE); 
            bar(537,40+110*j,597,56+110*j); 
 
            out_hz(543,42+110*j,"ȡ������",12,12,LIGHTBLUE); 
            j++; 
            q=sh; 
        } 
        if(p->life==1) // δ �� �� �� �� �� 
            l++; 
        p=p->next; 
    }        
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/*************************** liuyan.c *****************************/ 
int message(int*mx,int*my,int*mbutt,liuyan*lh) // �� �� �� �� �� �� 
{ 
 	int i; 
 	message_jm(); 
 	delay(100); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//�������˵� 
 	 	 	return 1; 
 	 	else if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
 	 	 	return 2; 
 	 	else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && *mbutt) 
 	 	 	return 3; 
 	 	else if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	exit(0); 
 	 	else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	return 0; 
 	 	else if(*mx>=200 && *mx<306 && *my>=223 && *my<=257 && *mbutt) 
 	 	{ // δ �� �� �� 
 	 	 	liuyan *lp1[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,0,1,lh,lp1,"\0"); 
 	 	 	return i; 
 	 	} 
 	 	else if(*mx>=459 && *mx<565 && *my>=223 && *my<=257 && *mbutt) 
 	 	{ // �� �� �� �� 
 	 	 	liuyan *lp2[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,0,0,lh,lp2,"\0"); 
 	 	 	return i; 
 	 	} 
 	 	else if(*mx>=330 && *mx<436 && *my>=123 && *my<=157 && *mbutt) 
 	 	{ // �� �� �� Ϣ 
            liuyan *lp3[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,1,1,lh,lp3,"\0"); 
 	 	 	return i; 
 	 	} 	
 	 	else if(*mx>=330 && *mx<436 && *my>=323 && *my<=357 && *mbutt) 
 	 	{ // �� �� �� Ϣ 
 	 	 	for(i=7;i==7;) 
 
 
 
 	 
 
 	 	 	i=bianxieliuyan(mx,my,mbutt,2,lh,"\0"); 
 	return i; 
} 
 	} 	
} 		
 		
int weidu(int *mx,int *my,int *mbutt,int iden,int g,int t,liuyan *lh,liuyan 
*lp[],char str[12]) // �� �� �� �� �� �� 
{ 
 	int i=0,k=0,l; 
 	liuyan*q,*p; 
 	p=lh; //p �� �� ͷ ָ �� 
 	if(iden==2&&g==0) //iden=2����ʾ����Ա����ʹ�����Թ��� 
    { //g ��ǣ�0 ��ʾ�ǽ��յ�����Ϣ��1 ��ʾ��������Ϣ   	 while(p!=NULL) //t=1 ��ʾδ����t=0,��ʾ�Ѷ� 
 	 	{ 
 	 	 	if(p->flag==0&&p->life==t) 
                lp[k++]=p;	//0 ��ʾ�û������̵����ԣ�1 ��ʾ���̸��û������� 
            p=p->next; 
        } 
    } 
    else if(iden==2) //����Ա��������Ϣ 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==1) 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
    else if(iden==1&&g==1) //�ض��û�����δ���Ļ��Ѷ�����Ϣ 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==1 && p->life==t && strcmp(p->yonghuname,str)==0) 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
    else if(iden==1) // �� �� �� �� �� �� �� �� �� Ϣ 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==0&&strcmp(p->yonghuname,str)==0) 
 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
 	weidu_jm(lh,&i,str,iden,g,t); //������ʾ���� 
 	l=(k-1)/5; 
 	cursor(*mx,*my); 
 	delay(100); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
        if(iden==2) 
        { 
 	 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//�����˵� 
 	 	 	 	return 1; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
 	 	 	 	return 2; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && 
*mbutt) 
 	 	 	 	return 3; 
 	 	 	else if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	exit(0); 
 	 	 	else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	return 0; 
 	 	} 
 	 	if(iden==1) // �� �� �� �� �� �� 
 	 	{ 
 	 	 	if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	exit(0); 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) 
 	 	 	 	return 1; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) 
 	 	 	 	return 2; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt) 
 	 	 	 	return 3; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt) 
 	 	 	 	return 4; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt) 
 	 	 	 	return 5; 
 	 	 	else if(*mx>=38 && *mx<87 && *my>=456 && *my<=490 && *mbutt) 
 	 	 	 	return 6; 	 
 	 	 	else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	return 0; 	 
 
 
 	 
 	if(*mx>=210 && *mx<592 && *my>=68 && *my<=88 && *mbutt &&i*5+1<=k) 
{ 
 	 	 	input_liuyan(211,68,iden,lp[i*5],q,lh); //�ظ�ҳ���һ������  
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88 && *my<=88*2 && *mbutt &&i*5+2<=k)
//�ظ�ҳ��ڶ������� 
 	 	{ 
 	 	 	input_liuyan(211,68+88,iden,lp[i*5+1],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*2 && *my<=88*3 && *mbutt 
&&i*5+3<=k) // �� �� ҳ �� �� �� �� �� �� 
 	 	{ 
 	 	 	input_liuyan(211,68+88*2,iden,lp[i*5+2],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*3 && *my<=88*4 && *mbutt 
&&i*5+4<=k)	//�ظ�ҳ����������� 
 	 	{ 
 	 	 	input_liuyan(211,68+88*3,iden,lp[i*5+3],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*4 && *my<=88*5 && *mbutt &&i*5+5<=k) // �� �� ҳ �� �� �� �� �� �� 
 	 	{ 
 	 	 	 
 	 	 	input_liuyan(211,68+88*4,iden,lp[i*5+4],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
 	 	{ // �� �� �� һ ҳ 
 	 	 	i++; 
 	 	 	weidu_jm(lh,&i,str,iden,g,t); 
 	 	 	cursor(*mx,*my); 
 	 	 	delay(100); 
 	 	} 
 	 	else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
 	 	{ // �� �� �� һ ҳ 
 	 	 	i--; 
 	 	 	weidu_jm(lh,&i,str,iden,g,t); 
 	 	 	cursor(*mx,*my); 
 	 	 	delay(100); 
 
        if(t==1) // �� �� δ �� �� �� 
        { 
 	 	 	if(*mx>=512 && *mx<592 && *my>=35 && *my<=51 && *mbutt &&i*5+1<=k) 
 	 	 	{                     // �� ҳ �� �� �� һ �� �� �� �� �� Ϊ �� �� 
 	 	 	 	lp[i*5]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88 && *my<=51+88 && *mbutt &&i*5+2<=k)		//��ҳ���ϵڶ������Ա��Ϊ�Ѷ� 
 	 	 	{ 
 	 	 	 	lp[i*5+1]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*2 && *my<=51+88*2 && *mbutt &&i*5+3<=k)		//��ҳ���ϵ��������Ա��Ϊ�Ѷ� 
 	 	 	{ 
 	 	 	 	lp[i*5+2]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*3 && *my<=51+88*3 && *mbutt &&i*5+4<=k) // �� ҳ �� �� �� �� �� �� �� �� �� Ϊ �� �� 
 	 	 	{ 
 	 	 	 	lp[i*5+3]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*4 && *my<=51+88*4 && *mbutt &&i*5+5<=k)		//��ҳ���ϵ��������Ա��Ϊ�Ѷ� 
 	 	 	{ 
 	 	 	 	lp[i*5+4]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
        } 
 	} 
} 
void message_jm() // �� �� �� �� �� �� �� 
{ 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,640,480); 
 	setfillstyle(1,WHITE); 
 
 	bar(200,223,306,257); 
 	bar(459,223,565,257); 
 	bar(330,123,436,157); 
 	bar(330,323,436,357); 
 	out_hz(205,228,"δ������",24,0,LIGHTBLUE); 
 	out_hz(464,228,"�Ѷ�����",24,0,LIGHTBLUE); 
 	out_hz(335,128,"�ѷ���Ϣ",24,0,LIGHTBLUE); 
 	out_hz(335,328,"��д��Ϣ",24,0,LIGHTBLUE); 
} 
 
void weidu_jm(liuyan*lh,int*i,char str[12],int iden,int g,int t) 
{ // �� ӡ �� i+1 ҳ �� �� �� 
    int l=0,j=0; 
 	liuyan *p; 
 	p=lh; 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,640,480); 
 	out_hz(587,451,"��ҳ",24,0,WHITE); 
 	out_hz(130,451,"��ҳ",24,0,WHITE); 
 	setcolor(WHITE); 
 	setfillstyle(1,WHITE); 
 	if(iden==2) 
 	{ 
 	 	if(g==0) 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==g&&p->life==t)//����Ա���յ������� 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) // �� �� �� �� �� t ֵ �� �� 
 	 	 	 	 	{ // �� ӡ �� �� 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"�û�",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"����",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"�ظ�",12,12,WHITE); 
 	 	 	 	 	 	if(t==1) 
 	 	 	 	 	 	 	out_hz(515,35+j*88,"���Ϊ�Ѷ�",16,16,WHITE); 
 	 	 	 	 	 	bar(210,68+88*j,592,88+88*j); 
 	 	 	 	 	 	outtextxy(210,54+88*j,p->xinxi); 
 	 	 	 	 	 	outtextxy(210,34+88*j,p->yonghuname); 
 	 	 	 	 	 	j++; 
 	 	 	 	 	} 
 	 	 	 	 	l++; 
 	 	 	 	} 	
 
 
 	 
 	 	 	p=p->next; 
 	} 
 	 	}   
 	 	else 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) // �� �� Ա �� �� �� �� �� 
 	 	 	{ 
 	 	 	 	if(p->flag==g) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"�û�",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"����",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"�ظ�",12,12,WHITE); 
 	 	 	 	 	 	bar(210,68+88*j,592,88+88*j); 
 	 	 	 	 	 	outtextxy(210,54+88*j,p->xinxi); 
 	 	 	 	 	 	outtextxy(210,34+88*j,p->yonghuname); 
 	 	 	 	 	 	j++; 
 	 	 	 	 	} 
 	 	 	 	 	l++; 
 	 	 	 	} 
 	 	 	 	p=p->next; 
 	 	 	} 
 	 	} 
    } 
 	 
    if(iden==1) 
    { 
 	 	if(g==1) // �� Ӧ �� �� �� �� �� �� �� �� 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==g&&p->life==t&&strcmp(str,p->yonghuname)==0) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) // �� �� �� �� 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"�û�",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"����",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"�ظ�",12,12,WHITE); 
 	 	 	 	 	 	if(t==1) 
 	 	 	 	 	 	 	out_hz(515,35+j*88,"���Ϊ�Ѷ�",16,16,WHITE); 
 	 	 	 	 	 	bar(210,68+88*j,592,88+88*j); 
 
 
 
 	 
 
 	 	 	 	 	outtextxy(210,54+88*j,p->xinxi); 
 	 	 	 	outtextxy(210,34+88*j,p->yonghuname); 
 	 	 	 	j++; 
 	 	 	 	 	} 
 	 	 	 	 	l++; 
 	 	 	 	} 
 	 	 	 	p=p->next; 
 	 	 	} 
 	 	}   
 	 	else // �� �� �� �� �� �� �� �� �� Ϣ 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==0&&strcmp(str,p->yonghuname)==0) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"�û�",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"����",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"�ظ�",12,12,WHITE); 
 	 	 	 	 	 	bar(210,68+88*j,592,88+88*j); 
 	 	 	 	 	 	outtextxy(210,54+88*j,p->xinxi); 
 	 	 	 	 	 	outtextxy(210,34+88*j,p->yonghuname); 
 	 	 	 	 	 	j++; 
 	 	 	 	 	} 	
 	 	 	 	 	l++; 
 	 	 	 	} 
 	 	 	 	p=p->next; 
 	 	 	} 	
 	 	} 	 	
    } 
} 
 
void input_liuyan(int x,int y,int iden,liuyan*lp,liuyan*q,liuyan*lh) 
{                                      // �� �� �� �� �� д �� �� �� 
 	FILE*fp; 
 	liuyan*p; 
 	int key,i=0; 
 	char zifu[2]; 
 	char str[80]; 
 	guangbiao(x,y); 
 	for(i=0;i<80;i++) 
 
 
 
 
 	 
 
 
 	delay(20); 
key=bioskey(0); if(key==Key_Enter) 
{ 
 	 	 	str[i]='\0'; 
 	 	 	xiaoguangbiao(x,y); 
 	 	 	break; 
 	 	} 
 	 	if(key==Key_Backspace) 
 	 	{ 
 	 	 	if(i==0) 
 	 	 	{ 
 	 	 	 	str[i]='\0'; 
 	 	 	 	i--; 
 	 	 	} 
 	 	 	else if(i>0) 
 	 	 	{ 
 	 	 	 	xiaoguangbiao(x,y); 
 	 	 	 	x=x-8; 
 	 	 	 	bar(x,y,x+8,y+16); 
 	 	 	 	i--; 
 	 	 	 	str[i]='\0'; 
 	 	 	 	guangbiao(x,y); 
 	 	 	 	i--; 
 	 	 	} 
 	 	} 
 	 	if(('!'<=(char)key&&(char)key<='~')) 
 	 	{ 
 	 	 	xiaoguangbiao(x,y); 
 	 	 	zifu[0]=(char)key; 
 	 	 	zifu[1]='\0'; 
 	 	 	setcolor(BLACK); 
 	 	 	outtextxy(x,y+4,zifu); 
 	 	 	str[i]=(char)key; 
 	 	 	x=x+8; 
 	 	 	guangbiao(x,y); 
 	 	} 
 	} 	
 	if(i==80) 
 	{ 
 	 	xiaoguangbiao(x,y); 
 	} 
 	if(i==0) 
 
 	 	return; 
 	} 
 	p=lh; 
 	while(p->next!=NULL) 
 	{ 
 	 	p=p->next; 
 	} 
 	if((q=(liuyan*)malloc(sizeof(liuyan)))==NULL) //�����ڴ�ռ䲢��� 
 	{ 
 	 	printf("No enough memory"); 
 	 	exit(0); 
    } 
 	p->next=q; 
 	q->next=NULL; 
 	p=lh; 
 	lp->life=0; 
 	if(iden==2) //����ǹ���Ա���ڱ༭��������Ϊ 1 
 	 	q->flag=1; 
 	else 
 	 	q->flag=0; 
 	q->life=1; 
 	strcpy(q->xinxi,str); 
 	strcpy(q->yonghuname,lp->yonghuname); 
 	if( (fp=fopen("txt\\liuyan.txt","wt"))==NULL) //���ļ� 
 	{ 
 	 	printf("unable to open liuyan.txt"); 
 	 	exit(1); 
 	} 
 	while(p!=NULL) // �� �� �� Ϣ д �� �� �� 
 	{ 
 	 	fprintf(fp,"%d %d %s %s\n",p->flag,p->life,p->yonghuname,p->xinxi); 
 	 	p=p->next; 
 	} 
 	fclose(fp); 
 	return; 
} 
int bianxieliuyan(int*mx,int*my,int*mbutt,int iden,liuyan*lh,char str[12]) 
{                                                          // �� �� �� �� 
 	bianxie_jm(iden); // �� �� �� �� 
 	char str1[12]; // �� �� �� 
 	char str2[80]; // �� �� �� �� 
 	str1[0]='\0'; 
 	str2[0]='\0'; 
 	cursor(*mx,*my); 
 
 	setfillstyle(1,WHITE); 
 	if(iden==1) 
 	 	strcpy(str1,str); //str����ǰ��¼�û����û��� 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(iden==2) 
 	 	{ 
 	 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//�����˵� 
 	 	 	 	return 1; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=87*2+44 && *my<=87*2+44*2 && *mbutt) 
 	 	 	 	return 2; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=87*3+44*2 && *my<=87*3+44*3 && 
*mbutt) 
 
 
 	 
 
 	 	 	return 3; 
 	else if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	 	exit(0); 
 	 	 	else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	return 0; 
 	 	} 
 	 	if(iden==1) // �� �� �� �� �� �� �� 
 	 	{ 
 	 	 	if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	exit(0); 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) 
 	 	 	 	return 1; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) 
 	 	 	 	return 2; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt) 
 	 	 	 	return 3; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt) 
 	 	 	 	return 4; 
 	 	 	else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt) 
 	 	 	 	return 5; 
 	 	 	else if(*mx>=38 && *mx<87 && *my>=456 && *my<=490 && *mbutt) 
 	 	 	 	return 6; 
 	 	 	else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) 
 	 	 	 	return 0; 
 	 	} 
 	 	if(*mx>=201 && *mx<595 && *my>=58+2*88 && *my<=76+2*88 && *mbutt&&iden==2) 
 	 	{ 
 	 	 	bar(202,58+2*88,594,76+2*88); // �� �� �� �� �� 
 	 	 	input_name(str1,202,59+2*88,12); 
 	 	} 
 
 
 	 
 	else if(*mx>=201 && *mx<595 && *my>=84+2*88 && *my<=104+2*88 && *mbutt) 
{ // �� �� �� Ϣ �� �� 
 	 	 	bar(202,85+2*88,594,103+2*88); 
 	 	 	input_name(str2,202,86+2*88,80); 
 	 	} 
 	 	else if(*mx>=564 && *mx<592 && *my>=129+2*88 && *my<=145+2*88 && *mbutt) 
 	 	{ // ȷ �� �� �� �� �� 
 	 	 	if(str1[0]=='\0'||str2[0]=='\0') 
 	 	 	{ 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else 
 	 	 	{ 
 	 	 	 	liuyan_xieru(lh,str1,str2,iden-1); 
 	 	 	 	delay(100); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	} 
 	 	else if(*mx>=173 && *mx<201 && *my>=129+2*88 && *my<=145+2*88 && *mbutt) 
 	 	 	return 7; 
 	} 	
} 		
void bianxie_jm(int iden) // �� д �� �� �� �� 
{ 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,640,480); 
 	setfillstyle(1,WHITE); 
 	bar(171,30+2*88,597,150+2*88); 
 	setfillstyle(1,LIGHTGRAY); 
 	bar(171,30+2*88,597,50+2*88); 
 	if(iden==2) 
 	 	bar(173,57+2*88,201,77+2*88); 
    bar(173,84+2*88,201,104+2*88); 
 	bar(564,129+2*88,592,145+2*88); 
 	bar(176,129+88*2,204,145+2*88); 
 	setcolor(LIGHTGRAY); 
 	if(iden==2) 
 	 	rectangle(173,57+2*88,595,77+2*88); 
 	rectangle(173,84+2*88,595,104+2*88); 
    out_hz(566,131+2*88,"ȷ��",12,12,DARKGRAY); 
    out_hz(178,131+88*2,"ȡ��",12,12,DARKGRAY); 
    if(iden==2) 
 	 	out_hz(175,61+2*88,"�û�",12,12,DARKGRAY); 
 	out_hz(175,88+2*88,"����",12,12,DARKGRAY); 
 
 	out_hz(360,34+2*88,"��д��Ϣ",12,12,DARKGRAY);   
} 
void liuyan_xieru(liuyan*h,char a[12],char c[80],int t) //����д�� txt �ļ� 
{ 
 	FILE *fp; 
 	liuyan *p,*q; 
 	p=h; 
 	if((q=(liuyan *)malloc(sizeof(liuyan)))==NULL) 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	strcpy(q->xinxi,c); 
 	strcpy(q->yonghuname,a); 
 	q->flag=t; 
 	q->life=1; 
 	while(p->next!=NULL) 
 	{ 
 	 	p=p->next; 
 	} 
 	p->next=q; 
 	q->next=NULL; 
 	p=h; 
 	if( (fp=fopen("txt\\liuyan.txt","wt"))==NULL) 
    { 
 	 	printf("unable to open liuyan.txt"); 
 	 	exit(1); 
    } 
    while(p!=NULL) 
    { 
 	 	fprintf(fp,"%d %d %s %s\n",p->flag,p->life,p->yonghuname,p->xinxi); 
 	 	p=p->next; 
    } 
    fclose(fp); 
} 
 
void xiely(liuyan *lh) // �� �� �� �� �� д �� liuyan.txt 
{ 
 	FILE*fp; 
 	liuyan*p; 
 	p=lh; 
 	if( (fp=fopen("txt\\liuyan.txt","wt"))==NULL) 
    { 
 	 	printf("unable to open liuyan.txt"); 
 
 	 	exit(1); 
    } 
    while(p!=NULL) 
    { 
 	 	fprintf(fp,"%d %d %s %s\n",p->flag,p->life,p->yonghuname,p->xinxi); 
 	 	p=p->next; 
    } 
    fclose(fp); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/************************** fuzhu.c *****************************/ 
 
void guangbiao(int x,int y) // �� �� �� 
{ 
 	 setfillstyle(1,BLACK); 
 	 bar(x,y,x+2,y+16); 
} 
void xiaoguangbiao(int x,int y) // �� �� �� �� 
{ 
     setfillstyle(1,WHITE); 
     bar(x,y,x+2,y+16); 
} 
void input_name(char *p,int x,int y,int length)  //�������뺯�� 
{ 
 	int key,i=0; 
 	char zifu[2]; 
 	guangbiao(x,y); 
 	for(i=0;i<length;i++) 
 	{ 
 	   delay(20); 
 	   key=bioskey(0); 
 	   if(key==Key_Enter) 
 	   { 
 	 	  *(p+i)='\0'; 
 	 	  xiaoguangbiao(x,y); 
 	 	  break; 
 	   } 
 	   if(key==Key_Backspace) 
 	   { 
 	 	   if(i==0) 
 	 	 	{ 
 	 	 	 	*(p+i)='\0'; 
 	 	 	 	i--; 
 	 	 	} 
 	 	   else if(i>0) 
 	 	   { 
 	 	 	 xiaoguangbiao(x,y); 
 	 	 	 x=x-8; 
 	 	 	 bar(x,y,x+8,y+16); 
 	 	 	 i--; 
 	 	 	 *(p+i)='\0'; 
 	 	 	 guangbiao(x,y); 
 	 	 	 i--; 
 	 	   } 
 	   } 
 	   if( char(key)>='!'&&char(key)<='~' ) 
 
 	   { 
 	 	  xiaoguangbiao(x,y); 
 	 	  zifu[0]=(char)key; 
 	 	  zifu[1]='\0'; 
 	 	  setcolor(BLACK); 
 	 	  outtextxy(x,y+4,zifu); 
 	 	  *(p+i)=(char)key; 
 	 	  x=x+8; 
 	 	  guangbiao(x,y); 
 	   } 
 	} 
 	if(i==length) 
 	  xiaoguangbiao(x,y); 
} 
 
void input_mima(char *p,int x,int y) // �� �� �� �� �� �� 
{ 
 	int key,i; 
 	guangbiao(x,y); 
 	for(i=0;i<12;i++) 
 	{ 
 	   key=bioskey(0); 
 	   if(key==Key_Enter) 
 	   { 
 	 	  *(p+i)='\0'; 
 	 	  xiaoguangbiao(x,y); 
 	 	  break; 
 	   } 
 	   if(key==Key_Backspace) 
 	   { 
 	 	   if(i==0) 
 	 	 	{ 
 	 	 	 	*(p+i)='\0'; 
 	 	 	 	i--; 
 	 	 	} 
 	 	   else 
 	 	   { 
 	 	 	   xiaoguangbiao(x,y); 
 	 	 	   x=x-8; 
 	 	 	   bar(x,y,x+8,y+16); 
 	 	 	   i--; 
 	 	 	   *(p+i)='\0'; 
 	 	 	   guangbiao(x,y); 
 	 	 	   i--; 
 
 	 	   } 
 	   } 
 	   if( char(key)>='!'&&char(key)<='~' ) 
 	   { 
 	 	  xiaoguangbiao(x,y); 
 	 	  setcolor(BLACK); 
 	 	  outtextxy(x,y+4,"*"); 
 	 	  *(p+i)=(char)key; 
 	 	  x=x+8; 
 	 	  guangbiao(x,y); 
 	   } 
 	} 
 	if(i==12) 
 	  xiaoguangbiao(x,y); 	 
} 
 
void outinformation(int *mx,int *my,int *mbutt,int x,int y,char *out)             
//��Ϣ����ʾ������ 
{ 
 	void *buffinf=NULL;		//x,y �����ľ���λ�ã�out ������ʾ���� 
 	unsigned size; 
 	size=imagesize(0,0,220,100); 	 
 	if(((void *)buffinf=malloc(size))==NULL) 
 	{ 
 	 	printf("not enough buffer"); 
 	 	exit(1); 
 	} 
 	getimage(x,y,220+x,100+y,buffinf); 
    setfillstyle(1,WHITE); 
    bar(x,y,x+220,y+100); 
    setfillstyle(1,DARKGRAY); 
    bar(x,y,x+220,y+20); 
    setfillstyle(1,LIGHTGRAY); 
    bar(x+170,y+75,x+210,y+95); 
 	out_hz(x+78,y+2,"��Ϣ��ʾ",16,16,BLACK); 
 	out_hz(x+174,y+77,"ȷ��",16,16,BLACK);         
    out_hz(x+20,y+37,out,16,16,BLACK);   	 	 	 	 	 
 	while(1) 
 	{ 
 	  newxy(mx,my,mbutt); 
 	  if(*mbutt==1) // �� �� " ȷ �� " �� �� 
 	  { 
 	 	 if((*mx>=x+170&&*mx<=x+210)&&(*my>=y+75&&*my<=y+95)) 
 	 	 	break; // �� �� ѭ �� 
 
 	  } 
 	} 
 	cursor(*mx,*my); 
 	putimage(x,y,buffinf,0); 
 	cursor(*mx,*my); // ͼ �� �� �� 
 	free(buffinf); // �� �� �� �� 
 	delay(100); 
} 
void timeoutput(int x,int y) // �� ָ �� λ �� �� ʾ ʱ �� 
{ 
 	long t; 
 	char s[80]; 
 	struct tm *p; 
    char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"}; 
 	t=time(NULL)-5*3600; 
 	p=gmtime(&t); 
 	sprintf(s,"%d-%02d-%02d %s %02d:%02d:%02d",(1900+p->tm_year),(1+p->tm_mon), (p->tm_mday),wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec); 
    //sprintf(s,"%d-%02d-%02d",(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday)); 
    outtextxy(x,y,s); 
} 
void exchangetime(long t,int x,int y) //������������ת���ɻ���ʱ���ַ��� 
{ 
 	char s[80]; 
 	struct tm *p; 
 	p=gmtime(&t); 
 	sprintf(s,"%d-%02d-%02d",(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday)); 
 	outtextxy(x,y,s); 
} 
 
/***��� BMP ͼ���Ƿ����Ҫ��***/ void BMP_check(bmp_picture image) 
{ 
 	if(image.file.bfType!=0X4D42) //����Ƿ�Ϊ bmp �ļ� 
 	{ 
 	 	printf("Not a BMP file!\n"); 
 	 	printf("image.file.bfType=%u\n",image.file.bfType); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	if(image.info.biCompression!=0) //����Ƿ�Ϊѹ���ļ� 
 	{ 
 	 	printf("Can Not Open a Compressed bmp file!\n"); 
 	 	printf("image.info.biCompression=%u\n",image.info.biCompression); 
 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	if(image.info.biBitCount!=8) //����Ƿ�Ϊ 256 ɫ�ļ� 
 	{ 
 	 	printf("Not a 256 color bmp file\n"); 
 	 	printf("image.info.biBitCount=%u",image.info.biBitCount); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	return; 
} 
/***��ҳ����***/ 
void Select_page(unsigned char page) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f05; 
 	regs.x.bx=0; 
 	regs.x.dx=page; 
 	int86(0x10,&regs,&regs); 
 	return; 
} 
/***���� SVGA ��ʾģʽ****/ 
unsigned char set_SVGA_mode(int vmode) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f02; 
 	regs.x.bx=vmode; 
 	int86(0x10,&regs,&regs); 
 	return(regs.h.ah); 
} 
/*** �� ȡ SVGA �� ʾ ģ ʽ ***/ unsigned int get_SVGA_mode(void) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f03; 
 	int86(0x10,&regs,&regs); 
 	return(regs.x.bx); 
} 
/***���õ�ɫ��***/ 
void set_SVGA_palette(unsigned char RED[],unsigned char GREEN[],unsigned char BLUE[]) 
{ 
 	int i; 
 
 	for(i=0;i<256;i++) 
 	{ 
 	 	outportb(0X03C8,i); 
 	 	outportb(0X03C9,RED[i]>>2); 
 	 	outportb(0X03C9,GREEN[i]>>2); 
 	 	outportb(0X03C9,BLUE[i]>>2); 
 	} 
 	return; 
} 
/***��ȡ BMP ͼ��***/ 
void Read_BMP(char *filename) // �� �� ͼ Ƭ �� �� 
{ 
 	FILE *fp; 
 	long ptrwidth, ptrheight; 
 	bmp_picture bmp256; 
 	char * buffer; 
 	unsigned int savemode; 
 	unsigned char np=0, op=0; 
 	unsigned long pos; 
 	unsigned int linebyte; 
 	unsigned char red[256],green[256],blue[256]; 
 	int i,j; 	 
 	if((fp=fopen(filename,"rb"))==NULL) 
 	{ 
 	 	printf("%s BMP File Open Fail!\n",filename); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
        fread((char *)&bmp256.file,sizeof(bitmapfile),1,fp); 
 	fread((char *)&bmp256.info,sizeof(bitmapinfo),1,fp); //��bmp ͼ����Ϣ�����ṹ bmp256 �� 
 	fread((char *)&bmp256.palette[0],1024,1,fp);   // �� bmp �� ɫ �� �� �� �ṹ bmp256 ��  
 	BMP_check(bmp256); //��� bmp �ļ� 
 	ptrwidth=bmp256.info.biWidth; 
 	ptrheight=bmp256.info.biHeight; 
 	linebyte=((ptrwidth*(long)bmp256.info.biBitCount+31)/32)*4; //�������  
 	savemode=get_SVGA_mode(); //���� SVGA ��ʾģʽ 
 	set_SVGA_mode(0x105); //���� SVGA ��ʾģʽΪ 1024*768 
 	for(i=0;i<256;i++) //��ͼƬ��ɫ����Ϣ���д���׼�� 
 	{ 
 	 	red[i]=bmp256.palette[i].red; 
 	 	green[i]=bmp256.palette[i].green; 
 	 	blue[i]=bmp256.palette[i].blue; 
 
 	} 
 	set_SVGA_palette(red,green,blue); //��ʼ���õ�ɫ�� 
 	fseek(fp,(long)bmp256.file.bfOffset,SEEK_SET); 
 	if((buffer=(char *)malloc(linebyte))==NULL) //���䶯̬�ڴ� 
 	{ 
 	 	printf("Malloc Func Used Fail!"); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	for(i=ptrheight-1;i>=0;i--) 
 	{ 
 	 	fread(buffer,linebyte,1,fp); 
 	 	for(j=0;j<ptrwidth;j++) 
 	 	{ 
 	 	 	pos=i*(long)1024l+j; 
 	 	 	np=pos/65536l; 
 	 	 	if(np!=op) //SVGA ��ʾ��ҳ 
 	 	 	{ 
 	 	 	 	Select_page(np); 
 	 	 	 	op=np;   
 	 	 	} 
 	 	 	pokeb(0xa000,pos%65536l,buffer[j]); 
 	 	} 
 	} 
 	free(buffer); 
 	fclose(fp); 
 	getch(); 
 	set_SVGA_mode(savemode); 
 	return; 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
/************************** mouse_hz.c **************************/ 
 
 
union REGS regs; 
void cursor(int x,int y) /*����꺯�� */ 
{ 
 	setwritemode(XOR_PUT); 
 	setlinestyle(0,0,1); 
 	setcolor(RED); 
 	line(x,y,x+11,y+11); 
 	line(x+10,y+11,x+8,y+11); 
 	line(x+7,y+11,x+10,y+19); 
 	line(x+9,y+20,x+8,y+20); 
 	line(x+7,y+19,x+4,y+12); 
 	line(x+3,y+13,x+1,y+15); 
 	line(x,y+16,x,y+1); 
 	setcolor(DARKGRAY); 
 	line(x+1,y+2,x+1,y+14); 
 	line(x+2,y+3,x+2,y+13); 
 	line(x+3,y+4,x+3,y+12); 
 	line(x+4,y+5,x+4,y+11); 
 	line(x+5,y+6,x+5,y+13); 
 	line(x+6,y+7,x+6,y+15); 
 	line(x+7,y+8,x+7,y+10); 
 	line(x+7,y+14,x+7,y+17); 
 	line(x+8,y+9,x+8,y+10); 
 	line(x+8,y+16,x+8,y+19); 
 	line(x+9,y+10,x+9,y+10); 
 	line(x+9,y+18,x+9,y+19); 
 	setwritemode(COPY_PUT); 
} 
 
int init_mouse(int xmi,int xma,int ymi,int yma) /* �������ʼ���� */ 
{ 
 	int retcode; 
 	regs.x.ax=0; 
 	int86(51,&regs,&regs); 
 	retcode=regs.x.ax; 
 	if(retcode==0) 
 	{ 
 	 	printf("mouse absent"); 
 	 	delay(5000); 
 	 	exit(0); /* ���� 0 ֵ��ʾ���������������δ��װ */ 
 	} 
 	regs.x.ax=7; 
 	regs.x.cx=xmi; 
 
 	regs.x.dx=xma; 
 	int86(51,&regs,&regs); 
 	regs.x.ax=8; 
 	regs.x.cx=ymi; 
 	regs.x.dx=yma; 
 	int86(51,&regs,&regs); /* ��ʾ����������������Ѱ�װ */ 
 	return retcode; 
} 
 
int read(int *mx,int *my,int *mbutt) /* ������λ�úͰ�ť״̬���� */ 
{ 
 	int xx0=*mx,yy0=*my,but0=0,mb; 
 	int xnew,ynew; 
 	do 
 	{ 
 	 	regs.x.ax=3; 
 	 	int86(51,&regs,&regs); 
 	 	xnew=regs.x.cx; 
 	 	ynew=regs.x.dx; 
 	 	*mbutt=regs.x.bx; 
 	} while(xnew==xx0 && ynew==yy0 && *mbutt==but0); 
 	*mx=xnew; 
 	*my=ynew; 
 	mb=(*mbutt); 
 	if(mb) 
 	{ 
 	 	if(mb==1) return 1;  /* ������� */ 
 	 	if(mb==2) return 2;  /* �Ҽ����� */ 
 	 	return 3 ; /* �� �� �� �� �� */ 
 	} 
 	else 
 	{ 
 	 	return 0 ; 
 	} 
} 
void newxy(int *mx,int *my,int *mbutt)/* �Ƿ��а�ť����,������,����λ�û�ʮ��*/ 
{ 
 	int ch,xx0=*mx,yy0=*my ,x,y; 
 	int xm,ym; 
 	ch=read(&xm,&ym,mbutt); 
 	switch(ch) 
 	{ 
 	 	case 0: 
 	 	 	cursor(xx0,yy0); 
 
 	 	 	cursor(xm,ym); 
 	 	 	break; 
 	 	case 1: 
 	 	 	cursor(xx0,yy0); 
 	 	 	cursor(xm,ym); 
 	 	 	//circle(xm,ym,6); 
 	 	 	break; 
 	 	/*case 2: 
 	 	 	cursor(xx0,yy0); 
 	 	 	cursor(xm,ym); 
 	 	 	rectangle(xm,ym,xm+12,ym+12); 
 	 	 	break; 
 	 	case 3: 
 	 	 	cursor(xx0,yy0); 
 	 	 	cursor(xm,ym); 
 	 	 	putpixel(xm,ym,7); 
 	 	 	break;*/ 
 	} 	
 	*mx=xm; 
 	*my=ym; 
}  
 
void out_hz(int x,int y,char *s,int size,int part,int color) //������� 
{ 
 	FILE *hzk_p; 
 	unsigned char quma,weima; 
 	unsigned long offset; 
 	unsigned char mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; 
 	int i,j,pos; 
 	switch(size) 
 	{ 
 	 	case 12: //12 �� �� �� �� �� 
 	 	{ 	 
 	 	 	char mat[24]; 
 	 	 	int y0=y; 
 	 	 	hzk_p=fopen("hzk\\hzk12","rb"); 
 	 	 	if(hzk_p==NULL) 
 	 	 	{ 
 	 	 	 	outtextxy(10,10,"Can not open file! Press any key to return"); 
 	 	 	 	getch(); 
 	 	 	 	exit(1); 
 	 	 	} 
 	 	 	while(*s!=NULL) 
 	 	 	{ 
 
 	 	 	y=y0; 
 	 	 	quma=s[0]-0xa0; 
 	 	 	weima=s[1]-0xa0; 
 	 	 	offset=(94*(quma-1)+(weima-1))*24L; 
 	 	 	fseek(hzk_p,offset,SEEK_SET); 
 	 	 	fread(mat,24,1,hzk_p); 
 	 	 	for(i=0;i<12;i++) 
 	 	 	{ 
 	 	 	 	pos=2*i; 
 	 	 	 	for(j=0;j<12;j++) 
 	 	 	 	{ 
 	 	 	 	 	if((mask[j%8]&mat[pos+j/8])!=NULL) 
 	 	 	 	 	{ 
 	 	 	 	 	 	putpixel(x+j,y,color); 
 	 	 	 	 	} 
 	 	 	 	} 
 	 	 	 	y++; 
 	 	 	} 
 	 	 	x+=part; 
 	 	 	s+=2; 
 	 	} 
 	 	fclose(hzk_p); 
 	 	break; 
 	 	} 
 	 	case 16: //16 �� �� �� �� �� 
 	 	{ 
 	 	 	char mat[32]; 
 	 	 	int y0=y; 
 	 	 	hzk_p=fopen("hzk\\hzk16","rb"); 
 	 	 	if(hzk_p==NULL) 
 	 	 	{ 
 	 	 	 	outtextxy(10,10,"Can not open file! Press any key to return"); 
 	 	 	 	getch(); 
 	 	 	 	exit(1); 
 	 	 	} 
 	 	 	while(*s!=NULL) 
 	 	 	{ 
 	 	 	 	y=y0; 
 	 	 	 	quma=s[0]-0xa0; 
 	 	 	 	weima=s[1]-0xa0; 
 	 	 	 	offset=(94*(quma-1)+(weima-1))*32L; 
 	 	 	 	fseek(hzk_p,offset,SEEK_SET); 
 	 	 	 	fread(mat,32,1,hzk_p); 
 	 	 	 	for(i=0;i<16;i++) 
 
 	 	 	 	{ 
 	 	 	 	 	pos=2*i; 
 	 	 	 	 	for(j=0;j<16;j++) 
 	 	 	 	 	{ 
 	 	 	 	 	 	if((mask[j%8]&mat[pos+j/8])!=NULL) 
 	 	 	 	 	 	{ 
 	 	 	 	 	 	 	putpixel(x+j,y,color); 
 	 	 	 	 	 	} 
 	 	 	 	 	} 
 	 	 	 	 	y++; 
 	 	 	 	} 
 	 	 	 	x+=part; 
 	 	 	 	s+=2; 
 	 	 	} 
 	 	 	fclose(hzk_p); 
 	 	 	break; 
 	 	} 
 	 	case 24: //24 �� �� �� �� �� 
 	 	{ 
 	 	 	char mat[72]; 
 	 	 	int y0=y; 
 	 	 	hzk_p=fopen("hzk\\HZK24","rb"); 
 	 	 	if(hzk_p==NULL) 
 	 	 	{ 
 	 	 	 	outtextxy(10,10,"Can not open file! Press any key to return"); 
 	 	 	 	getch(); 
 	 	 	 	exit(1); 
 	 	 	} 
 	 	 	while(*s!=NULL) 
 	 	 	{ 
 	 	 	 	y=y0; 
 	 	 	 	quma=s[0]-0xa0; 
 	 	 	 	weima=s[1]-0xa0; 
 	 	 	 	offset=(94*(quma-1-15)+(weima-1))*72L; 
 	 	 	 	fseek(hzk_p,offset,SEEK_SET); 
 	 	 	 	fread(mat,72,1,hzk_p); 
 	 	 	 	for(i=0;i<24;i++) 
 	 	 	 	{ 
 	 	 	 	 	pos=3*i; 
 	 	 	 	 	for(j=0;j<24;j++) 
 	 	 	 	 	{ 
 	 	 	 	 	 	if((mask[j%8]&mat[pos+j/8])!=NULL) 
 	 	 	 	 	 	{ 
 	 	 	 	 	 	 	putpixel(x,y+j,color); 
 
 	 	 	 	 	 	} 
 	 	 	 	 	} 
 	 	 	 	 	x++; 
 	 	 	 	} 
 	 	 	 	x+=part; 
 	 	 	 	s+=2; 
 	 	 	} 
 	 	 	fclose(hzk_p); 
 	 	 	break; 
 	 	} 
 	 	case 48: //48 �� �� �� �� �� 
 	 	{ 
 	 	 	char mat[288]; 
 	 	 	int y0=y; 
 	 	 	hzk_p=fopen("hzk\\HZK48s","rb"); 
 	 	 	if(hzk_p==NULL) 
 	 	 	{ 
 	 	 	 	outtextxy(10,10,"Can not open file! Press any key to return"); 
 	 	 	 	getch(); 
 	 	 	 	exit(1); 
 	 	 	} 
 	 	 	while(*s!=NULL) 
 	 	 	{ 
 	 	 	 	y=y0; 
 	 	 	 	quma=s[0]-0xa0; 
 	 	 	 	weima=s[1]-0xa0; 
 	 	 	 	offset=(94*(quma-1-15)+(weima-1))*288L; 
 	 	 	 	fseek(hzk_p,offset,SEEK_SET); 
 	 	 	 	fread(mat,288,1,hzk_p); 
 	 	 	 	for(i=0;i<48;i++) 
 	 	 	 	{ 
 	 	 	 	 	pos=6*i; 
 	 	 	 	 	for(j=0;j<48;j++) 
 	 	 	 	 	{ 
 	 	 	 	 	 	if((mask[j%8]&mat[pos+j/8])!=NULL) 
 	 	 	 	 	 	{ 
 	 	 	 	 	 	 	putpixel(x+j,y,color); 
 	 	 	 	 	 	} 
 	 	 	 	 	} 
 	 	 	 	 	y++; 
 	 	 	 	} 
 	 	 	 	x+=part; 
 	 	 	 	s+=2; 
