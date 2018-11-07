#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "random.h"

template<class T>
void swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

//n�Ľ׳ˣ�����n��һ���Ƚ�С�������������������������û�д�������������
long fac(int n)
{
	if (n < 0) return -1;
	else if (n < 2) return 1;
	else
	{
		long sum = 1;
		for (int i = 2; i <= n; i++)
		{
			sum *= i;
		}

		return sum;
	}
}

//��p(n, m)����������
long permCount(int n, int m)
{
	assert(n > 0 && m >= 0 && n >= m);

	long value = 1;
	for (int i = 0; i < m; i++)
	{
		value *= (n - i);
	}

	return value;
}

//��c(n, m)���������
long combineCount(int n, int m)
{
	assert(n > 0 && m >= 0 && n >= m);

	return permCount(n, m) / fac(m);
}

//[minValue, maxValue]
int getARandom(int minValue, int maxValue)
{
	if (minValue > maxValue) swap(minValue, maxValue);
	srand((unsigned)time(NULL));

	return rand() % (maxValue - minValue + 1) + minValue;
}

//����size��������ͬ�����������Χ��[minValue, maxValue]֮��
bool generateUniqueNNumbers(int a[], int size, int minValue, int maxValue)
{
	int i, j, value;

	if (minValue > maxValue) swap(minValue, maxValue);
	if (a == NULL || size < 1 || maxValue - minValue + 1 < size)
	{
		return false;
	}

	srand((unsigned)time(NULL));
	for (i = 0; i < size; i++)
	{
		value = rand() % (maxValue - minValue + 1) + minValue;

		for (j = 0; j < i; j++)
		{
			if (value == a[j])
			{
				i--;
				break;
			}
		}

		if (j >= i)
		{
			a[i] = value;
		}
	}

	return true;
}

//����size�����������Χ��[minValue, maxValue]֮��
bool generateNNumbers(int a[], int size, int minValue, int maxValue)
{
	if (minValue > maxValue) swap(minValue, maxValue);
	if (a == NULL || size < 1)
	{
		return false;
	}

	srand((unsigned)time(NULL));
	for (int i = 0; i < size; i++)
	{
		a[i] = rand() % (maxValue - minValue + 1) + minValue;
	}

	return true;
}

template<typename T, typename V>
bool generateNNumbers2D(T **a, int rows, int cols, V minValue, V maxValue)
{
	//assert(a != NULL && rows > 0 && cols > 0);
	if (a == NULL || rows < 1 && cols < 1) return false;
	if (minValue > maxValue) swap(minValue, maxValue);

	srand((unsigned)time(NULL));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			a[i][j] = rand() % (int)(maxValue - minValue + 1) + minValue;
		}
	}

	return true;
}

//�����㷨�����������±�Ϊk����m��Ԫ�ؽ���ȫ���С�[k, m]��countΪ������ȫ���е�������
void perm(int list[], int k, int m, int &count)
{
	int i;

	if (k > m)
	{
		for (i = 0; i <= m; i++)
			printf("%d ", list[i]);

		printf("\n");
		count++;
	}

	else
	{
		for (i = k; i <= m; i++)
		{
			swap(list[k], list[i]);
			perm(list, k + 1, m, count);
			swap(list[k], list[i]);
		}
	}
}

//�����㷨�����������±�Ϊk����m��Ԫ�ؽ���ȫ���С�[k, m]��countΪ������ȫ���е�������
void myPerm(int list[], int k, int m, int &count, int **result)
{
	int i;

	if (k > m)
	{
		for (i = 0; i <= m; i++)
			result[count][i] = list[i];

		count++;
	}

	else
	{
		for (i = k; i <= m; i++)
		{
			swap(list[k], list[i]);
			myPerm(list, k + 1, m, count, result);
			swap(list[k], list[i]);
		}
	}
}

//����㷨
/// �������a[1..n]����ѡm��Ԫ�ص�������ϡ�
/// a[1..n]��ʾ��ѡ����nΪ��ѡ����С��n>=m>0��
/// b[1..M]�����洢��ǰ����е�Ԫ��(����洢����Ԫ���±�)��
/// ����M��ʾ����������һ�������Ԫ�صĸ�����M=m��������������������������
void combine(int a[], int n, int m, int b[], const int M)
{
	for (int i = n; i >= m; i--)  // ע�������ѭ����Χ
	{
		b[m - 1] = i - 1;

		if (m > 1)
			combine(a, i - 1, m - 1, b, M);
		else           // m == 1, ���һ�����
		{
			for (int j = M - 1; j >= 0; j--)
				//cout << a[b[j]] << " ";
				printf("%d ", a[b[j]]);

			//cout << endl;
			printf("\n");
		}
	}
}

