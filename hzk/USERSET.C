#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

#define USERMAX 10

int zhuce(master *p,user *head);
int login(user *p,char *id,char *key);


int zhuce(master *p,char *id,char *key,char *key2)
{
 FILE *pfile;
 char temp[20]="USER\\";
 user *current;
 int flag;
 user *head;
 clearbuffer();
 if(id[0]=='\0')
 {
    func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
    func_load_pic(160,140,160+319,140+199,"TEMP\\zcsb1.dat");  
    getch();
    func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
    return 2;  
  }
 flag=checksame(id);//¼ìÖØ
 if(flag==0)	//×¢²áÊ§°Ü£¬ÓÃ»§ÃûÒÑ´æÔÚ
 {
  func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
  func_load_pic(160,140,160+319,140+199,"TEMP\\zcsb1.dat");  
  getch();
  func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
  return 2;
 }
 if(strcmp(key,key2)!=0)	//×¢²áÊ§°Ü£¬Á½´ÎÃÜÂë²»Ò»
      {
	func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
  	func_load_pic(160,140,160+319,140+199,"TEMP\\zcsb2.dat");  
	getch();
	func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
        return 2;
       }
 strcat(temp,id);
 strcat(temp,".dat");
 if((pfile=(fopen(temp,"wb")))==NULL)
  {
    outtext("can not open the first file");
    return 0;
   }	//×¢²á³É¹¦
  func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
  func_load_pic(160,140,160+319,140+199,"TEMP\\zccg.dat");  
  getch();
  func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
 readuser(&head);
 current=head;
 if(current!=NULL)
  {
    while(current->pnext!=NULL)
      {
        current=current->pnext;
       }//while
    current->pnext=malloc(sizeof(user));
    if(current->pnext==NULL){exit(0);}
    current=current->pnext;
   }//if
 else
   {
      head=malloc(sizeof(user));
      current=head;
      if(current==NULL){exit(0);}
    }
 inituser(current,id,key);
 suf(head);
 fwrite(current,sizeof(user),1,pfile);
 freeuser(head);
 fclose(pfile);
 return 0;
}
int login(user *p,char *id,char *key)
{
 user player;
 FILE *puser;
 char temp[20];
 strcpy(temp,"USER\\");
 strcat(temp,id);
 strcat(temp,".dat");
 if((puser=fopen(temp,"r"))==NULL)
 {
  func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
  func_load_pic(160,140,160+319,140+199,"TEMP\\dlsb.dat");  
  getch();
  func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
  return 0;//µÇÂ½Ê§°Ü
 }
 fread(p,sizeof(user),1,puser);
 fclose(puser);
 if(strcmp(id,p->id)==0&&strcmp(key,p->key)==0)
  {
   return 3;//µÇÂ¼³É¹¦
  }
  else
  {
   func_save_pic(160,140,160+319,140+199,"TEMP\\temp.dat");
   func_load_pic(160,140,160+319,140+199,"TEMP\\dlsb.dat");  
   getch();
   func_load_pic(160,140,160+319,140+199,"TEMP\\temp.dat");  
   return 0;//Ê§°Ü
  }
}