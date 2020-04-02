#include<bits/stdc++.h>
//#include<pthread.h>
//#include<mutex>
using namespace std;
//��װ��������
const int N = 9;
const int maxn = N * N * N + 10;
const int maxnode = maxn * 4 + maxn + 10;
//���߳�ִ�����Ϊ�˲��������������һ��ȫ�ֽ����������������
char result[1010][100];
//HANDLE cout_mutex;//������
#define NUM_THREADS 1000
mutex mtx;
clock_t startTime,endTime;

struct DLX
{
    int n,m,sz;
    int U[maxnode],D[maxnode],R[maxnode],L[maxnode],Row[maxnode],Col[maxnode];
    int H[maxn],S[maxn];
    char g[maxn];
    int ansd,ans[maxn];
    void init(int _n,int _m)
    {
        n = _n;
        m = _m;
        for(int i = 0;i <= m;i ++)
        {
            S[i] = 0;
            U[i] = D[i] = i;
            L[i] = i - 1;
            R[i] = i + 1;
        }
        R[m] = 0;
        L[0] = m;
        sz = m;
        for(int i = 1;i <= n;i ++)
        {
            H[i] = -1;
        }
    }

    void Link(int r,int c)//�����ݵ�����
    {
        ++ S[Col[++ sz] = c];
        Row[sz] = r;
        D[sz] = D[c];
        U[D[c]] = sz;
        U[sz] = c;
        D[c] = sz;
        if(H[r] < 0) H[r] = L[sz] = R[sz] = sz;
        else
        {
            R[sz] = R[H[r]];
            L[R[H[r]]] = sz;
            L[sz] = H[r];
            R[H[r]] = sz;
        }
    }

    void Remove(int c)
    {
        L[R[c]] = L[c];
        R[L[c]] = R[c];
        for(int i = D[c];i != c;i = D[i])
        {
            for(int j = R[i];j != i;j = R[j])
            {
                U[D[j]] = U[j];
                D[U[j]] = D[j];
                -- S[Col[j]];
            }
        }
    }

    void resume(int c)
    {
        for(int i = U[c];i != c;i = U[i])
        {
            for(int j = L[i];j != i;j = L[j])
            {
                ++ S[Col[U[D[j]] = D[U[j]] = j]];
            }
        }
        L[R[c]] = R[L[c]] = c;
    }

    bool Dance(int d)
    {
        if(R[0] == 0)
        {
            for(int i = 0;i < d;i ++)
            {
                g[(ans[i] - 1) / 9] = (ans[i] - 1) % 9 + '1';
            }
          /*  for(int i = 0;i < N * N;i ++)
            {
                printf("%c",g[i]);
            }
            printf("\n");*/
            return true;
        }
        int c = R[0];
        for(int i = R[0];i != 0;i = R[i])
        {
            if(S[i] < S[c])
                c = i;
        }
        Remove(c);
        for(int i = D[c];i != c;i = D[i])
        {
            ans[d] = Row[i];
            for(int j = R[i]; j != i;j = R[j]) Remove(Col[j]);
            if(Dance(d + 1)) return true;
            for(int j = L[i];j != i;j = L[j]) resume(Col[j]);
        }
        resume(c);
        return false;
    }
};

void place(int &r,int &c1,int &c2,int &c3,int &c4,int i,int j,int k)//�������û��ɶ�Ӧ����
{
    r = (i * N + j) * N + k;
    c1 = i * N + j + 1;
    c2 = N * N + i * N + k;
    c3 = N * N * 2 + j * N + k;
    c4 = N * N * 3 + ((i / 3) * 3 + (j / 3)) * N + k;
}



struct LEI//���ṹ��Ѳ��������߳�
{
    int id;
    char line[100];
};

int counter = 0;//ȫ�ֱ��������ڼ�¼�߳������Ƿ���� 

void *MyThread(void *lpParamter)
{
	
    LEI *p = (LEI *)lpParamter;
    
	char g[100];
	strcpy(g, p->line);
	DLX dlx;
	strcpy(dlx.g, p->line);
	dlx.init(N * N * N, N * N * 4);
    int r,c1,c2,c3,c4;
    for(int i = 0;i < N;i ++)
    {
        for(int j = 0;j < N;j ++)
        {
            for(int k = 1;k <= N;k ++)
            {
                if(g[i * N + j] == '0' || g[i * N + j] == '0' + k)
                {
                    place(r,c1,c2,c3,c4,i,j,k);
                    dlx.Link(r,c1);
                    dlx.Link(r,c2);
                    dlx.Link(r,c3);
                    dlx.Link(r,c4);
                }
            }
        }
    }
	dlx.Dance(0);
	strcpy(result[p->id], dlx.g);//������������Ȳ��ż����
	
	
    mtx.lock();
    counter--;
    mtx.unlock();
    
    pthread_exit(0);
	return 0L;
}

int main()
{
	startTime = clock();//��ʱ��ʼ
	ifstream in("test1000");//���ļ�����
	string line;
    string a[1010];
    int i=0;
    pthread_t tids[NUM_THREADS];
    //cout_mutex = CreateMutex(NULL, FALSE, NULL);
    LEI parameter[1010];//����Ҫ����ռ�,�߳����ظ����õĻ���ˢ������
	while (getline (in, line)) // line�в�����ÿ�еĻ��з�
	{
	    i++;
		a[i]=line;
		strcpy(parameter[i].line, line.c_str());
		parameter[i].id = i;
		LEI *p = &parameter[i];//��ָ��ṹ�崫��
		//WaitForSingleObject(cout_mutex, INFINITE);
		mtx.lock();
		counter++;//����һ���߳̾�+1��������һ���߳�-1
		mtx.unlock();
		//ReleaseMutex(cout_mutex);
		int ret = pthread_create(&tids[i], NULL, MyThread, (void*)p);
	}

    while(counter>0){//Sleep(500);
	   //for(int i=0; i <=1; i++);
	}
	
	//printf("�ȴ�%d��ʣ��\n",counter);}//ֻ�����еĶ�ִ�����ˣ����ܼ������
    //cout<<endl<<endl;
    //pthread_exit(NULL);
    for(int j=1;j<=1000;j++)
    {
        //cout<<"id = "<<j<<endl;
        //cout<<a[j]<<endl; 
        cout<<result[j]<<endl;
    }
    endTime = clock();//��ʱ����
    //cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    exit(0);
	return 0;

}
