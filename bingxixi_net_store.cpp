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
typedef struct node1 //用户结构体 
{ 
 	char name[12]; // 用 户 名 
 	char mima[12]; // 密 码   
 	char zhifumima[12];//支付密码 
 	char phone[12];    //手机号码 
 	char address[12]; //地址 
 	float money; // 账 户 余 额 
 	struct node1 *next; 
}yonghu; 
 
typedef struct node2 //管理员结构体 
{ 
 	char name[12]; 
 	char mima[12]; 
 	struct node2 *next; 
}guanli; 
 
typedef struct node3 //商品结构体 
{ 
 
 	int life; // 用 以 判 断 是 否 还 在 销 售 
 	int flag;   
 	int cla;  //用以判断用户是否已付款 
 	char bianhao[10]; 
 	int kucun; // 库 存 量 
 	int xiaoshou; // 销 售 量 
 	long t;             // 用 户 购 买 商 品 的 时 间float chengben; // 商 品 成 本 
 	float price; // 价 格 
 	float  profit;               // 利 润char name[Max_name_length]; 
 	char name_pinyin[Max_name_pinyin]; //商品名的拼音，用于搜索 
 	char miaoshu[Max_miaoshu_length]; // 商 品 描 述                
 	struct node3 *next; 
}shangpin; 
 
typedef struct BANK_CARD //银行卡结构体 
{ 
 	char count[12]; 
 	char mima[12]; 
 	float money; // 银 行 余 额 
 	struct BANK_CARD *next; 
}bank; 
 
typedef struct Liuyan //留言结构体 
{ 
 	int flag; //0 表示用户给店铺的留言 
 	int life; //0 表示已读，1 表示未读 
 	char yonghuname[12]; //用户名 
 	char xinxi[80]; 
 	struct Liuyan *next; 
}liuyan; 
 
typedef struct Dingdan //订单结构体 
{ 
 	int life; //标记是否被处理的订单 
 	char bianhao[10];//商品编号 
 	char name[12]; //用户名 
 	char phone[12]; //用户手机号 
 	char address[12];//用户地址 
 	struct Dingdan *next; 
}dingdan; 
 
 
 
/************************* main.c ****************************/ 
 
void main() 
{ 
   int driver=VGA; //用于图形界面初始化 
   int mode=VGAHI; 
   int mx,my,mbutt; // 鼠 标 
   int iden=0; // 身 份 标 记 
   char str_name[12],str_mima[12]; //用户名和密码 
   yonghu *current; //指向当前用户 
   yonghu *yh_head=yhcreat();  //创建用户链表 
   guanli *gl_head=glcreat();  //创建管理员链表 
   shangpin *sp_head=spcreat();  //创建商品链表 
   liuyan*ly_head=lycreat();     //创建留言链表 
   str_name[0]='\0'; 
   str_mima[0]='\0'; 
   initgraph(&driver,&mode,"c:\\borlandc\\bgi"); 
   init_mouse(0,639,0,479); //鼠标初始化 
   mbutt=0; 
   for(;;) 
   { 
      dljiemian(); // 登 陆 界 面 绘 制 
 	  for(;;) 
 	  { 
 	     mbutt=0; 
 	     delay(20); 
 	 	 newxy(&mx,&my,&mbutt); 
 	 	 dl_xuanze(&mx,&my,&mbutt,&iden);       // 选 择 登 录 身 份 
 	 	 dl_input(&mx,&my,&mbutt,str_name,str_mima); //输入登录信息 
 	 	 zhuce(&mx,&my,&mbutt,yh_head,gl_head); // 注 册 函 数 
 	 	 if(dl_yanzheng(&mx,&my,&mbutt,str_name,str_mima,iden)==1) // 检验登录信息是否填写完全！ 
 	 	 { 
 	 	 	 if((xinxi_yz(iden,str_name,str_mima,yh_head,gl_head,&current,&mx,&my,&mbutt))== 1) 
 	 	 	 { // 检 验 登 录 信 息 是 否 正 确 
 	 	 	  	str_name[0]='\0'; 
                str_mima[0]='\0'; 
                delay(500); 
 	 	 	  	break; 
 	 	 	 }       
 	 	 } 
 	  } 
 
    if(iden==1) //登录身份为 1 时，进入用户模块 
 	      customer(&mx,&my,&mbutt,current,yh_head,sp_head,ly_head); 
    if(iden==2) // 进 入 管 理 员 模 块 
    { 
          dingdan*dd_head=ddcreat();        //创建订单链表 
          manager(&mx,&my,&mbutt,ly_head,sp_head,dd_head); 
 	} 
  } 
} 
yonghu * yhcreat(void) // 创 建 用 户 链 表 
{ 
   FILE *fp; 
   int size; 
   char s[30]; 
   yonghu *head,*p1,*p2; //head 指向链表头节点，p1 指向当前节点，p2 指向上一节点 
   size=sizeof(yonghu); 
   head=(yonghu*)malloc(size); 
   if(head==NULL) 
   { 
 	  printf("yonghu creat:no enough memory"); 
 	  exit(1); 
   } 
   if((fp=fopen("txt\\yonghu.txt","rt"))==NULL) //只读模式打开文件 
   { 
 	 printf("unable to open yonghu.txt"); 
 	 exit(0); 
   } 
   p1=p2=head; 
   rewind(fp); 
   
fscanf(fp,"%s %s %s %s %s %s\n",head->name,head->mima,head->zhifumima,head->p hone,head->address,s); //格式化方式读入文件信息 
   head->money=atof(s); //先以字符串形式读入再转换成浮点数 
   head->next=NULL; 
   while(!feof(fp)) 
   { 
 	  if( ( p1=(yonghu*)malloc(sizeof(yonghu))) ==NULL)//分配动态内存并检测 
 	  { 
 	 	printf("No enough memory"); 
 	 	exit(0); 
 	  } 
 	   
fscanf(fp,"%s %s %s %s %s %s\n",p1->name,p1->mima,p1->zhifumima,p1->phone,p1->a ddress,s); 
 
 	   p1->money=atof(s); 
 	   p2->next=p1; 把 p1 的地址赋给 p2 所指向的结点的链域，这样就把 p1 和 p2 所指向的结点连接起来了 
 	   p1->next=NULL; 
 	   p2=p1; 
   } 
   fclose(fp); 
   return head; 
} 
 
guanli *glcreat() //管理员链表创建，原理同用户链表的创建 
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
 	char str[20];                //sh 指向链表头节点，s1 指向当前节点，s2 指向上一节点 
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
liuyan *lycreat(void) //留言链表创建函数 
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
dingdan *ddcreat() //订单链表创建函数 
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
void tuichu(int *mx,int *my,int *mbutt) // 退 出 弹 出 框 
{ 
 	  void *buffer; 
 
      unsigned size; 
      size=imagesize(170,140,470,340); //检测屏幕区域的大小 
      if(((void*)buffer=malloc(size))==NULL)//分配内存空间并检测 
      { 
         printf("No enough memory"); 
         exit(1); // 分 配 失 败 即 退 出 
      } 
      getimage(170,140,470,340,buffer); //获取屏幕截图 
      setfillstyle(1,WHITE); // 弹 出 框 绘 制 
      bar(170,140,470,340); 
      setfillstyle(1,DARKGRAY); 
      bar(170,140,470,160); 
      setfillstyle(1,LIGHTGRAY); 
      bar(190,310,230,330); 
      bar(410,310,450,330); 
      out_hz(260,230,"确定退出吗",24,0,BLACK); 
      out_hz(194,312,"取消",16,16,BLACK); 
      out_hz(414,312,"确认",16,16,BLACK); 
      for(;;) 
      { 
        newxy(mx,my,mbutt); 
 	    if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //点击取消时 
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
 
void dl_xuanze(int *mx,int *my,int *mbutt,int *iden) //登录身份选择 
{  
   if(*mx>547&&*mx<585&&*my>427&&*my<449&&*mbutt==1) // 点 击 退 出 
      tuichu(mx,my,mbutt);    
   if(*mx>205&&*mx<215&&*my>185&&*my<195&&*mbutt==1) //选择客户登录 
   { 
      yh_dl_xz();            
      *iden=1; 
   } 
   else if(*mx>310&&*mx<320&&*my>185&&*my<195&&*mbutt==1) //选择管理员登录 
   { 
      gl_dl_xz(); 
      *iden=2; 
   } 
} 
 
void dl_input(int *mx,int *my,int *mbutt,char *str_name,char *str_mima) 
{ 
   if(*mx>252&&*mx<425&&*my>210&&*my<230&&*mbutt==1) // 输 入 帐 号 
   { 
      setfillstyle(1,WHITE); 
      bar(250,210,425,230);               
 	  input_name(str_name,254,212,12); 
   } 
   if(*mx>252&&*mx<425&&*my>250&&*my<270&&*mbutt==1) // 输 入 密 码 
   { 
      setfillstyle(1,WHITE); 
      bar(250,250,425,270); 
      input_mima(str_mima,254,252); 
   } 
} 
 
int dl_yanzheng(int *mx,int *my,int *mbutt,char*str_name,char*str_mima,int iden) 
//检验登入信息是否完整 
{ 
 	if(*mx>320&&*mx<360&&*my>315&&*my<335&&*mbutt==1) // 点 击 登 录 
    { 
      if(!(iden==1||iden==2)) // 没 有 选 择 身 份 时 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"请选择身份！"); 
 	 	return 0; 
      } 
 
      else if(*str_name=='\0') //没有输入帐号时 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"请输入帐号！"); 
 	 	return 0; 
      } 
      else if(*str_mima=='\0') //没有输入密码时 
      { 
 	 	outinformation(mx,my,mbutt,200,200,"请输入密码！"); 
 	 	return 0; 
 	  } 
 	  else     
 	    return 1; //确认信息填写完整，函数返回值为 1，否则为 0.   
    } 
} 
 
int xinxi_yz(int iden,char *str_name,char*str_mima,yonghu*yh,guanli*gh,yonghu 
**cu,int *mx,int *my,int *mbutt) // 验 证 登 录 信 息 
{ 
 	yonghu*p; 
 	guanli*q; 
 	switch(iden)                     
 	{ 
 	 	case 1: // 是 用 户 登 录 时 
 	 	 	if( (p=yh_name_yz(str_name,yh))==NULL) //在链表中没有找到用户名时 
 	 	      { 
 	 	         	outinformation(mx,my,mbutt,200,200,"帐号不存在！"); 
 	 	         	return 0; 
 	 	      } 
 	 	 	else 
 	 	        { 
 	 	 	 	    if(yh_mima_yz(str_mima,p)==0) //检测到密码错误时 
 	 	         	{ 
 	 	         	 	outinformation(mx,my,mbutt,200,200,"密码错误！"); 
 	 	         	 	return 0; 
 	 	         	} 
 	 	         	Else // 验 证 通 过 
 	 	         	{ 
                        *cu=p; 把当前登录用户的指针赋给 cu 
                        return 1; 
 	 	 	 	 	} 
                } 
 	 	        break; 
 	 	case 2:  
 	 	        if((q=gl_name_yz(str_name,gh))==NULL)//在链表中没有找到管理员时 
 
 	 	        { 
 	 	         	outinformation(mx,my,mbutt,200,200,"帐号不存在！"); 
 	 	         	return 0; 
 	 	        } 
 	 	 	 	else 
 	 	        { 
 	 	 	 	 	if(gl_mima_yz(str_mima,q)==0) //输入密码错误时 
 	 	         	{ 
 	 	         	 	outinformation(mx,my,mbutt,200,200,"密码错误！"); 
 	 	         	 	return 0; 
 	 	         	} 
 	 	 	 	 	if(gl_mima_yz(str_mima,q)==1) //登入信息通过时 
                        return 1; 
 	 	        } 
 	 	 	 	break; 
 	} 
} 
 
void yh_dl_xz() //选择用户身份时把用户圈圈涂黑，把管理员圈圈涂白 
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
void gl_dl_xz() //选择管理员身份时把用户圈圈涂黑，把用户圈圈涂白 
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
 
yonghu* yh_name_yz(char *str1,yonghu *p) //用户登录帐号验证函数 
{ 
   while(p!=NULL) 
 
   { 
 	 if(strcmp(p->name,str1)==0) //检测到相应用户是退出循环 
 	  	break;  
 	 else 
 	  	p=p->next; 
   } 
   return p; //找到对应用户，返回 yonghu 指针 P；否则返回空指针 
} 
int yh_mima_yz(char*str2,yonghu *p) 
{ 
 	 	if(strcmp(p->mima,str2)==0) //如果密码验证成功,函数返回值为 1；否则返回0.   	 
 	 	   return 1; 	 	 
 	 	else 	 	 
 	 	  return 0;  	 
} 
  	 
