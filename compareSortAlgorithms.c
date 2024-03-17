#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}


void heapify(int arr[], int n, int i) { 
    int temp, maximum, left_index, right_index; 

    maximum = i; 
    right_index = 2 * i + 2; 
    left_index = 2 * i + 1; 
  
    // left greater than max, the max is the left index
    if (left_index < n && arr[left_index] > arr[maximum]) 
        maximum = left_index; 
  
    // right greather than max, the max is the left index
    if (right_index < n && arr[right_index] > arr[maximum]) 
        maximum = right_index; 
  
    //check if swap needed
    if (maximum != i) { 
        temp = arr[i]; 
        arr[i] = arr[maximum]; 
        arr[maximum] = temp; 
        heapify(arr, n, maximum); 
    } 
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n){
	int i, temp; 
  
	//recursively call heapify
    for (i = n / 2 - 1; i >= 0; i--) { 
        heapify(arr, n, i); 
    } 
    for (i = n - 1; i > 0; i--) { 
        temp = arr[0]; 
        arr[0] = arr[i]; 
        arr[i] = temp; 
        heapify(arr, i, 0); 
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) { 
        
		//midpoint of array
        int m = (l+r) / 2; 
  
        // Sort first and second halves 
        mergeSort(pData, l, m); 
        mergeSort(pData, m + 1, r); 
  
        //merging subarrays together
		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;
		// create temp arrays 
		int *L = Alloc(n1*sizeof(int));
		int *R = Alloc(n2*sizeof(int));
		// Copy data to temp arrays 
		for (i = 0; i < n1; i++)
			L[i] = pData[l + i];
		for (j = 0; j < n2; j++)
			R[j] = pData[m + 1+ j];
		
		i = 0; 
		j = 0; 
		k = l; 
		//continuously split arrays
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]){
				pData[k] = L[i];
				i++;
			}
			else{
				pData[k] = R[j];
				j++;
			}
		k++;
		}

		while (i < n1){
			pData[k] = L[i];
			i++;
			k++;
		}

		while (j < n2){
			pData[k] = R[j];
			j++;
			k++;
		}
		DeAlloc(L);
		DeAlloc(R); 
    } 
}


// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n){
	
	int i, item, j;
	//moving elements if they are greater than item
	for (i = 1; i < n; i++){
		item = pData[i];
		for(j=i-1; j>=0; j--){
			if(pData[j]>item)
			pData[j+1] = pData[j];
			else
				break;
		}
		pData[j+1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n){
	
	int i, j,temp =0;
	for (i = 0; i < n-1; i++){
		for (j = 0; j < n-i-1; j++){
			//if adjacent is less swap
			if (pData[j] > pData[j+1]){
				temp=pData[j];
				pData[j]=pData[j+1];
				pData[j+1]=temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n){	
	
	int i, j, min_idx, temp;
	//move boundaries
	for (i = 0; i < n-1; i++){
		//min element set to first index
		min_idx = i;
		for (j = i+1; j < n; j++)
			if (pData[j] < pData[min_idx])
				min_idx = j;
			//swap first and min
			temp = pData[i];
			pData[i] = pData[min_idx];
			pData[min_idx] = temp;
	}
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}


// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz );
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
