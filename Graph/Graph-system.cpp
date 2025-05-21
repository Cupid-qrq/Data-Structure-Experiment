/*---------------------------------------------
|  Graph System On Adjacency List             |
|  Author:qrq 2402                            |
|  Date:2025.5.21                             |
---------------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20
#define MAX_GRAPH_SIZE 10

typedef int status;
typedef int KeyType;

typedef enum
{
    DG,
    DN,
    UDG,
    UDN
} GraphKind; // 本程序以无向图为例

typedef struct
{
    KeyType key;
    char others[20];
} VertexType; // 顶点类型定义

typedef struct ArcNode
{                            // 邻接表结点类型定义
    int adjvex;              // 顶点位置编号
    struct ArcNode *nextarc; // 下一个结点指针
} ArcNode;
typedef struct VNode
{                      // 头结点及其数组类型定义
    VertexType data;   // 顶点信息
    ArcNode *firstarc; // 指向第一条弧
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct
{                       // 邻接表的类型定义
    AdjList vertices;   // 头结点数组
    int vexnum, arcnum; // 顶点数、弧数
    GraphKind kind;     // 图的类型
    char name[30];      // 图的名称
} ALGraph;

typedef struct
{
    ALGraph elem[MAX_GRAPH_SIZE];
    int length;
    int cur_index;
} GRAPHS;

/*----------------------------------------------*/

// basic operations
status isKeyTypeUnique(VertexType V[]);
status isKeyTypeUniqueInAdjlist(AdjList list, int len);
status visit(VertexType v);
void dfs(ALGraph G, int n, int visited[]);

// ALGraph system operations
status CreateGraph(ALGraph &G, VertexType V[], KeyType VR[][2]);
status DestroyGraph(ALGraph &G);
int LocateVertex(ALGraph G, KeyType u);
status PutVex(ALGraph &G, KeyType u, VertexType value);
int FirstAdjVex(ALGraph G, KeyType u);
int NextAdjVex(ALGraph G, KeyType v, KeyType w);
status InsertVex(ALGraph &G, VertexType v);
status DeleteVex(ALGraph &G, KeyType v);
status InsertArc(ALGraph &G, KeyType v, KeyType w);
status DeleteArc(ALGraph &G, KeyType v, KeyType w);
status DFSTraverse(ALGraph &G);
status BFSTraverse(ALGraph &G);

// Additional operations
status SaveGraph(ALGraph G, const char FileName[]);
status LoadGraph(ALGraph &G, const char FileName[]);
int ShortestPathLength(ALGraph G, KeyType v, KeyType k);
void VerticesWithinDistanceK(ALGraph G, KeyType v, KeyType K);
int ConnectedComponentsNum(ALGraph G);

// multiple graphs operation
int LocateGraph(GRAPHS Graphs, char *Graphname);
status AddGraph(GRAPHS &Graphs, char *Graphname);
status RemoveGraph(GRAPHS &Graphs, char *Graphname);
status SwitchGraph(GRAPHS &Graphs, char *Graphname);
void PrintGraphslist(GRAPHS Graphs);
void PrintMenu(GRAPHS Graphs);
void PrintGraphWithAdjList(ALGraph G);

/*----------------------------------------------*/

status isKeyTypeUnique(VertexType V[])
{
    unordered_map<int, int> keyCnt;
    unordered_map<string, int> othersCnt;
    int i = 0;
    while (V[i].key != -1)
    {
        keyCnt[V[i].key]++;
        othersCnt[V[i].others]++;
        if (keyCnt[V[i].key] > 1 || othersCnt[V[i].others] > 1)
            return ERROR;
        i++;
    }
    return OK;
}

