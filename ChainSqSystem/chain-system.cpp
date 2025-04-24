/*---------------------------------------------
|  Chain Table On Sequence Structure          |
|  Author:qrq 2402                            |
|  Date:2025.4.14                              |
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>

typedef int status;
typedef int ElemType;

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
#define MAXSIZE 10

using namespace std;

typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

typedef struct
{
    char name[30]; // Name of the list
    LinkList head; // Head pointer to the list
} List;

typedef struct
{
    List elem[MAXSIZE]; // Array of lists
    int length;         // Number of lists
    int cur_index;      // Current active list index
} LISTS;

/*--------------------------------------------------*/
// Fundamental function
status InitList(List &L);
status DestroyList(List &L);
status ClearList(List &L);
status ListEmpty(List &L);
int ListLength(List &L);
status GetElem(List &L, int i, ElemType &e);
int LocateElem(List &L, ElemType e);
status PriorElem(List &L, ElemType cur, ElemType &pre_e);
status NextElem(List &L, ElemType cur, ElemType &next_e);
status ListInsert(List &L, int i, ElemType &e);
status ListDelete(List &L, int i, ElemType &e);
status ListTraverse(List &L);

// Additional functions
status ReverseList(List &L);
status RemoveNthFromEnd(List &L, int n);
status SortList(List &L);
status SaveList(List L, char FileName[]);
status LoadList(List &L, char FileName[]);

// Function for multiple lists
int LocateList(LISTS Lists, char ListName[]);
status AddList(LISTS &Lists, char ListName[]);
status RemoveList(LISTS &Lists, char ListName[]);
status SwitchList(LISTS &Lists, char ListName[]);
void PrintLists(LISTS Lists);
void printMenu(LISTS &Lists);
/*--------------------------------------------------*/

status
InitList(List &L)
{
    if (L.head != NULL)
        return INFEASIBLE;
    L.head = (LinkList)malloc(sizeof(LNode));
    L.head->next = NULL;

    return OK;
}

status DestroyList(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head, q;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    L.head = NULL;

    return OK;
}

status ClearList(List &L)

{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next;
    while (p != NULL)
    {
        LinkList temp = p;
        p = p->next;
        free(temp);
    }
    L.head->next = NULL;

    return OK;
}

status ListEmpty(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    if (L.head->next == NULL)
        return TRUE;
    else
        return FALSE;
}

int ListLength(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next;
    int length = 0;
    while (p)
    {
        p = p->next;
        length++;
    }
    return length;
}

status GetElem(List &L, int i, ElemType &e)
{
    if (L.head == NULL)
        return INFEASIBLE;

    if (i < 1)
        return ERROR;

    LinkList p = L.head->next;
    int length = 1;
    while (p != NULL && length < i)
    {
        p = p->next;
        length++;
    }

    if (p != NULL && length == i)
    {
        e = p->data;
        return OK;
    }

    return ERROR;
}

status LocateElem(List &L, ElemType e)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next;
    int pos = 1;

    while (p != NULL)
    {
        if (p->data == e)
            return pos;
        p = p->next;
        pos++;
    }

    return ERROR;
}

status PriorElem(List &L, ElemType e, ElemType &pre)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next, q;
    int pos = 1;
    while (p)
    {
        if (p->data == e && pos > 1)
        {
            pre = q->data;
            return OK;
        }
        q = p;
        p = p->next;
        pos++;
    }

    return ERROR;
}

status NextElem(List &L, ElemType e, ElemType &next)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next;
    while (p)
    {
        if (p->data == e && p->next != NULL)
        {
            next = p->next->data;
            return OK;
        }
        p = p->next;
    }

    return ERROR;
}

status ListInsert(List &L, int i, ElemType &e)
{
    if (L.head == NULL)
        return INFEASIBLE;

    if (i < 1)
        return ERROR;

    LinkList p = L.head;
    int pos = 0;

    while (p && pos < i - 1)
    {
        p = p->next;
        pos++;
    }

    if (!p)
        return ERROR;

    LinkList newNode = (LinkList)malloc(sizeof(LNode));
    if (!newNode)
        return OVERFLOW;

    newNode->data = e;
    newNode->next = p->next;
    p->next = newNode;

    return OK;
}

