/*---------------------------------------------
|  Chain Table On Sequence Structure          |
|  Author:qrq 2402                            |
|  Date:2025.4.27                             |
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

typedef int status;
typedef int KeyType;

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2

using namespace std;
#define MAXSIZE 10 // Maximum number of nodes in the tree

typedef struct
{
    KeyType key;
    char others[30]; // Other information
} TElemType;         // data type of the element

typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild; // Left and right children
} BiTNode, *BiTree;                  // Binary tree node type

typedef struct
{
    BiTree root;   // Root node of the tree
    char name[30]; // Name of the tree
} Tree;            // Tree type

typedef struct Trees
{
    Tree elem[MAXSIZE]; // Array of trees
    int length;
    int cur_index; // Current index of the tree
} TREES;           // Tree array type

/*--------------------------------------------------*/

// basic operations
status CheckFromDef(TElemType definition[]);
status KeyExistInTree(BiTree T, KeyType key);
status CreateWithDef(BiTree &T, TElemType definition[], int &index);
BiTNode *GetParent(BiTree T, KeyType key);
status preorderSave(BiTree T, FILE *fp);
status visit(BiTree T);

// BiTree system operations
status CreateBiTree(BiTree &T, TElemType definition[]);
status DestroyBiTree(BiTree &T);
status ClearBiTree(BiTree &T);
status BiTreeEmpty(BiTree T);
int BiTreeDepth(BiTree T);
BiTNode *LocateNode(BiTree T, KeyType e);
status Assign(BiTree &T, KeyType e, TElemType value);
BiTNode *GetSibling(BiTree T, KeyType e);
status InsertNode(BiTree &T, KeyType e, int LR, TElemType c);
status DeleteNode(BiTree &T, KeyType e);
status PreOrderTraverse(BiTree T);
status InOrderTraverse(BiTree T);
status PostOrderTraverse(BiTree T);
status LevelOrderTraverse(BiTree T);

// Additional operations
status SaveBiTree(BiTree T, char FileName[]);
status LoadBiTree(BiTree &T, char FileName[]);
status MaxPathSum(BiTree T);
BiTNode *LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2);
status InvertTree(BiTree &T);

// multiple trees operations
int LocateTree(TREES Trees, char FileName[]);
status AddTree(TREES &Trees, char FileName[]);
status RemoveTree(TREES T, char ListName[]);
status SwitchTree(TREES &Trees, char ListName[]);
void PrintTrees(TREES Trees);
void PrintMenu(TREES &Trees);

/*--------------------------------------------------*/

status CreateBiTree(BiTree &T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T != NULL)
        return INFEASIBLE;

    int unique = CheckFromDef(definition);
    if (unique != OK)
        return ERROR;

    int index = 0;
    CreateWithDef(T, definition, index);
    return OK;
    /********** End **********/
}

status CheckFromDef(TElemType definition[])
{
    // 检查定义数组中是否有重复的关键字
    int i = 0;
    int unique[1000] = {0};
    while (definition[i].key != -1)
    {
        if (definition[i].key != 0)
        {
            unique[definition[i].key] += 1;
            if (unique[definition[i].key] > 1)
                return ERROR;
        }
        i++;
    }
    return OK;
}

status CreateWithDef(BiTree &T, TElemType definition[], int &index)
{
    if (definition[index].key == 0)
    {
        T = NULL;
        index++;
        return OK;
    }
    else
    {
        T = (BiTree)malloc(sizeof(BiTNode));
        if (!T)
            return OVERFLOW;
        T->data = definition[index++];
        CreateWithDef(T->lchild, definition, index);
        CreateWithDef(T->rchild, definition, index);
    }
    return OK;
}

status DestroyBiTree(BiTree &T)
// 初始条件是二叉树T已存在；操作结果是销毁二叉树T；销毁了则原先所有分配的空间都销毁。
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;

    DestroyBiTree(T->lchild);
    DestroyBiTree(T->rchild);

    free(T);
    T = NULL;

    return OK;
    /********** End **********/
}

