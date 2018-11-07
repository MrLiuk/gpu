#ifndef _RANDOM_H
#define _RANDOM_H

long fac(int n);		//求n的阶乘

long permCount(int n, int m);	//求p(n, m)，排列问题

long combineCount(int n, int m);//求c(n, m)，组合问题

int getARandom(int minValue, int maxValue);	//生成一个在[minValue, maxValue]之间的随机数

											//生成size个互不相同的随机数，范围在[minValue, maxValue]之间
bool generateUniqueNNumbers(int a[], int size, int minValue, int maxValue);

//生成size个随机数，范围在[minValue, maxValue]之间
bool generateNNumbers(int a[], int size, int minValue, int maxValue);

//排列算法，对数组中下标为k到，m的元素进行全排列。[k, m]，count为这样的全排列的数量。
void myPerm(int list[], int k, int m, int &count, int **result);

/// 求从数组a[1..n]中任选m个元素的所有组合。
/// a[1..n]表示候选集，m表示一个组合的元素个数。
/// 返回所有组合的总数。C(n, m)
int myCombine1(int a[], int n, int m, int **result);

//格式化输出一个一维数组
void displayArray(const int *a, const int start, const int end);

//格式化输出一个二维数组
void displayArray2D(int **a, const int rows, const int cols);

//格式化输出一个二维数组
void displayArray2D(double **a, const int rows, const int cols);

//动态分配大小位size的一维数组
template<typename T>
bool allocateMemory1D(T **p, const int size);

//动态分配rows行、cols列的二维数组
template<typename T>
bool allocateMemory2D(T ***p, const int rows, const int cols);

//释放一维动态数组的空间
template<typename T>
void freeMemory1D(T **p);

//释放二维动态数组的空间
template<typename T>
void freeMemory2D(T ***p, const int rows);

template<class T>
void swap(T &a, T &b);	//交换a与b

template<class T>	//对a[start, end]进行排序
void bubbleSort(T a[], const int start, const int end, bool isASC);

//用位于[minValue, mavValue]区间的随机值填充二维数组a
template<typename T, typename V>
bool generateNNumbers2D(T **a, int rows, int cols, V minValue, V maxValue);

#endif