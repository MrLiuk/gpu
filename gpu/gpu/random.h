#ifndef _RANDOM_H
#define _RANDOM_H

long fac(int n);		//��n�Ľ׳�

long permCount(int n, int m);	//��p(n, m)����������

long combineCount(int n, int m);//��c(n, m)���������

int getARandom(int minValue, int maxValue);	//����һ����[minValue, maxValue]֮��������

											//����size��������ͬ�����������Χ��[minValue, maxValue]֮��
bool generateUniqueNNumbers(int a[], int size, int minValue, int maxValue);

//����size�����������Χ��[minValue, maxValue]֮��
bool generateNNumbers(int a[], int size, int minValue, int maxValue);

//�����㷨�����������±�Ϊk����m��Ԫ�ؽ���ȫ���С�[k, m]��countΪ������ȫ���е�������
void myPerm(int list[], int k, int m, int &count, int **result);

/// �������a[1..n]����ѡm��Ԫ�ص�������ϡ�
/// a[1..n]��ʾ��ѡ����m��ʾһ����ϵ�Ԫ�ظ�����
/// ����������ϵ�������C(n, m)
int myCombine1(int a[], int n, int m, int **result);

//��ʽ�����һ��һά����
void displayArray(const int *a, const int start, const int end);

//��ʽ�����һ����ά����
void displayArray2D(int **a, const int rows, const int cols);

//��ʽ�����һ����ά����
void displayArray2D(double **a, const int rows, const int cols);

//��̬�����Сλsize��һά����
template<typename T>
bool allocateMemory1D(T **p, const int size);

//��̬����rows�С�cols�еĶ�ά����
template<typename T>
bool allocateMemory2D(T ***p, const int rows, const int cols);

//�ͷ�һά��̬����Ŀռ�
template<typename T>
void freeMemory1D(T **p);

//�ͷŶ�ά��̬����Ŀռ�
template<typename T>
void freeMemory2D(T ***p, const int rows);

template<class T>
void swap(T &a, T &b);	//����a��b

template<class T>	//��a[start, end]��������
void bubbleSort(T a[], const int start, const int end, bool isASC);

//��λ��[minValue, mavValue]��������ֵ����ά����a
template<typename T, typename V>
bool generateNNumbers2D(T **a, int rows, int cols, V minValue, V maxValue);

#endif