/// �������a[1..n]����ѡm��Ԫ�ص�������ϡ�
/// a[1..n]��ʾ��ѡ����m��ʾһ����ϵ�Ԫ�ظ�����
/// ����������ϵ�������C(n, m)
int combine1(int a[], int n, int m)
{
	m = m > n ? n : m;
	int* order = new int[m + 1];
	for (int i = 0; i <= m; i++)
		order[i] = i - 1;            // ע������order[0]=-1������Ϊѭ���жϱ�ʶ

	int count = 0;
	int k = m;
	bool flag = true;           // ��־�ҵ�һ����Ч���
	while (order[0] == -1)
	{
		if (flag)                   // �������Ҫ������
		{
			for (int i = 1; i <= m; i++)
				//cout << a[order[i]] << " ";
				printf("%d ", a[order[i]]);

			//cout << endl;
			printf("\n");

			count++;
			flag = false;
		}
		order[k]++;                // �ڵ�ǰλ��ѡ���µ�����
		if (order[k] == n)          // ��ǰλ���������ֿ�ѡ������
		{
			order[k--] = 0;
			continue;
		}

		if (k < m)                  // ���µ�ǰλ�õ���һλ�õ�����          
		{
			order[++k] = order[k - 1];
			continue;
		}

		if (k == m)
			flag = true;
	}
	delete[] order;
	return count;
}

int myCombine1(int a[], int n, int m, int **result)
{
	assert(a != NULL && n > 0 && m > 0 && result != NULL);

	m = m > n ? n : m;
	int* order = new int[m + 1];
	for (int i = 0; i <= m; i++)
		order[i] = i - 1;            // ע������order[0]=-1������Ϊѭ���жϱ�ʶ

	int count = 0;
	int k = m;
	bool flag = true;           // ��־�ҵ�һ����Ч���
	while (order[0] == -1)
	{
		if (flag)                   // �������Ҫ������
		{
			for (int i = 1; i <= m; i++)
				result[count][i - 1] = a[order[i]];
			//cout << a[order[i]] << " ";
			//printf("%d ", a[order[i]]);

			//cout << endl;
			//printf("\n");

			count++;
			flag = false;
		}
		order[k]++;                // �ڵ�ǰλ��ѡ���µ�����
		if (order[k] == n)          // ��ǰλ���������ֿ�ѡ������
		{
			order[k--] = 0;
			continue;
		}

		if (k < m)                  // ���µ�ǰλ�õ���һλ�õ�����          
		{
			order[++k] = order[k - 1];
			continue;
		}

		if (k == m)
			flag = true;
	}
	delete[] order;
	return count;
}

template<class T>	//��a[start, end]��������
void bubbleSort(T a[], const int start, const int end, bool isASC)
{
	assert(a != NULL && start >= 0 && start <= end);
	bool isOver;

	if (isASC)
	{
		for (int i = start; i < end - start; i++)
		{
			isOver = true;
			for (int j = start; j < end - i + start; j++)
			{
				if (a[j] > a[j + 1])
				{
					swap(a[j], a[j + 1]);
					isOver = false;
				}
			}
			if (isOver) break;
		}
	}
	else
	{
		for (int i = start; i < end - start; i++)
		{
			isOver = true;
			for (int j = start; j < end - i + start; j++)
			{
				if (a[j] < a[j + 1])
				{
					swap(a[j], a[j + 1]);
					isOver = false;
				}
			}
			if (isOver) break;
		}
	}
}

