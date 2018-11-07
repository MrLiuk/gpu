
#include <stdio.h>  
#include <stdlib.h>  
#include <cuda_runtime.h>
#include"Lock.h"
#include"random.h"
//#include <guiddef.h>
#include "device_launch_parameters.h"

//�쳣���
static void HandleError(cudaError_t err,
	const char *file,
	int line) {
	if (err != cudaSuccess) {
		printf("%s in %s at line %d\n", cudaGetErrorString(err),
			file, line);
		exit(EXIT_FAILURE);
	}
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))
#define TESTCPPDLL_API __declspec(dllexport)
#define CHECK(res) if(res!=cudaSuccess){exit(-1);}
extern "C" TESTCPPDLL_API float* __stdcall Run(float* prices, int rows, int cols, bool savefile);

using namespace std;

//--�豸������ͬ����--//
static int selectGpuCount;						//ѡ���gpu����
static int *gpuDeviceID = NULL;					//ѡ���gpu���豸ID��
static int *gpuDeviceIsBusy = NULL;				//GPU�豸�Ƿ�æµ
static int StreamNum = 0;                       //3�״�������������
Mutex gpuDevLock;
CLock gpuLock(gpuDevLock);
//-------------------//

//��ʼ��CUDA�豸
void iniCUDA() {
	int gpuCount, i;
	int dev;
	HANDLE_ERROR(cudaGetDeviceCount(&gpuCount));
	for (dev = 0; dev < gpuCount; dev++)
	{
		cudaDeviceProp deviceProp;
		cudaGetDeviceProperties(&deviceProp, dev);
		StreamNum = deviceProp.multiProcessorCount*8;
	}


	if (gpuCount < 1)
	{
		fprintf(stderr, "There is no GPU device.");
		exit(1);
	}
	selectGpuCount = gpuCount;
	allocateMemory1D(&gpuDeviceID, selectGpuCount);
	allocateMemory1D(&gpuDeviceIsBusy, selectGpuCount);
	generateUniqueNNumbers(gpuDeviceID, selectGpuCount, 0, selectGpuCount - 1);
	memset(gpuDeviceIsBusy, 0, selectGpuCount * sizeof(*gpuDeviceIsBusy));
}
//DLL��װ����
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		//��ʼCUDA
		iniCUDA();
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

__global__ void tensor(float **da, float ** preda, long rows, long  cols, float *D, int ite)
{
	long row = blockDim.y*blockIdx.y + threadIdx.y;
	long col = blockDim.x*blockIdx.x + threadIdx.x;

	if (row < rows && row>(2 * cols) && col > 1 && col < cols + 1)
		if (ite < col)
		{
			float re = 0;
			float rate = 0;
			for (int j = 0; j < col; j++)
			{
				int pos = j + ite;
				re += preda[row - pos][0];
			}
			rate = (da[row - ite][0] - da[row - ite - col][0]) / da[row - ite - col][0];
			//�����豸֧��ԭ�Ӳ������ɽ���3�׶ε������в���
			atomicAdd(&da[row][col - 1], 100000000 * (re - rate));
			//da[row][col - 1] += 100000000 * (re - rate);
		}
}

__global__ void tensor_data(float **da, unsigned int rows, unsigned int cols, const float *D)
{
	unsigned int row = blockDim.y*blockIdx.y + threadIdx.y;
	//unsigned int col = blockDim.x*blockIdx.x + threadIdx.x;
	if (row < rows)
		if (row > 1)
			da[row][0] = (D[row] - D[row - 1]) / D[row - 1];
		else
			da[row][0] = D[row];
}

__global__ void copypricedata(float **da, unsigned int rows, unsigned int cols, const float *D)
{
	unsigned int row = blockDim.y*blockIdx.y + threadIdx.y;
	//unsigned int col = blockDim.x*blockIdx.x + threadIdx.x;
	if (row < rows)
		da[row][0] = D[row];
}

