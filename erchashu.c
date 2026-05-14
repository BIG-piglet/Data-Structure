#include<stdio.h>
#include<stdlib.h>

//二叉树节点结构体
typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

//创建新节点
TreeNode *createNode(int data)
{
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (!newNode)
    {
        printf("内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//普通二叉树插入（按层序完全二叉树填充）
//队列辅助：用于找到第一个缺失左或右孩子的节点

//队列节点结构体
typedef struct QueuNode
{
    //二叉树节点
    TreeNode *treeNode;
    //队列节点后继
    struct QueuNode *next;
} QueueNode;

//队列结构体
typedef struct 
{
    //首节点
    QueueNode *front;
    //尾节点
    QueueNode *rear;
} Queue;

//队列初始化
void initQueue(Queue *q)
{
    q->front = q->rear = NULL;
}

//队列判空
int isEmpty(Queue *q)
{
    return q->front == NULL;
}

//队列入队
void enqueue(Queue *q, TreeNode *node)
{
    //为队列节点申请内存
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    //为队列节点中的二叉树节点赋予地址
    newNode->treeNode = node;
    //此时队列节点未入队，所以将后继节点置空
    newNode->next = NULL;

    if (isEmpty(q))
    {
        //队列为空
        //首节点与尾节点相同
        q->front = q->rear = newNode;
    }
    else
    {
        //队列非空
        //此时未入队，将新节点设置为当前队列的尾节点的后继节点
        q->rear->next = newNode;
        //新节点入队，成为尾节点
        q->rear = newNode;
    }
}

//队列出队
TreeNode *dequeue(Queue *q)
{
    
    if(isEmpty(q))
    {
        //队列为空
        return NULL;
    }

    //准备出队节点
    QueueNode *temp = q->front;
    //记录出队队列节点的二叉树节点地址，为二叉树的构建埋下伏笔
    TreeNode *node = temp->treeNode;

    //首节点出队前与其后继节点完成交接仪式
    q->front = q->front->next;
    if (q->front == NULL)
    {
        //队列内唯一节点已出队情况
        //将队列置空
        q->rear = NULL;
    }
    //释放出队节点的内存
    free(temp);
    //返回记录的二叉树节点地址
    return node;
}

//队列销毁
void freeQueue(Queue *q)
{
    while (!isEmpty(q))
    {
        dequeue(q);
    }
}

//按层序完全二叉树插入
//root：根节点指针的指针；data：新节点数据
// TreeNode **root 二级指针：
// 因为在当前函数中我们要改变根节点的地址，同时要让外部的根节点的地址与函数内保持一致，所以要传入一个指针，
// 而在main函数中，根节点本身就是一个指针类型，
// 对其取地址后，传入的参数就变为了一个二级指针，所以接收参数应该为二级指针
void insertLevelOrder(TreeNode **root, int data)
{
    TreeNode *newNode = createNode(data);
    if (*root == NULL)
    {
        //空二叉树时，新创建节点即为根节点
        *root = newNode;
        return;
    }
    //辅助队列
    Queue q;
    //队列初始化
    initQueue(&q);
    //队列入队
    enqueue(&q, *root);

    //队列非空进入循环
    while (!isEmpty(&q))
    {
        //队列节点出队，返回其二叉树节点地址，为构建二叉树做准备
        //这里的队列出队操作，使得队列首节点得到更新，从而使循环成立
        TreeNode *current = dequeue(&q);
        if (current->left == NULL)
        {
            //二叉树节点左孩子为空，为左孩子赋予新地址
            current->left = newNode;
            //销毁队列
            freeQueue(&q);
            return;
        }
        else if (current->right == NULL)
        {
            //同左孩子
            current->right = newNode;
            freeQueue(&q);
            return;
        }
        else
        {
            //二叉树节点左右孩子非空，左右孩子进入队列
            enqueue(&q, current->left);
            enqueue(&q, current->right);
        }
        //进入下一轮循环，直到找到一个空的左孩子或右孩子
    }
    //销毁整个队列（实际上代码不会走到这里，为了代码的健壮性，所以在此进行销毁队列）
    freeQueue(&q);
}

//---------- 遍历 ----------
//在遍历中，我们不需要改变函数外的根节点的地址，只是读取根节点（当然，进入递归后读取的是子节点），所以这里我们只需要传入一级指针

//前序遍历（根->左->右）
void preorder(TreeNode *root)
{
    if (root == NULL)
    {
        //二叉树节点为空
        return;
    }
    //读取传入节点
    printf("%d ",root->data);
    //调用函数读取节点左孩子（递归）
    preorder(root->left);
    //调用函数读取节点右孩子（递归）
    preorder(root->right);
}
//中序遍历（左->根->右）
void inorder(TreeNode *root)
{
    //原理与前序遍历基本相同
    if (root == NULL)
    {
        return;
    }
    inorder(root->left);
    printf("%d ",root->data);
    inorder(root->right);
}
//后序遍历（左->右->根）
void postorder(TreeNode *root)
{
    //原理与前序遍历基本相同
    if (root == NULL)
    {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ",root->data);
}
//层序遍历（广度优先），需要队列辅助
void levelOrder(TreeNode *root)
{
    if(root == NULL)
    {
        return;
    }
    Queue q;
    //队列初始化
    initQueue(&q);
    //队列入队
    enqueue(&q, root);
    while (!isEmpty(&q))
    {
        //队列节点出队，返回其二叉树节点地址
        TreeNode *current = dequeue(&q);
        printf("%d ",current->data);

        //左右孩子入队，以便后续出队读取
        if (current->left)
        {
            enqueue(&q, current->left);
        }
        if (current->right)
        {
            enqueue(&q, current->right);
        }
    }
    printf("\n");
    //遍历完毕，销毁队列
    freeQueue(&q);
}

// ---------- 统计功能 ----------
// 计算树的高度（节点数最大层数，空树高度0）
int getHeight(TreeNode *root)
{                                             
    if(root == NULL)                           
    {                                          
        //节点为空                               
        return 0;                              
    }                                          //这里结合例子更好理解一些，需要自己执行一遍例子的递归过程才能更好理解
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

//计算节点总数
int countNode(TreeNode *root)
{
    if(root == NULL)
    {
        return 0;
    }
    //当递归到叶子节点的左右孩子后，会返回0
    //0 + 0 + 1 = 1，这样就得到了一个叶子节点，其他叶子节点同理
    //叶子节点再与其兄弟节点（如果有）相加 并 +1，依此递推 
    return countNode(root->left) + countNode(root->right) + 1;
}

//计算叶子节点数
int countLeaves(TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->left == NULL && root->right == NULL)
    {
        //当左右孩子都为空时，返回1,即当前节点为叶子节点
        return 1;
    }
    //通过递归查找到叶子节点，每个叶子节点都会返回1，相加即为叶子节点数
    return countLeaves(root->left) + countLeaves(root->right);
}

//普通二叉树查找（必须遍历整棵树，无快速定位）
//返回是否找到，如果想返回节点，可改为返回指针
int search(TreeNode *root, int target)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->data == target)
    {
        //找到返回 1
        return 1;
    }
    //通过递归遍历左右子树，左右子树有一方找到便可返回
    return search(root->left, target) || search(root->right, target);
}

//释放整棵树的内存
void freeTree(TreeNode *root)
{
    //只有节点为空才返回
    if(root == NULL)
    {
        return;
    }
    //调用函数释放左孩子内存（递归）
    freeTree(root->left);
    //调用函数释放右孩子内存（递归）
    freeTree(root->right);
    //释放当前节点内存
    free(root);
}

int main()
{
    printf("========== 1. 按完全二叉树层序插入构建 ==========\n");
    TreeNode *root1 = NULL;
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(arr)/sizeof(arr[0]);
    printf("依次插入：");
    for(int i = 0; i < n; i++)
    {
        printf("%d ",arr[i]);
        insertLevelOrder(&root1, arr[i]);
    }
    printf("\n层序遍历: ");
    levelOrder(root1);
    printf("前序遍历：");
    preorder(root1);
    printf("\n中序遍历: ");
    inorder(root1);
    printf("\n后序遍历: ");
    postorder(root1);
    printf("\n");

    printf("树的高度：%d\n", getHeight(root1));
    printf("节点总数：%d\n", countNode(root1));
    printf("叶子节点数：%d\n", countLeaves(root1));
    int target = 5;
    printf("查找 %d: %s\n", target, search(root1, target) ? "找到" : "未找到");
    target = 10;
    printf("查找 %d: %s\n", target, search(root1, target) ? "找到" : "未找到");

    //释放内存
    freeTree(root1);
    return 0;
}