status ClearBiTree(BiTree &T)
// 将二叉树设置成空，但保留结点空间
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;

    ClearBiTree(T->lchild);
    ClearBiTree(T->rchild);

    T->lchild = NULL;
    T->rchild = NULL;

    return OK;
    /********** End **********/
}

status BiTreeEmpty(BiTree T)
// 判断二叉树是否为空
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return TRUE;
    else
        return FALSE;
    /********** End **********/
}

int BiTreeDepth(BiTree T)
// 求二叉树T的深度
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return 0;

    int Ldepth = BiTreeDepth(T->lchild);
    int Rdepth = BiTreeDepth(T->rchild);

    return 1 + (Ldepth > Rdepth ? Ldepth : Rdepth);

    /********** End **********/
}

BiTNode *LocateNode(BiTree T, KeyType e)
// 查找结点
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL || T->data.key == e)
        return T;

    BiTNode *left = LocateNode(T->lchild, e);
    return left ? left : LocateNode(T->rchild, e);
    /********** End **********/
}

status Assign(BiTree &T, KeyType e, TElemType value)
// 实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;

    BiTree p = LocateNode(T, e);

    if (p != NULL)
    {
        // 暂存原始关键字
        KeyType originalKey = p->data.key;

        // 赋值新值
        p->data = value;

        // 检查赋值后是否存在重复关键字
        if (KeyExistInTree(T, value.key) > 1)
        {
            // 如果发现重复关键字，恢复原始关键字
            p->data.key = originalKey;
            return ERROR;
        }
        return OK;
    }

    return ERROR;
    /********** End **********/
}

BiTNode *GetSibling(BiTree T, KeyType e)
// 获取兄弟结点
{
    // 如果树为空或者没有兄弟结点，返回NULL
    if (T == NULL || T->lchild == NULL || T->rchild == NULL)
        return NULL;

    // 检查左孩子是否是给定关键字的兄弟
    if (T->lchild->data.key == e)
        return T->rchild;

    // 检查右孩子是否是给定关键字的兄弟
    if (T->rchild->data.key == e)
        return T->lchild;

    // 递归在左子树和右子树中查找兄弟结点
    BiTNode *sibling = GetSibling(T->lchild, e);
    return sibling ? sibling : GetSibling(T->rchild, e);
}

int KeyExistInTree(BiTree T, KeyType key)
{
    if (!T)
    {
        return 0;
    }
    if (T->data.key == key)
    {
        return 1;
    }
    int leftResult = KeyExistInTree(T->lchild, key);
    int rightResult = KeyExistInTree(T->rchild, key);

    return leftResult + rightResult;
}

status InsertNode(BiTree &T, KeyType e, int LR, TElemType c)
/*插入结点。此题允许通过增加其它函数辅助实现本关任务初始条件是二叉树T存在，e是和T中结点关键字类型相同的给定值，LR为0或1，c是待插入结点；操作结果是根据LR为0或者1，插入结点c到T中，作为关键字为e的结点的左或右孩子结点，结点e的原有左子树或右子树则为结点c的右子树；
特殊情况，LR-1时，作为根结点插入，原根结点作为c的右子树。*/
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (KeyExistInTree(T, c.key))
        return INFEASIBLE;

    if (LR == -1)
    {
        BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
        NewNode->data = c;
        NewNode->rchild = T;
        T = NewNode;
        return OK;
    }
    else
    {
        BiTree t = LocateNode(T, e); // 未找到关键字为e的结点
        if (!t)
            return ERROR;
        else
        {
            if (LR == 1)
            {
                BiTree tr = t->rchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->rchild = NewNode;
                NewNode->rchild = tr;
                NewNode->lchild = NULL;
            }
            else if (LR == 0)
            {
                BiTree tl = t->lchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->lchild = NewNode;
                NewNode->rchild = tl;
                NewNode->lchild = NULL;
            }
            return OK;
        }
    }
    /********** End **********/
}