guanli* gl_name_yz(char *str1,guanli *p) //管理员帐号验证 
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
int gl_mima_yz(char *str2,guanli *p) //管理员密码验证 
{ 
 	 	if(strcmp(p->mima,str2)==0)  //如果密码验证成功,函数返回值为 1；否则返回 0. 	 	 
 	 	  return 1;  	 
 	 	else 
 	 	  return 0; 
}  
 
void dljiemian() //登录界面绘制 
{ 
  cleardevice(); 
  int triangle1[]={150,160,460,160,460,70}; //三角形坐标的数组 
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
  out_hz(204,210,"用户名",16,16,BLACK); 
  out_hz(204,250,"密码",16,26,BLACK); 
  out_hz(325,317,"登录",16,16,WHITE); 
  out_hz(375,317,"注册",16,16,WHITE); 
  out_hz(550,430,"退出",16,16,WHITE); 
  setcolor(BLACK); 
  circle(210,190,5); 
  outtextxy(220,187,"Customer"); 
  circle(315,190,5); 
  outtextxy(325,187,"Administrator"); 
  setlinestyle(0,0,3); 
  setcolor(WHITE); 
  rectangle(547,427,585,449); 
  timeoutput(420,20); 
 
/**********************************zhuce.c********************************/ void zhuce(int *mx,int *my,int *mbutt,yonghu *yh,guanli *gh)//注册主函数 
{ 
   if(*mx>370&&*mx<410&&*my>315&&*my<335&&*mbutt==1) //点击登录界面上的的注册 
   {                                     
 	  zhuce_menu(); // 注 册 菜 单 绘 制 
 	  for(;;) 
 	  { 
 	     newxy(mx,my,mbutt); 
 	     if(*mx>370&&*mx<450&&*my>335&&*my<355&&*mbutt==1)//点击用户注册 
 	     { 
 	      	yh_zhuce_jm();                                        // 用 户 注册界面绘制 
 	      	if(yh_zhuce(mx,my,mbutt,yh)==1) //用户注册成功时返回登录界面 
 	      	{ 
 	      	 	dljiemian(); 
 	      	 	break; 
 	      	} 
 	     } 
 	     if(*mx>370&&*mx<450&&*my>355&&*my<375&&*mbutt==1)//点击管理员注册 
 	     { 
 	      	gl_zhuce_jm(); 
 	      	if(gl_zhuce(mx,my,mbutt,gh)==1) //管理员注册成功返回登录界面 
 	      	{ 
 	      	 	dljiemian(); 
 	      	 	break; 
 	      	} 
 	 	 } 
 	  } 
   } 
} 
 
void zhuce_menu()  //注册菜单绘制 
{ 
 	setcolor(BLACK); 
 	setfillstyle(1,WHITE); 
 	bar(370,335,450,355); 
 	bar(370,355,450,375); 
 	rectangle(370,335,450,355); 
 	rectangle(370,355,450,375); 
 	setcolor(BLACK); 
 	out_hz(370,336,"新用户注册",16,16,BLACK); 
 	out_hz(370,356,"管理员注册",16,16,BLACK); 
 
void yh_zhuce_jm() //用户注册界面绘制 
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
   out_hz(280,105,"新用户注册",16,20,BLACK); 
   out_hz(220,155,"用户名",16,25,BLACK); 
   out_hz(x+11,y+41,"密码",16,26,BLACK); 
   out_hz(x+11,y+80,"手机",16,26,BLACK); 
   out_hz(x+11,y+120,"地址",16,26,BLACK); 
   out_hz(x,y+160,"支付密码",16,16,BLACK); 
   out_hz(383,350,"确认",16,20,BLACK); 
   setcolor(WHITE); 
   circle(30,30,12); 
   setlinestyle(0,0,3); 
   line(20,30,32,25); 
   line(20,30,32,35); 
   line(20,30,40,30); 
   setlinestyle(0,0,1); 
   timeoutput(420,20); 
} 
int yh_zhuce(int *mx,int *my,int *mbutt,yonghu *yh) //用户注册函数 
{ 
 	int i,n=0,a[5]; //数组用于标记每一项信息填写的状态，填写完成值为 1 
 	char shuzu[5][12];//字符数组用于接收用户输入的信息 
 	for(i=0;i<5;i++) 
 	 	a[i]=0; // 数 组 a[5] 的 初 始 化 
 	for(;;) 
 
 	{ 	
 	 	n=a[0]+a[1]+a[2]+a[3]+a[4]; //n 为数组 a 五项元素的和 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>18&&*mx<42&&*my>18&&*my<42&&*mbutt==1) //点击返回按钮时 
 	 	 	break; 
 	 	if(*mx>284&&*mx<420&&*my>153&&*my<173&&*mbutt==1)//点击用户名输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,153,420,173); 
 	 	 	input_name(shuzu[0],286,155,12); //输入用户名，存储到数组[0] 
 	 	 	if(shuzu[0][0]!='\0') //确认用户名填写完成，a[0]赋值 1 
 	 	 	 	a[0]=1;  	     
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>193&&*my<213&&*mbutt==1) //点击密码输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,193,420,213); 
 	 	 	input_name(shuzu[1],286,195,12); //密码存储到 shuzu[1] 
 	 	 	if(shuzu[1][0]!='\0') //确认密码填写完成，a[1]赋值 1 
 	 	 	    a[1]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>233&&*my<253&&*mbutt==1) //点击输入手机号 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,233,420,253); 
 	 	 	input_name(shuzu[2],286,235,12); //手机号存储到 shuzu[2] 
 	 	 	if(shuzu[2][0]!='\0') //确认手机号填写完成，a[2]赋值 1 
 	 	 	    a[2]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>273&&*my<293&&*mbutt==1) //点击地址输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,273,420,293); 
 	 	 	input_name(shuzu[3],286,275,12); //地址存储到 shuzu[3], 
 	 	 	if(shuzu[3][0]!='\0') //确认地址填写完成，a[3]赋值 1   	    
		 a[3]=1; 
 	 	} 
 	 	if(*mx>284&&*mx<420&&*my>313&&*my<333&&*mbutt==1) //点击支付密码输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(284,313,420,333); 
 	 	 	input_name(shuzu[4],286,315,12);//支付密码存储到 shuzu[4] 
 	 	 	if(shuzu[4][0]!='\0') //确认支付密码填写完成，a[4]赋值 1 
 
 	 	 	    a[4]=1; 
 	 	} 
 	 	if(*mx>380&&*mx<420&&*my>350&&*my<370&&*mbutt==1) //点击确认 
 	 	{ 
 	 	 	if(n!=5) //信息填写不完全 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"信息不完全！"); 
 	 	 	else 
 	 	 	{ 
 	 	 	 	if(yh_name_yz(shuzu[0],yh)!=NULL)//检测用户名是否已存在  	 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"帐户已存在！");  
 	 	
 	 	 	 	else //检测到用户名可用时，将新用户信息写入文件 
 	 	 	 	{ 
 	 	 	 	 	yh_xieru(yh,shuzu);   
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"注册成功！"); 
 	 	 	 	    break; //验证与写入,这里写一个写文件的函数 
 	 	 	 	} 
 	 	 	} 
 	 	} 
 		
 	} 
 	return 1; // 注 册 成 功 ， 函 数 值 返 回 1 
} 
void yh_xieru(yonghu*h,char c[][12]) //将新用户写入文件的函数 
{ 
 	FILE *fp; 
 	yonghu *p,*q; 
    char s[30]="txt\\";//s 用于表示新用户的专属 txt 文件路径 
 	p=h; 
 	if((q=(yonghu *)malloc(sizeof(yonghu)))==NULL)//分配空间并检测 
 	{ 
 	 	printf("No enough memory!"); 
 	 	exit(1); 
 	} 
 	strcpy(q->name,c[0]); //将用户信息写入新空间中 
 	strcpy(q->mima,c[1]); 
 	strcpy(q->phone,c[2]); 
 	strcpy(q->address,c[3]); 
 	strcpy(q->zhifumima,c[4]); 
 	q->money=0; // 新 用 户 余 额 赋 0 strcat(s,c[0]);  
 	strcat(s,".txt"); //将 s 连接成"txt\\yonghuname.txt"的形式 
 	while(p->next!=NULL) //使 p 指向用户链表最后一个节点 
 	 	p=p->next; 
 
 	p->next=q; // 使 p 指 向 q 
 	q->next=NULL; 
 	p=h; //重新将用户头指针赋予 p 
 	if( (fp=fopen("txt\\yonghu.txt","wt"))==NULL ) //打开 yonghu.txt 
    { 
     	printf("unable to open yonghu.txt"); 
     	exit(1); 
    } 
while(p!=NULL) // 将 链 表 写 入 文 件 
{     
fprintf(fp,"%s %s %s %s %s %f\n",p->name,p->mima,p->zhifumima,p->phone, p->address,p->money); 
     	p=p->next; 
    } 
    fclose(fp); 
    if( (fp=fopen(s,"wt"))==NULL) //为用户新建一个文本文件 
    { 
     	printf("unable to open txt when build Customer list"); 
     	exit(1); 
    } 
    fclose(fp); 
} 
void gl_zhuce_jm() //管理员注册界面绘制 
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
   out_hz(280,153,"管理员注册",16,16,BLACK); 
   out_hz(231,191,"账号",16,20,BLACK); 
   out_hz(231,231,"密码",16,20,BLACK); 
 
   out_hz(220,271,"序列号",16,16,BLACK); 
   out_hz(385,306,"确认",16,20,BLACK); 
   setcolor(WHITE); 
   circle(30,30,12); 
   setlinestyle(0,0,3); 
   line(20,30,32,25); 
   line(20,30,32,35); 
   line(20,30,40,30); 
   setlinestyle(0,0,1); 
   timeoutput(420,20); 
} 
int gl_zhuce(int *mx,int *my,int *mbutt,guanli *gh) //管理员注册函数 
{ 
 	char shuzu[3][12]; //字符数组 shuzu 用于接收输入信息 
 	int n=0,a[3]; 
 	a[0]=a[1]=a[2]=0; 
 	for(;;) 
 	{ 
 	 	n=a[0]+a[1]+a[2]; 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>18&&*mx<42&&*my>18&&*my<42&&*mbutt==1) //点击返回 
 	 	 	break; 
 	 	if(*mx>270&&*mx<420&&*my>190&&*my<210&&*mbutt==1) //点击名字输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,190,420,210); 
 	 	 	input_name(shuzu[0],272,192,12); 
 	 	 	if(shuzu[0][0]!='\0') 
 	 	 	    a[0]=1; 
 	 	}  
 	 	if(*mx>270&&*mx<420&&*my>230&&*my<250&&*mbutt==1) //点击密码输入框 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,230,420,250); 
 	 	 	input_name(shuzu[1],272,232,12); 
 	 	 	if(shuzu[1][0]!='\0') 
 	 	 	    a[1]=1; 
 	 	} 
 	 	if(*mx>270&&*mx<420&&*my>270&&*my<290&&*mbutt==1) //点击输入序列号 
 	 	{ 
 	 	 	setfillstyle(1,WHITE); 
 	 	 	bar(270,270,420,290); 
            input_name(shuzu[2],272,272,12); 
 
            if(shuzu[2][0]!='\0') 
                a[2]=1; 
 	 	} 
 	 	if(*mx>383&&*mx<423&&*my>305&&*my<325&&*mbutt==1) //点击确认 
 	 	{ 
 	 	 	if(n!=3) //n!=3, 表 示 信 息 填 写 不 全 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"信息不完全！"); 
 	 	 	else 
 	 	 	{ 
 	 	 	 	if(gl_name_yz(shuzu[0],gh)!=NULL)//检测到账户名已存在时 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"帐户已存在！"); 
 	 	 	 	else  
 	 	 	 	{ 
 	 	 	 	 	if(strcmp(shuzu[2],"zidonghua")!=0) //序列号匹配失败 
 	 	 	 	 	   outinformation(mx,my,mbutt,210,200,"序列号错误"); 
 	 	 	 	 	else 
 	 	 	 	    { 
 	 	 	 	 	   gl_xieru(gh,shuzu); //将新管理员信息写入文件 
 	 	 	 	 	   outinformation(mx,my,mbutt,210,200,"注册成功！"); 
 	 	 	 	 	   break;				} 
 	 	 	 	} 
 	 	 	} 	
 	 	} 	 	         
 	} 	
 	return 1; 	
} 		
 		