status ListDelete(List &L, int i, ElemType &e)
{
    if (L.head == NULL)
        return INFEASIBLE;

    if (i < 1)
        return ERROR;

    LinkList p = L.head;
    int pos = 0;

    while (p->next && pos < i - 1)
    {
        p = p->next;
        pos++;
    }

    if (!p->next)
        return ERROR;

    LinkList temp = p->next;
    e = p->next->data;
    p->next = p->next->next;
    free(temp);
    return OK;
}

status ListTraverse(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList p = L.head->next;
    printf("\n------------------------------------------------\n");
    while (p)
    {
        printf("%d", p->data);
        if (p->next != NULL)
            printf("  ->  ");
        p = p->next;
    }
    printf("\n------------------------------------------------\n");
    return OK;
}

status ReverseList(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    LinkList prev = NULL, curr = L.head->next, next = NULL;
    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    L.head->next = prev;

    return OK;
}

status RemoveNthFromEnd(List &L, int n)
{
    if (L.head == NULL || n < 1)
        return INFEASIBLE;

    LinkList fast = L.head, slow = L.head;
    for (int i = 0; i <= n; i++)
    {
        if (fast == NULL)
            return ERROR;
        fast = fast->next;
    }

    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
    }
    LinkList temp = slow->next;
    slow->next = slow->next->next;
    free(temp);

    return OK;
}

status SortList(List &L)
{
    if (L.head == NULL)
        return INFEASIBLE;

    vector<int> arr;
    LinkList p = L.head->next;

    while (p != NULL)
    {
        arr.push_back(p->data);
        p = p->next;
    }

    sort(arr.begin(), arr.end());
    p = L.head->next;

    for (std::vector<int>::size_type i = 0; i < arr.size(); i++)
    {
        p->data = arr[i];
        p = p->next;
    }

    return OK;
}

status SaveList(List L, char FileName[])
{
    if (L.head == NULL)
        return INFEASIBLE;

    FILE *fp = fopen(FileName, "w");
    if (fp == NULL)
        return ERROR;

    LinkList p = L.head->next;
    while (p != NULL)
    {
        fprintf(fp, "%d ", p->data);
        p = p->next;
    }
    fclose(fp);

    return OK;
}

status LoadList(List &L, char FileName[])
{
    if (L.head != NULL)
        return INFEASIBLE;

    FILE *fp = fopen(FileName, "r");
    if (fp == NULL)
        return ERROR;

    L.head = (LinkList)malloc(sizeof(LNode));
    L.head->next = NULL;
    LinkList p = L.head;

    ElemType num;
    while (fscanf(fp, "%d", &num) == 1)
    {
        LinkList newNode = (LinkList)malloc(sizeof(LNode));
        newNode->data = num;
        newNode->next = NULL;
        p->next = newNode;
        p = newNode;
    }
    fclose(fp);

    return OK;
}

status AddList(LISTS &Lists, char ListName[])
{
    if (Lists.length >= MAXSIZE)
        return ERROR;

    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)
            return ERROR;
    }

    List *newList = &Lists.elem[Lists.length];
    newList->head = NULL;
    strcpy(newList->name, ListName);
    Lists.length++;

    Lists.cur_index = Lists.length - 1;
    return OK;
}

int RemoveList(LISTS &Lists, char ListName[])
{
    if (Lists.length <= 1)
        return ERROR;

    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)
        {
            DestroyList(Lists.elem[i]);
            for (int j = i; j < Lists.length - 1; j++)
            {
                Lists.elem[j] = Lists.elem[j + 1];
            }
            Lists.elem[Lists.length - 1].head = NULL;
            Lists.elem[Lists.length - 1].name[0] = '\0';
            Lists.length--;

            return OK;
        }
    }

    return ERROR;
}

status LocateList(LISTS Lists, char ListName[])
{
    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)
            return i + 1;
    }

    return ERROR;
}