BiTNode *GetParent(BiTree T, KeyType key)
{
    if (T == NULL || T->data.key == key)
        return NULL;

    if ((T->lchild != NULL && T->lchild->data.key == key) ||
        (T->rchild != NULL && T->rchild->data.key == key))
    {
        return T;
    }

    BiTNode *parent = GetParent(T->lchild, key);
    return parent ? parent : GetParent(T->rchild, key);
}

status DeleteNode(BiTree &T, KeyType e)
// 删除结点。此题允许通过增加其它函数辅助实现本关任务
/*初始条件是二叉树T存在，e是和T中结点关键字类型相同的给定值。操作结果是删除T中关键字为e的结点；同时，如果关键字为e的结点度为0，删除即可；如关键字为e的结点度为1，用关键字为e的结点孩子代替被删除的e位置；如关键字为e的结点度为2，用e的左孩子代替被删除的e位置，e的右子树作为e的左子树中最右结点的右子树；
 */
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;

    BiTree current = LocateNode(T, e);
    if (current == NULL)
        return ERROR;

    int degree = (T->lchild != NULL) + (T->rchild != NULL);

    BiTNode *parent = GetParent(T, e);

    if (degree == 0)
    {
        if (current == NULL)
        {
            T = NULL;
        }
        else
        {
            if (parent->lchild == current)
            {
                parent->lchild = NULL;
            }
            else
            {
                parent->rchild = NULL;
            }
        }
        free(current);

        return OK;
    }
    else if (degree == 1)
    {
        BiTNode *child = (current->lchild != NULL) ? current->lchild : current->rchild;

        if (parent == NULL)
        {
            T = child;
        }
        else
        {
            if (parent->lchild == current)
            {
                parent->lchild = child;
            }
            else
            {
                parent->rchild = child;
            }
        }
        free(current);

        return OK;
    }
    else
    {
        BiTNode *LC = current->lchild;
        BiTNode *RC = current->rchild;

        BiTNode *rightMostInLeft = LC;
        while (rightMostInLeft != NULL && rightMostInLeft->rchild != NULL)
        {
            rightMostInLeft = rightMostInLeft->rchild;
        }

        rightMostInLeft->rchild = RC;

        if (parent == NULL)
        {
            T = LC;
        }
        else
        {
            if (parent->lchild == current)
            {
                parent->lchild = LC;
            }
            else
            {
                parent->rchild = LC;
            }
        }
        free(current);
        return OK;
    }

    /********** End **********/
}

status visit(BiTree T)
{
    // 访问结点的操作
    if (T != NULL)
    {
        printf("%d %s\n", T->data.key, T->data.others);
    }
    return OK;
}

status PreOrderTraverse(BiTree T)
// 先序遍历二叉树T，非递归实现
// 先序遍历的顺序是：根结点->左子树->右子树
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;
    BiTree stack[1000];
    int top = -1;
    stack[++top] = T;

    while (top != -1)
    {
        BiTree cur = stack[top--];
        visit(cur);

        if (cur->rchild)
            stack[++top] = cur->rchild;
        if (cur->lchild)
            stack[++top] = cur->lchild;
    }

    return OK;

    /********** End **********/
}

status InOrderTraverse(BiTree T)
// 中序遍历二叉树T，非递归实现
// 中序遍历的顺序是：左子树->根结点->右子树
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    BiTree stack[1000];
    int top = -1;
    BiTree cur = T;

    while (cur != NULL || top != -1)
    {
        while (cur != NULL)
        {
            stack[++top] = cur;
            cur = cur->lchild;
        }

        cur = stack[top--];
        visit(cur);

        cur = cur->rchild;
    }

    return OK;
    /********** End **********/
}

status PostOrderTraverse(BiTree T)
// 后序遍历二叉树T
{
    /********** Begin *********/
    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        visit(T);

        return OK;
    }
    else
        return INFEASIBLE;
    /********** End **********/
}

