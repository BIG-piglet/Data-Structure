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
    if(!newNode)
    {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// ==================== BST 核心操作 ====================

// ---------- BST 插入 ----------
// 规则：若 data < 当前节点，插入左子树；若 data > 当前节点，插入右子树
// 不允许重复值
TreeNode *bstInsert(TreeNode *root, int data)
{
    if(root == NULL)
    {
        //找到空位置，创建新节点
        return createNode(data);
    }
    if(data < root->data)
    {
        //小于当前节点，递归插入左子树
        root->left = bstInsert(root->left, data);
    }
    else if(data > root->data)
    {
        //大于当前节点，递归插入右子树
        root->right = bstInsert(root->right, data);
    }
    //若 data == root->data，可选择不操作或更新，这里保持原树不变

    return root;
}

// ---------- BST 查找 ----------
// 利用 BST 性质，平均时间复杂度 O(log n)，最坏 O(n)（退化为链表）
// 返回找到的节点指针，未找到返回 NULL
TreeNode *bstSearch(TreeNode *root, int target)
{
    if(root == NULL || root->data == target)
    {
        return root;
    }
    if(target < root->data)
    {
        //目标值更小，去左子树找
        return bstSearch(root->left, target);
    }
    else
    {
        //目标值更大，去右子树找
        return bstSearch(root->right, target);
    }
}

// ---------- 查找最小值节点 ----------
// BST 中最小值一定在最左边的叶子节点
TreeNode *bstFindMin(TreeNode *root)
{
    if(root == NULL)
    {
        return NULL;
    }
    while (root->left != NULL)
    {
        //不断的往左子树查找，直到最后一个叶子节点
        root = root->left;
    }
    return root;
}

// ---------- 查找最大值节点 ----------
// BST 中最大值一定在最右边的叶子节点
TreeNode *bstFindMax(TreeNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

// ---------- BST 删除（最复杂操作） ----------
// 分三种情况：
// 1. 待删节点是叶子节点：直接删除
// 2. 待删节点只有一个孩子：用孩子顶替自己
// 3. 待删节点有两个孩子：用右子树的最小节点（或左子树的最大节点）顶替自己，然后递归删除那个顶替节点
TreeNode *bstDelete(TreeNode *root, int target)
{
    if(root == NULL)
    {
        return NULL;
    }


    //1、首先通过BST的左小右大的特点，递归查找目标节点
    //2、找到目标节点后，如果是情况1或2
    //3、情况1为空孩子，也不需要替代节点，所以情况1返回的是空，
    //   所以我们只需要判断情况1的左右孩子任何一方为空就可以，然后释放情况1的节点，并返回其任何一方的节点（也就是空）
    //   但为了提高代码的复用性，所以我们将情况1与情况2结合（可以理解为情况1是情况2的特殊情况，可以完全适配情况2）
    //4、情况2有一个孩子和一个空孩子，我们只需要判断情况2的左右孩子有一方为空，然后用另一方替代目标节点


    //先找到待删除节点
    if(target < root->data)
    {
        root->left = bstDelete(root->left, target);
    }
    else if(target > root->data)
    {
        root->right = bstDelete(root->right, target);
    }
    else
    {
        //找到待删除节点

        //情况1或2：最多只有一个孩子
        if(root->left == NULL)
        {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL)
        {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }
        // 情况3：有两个孩子
        // 找到右子树中的最小节点（后继节点），用它替换当前节点
        TreeNode *temp = bstFindMin(root->right);
        //将后继节点的值复制
        root->data = temp->data;
        //递归删除右子树中的那个后继结点
        root->right = bstDelete(root->right, temp->data);
    }
    return root;
}

// ==================== 遍历（复用原有代码） ====================

// 中序遍历（左->根->右）
// 在 BST 中，中序遍历的结果就是升序排列！
void inorder(TreeNode *root)
{
    if(root == NULL)
    {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}
// 前序遍历（根->左->右）
void preorder(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// 释放整棵树（复用原有函数）
void freeTree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// ==================== 验证工具 ====================

// 验证一棵树是否为合法 BST
// 每个节点必须满足：min < node->data < max
int isValidBST(TreeNode *root, long long minVal, long long maxVal)
{
    if(root == NULL)
    {
        return 1;
    }
    if(root->data <= minVal || root->data >= maxVal)
    {
        return 0;
    }

    //因为每次递归传入的数值不同，所以maxVal与minVal在每次递归中都会改变
    //BST要求左子树全部小于根节点，右子树全部大于根节点
    //每次的递归就是不断收缩子节点的可取值范围
    return isValidBST(root->left, minVal, root->data) && 
           isValidBST(root->right, root->data, maxVal);
}

int main()
{
    printf("========== 二叉搜索树(BST)演示 ==========\n\n");
    
    TreeNode *root = NULL;
    
    // 插入数据（故意打乱顺序，BST 会自动按规则排列）
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("依次插入：");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
        root = bstInsert(root, arr[i]);
    }
    printf("\n\n");
    
    printf("BST 是否合法：%s\n", isValidBST(root, -2147483649LL, 2147483648LL) ? "是" : "否");
    
    printf("中序遍历（升序）：");
    inorder(root);
    printf("\n");
    
    printf("前序遍历：");
    preorder(root);
    printf("\n");
    
    // 查找演示
    int targets[] = {25, 100};
    for (int i = 0; i < 2; i++)
    {
        TreeNode *found = bstSearch(root, targets[i]);
        printf("查找 %d: %s\n", targets[i], found ? "找到" : "未找到");
    }
    
    // 最值演示
    TreeNode *minNode = bstFindMin(root);
    TreeNode *maxNode = bstFindMax(root);
    printf("最小值：%d, 最大值：%d\n", minNode->data, maxNode->data);
    
    // 删除演示（分别演示删叶子、删单孩子、删双孩子节点）
    printf("\n--- 删除演示 ---\n");
    
    // 删除叶子节点 10
    printf("删除叶子节点 10, 中序遍历：");
    root = bstDelete(root, 10);
    inorder(root);
    printf("\n");
    
    // 删除只有一个孩子的节点 20（删完 10 后，20 只有右孩子 25）
    printf("删除单孩子节点 20, 中序遍历：");
    root = bstDelete(root, 20);
    inorder(root);
    printf("\n");
    
    // 删除有两个孩子的节点 30
    printf("删除双孩子节点 30, 中序遍历：");
    root = bstDelete(root, 30);
    inorder(root);
    printf("\n");
    
    printf("\nBST 是否合法：%s\n", isValidBST(root, -2147483649LL, 2147483648LL) ? "是" : "否");
    
    // 释放内存
    freeTree(root);
    return 0;
}