status SwitchList(LISTS &Lists, char ListName[])
{
    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)
        {
            Lists.cur_index = i;
            return OK;
        }
    }

    return ERROR;
}

void PrintLists(LISTS Lists)
{
    printf("--------------------------------------------------\n");
    printf("当前线性表列表：\n");
    for (int i = 0; i < Lists.length; i++)
    {
        printf("%d. %s\n", i + 1, Lists.elem[i].name);
    }
    printf("--------------------------------------------------\n");
}

void printMenu(LISTS &Lists)
{
    printf("\n");
    printf("--------------------------------------------------\n");
    printf("   Menu for Chain Table On Sequence Structure \n");
    printf("--------------------------------------------------\n\n");
    printf("当前操作表为：%s\n\n", Lists.elem[Lists.cur_index].name);
    printf("        1.  InitList       11. ListDelete   \n");
    printf("        2.  DestroyList    12. ListTraverse \n");
    printf("        3.  ClearList      13. ReverseList  \n");
    printf("        4.  ListEmpty      14. RemovelastN  \n");
    printf("        5.  ListLength     15. SortList     \n");
    printf("        6.  GetElem        16. SaveList     \n");
    printf("        7.  LocateElem     17. LoadList     \n");
    printf("        8.  PriorElem      18. AddList      \n");
    printf("        9.  NextElem       19. RemoveList   \n");
    printf("       10.  ListInsert     20. LocateList   \n");
    printf("       21.  SwitchList     22. PrintLists    \n");
    printf("        0.Exit    \n\n");
    printf("--------------------------------------------------\n");
    printf("请输入功能序号[0~20]:");
}