status LevelOrderTraverse(BiTree T)
// 按层遍历二叉树T
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (T == NULL)
        return INFEASIBLE;
    BiTree queue[1000];
    int front = 0, rear = 0;
    queue[rear++] = T;

    while (front < rear)
    {
        T = queue[front++];
        visit(T);

        if (T->lchild != NULL)
            queue[rear++] = T->lchild;
        if (T->rchild != NULL)
            queue[rear++] = T->rchild;
    }

    return OK;

    /********** End **********/
}

status preorderSave(BiTree T, FILE *fp)
{
    // 先序遍历保存二叉树到文件
    if (T)
    {
        fprintf(fp, "%d %s ", T->data.key, T->data.others);
        preorderSave(T->lchild, fp);
        preorderSave(T->rchild, fp);
        return OK;
    }
    else
    {
        // 如果结点为空，输出0和null
        fprintf(fp, "0 null ");
        return OK;
    }

    return ERROR;
}

status SaveBiTree(BiTree T, char FileName[])
// 将二叉树的结点数据写入到文件FileName中
{
    FILE *fp = fopen(FileName, "w");
    if (!fp)
        return ERROR;

    preorderSave(T, fp);

    fprintf(fp, "-1 null");
    fclose(fp);

    return OK;
}

status LoadBiTree(BiTree &T, char FileName[])
// 读入文件FileName的结点数据，创建二叉树
{
    FILE *fp = fopen(FileName, "r");
    if (!fp)
        return ERROR;

    TElemType definition[100];
    int i = 0;
    do
    {
        fscanf(fp, "%d %s", &definition[i].key, definition[i].others);
    } while (definition[i++].key != -1);

    if (CreateBiTree(T, definition))
        return OK;
    else
        return ERROR;
}

status MaxPathSum(BiTree T)
{
    if (T == NULL)
        return 0;

    if (!T->lchild && !T->rchild)
    {
        return T->data.key;
    }

    int lsum = MaxPathSum(T->lchild);
    int rsum = MaxPathSum(T->rchild);

    return T->data.key + max(lsum, rsum);
}

BiTNode *LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2)
{
    if (T == NULL || T->data.key == e1 || T->data.key == e2)
        return T;

    BiTNode *left = LowestCommonAncestor(T->lchild, e1, e2);
    BiTNode *right = LowestCommonAncestor(T->rchild, e1, e2);

    if (left && right)
        return T;
    else
        return left ? left : right;
}

status InvertTree(BiTree &T)
{
    if (T == NULL)
        return INFEASIBLE;

    BiTNode *temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;

    InvertTree(T->lchild);
    InvertTree(T->rchild);

    return OK;
}

status AddTree(TREES &Trees, char FileName[])
{
    if (Trees.length >= MAXSIZE)
        return ERROR;

    for (int i = 0; i < Trees.length; i++)
    {
        if (strcmp(Trees.elem[i].name, FileName) == 0)
            return ERROR;
    }

    Trees.elem[Trees.length].root = NULL;
    strcpy(Trees.elem[Trees.length].name, FileName);
    Trees.length++;
    Trees.cur_index = Trees.length - 1;

    printf("当前二叉树：%s\n\n", Trees.elem[Trees.cur_index].name);
    return OK;
}

status RemoveTree(TREES T, char ListName[])
{
    for (int i = 0; i < T.length; i++)
    {
        if (strcmp(T.elem[i].name, ListName) == 0)
        {
            DestroyBiTree(T.elem[i].root);
            for (int j = i; j < T.length - 1; j++)
            {
                T.elem[j] = T.elem[j + 1];
            }
            T.elem[T.length - 1].root = NULL;
            memset(T.elem[T.length - 1].name, 0, 30);

            T.length--;
            return OK;
        }
    }
    return ERROR;
}

status SwitchTree(TREES &Trees, char ListName[])
{
    for (int i = 0; i < Trees.length; i++)
    {
        if (strcmp(Trees.elem[i].name, ListName) == 0)
        {
            Trees.cur_index = i;
            return OK;
        }
    }
    return ERROR;
}