//�ں˼���
float* KernelComputer(float* prices, int rows, int cols, bool savefile)
{
	printf("[tensor of  %d elements]\n", rows);
	cudaError_t err = cudaSuccess;
	cudaDeviceReset();
	size_t size = rows * sizeof(float);
	float *h_close = (float *)malloc(size);
	for (int i = 0; i < rows; i++)
		h_close[i] = prices[i];
	float *d_close = NULL;
	cudaMalloc((void **)&d_close, size);
	cudaMemcpy(d_close, h_close, size, cudaMemcpyHostToDevice);

	// Launch the Vector Add CUDA Kernel
	int threadsPerBlock = 1024;
	int blocksPerGrid = (rows + threadsPerBlock - 1) / threadsPerBlock;
	printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
	//--1,2�������в���--//
	//1��
	static float **da = NULL;
	float **ha = NULL;
	float *dc = NULL;
	float *hc = NULL;
	//2��
	float **da1 = NULL;
	float **ha1 = NULL;
	float *dc1 = NULL;
	//------------------//
	cudaError_t res;
	//--����׼��--//
	//1��
	int r, c;
	res = cudaMalloc((void**)(&da), rows * sizeof(float*)); CHECK(res);
	res = cudaMalloc((void**)(&dc), rows*cols * sizeof(float)); CHECK(res);

	ha = (float**)malloc(rows * sizeof(float*));
	hc = (float*)malloc(rows*cols * sizeof(float));

	for (r = 0; r < rows; r++)
		ha[r] = dc + r*cols;

	dim3 dimBlock(32, 32);
	dim3 dimGrid((cols + dimBlock.x - 1) / (dimBlock.x), (rows + dimBlock.y - 1) / (dimBlock.y));
	//2��
	res = cudaMalloc((void**)(&da1), rows * sizeof(float*)); CHECK(res);
	res = cudaMalloc((void**)(&dc1), rows*cols * sizeof(float)); CHECK(res);
	ha1 = (float**)malloc(rows * sizeof(float*));
	for (r = 0; r < rows; r++)
		ha1[r] = dc1 + r*cols;

	printf("preparing data...\n");
	//-------------------//
	//--1,2�����в���--//
    //��������������
	cudaStream_t stream[2];
	for (int i = 0; i < 2; i++) {
		cudaStreamCreate(&stream[i]);
	}
	//1��
	res = cudaMemcpyAsync((void*)(da), (void*)(ha), rows * sizeof(float*), cudaMemcpyHostToDevice,stream[0]); CHECK(res);
	copypricedata << <dimGrid, dimBlock,0,stream[0] >> > (da, rows, cols, d_close);
	//2��
	res = cudaMemcpyAsync((void*)(da1), (void*)(ha1), rows * sizeof(float*), cudaMemcpyHostToDevice,stream[1]); CHECK(res);
	tensor_data << <dimGrid, dimBlock,0,stream[1] >> > (da1, rows, cols, d_close);
	//-----------------//
	//ͬ��(����������Դ��Ҫ�ⲿ�ֽ��)
	res = cudaDeviceSynchronize();
	if (res != cudaSuccess)
	{
		fprintf(stderr, "cudaThreadSynchronize returned error code %d after launching addKernel!\n", res);
		exit(EXIT_FAILURE);
	}
	//������
	for (int i = 0; i < 2; i++) {
		cudaStreamDestroy(stream[i]);
	}
	//3�׼���
	cudaStream_t *streamT=new cudaStream_t[StreamNum];
	for (int i = 0; i < StreamNum; i++) {
		cudaStreamCreate(&streamT[i]);
	}
	printf("computing tensor..\n");
	for (int ite = 0; ite < cols; ++ite)
	{
		tensor << <dimGrid, dimBlock,0,streamT[ite%StreamNum]>> > (da, da1, rows, cols, d_close, ite);
		if((ite%StreamNum)==(StreamNum-1))cudaDeviceSynchronize();
	}
	res = cudaMemcpy((void*)(hc), (void*)(dc), rows*cols * sizeof(float), cudaMemcpyDeviceToHost); CHECK(res);

	if (savefile) {
		printf("saving to file...\n");
		FILE * output = fopen("output-gpu.txt", "w");
		for (r = 0; r < rows; r++)
		{
			for (c = 0; c < cols; c++)
				fprintf(output, "%f\t", hc[r*cols + c]);
			fprintf(output, "\n");
		}
		fclose(output);
	}
	if (err != cudaSuccess)
	{
		fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	//������
	for (int i = 0; i < StreamNum; i++) {
		cudaStreamDestroy(streamT[i]);
	}
	delete[] streamT;
	cudaFree(d_close);
	cudaFree((void*)da);
	cudaFree((void*)dc);
	free(h_close);
	free(ha);
	printf("ok...\n");
	return hc;
}

TESTCPPDLL_API float* __stdcall Run(float* prices, int rows, int cols, bool savefile)
{
	//���ָ��
	float *runPort = NULL;
	int i = 0;
	int aVacantGpu;
	bool retry = true;
	while (retry)
	{
		if (getARandom(0, 128) % 2 == 0)
		{
			gpuLock.Lock();
			for (i = 0; i < selectGpuCount; i++)
			{
				if (!gpuDeviceIsBusy[gpuDeviceID[i]])	//�ҵ�һ�����е�gpu�豸
				{
					aVacantGpu = gpuDeviceID[i];
					gpuDeviceIsBusy[gpuDeviceID[i]] = 1;//����ռ�ñ�־�������豸״̬Ϊæ��

					gpuLock.Unlock();					//�ǵü�ʱ�ͷ���
					retry = false;

					//printf("Thread have get a vacant gpu device%d.\n", aVacantGpu);
					break;
				}
			}
			if (i >= selectGpuCount)					//����gpu�豸����ռ����
			{
				gpuLock.Unlock();						//��ʱ�ͷ���

														//printf("No gpu device is vacant, thread  have to wait.\n");

				//Sleep(1);							//˯��һ����������
				retry = true;
			}
		}
		else
		{
			gpuLock.Lock();
			for (i = selectGpuCount - 1; i >= 0; i--)
			{
				if (!gpuDeviceIsBusy[gpuDeviceID[i]])	//�ҵ�һ�����е�gpu�豸
				{
					aVacantGpu = gpuDeviceID[i];
					gpuDeviceIsBusy[gpuDeviceID[i]] = 1;//����ռ�ñ�־�������豸״̬Ϊæ��

					gpuLock.Unlock();					//�ǵü�ʱ�ͷ���
					retry = false;

					//printf("Thread have get a vacant gpu device%d.\n", aVacantGpu);
					break;
				}
			}
			if (i < 0)									//����gpu�豸����ռ����
			{
				gpuLock.Unlock();						//��ʱ�ͷ���

				//Sleep(1);							//˯��һ����������
				retry = true;

				//printf("No gpu device is vacant, thread have to wait.\n");
			}
		}
	}
	cudaSetDevice(aVacantGpu);
	//�ں˼���
	runPort=KernelComputer(prices,rows,cols,savefile);
	//�ǵü�ʱ�黹gpu�����ÿ��б�־Ҳ��Ҫͬ������Ϊ�����������̶߳�ȡ
	//gpuLockһ����������gpuDeviceIsBusy
	gpuLock.Lock();
	gpuDeviceIsBusy[aVacantGpu] = 0;
	gpuLock.Unlock();
	return runPort;
}