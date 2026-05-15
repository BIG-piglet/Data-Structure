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
        return bstInsert(root->right, target);
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

int main()
{

}