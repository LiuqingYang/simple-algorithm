// diskcom.cpp : 直接对比两硬盘数据

#include "stdafx.h"
#define BUFF 65536	

int readfile(HANDLE &reads,char *data)	//读取文件中一块数据
{
	DWORD dwCB;
	int num;
	ReadFile(reads,(char*)(&num),sizeof(num),&dwCB,NULL);
	if(dwCB==0) return -1;
	ReadFile(reads,data,BUFF,&dwCB,NULL);
	return num;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool issame=true,iscontinue=true;
	//int Drive1,Drive2;
	int RW=-1;	//备份恢复开关
	HANDLE Outstream; // 备份输出文件
	int j=0;	//执行64K的次数
	DWORD dwCB1,dwCB2;	//已读取字节数
	char filenametemp[1000];	//文件读取缓存
	int filecount=-1;
	while(RW!=0 && RW!=1)
	{
		cout<<"两硬盘比较备份程序\n";
		cout<<"0.备份\n1.恢复\n";
		cin>>RW;
		cout<<RW;
	}
	while(filecount<0 || filecount>9)
	{
		cout<<"请选择已有备份：\n";
		cout<<"0.无备份\n";
		cout<<"n.已有n备份(1<=n<=9)\n";
		cin>>filecount;
	}
	HANDLE *files=new HANDLE[filecount];	//已有备份存储
	int *filesblock=new int[filecount];
	char *filesdata=new char[filecount*BUFF];
	for(int i=0;i<filecount;i++)
	{
		cout<<"输入第"<<i+1<<"个备份文件名\n";
		cin>>filenametemp;
		files[i]=CreateFile(filenametemp,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if(!files[i])
		{
			cout<<"无法打开文件！\n";
			return 1;
		}
		filesblock[i]=readfile(files[i],filesdata+i*BUFF);
	}
	if(!RW)
	{
		cout<<"输入增量备份文件路径\n";
		cin>>filenametemp;
		//Outstream.open(filenametemp,ios::out|ios::binary);
		Outstream=CreateFile(filenametemp,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if(!Outstream)
		{
			cout<<"无法打开文件！\n";
			return 1;
		}
	}
	char devName1[] =("\\\\.\\E:");	
	char devName2[] =("\\\\.\\F:");
	//cout<<"输入比较的两磁盘号，如0,1,2...\n";
	//cin>>Drive1>>Drive2;
	//devName1[strlen(devName1) - 1] = devName1[strlen(devName1) - 1] + Drive1;
	//devName2[strlen(devName2) - 1] = devName2[strlen(devName2) - 1] + Drive2;
	HANDLE hDev1 = CreateFile(devName1, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING,  FILE_FLAG_SEQUENTIAL_SCAN , NULL);
	HANDLE hDev2 = CreateFile(devName2, GENERIC_READ|GENERIC_WRITE ,0 , NULL, OPEN_EXISTING,  FILE_FLAG_SEQUENTIAL_SCAN , NULL);
	if(hDev1 == INVALID_HANDLE_VALUE && hDev2 == INVALID_HANDLE_VALUE)
	{
		cout<<"无法打开磁盘！\n";
		return 2;
	}
	char szOverBuffer1[BUFF] = {0};	//64K缓存
	char szOverBuffer2[BUFF] = {0};
	while(iscontinue)
	{
		issame=true;
		//SetFilePointer(hDev1,BUFF*j,0, FILE_BEGIN);
		//SetFilePointer(hDev2,BUFF*j,0, FILE_BEGIN);
		ReadFile(hDev1, szOverBuffer1, BUFF, &dwCB1, NULL);
		ReadFile(hDev2, szOverBuffer2, BUFF, &dwCB2, NULL);
		if(dwCB1!=dwCB2)		//磁盘大小不一致
		{
			issame=false;
			break;
		}
		if(dwCB1==0 || dwCB2==0) break;	//读完磁盘
		for(int i=0;i<filecount;i++)			//增量包读取
		{
			if(filesblock[i]==-1) break;
			if(filesblock[i]==j)
			{
				for(int k=0;k<BUFF;k++)
					szOverBuffer1[k]=filesdata[i*BUFF+k];
				filesblock[i]=readfile(files[i],filesdata+i*BUFF);
			}

		}
		issame = strncmp(szOverBuffer1, szOverBuffer2, BUFF);
		/*for(int i=0;i<BUFF;i++)		//数据比较
		{  
			if(szOverBuffer1[i]!=szOverBuffer2[i])
			{
				//printf("%08X %04X:%02X %02X\n",j,i,(unsigned char)szOverBuffer1[i],(unsigned char)szOverBuffer2[i]);
				//Outstream<<setw(8)<<j<<setw(5)<<i<<':'<<(int)((unsigned char)szOverBuffer1[i])<<' '<<(int)((unsigned char)szOverBuffer2[i])<<'\n';
				issame=false;
				break;
			}
		}*/
		if(!RW)		//备份
		{
			if(!issame)		//数据不一致写入文件
			{
				//Outstream.write((char*)(&j),sizeof(j));
				WriteFile(Outstream,(char*)(&j),sizeof(j),&dwCB1,NULL);
				//Outstream.write(szOverBuffer2,BUFF);
				WriteFile(Outstream,szOverBuffer2,BUFF,&dwCB1,NULL);
			}
		}
		else		//恢复
		{
			if(!issame)
				WriteFile(hDev2, szOverBuffer1, BUFF, &dwCB2, NULL);
		}
		j++;
	}
	//if(issame) cout<<"两磁盘相同\n";
	//else cout<<"两磁盘不同\n";
	for(int i=0;i<filecount;i++)
		CloseHandle(files[i]);
	CloseHandle(hDev1);
	CloseHandle(hDev2);
	system("pause");
	return 0;
}


