/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2020. All rights reserved.
 ******************************************************************************************/

#pragma once

#pragma warning(disable : 4996 4800)
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "release.h"

class Bitmap { //λͼBitmap��
private:
   char* M; int N; //����ͼ����ŵĿռ�M[]������ΪN*sizeof(char)*8����
protected:
   void init ( int n ) { M = new char[N = ( n + 7 ) / 8]; memset ( M, 0, N ); }
public:
   Bitmap ( int n = 8 ) { init ( n ); } //��ָ����Ĭ�Ϲ�ģ��������ͼ��Ϊ������ʱѡ�ý�С��Ĭ��ֵ��
   Bitmap ( char* file, int n = 8 ) //��ָ����Ĭ�Ϲ�ģ����ָ���ļ��ж�ȡ����ͼ
   {  init ( n ); FILE* fp = fopen ( file, "r" ); fread ( M, sizeof ( char ), N, fp ); fclose ( fp );  }
   ~Bitmap() { delete [] M; M = NULL; } //����ʱ�ͷű���ͼ�ռ�

   void set   ( int k ) { expand ( k );        M[k >> 3] |=   ( 0x80 >> ( k & 0x07 ) ); }
   void clear ( int k ) { expand ( k );        M[k >> 3] &= ~ ( 0x80 >> ( k & 0x07 ) ); }
   bool test  ( int k ) { expand ( k ); return M[k >> 3] &    ( 0x80 >> ( k & 0x07 ) ); }

   void dump ( char* file ) //��λͼ���嵼����ָ�����ļ����Ա�Դ˺����λͼ������ʼ��
   {  FILE* fp = fopen ( file, "w" ); fwrite ( M, sizeof ( char ), N, fp ); fclose ( fp );  }
   char* bits2string ( int n ) { //��ǰnλת��Ϊ�ַ�������
      expand ( n - 1 ); //��ʱ���ܱ����ʵ����λΪbitmap[n - 1]
      char* s = new char[n + 1]; s[n] = '\0'; //�ַ�����ռ�ռ䣬���ϲ�����߸����ͷ�
      for ( int i = 0; i < n; i++ ) s[i] = test ( i ) ? '1' : '0';
      return s; //�����ַ���λ��
   }
   void expand ( int k ) { //�������ʵ�Bitmap[k]�ѳ��磬��������
      if ( k < 8 * N ) return; //���ڽ��ڣ���������
      int oldN = N; char* oldM = M;
      init ( 2 * k ); //���������ƣ��ӱ�����
      memcpy_s ( M, N, oldM, oldN ); delete [] oldM; //ԭ����ת�����¿ռ�
   }
   /*DSA*/
   /*DSA*/   void print ( int n ) //��λ��ӡ�Լ���λͼ���ݣ��Ǳ���ӿ�
   /*DSA*/   {  expand ( n ); for ( int i = 0; i < n; i++ ) printf ( test ( i ) ? "1" : "0" );  }
};