status LocateTree(TREES Trees, char ListName[])
{
    for (int i = 0; i < Trees.length; i++)
    {
        if (strcmp(Trees.elem[i].name, ListName) == 0)
            return i + 1;
    }
    return ERROR;
}

void PrintTrees(TREES Trees)
{
    printf("--------------------------------------------------\n");
    printf("当前二叉树列表：\n");
    for (int i = 0; i < Trees.length; i++)
    {
        printf("%d. %s\n", i + 1, Trees.elem[i].name);
    }
    printf("--------------------------------------------------\n");
}

void PrintMenu(TREES &Trees)
{
    printf("\n");
    printf("---------------------------------------------------------\n");
    printf(" Menu for Binary Tree System On Binary Linked List \n");
    printf("---------------------------------------------------------\n\n");
    printf("当前二叉树：%s\n\n", Trees.elem[Trees.cur_index].name);
    printf("Basic Operations:\n");
    printf("      1. CreateTree               2. DestoryTree         \n");
    printf("      3. ClearTree                4. BiTreeEmpty         \n");
    printf("      5. BiTreeDepth              6. LocateNode          \n");
    printf("      7. Assign                   8. GetSibling          \n");
    printf("      9. InsertNode               10. DeleteNode         \n");
    printf("     11. PreOrderTraverse         12. InOrderTraverse    \n");
    printf("     13. PostOrderTraverse        14. LevelOrderTraverse \n");
    printf("Additional Operations:\n");
    printf("     15. MaxPathSum               16. InvertTree         \n");
    printf("     17. LoadTree                 18. SaveTree           \n");
    printf("     19. LowestCommonAncestor                            \n");
    printf("Multiple Trees Operations:\n");
    printf("     20. AddTree                  21. RemoveTree         \n");
    printf("     22. SwitchTree               23. PrintTrees         \n");
    printf("     0. Exit\n");
    printf("---------------------------------------------------------\n");
    printf("请输入操作选项：\n");
}