void gl_xieru(guanli*h,char c[][12]) //管理员写入函数 
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
{ // 用 户 模 块 主 函 数 
   int n=7; 
   shangpin*th=truck_creat(cu); //购物车链表的创建 
   yonghu_jm();                    // 用 户 界 面 绘 制 
   huanying();                     // 欢 迎 动 画 显 示 
   cursor(*mx,*my); 
   for(;;) 
   { 
       delay(20); 
       newxy(mx,my,mbutt); 
       if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) //点击退出 
              exit(0)； 
       else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) //个人信息 
 	 	 	  n=personal_info(mx,my,mbutt,cu,yh); 
       else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) //购物车 
              n=buy_truck(mx,my,mbutt,cu,yh,th);           
       else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt)//消费记录              
              n=consume(mx,my,mbutt,th);              
       else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt)   // 搜索 
              n=search(mx,my,mbutt,sh,cu,th);         
       else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt)   // 支付宝 
 	 	 	  n=zhifubao(mx,my,mbutt,cu,yh);             
       else if(*mx>=38 && *mx<87 && *my>=456 && *my<=479 && *mbutt)//留言功能  
              n=gukeliuyan(mx,my,mbutt,cu,lh); 
       else if(*mx>=87 && *mx<125 && *my>=456 && *my<=490 && *mbutt //点击返回 
             n=cust_fun(0,mx,my,mbutt,cu,yh,sh,th,lh); 
       if(n==0) //函数值 n 为 0，返回登录界面 
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
 	      personal_info(mx,my,mbutt,cu,yh); //个人信息 
 	      return 1; 
    case 2: 
          buy_truck(mx,my,mbutt,cu,yh,th); //购物车管理 
          return 2; 
    case 3: 
          consume(mx,my,mbutt,th); // 消 费 记 录 
          return 3; 
    case 4: 
 	      search(mx,my,mbutt,sh,cu,th); //搜索商品 
          return 4; 
    case 5: 
 	      zhifubao(mx,my,mbutt,cu,yh); //支付宝充值 
          return 5; 
    case 6: 
          gukeliuyan(mx,my,mbutt,cu,lh); //留言功能 
          return 6; 
    case 0: 
          return 0; 
  } 
} 
shangpin *truck_creat(yonghu *cu) //购物车链表的创建 
{ 
 	FILE *fp; 
    char ch; 
 	char c1[30]="txt\\"; 
 	char c2[10]=".txt";           //用于生成路径"txt\\yonghuname.txt" 
 	shangpin *sh,*s1,*s2;  
 	strcat(c1,cu->name); 
 	strcat(c1,c2);    
 	char str[20];                //sh 指向链表头节点，s1 指向当前节点，s2 指向上一节点 
 	char str1[20]; //文件信息以字符串格式读入，再转换成浮点数 
 	char str2[20]; 
 	char str3[20]; 
 	if((sh=(shangpin*)malloc(sizeof(shangpin)))==NULL) 
 	{ 
 	 	printf("No enough memory when build truck list\n"); 
 	 	exit(1); 
 	} 
  sh->next=NULL; //sh 头指针，指向空域 
 
  s1=s2=sh; if((fp=fopen(c1,"rt"))==NULL) 
{ 
 	 	printf("unable to open shangpin.txt"); 
 	 	exit(1); 
} 
  ch=fgetc(fp); 
  if(ch==EOF) //如果文件为空，不用再从文件内读入数据 
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
 
int menu_back(int *mx,int*my,int*mbutt) //返回主菜单功能函数 
{ 
    if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt) //点击退出 
      exit(0); 
    else if(*mx>=12 && *mx<112 && *my>=25 && *my<=69 && *mbutt) //个人信息 
      return 1; 
    else if(*mx>=12 && *mx<112 && *my>=100 && *my<=69+75 && *mbutt) //购物车 
      return 2; 
    else if(*mx>=12 && *mx<112 && *my>=175 && *my<=69+150 && *mbutt) //消费记录 
 
      return 3; 
    else if(*mx>=12 && *mx<112 && *my>=250 && *my<=69+225 && *mbutt) //搜索商品 
      return 4; 
    else if(*mx>=12 && *mx<112 && *my>=325 && *my<=69+300 && *mbutt) //支付宝 
      return 5; 
    else if(*mx>=38 && *mx<87 && *my>=456 && *my<=479 && *mbutt) //留言 
      return 6; 
    else if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt) //返回 
      return 0; 
} 
 
void yonghu_jm()  //用户主界面 
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
   out_hz(27,a[0],"个人信息",16,17,WHITE); 
   out_hz(36,a[1],"购物车",16,19,WHITE); 
   out_hz(27,a[2],"消费记录",16,17,WHITE); 
   out_hz(27,a[3],"搜索商品",16,17,WHITE); 
   out_hz(36,a[4],"支付宝",16,19,WHITE); 
   out_hz(2,460,"退出",16,16,WHITE); 
   out_hz(88,460,"返回",16,16,WHITE); 
   out_hz(47,460,"留言",16,16,LIGHTBLUE); 
} 
void huanying() // 欢 迎 动 画 
{ 
 	int i,j; 
  setcolor(WHITE); 
  for(i=0;i<12;i++) 
  { 
 
    delay(200); 
    rectangle(130+20*i,10+19*i,630-20*i,470-18*i); 
  } 
  delay(400); 
  out_hz(356,221,"欢",48,0,WHITE); 
  delay(300); 
  setfillstyle(1,LIGHTBLUE); 
  bar(350,219,410,272); 
  out_hz(356,221,"迎",48,0,WHITE); 
  delay(300); 
  bar(350,219,410,272); 
  out_hz(356,221,"光",48,0,WHITE); 
  delay(300); 
  bar(350,219,410,272); 
  out_hz(356,221,"临",48,0,WHITE); 
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
  out_hz(285,160,"天",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(260,85,"上",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(238+150,250,"天",48,0,LIGHTBLUE); 
  delay(400); 
 
  out_hz(432,353,"下",24,0,LIGHTBLUE); 
  delay(400); 
  out_hz(363+110,125,"购",24,0,LIGHTBLUE); 
  delay(400); 
  out_hz(313+135,185,"物",48,0,LIGHTBLUE); 
  delay(400); 
  out_hz(210+30,210,"网",24,0,LIGHTBLUE); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/******************************customer_infor********************************/ int personal_info(int*mx,int*my,int*mbutt,yonghu*p,yonghu*h) //个人信息模块 
{ 
    personal_info_jm(p); // 个 人 信 息 界 面 绘 制 
    delay(100); 
    for(;;) 
   { 
        newxy(mx,my,mbutt); 
 	    return xiugai(mx,my,mbutt,p,h); 
   } 
} 
int xiugai(int*mx,int*my,int*mbutt,yonghu*p,yonghu*h) //修改用户信息的函数 
{ 
 	char str[4][12];  //二维数组用于接收用户输入的信息 
 	int n=0; 
 	str[0][0]=str[1][0]=str[2][0]=str[3][0]='\0'; 
 	setfillstyle(1,WHITE); 
 	for(;;) 
 	{ 
 	 	delay(20); 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>340&&*mx<482&&*my>193&&*my<213&&*mbutt) //密码修改框 
 	    { 
 	 	     bar(340,195,450,211); 
 	 	     input_name(str[0],346,195,12);  
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>233&&*my<253&&*mbutt)//手机号修改框 
 	    { 	 	      
 	 	     bar(340,235,450,251); 
 	 	     input_name(str[1],346,235,12); 
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>273&&*my<293&&*mbutt)//地址修改栏 
 	    { 
 	 	     bar(340,275,450,291); 
             input_name(str[2],346,275,12); 
 	    } 
 	    else if(*mx>340&&*mx<482&&*my>313&&*my<333&&*mbutt)//支付密码修改栏 
 	    { 
 	 	     bar(340,315,450,331); 
 	 	     input_name(str[3],346,315,12); 
 	    } 
 	    else if(*mx>406&&*mx<470&&*my>385&&*my<405&&*mbutt)//点击确认修改时 
 	    { 
 
 	     	if(str[0][0]!='\0') //确认密码被修改，将修改写入链表 
 	        { 
 	 	         strcpy(p->mima,str[0]); 
 	 	         n=1; 
 	        } 
 	        if(str[1][0]!='\0') //确认手机号被修改 
 	        { 
 	 	         strcpy(p->phone,str[1]); 
 	  	         n=1; 
 	        } 
 	        if(str[2][0]!='\0') //确认地址被修改 
 	        { 
 	 	         strcpy(p->address,str[2]); 
 	 	         n=1; 
 	        } 
 	        if(str[3][0]!='\0') //确认支付密码被修改 
 	        { 
 	 	         strcpy(p->zhifumima,str[3]); 
 	 	         n=1; 
 	        } 
 	        if(n) //n 非零，即有信息被修改时，将修改写入文件 
 	        { 
                 xiugai_xieru(h); 
 	 	 	 	 outinformation(mx,my,mbutt,210,200,"修改成功！"); 
 	        } 
 	         if(n==0) //没有信息被修改时 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"错误的修改方式！"); 
 	    } 
 	    else if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )     
 	     	return menu_back(mx,my,mbutt); // 选择主菜单功能             	                    
 	 } 
} 
 
void xiugai_xieru(yonghu*h) //用户信息修改成功后，将修改后的信息写入文件 
{ 
 	FILE*fp; 
 	if((fp=fopen("txt\\yonghu.txt","wt"))==NULL) //打开 yonghu.txt 
 	{ 
 	 	  printf("un able to open txt when xiugai_xieru"); 
 	 	  exit(1); 
 	} 
 	while(h!=NULL) //将链表重新写入文件 
 	{ 
 	    
 
fprintf(fp,"%s %s %s %s %s %f\n",h->name,h->mima,h->zhifumima,h->phon e,h->address,h->money); 
          h=h->next; 
 	} 
 	fclose(fp); 
} 
 
void personal_info_jm(yonghu *p) //个人信息界面绘制 
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
   out_hz(406,387,"确认修改",16,16,DARKGRAY); 
   out_hz(334,104,"个人信息",16,16,DARKGRAY); 
   out_hz(270,155,"用户名",16,25,DARKGRAY); 
   out_hz(281,y+40,"密码",16,26,DARKGRAY); 
   out_hz(281,y+80,"手机",16,26,DARKGRAY); 
   out_hz(281,y+120,"地址",16,26,DARKGRAY); 
   out_hz(270,y+160,"支付密码",16,16,DARKGRAY); 
   out_hz(281,y+200,"余额",16,26,DARKGRAY); 
   outtextxy(346,159,p->name); 
   outtextxy(346,199,p->mima); 
   outtextxy(346,239,p->phone); 
   outtextxy(346,279,p->address); 
   outtextxy(346,319,p->zhifumima); 
   sprintf(s,"%.2f",p->money); 
   outtextxy(346,359,s); 
   timeoutput(420,20); 
 
} 
/************************** customer_jilu.c *****************************/ int consume(int*mx,int*my,int*mbutt,shangpin*th) //消费记录 
{ 
 int n; //n, 已 购 买 的 商 品 总 数 
   int i=1,k; //i, 页 码 
   shangpin *temp[30]; //指针数组，存储 truck 链表中已购买的商品 
   n=truck_tongji(1,th,temp); //统计已购买的商品数 
   consume_jm(n,temp); //消费记录界面 
   if(n%8==0) 
       k=n/8; 
   else 
       k=n/8+1; //k 总 页 数 
   jl_output(n,i,temp); 
   for(;;) 
   { 
         delay(50); 
         newxy(mx,my,mbutt); 
         if(*mx>603 && *mx<627 && *my>3 && *my<27 && *mbutt) //点击右箭头 
         { 
             if(i<k) //页码未翻到最后一页 
             { 
          	       i++; 
          	       consume_jm(n,temp); 
          	       jl_output(n-8*i+8,i,temp); //打印第 i 页商品 
          	       cursor(*mx,*my); 
             } 
         } 
         if(*mx>540 && *mx<570 && *my>3 && *my<27 && *mbutt) //点击左箭头 
         { 
      	     
 
 
if(i>1) //页码不在第一页 
      	    { 
      	        i--; 	 
      	 	    consume_jm(n,temp);                   
      	 	    jl_output(n-8*i+8,i,temp); //打印第 i 页记录 
      	 	    cursor(*mx,*my); 
      	    } 
         } 
        if(0<= menu_back(mx,my,mbutt)&& menu_back(mx,my,mbutt)<=6)//选择主菜单功能 
      	     break; 
 
   } 
   return menu_back(mx,my,mbutt); 
} 
void jl_output(int n,int k,shangpin**temp) //打印第 k 消费记录 
{ 
    int i=0,j; 
    char s[20];   
    if(n<=8) //n,从第 k 页起的商品总数 
    { 
        for(i=0;i<n;i++) //打印第 k 页的 n 项记录 
        { 
     	 	    j=i/2; //j 第 i+1 件 商 品 所 在 行 数 
     	 	    out_hz(218+260*(i%2),42+120*j,temp[i+8*k-8]->name,16,16,BLACK); 
                sprintf(s,"%.2f",temp[i+8*k-8]->price); 
                setcolor(BLACK); 
                outtextxy(218+260*(i%2),70+120*j,s);//打印价格   
                exchangetime(temp[i+8*k-8]->t,218+260*(i%2),94+120*j);// 打 印时间 
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
   setcolor(WHITE); // 画 左 箭 头 
   circle(15+550,15,12); 
   setlinestyle(0,0,3); 
   line(5+550,15,17+550,10); 
   line(5+550,15,17+550,20); 
   line(5+550,15,25+550,15); 
   setlinestyle(0,0,1); 
} 
void consume_jm(int n,shangpin**temp) //消费记录界面 
{ 
  int i,j; 
  char s[20]; 
 
  float sum=0; //sum 当前消费总额 
  setfillstyle(1,LIGHTBLUE); 
  bar(125,0,639,479); 
  setcolor(WHITE); 
  for(i=0;i<4;i++) //画 8 个显示框 
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
      out_hz(151+260*j,42+120*i,"商品名",16,16,DARKGRAY); 
      out_hz(151+260*j,66+120*i,"价格",16,16,DARKGRAY); 
      out_hz(151+260*j,90+120*i,"购买时间",16,16,DARKGRAY); 
    } 
  } 
   for(i=0;i<n;i++) // 计 算 n 件 商 品 总 价 
        sum+=temp[i]->price; 
   sprintf(s,"%.2f",sum); //将浮点数 sum 转换成字符串 
   setcolor(BLACK); 
   out_hz(130,10,"当前总消费额：",12,12,BLACK); 
   outtextxy(220,12,s); //打印消费总额 
 
   setcolor(WHITE); // 画 右 箭 头 
   circle(15+600,15,12); 
   setlinestyle(0,0,3); 
   line(25+600,15,13+600,10); 
   line(25+600,15,13+600,20); 
   line(5+600,15,25+600,15); 
   setlinestyle(0,0,1); 
} 
 
 
 
 
 
 
 
 
 
