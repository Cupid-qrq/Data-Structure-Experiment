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
} GraphKind; // ������������ͼΪ��

typedef struct
{
    KeyType key;
    char others[20];
} VertexType; // �������Ͷ���

typedef struct ArcNode
{                            // �ڽӱ������Ͷ���
    int adjvex;              // ����λ�ñ��
    struct ArcNode *nextarc; // ��һ�����ָ��
} ArcNode;
typedef struct VNode
{                      // ͷ��㼰���������Ͷ���
    VertexType data;   // ������Ϣ
    ArcNode *firstarc; // ָ���һ����
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct
{                       // �ڽӱ�����Ͷ���
    AdjList vertices;   // ͷ�������
    int vexnum, arcnum; // ������������
    GraphKind kind;     // ͼ������
    char name[30];      // ͼ������
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

    // ����������ͱ���
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

    // �ж϶������ͱ����Ƿ񳬹����ֵ
    if (G.arcnum > MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2)
        return ERROR;
    if (G.vexnum > MAX_VERTEX_NUM)
        return ERROR;
    if (isKeyTypeUnique(V) == ERROR)
        return ERROR;

    // ����ͼ
    for (int i = 0; i < G.vexnum; i++)
    {
        G.vertices[i].data = V[i];
        G.vertices[i].firstarc = NULL;
    }

    for (int i = 0; i < j; i++)
    {
        int v1 = LocateVertex(G, VR[i][0]);
        int v2 = LocateVertex(G, VR[i][1]);

        // �жϱߵ����������Ƿ���ͼ��
        if (v1 == -1 || v2 == -1)
            return ERROR;
        // �жϱߵ����������Ƿ���ͬ�������ͬ������
        if (v1 == v2)
        {
            G.arcnum--;
            continue;
        }
        // �жϱߵ����������Ƿ��Ѿ�����
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

        // ��v1���ڽӱ��в���v2
        ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
        if (!p)
            return OVERFLOW;
        p->adjvex = v2;
        p->nextarc = G.vertices[v1].firstarc;
        G.vertices[v1].firstarc = p;

        // ��v2���ڽӱ��в���v1
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
/*��������ͼG,ɾ��G��ȫ������ͱ�*/
{
    // �������ﲹ����룬��ɱ�������
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
// ����u��ͼG�в��Ҷ��㣬���ҳɹ�����λ�򣬷��򷵻�-1��
{
    // �������ﲹ����룬��ɱ�������
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
// ����u��ͼG�в��Ҷ��㣬���ҳɹ����ö���ֵ�޸ĳ�value������OK��
// �������ʧ�ܻ�ؼ��ֲ�Ψһ������ERROR
{
    // �������ﲹ����룬��ɱ�������
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
// ����u��ͼG�в��Ҷ��㣬���ҳɹ����ض���u�ĵ�һ�ڽӶ���λ�򣬷��򷵻�-1��
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/

    int i = LocateVertex(G, u);
    if (i == -1)
    {

        return -1;
    }
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
// v��ӦG��һ������,w��Ӧv���ڽӶ��㣻��������Ƿ���v�ģ������w����һ���ڽӶ����λ�����w�����һ���ڽӶ��㣬��v��w��Ӧ���㲻���ڣ��򷵻�-1��
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
    {
        printf("���㲻����\n");
        return -1;
    }

    ArcNode *p = G.vertices[n].firstarc;
    while (p && p->adjvex != m)
    {
        p = p->nextarc;
    }

    if (p == NULL)
    {
        printf("���� %d ���Ƕ��� %d ���ڽӶ���\n", w, v);
        return -1;
    }

    if (p && p->nextarc)
    {
        return p->nextarc->adjvex;
    }
    else
    {
        printf("���� %d �Ƕ��� %d �����һ���ڽӶ���,û����һ���ڽӶ���\n", w, v);
        return -1;
    }
    /********** End **********/
}

status InsertVex(ALGraph &G, VertexType v)
// ��ͼG�в��붥��v���ɹ�����OK,���򷵻�ERROR
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/
    if (G.vexnum == MAX_VERTEX_NUM)
    {
        printf("ͼ�Ķ������Ѵ����ޣ��޷������¶���\n");
        return ERROR;
    }
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.vertices[i].data.key == v.key || strcmp(G.vertices[i].data.others, v.others) == 0)
        {
            printf("ͼ���Ѵ��ڸö��㣬�޷�����\n");
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
// ��ͼG��ɾ���ؼ���v��Ӧ�Ķ����Լ���صĻ����ɹ�����OK,���򷵻�ERROR
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/
    int n = LocateVertex(G, v);
    if (n == -1)
    {
        printf("ͼ�в����ڸö��㣬�޷�ɾ��\n");
        return ERROR;
    }

    // ���ֻ��һ����㣬����ɾ��
    if (G.vexnum == 1)
    {
        printf("ͼ��ֻ��һ����㣬�޷�ɾ��\n");
        return ERROR;
    }

    // 1.ɾ������ָ��õ���ڽӱ���
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

    // 2.ɾ���õ�������ڽӱ���
    ArcNode *p = G.vertices[n].firstarc;
    while (p)
    {
        ArcNode *tmp = p;
        p = p->nextarc;
        free(tmp);
    }

    // 3.������������
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
// ��ͼG�����ӻ�<v,w>���ɹ�����OK,���򷵻�ERROR
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
    {
        printf("ͼ�в����ڸö��㣬�޷�����\n");
        return ERROR;
    }

    ArcNode *temp = G.vertices[m].firstarc;
    while (temp)
    {
        if (temp->adjvex == n)
        {
            printf("ͼ���Ѵ��ڸû����޷�����\n");
            return ERROR;
        }
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
// ��ͼG��ɾ����<v,w>���ɹ�����OK,���򷵻�ERROR
{
    // �������ﲹ����룬��ɱ�������
    /********** Begin *********/
    int n = LocateVertex(G, v);
    int m = LocateVertex(G, w);
    if (n == -1 || m == -1)
    {
        printf("ͼ�в����ڸö��㣬�޷�ɾ��\n");
        return ERROR;
    }

    ArcNode *temp = G.vertices[m].firstarc;
    int flag = 0;
    while (temp)
    {
        if (temp->adjvex == n)
            flag = 1;
        temp = temp->nextarc;
    }
    if (!flag)
    {
        printf("ͼ�в����ڸû����޷�ɾ��\n");
        return ERROR;
    }

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
    // ���ʶ���Ĳ���
    printf("%d %s\n", v.key, v.others);
    return OK;
}

status DFSTraverse(ALGraph &G)
// ��ͼG������������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
{
    // �������ﲹ����룬��ɱ�������
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
// ��ͼG���й�������������������ζ�ͼ�е�ÿһ������ʹ�ú���visit����һ�Σ��ҽ�����һ��
{
    // �������ﲹ����룬��ɱ�������
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

    fprintf(fp, "%d %d\n", G.vexnum, G.arcnum); // д�붥�����ͱ���

    for (int i = 0; i < G.vexnum; ++i)
    {
        // д�붥�� key �� others
        fprintf(fp, "%d %s", G.vertices[i].data.key, G.vertices[i].data.others);

        // д���ڽӵ�����
        ArcNode *p = G.vertices[i].firstarc;
        while (p != NULL)
        {
            fprintf(fp, " %d", p->adjvex); // д���ڽӵ��������е�����
            p = p->nextarc;
        }
        fprintf(fp, " -1 "); // �����ڽӵ������
        // ����
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

    // ��ȡ�������ͱ���
    fscanf(fp, "%d %d", &G.vexnum, &G.arcnum);
    if (G.vexnum > MAX_VERTEX_NUM || G.arcnum > MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2)
        return ERROR;

    // ��ȡ����������ڽӵ�
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
            // β�巨�����ڽӱ�
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
        printf("�����Kֵ���Ϸ�\n");
        return;
    }

    int start = LocateVertex(G, v);
    if (start == -1)
    {
        printf("����Ķ��㲻�Ϸ�\n");
        return;
    }

    if (G.vexnum == 0)
    {
        printf("ͼΪ��\n");
        return;
    }

    if (G.arcnum == 0)
    {
        printf("ͼ��û�б�\n");
        return;
    }

    // ʹ��һ��BFS�����е㵽v�ľ��룬������ShortestPathLength����
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
        printf("û�����������Ķ���\n");

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
        printf("ͼ�������Ѵ����ޣ��޷������ͼ\n");
        return ERROR;
    }

    for (int i = 0; i < Graphs.length; i++)
    {
        if (strcmp(Graphs.elem[i].name, Graphname) == 0)
        {
            printf("ͼ�Ѵ��ڣ��޷����\n");
            return ERROR;
        }
    }

    // ������ͼ
    ALGraph newGraph;
    newGraph.vexnum = 0;
    newGraph.arcnum = 0;
    newGraph.kind = UDG; // Ĭ������ͼ
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
        printf("ͼ�����ڣ��޷�ɾ��\n");
        return ERROR;
    }

    // ����ͼ
    DestroyGraph(Graphs.elem[index]);
    memset(&Graphs.elem[index], 0, sizeof(ALGraph));

    // ɾ��ͼ
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
        printf("ͼ�����ڣ��޷��л�\n");
        return ERROR;
    }

    Graphs.cur_index = index;
    return OK;
}

void PrintGraphslist(GRAPHS Graphs)
{
    printf("��ǰͼ�б�\n");
    for (int i = 0; i < Graphs.length; i++)
    {
        printf("%d: %s\n", i + 1, Graphs.elem[i].name);
    }
}

void PrintGraphWithAdjList(ALGraph G)
{
    printf("\n��ǰ��ͼ���ڽӱ�\n");
    printf("-----------------------------------------------\n");
    printf("ͼ���ƣ�%s\n", G.name);
    printf("��������%d��������%d\n", G.vexnum, G.arcnum);
    printf("-----------------------------------------------\n");
    printf("λ�����  ������Ϣ         �ڽӵ�\n");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < G.vexnum; i++)
    {
        printf("%-9d %-3d %-10s ", i, G.vertices[i].data.key, G.vertices[i].data.others);

        ArcNode *p = G.vertices[i].firstarc;
        if (!p)
        {
            printf("(���ڽӵ�)");
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
    printf("��ǰͼ��%s\n", Graphs.elem[Graphs.cur_index].name);
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
    printf("���������ѡ�\n");
}

int main()
{
    GRAPHS Graphs;
    Graphs.length = 1;
    Graphs.cur_index = 0;
    ALGraph G;
    G.vexnum = 0;
    G.arcnum = 0;
    G.kind = UDG; // Ĭ������ͼ
    Graphs.elem[0] = G;
    char firstname[30];
    system("cls");
    printf("--------------------------------------------------\n");
    printf("�ڽӱ�ʵ�ֵ�ͼϵͳ�����ԶԵ�����������ͼ���в���\n");
    printf("Author: HUST Cupid-qrq\n");
    printf("--------------------------------------------------\n");
    printf("�ڿ�ʼ֮ǰ�����������һ��ͼ�����ƣ�\n");
    if (scanf("%s", firstname) != 1)
    {
        printf("����������������룺\n");
        scanf("%s", firstname);
    }
    printf("����ɹ�����һ��ͼ�������ǣ�%s\n", firstname);
    strcpy(Graphs.elem[0].name, firstname);
    printf("������������˵����棬�������������\n");
    getchar();
    getchar();

    int op = 1;                    // �˵�ѡ��
    VertexType V[MAX_VERTEX_NUM];  // ��������
    KeyType VR[MAX_VERTEX_NUM][2]; // ������
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
            // ����ͼ
            if (Graphs.elem[Graphs.cur_index].vexnum != 0)
            {
                printf("��ǰͼ�Ѵ��ڣ�����ʧ�ܣ�\n");
                break;
            }

            // ��ʼ����������

            memset(V, 0, sizeof(V));
            memset(VR, 0, sizeof(VR));

            printf("������ͼ�Ķ������У���ʽΪ��key others���� -1 nil ������\n");
            int i = 0;
            do
            {
                scanf("%d%s", &V[i].key, V[i].others);
            } while (V[i++].key != -1);
            i = 0;
            printf("������ͼ�ı����У���ʽΪ��v1 v2���� -1 -1 ������\n");
            do
            {
                scanf("%d%d", &VR[i][0], &VR[i][1]);
            } while (VR[i++][0] != -1);

            // ������ͼ
            status res = CreateGraph(Graphs.elem[Graphs.cur_index], V, VR);
            if (res == OK)
            {
                printf("ͼ�����ɹ���\n");
            }
            else
            {
                printf("ͼ����ʧ�ܣ�����ԭ���ظ� key ���ڴ治�㡣\n");
            }

            break;
        }
        case 2:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            if (DestroyGraph(Graphs.elem[Graphs.cur_index]) == OK)
                printf("ͼ���ٳɹ���\n");
            else
                printf("ͼ�����ڣ�����ʧ�ܣ�\n");

            break;
        }
        case 3:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            int index;
            printf("������Ҫ���ҵĽ��Ĺؼ��֣�\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            index = LocateVertex(Graphs.elem[Graphs.cur_index], exist);
            if (index == -1)
            {
                printf("����ʧ�ܣ���㲻���ڣ�\n");
            }
            else
            {
                printf("���ҳɹ������Ķ���λ������ǣ�%d\n", index);
            }

            break;
        }
        case 4:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ��޸�ʧ�ܣ�\n");
                break;
            }

            exist = 0;
            printf("������Ҫ�޸ĵĽ��Ĺؼ��֣�\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            int index = LocateVertex(Graphs.elem[Graphs.cur_index], exist);
            if (index == -1)
            {
                printf("����ʧ�ܣ���㲻���ڣ�\n");
            }
            else
            {
                printf("�������µĽ��ֵ��\n");
                VertexType newVex;
                scanf("%d", &newVex.key);
                strcpy(newVex.others, Graphs.elem[Graphs.cur_index].vertices[index].data.others);

                if (PutVex(Graphs.elem[Graphs.cur_index], exist, newVex) == OK)
                {
                    printf("�޸ĳɹ���\n");
                }
                else
                {
                    printf("�޸�ʧ�ܣ�����ԭ���ظ� key ���ڴ治�㡣\n");
                }
            }

            break;
        }
        case 5:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            exist = 0;
            printf("������Ҫ���ҵĽ��Ĺؼ��֣�\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("����������������룺\n");
                continue;
            }
            int res = FirstAdjVex(Graphs.elem[Graphs.cur_index], exist);
            if (res == -1)
            {
                printf("����ʧ�ܣ���㲻���ڣ�\n");
            }
            else
            {
                printf("���ҳɹ������ĵ�һ�ڽӶ����ǣ�\n");
                printf(" %d %s\n", Graphs.elem[Graphs.cur_index].vertices[res].data.key, Graphs.elem[Graphs.cur_index].vertices[res].data.others);
            }

            break;
        }
        case 6:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            int v, w;
            printf("������Ҫ���ҵĽ��Ĺؼ��֣�\n");
            if (scanf("%d", &v) != 1)
            {
                printf("����������������룺\n");
                continue;
            }
            printf("������ý���һ���ڽӽ��Ĺؼ��֣�\n");
            if (scanf("%d", &w) != 1)
            {
                printf("����������������룺\n");
                continue;
            }
            int res2 = NextAdjVex(Graphs.elem[Graphs.cur_index], v, w);
            if (res2 == -1)
            {
                printf("����ʧ��\n");
            }
            else
            {
                printf("���ҳɹ���������һ���ڽӶ����ǣ�\n");
                printf(" %d %s\n", Graphs.elem[Graphs.cur_index].vertices[res2].data.key, Graphs.elem[Graphs.cur_index].vertices[res2].data.others);
            }

            break;
        }
        case 7:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ����붥��ʧ�ܣ�\n");
                break;
            }

            VertexType newVex;
            printf("������Ҫ����Ľ���key��others��\n");
            if (scanf("%d %s", &newVex.key, newVex.others) != 2)
            {
                printf("����������������룺\n");
                continue;
            }

            if (InsertVex(Graphs.elem[Graphs.cur_index], newVex) == OK)
            {
                printf("����ɹ���\n");
            }
            else
            {
                printf("����ʧ��\n");
            }

            break;
        }
        case 8:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�ɾ������ʧ�ܣ�\n");
                break;
            }

            int exist = 0;
            printf("������Ҫɾ���Ľ��Ĺؼ��֣�\n");
            if (scanf("%d", &exist) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (DeleteVex(Graphs.elem[Graphs.cur_index], exist) == OK)
            {
                printf("ɾ���ɹ���\n");
            }
            else
            {
                printf("ɾ��ʧ�ܡ�\n");
            }

            break;
        }
        case 9:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ������ʧ�ܣ�\n");
                break;
            }

            int v1, v2;
            printf("������Ҫ����Ļ�����������Ĺؼ��֣�\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("����������������룺\n");
                continue;
            }

            if (InsertArc(Graphs.elem[Graphs.cur_index], v1, v2) == OK)
            {
                printf("����ɹ���\n");
            }
            else
            {
                printf("����ʧ�ܡ�\n");
            }

            break;
        }
        case 10:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�ɾ����ʧ�ܣ�\n");
                break;
            }

            int v1, v2;
            v1 = v2 = 0;
            printf("������Ҫɾ���Ļ�����������Ĺؼ��֣�\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("����������������룺\n");
                continue;
            }

            if (DeleteArc(Graphs.elem[Graphs.cur_index], v1, v2) == OK)
            {
                printf("ɾ���ɹ���\n");
            }
            else
            {
                printf("ɾ��ʧ�ܡ�\n");
            }

            break;
        }
        case 11:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            printf("������ȱ��������\n");
            DFSTraverse(Graphs.elem[Graphs.cur_index]);
            printf("\n");

            break;
        }
        case 12:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            printf("������ȱ��������\n");
            BFSTraverse(Graphs.elem[Graphs.cur_index]);
            printf("\n");

            break;
        }
        case 13:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            printf("������Ҫ������ļ�����\n");
            if (scanf("%s", filename) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (SaveGraph(Graphs.elem[Graphs.cur_index], filename) == OK)
            {
                printf("����ɹ���\n");
            }
            else
            {
                printf("����ʧ�ܡ�\n");
            }

            break;
        }
        case 14:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum != 0)
            {
                printf("��ǰͼ�Ѵ��ڣ�����ʧ�ܣ�\n");
                break;
            }

            printf("������Ҫ���ص��ļ�����\n");
            if (scanf("%s", filename) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (LoadGraph(Graphs.elem[Graphs.cur_index], filename) == OK)
            {
                printf("���سɹ���\n");
            }
            else
            {
                printf("����ʧ�ܡ�\n");
            }

            break;
        }
        case 15:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            int v1, v2;
            v1 = v2 = 0;
            printf("������Ҫ���ҵĽ��Ĺؼ��֣�\n");
            if (scanf("%d %d", &v1, &v2) != 2)
            {
                printf("����������������룺\n");
                continue;
            }

            int res3 = ShortestPathLength(Graphs.elem[Graphs.cur_index], v1, v2);
            if (res3 == ERROR)
            {
                printf("����ʧ�ܣ���㲻���ڣ�\n");
            }
            else
            {
                printf("���ҳɹ������·�������ǣ�%d\n", res3);
            }

            break;
        }
        case 16:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            int v1 = 0;
            int k = 0;
            printf("������Ҫ���ҵĽ��Ĺؼ��֣�\n");
            if (scanf("%d", &v1) != 1)
            {
                printf("����������������룺\n");
                continue;
            }
            printf("������Ҫ���ҵľ��룺\n");
            if (scanf("%d", &k) != 1)
            {
                printf("����������������룺\n");
                continue;
            }
            printf("���ҽ����\n");
            VerticesWithinDistanceK(Graphs.elem[Graphs.cur_index], v1, k);
            printf("\n");

            break;
        }
        case 17:
        {
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ�����ʧ�ܣ�\n");
                break;
            }

            int num = ConnectedComponentsNum(Graphs.elem[Graphs.cur_index]);
            printf("��ͨ�����ĸ����ǣ�%d\n", num);

            break;
        }
        case 18:
        {
            if (Graphs.length >= MAX_GRAPH_SIZE)
            {
                printf("ͼ�������Ѵ����ޣ��޷������ͼ\n");
                break;
            }

            printf("������Ҫ��ӵ�ͼ�����ƣ�\n");
            if (scanf("%s", treename) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (AddGraph(Graphs, treename) == OK)
            {
                printf("��ӳɹ���\n");
            }
            else
            {
                printf("���ʧ�ܡ�\n");
            }

            break;
        }
        case 19:
        {
            printf("������Ҫɾ����ͼ�����ƣ�\n");
            if (scanf("%s", treename) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (RemoveGraph(Graphs, treename) == OK)
            {
                printf("ɾ���ɹ���\n");
            }

            break;
        }
        case 20:
        {
            printf("������Ҫ�л���ͼ�����ƣ�\n");
            if (scanf("%s", treename) != 1)
            {
                printf("����������������룺\n");
                continue;
            }

            if (SwitchGraph(Graphs, treename) == OK)
            {
                printf("�л��ɹ���\n");
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
            if (Graphs.elem[Graphs.cur_index].vexnum == 0)
            {
                printf("��ǰͼ�����ڣ���ӡʧ�ܣ�\n");
                break;
            }
            PrintGraphWithAdjList(Graphs.elem[Graphs.cur_index]);
            break;
        }
        case 0:
        {
            printf("�˳�ϵͳ��\n");
            break;
        }
        default:
            printf("����������������룺\n");
            break;
        }

        if (op != 0)
        {
            printf("���س�������...\n");
            getchar();
            getchar();
        }
    }

    return 0;
}