int main()
{
    TREES Trees;
    Trees.length = 1;
    Trees.cur_index = 0;
    Trees.elem[0].root = NULL;
    char firstname[30];
    system("cls");
    printf("--------------------------------------------------\n");
    printf("这是一个多二叉树系统，可以对单个或多个二叉树进行操作\n");
    printf("Author：HUST Cupid-qrq\n");
    printf("--------------------------------------------------\n");
    printf("在开始之前，请先输入第一个二叉树的名称：\n");
    if (scanf("%s", firstname) != 1)
    {
        printf("输入错误，请重新输入：\n");
        scanf("%s", firstname);
    }
    printf("输入成功，第一个二叉树的名称是：%s\n", firstname);
    strcpy(Trees.elem[0].name, firstname);
    printf("接下来，进入菜单界面，按任意键继续：\n");
    getchar();
    getchar();

    int op = 1;                // 菜单选项
    int i;                     // 循环变量
    int depth;                 // 二叉树深度
    TElemType definition[100]; // 创建二叉树的定义数组，存储先根遍历结点序列，-1表示结束
    int exist;                 // 查找结点的关键字
    int exist2;                // 赋值结点的关键字
    TElemType value;           // 赋值结点的值
    int e1, e2;                // 查找最近公共祖先的两个结点的关键字
    int LR;                    // 插入函数的参数，0表示左子树，1表示右子树，-1表示根结点
    TElemType c;               // 待插入结点
    BiTNode *node;             // 查找结点的指针
    BiTNode *sibling;          // 兄弟结点的指针
    BiTNode *lca;              // 最近公共祖先的指针
    int res;                   // 函数的返回值
    int index;                 // 二叉树的索引
    int maxPathSum;            // 最大路径和

    char filename[100], treename[100];

    while (op)
    {
        PrintMenu(Trees);

        if (scanf("%d", &op) != 1)
        {
            op = -1;
        }

        switch (op)

        {
        case 1:
            printf("请输入二叉树的带空结点的先根遍历序列，空结点用\"0 null\"表示，结束用\"-1 null\"表示：\n");
            printf("格式：\"key others\"\n");
            i = 0;
            while (1)
            {
                scanf("%d %s", &definition[i].key, definition[i].others);
                if (definition[i].key == -1)
                    break;
                i++;
            }
            if (CreateBiTree(Trees.elem[Trees.cur_index].root, definition) == OK)
            {
                printf("二叉树创建成功！\n");
                printf("二叉树的先序遍历结果为：\n");
                PreOrderTraverse(Trees.elem[Trees.cur_index].root);
                printf("二叉树的中序遍历结果为：\n");
                InOrderTraverse(Trees.elem[Trees.cur_index].root);
            }
            else
                printf("二叉树创建失败，可能是内存不足或关键字重复！\n");

            break;

        case 2:
            if (DestroyBiTree(Trees.elem[Trees.cur_index].root) == OK)
                printf("二叉树销毁成功！\n");
            else
                printf("二叉树不存在，销毁失败！\n");
            break;

        case 3:
            if (ClearBiTree(Trees.elem[Trees.cur_index].root) == OK)
                printf("二叉树清空成功！\n");
            else
                printf("二叉树不存在，清空失败！\n");
            break;

        case 4:
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == FALSE)
                printf("二叉树非空！\n");
            else
                printf("二叉树不存在！\n");
            break;

        case 5:
            depth = BiTreeDepth(Trees.elem[Trees.cur_index].root);
            if (depth != 0)
                printf("二叉树的深度为：%d\n", depth);
            else
                printf("二叉树不存在！\n");
            break;

        case 6:
            printf("请输入要查找的结点的关键字：\n");
            scanf("%d", &exist);
            node = LocateNode(Trees.elem[Trees.cur_index].root, exist);
            if (node != NULL)
                printf("结点 %d 的信息为：%d %s\n", exist, node->data.key, node->data.others);
            else
                printf("结点 %d 不存在！\n", exist);
            break;

        case 7:
            printf("请输入要赋值的结点的关键字：\n");
            scanf("%d", &exist2);
            printf("请输入新的结点信息（格式：\"key others\"）：\n");
            scanf("%d %s", &value.key, value.others);
            if (Assign(Trees.elem[Trees.cur_index].root, exist2, value) == OK)
            {
                printf("赋值成功！\n");
                BiTNode *assignedNode = LocateNode(Trees.elem[Trees.cur_index].root, exist2);
                printf("赋值后的结点信息为：%d %s\n", assignedNode->data.key, assignedNode->data.others);
            }
            else
                printf("赋值失败，结点不存在或关键字重复！\n");
            break;

        case 8:
            printf("请输入要查找的结点的关键字：\n");
            scanf("%d", &exist);
            sibling = GetSibling(Trees.elem[Trees.cur_index].root, exist);
            if (sibling != NULL)
                printf("结点 %d 的兄弟结点为：%d %s\n", exist, sibling->data.key, sibling->data.others);
            else
                printf("结点 %d 没有兄弟结点或不存在！\n", exist);
            break;

        case 9:
            printf("请输入要插入的位置的关键字：\n");
            scanf("%d", &exist);
            printf("请输入插入的结点的信息（格式：\"key others\"）：\n");
            scanf("%d %s", &c.key, c.others);
            printf("请输入插入的方式（0表示左子树，1表示右子树，-1表示根结点）：\n");
            scanf("%d", &LR);
            res = InsertNode(Trees.elem[Trees.cur_index].root, exist, LR, c);
            if (res == INFEASIBLE)
                printf("插入失败，关键字重复！\n");
            else if (res == ERROR)
                printf("插入失败，插入位置不存在！\n");
            else
                printf("插入成功！\n");
            break;

        case 10:
            printf("请输入要删除的结点的关键字：\n");
            scanf("%d", &exist);
            if (DeleteNode(Trees.elem[Trees.cur_index].root, exist) == OK)
                printf("删除成功！\n");
            else
                printf("删除失败，结点不存在或树为空！\n");
            break;

        case 11:
            printf("前序遍历结果：\n");
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                printf("--------------------------------------------------\n");
                PreOrderTraverse(Trees.elem[Trees.cur_index].root);
                printf("--------------------------------------------------\n");
            }
            break;

        case 12:
            printf("中序遍历结果：\n");
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                printf("--------------------------------------------------\n");
                InOrderTraverse(Trees.elem[Trees.cur_index].root);
                printf("--------------------------------------------------\n");
            }
            break;

        case 13:
            printf("后序遍历结果：\n");
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                printf("--------------------------------------------------\n");
                PostOrderTraverse(Trees.elem[Trees.cur_index].root);
                printf("--------------------------------------------------\n");
            }
            break;

        case 14:
            printf("层序遍历结果：\n");
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                printf("--------------------------------------------------\n");
                LevelOrderTraverse(Trees.elem[Trees.cur_index].root);
                printf("--------------------------------------------------\n");
            }
            break;

        case 15:
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                maxPathSum = MaxPathSum(Trees.elem[Trees.cur_index].root);
                printf("二叉树的最大路径和为：%d\n", maxPathSum);
            }
            break;

        case 16:
            if (InvertTree(Trees.elem[Trees.cur_index].root) == OK)
                printf("二叉树反转成功！\n");
            else
                printf("二叉树不存在，反转失败！\n");

            break;

        case 17:
            printf("请输入要加载的文件名：\n");
            scanf("%s", filename);
            if (LoadBiTree(Trees.elem[Trees.cur_index].root, filename) == OK)
                printf("二叉树加载成功！\n");
            else
                printf("二叉树加载失败！\n");
            break;

        case 18:
            if (BiTreeEmpty(Trees.elem[Trees.cur_index].root) == TRUE)
                printf("二叉树为空！\n");
            else
            {
                printf("请输入要保存的文件名：\n");
                scanf("%s", filename);
                if (SaveBiTree(Trees.elem[Trees.cur_index].root, filename) == OK)
                    printf("二叉树保存成功！\n");
                else
                    printf("二叉树保存失败！\n");
            }
            break;

        case 19:
            printf("请输入要查找的两个结点的关键字：\n");
            scanf("%d %d", &e1, &e2);
            lca = LowestCommonAncestor(Trees.elem[Trees.cur_index].root, e1, e2);
            if (lca != NULL)
                printf("最近公共祖先结点为：%d %s\n", lca->data.key, lca->data.others);
            else
                printf("最近公共祖先结点不存在！\n");
            break;

        case 20:
            printf("请输入要添加的树的名称：\n");
            scanf("%s", treename);
            if (AddTree(Trees, treename) == OK)
                printf("树添加成功！\n");
            else
                printf("树添加失败！\n");
            break;

        case 21:
            printf("请输入要删除的树的名称：\n");
            scanf("%s", treename);
            if (RemoveTree(Trees, treename) == OK)
                printf("树删除成功！\n");
            else
                printf("树删除失败！\n");
            break;

        case 22:
            printf("请输入要切换的树的名称：\n");
            scanf("%s", treename);
            index = LocateTree(Trees, treename);
            if (index != ERROR)
            {
                Trees.cur_index = index - 1;
                printf("切换成功！当前树为：%s\n", Trees.elem[Trees.cur_index].name);
            }
            else
                printf("树不存在，切换失败！\n");
            break;

        case 23:
            PrintTrees(Trees);
            break;

        case 0:
            printf("退出程序！\n");
            break;

        default:
            printf("无效操作，请重新输入！\n");
            break;
        }

        if (op != 0)
        {
            printf("按回车键继续...\n");
            getchar();
            getchar();
        }
    }

    return 0;
}