status CreateGraph(ALGraph &G, VertexType V[], KeyType VR[][2])
{
    // G.kind = UDG;

    // 算出顶点数和边数
    G.vexnum = 0;
    G.arcnum = 0;
    int i = 0, j = 0;

    while (V[i].key != -1)
    {
        i++;
    }

    while (VR[j][0] != -1)
    {
        j++;
    }

    G.vexnum = i;
    G.arcnum = j;

    // 判断顶点数和边数是否超过最大值
    if (G.arcnum > MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2)
        return ERROR;
    if (G.vexnum > MAX_VERTEX_NUM)
        return ERROR;
    if (isKeyTypeUnique(V) == ERROR)
        return ERROR;

    // 创建图
    for (int i = 0; i < G.vexnum; i++)
    {
        G.vertices[i].data = V[i];
        G.vertices[i].firstarc = NULL;
    }

    for (int i = 0; i < j; i++)
    {
        int v1 = LocateVertex(G, VR[i][0]);
        int v2 = LocateVertex(G, VR[i][1]);

        // 判断边的两个顶点是否在图中
        if (v1 == -1 || v2 == -1)
            return ERROR;
        // 判断边的两个顶点是否相同，如果相同则跳过
        if (v1 == v2)
        {
            G.arcnum--;
            continue;
        }
        // 判断边的两个顶点是否已经存在
        int flag = 1;
        if (G.vertices[v1].firstarc != NULL)
        {
            ArcNode *p = G.vertices[v1].firstarc;
            while (p != NULL)
            {
                if (p->adjvex == v2)
                    flag = 0;
                p = p->nextarc;
            }
        }
        if (!flag)
        {
            G.arcnum--;
            continue;
        }

        // 在v1的邻接表中插入v2
        ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
        if (!p)
            return OVERFLOW;
        p->adjvex = v2;
        p->nextarc = G.vertices[v1].firstarc;
        G.vertices[v1].firstarc = p;

        // 在v2的邻接表中插入v1
        ArcNode *q = (ArcNode *)malloc(sizeof(ArcNode));
        if (!q)
            return OVERFLOW;
        q->adjvex = v1;
        q->nextarc = G.vertices[v2].firstarc;
        G.vertices[v2].firstarc = q;
    }

    return OK;
}

status DestroyGraph(ALGraph &G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    for (int i = 0; i < G.vexnum; i++)
    {
        ArcNode *p = G.vertices[i].firstarc;
        while (p)
        {
            ArcNode *q = p;
            p = p->nextarc;
            free(q);
        }
        G.vertices[i].firstarc = NULL;
    }

    G.vexnum = 0;
    G.arcnum = 0;

    return OK;
    /********** End **********/
}

int LocateVertex(ALGraph G, KeyType u)
// 根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.vertices[i].data.key == u)
            return i;
    }
    return -1;

    /********** End **********/
}

status isKeyTypeUniqueInAdjlist(AdjList list, int len)
{
    unordered_map<int, int> keyCnt;
    unordered_map<string, int> othersCnt;
    for (int i = 0; i < len; i++)
    {
        keyCnt[list[i].data.key]++;
        othersCnt[list[i].data.others]++;
        if (keyCnt[list[i].data.key] > 1 || othersCnt[list[i].data.others] > 1)
            return ERROR;
    }
    return OK;
}

status PutVex(ALGraph &G, KeyType u, VertexType value)
// 根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
// 如果查找失败或关键字不唯一，返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/

    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.vertices[i].data.key == u)
        {
            VertexType temp = G.vertices[i].data;
            G.vertices[i].data = value;
            if (isKeyTypeUniqueInAdjlist(G.vertices, G.vexnum))
            {
                return OK;
            }
            else
            {
                G.vertices[i].data = temp;
                return ERROR;
            }
        }
    }

    return ERROR;
    /********** End **********/
}

int FirstAdjVex(ALGraph G, KeyType u)
// 根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int i = LocateVertex(G, u);
    ArcNode *p = G.vertices[i].firstarc;
    if (p)
    {
        return p->adjvex;
    }
    else
        return -1;
    /********** End **********/
}

int NextAdjVex(ALGraph G, KeyType v, KeyType w)
// v对应G的一个顶点,w对应v的邻接顶点；操作结果是返回v的（相对于w）下一个邻接顶点的位序；如果w是最后一个邻接顶点，或v、w对应顶点不存在，则返回-1。
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
        return -1;

    ArcNode *p = G.vertices[n].firstarc;
    while (p && p->adjvex != m)
    {
        p = p->nextarc;
    }

    if (p && p->nextarc)
    {
        return p->nextarc->adjvex;
    }
    else
        return -1;

    /********** End **********/
}