void displayArray(const int *a, const int start, const int end)
{
	assert(a != NULL);
	for (int i = start; i <= end; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

void displayArray2D(int **a, const int rows, const int cols)
{
	assert(a != NULL && rows > 0 && cols > 0);

	for (int i = 0; i < rows; i++)
	{
		printf("%d: ", i + 1);
		for (int j = 0; j < cols; j++)
		{
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

void displayArray2D(double **a, const int rows, const int cols)
{
	assert(a != NULL && rows > 0 && cols > 0);

	for (int i = 0; i < rows; i++)
	{
		printf("%d: ", i + 1);
		for (int j = 0; j < cols; j++)
		{
			printf("%lf ", a[i][j]);
		}
		printf("\n");
	}
}

template<typename T>
bool allocateMemory1D(T **p, const int size)
{
	*p = NULL;
	*p = (T *)malloc(size * sizeof(T));

	if (*p == NULL) return false;
	return true;
}

template<typename T>
bool allocateMemory2D(T ***p, const int rows, const int cols)
{
	*p = NULL;
	*p = (T **)malloc(rows * sizeof(T *));
	if (*p == NULL) return false;

	memset(*p, 0, sizeof(T *) * rows);
	for (int i = 0; i < rows; i++)
	{
		(*p)[i] = (T *)malloc(cols * sizeof(T));
		if ((*p)[i] == NULL) return false;
	}

	return true;
}

template<typename T>
void freeMemory1D(T **p)
{
	if (*p != NULL)
	{
		free(*p);
		*p = NULL;
	}
}

template<typename T>
void freeMemory2D(T ***p, const int rows)
{
	if (*p != NULL)
	{
		for (int i = 0; i < rows; i++)
		{
			if ((*p)[i] != NULL)
			{
				free((*p)[i]);
				(*p)[i] = NULL;
			}
		}

		free(*p);
		*p = NULL;
	}
}

extern void randomTest()
{
	int a = 23, b = 2;
	printf("before swap: a = %d, b = %d\n", a, b);

	swap(a, b);
	printf("after swap: a = %d, b = %d\n", a, b);

	int value = getARandom(a, b);
	printf("random value = %d\n", value);

	const int size = 10;
	int array[size];
	bool ret;

	ret = generateUniqueNNumbers(array, size, 1, 10);
	if (!ret) fprintf(stderr, "%s\n", "failed!");
	else
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d ", array[i]);
		}
		printf("\n");
	}

	bubbleSort(array, 0, sizeof(array) / sizeof(*array) - 1, false);
	displayArray(array, 0, sizeof(array) / sizeof(*array) - 1);
	getchar();

	ret = generateUniqueNNumbers(array, size, 1, 9);
	if (!ret) fprintf(stderr, "%s\n", "failed!");
	else
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d ", array[i]);
		}
		printf("\n");
	}

	bubbleSort(array, 0, sizeof(array) / sizeof(*array) - 1, false);
	displayArray(array, 0, sizeof(array) / sizeof(*array) - 1);
	getchar();

	ret = generateNNumbers(array, size, 1, 9);
	if (!ret) fprintf(stderr, "%s\n", "failed!");
	else
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d ", array[i]);
		}
		printf("\n");
	}

	bubbleSort(array, 0, sizeof(array) / sizeof(*array) - 1, false);
	displayArray(array, 0, sizeof(array) / sizeof(*array) - 1);
	getchar();

	int list[] = { 1, 2, 3, 4, 5 };
	int count = 0;
	perm(list, 2, 4, count);
	printf("total:%d\n", count);

	const int N = 4;
	const int M = 1;
	int aa[N];

	for (int i = 0; i < N; i++)
		aa[i] = i + 1;

	// ���ݷ���
	//cout << combine1(aa, N, 3) << endl;
	printf("count: %d\n", combine1(aa, N, 1));
	getchar();

	// �ݹ鷽�� C(N, M)
	int bb[M];
	combine(aa, N, M, bb, M);

	for (int i = -1; i < 11; i++)
	{
		printf("%d! = %ld\n", i, fac(i));
	}

	for (int i = 0; i <= 10; i++)
	{
		printf("p(10, %d) = %ld\n", i, permCount(10, i));
	}

	for (int i = 0; i <= 10; i++)
	{
		printf("c(10, %d) = %ld\n", i, combineCount(10, i));
	}

	//test for allocate Memory and free Memory
	int *iArray1D;
	int SIZE = 12;
	allocateMemory1D(&iArray1D, SIZE);
	generateNNumbers(iArray1D, SIZE, 0, 6);
	displayArray(iArray1D, 0, SIZE - 1);
	freeMemory1D(&iArray1D);
	getchar();

	double **dArray2D;
	int rows = 10, cols = 6;
	allocateMemory2D(&dArray2D, rows, cols);
	generateNNumbers2D(dArray2D, rows, cols, rows, cols);
	displayArray2D(dArray2D, rows, cols);
	freeMemory2D(&dArray2D, rows);

	//test myCombine1
	//int myCombine1(int a[], int n, int m, int **result)
	printf("start to test myCombine1.\n");
	int **iArray2D;
	int n = 12, m = 2;
	rows = combineCount(n, m);
	cols = m;

	allocateMemory1D(&iArray1D, n);
	generateUniqueNNumbers(iArray1D, n, 1, n);
	displayArray(iArray1D, 0, n - 1);
	getchar();

	allocateMemory2D(&iArray2D, rows, cols);
	int totalCounts = myCombine1(iArray1D, n, m, iArray2D);
	printf("totalCounts = %d\n", totalCounts);
	printf("rows = %d\n", rows);
	getchar();
	displayArray2D(iArray2D, rows, cols);

	freeMemory1D(&iArray1D);
	freeMemory2D(&dArray2D, rows);

	//test myPerm
	//void myPerm(int list[], int k, int m, int &count, int **result)
	printf("start to test myPerm.\n");
	getchar();
	int k = 0;
	n = 12, m = 3;
	rows = permCount(m - k + 1, m - k + 1);
	cols = m - k + 1;
	totalCounts = 0;

	allocateMemory1D(&iArray1D, n);
	generateUniqueNNumbers(iArray1D, n, 0, n - 1);
	displayArray(iArray1D, 0, n - 1);
	getchar();

	allocateMemory2D(&iArray2D, rows, cols);
	myPerm(iArray1D, k, m, totalCounts, iArray2D);
	printf("totalCounts = %d\n", totalCounts);
	printf("rows = %d\n", rows);
	getchar();
	displayArray2D(iArray2D, rows, cols);

	freeMemory1D(&iArray1D);
	freeMemory2D(&iArray2D, rows);
}