int main()
{
    LISTS Lists;
    Lists.length = 1;
    Lists.cur_index = 0;
    Lists.elem[0].head = NULL;
    char firstname[30];
    system("cls");
    printf("--------------------------------------------------\n");
    printf("这是一个多线性表系统，可以对单个或多个线性表进行操作\n");
    printf("Ahtuor：HUST Cupid-qrq\n");
    printf("--------------------------------------------------\n");
    printf("在开始之前，请先输入第一个线性表的名称：\n");
    if (scanf("%s", firstname) != 1)
    {
        printf("输入错误，请重新输入：\n");
        scanf("%s", firstname);
    }
    printf("输入成功，第一个线性表的名称是：%s\n", firstname);
    strcpy(Lists.elem[0].name, firstname);
    printf("接下来，进入菜单界面，按任意键继续：\n");
    getchar();
    getchar();

    int op = 1;
    int i, e;
    int pre_e, cur_e, next_e;
    char filename[30], listname[30];

    while (op)
    {
        printMenu(Lists);
        if (scanf("%d", &op) != 1)
        {
            op = -1;
        }

        switch (op)
        {
        case 1:
            if (InitList(Lists.elem[Lists.cur_index]) == OK)
                printf("线性表初始化成功!\n");
            else
                printf("线性表已存在或初始化失败!\n");
            break;

        case 2:
            if (DestroyList(Lists.elem[Lists.cur_index]) == OK)
                printf("表已销毁！\n");
            else
                printf("表不存在，销毁失败！\n");
            break;

        case 3:
            if (ClearList(Lists.elem[Lists.cur_index]) == OK)
                printf("表已清空！\n");
            else
                printf("清空失败，表不存在！\n");
            break;

        case 4:
            if (ListEmpty(Lists.elem[Lists.cur_index]) == TRUE)
                printf("表为空！\n");
            else if (ListEmpty(Lists.elem[Lists.cur_index]) == FALSE)
                printf("表非空！\n");
            else
                printf("表不存在！\n");
            break;

        case 5:
            if (ListLength(Lists.elem[Lists.cur_index]) != INFEASIBLE)
                printf("表长度为：%d\n", ListLength(Lists.elem[Lists.cur_index]));
            else
                printf("表不存在！\n");
            break;

        case 6:
            printf("请输入要获取的元素位置：");
            scanf("%d", &i);
            if (GetElem(Lists.elem[Lists.cur_index], i, e) == OK)
                printf("第 %d 个元素为：%d\n", i, e);
            else
                printf("位置非法或表不存在！\n");
            break;

        case 7:
            printf("请输入要查找的元素值：");
            scanf("%d", &e);
            i = LocateElem(Lists.elem[Lists.cur_index], e);
            if (i != ERROR)
                printf("元素 %d 的位置为：%d\n", e, i);
            else
                printf("查找失败！\n");
            break;

        case 8:
            printf("请输入当前元素值：");
            scanf("%d", &cur_e);
            if (PriorElem(Lists.elem[Lists.cur_index], cur_e, pre_e) == OK)
                printf("前驱元素为：%d\n", pre_e);
            else
                printf("寻找前驱失败！\n");
            break;

        case 9:
            printf("请输入当前元素值：");
            scanf("%d", &cur_e);
            if (NextElem(Lists.elem[Lists.cur_index], cur_e, next_e) == OK)
                printf("后继元素为：%d\n", next_e);
            else
                printf("无后继元素或表不存在！\n");
            break;

        case 10:
            printf("请输入插入位置和元素值：");
            scanf("%d%d", &i, &e);
            if (ListInsert(Lists.elem[Lists.cur_index], i, e) == OK)
                printf("插入成功！\n");
            else
                printf("插入失败，位置非法或表不存在！\n");
            break;

        case 11:
            printf("请输入要删除的位置：");
            scanf("%d", &i);
            if (ListDelete(Lists.elem[Lists.cur_index], i, e) == OK)
                printf("删除成功，删除元素为：%d\n", e);
            else
                printf("删除失败，位置非法或表不存在！\n");
            break;

        case 12:
            printf("当前表内容为：\n");
            if (ListTraverse(Lists.elem[Lists.cur_index]) == INFEASIBLE)
                printf("表不存在！\n");
            break;

        case 13:
            if (ReverseList(Lists.elem[Lists.cur_index]) == OK)
                printf("线性表已反转成功！\n");
            else
                printf("表不存在！\n");
            break;

        case 14:
            printf("请输入要删除的倒数的元素位置：");
            scanf("%d", &i);
            if (RemoveNthFromEnd(Lists.elem[Lists.cur_index], i) == OK)
                printf("删除成功！\n");
            else
                printf("删除失败，位置非法或表不存在！\n");
            break;

        case 15:
            if (SortList(Lists.elem[Lists.cur_index]) == OK)
                printf("线性表已排序成功！\n");
            else
                printf("表不存在！\n");
            break;

        case 16:
            printf("请输入文件名：");
            scanf("%s", filename);
            if (SaveList(Lists.elem[Lists.cur_index], filename) == OK)
                printf("保存成功！\n");
            else
                printf("保存失败或表不存在！\n");
            break;

        case 17:
            printf("请输入文件名：");
            scanf("%s", filename);
            if (LoadList(Lists.elem[Lists.cur_index], filename) == OK)
                printf("加载成功！\n");
            else
                printf("加载失败或表已存在！\n");
            break;

        case 18:
            printf("请输入要添加的表名：");
            scanf("%s", listname);
            if (AddList(Lists, listname) == OK)
                printf("添加成功！\n");
            else
                printf("添加失败，表已存在或内存不足！\n");
            break;

        case 19:
            printf("请输入要删除的表名：");
            scanf("%s", listname);
            if (RemoveList(Lists, listname) == OK)
                printf("删除成功！\n");
            else
                printf("删除失败，表不存在！\n");
            break;

        case 20:
            printf("请输入要查找的表名：");
            scanf("%s", listname);
            i = LocateList(Lists, listname);
            if (i != ERROR)
                printf("表 %s 的位置为：%d\n", listname, i);
            else
                printf("表未找到！\n");
            break;

        case 21:
            printf("请输入要切换到的表名：");
            scanf("%s", listname);
            i = LocateList(Lists, listname);
            if (i != ERROR)
            {
                Lists.cur_index = i - 1;
                printf("切换成功，当前操作表为：%s\n", Lists.elem[Lists.cur_index].name);
            }
            else
                printf("输入的表不存在！\n");
            break;

        case 22:
            PrintLists(Lists);
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