status InsertVex(ALGraph &G, VertexType v)
// 在图G中插入顶点v，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if (G.vexnum == MAX_VERTEX_NUM)
        return ERROR;

    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.vertices[i].data.key == v.key || strcmp(G.vertices[i].data.others, v.others) == 0)
        {
            return ERROR;
        }
    }

    G.vertices[G.vexnum].data = v;
    G.vertices[G.vexnum].firstarc = NULL;
    G.vexnum++;
    return OK;
    /********** End **********/
}

status DeleteVex(ALGraph &G, KeyType v)
// 在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int n = LocateVertex(G, v);
    if (n == -1)
        return ERROR;

    // 如果只有一个结点，不能删除
    if (G.vexnum == 1)
        return ERROR;

    // 1.删除所有指向该点的邻接表结点
    for (int i = 0; i < G.vexnum; i++)
    {
        if (i == n)
            continue;
        ArcNode *pre = NULL;
        ArcNode *p = G.vertices[i].firstarc;

        while (p)
        {
            if (p->adjvex == n)
            {
                if (pre == NULL)
                {
                    G.vertices[i].firstarc = p->nextarc;
                }
                else
                {
                    pre->nextarc = p->nextarc;
                }

                ArcNode *tmp = p;
                p = p->nextarc;
                free(tmp);
                G.arcnum--;
            }
            else
            {
                if (p->adjvex > n)
                    p->adjvex--;

                pre = p;
                p = p->nextarc;
            }
        }
    }

    // 2.删除该点关联的邻接表结点
    ArcNode *p = G.vertices[n].firstarc;
    while (p)
    {
        ArcNode *tmp = p;
        p = p->nextarc;
        free(tmp);
    }

    // 3.调整顶点数组
    for (int i = n; i < G.vexnum - 1; i++)
    {
        G.vertices[i] = G.vertices[i + 1];
    }
    G.vexnum--;

    // printf("%d %d",G.vexnum,G.arcnum);

    return OK;
    /********** End **********/
}

status InsertArc(ALGraph &G, KeyType v, KeyType w)
// 在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
        return ERROR;

    ArcNode *temp = G.vertices[m].firstarc;
    while (temp)
    {
        if (temp->adjvex == n)
            return ERROR;
        temp = temp->nextarc;
    }

    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = n;
    p->nextarc = G.vertices[m].firstarc;
    G.vertices[m].firstarc = p;

    ArcNode *q = (ArcNode *)malloc(sizeof(ArcNode));
    q->adjvex = m;
    q->nextarc = G.vertices[n].firstarc;
    G.vertices[n].firstarc = q;

    G.arcnum++;

    return OK;
    /********** End **********/
}

status DeleteArc(ALGraph &G, KeyType v, KeyType w)
// 在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
        return ERROR;

    ArcNode *temp = G.vertices[m].firstarc;
    int flag = 0;
    while (temp)
    {
        if (temp->adjvex == n)
            flag = 1;
        temp = temp->nextarc;
    }
    if (!flag)
        return ERROR;

    ArcNode *p = G.vertices[m].firstarc;
    temp = NULL;
    while (p && p->adjvex != n)
    {
        temp = p;
        p = p->nextarc;
    }

    if (p == G.vertices[m].firstarc)
    {
        G.vertices[m].firstarc = p->nextarc;
    }
    else
    {
        temp->nextarc = p->nextarc;
    }
    free(p);

    ArcNode *q = G.vertices[n].firstarc;
    temp = NULL;
    while (q && q->adjvex != m)
    {
        temp = q;
        q = q->nextarc;
    }

    if (q == G.vertices[n].firstarc)
    {
        G.vertices[n].firstarc = q->nextarc;
    }
    else
    {
        temp->nextarc = q->nextarc;
    }
    free(q);

    G.arcnum--;

    return OK;

    /********** End **********/
}

void dfs(ALGraph G, int n, int visited[])
{
    visited[n] = 1;
    visit(G.vertices[n].data);

    ArcNode *p = G.vertices[n].firstarc;
    while (p)
    {
        if (!visited[p->adjvex])
        {
            dfs(G, p->adjvex, visited);
        }
        p = p->nextarc;
    }

    return;
}