/*************************** customer_truck ******************************/ 
 
int buy_truck(int*mx,int*my,int*mbutt,yonghu*cu,yonghu*yh,shangpin*th)         
{ // 购 物 车 主 函 数 
    int n; // n 购 物 车 内 的 商 品 总 数 
    shangpin *temp[20]; 
    n=truck_tongji(0,th,temp);          
    cursor(*mx,*my); 
    timeoutput(420,20); // 输 出 当 前 时 间 
    return sp_xuanze2(mx,my,mbutt,n,cu,yh,temp,th); 
} 
 
int truck_tongji(int k,shangpin*th,shangpin**temp) //购物车商品统计函数 
{ 
 	int i=0,n=0; // i 页 码 
 	shangpin *p=th->next; // p 指向购物车内第一件商品 
    while(p!=NULL) 
    { 
     	if(p->flag==k) 
     	{ 
     	 	temp[i++]=p; 
     	 	n++; 
     	} 
     	p=p->next; 
    } 
    return n; // 返 回 商 品 总 数 
} 
 
int sp_xuanze2(int *mx,int *my,int *mbutt,int n,yonghu*cu,yonghu *yh,shangpin 
**temp,shangpin *th) // 商 品 操 作 函 数 
{ 
 	int i=1,k=1;  //i 当前显示页码,k 为总页数 
 	char str[10]; 
    if(n%4!=0) 
     	k=n/4+1; 
    else 
     	k=n/4; 
    if(n<=4) // 显 示 第 一 页 商 品 
     	sp_output(n,2,temp);        
    else 
     	sp_output(4,2,temp); 
    for(;;) 
    {   
 
     	delay(30); 
     	newxy(mx,my,mbutt); 
     	if(*mx>587 && *mx<635 && *my>=451 && *my<=475 && *mbutt) //点击下一页 
     	{ 
     	    if(i==k) 
     	     	outinformation(mx,my,mbutt,210,200,"已是最后一页！"); 
     	    if(i<k) 
     	 	    {	//打印下一页商品 
     	 	        if((n-4*i)>=4) 
     	 	            sp_output(4,2,temp+4*i); 
     	 	        else 	     	 
     	 	     	    sp_output(n-4*i,2,temp+4*i); 
     	 	     	i++; 
     	 	    } 
     	 	     
     	} 
     	if(*mx>130 && *mx<178 && *my>=451 && *my<=475 && *mbutt) //点击上一页 
     	{ 
     	 	if(i==1) 
     	 	 	outinformation(mx,my,mbutt,210,200,"第一页！"); 
     	 	else     	 	 
     	 	 	{ // 打 印 上 一 页 商 品 
     	 	 	 	sp_output(4,2,temp+4*i-8); 
     	 	 	 	i--; 
     	 	 	} 
     	} 
 	    if(fukuan(mx,my,mbutt,i,n,temp,yh,cu,th)==1)//付款成功返回购物车 
 	     	return 2; 
 	 	if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )     
 	     	 return menu_back(mx,my,mbutt); 
 	} 
} 
int fukuan(int *mx,int *my,int *mbutt,int i,int n,shangpin **temp,yonghu *yh,yonghu 
*cu,shangpin *th) 
{ 
    if(*mx>497 && *mx<597 && *my>80 && *my<100 && *mbutt)//页面上第一件商品付款 
 	{ 
 	 	if(4*i-3<=n) 
 	 	    if( zhuanzhang(mx,my,mbutt,i,0,cu,yh,temp,th)==1 ) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40 && *my<60 && *mbutt) 
 	{ 
        sp_cancel(i,0,n,cu,temp,&th); //删除页面上第一件商品 
 
        return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+110 && *my<100+110 && *mbutt)//页面上第二件商品付款 
 	{ 
 	 	if(4*i-2<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,1,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+110 && *my<60+110 && *mbutt) 
 	{ 
         sp_cancel(i,1,n,cu,temp,&th); // 删 除 页 面 上 第 二 件 商 品 
         return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+220 && *my<100+220 && *mbutt)//页面上第三件商品付款 
 	{ 
 	 	if(4*i-1<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,2,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+220 && *my<60+220 && *mbutt)//删除页面上第三件商品 
 	{ 
        sp_cancel(i,2,n,cu,temp,&th); 
        return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+330 && *my<100+330 && *mbutt)//页面上第四件商品付款 
 	{ 
 	 	if(4*i<=n) 
 	 	    if(zhuanzhang(mx,my,mbutt,i,3,cu,yh,temp,th)==1) 
 	 	     	return 1; 
 	} 
 	if(*mx>497 && *mx<597 && *my>40+330 && *my<60+330 && *mbutt)//删除页面上第四件商品 
 	{ 
        sp_cancel(i,3,n,cu,temp,&th); 
        return 1; 
 	} 
} 
void sp_cancel(int i,int j,int n,yonghu*cu,shangpin**temp,shangpin**th) 
{ // 删 除 商 品 函 数 
 	shangpin *p; 
 
 	p=(*th)->next; // p 链 表 头 节 点 
 	if((4*i-3+j)==1) //如果要删除的商品是第一页的第一件商品 
 	{ 
 	 	if(temp[0]==p) //如果删除的就是头节点  
 	 	{ 
 	 	    (*th)->next=(*th)->next->next; 
 	 	    free(p); 
 	 	} 
 	 	else 
 	 	{ 
 	 	 	while(p->next!=temp[0]) //在链表中找到待删除商品的上一个节点 	 p=p->next; 
 	 	    p->next=temp[0]->next; //在链表中删除节点 
 	 	    free(temp[0]); 
 	 	} 
 	} 
 	else 
 	{ 
 	 	while(p->next!=temp[4*i+j-4]) //在链表中找到待删除商品的上一个节点 
 	 	 	p=p->next; 
 	 	p->next=temp[4*i+j-4]->next; 
 	 	free(temp[4*i+j-4]); 
 	} 
    xieru_yonghu(cu,*th); //将修改写入用户专属 txt 文件 
} 
 int zhuanzhang(int *mx,int *my,int *mbutt,int i,int j,yonghu *cu,yonghu 
*yh,shangpin **temp,shangpin *th) // 转 账 函 数 
{ 
 	int x=zhifu_tanchu(mx,my,mbutt,cu); //支付密码验证通过 
 	if(x==1) 
 	{ 
        if(cu->money<temp[4*i+j-4]->price) //用户余额小于商品价格时 
        { 
         	outinformation(mx,my,mbutt,210,200,"账户余额不足！"); 
         	return 0; 
        } 
        else // 从 用 户 余 额 中 扣 除 商 品 价            
        { 
         	cu->money-=temp[4*i+j-4]->price; 
         	temp[4*i+j-4]->flag=1; //付款标记置 1 
         	temp[4*i+j-4]->t=time(NULL)-5*3600; //调用系统时间 
         	xieru_yonghu(cu,th); //将最新信息写入用户的文件 
         	xiugai_xieru(yh); //将用户链表再次写入 yonghu.txt 
         	xieru_dingdan(cu,temp[4*i+j-4]);//将用户所下的订单写入 dingdan.txt 
 
         	return 1; 
        } 
 	} // 转 账 成 功 函 数 值 返 回 1， 其 余 情 况 返 回 0 
 	else 
 	 	return 0; 
} 
void xieru_dingdan(yonghu*cu,shangpin*p) //用户的订单写入 dingdan.txt 
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
void xieru_yonghu(yonghu*cu,shangpin*th) //修改以用户名命名的文本文件内的信息 
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
int zhifu_tanchu(int*mx,int*my,int*mbutt,yonghu*cu) //确认购买时支付宝弹出 
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
 	getimage(210,190,430,290,buffer);//取屏幕截图存储到 buffer 中 
    setfillstyle(1,LIGHTGRAY); //画弹出框 
    bar(210,190,430,290); 
    setfillstyle(1,BROWN); 
    bar(210,190,430,210); 
    bar(230,260,270,280); 
    bar(370,260,410,280); 
    setfillstyle(1,WHITE); 
    bar(260,230,390,250); 
 
 
 	out_hz(280,192,"支付宝密码",16,16,BLACK); 
out_hz(234,262,"确定",16,16,BLACK); 
out_hz(374,262,"取消",16,16,BLACK);          	
 	
 	
 	
 
 	while(1) 				
 	{ 				
 	  newxy(mx,my,mbutt); 				
 	  if(*mx>300 && *mx<400 && *my>230 && *my<250 && *mbutt)//点击密码输入框 
 	  { 
 	      bar(260,230,390,250); 
 	 	  input_mima(mima,265,232); 
 	  } 
 	  if(*mx>230 && *mx<270 && *my>260 && *my<280 && *mbutt)//点击确认 
 	  { 
 	      if(strcmp(cu->zhifumima,mima)==0) //密码通过时 
 	      { 
             putimage(210,190,buffer,0); 
             free(buffer);  
             outinformation(mx,my,mbutt,210,200,"密码验证通过！！"); 
 	         return 1; 
 	      } 
 	      else 
 	        outinformation(mx,my,mbutt,210,250,"密码错误"); 
 	  } 
 
 	  if(*mx>370 && *mx<410 && *my>260 && *my<280 && *mbutt)//点击取消 
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
   int n; //n 为相应搜索方式搜到的商品总数 
   price1[0]=price2[0]='\0'; 
   search_jm(); 
   for(;;) 
   {   
       delay(20); 
       newxy(mx,my,mbutt); 
       if(*mx>278 && *mx<566 && *my>=375 && *my<=404 && *mbutt) //商品名输入 
      	    input_name(mubiao,280,381,12); 
 	   if(*mx>278 && *mx<338 && *my>=426 && *my<=455 && *mbutt) //左边的价格 
 	  	    input_name(price1,280,433,12); 
 	   if(*mx>400 && *mx<460 && *my>=426 && *my<=455 && *mbutt) //右边的价格 
 	  	    input_name(price2,402,433,12); 
 	   if(*mx>547 && *mx<585 && *my>=427 && *my<=449 && *mbutt) //点击确定时 
 	   { 
            n=searching(mubiao,price1,price2,sh,temp); 
            sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=183 && *mx<320 && *my>=104 && *my<=193 && *mbutt==1)//搜电子产品 
 	   { 
 	 	    n=sp_cla_search(1,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=437 && *mx<574 && *my>=104 && *my<=193 && *mbutt==1) //搜索书籍 
 	   { 
 	 	    n=sp_cla_search(2,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=183 && *mx<320 && *my>=275 && *my<=364 && *mbutt==1) //搜索服饰 
 	   { 
 	 	    n=sp_cla_search(3,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 
 	   if(*mx>=437 && *mx<574 && *my>=275 && *my<=364 && *mbutt==1) //搜索食品 
 
 	   { 
 	 	    n=sp_cla_search(4,sh,temp); 
 	 	    sp_xuanze1(mx,my,mbutt,n,cu,temp,th); 
 	   } 	 	 	 	 	 	 
 	   if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 )//主菜单功能     
 	     	 break; 
   } 
   return menu_back(mx,my,mbutt); 
} 
 
void search_jm() //搜索功能界面绘制 
{ 
   setfillstyle(1,LIGHTBLUE); 
   bar(125,0,639,479); 
   sp_cla_jm();    
   setfillstyle(1,WHITE); // 搜 索 框 绘 制 
   bar(278,375,566,404); 
   bar(278,426,338,455); 
   bar(400,426,460,455); 
   setcolor(WHITE); 
   rectangle(547,427,585,449); 
   out_hz(550,430,"确定",16,16,WHITE); 
   out_hz(209,381,"普通搜索",16,16,WHITE); 
   out_hz(209,432,"价格限定",16,16,WHITE); 
   timeoutput(420,20); 
} 
void sp_cla_jm() // 商 品 分 类 界 面 
{ 
   setfillstyle(1,LIGHTBLUE); 
   bar(125,0,639,479); 
   out_hz(349,44,"商品分类",16,16,WHITE); 
   setfillstyle(1,WHITE); //四块商品区 
   bar(183,104,320,193); 
   bar(183,275,320,364); 
   bar(437,104,574,193); 
   bar(437,275,574,364); 
   setcolor(LIGHTBLUE); 
   setfillstyle(1,LIGHTBLUE); //画电子产品区域  
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
   line(467,119,467,178); // 画 书 籍 
   line(467,178,505,188); 
   line(505,188,505,109); 
   line(505,109,467,119); 
   rectangle(505,115,540,182); 
   bar(514,120,536,125); 
   bar(514,132,536,137); 
   bar(514,145,536,150); 
   bar(514,157,536,162); 
   bar(514,169,536,174); 
   moveto(251,302);//画服饰 
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
   setfillstyle(1,WHITE); // 画 碗 
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
   out_hz(215,87,"电子产品",16,16,WHITE); 
   out_hz(489,87,"图书",16,16,WHITE); 
   out_hz(235,258,"服饰",16,16,WHITE); 
   out_hz(489,258,"食品",16,16,WHITE); 
} 
 
int searching(char*mubiao,char*p1,char*p2,shangpin*sh,shangpin**temp)  //查找有多少个满足条件的商品 
{ 
 	shangpin *s=sh; 
 	int i=0,n=0,t=0; 
 	float f1,f2; 
 	if(p1[0]=='\0'&&p2[0]=='\0') //没有限定价格的情况 
 	{ 
 	    while(s!=NULL) 
 	    { 
           if(strstr(s->name_pinyin,mubiao)!=NULL&&s->life==1) 
 	 	   { 
 	 	         temp[i++]=s; //将找到的商品存到指针数组中 
 	 	 	     n++;                   
           } 	 
 	 	   s=s->next; 
 	     } 
 	} 
    else if(p1[0]!='\0'&&p2[0]=='\0') //只限定价格下线时 
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
 	else if (p1[0]=='\0'&&p2[0]!='\0') // 只 限 定 了 价 格 上 限 时 
 
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
 	else if( p1[0]!='\0'&& p2[0]!='\0' ) //价格上下限都有时 
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
 	return n; // 返 回 满 足 条 件 的 商 品 总 数 n  
} 
 	 
int sp_cla_search(int n,shangpin*sh,shangpin **temp) //商品的分类搜索 
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
 	return k; //返回该类的商品总数 
 
}    
 
void sp_huizhi(int flag,int j,shangpin*sp) //在页面指定位置绘制商品 
{ 
 	  char s[20]; 
 	  setfillstyle(1,WHITE); 
 	  setcolor(WHITE); 
      rectangle(171,40+j*110,597,100+j*110); 
      bar(497,40+j*110,597,60+j*110); 
      bar(497,80+j*110,597,100+j*110); 
      if(flag==1) //flag=1，绘制搜索功能里的商品 
      { 
         out_hz(499,42+j*110,"查看详细信息",16,16,LIGHTBLUE); 
         out_hz(507,82+j*110,"加入购物车",16,16,LIGHTBLUE); 
      } 
      if(flag==2) //flag=2， 绘 制 购 物 车 里 的 商 品 
      { 
         out_hz(531,42+j*110,"删除",16,16,LIGHTBLUE); 
         out_hz(515,82+j*110,"确认购买",16,16,LIGHTBLUE); 
      } 
      out_hz(173,42+j*110,"编号",16,16,WHITE); 
      out_hz(380,42+j*110,"售价",16,16,WHITE); 
      out_hz(173,62+j*110,"名称",16,16,WHITE); 
      out_hz(380,62+j*110,"库存",16,16,WHITE); 
      out_hz(173,82+j*110,"描述",16,16,WHITE); 
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
 	int i=1,k=1; //i 代表当前显示页码,k 为总共的页数 
 	char str[10]; 
    if(n%4!=0) 
     	k=n/4+1; 
    else 
     	k=n/4; 
    if(n<=4) 
 
     	sp_output(n,1,temp); 
    else 
     	sp_output(4,1,temp); //打印第一页的商品内容  
 
    out_hz(145,15,"共找到",12,12,WHITE); 
 	out_hz(200,15,"条搜索结果",12,12,WHITE); 
 	setcolor(WHITE); 
 	sprintf(str,"%d",n); 
 	outtextxy(186,17,str); 
    cursor(*mx,*my); // 输 出 " 搜 索 到 n 条 商 品 " 的 字 样 
 
    for(;;) // 对 商 品 进 行 操 作 
    { 
     	newxy(mx,my,mbutt); 
     	if(*mx>587 && *mx<635 && *my>=451 && *my<=475 && *mbutt) //点击下一页 
     	{ 
     	    if(i==k) 
     	     	outinformation(mx,my,mbutt,210,200,"已是最后一页！"); 
     	    if(i<k) 
     	 	    { 	 
     	 	        if((n-4*i)>=4) 
     	 	          sp_output(4,1,temp+4*i); 
     	 	        else 	     	 
     	 	     	  sp_output(n-4*i,1,temp+4*i); 
     	 	     	i++; 
     	 	    } 
     	 	     
     	} 
     	if(*mx>130 && *mx<178 && *my>=451 && *my<=475 && *mbutt)//点击上一页 
     	{ 
     	 	if(i==1) 
     	 	 	outinformation(mx,my,mbutt,210,200,"第一页！"); 
     	 	else     	 	 
     	 	 	{ 
     	 	 	 	sp_output(4,1,temp+4*i-8); 
     	 	 	 	i--; 
     	 	 	} 
     	} 
 	    goumai(mx,my,mbutt,i,n,temp,cu,th); // 加 入 购 物 车 的 操 作 
 	 	sp_xinxi(mx,my,mbutt,i,n,temp); //查看商品详细信息的操作 
 	 	if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=5 )     
 	     	 break; 
    } 
} 
 
 
void sp_output(int n,int flag,shangpin **temp) //绘制某一页上的商品 
{ 
 	int i; 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,639,479);  	 
 	out_hz(587,451,"下页",24,0,WHITE); 
    out_hz(130,451,"上页",24,0,WHITE); 
 	for(i=0;i<n;i++) 	 	 
 	sp_huizhi(flag,i,temp[i]);   	 
} 
 
void goumai(int *mx,int *my,int *mbutt,int i,int n,shangpin **temp,yonghu 
*cu,shangpin *th) //i 表 示 页 码 
{ 
 	if(*mx>497 && *mx<597 && *my>80 && *my<100 && *mbutt) 
 	{ 
        if(4*i-3<=n) // 将 页 面 上 第 一 件 商 品 加 入 购 物 车 
        { 
            to_gouwuche(cu,temp[4*i-4],th); 
            outinformation(mx,my,mbutt,210,200,"成功加入购物车！"); 
        } 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+110 && *my<100+110 && *mbutt) 
 	{ 
 	 	if(4*i-2<=n) // 将 页 面 上 第 二 件 商 品 加 入 购 物 车 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-3],th); 
 	 	    outinformation(mx,my,mbutt,210,200,"成功加入购物车！"); 
 	 	} 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+220 && *my<100+220 && *mbutt) 
 	{ 
 	 	if(4*i-1<=n) // 将 页 面 上 第 三 件 商 品 加 入 购 物 车 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-2],th); 
 	 	    outinformation(mx,my,mbutt,210,200,"成功加入购物车！"); 
 	 	} 
 	} 
 	if(*mx>497 && *mx<597 && *my>80+330 && *my<100+330 && *mbutt) 
 	{ 
 	 	if(4*i<=n) // 将 页 面 上 第 四 件 商 品 加 入 购 物 车 
 	 	{ 
 	 	    to_gouwuche(cu,temp[4*i-1],th); 
 
 	 	    outinformation(mx,my,mbutt,210,200,"成功加入购物车！"); 
 	    } 
 	} 
} 
void sp_xinxi(int*mx,int*my,int*mbutt,int i,int n,shangpin**temp)//查看商品信息 
{ 
    if(*mx>497 && *mx<597 && *my>40 && *my<60 && *mbutt) 
    { // 查 看 页 面 上 第 一 件 商 品 的 详 细 信 息 
        show_bmp(i,0,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+110 && *my<60+110 && *mbutt) 
    { // 查 看 页 面 上 第 二 件 商 品 的 详 细 信 息 
        show_bmp(i,1,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+220 && *my<60+220 && *mbutt) 
    { // 查 看 页 面 上 第 三 件 商 品 的 详 细 信 息 
        show_bmp(i,2,n,temp); 
        cursor(*mx,*my); 
    } 
    if(*mx>497 && *mx<597 && *my>40+330 && *my<60+330 && *mbutt) 
    { // 查 看 页 面 上 第 四 件 商 品 的 详 细 信 息 
        show_bmp(i,3,n,temp);   
        cursor(*mx,*my); 
    } 
} 
 
void show_bmp(int i,int j,int n,shangpin**temp) // 显 示 商 品 的 图 片 
{ 
  char s1[20]="bmp\\"; 
  char s2[20]=".bmp"; 
  strcat(s1,temp[4*i+j-4]->bianhao); 
  strcat(s1,s2); 
  Read_BMP(s1); 
  yonghu_jm(); // 查 看 完 商 品 信 息 返 回 原 界 面 
  if(n-4*i+4<=4) 
      sp_output(n-4*i+4,1,temp+4*i-4); 
  else 
      sp_output(4,1,temp+4*i-4); 
} 
void to_gouwuche(yonghu*cu,shangpin*temp,shangpin*th) //加入购物车 
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
    *q=*temp; // 将 相 应 商 品 信 息 复 制 到 q 
    q->next=NULL; 
    p=th->next; 
    while(p!=NULL) 
    { 
        fprintf(fp,"%d %d %d %s %d %d %ld %f %f %f %s %s %s\n",p->life,p->flag, p->cla,p->bianhao,p->kucun,p->xiaoshou,p->t,p->chengben,p->price,p->pro fit,p->name,p->name_pinyin,p->miaoshu); 
        p=p->next; 
    } 
 	fclose(fp); 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************customer_zhifubao.c*****************************/ int zhifubao(int*mx,int*my,int*mbutt,yonghu*p,yonghu*yh) // 支 付 宝 功 能 
{ 
 	char nu[12];               // 银 行 卡 账 号 
 	char mima[12];             // 银 行 卡 密 码 
 	char money[10]; // 充 值 金 额 
 	bank *bh,*q; //bh 银 行 链 表 的 头 指 针 
    zhifubao_jm(p); // 支 付 宝 界 面 绘 制 
    delay(100); 
    for(;;) 
    { 
      newxy(mx,my,mbutt); 
      if(*mx>346 && *mx<488 && *my>=195 && *my<=215 && *mbutt)  //输入银行卡 
      { 
         bar(346,195,488,215); 
         input_name(nu,348,197,12); 
      } 
      if(*mx>346 && *mx<488 && *my>=225 && *my<=245 && *mbutt) //输入密码 
      { 
         bar(346,225,488,245); 
         input_mima(mima,348,227); 
      } 
      if(*mx>346 && *mx<488 && *my>=255 && *my<=275 && *mbutt) //输入金额 
      {  
         bar(346,255,488,275); 
         input_name(money,348,257,12); 
      } 
      if(*mx>436 && *mx<478 && *my>=305 && *my<=331 && *mbutt) //确认充值 
      { 
        if(nu[0]=='\0') 
 	 	 	outinformation(mx,my,mbutt,210,200,"卡号不能为空！"); 
        else if(money[0]=='\0')       	 
 	 	 	outinformation(mx,my,mbutt,210,200,"请输入金额！");       	 
        else  
        { 
 	 	 	bh=bank_creat(); // 创 建 银 行 卡 链 表 
 	 	 	if((q=bank_yz(bh,nu,mima))==NULL) 
 	 	 	 	outinformation(mx,my,mbutt,210,200,"充值失败！"); 
            else 
            { 
                if(chongzhi(p,q,atof(money))==0) 
 	 	 	 	 	outinformation(mx,my,mbutt,210,200,"银行卡余额不足！"); 
                else 
 
                {    
                    outinformation(mx,my,mbutt,210,200,"充值成功！");        
                 	xiugai_xieru(yh);    //用户最新信息写入 yonghu.txt 
 	 	 	 	 	bank_xieru(bh);      //银行卡最新信息写入 bank.txt 
 	 	 	 	 	bank_clear(bh);	//清除银行卡链表 
                 	zhifubao_jm(p);      //返回支付宝界面 
                 	cursor(*mx,*my); 
                 	money[0]='\0'; 
                 	nu[0]='\0'; 
                 	mima[0]='\0'; 
                } 
            } 
        } 
      } 
     if( menu_back(mx,my,mbutt)>=0&& menu_back(mx,my,mbutt)<=6 ) //返回主菜单    
 	     	 break; 
    } 
    return menu_back(mx,my,mbutt); 
} 
 
int chongzhi(yonghu*p,bank*q,float x) //p 当 前 用 户 ，q 当 前 银 行 卡 
{ 
    if(x>q->money) 
     	return 0; // 余 额 不 足 
    else 
    { 
     	q->money-=x; 
     	p->money+=x; 
     	return 1; // 充 值 成 功 
    } 
} 
bank* bank_yz(bank*h,char*name,char *mima) //验证银行卡号与密码是否正确 
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
 	 	return NULL; //验证成功返回银行卡指针，否则返回 NULL 
} 
void bank_xieru(bank*h) //充值成功后相应银行卡要扣款 
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
bank *bank_creat() // 银 行 账 户 链 表 创 建 
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
void bank_clear(bank*bh) //银行链表的清除 
{ 
 	bank *p=bh; 
    while(bh!=NULL) 
    { 
     	p=bh->next; 
     	free(bh); 
     	bh=p; 
    } 
} 
 
void zhifubao_jm(yonghu*p) // 支 付 宝 界 面 绘 制 函 数 
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
  bar(436,305,478,331);//确认按钮 
  timeoutput(420,20);  
  out_hz(265,105,"余额",16,16,WHITE); 
 
  out_hz(265,148,"充值",16,16,WHITE); 
  out_hz(298,197,"银行卡",16,16,WHITE); 
  out_hz(306,227,"密码",16,16,WHITE); 
  out_hz(272,257,"输入金额",16,16,WHITE); 
  out_hz(441,310,"确认",16,16,LIGHTBLUE); 
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
        if(*mx>=200 && *mx<306 && *my>=223 && *my<=257 && *mbutt) //未读留言 
        { 
            liuyan *lp1[20]; 
            for(i=7;i==7;) 
                i=weidu(mx,my,mbutt,1,1,1,lh,lp1,cu->name); 
            return i; 
        } 
        if(*mx>=459 && *mx<565 && *my>=223 && *my<=257 && *mbutt) //已读留言 
        { 
            liuyan *lp2[20]; 
            for(i=7;i==7;) 
                i=weidu(mx,my,mbutt,1,1,0,lh,lp2,cu->name); 
            return i; 
        } 
         if(*mx>=330 && *mx<436 && *my>=123 && *my<=157 && *mbutt)  //已发信息  
         { 
             liuyan *lp3[20]; 
             for(i=7;i==7;) 
                 i=weidu(mx,my,mbutt,1,0,1,lh,lp3,cu->name); 
             return i; 
         } 
         if(*mx>=330 && *mx<436 && *my>=323 && *my<=357&&*mbutt) //编写留言 
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
 
 	manager_jm(); // 管 理 员 主 界 面 
 	cursor(*mx,*my); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>=0 && *mx<38 && *my>=456 && *my<=480 && *mbutt)//退出 
 	 	 	exit(0); 
 	 	if(*mx>=87 && *mx<125 && *my>=456 && *my<=480 && *mbutt)//返回 
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
{ // 根 据 不 同 功 能 函 数 的 返 回 值 进 入 下 一 功 能 
 	for(;;) 
 	{ 
 	 	switch (i) 
 	 	{ 
 	 	  case 1: 
 	 	 	i=good_mana(mx,my,mbutt,sh); // 商 品 管 理 
 	 	 	break; 
 	 	case 2: 
 	 	 	i=order_mana(mx,my,mbutt,sh,dh); // 订 单 处 理 
 	 	 	break; 
 	 	case 3: 
 	 	 	i=message(mx,my,mbutt,lh); // 留 言 功 能 
 	 	 	break; 
 	 	default: 
 	 	 	return 0; 
 
 	 	} 
    } 
} 
void manager_jm() // 管 理 员 主 界 面 
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
 	out_hz(27,87+14,"商品管理",16,16,WHITE); 
 	out_hz(27,87*2+58,"订单处理",16,16,WHITE); 
 	out_hz(27,87*3+44*2+14,"查看留言",16,16,WHITE); 
 	bar(0,456,38,480); 
 	bar(87,456,125,480); 
 	out_hz(2,460,"退出",16,16,WHITE); 
 	out_hz(88,460,"返回",16,16,WHITE); 
} 
 
