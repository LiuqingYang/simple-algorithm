/*
Refer to:
Introduction to Algorithms

Some sort algorithms
Two binary search alorithms with a unit test for it

Liuqing Yang<yllqq#outlook.com>

Compile with VS2010+ or gcc
*/

#include <stdio.h>

template<class T> void ins_sort(T* data, int num)
{	//insert sort
	for(int j = 1; j < num; j++)
	{
		T key = data[j];
		int i = j - 1;
		/*while(i >= 0 && data[i] > key)
		{
			data[i+1] = data[i];
			i--;
		}*/
		for(; i>=0 && data[i] > key; i--)
			data[i+1] = data[i];
		data[i+1] = key;
	}
}

template<class T> void sel_sort(T* data, int num)
{	//selete sort
	for(int i = 0; i < num - 1; i++)
	{
		T max = data[i];
		int maxposi = i;
		for(int j = i; j < num; j++)
		{
			if(data[j] < max)
			{
				max = data[j];
				maxposi = j;
			}
		}
		data[maxposi] = data[i];
		data[i] = max;
	}
}

template<class T> int merge(T* data, int p, int q, int r)
{
	int inversion = 0;
	int n1 = q - p + 1; 
	int n2 = r - q;
	T *L, *R;
	L = new int[n1];
	R = new int[n2]; 
	for(int i = 0; i < n1; i++)
		L[i] = data[p + i];
	for(int j = 0; j < n2; j++)
		R[j] = data[q + j + 1];
	int i = 0;
	int j = 0;
	int k = p;
	for(; i < n1 && j < n2; k++)
	{
		if(L[i] <= R[j])
		{
			data[k] = L[i];
			i++;
		}
		else
		{
			inversion += n1 - i;
			data[k] = R[j];
			j++;
		}
	}
	if(i >= n1)
		for(; j < n2; k++)
		{
			data[k] = R[j];
			j++;
		}
	else
		for(; i < n1; k++)
		{
			data[k] = L[i];
			i++;
		}
	delete[] L;
	delete[] R;
	return inversion;
}

template<class T> int merge_sort(T* data, int p, int r)
{
	int inversion = 0;
	if(p != r)
	{
		int q = (p + r) / 2;
		inversion += merge_sort(data, p, q);
		inversion += merge_sort(data, q + 1, r);
		inversion += merge(data, p, q, r);
	}
	return inversion;
}

template<class T> int binary_search2(T* data, T key, int num)
{
	int p = 0;
	int q = num - 1;

	while(1)
	{
		if (p > q)
			return -1;
		else if (p == q)
		{
			if (data[p] == key) return p;
			else return -1;
		}
		int middle = (p + q) / 2;
		if(data[middle] == key) return middle;
		if(data[middle] < key) p = middle + 1;
		else q = middle - 1;
	}
	return -1;
}

template<class T> int binary_search(T* data, T key, int num)
{
	int p = -1;
	int q = num;
	while(q - p > 1)
	{
		int middle = (p + q) / 2;
		if(data[middle] == key) return middle;
		if(data[middle] < key) p = middle;
		else q = middle;
	}
	return -1;
}

//unit test
int binary_search_unittest()
{
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	if (binary_search(data, 1, 1) != 0)
		printf("1 number binary search fail!\n");
	else
		printf("1 number binary search success!\n");

	if (binary_search(data, 1, 2) != 0)
		printf("2 number binary search fail!\n");
	else
		printf("2 number binary search success!\n");		

	if (binary_search(data, 2, 2) != 1)
		printf("2 number binary search 2 fail!\n");
	else
		printf("2 number binary search 2 success!\n");		

	for (int i = 1; i < 9; i++)
	{
		if (binary_search(data, i, 8) != (i-1))
			printf("Even number of binary search fail(i=%d)!\n", i);
	}
	printf("Complete even number of binary search\n");

	for (int i = 1; i < 10; i++)
	{
		if (binary_search(data, i, 9) != (i-1))
			printf("Odd number of binary search fail(i=%d)!\n", i);
	}
	printf("Complete odd number of binary search\n");

	if (binary_search(data, 2, 1) != -1)
		printf("fail case 1 search fail!\n");
	else
		printf("fail case 1 search success!\n");
	if (binary_search(data, -1, 2) != -1)
		printf("fail case 2 search fail!\n");
	else
		printf("fail case 2 search success!\n");
	if (binary_search(data, -1, 8) != -1)
		printf("fail case 3 search success!\n");
	else
		printf("fail case 3 search success!\n");
	if (binary_search(data, 11, 9) != -1)
		printf("fail case 4 search fail!\n");
	else
		printf("fail case 4 search success!\n");
	
}

int main(int argc, char* argv[])
{
	int data[] = {6, 5, 4, 3, 2, 1, 8, 7};
	int i = merge_sort(data, 0, 7);
	binary_search_unittest();
	return 0;
}