status visit(VertexType v)
{
    // 访问顶点的操作
    printf("%d %s\n", v.key, v.others);
    return OK;
}

status DFSTraverse(ALGraph &G)
// 对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int visited[G.vexnum] = {0};

    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            dfs(G, i, visited);
        }
    }

    return OK;
    /********** End **********/
}

status BFSTraverse(ALGraph &G)
// 对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    bool visited[G.vexnum] = {FALSE};
    queue<int> q;

    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            visit(G.vertices[i].data);
            visited[i] = TRUE;
            q.push(i);
        }

        while (!q.empty())
        {
            int n = q.front();
            q.pop();
            ArcNode *p = G.vertices[n].firstarc;

            while (p)
            {
                int m = p->adjvex;
                if (!visited[m])
                {
                    visit(G.vertices[m].data);
                    visited[m] = TRUE;
                    q.push(m);
                }
                p = p->nextarc;
            }
        }
    }

    return OK;
    /********** End **********/
}

status SaveGraph(ALGraph G, const char FileName[])
{
    FILE *fp = fopen(FileName, "w");
    if (!fp)
        return ERROR;

    fprintf(fp, "%d %d\n", G.vexnum, G.arcnum); // 写入顶点数和边数

    for (int i = 0; i < G.vexnum; ++i)
    {
        // 写入顶点 key 和 others
        fprintf(fp, "%d %s", G.vertices[i].data.key, G.vertices[i].data.others);

        // 写入邻接点索引
        ArcNode *p = G.vertices[i].firstarc;
        while (p != NULL)
        {
            fprintf(fp, " %d", p->adjvex); // 写入邻接点在数组中的索引
            p = p->nextarc;
        }
        fprintf(fp, " -1 "); // 结束邻接点的输入
        // 换行
        fprintf(fp, "\n");
    }

    fclose(fp);
    return OK;
}

status LoadGraph(ALGraph &G, const char FileName[])
{
    FILE *fp = fopen(FileName, "r");
    if (!fp)
        return ERROR;

    // 读取顶点数和边数
    fscanf(fp, "%d %d", &G.vexnum, &G.arcnum);
    if (G.vexnum > MAX_VERTEX_NUM || G.arcnum > MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2)
        return ERROR;

    // 读取顶点和它的邻接点
    for (int i = 0; i < G.vexnum; ++i)
    {
        fscanf(fp, "%d %s", &G.vertices[i].data.key, G.vertices[i].data.others);
        G.vertices[i].firstarc = NULL;
        int adjvex;
        while (fscanf(fp, "%d", &adjvex) == 1 && adjvex != -1)
        {
            ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
            if (!p)
                return OVERFLOW;
            p->adjvex = adjvex;
            // 尾插法构建邻接表
            ArcNode *q = G.vertices[i].firstarc;
            if (q == NULL)
            {
                G.vertices[i].firstarc = p;
                p->nextarc = NULL;
            }
            else
            {
                while (q->nextarc != NULL)
                    q = q->nextarc;
                q->nextarc = p;
                p->nextarc = NULL;
            }
        }
    }

    fclose(fp);
    return OK;
}

int ShortestPathLength(ALGraph G, KeyType v, KeyType k)
{
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, k);
    if (n == -1 || m == -1)
        return ERROR;

    int visited[G.vexnum] = {0};
    int distance[G.vexnum] = {0};

    queue<int> q;
    q.push(n);

    visited[n] = 1;
    distance[n] = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        ArcNode *p = G.vertices[u].firstarc;
        while (p)
        {
            int w = p->adjvex;
            if (!visited[w])
            {
                visited[w] = 1;
                distance[w] = distance[u] + 1;
                q.push(w);
            }
            p = p->nextarc;
        }
    }

    if (visited[m])
        return distance[m];
    else
        return ERROR;
}