void kucunbuzu(int*mx,int*my,int*mbutt,int t) //库存不足与商品停售提醒 
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
 	 	out_hz(272,230,"库存不足",24,0,BLACK); 
 	if(t==2) 
 	 	out_hz(260,230,"商品已停售",24,0,BLACK); 
    out_hz(304,312,"确认",16,16,BLACK); 
    for(;;) 
    { 
        newxy(mx,my,mbutt); 
 
 	 
 	if(*mx>300&&*mx<340&&*my>310&&*my<330&&*mbutt==1) //点击确认 
{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
    } 
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************** manager_spgl.c *****************************/ int good_mana(int*mx,int*my,int*mbutt,shangpin*sh) //商品管理主功能函数 
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
  l=(k-1)/5; //l 总 页 数 
  good_jm(sh,&i); // 商 品 管 理 界 面 
  if(*mx>125&&*mx<=640) 
    cursor(*mx,*my); 
  delay(100); 
  for(;;) 
  { 
 	  newxy(mx,my,mbutt); 
 	  if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//主菜单功能 
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
 	  { // 删 除 本 页 第 一 件 商 品 
 	 	  shanchusp(mx,my,mbutt,sp[i*5],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88 && *my<=88*2 && *mbutt &&i*5+2<=k) 
      { // 删 除 该 页 第 二 件 商 品 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+1],&f); 
 	 	  if(f==1) 
 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*2 && *my<=88*3 && *mbutt &&i*5+3<=k)                
//删除该页第三件商品 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+2],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*3 && *my<=88*4 && *mbutt &&i*5+4<=k)                
//删除该页第四件商品 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+3],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=528 && *mx<592 && *my>=68+88*4 && *my<=88*5 && *mbutt &&i*5+5<=k)                
//删除页面上的第五件商品 
 	  { 
 	 	  shanchusp(mx,my,mbutt,sp[i*5+4],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 
 	  else if(*mx>=400 && *mx<416 && *my>=31 && *my<=45 && *mbutt &&i*5+1<=k) 
 	  {                                // 修 改 页 面 第 一 件 商 品 的 价 格 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88 && *my<=45+88 && *mbutt &&i*5+2<=k)
//修改页面第二件商品的价格 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+1],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*2 && *my<=45+88*2 && *mbutt &&i*5+3<=k) // 修 改 页 面 第 三 件 商 品 的 价 格 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+2],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*3 && *my<=45+88*3 && *mbutt &&i*5+4<=k)	//修改页面第四件商品的价格 
 	  { 
 
 	 
 	  xiugai_jg(mx,my,mbutt,sp[i*5+3],&f); 
  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 	  else if(*mx>=400 && *mx<416 && *my>=31+88*4 && *my<=45+88*4 && *mbutt &&i*5+5<=k) // 修 改 页 面 第 五 件 商 品 的 价 格 
 	  { 
 	 	  xiugai_jg(mx,my,mbutt,sp[i*5+4],&f); 
 	 	  if(f==1) 
 	 	  { 
 	 	    xiesp(sh); 
 	 	    return 1; 
 	 	  } 
 	  } 
 
 	  else if(*mx>=494 && *mx<510 && *my>=31 && *my<=45 && *mbutt&&i*5<k) 
 	  { // 为 页 面 上 第 一 件 商 品 补 货 
 
 	      buchong_kc(mx,my,mbutt,sp[i*5],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88 && *my<=45+88 && *mbutt&&i*5+1<k) 
 	  { // 为 页 面 上 第 二 件 商 品 补 货 
 	      buchong_kc(mx,my,mbutt,sp[i*5+1],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88*2 && *my<=45+88*2 && 
*mbutt&&i*5+2<k) // 为 页 面 上 第 三 件 商 品 补 货 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+2],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else  if(*mx>=494  &&  *mx<510  &&  *my>=31+88*3  &&  *my<=45+88*3  && 
*mbutt&&i*5+3<k)                            // 为 页 面 上 第 四 件 商 品 补 货 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+3],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 	      } 
 	  } 
 	  else if(*mx>=494 && *mx<510 && *my>=31+88*4 && *my<=45+88*4 && 
*mbutt&&i*5+4<k) // 为 页 面 上 第 五 件 商 品 补 货 
 	  { 
 	      buchong_kc(mx,my,mbutt,sp[i*5+4],&f); 
 	      if(f==1) 
 	      { 
 	        xiesp(sh); 
 	        return 1; 
 
 	      } 
 	  } 
 	  else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
 	  { // 点 击 下 一 页 
 	 	  i++; 
 	 	  good_jm(sh,&i); 
          cursor(*mx,*my); 
     	  delay(100); 
 	  } 
 	  else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
 	  { // 点 击 上 一 页 
 	      i--; 
 	      good_jm(sh,&i); 
 	      cursor(*mx,*my); 
 	      delay(100); 
 	  } 
 
  } 
} 
void shanchusp(int *mx,int *my,int *mbutt,shangpin*p,int*f)//删除商品弹出框 
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
 	setfillstyle(1,WHITE); // 画 弹 出 框 
 	bar(170,140,470,340); 
 	setfillstyle(1,DARKGRAY); 
 	bar(170,140,470,160); 
 	setfillstyle(1,LIGHTGRAY); 
 	bar(190,310,230,330); 
 	bar(410,310,450,330); 
 	out_hz(260,230,"确认删除吗",24,0,BLACK); 
 	out_hz(194,312,"取消",16,16,BLACK); 
 	out_hz(414,312,"确认",16,16,BLACK); 
 	for(;;) 
 	{ 
        newxy(mx,my,mbutt); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //点击取消 
 
 	 	{ 	
 	 	 	putimage(170,140,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
        if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //确认删除    
        {  
 	 	 	p->life=0;      
 	 	 	putimage(170,140,buffer,0); 
 	 	 	free(buffer); 
 	 	 	*f=1; 
 	 	 	break; 
 	 	} 
 	} 
} 
void xiugai_jg(int*mx,int*my,int*mbutt,shangpin*p,int*f) //修改价格函数 
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
 	setfillstyle(1,WHITE); // 绘 制 输 入 框 
 	bar(170,140,470,340); 
 	setfillstyle(1,DARKGRAY); 
 	bar(170,120,470,140); 
 	setfillstyle(1,LIGHTGRAY); 
 	bar(190,310,230,330); 
 	bar(410,310,450,330); 
 	out_hz(200,230,"输入价格",16,16,BLACK); 
 	out_hz(194,312,"取消",16,16,BLACK); 
 	out_hz(414,312,"确认",16,16,BLACK); 
 	setcolor(LIGHTGRAY); 
 	rectangle(266,228,440,248); 
 	if(*mx>170&&*mx<470&&*my>120&&*my<340) 
 
 	 	cursor(*mx,*my); 
 	for(;;) 
 	{ 
        newxy(mx,my,mbutt); 
        if(*mx>266&&*mx<440&&*my>232&&*my<252&&*mbutt==1) //输入价格 
 
 
 	 
 
 	 	input_name(str,267,230,12); 
if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //点击取消 
{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
        } 
        if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //确认修改  
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
void buchong_kc(int*mx,int*my,int*mbutt,shangpin*p,int*f)//补充商品库存 
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
    setfillstyle(1,WHITE); // 输 入 框 绘 制 
    bar(170,140,470,340); 
    setfillstyle(1,DARKGRAY); 
    bar(170,120,470,140); 
    setfillstyle(1,LIGHTGRAY); 
    bar(190,310,230,330); 
    bar(410,310,450,330); 
 
    out_hz(184,230,"输入补充数量",16,16,BLACK); 
    out_hz(194,312,"取消",16,16,BLACK); 
    out_hz(414,312,"确认",16,16,BLACK); 
    setcolor(LIGHTGRAY); 
    rectangle(282,228,440,248); 
     
    for(;;) 
    { 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>266&&*mx<440&&*my>232&&*my<252&&*mbutt==1) //输入补充数量 
 	 	 	input_name(str,283,230,12); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //取消进货 
 	 	{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
 	 	} 
 	 	if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //确认进货 
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
void xiesp(shangpin *sh) // 商 品 信 息 写 入 shangpin.txt 
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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/**************************** manager_dingdan.c *****************************/ int order_mana(int*mx,int*my,int*mbutt,shangpin*sh,dingdan*dh) //订单管理 
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
    l=(k-1)/4; //l 总 页 数 
    order_jm(dh,sh,&i); 
    if(*mx>=125) 
        cursor(*mx,*my); 
    delay(100); 
    for(;;) 
    { 
        newxy(mx,my,mbutt); 
        if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//返回主要功能 
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
//第一件商品发货 
        { 
            if(sq[i*4]->life==0) // 商 品 停 售 时 提 醒 
            { 
 
                kucunbuzu(mx,my,mbutt,2); 
                return 2; 
            } 
            else if(sq[i*4]->kucun==0) // 商 品 已 售 完 提 醒 
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
        else if(*mx>=513-48 && *mx<597 && *my>=70+110 && *my<=100+110 && *mbutt &&i*4+2<=k) // 第 二 件 商 品 发 货 
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
        else if(*mx>=513-48 && *mx<597 && *my>=70+110*2 && *my<=100+110*2 && *mbutt &&i*4+3<=k) // 页 面 第 三 件 商 品 发 货 
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
        else if(*mx>=513-48 && *mx<597 && *my>=70+110*3 && *my<=100+110*3 && *mbutt &&i*4+4<=k) // 页 面 第 四 件 商 品 发 货 
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
        { // 删 除 页 面 第 一 笔 订 单 
            shanchudd(mx,my,mbutt,dp[i*4]); 
 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110 && *my<=56+110 && *mbutt &&i*4+2<=k) // 删 除 页 面 第 二 笔 订 单 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+1]); 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110*2 && *my<=56+110*2 && *mbutt &&i*4+3<=k) // 删 除 页 面 第 三 笔 订 单 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+2]); 
            return 2; 
        } 
        else if(*mx>=537 && *mx<597 && *my>=40+110*3 && *my<=56+110*3 && *mbutt &&i*4+4<=k) // 删 除 页 面 的 第 四 笔 订 单 
        { 
            shanchudd(mx,my,mbutt,dp[i*4+3]); 
            return 2; 
        } 
        else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
        { // 点 击 下 一 页 
            i++; 
            order_jm(dh,sh,&i); 
            cursor(*mx,*my); 
            delay(100); 
        } 
        else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
        { // 点 击 上 一 页 
            i--; 
            order_jm(dh,sh,&i); 
            cursor(*mx,*my); 
            delay(100); 
        } 
    } 
} 
void shanchudd(int *mx,int *my,int *mbutt,dingdan*p) //删除订单 
{ 
    void *buffer; 
    unsigned size; 
    size=imagesize(170,120,470,340); 
    if(((void*)buffer=malloc(size))==NULL) 
    { 
 	 	printf("No enough memory"); 
 
 	 	exit(1); 
    } 
    getimage(170,120,470,340,buffer); // 弹 出 框 绘 制 
    setfillstyle(1,WHITE); 
    bar(170,140,470,340); 
    setfillstyle(1,DARKGRAY); 
    bar(170,120,470,140); 
    setfillstyle(1,LIGHTGRAY); 
    bar(190,310,230,330); 
    bar(410,310,450,330); 
    out_hz(194,312,"取消",16,16,BLACK); 
    out_hz(414,312,"确认",16,16,BLACK); 
    out_hz(260,230,"确认删除吗",24,0,BLACK); 
    for(;;) 
    { 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>190&&*mx<230&&*my>310&&*my<330&&*mbutt==1) //点击取消 
 	 	{ 
 	 	 	putimage(170,120,buffer,0); 
 	 	 	cursor(*mx,*my); 
 	 	 	free(buffer);   
 	 	 	break; 
 	 	} 
 	 	if(*mx>410&&*mx<450&&*my>310&&*my<330&&*mbutt==1) //点击确认 
 	 	{  
 	 	 	p->life=0; 
 	 	 	putimage(170,140,buffer,0); 
 	 	 	free(buffer); 
 	 	 	cursor(*mx,*my); 
 	 	 	break; 
 	 	} 
    } 
} 
 
void xiedd(dingdan*dh) // 重 写 订 单 文 件 txt 
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
void order_jm(dingdan*dh,shangpin*sh,int*i) // 订 单 管 理 第 i+1 页 界 面 
{ 
    int j=0,l=0; // l， 订 单 序 号 
    char str[30]; 
    dingdan *p; // 订 单 链 表 的 头 指 针 
    shangpin *q; 
    q=sh; 
    p=dh; 
    setfillstyle(1,LIGHTBLUE); 
    bar(125,0,640,480); 
    out_hz(587,451,"下页",24,0,WHITE); 
    out_hz(130,451,"上页",24,0,WHITE); 
    setcolor(WHITE); 
    setfillstyle(1,WHITE); 
    while(p!=NULL&&l<4+4*(*i)) // 对 订 单 逐 项 扫 描 
    {   
        if(l>=4*(*i)&&p->life==1) 
        { 
            while(strcmp(p->bianhao,q->bianhao)!=0&&q!=NULL) 
            { 
                q=q->next; 
            } 
            strcpy(str,q->name); 
            rectangle(171,40+j*110,597,100+110*j); // 打 印 商 品 信 息 
            out_hz(173,42+110*j,"用户",16,16,WHITE); 
            out_hz(173,62+110*j,"商品",16,16,WHITE); 
            out_hz(173,82+110*j,"地址",16,16,WHITE); 
            out_hz(380,42+110*j,"电话",16,16,WHITE); 
            outtextxy(210,44+110*j,p->name); 
            out_hz(210,62+110*j,str,16,16,WHITE); 
            outtextxy(210,84+110*j,p->address); 
            outtextxy(417,44+110*j,p->phone);   
            bar(513-48,70+110*j,597,100+110*j); 
            out_hz(531-48,73+110*j,"点击发货",24,0,LIGHTBLUE); 
            bar(537,40+110*j,597,56+110*j); 
 
            out_hz(543,42+110*j,"取消订单",12,12,LIGHTBLUE); 
            j++; 
            q=sh; 
        } 
        if(p->life==1) // 未 处 理 的 订 单 
            l++; 
        p=p->next; 
    }        
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/*************************** liuyan.c *****************************/ 
int message(int*mx,int*my,int*mbutt,liuyan*lh) // 留 言 功 能 函 数 
{ 
 	int i; 
 	message_jm(); 
 	delay(100); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//返回主菜单 
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
 	 	{ // 未 读 留 言 
 	 	 	liuyan *lp1[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,0,1,lh,lp1,"\0"); 
 	 	 	return i; 
 	 	} 
 	 	else if(*mx>=459 && *mx<565 && *my>=223 && *my<=257 && *mbutt) 
 	 	{ // 已 读 留 言 
 	 	 	liuyan *lp2[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,0,0,lh,lp2,"\0"); 
 	 	 	return i; 
 	 	} 
 	 	else if(*mx>=330 && *mx<436 && *my>=123 && *my<=157 && *mbutt) 
 	 	{ // 已 发 信 息 
            liuyan *lp3[20]; 
 	 	 	for(i=7;i==7;) 
 	 	 	 	i=weidu(mx,my,mbutt,2,1,1,lh,lp3,"\0"); 
 	 	 	return i; 
 	 	} 	
 	 	else if(*mx>=330 && *mx<436 && *my>=323 && *my<=357 && *mbutt) 
 	 	{ // 编 辑 信 息 
 	 	 	for(i=7;i==7;) 
 
 
 
 	 
 
 	 	 	i=bianxieliuyan(mx,my,mbutt,2,lh,"\0"); 
 	return i; 
} 
 	} 	
} 		
 		
int weidu(int *mx,int *my,int *mbutt,int iden,int g,int t,liuyan *lh,liuyan 
*lp[],char str[12]) // 留 言 处 理 函 数 
{ 
 	int i=0,k=0,l; 
 	liuyan*q,*p; 
 	p=lh; //p 留 言 头 指 针 
 	if(iden==2&&g==0) //iden=2，表示管理员正在使用留言功能 
    { //g 标记，0 表示是接收到的信息，1 表示发出的信息   	 while(p!=NULL) //t=1 表示未读，t=0,表示已读 
 	 	{ 
 	 	 	if(p->flag==0&&p->life==t) 
                lp[k++]=p;	//0 表示用户给店铺的留言，1 表示店铺给用户的留言 
            p=p->next; 
        } 
    } 
    else if(iden==2) //管理员发出的信息 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==1) 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
    else if(iden==1&&g==1) //特定用户接收未读的或已读的信息 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==1 && p->life==t && strcmp(p->yonghuname,str)==0) 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
    else if(iden==1) // 特 定 用 户 已 发 出 的 信 息 
    { 
 	 	while(p!=NULL) 
 	 	{ 
 	 	 	if(p->flag==0&&strcmp(p->yonghuname,str)==0) 
 
 	 	 	 	lp[k++]=p; 
 	 	 	p=p->next; 
 	 	} 
    } 
 	weidu_jm(lh,&i,str,iden,g,t); //留言显示界面 
 	l=(k-1)/5; 
 	cursor(*mx,*my); 
 	delay(100); 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
        if(iden==2) 
        { 
 	 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//回主菜单 
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
 	 	if(iden==1) // 回 用 户 主 菜 单 
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
 	 	 	input_liuyan(211,68,iden,lp[i*5],q,lh); //回复页面第一条留言  
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88 && *my<=88*2 && *mbutt &&i*5+2<=k)
//回复页面第二条留言 
 	 	{ 
 	 	 	input_liuyan(211,68+88,iden,lp[i*5+1],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*2 && *my<=88*3 && *mbutt 
&&i*5+3<=k) // 回 复 页 面 第 三 条 留 言 
 	 	{ 
 	 	 	input_liuyan(211,68+88*2,iden,lp[i*5+2],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*3 && *my<=88*4 && *mbutt 
&&i*5+4<=k)	//回复页面第四条留言 
 	 	{ 
 	 	 	input_liuyan(211,68+88*3,iden,lp[i*5+3],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=210 && *mx<592 && *my>=68+88*4 && *my<=88*5 && *mbutt &&i*5+5<=k) // 回 复 页 面 第 五 条 留 言 
 	 	{ 
 	 	 	 
 	 	 	input_liuyan(211,68+88*4,iden,lp[i*5+4],q,lh); 
 	 	 	return 7; 
 	 	} 
 	 	else if(*mx>=587 && *mx<635 && *my>=451 && *my<=475 && *mbutt&&i<l) 
 	 	{ // 点 击 下 一 页 
 	 	 	i++; 
 	 	 	weidu_jm(lh,&i,str,iden,g,t); 
 	 	 	cursor(*mx,*my); 
 	 	 	delay(100); 
 	 	} 
 	 	else if(*mx>=130 && *mx<178 && *my>=451 && *my<=475 && *mbutt&&i>0) 
 	 	{ // 点 击 上 一 页 
 	 	 	i--; 
 	 	 	weidu_jm(lh,&i,str,iden,g,t); 
 	 	 	cursor(*mx,*my); 
 	 	 	delay(100); 
 
        if(t==1) // 若 是 未 读 留 言 
        { 
 	 	 	if(*mx>=512 && *mx<592 && *my>=35 && *my<=51 && *mbutt &&i*5+1<=k) 
 	 	 	{                     // 将 页 面 上 第 一 条 留 言 标 记 为 已 读 
 	 	 	 	lp[i*5]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88 && *my<=51+88 && *mbutt &&i*5+2<=k)		//将页面上第二条留言标记为已读 
 	 	 	{ 
 	 	 	 	lp[i*5+1]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*2 && *my<=51+88*2 && *mbutt &&i*5+3<=k)		//将页面上第三条留言标记为已读 
 	 	 	{ 
 	 	 	 	lp[i*5+2]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*3 && *my<=51+88*3 && *mbutt &&i*5+4<=k) // 将 页 面 上 第 四 条 留 言 标 记 为 已 读 
 	 	 	{ 
 	 	 	 	lp[i*5+3]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
 	 	 	else if(*mx>=512 && *mx<592 && *my>=35+88*4 && *my<=51+88*4 && *mbutt &&i*5+5<=k)		//将页面上第五条留言标记为已读 
 	 	 	{ 
 	 	 	 	lp[i*5+4]->life=0; 
 	 	 	 	xiely(lh); 
 	 	 	 	return 7; 
 	 	 	} 
        } 
 	} 
} 
void message_jm() // 留 言 主 界 面 绘 制 
{ 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,640,480); 
 	setfillstyle(1,WHITE); 
 
 	bar(200,223,306,257); 
 	bar(459,223,565,257); 
 	bar(330,123,436,157); 
 	bar(330,323,436,357); 
 	out_hz(205,228,"未读留言",24,0,LIGHTBLUE); 
 	out_hz(464,228,"已读留言",24,0,LIGHTBLUE); 
 	out_hz(335,128,"已发信息",24,0,LIGHTBLUE); 
 	out_hz(335,328,"编写信息",24,0,LIGHTBLUE); 
} 
 
void weidu_jm(liuyan*lh,int*i,char str[12],int iden,int g,int t) 
{ // 打 印 第 i+1 页 的 留 言 
    int l=0,j=0; 
 	liuyan *p; 
 	p=lh; 
 	setfillstyle(1,LIGHTBLUE); 
 	bar(125,0,640,480); 
 	out_hz(587,451,"下页",24,0,WHITE); 
 	out_hz(130,451,"上页",24,0,WHITE); 
 	setcolor(WHITE); 
 	setfillstyle(1,WHITE); 
 	if(iden==2) 
 	{ 
 	 	if(g==0) 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==g&&p->life==t)//管理员接收到的留言 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) // 是 否 已 读 由 t 值 决 定 
 	 	 	 	 	{ // 打 印 留 言 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"用户",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"内容",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"回复",12,12,WHITE); 
 	 	 	 	 	 	if(t==1) 
 	 	 	 	 	 	 	out_hz(515,35+j*88,"标记为已读",16,16,WHITE); 
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
 	 	 	while(p!=NULL&&l<5+5*(*i)) // 管 理 员 发 出 的 留 言 
 	 	 	{ 
 	 	 	 	if(p->flag==g) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"用户",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"内容",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"回复",12,12,WHITE); 
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
 	 	if(g==1) // 相 应 用 户 接 收 到 的 留 言 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==g&&p->life==t&&strcmp(str,p->yonghuname)==0) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) // 留 言 绘 制 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"用户",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"内容",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"回复",12,12,WHITE); 
 	 	 	 	 	 	if(t==1) 
 	 	 	 	 	 	 	out_hz(515,35+j*88,"标记为已读",16,16,WHITE); 
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
 	 	else // 特 定 用 户 已 发 出 的 信 息 
 	 	{ 
 	 	 	while(p!=NULL&&l<5+5*(*i)) 
 	 	 	{ 
 	 	 	 	if(p->flag==0&&strcmp(str,p->yonghuname)==0) 
 	 	 	 	{   
 	 	 	 	 	if(l>=5*(*i)) 
 	 	 	 	 	{ 
 	 	 	 	 	 	rectangle(171,30+j*88,597,90+j*88); 
 	 	 	 	 	 	out_hz(173,32+j*88,"用户",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,52+j*88,"内容",12,12,WHITE); 
 	 	 	 	 	 	out_hz(173,72+j*88,"回复",12,12,WHITE); 
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
{                                      // 输 入 留 言 并 写 入 文 件 
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
 	if((q=(liuyan*)malloc(sizeof(liuyan)))==NULL) //分配内存空间并检测 
 	{ 
 	 	printf("No enough memory"); 
 	 	exit(0); 
    } 
 	p->next=q; 
 	q->next=NULL; 
 	p=lh; 
 	lp->life=0; 
 	if(iden==2) //如果是管理员正在编辑，则方向标记为 1 
 	 	q->flag=1; 
 	else 
 	 	q->flag=0; 
 	q->life=1; 
 	strcpy(q->xinxi,str); 
 	strcpy(q->yonghuname,lp->yonghuname); 
 	if( (fp=fopen("txt\\liuyan.txt","wt"))==NULL) //打开文件 
 	{ 
 	 	printf("unable to open liuyan.txt"); 
 	 	exit(1); 
 	} 
 	while(p!=NULL) // 留 言 信 息 写 入 文 件 
 	{ 
 	 	fprintf(fp,"%d %d %s %s\n",p->flag,p->life,p->yonghuname,p->xinxi); 
 	 	p=p->next; 
 	} 
 	fclose(fp); 
 	return; 
} 
int bianxieliuyan(int*mx,int*my,int*mbutt,int iden,liuyan*lh,char str[12]) 
{                                                          // 编 辑 留 言 
 	bianxie_jm(iden); // 界 面 绘 制 
 	char str1[12]; // 用 户 名 
 	char str2[80]; // 留 言 正 文 
 	str1[0]='\0'; 
 	str2[0]='\0'; 
 	cursor(*mx,*my); 
 
 	setfillstyle(1,WHITE); 
 	if(iden==1) 
 	 	strcpy(str1,str); //str，当前登录用户的用户名 
 	for(;;) 
 	{ 
 	 	newxy(mx,my,mbutt); 
 	 	if(iden==2) 
 	 	{ 
 	 	 	if(*mx>=12 && *mx<112 && *my>=87 && *my<=87+44 && *mbutt)//回主菜单 
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
 	 	if(iden==1) // 返 回 用 户 主 菜 单 
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
 	 	 	bar(202,58+2*88,594,76+2*88); // 输 入 用 户 名 
 	 	 	input_name(str1,202,59+2*88,12); 
 	 	} 
 
 
 	 
 	else if(*mx>=201 && *mx<595 && *my>=84+2*88 && *my<=104+2*88 && *mbutt) 
{ // 输 入 信 息 内 容 
 	 	 	bar(202,85+2*88,594,103+2*88); 
 	 	 	input_name(str2,202,86+2*88,80); 
 	 	} 
 	 	else if(*mx>=564 && *mx<592 && *my>=129+2*88 && *my<=145+2*88 && *mbutt) 
 	 	{ // 确 认 发 送 留 言 
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
void bianxie_jm(int iden) // 编 写 界 面 绘 制 
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
    out_hz(566,131+2*88,"确认",12,12,DARKGRAY); 
    out_hz(178,131+88*2,"取消",12,12,DARKGRAY); 
    if(iden==2) 
 	 	out_hz(175,61+2*88,"用户",12,12,DARKGRAY); 
 	out_hz(175,88+2*88,"内容",12,12,DARKGRAY); 
 
 	out_hz(360,34+2*88,"编写信息",12,12,DARKGRAY);   
} 
void liuyan_xieru(liuyan*h,char a[12],char c[80],int t) //留言写入 txt 文件 
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
 
void xiely(liuyan *lh) // 将 留 言 链 表 写 入 liuyan.txt 
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
 
void guangbiao(int x,int y) // 画 光 标 
{ 
 	 setfillstyle(1,BLACK); 
 	 bar(x,y,x+2,y+16); 
} 
void xiaoguangbiao(int x,int y) // 消 除 光 标 
{ 
     setfillstyle(1,WHITE); 
     bar(x,y,x+2,y+16); 
} 
void input_name(char *p,int x,int y,int length)  //明文输入函数 
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
 
void input_mima(char *p,int x,int y) // 密 文 输 入 函 数 
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
//信息的提示弹出框 
{ 
 	void *buffinf=NULL;		//x,y 代表框的绝对位置，out 代表提示内容 
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
 	out_hz(x+78,y+2,"消息提示",16,16,BLACK); 
 	out_hz(x+174,y+77,"确定",16,16,BLACK);         
    out_hz(x+20,y+37,out,16,16,BLACK);   	 	 	 	 	 
 	while(1) 
 	{ 
 	  newxy(mx,my,mbutt); 
 	  if(*mbutt==1) // 单 击 " 确 定 " 退 出 
 	  { 
 	 	 if((*mx>=x+170&&*mx<=x+210)&&(*my>=y+75&&*my<=y+95)) 
 	 	 	break; // 退 出 循 环 
 
 	  } 
 	} 
 	cursor(*mx,*my); 
 	putimage(x,y,buffinf,0); 
 	cursor(*mx,*my); // 图 像 操 作 
 	free(buffinf); // 释 放 内 存 
 	delay(100); 
} 
void timeoutput(int x,int y) // 在 指 定 位 置 显 示 时 间 
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
void exchangetime(long t,int x,int y) //将长整形数据转换成换成时间字符串 
{ 
 	char s[80]; 
 	struct tm *p; 
 	p=gmtime(&t); 
 	sprintf(s,"%d-%02d-%02d",(1900+p->tm_year),(1+p->tm_mon),(p->tm_mday)); 
 	outtextxy(x,y,s); 
} 
 
/***检查 BMP 图像是否符合要求***/ void BMP_check(bmp_picture image) 
{ 
 	if(image.file.bfType!=0X4D42) //检查是否为 bmp 文件 
 	{ 
 	 	printf("Not a BMP file!\n"); 
 	 	printf("image.file.bfType=%u\n",image.file.bfType); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	if(image.info.biCompression!=0) //检查是否为压缩文件 
 	{ 
 	 	printf("Can Not Open a Compressed bmp file!\n"); 
 	 	printf("image.info.biCompression=%u\n",image.info.biCompression); 
 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	if(image.info.biBitCount!=8) //检查是否为 256 色文件 
 	{ 
 	 	printf("Not a 256 color bmp file\n"); 
 	 	printf("image.info.biBitCount=%u",image.info.biBitCount); 
 	 	delay(5000); 
 	 	exit(1); 
 	} 
 	return; 
} 
/***换页函数***/ 
void Select_page(unsigned char page) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f05; 
 	regs.x.bx=0; 
 	regs.x.dx=page; 
 	int86(0x10,&regs,&regs); 
 	return; 
} 
/***设置 SVGA 显示模式****/ 
unsigned char set_SVGA_mode(int vmode) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f02; 
 	regs.x.bx=vmode; 
 	int86(0x10,&regs,&regs); 
 	return(regs.h.ah); 
} 
/*** 获 取 SVGA 显 示 模 式 ***/ unsigned int get_SVGA_mode(void) 
{ 
 	union REGS regs; 
 	regs.x.ax=0x4f03; 
 	int86(0x10,&regs,&regs); 
 	return(regs.x.bx); 
} 
/***设置调色板***/ 
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
/***读取 BMP 图像***/ 
void Read_BMP(char *filename) // 调 用 图 片 函 数 
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
 	fread((char *)&bmp256.info,sizeof(bitmapinfo),1,fp); //将bmp 图像信息调至结构 bmp256 中 
 	fread((char *)&bmp256.palette[0],1024,1,fp);   // 将 bmp 调 色 板 调 至 结构 bmp256 中  
 	BMP_check(bmp256); //检查 bmp 文件 
 	ptrwidth=bmp256.info.biWidth; 
 	ptrheight=bmp256.info.biHeight; 
 	linebyte=((ptrwidth*(long)bmp256.info.biBitCount+31)/32)*4; //宽度修正  
 	savemode=get_SVGA_mode(); //保存 SVGA 显示模式 
 	set_SVGA_mode(0x105); //设置 SVGA 显示模式为 1024*768 
 	for(i=0;i<256;i++) //将图片调色板信息进行处理准备 
 	{ 
 	 	red[i]=bmp256.palette[i].red; 
 	 	green[i]=bmp256.palette[i].green; 
 	 	blue[i]=bmp256.palette[i].blue; 
 
 	} 
 	set_SVGA_palette(red,green,blue); //开始设置调色板 
 	fseek(fp,(long)bmp256.file.bfOffset,SEEK_SET); 
 	if((buffer=(char *)malloc(linebyte))==NULL) //分配动态内存 
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
 	 	 	if(np!=op) //SVGA 显示换页 
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
void cursor(int x,int y) /*画鼠标函数 */ 
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
 
int init_mouse(int xmi,int xma,int ymi,int yma) /* 鼠标器初始函数 */ 
{ 
 	int retcode; 
 	regs.x.ax=0; 
 	int86(51,&regs,&regs); 
 	retcode=regs.x.ax; 
 	if(retcode==0) 
 	{ 
 	 	printf("mouse absent"); 
 	 	delay(5000); 
 	 	exit(0); /* 返回 0 值表示鼠标或鼠标驱动程序未安装 */ 
 	} 
 	regs.x.ax=7; 
 	regs.x.cx=xmi; 
 
 	regs.x.dx=xma; 
 	int86(51,&regs,&regs); 
 	regs.x.ax=8; 
 	regs.x.cx=ymi; 
 	regs.x.dx=yma; 
 	int86(51,&regs,&regs); /* 表示鼠标器和驱动程序已安装 */ 
 	return retcode; 
} 
 
int read(int *mx,int *my,int *mbutt) /* 读鼠标的位置和按钮状态函数 */ 
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
 	 	if(mb==1) return 1;  /* 左键按下 */ 
 	 	if(mb==2) return 2;  /* 右键按下 */ 
 	 	return 3 ; /* 其 他 键 按 下 */ 
 	} 
 	else 
 	{ 
 	 	return 0 ; 
 	} 
} 
void newxy(int *mx,int *my,int *mbutt)/* 是否有按钮按下,若按下,在新位置画十字*/ 
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
 
void out_hz(int x,int y,char *s,int size,int part,int color) //汉字输出 
{ 
 	FILE *hzk_p; 
 	unsigned char quma,weima; 
 	unsigned long offset; 
 	unsigned char mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}; 
 	int i,j,pos; 
 	switch(size) 
 	{ 
 	 	case 12: //12 号 汉 字 输 出 
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
 	 	case 16: //16 号 汉 字 输 出 
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
 	 	case 24: //24 号 汉 字 输 出 
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
 	 	case 48: //48 号 汉 字 输 出 
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
