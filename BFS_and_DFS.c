#include<stdio.h>
#include<stdlib.h>

#define MAX_V 100   //  最大顶点数

//邻接表节点
typedef struct AdijLisNode 
{
    int dest;                   //节点代号
    struct AdijLisNode *next;   //下一个节点地址
} AdijLisNode;

//图结构（同时维护矩阵和邻接表）
typedef struct Graph
{
    int V;                          //顶点数
    int adjMatrix[MAX_V][MAX_V];    //邻接矩阵
    AdijLisNode *adjList[MAX_V];    //邻接表（指针数组）
} Graph;

//队列结构（用于BFS）
//这里使用的是数组队列结构，静态分配内存，适用于已知图的顶点数的情况
//链表队列结构适用于图规模未知的情况
typedef struct Queue
{
    int items[MAX_V];   //存储队列元素
    int front;  //队头
    int rear;   //队尾
} Queue;

//--------------------构建图的逻辑结构--------------------

//创建图（创建各个顶点并初始化）
Graph *createGraph(int V)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->V = V;
    for(int i = 0; i < V; i++)
    {
        for(int j = 0;j < V; j++)
        {
            g->adjMatrix[i][j] = 0; //矩阵初始化为0；
        }
        g->adjList[i] = NULL;       //链表头初始化为 NULL
    }
    return g;
}

//添加边（无向图）
void addEdge(Graph *g, int src, int dest)
{
    //src 与 dest 是要连接的两个顶点
    if(src < 0 || src >= g->V || dest < 0 || dest >= g->V)
    {
        //输入顶点不存在当前图
        return;
    }

    //1、更新邻接矩阵
    //邻接矩阵关于对角线对称
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;

    //2、更新邻接表（头插法）
    AdijLisNode *newNode1 = (AdijLisNode *)malloc(sizeof(AdijLisNode));
    newNode1->dest = dest;
    //adjList[src]是一个链表的头节点，头插法要将插入节点的next指向当前的头节点
    newNode1->next = g->adjList[src];
    //更新 新插入的节点为头节点
    g->adjList[src] = newNode1;

    //对称操作，将与顶点src相连的 顶点dest的链表中插入顶点src的信息
    AdijLisNode *newNode2 = (AdijLisNode *)malloc(sizeof(AdijLisNode));
    newNode2->dest = src;
    newNode2->next = g->adjList[dest];
    g->adjList[dest] = newNode2;
}

//--------------------图的遍历--------------------

//图的遍历操作所依据的模型均为邻接矩阵或邻接表这种存储模型（图在计算机中的物理存储）
//我们画出来的图为逻辑模型

//辅助队列（BFS）

//创建队列
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

//队列判空
int isEmpty(Queue *q)
{
    return (q->rear == -1); 
}

//入队
void enqueue(Queue *q, int value)
{
    if(q->rear == MAX_V - 1)
    {
        //队列为满
        return;
    }
    if(q->front == - 1)
    {
        //队列为空，第一次入队将front变为0；
        q->front = 0;
    }
    //q->rear 的初始值是-1，++(q->rear) 的初始值为0
    //队列q 的队尾指针向后移并写入数据到队列专门用来存储节点元素的数组items[]
    q->items[++(q->rear)] = value;
}

//出队
int dequeue(Queue *q)
{
    if(isEmpty(q))
    {
        //队列为空
        return -1;
    }
    //记录队头即将出队的元素
    int item = q->items[q->front];

    //这里使用 >= 而没有使用 == 是为了代码的健壮性，防止因为其他错误而导致 front > rear 的情况
    if(q->front >= q->rear)
    {
        //当队列只剩下一个元素时，将 front 和 rear 赋值为 -1 重置队列
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        //让队头指向下一个元素，指向下一个待出队元素
        q->front++;
    }
    //返回要出队的元素，相当于出队操作
    return item;
}

//遍历算法
//DFS辅助函数（递归）
void DFSUtil(Graph *g, int v, int visited[])
{
    //用 visited 数组来标记每个节点是否已经被访问
    visited[v] = 1;
    //打印已经访问的节点
    printf("%d ", v);

    //遍历当前顶点的所有邻接点
    //此处用的是邻接表结构，adjList[] 一个 AdijLisNode * 类型的结构体
    for (AdijLisNode *curr = g->adjList[v]; curr != NULL; curr = curr->next)
    {
        //当节点被访问过 visited[] == 1, !visited[] == 0;
        if(!visited[curr->dest])
        {
            //进入，说明 visited[] 没被访问, 进入递归，进行访问
            DFSUtil(g, curr->dest, visited);
        }
    }
    //for 循环结束，代表当前顶点的所有分支探索完毕
}
//DFS入口
void DFS(Graph *g, int start)
{
    //对 visited 数组全部初始化为未访问（0）
    int visited[MAX_V] = {0};
    printf("深度优先遍历(DFS): ");
    DFSUtil(g, start, visited);
    printf("\n\n");
}

//BFS入口
void BFS(Graph *g, int start)
{
    //对 visited 数组全部初始化为未访问（0）
    int visited[MAX_V] = {0};
    //创建队列
    Queue *q = createQueue();

    visited[start] = 1;
    //入队
    enqueue(q, start);

    printf("广度优先遍历(BFS): ");
    //队列 q 不为空，进入
    while(!isEmpty(q))
    {
        //接收出队元素
        int curr = dequeue(q);
        printf("%d ",curr);

        //这个for循环的作用在于让与该顶点 相连的 其他未经访问的顶点 入队
        //当与该顶点 相连的 其他未经访问的顶点 全部入队后，for循环结束
        //for循环结束后，开始while循环，进行出队操作并打印
        for(AdijLisNode *temp = g->adjList[curr]; temp != NULL; temp = temp->next)
        {
            if(!visited[temp->dest])
            {
                visited[temp->dest] = 1;
                enqueue(q, temp->dest);
            }
        }
    }
    printf("\n\n");
    //队列q，调用createQueue 申请了内存空间，这里要释放
    free(q);
}

int main()
{
    
    return 0;
}