void VerticesWithinDistanceK(ALGraph G, KeyType v, KeyType K)
{
    if (K < 0)
    {
        printf("输入的K值不合法\n");
        return;
    }

    int start = LocateVertex(G, v);
    if (start == -1)
    {
        printf("输入的顶点不合法\n");
        return;
    }

    if (G.vexnum == 0)
    {
        printf("图为空\n");
        return;
    }

    if (G.arcnum == 0)
    {
        printf("图中没有边\n");
        return;
    }

    // 使用一次BFS求所有点到v的距离，避免多次ShortestPathLength调用
    int distance[MAX_VERTEX_NUM];
    for (int i = 0; i < G.vexnum; ++i)
        distance[i] = -1;
    queue<int> q;
    distance[start] = 0;
    q.push(start);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        ArcNode *p = G.vertices[u].firstarc;
        while (p)
        {
            int w = p->adjvex;
            if (distance[w] == -1)
            {
                distance[w] = distance[u] + 1;
                q.push(w);
            }
            p = p->nextarc;
        }
    }

    int flag = 0;
    for (int i = 0; i < G.vexnum; i++)
    {
        if (i == start)
            continue;
        if (distance[i] != -1 && distance[i] < K)
        {
            flag = 1;
            printf(" %d %s", G.vertices[i].data.key, G.vertices[i].data.others);
        }
    }

    if (!flag)
        printf("没有满足条件的顶点\n");

    return;
}

int ConnectedComponentsNum(ALGraph G)
{
    vector<int> visited(G.vexnum, 0);
    int cnt = 0;

    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            cnt++;
            queue<int> q;
            q.push(i);
            visited[i] = 1;

            while (!q.empty())
            {
                int u = q.front();
                q.pop();

                ArcNode *p = G.vertices[u].firstarc;
                while (p)
                {
                    int w = p->adjvex;
                    if (!visited[w])
                    {
                        visited[w] = 1;
                        q.push(w);
                    }
                    p = p->nextarc;
                }
            }
        }
    }

    return cnt;
}

int LocateGraph(GRAPHS Graphs, char *Graphname)
{
    for (int i = 0; i < Graphs.length; i++)
    {
        if (strcmp(Graphs.elem[i].name, Graphname) == 0)
            return i;
    }
    return -1;
}

status AddGraph(GRAPHS &Graphs, char *Graphname)
{
    if (Graphs.length >= MAX_GRAPH_SIZE)
    {
        printf("图的数量已达上限，无法添加新图\n");
        return ERROR;
    }

    for (int i = 0; i < Graphs.length; i++)
    {
        if (strcmp(Graphs.elem[i].vertices[0].data.others, Graphname) == 0)
            return ERROR;
    }

    // 创建新图
    ALGraph newGraph;
    newGraph.vexnum = 0;
    newGraph.arcnum = 0;
    newGraph.kind = UDG; // 默认无向图
    strcpy(newGraph.name, Graphname);
    Graphs.elem[Graphs.length] = newGraph;
    Graphs.length++;
    Graphs.cur_index = Graphs.length - 1;

    return OK;
}

status RemoveGraph(GRAPHS &Graphs, char *Graphname)
{
    int index = LocateGraph(Graphs, Graphname);
    if (index == -1)
    {
        printf("图不存在，无法删除\n");
        return ERROR;
    }

    // 销毁图
    DestroyGraph(Graphs.elem[index]);
    memset(&Graphs.elem[index], 0, sizeof(ALGraph));

    // 删除图
    for (int i = index; i < Graphs.length - 1; i++)
    {
        Graphs.elem[i] = Graphs.elem[i + 1];
    }
    Graphs.length--;

    return OK;
}

status SwitchGraph(GRAPHS &Graphs, char *Graphname)
{
    int index = LocateGraph(Graphs, Graphname);
    if (index == -1)
    {
        printf("图不存在，无法切换\n");
        return ERROR;
    }

    Graphs.cur_index = index;
    return OK;
}

void PrintGraphslist(GRAPHS Graphs)
{
    printf("当前图列表：\n");
    for (int i = 0; i < Graphs.length; i++)
    {
        printf("%d: %s\n", i + 1, Graphs.elem[i].name);
    }
}

void PrintGraphWithAdjList(ALGraph G)
{
    printf("\n当前的图和邻接表：\n");
    printf("-----------------------------------------------\n");
    printf("图名称：%s\n", G.name);
    printf("顶点数：%d，边数：%d\n", G.vexnum, G.arcnum);
    printf("-----------------------------------------------\n");
    printf("位置序号  顶点信息         邻接点\n");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < G.vexnum; i++)
    {
        printf("%-9d %-3d %-10s ", i, G.vertices[i].data.key, G.vertices[i].data.others);

        ArcNode *p = G.vertices[i].firstarc;
        if (!p)
        {
            printf("(无邻接点)");
        }
        else
        {
            while (p)
            {
                printf("%d", p->adjvex);
                p = p->nextarc;
                if (p)
                    printf("->");
            }
        }
        printf("\n");
    }
    printf("-----------------------------------------------\n");
}

void PrintMenu(GRAPHS Graphs)
{
    printf("\n");
    printf("---------------------------------------------------------\n");
    printf("   Menu for Graph System On Adjacency List \n");
    printf("---------------------------------------------------------\n\n");
    printf("当前图：%s\n", Graphs.elem[Graphs.cur_index].name);
    printf("Basic Operations:\n");
    printf("      1. CreateGraph              2. DestroyGraph        \n");
    printf("      3. LocateVex                4. PutVex              \n");
    printf("      5. FirstAdjVex              6. NextAdjVex          \n");
    printf("      7. InsertVex                8. DeleteVex           \n");
    printf("      9. InsertArc               10. DeleteArc           \n");
    printf("     11. DFSTraverse             12. BFSTraverse         \n");
    printf("Additional Operations:\n");
    printf("     13. SaveGraph               14. LoadGraph           \n");
    printf("     15. ShortestPathLength      16. VerticesWithinK     \n");
    printf("     17. ConnectedComponentsNum                          \n");
    printf("Multiple Graphs Operations:\n");
    printf("     18. AddGraph                19. RemoveGraph         \n");
    printf("     20. SwitchGraph             21. PrintGraphs         \n");
    printf("     0. Exit                     22. PrintGraphWithAdjList\n");
    printf("---------------------------------------------------------\n");
    printf("请输入操作选项：\n");
}

int main()
{
    GRAPHS Graphs;
    Graphs.length = 1;
    Graphs.cur_index = 0;
    ALGraph G;
    G.vexnum = 0;
    G.arcnum = 0;
    G.kind = UDG; // 默认无向图
    Graphs.elem[0] = G;
    char firstname[30];
    system("cls");
    printf("--------------------------------------------------\n");
    printf("邻接表实现的图系统，可以对单个或多个无向图进行操作\n");
    printf("Author: HUST Cupid-qrq\n");
    printf("--------------------------------------------------\n");
    printf("在开始之前，请先输入第一个图的名称：\n");
    if (scanf("%s", firstname) != 1)
    {
        printf("输入错误，请重新输入：\n");
        scanf("%s", firstname);
    }
    printf("输入成功，第一个图的名称是：%s\n", firstname);
    strcpy(Graphs.elem[0].name, firstname);
    printf("接下来，进入菜单界面，按任意键继续：\n");
    getchar();
    getchar();

    int op = 1;                    // 菜单选项
    VertexType V[MAX_VERTEX_NUM];  // 顶点数组
    KeyType VR[MAX_VERTEX_NUM][2]; // 边数组
    int exist;

    char filename[100], treename[100];
    // to be continued

    while (op)
    {
        PrintMenu(Graphs);

        if (scanf("%d", &op) != 1)
        {
            op = -1;
        }

        switch (op)
        {
        case 1:
        {
            // 创建图
            if (Graphs.elem[Graphs.cur_index].vexnum != 0)
            {
                printf("当前图已存在，创建失败！\n");
                break;
            }

            // 初始化定义数组

            memset(V, 0, sizeof(V));
            memset(VR, 0, sizeof(VR));

            printf("请输入图的顶点序列，格式为：key others，以 -1 nil 结束。\n");
            int i = 0;
            do
            {
                scanf("%d%s", &V[i].key, V[i].others);
            } while (V[i++].key != -1);
            i = 0;
            printf("请输入图的边序列，格式为：v1 v2，以 -1 -1 结束。\n");
            do
            {
                scanf("%d%d", &VR[i][0], &VR[i][1]);
            } while (VR[i++][0] != -1);

            // 构造新图
            status res = CreateGraph(Graphs.elem[Graphs.cur_index], V, VR);
            if (res == OK)
            {
                printf("图创建成功。\n");
            }
            else
            {
                printf("图创建失败，可能原因：重复 key 或内存不足。\n");
            }

            break;
        }
        case 2:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，销毁失败！\n");
                break;
            }

            if (DestroyGraph(Graphs.elem[Graphs.cur_index]) == OK)
                printf("图销毁成功！\n");
            else
                printf("图不存在，销毁失败！\n");

            break;
        }
        case 3:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            int index;
            printf("请输入要查找的结点的关键字：\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            index = LocateVertex(Graphs.elem[Graphs.cur_index], exist);
            if (index == -1)
            {
                printf("查找失败，结点不存在！\n");
            }
            else
            {
                printf("查找成功，结点的顶点位置序号是：%d\n", index);
            }

            break;
        }
        case 4:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，修改失败！\n");
                break;
            }

            exist = 0;
            printf("请输入要修改的结点的关键字：\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            int index = LocateVertex(Graphs.elem[Graphs.cur_index], exist);
            if (index == -1)
            {
                printf("查找失败，结点不存在！\n");
            }
            else
            {
                printf("请输入新的结点值：\n");
                VertexType newVex;
                scanf("%d", &newVex.key);
                strcpy(newVex.others, Graphs.elem[Graphs.cur_index].vertices[index].data.others);

                if (PutVex(Graphs.elem[Graphs.cur_index], exist, newVex) == OK)
                {
                    printf("修改成功！\n");
                }
                else
                {
                    printf("修改失败，可能原因：重复 key 或内存不足。\n");
                }
            }

            break;
        }
        case 5:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            exist = 0;
            printf("请输入要查找的结点的关键字：\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }
            int res = FirstAdjVex(Graphs.elem[Graphs.cur_index], exist);
            if (res == -1)
            {
                printf("查找失败，结点不存在！\n");
            }
            else
            {
                printf("查找成功，结点的第一邻接顶点是：\n");
                printf(" %d %s\n", Graphs.elem[Graphs.cur_index].vertices[res].data.key, Graphs.elem[Graphs.cur_index].vertices[res].data.others);
            }

            break;
        }
        case 6:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            int v, w;
            printf("请输入要查找的结点的关键字：\n");
            if (scanf("%d", &v) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }
            printf("请输入该结点的一个邻接结点的关键字：\n");
            if (scanf("%d", &w) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }
            int res2 = NextAdjVex(Graphs.elem[Graphs.cur_index], v, w);
            if (res2 == -1)
            {
                printf("查找失败，该结点的这个邻接点的下一个邻接点不存在，或该结点不存在，或该结点的这个邻接点不存在！\n");
            }
            else
            {
                printf("查找成功，结点的下一个邻接顶点是：\n");
                printf(" %d %s\n", Graphs.elem[Graphs.cur_index].vertices[res2].data.key, Graphs.elem[Graphs.cur_index].vertices[res2].data.others);
            }

            break;
        }
        case 7:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，插入顶点失败！\n");
                break;
            }

            VertexType newVex;
            printf("请输入要插入的结点的key和others：\n");
            if (scanf("%d %s", &newVex.key, newVex.others) != 2)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (InsertVex(Graphs.elem[Graphs.cur_index], newVex) == OK)
            {
                printf("插入成功！\n");
            }
            else
            {
                printf("插入失败，可能原因：重复 key 或顶点已满。\n");
            }

            break;
        }
        case 8:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，删除顶点失败！\n");
                break;
            }

            int exist = 0;
            printf("请输入要删除的结点的关键字：\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (DeleteVex(Graphs.elem[Graphs.cur_index], exist) == OK)
            {
                printf("删除成功！\n");
            }
            else
            {
                printf("删除失败，可能原因：结点不存在或只剩一个结点。\n");
            }

            break;
        }
        case 9:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，插入边失败！\n");
                break;
            }

            int v1, v2;
            printf("请输入要插入的弧的两个顶点的关键字：\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (InsertArc(Graphs.elem[Graphs.cur_index], v1, v2) == OK)
            {
                printf("插入成功！\n");
            }
            else
            {
                printf("插入失败，可能原因：弧已存在或顶点不存在。\n");
            }

            break;
        }
        case 10:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，删除边失败！\n");
                break;
            }

            int v1, v2;
            v1 = v2 = 0;
            printf("请输入要删除的弧的两个顶点的关键字：\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (DeleteArc(Graphs.elem[Graphs.cur_index], v1, v2) == OK)
            {
                printf("删除成功！\n");
            }
            else
            {
                printf("删除失败，可能原因：边不存在或顶点不存在。\n");
            }

            break;
        }
        case 11:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，遍历失败！\n");
                break;
            }

            printf("深度优先遍历结果：\n");
            DFSTraverse(Graphs.elem[Graphs.cur_index]);
            printf("\n");

            break;
        }
        case 12:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，遍历失败！\n");
                break;
            }

            printf("广度优先遍历结果：\n");
            BFSTraverse(Graphs.elem[Graphs.cur_index]);
            printf("\n");

            break;
        }
        case 13:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，保存失败！\n");
                break;
            }

            printf("请输入要保存的文件名：\n");
            if (scanf("%s", filename) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (SaveGraph(Graphs.elem[Graphs.cur_index], filename) == OK)
            {
                printf("保存成功！\n");
            }
            else
            {
                printf("保存失败。\n");
            }

            break;
        }
        case 14:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum != 0)
            {
                printf("当前图已存在，加载失败！\n");
                break;
            }

            printf("请输入要加载的文件名：\n");
            if (scanf("%s", filename) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (LoadGraph(Graphs.elem[Graphs.cur_index], filename) == OK)
            {
                printf("加载成功！\n");
            }
            else
            {
                printf("加载失败。\n");
            }

            break;
        }
        case 15:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            int v1, v2;
            v1 = v2 = 0;
            printf("请输入要查找的结点的关键字：\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            int res3 = ShortestPathLength(Graphs.elem[Graphs.cur_index], v1, v2);
            if (res3 == ERROR)
            {
                printf("查找失败，结点不存在！\n");
            }
            else
            {
                printf("查找成功，最短路径长度是：%d\n", res3);
            }

            break;
        }
        case 16:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            int v1 = 0;
            int k = 0;
            printf("请输入要查找的结点的关键字：\n");
            if (scanf("%d", &v1) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }
            printf("请输入要查找的距离：\n");
            if (scanf("%d", &k) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }
            printf("查找结果：\n");
            VerticesWithinDistanceK(Graphs.elem[Graphs.cur_index], v1, k);
            printf("\n");

            break;
        }
        case 17:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("当前图不存在，查找失败！\n");
                break;
            }

            int num = ConnectedComponentsNum(Graphs.elem[Graphs.cur_index]);
            printf("连通分量的个数是：%d\n", num);

            break;
        }
        case 18:
        {
            if (Graphs.length >= MAX_GRAPH_SIZE)
            {
                printf("图的数量已达上限，无法添加新图\n");
                break;
            }

            printf("请输入要添加的图的名称：\n");
            if (scanf("%s", treename) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (AddGraph(Graphs, treename) == OK)
            {
                printf("添加成功！\n");
            }
            else
            {
                printf("添加失败，可能原因：图已存在或内存不足。\n");
            }

            break;
        }
        case 19:
        {
            printf("请输入要删除的图的名称：\n");
            if (scanf("%s", treename) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (RemoveGraph(Graphs, treename) == OK)
            {
                printf("删除成功！\n");
            }

            break;
        }
        case 20:
        {
            printf("请输入要切换的图的名称：\n");
            if (scanf("%s", treename) != 1)
            {
                printf("输入错误，请重新输入：\n");
                continue;
            }

            if (SwitchGraph(Graphs, treename) == OK)
            {
                printf("切换成功！\n");
            }

            break;
        }
        case 21:
        {
            PrintGraphslist(Graphs);
            break;
        }

        case 22:
        {
            PrintGraphWithAdjList(Graphs.elem[Graphs.cur_index]);
            break;
        }
        case 0:
        {
            printf("退出系统！\n");
            break;
        }
        default:
            printf("输入错误，请重新输入：\n");
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
