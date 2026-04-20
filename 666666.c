#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>              
#include <stdlib.h>             
#include <string.h>            

// 跨平台清屏指令定义：Windows用cls，Linux/Mac用clear
#ifdef _WIN32
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

// 全局常量定义：各字符串最大长度
#define MAX_GOODS_NAME 30   // 商品名称最大长度
#define MAX_USER_NAME  20   // 用户名最大长度
#define MAX_PWD        20   // 密码最大长度

// 身份枚举：0-管理员，1-顾客
typedef enum { ADMIN = 0, CUSTOMER = 1 } Identity;

/* ===================== 数据结构定义 ===================== */
// 商品结构体：存储商品ID、名称、单价
typedef struct {
    int goods_id;                // 商品唯一编号
    char goods_name[MAX_GOODS_NAME]; // 商品名称
    float price;                 // 商品单价
} Goods;

// 用户结构体：存储用户名、密码、身份
typedef struct {
    char username[MAX_USER_NAME]; // 用户名
    char pwd[MAX_PWD];            // 密码
    Identity idt;                 // 用户身份（管理员/顾客）
} User;

// 购物车结构体：存储用户名、商品ID、购买数量、总金额
typedef struct {
    char username[MAX_USER_NAME]; // 所属用户
    int goods_id;                // 商品编号
    int num;                     // 购买数量
    float pay;                   // 小计金额
} Cart;

// 商品链表节点：商品数据 + 下一个节点指针
typedef struct GoodsNode {
    Goods goods;                 // 商品数据
    struct GoodsNode* next;      // 后继节点指针
} GoodsNode, * GoodsList;

// 用户链表节点：用户数据 + 下一个节点指针
typedef struct UserNode {
    User user;                   // 用户数据
    struct UserNode* next;       // 后继节点指针
} UserNode, * UserList;

// 购物车链表节点：购物车数据 + 下一个节点指针
typedef struct CartNode {
    Cart cart;                   // 购物车数据
    struct CartNode* next;       // 后继节点指针
} CartNode, * CartList;

/* ===================== 函数声明 ===================== */
// 工具函数
void ClearInput(void);           // 清空输入缓冲区
void Pause(void);                // 按回车暂停
void ClearScreen(void);          // 清屏

// 链表初始化函数
GoodsList InitGoodsList(void);   // 初始化商品链表
UserList InitUserList(void);     // 初始化用户链表
CartList InitCartList(void);     // 初始化购物车链表

// 内存释放函数
void FreeGoodsList(GoodsList L); // 释放商品链表内存
void FreeUserList(UserList L);   // 释放用户链表内存
void FreeCartList(CartList L);   // 释放购物车链表内存

// 统计数量函数
int CountGoods(GoodsList L);     // 统计商品总数
int CountUsers(UserList L);      // 统计用户总数
int CountCarts(CartList L);      // 统计购物车记录总数

// 文件操作函数
void SaveGoodsToFile(GoodsList L);    // 商品数据写入文件
void LoadGoodsFromFile(GoodsList L);  // 从文件加载商品
void SaveUsersToFile(UserList L);     // 用户数据写入文件
void LoadUsersFromFile(UserList L);   // 从文件加载用户
void SaveCartToFile(CartList L);      // 购物车数据写入文件
void LoadCartFromFile(CartList L);    // 从文件加载购物车
void SaveAll(GoodsList GL, UserList UL, CartList CL); // 保存所有数据

// 登录与管理员初始化
int AddDefaultAdmin(UserList L);      // 添加默认管理员
int Login(UserList L, char* u, char* p, Identity* idt); // 登录验证

// 查找函数
UserNode* FindUserByName(UserList L, const char* name);    // 按用户名查找用户
GoodsNode* FindGoodsByID(GoodsList L, int id);            // 按商品ID查找商品
int NextGoodsID(GoodsList L);                             // 生成下一个商品ID

// 链表尾插函数
void AppendGoodsNode(GoodsList L, Goods g);   // 尾插商品节点
void AppendUserNode(UserList L, User u);      // 尾插用户节点
void AppendCartNode(CartList L, Cart c);      // 尾插购物车节点

// 购物车删除辅助函数
void RemoveCartItemsByGoodsID(CartList L, int goods_id);        // 删除指定商品的所有购物车记录
void RemoveCartItemsByUsername(CartList L, const char* username); // 删除指定用户的所有购物车记录
void RemoveCartOneByUserAndGoodsID(CartList L, const char* username, int goods_id); // 删除单条购物车记录

// 商品信息获取函数
float GetGoodsPriceByID(GoodsList L, int id, int* found); // 按ID获取商品价格
char* GetGoodsNameByID(GoodsList L, int id);             // 按ID获取商品名称

// 购物车金额刷新函数
void RefreshAllCartPay(CartList CL, GoodsList GL);        // 刷新所有购物车金额
void RefreshCartPayByGoodsID(CartList CL, GoodsList GL, int goods_id); // 刷新指定商品购物车金额

// 打印表头函数
void PrintGoodsHeader(void);    // 打印商品列表表头
void PrintCartHeader(void);     // 打印购物车列表表头

// 商品查询函数
void PrintAllGoodsPage(GoodsList GL);    // 商品分页浏览
void SearchGoodsByName(GoodsList GL);    // 按商品名称查询
void SearchGoodsByID(GoodsList GL);      // 按商品ID查询

// 商品管理函数
void AddGoodsSingle(GoodsList GL);       // 单个添加商品
void AddGoodsBatch(GoodsList GL);        // 批量添加商品
void ModifyGoodsByID(GoodsList GL, CartList CL); // 修改商品信息
void DeleteGoodsByID(GoodsList GL, CartList CL); // 删除商品

// 用户管理函数
void AddCustomer(UserList UL);           // 添加顾客账号
void DeleteCustomer(UserList UL, CartList CL); // 删除顾客账号

// 购物车业务函数
void AddToCart(GoodsList GL, CartList CL, const char* username); // 添加商品到购物车
void RemoveFromCart(CartList CL, const char* username);          // 从购物车删除商品
void ViewMyCart(GoodsList GL, CartList CL, const char* username); // 查看个人购物车
void AdminSearchUserCart(GoodsList GL, CartList CL, const char* username); // 管理员查询用户购物车

// 排序函数
void SortGoodsArrayByPriceDesc(GoodsNode** arr, int n); // 商品按价格降序排序
void SortCartArrayByPayDesc(CartNode** arr, int n);     // 购物车按金额降序排序

// 统计函数
void GoodsTop10Stats(GoodsList GL);        // 商品价格TOP10统计
void CartTop10Stats(GoodsList GL, CartList CL); // 购物车消费TOP10统计

// 菜单函数
void AdminGoodsMenu(GoodsList GL, CartList CL);    // 管理员商品管理子菜单
void AdminUserMenu(UserList UL, CartList CL);      // 管理员用户管理子菜单
void AdminQueryMenu(GoodsList GL, CartList CL);    // 管理员查询子菜单
void AdminStatsMenu(GoodsList GL, CartList CL);     // 管理员统计子菜单
void AdminMenu(GoodsList GL, UserList UL, CartList CL); // 管理员主菜单
void CustomerMenu(GoodsList GL, CartList CL, const char* username); // 顾客菜单

/* ===================== 工具函数实现 ===================== */
// 清空输入缓冲区，解决scanf残留换行符问题
void ClearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 按回车继续，标准跨平台暂停函数
void Pause(void) {
    printf("\n按回车继续...");
    ClearInput();
    getchar();
}

// 跨平台清屏函数
void ClearScreen(void) {
    system(CLEAR_CMD);
}

/* ===================== 链表初始化 ===================== */
// 初始化商品链表，创建头节点
GoodsList InitGoodsList(void) {
    GoodsList L = (GoodsList)malloc(sizeof(GoodsNode));
    if (!L) exit(1); // 内存分配失败则退出
    L->next = NULL;
    return L;
}

// 初始化用户链表，创建头节点
UserList InitUserList(void) {
    UserList L = (UserList)malloc(sizeof(UserNode));
    if (!L) exit(1);
    L->next = NULL;
    return L;
}

// 初始化购物车链表，创建头节点
CartList InitCartList(void) {
    CartList L = (CartList)malloc(sizeof(CartNode));
    if (!L) exit(1);
    L->next = NULL;
    return L;
}

/* ===================== 内存释放 ===================== */
// 释放商品链表所有节点内存
void FreeGoodsList(GoodsList L) {
    GoodsNode* p = L->next;
    while (p) {
        GoodsNode* q = p;
        p = p->next;
        free(q);
    }
    free(L); // 释放头节点
}

// 释放用户链表所有节点内存
void FreeUserList(UserList L) {
    UserNode* p = L->next;
    while (p) {
        UserNode* q = p;
        p = p->next;
        free(q);
    }
    free(L);
}

// 释放购物车链表所有节点内存
void FreeCartList(CartList L) {
    CartNode* p = L->next;
    while (p) {
        CartNode* q = p;
        p = p->next;
        free(q);
    }
    free(L);
}

/* ===================== 统计数量 ===================== */
// 统计商品链表节点总数
int CountGoods(GoodsList L) {
    int n = 0;
    GoodsNode* p = L->next;
    while (p) {
        n++;
        p = p->next;
    }
    return n;
}

// 统计用户链表节点总数
int CountUsers(UserList L) {
    int n = 0;
    UserNode* p = L->next;
    while (p) {
        n++;
        p = p->next;
    }
    return n;
}

// 统计购物车链表节点总数
int CountCarts(CartList L) {
    int n = 0;
    CartNode* p = L->next;
    while (p) {
        n++;
        p = p->next;
    }
    return n;
}

/* ===================== 文件读写 ===================== */
// 将商品链表数据保存到goods.txt
void SaveGoodsToFile(GoodsList L) {
    FILE* fp = fopen("goods.txt", "w");
    GoodsNode* p;

    if (!fp) return; // 文件打开失败直接返回
    p = L->next;
    // 遍历链表写入文件
    while (p) {
        fprintf(fp, "%d %s %.2f\n", p->goods.goods_id, p->goods.goods_name, p->goods.price);
        p = p->next;
    }
    fclose(fp);
}

// 从goods.txt加载商品数据到链表
void LoadGoodsFromFile(GoodsList L) {
    FILE* fp = fopen("goods.txt", "r");
    Goods g;

    if (!fp) return;
    // 循环读取文件数据
    while (fscanf(fp, "%d %29s %f", &g.goods_id, g.goods_name, &g.price) == 3) {
        AppendGoodsNode(L, g); // 尾插节点
    }
    fclose(fp);
}

// 将用户链表数据保存到user.txt
void SaveUsersToFile(UserList L) {
    FILE* fp = fopen("user.txt", "w");
    UserNode* p;

    if (!fp) return;
    p = L->next;
    while (p) {
        fprintf(fp, "%s %s %d\n", p->user.username, p->user.pwd, p->user.idt);
        p = p->next;
    }
    fclose(fp);
}

// 从user.txt加载用户数据到链表
void LoadUsersFromFile(UserList L) {
    FILE* fp = fopen("user.txt", "r");
    User u;
    int idt;

    if (!fp) return;
    while (fscanf(fp, "%19s %19s %d", u.username, u.pwd, &idt) == 3) {
        u.idt = (Identity)idt;
        AppendUserNode(L, u);
    }
    fclose(fp);
}

// 将购物车链表数据保存到cart.txt
void SaveCartToFile(CartList L) {
    FILE* fp = fopen("cart.txt", "w");
    CartNode* p;

    if (!fp) return;
    p = L->next;
    while (p) {
        fprintf(fp, "%s %d %d %.2f\n", p->cart.username, p->cart.goods_id, p->cart.num, p->cart.pay);
        p = p->next;
    }
    fclose(fp);
}

// 从cart.txt加载购物车数据到链表
void LoadCartFromFile(CartList L) {
    FILE* fp = fopen("cart.txt", "r");
    Cart c;

    if (!fp) return;
    while (fscanf(fp, "%19s %d %d %f", c.username, &c.goods_id, &c.num, &c.pay) == 4) {
        AppendCartNode(L, c);
    }
    fclose(fp);
}

// 统一保存所有数据（商品+用户+购物车）
void SaveAll(GoodsList GL, UserList UL, CartList CL) {
    SaveGoodsToFile(GL);
    SaveUsersToFile(UL);
    SaveCartToFile(CL);
}

/* ===================== 链表尾插 ===================== */
// 商品链表尾插法添加节点
void AppendGoodsNode(GoodsList L, Goods g) {
    GoodsNode* node = (GoodsNode*)malloc(sizeof(GoodsNode));
    GoodsNode* p = L;
    if (!node) exit(1);

    node->goods = g;
    node->next = NULL;
    // 找到尾节点
    while (p->next) p = p->next;
    p->next = node;
}

// 用户链表尾插法添加节点
void AppendUserNode(UserList L, User u) {
    UserNode* node = (UserNode*)malloc(sizeof(UserNode));
    UserNode* p = L;
    if (!node) exit(1);

    node->user = u;
    node->next = NULL;
    while (p->next) p = p->next;
    p->next = node;
}

// 购物车链表尾插法添加节点
void AppendCartNode(CartList L, Cart c) {
    CartNode* node = (CartNode*)malloc(sizeof(CartNode));
    CartNode* p = L;
    if (!node) exit(1);

    node->cart = c;
    node->next = NULL;
    while (p->next) p = p->next;
    p->next = node;
}

/* ===================== 查找 ===================== */
// 按用户名查找用户节点，返回节点指针
UserNode* FindUserByName(UserList L, const char* name) {
    UserNode* p = L->next;
    while (p) {
        if (strcmp(p->user.username, name) == 0) return p;
        p = p->next;
    }
    return NULL; // 未找到返回NULL
}

// 按商品ID查找商品节点，返回节点指针
GoodsNode* FindGoodsByID(GoodsList L, int id) {
    GoodsNode* p = L->next;
    while (p) {
        if (p->goods.goods_id == id) return p;
        p = p->next;
    }
    return NULL;
}

// 生成自增商品ID（最大ID+1）
int NextGoodsID(GoodsList L) {
    int maxid = 0;
    GoodsNode* p = L->next;
    while (p) {
        if (p->goods.goods_id > maxid) maxid = p->goods.goods_id;
        p = p->next;
    }
    return maxid + 1;
}

// 按商品ID获取单价
float GetGoodsPriceByID(GoodsList L, int id, int* found) {
    GoodsNode* p = FindGoodsByID(L, id);
    if (p) {
        if (found) *found = 1;
        return p->goods.price;
    }
    if (found) *found = 0;
    return 0.0f;
}

// 按商品ID获取名称
char* GetGoodsNameByID(GoodsList L, int id) {
    GoodsNode* p = FindGoodsByID(L, id);
    if (p) return p->goods.goods_name;
    return NULL;
}

/* ===================== 删除购物车辅助 ===================== */
// 删除所有包含指定商品ID的购物车记录
void RemoveCartItemsByGoodsID(CartList L, int goods_id) {
    CartNode* prev = L;
    CartNode* cur = L->next;

    while (cur) {
        if (cur->cart.goods_id == goods_id) {
            prev->next = cur->next;
            free(cur); // 释放节点
            cur = prev->next;
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    }
}

// 删除指定用户的所有购物车记录
void RemoveCartItemsByUsername(CartList L, const char* username) {
    CartNode* prev = L;
    CartNode* cur = L->next;

    while (cur) {
        if (strcmp(cur->cart.username, username) == 0) {
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    }
}

// 删除指定用户+指定商品的单条购物车记录
void RemoveCartOneByUserAndGoodsID(CartList L, const char* username, int goods_id) {
    CartNode* prev = L;
    CartNode* cur = L->next;

    while (cur) {
        if (strcmp(cur->cart.username, username) == 0 && cur->cart.goods_id == goods_id) {
            prev->next = cur->next;
            free(cur);
            return; // 删除成功直接返回
        }
        prev = cur;
        cur = cur->next;
    }
}

/* ===================== 购物车金额刷新 ===================== */
// 刷新指定商品的购物车小计金额
void RefreshCartPayByGoodsID(CartList CL, GoodsList GL, int goods_id) {
    float price;
    int found = 0;
    CartNode* p = CL->next;

    price = GetGoodsPriceByID(GL, goods_id, &found);
    if (!found) return; // 商品不存在直接返回

    // 遍历更新金额
    while (p) {
        if (p->cart.goods_id == goods_id) {
            p->cart.pay = price * p->cart.num;
        }
        p = p->next;
    }
}

// 刷新所有购物车记录的小计金额
void RefreshAllCartPay(CartList CL, GoodsList GL) {
    CartNode* p = CL->next;
    while (p) {
        int found = 0;
        float price = GetGoodsPriceByID(GL, p->cart.goods_id, &found);
        if (found) {
            p->cart.pay = price * p->cart.num;
        }
        p = p->next;
    }
}

/* ===================== 登录与默认管理员 ===================== */
// 添加默认管理员admin，密码123456
int AddDefaultAdmin(UserList L) {
    UserNode* p = FindUserByName(L, "admin");
    User u;

    if (p) return 0; // 已存在则不添加

    strcpy(u.username, "admin");
    strcpy(u.pwd, "123456");
    u.idt = ADMIN;
    AppendUserNode(L, u);
    return 1;
}

// 登录验证函数：验证用户名密码，返回身份
int Login(UserList L, char* u, char* p, Identity* idt) {
    UserNode* q = L->next;
    while (q) {
        // 用户名密码匹配则登录成功
        if (strcmp(q->user.username, u) == 0 && strcmp(q->user.pwd, p) == 0) {
            *idt = q->user.idt;
            return 1;
        }
        q = q->next;
    }
    return 0; // 登录失败
}

/* ===================== 输出表头 ===================== */
// 打印商品列表表头
void PrintGoodsHeader(void) {
    printf("编号\t名称\t\t\t单价\n");
    printf("----------------------------------------------------\n");
}

// 打印购物车列表表头
void PrintCartHeader(void) {
    printf("用户名\t编号\t名称\t\t\t数量\t应付金额\n");
    printf("--------------------------------------------------------------------------\n");
}

/* ===================== 商品全体分页查询 ===================== */
// 商品分页浏览功能（10条/页）
void PrintAllGoodsPage(GoodsList GL) {
    int count, totalPages, page, start, end, i;
    int cmd;
    GoodsNode** arr;

    count = CountGoods(GL);
    if (count == 0) {
        ClearScreen();
        printf("当前没有商品数据。\n");
        Pause();
        return;
    }

    // 动态数组存储商品节点指针
    arr = (GoodsNode**)malloc(sizeof(GoodsNode*) * count);
    if (!arr) exit(1);

    i = 0;
    {
        GoodsNode* p = GL->next;
        while (p) {
            arr[i++] = p;
            p = p->next;
        }
    }

    page = 1;
    totalPages = (count + 9) / 10; // 计算总页数

    // 分页循环
    while (1) {
        ClearScreen();
        printf("商品列表（第 %d / %d 页）\n", page, totalPages);
        printf("====================================================\n");
        PrintGoodsHeader();

        start = (page - 1) * 10;
        end = start + 10;
        if (end > count) end = count;

        // 打印当前页商品
        for (i = start; i < end; i++) {
            printf("%d\t%-16s\t%.2f\n",
                arr[i]->goods.goods_id,
                arr[i]->goods.goods_name,
                arr[i]->goods.price);
        }

        printf("\nF-首页  P-上一页  N-下一页  L-最后一页  Q-退出\n");
        printf("请输入操作：");
        ClearInput();
        cmd = getchar();

        // 分页控制
        if (cmd == 'f' || cmd == 'F') {
            page = 1;
        }
        else if (cmd == 'p' || cmd == 'P') {
            if (page > 1) page--;
        }
        else if (cmd == 'n' || cmd == 'N') {
            if (page < totalPages) page++;
        }
        else if (cmd == 'l' || cmd == 'L') {
            page = totalPages;
        }
        else if (cmd == 'q' || cmd == 'Q') {
            break;
        }
    }

    free(arr); // 释放动态数组
}

/* ===================== 按名称查询商品 ===================== */
void SearchGoodsByName(GoodsList GL) {
    char key[MAX_GOODS_NAME];
    GoodsNode* p;
    int found = 0;

    ClearScreen();
    printf("商品名称查询\n");
    printf("请输入名称关键词：");
    scanf("%29s", key);

    printf("\n查询结果：\n");
    PrintGoodsHeader();

    // 模糊匹配商品名称
    p = GL->next;
    while (p) {
        if (strstr(p->goods.goods_name, key) != NULL) {
            printf("%d\t%-16s\t%.2f\n",
                p->goods.goods_id,
                p->goods.goods_name,
                p->goods.price);
            found = 1;
        }
        p = p->next;
    }

    if (!found) printf("未找到相关商品。\n");
    Pause();
}

/* ===================== 按编号查询商品 ===================== */
void SearchGoodsByID(GoodsList GL) {
    int id;
    GoodsNode* p;

    ClearScreen();
    printf("按编号查询商品\n");
    printf("请输入商品编号：");
    scanf("%d", &id);

    p = FindGoodsByID(GL, id);
    if (!p) {
        printf("未找到该商品。\n");
    }
    else {
        PrintGoodsHeader();
        printf("%d\t%-16s\t%.2f\n",
            p->goods.goods_id,
            p->goods.goods_name,
            p->goods.price);
    }
    Pause();
}

/* ===================== 添加商品 ===================== */
// 单个添加商品
void AddGoodsSingle(GoodsList GL) {
    Goods g;

    ClearScreen();
    printf("添加单个商品\n");
    g.goods_id = NextGoodsID(GL); // 自动生成ID
    printf("自动生成商品编号：%d\n", g.goods_id);
    printf("请输入商品名称：");
    scanf("%29s", g.goods_name);
    printf("请输入商品单价：");
    scanf("%f", &g.price);

    AppendGoodsNode(GL, g);
    SaveGoodsToFile(GL); // 保存文件

    printf("添加成功。\n");
    Pause();
}

// 批量添加商品
void AddGoodsBatch(GoodsList GL) {
    int n, i;
    Goods g;

    ClearScreen();
    printf("批量添加商品\n");
    printf("请输入添加数量：");
    scanf("%d", &n);

    if (n <= 0) {
        printf("数量输入错误。\n");
        Pause();
        return;
    }

    // 循环添加n个商品
    for (i = 0; i < n; i++) {
        g.goods_id = NextGoodsID(GL);
        printf("\n第 %d 个商品\n", i + 1);
        printf("商品编号：%d\n", g.goods_id);
        printf("商品名称：");
        scanf("%29s", g.goods_name);
        printf("商品单价：");
        scanf("%f", &g.price);
        AppendGoodsNode(GL, g);
    }

    SaveGoodsToFile(GL);
    printf("批量添加成功。\n");
    Pause();
}

/* ===================== 修改/删除商品 ===================== */
// 修改商品信息
void ModifyGoodsByID(GoodsList GL, CartList CL) {
    int id;
    GoodsNode* p;

    ClearScreen();
    printf("修改商品\n");
    printf("请输入要修改的商品编号：");
    scanf("%d", &id);

    p = FindGoodsByID(GL, id);
    if (!p) {
        printf("未找到该商品。\n");
        Pause();
        return;
    }

    // 输入新信息
    printf("原商品名称：%s\n", p->goods.goods_name);
    printf("原商品单价：%.2f\n", p->goods.price);
    printf("请输入新的商品名称：");
    scanf("%29s", p->goods.goods_name);
    printf("请输入新的商品单价：");
    scanf("%f", &p->goods.price);

    SaveGoodsToFile(GL);
    RefreshCartPayByGoodsID(CL, GL, id); // 刷新购物车金额
    SaveCartToFile(CL);

    printf("修改成功。\n");
    Pause();
}

// 删除商品，同时清理关联购物车记录
void DeleteGoodsByID(GoodsList GL, CartList CL) {
    int id;
    GoodsNode* prev, * cur;

    ClearScreen();
    printf("删除商品\n");
    printf("请输入要删除的商品编号：");
    scanf("%d", &id);

    // 查找商品节点
    prev = GL;
    cur = GL->next;
    while (cur && cur->goods.goods_id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        printf("未找到该商品。\n");
        Pause();
        return;
    }

    // 删除商品节点
    prev->next = cur->next;
    free(cur);

    RemoveCartItemsByGoodsID(CL, id); // 删除关联购物车

    SaveGoodsToFile(GL);
    SaveCartToFile(CL);

    printf("删除成功，关联购物车记录也已清理。\n");
    Pause();
}

/* ===================== 用户管理 ===================== */
// 添加顾客账号
void AddCustomer(UserList UL) {
    User u;
    UserNode* found;

    ClearScreen();
    printf("添加顾客账号\n");
    printf("请输入用户名：");
    scanf("%19s", u.username);

    found = FindUserByName(UL, u.username);
    if (found) {
        printf("该用户名已存在。\n");
        Pause();
        return;
    }

    printf("请输入密码：");
    scanf("%19s", u.pwd);
    u.idt = CUSTOMER;

    AppendUserNode(UL, u);
    SaveUsersToFile(UL);

    printf("顾客账号创建成功。\n");
    Pause();
}

// 删除顾客账号，同时清理购物车
void DeleteCustomer(UserList UL, CartList CL) {
    char name[MAX_USER_NAME];
    UserNode* prev, * cur;

    ClearScreen();
    printf("删除顾客账号\n");
    printf("请输入要删除的用户名：");
    scanf("%19s", name);

    // 禁止删除管理员
    if (strcmp(name, "admin") == 0) {
        printf("默认管理员不能删除。\n");
        Pause();
        return;
    }

    // 查找顾客节点
    prev = UL;
    cur = UL->next;
    while (cur) {
        if (strcmp(cur->user.username, name) == 0 && cur->user.idt == CUSTOMER) {
            prev->next = cur->next;
            free(cur);

            RemoveCartItemsByUsername(CL, name);
            SaveUsersToFile(UL);
            SaveCartToFile(CL);

            printf("删除成功，相关购物车记录也已清理。\n");
            Pause();
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("未找到该顾客账号。\n");
    Pause();
}

/* ===================== 购物车功能 ===================== */
// 添加商品到购物车
void AddToCart(GoodsList GL, CartList CL, const char* username) {
    int id, num;
    GoodsNode* p;
    Cart c;

    ClearScreen();
    printf("添加商品到购物车\n");
    printf("请输入商品编号：");
    scanf("%d", &id);
    printf("请输入购买数量：");
    scanf("%d", &num);

    if (num <= 0) {
        printf("购买数量必须大于 0。\n");
        Pause();
        return;
    }

    p = FindGoodsByID(GL, id);
    if (!p) {
        printf("该商品不存在。\n");
        Pause();
        return;
    }

    // 组装购物车数据
    strcpy(c.username, username);
    c.goods_id = id;
    c.num = num;
    c.pay = p->goods.price * num;

    AppendCartNode(CL, c);
    SaveCartToFile(CL);

    printf("添加成功。\n");
    Pause();
}

// 从购物车删除商品
void RemoveFromCart(CartList CL, const char* username) {
    int id;

    ClearScreen();
    printf("按商品编号移除购物车\n");
    printf("请输入要删除的商品编号：");
    scanf("%d", &id);

    RemoveCartOneByUserAndGoodsID(CL, username, id);
    SaveCartToFile(CL);

    printf("删除完成（如果该商品在你的购物车里存在，已删除第一条匹配记录）。\n");
    Pause();
}

// 查看个人购物车
void ViewMyCart(GoodsList GL, CartList CL, const char* username) {
    CartNode* p;
    float sum = 0.0f;
    int found = 0;

    ClearScreen();
    printf("我的购物车\n");
    PrintCartHeader();

    p = CL->next;
    while (p) {
        if (strcmp(p->cart.username, username) == 0) {
            char* gname = GetGoodsNameByID(GL, p->cart.goods_id);
            if (!gname) gname = "未知商品";
            printf("%-8s%d\t%-16s\t%d\t%.2f\n",
                p->cart.username,
                p->cart.goods_id,
                gname,
                p->cart.num,
                p->cart.pay);
            sum += p->cart.pay;
            found = 1;
        }
        p = p->next;
    }

    if (!found) {
        printf("当前购物车为空。\n");
    }
    else {
        printf("--------------------------------------------------------------------------\n");
        printf("购物车总金额：%.2f\n", sum);
    }
    Pause();
}

// 管理员查询指定用户购物车
void AdminSearchUserCart(GoodsList GL, CartList CL, const char* username) {
    CartNode* p;
    float sum = 0.0f;
    int found = 0;

    ClearScreen();
    printf("查询用户购物车：%s\n", username);
    PrintCartHeader();

    p = CL->next;
    while (p) {
        if (strcmp(p->cart.username, username) == 0) {
            char* gname = GetGoodsNameByID(GL, p->cart.goods_id);
            if (!gname) gname = "未知商品";
            printf("%-8s%d\t%-16s\t%d\t%.2f\n",
                p->cart.username,
                p->cart.goods_id,
                gname,
                p->cart.num,
                p->cart.pay);
            sum += p->cart.pay;
            found = 1;
        }
        p = p->next;
    }

    if (!found) {
        printf("该用户没有购物车数据。\n");
    }
    else {
        printf("--------------------------------------------------------------------------\n");
        printf("该用户购物车总金额：%.2f\n", sum);
    }
    Pause();
}

/* ===================== 排序 ===================== */
// 商品按价格降序排序（冒泡排序）
void SortGoodsArrayByPriceDesc(GoodsNode** arr, int n) {
    int i, j;
    GoodsNode* tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j]->goods.price < arr[j + 1]->goods.price) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// 购物车按消费金额降序排序
void SortCartArrayByPayDesc(CartNode** arr, int n) {
    int i, j;
    CartNode* tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j]->cart.pay < arr[j + 1]->cart.pay) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

/* ===================== 统计功能 ===================== */
// 商品价格TOP10统计
void GoodsTop10Stats(GoodsList GL) {
    int count, i, show;
    GoodsNode** arr;

    count = CountGoods(GL);
    ClearScreen();
    printf("商品统计（按单价从大到小，最多显示前10条）\n");
    printf("====================================================\n");

    if (count == 0) {
        printf("当前没有商品数据。\n");
        Pause();
        return;
    }

    arr = (GoodsNode**)malloc(sizeof(GoodsNode*) * count);
    if (!arr) exit(1);

    i = 0;
    {
        GoodsNode* p = GL->next;
        while (p) {
            arr[i++] = p;
            p = p->next;
        }
    }

    SortGoodsArrayByPriceDesc(arr, count); // 排序

    PrintGoodsHeader();
    show = (count < 10) ? count : 10;
    for (i = 0; i < show; i++) {
        printf("%d\t%-16s\t%.2f\n",
            arr[i]->goods.goods_id,
            arr[i]->goods.goods_name,
            arr[i]->goods.price);
    }

    free(arr);
    Pause();
}

// 购物车消费TOP10统计
void CartTop10Stats(GoodsList GL, CartList CL) {
    int count, i, show;
    CartNode** arr;

    count = CountCarts(CL);
    ClearScreen();
    printf("购物车价格统计（按应付金额从大到小，最多显示前10条）\n");
    printf("====================================================\n");

    if (count == 0) {
        printf("当前没有购物车数据。\n");
        Pause();
        return;
    }

    arr = (CartNode**)malloc(sizeof(CartNode*) * count);
    if (!arr) exit(1);

    i = 0;
    {
        CartNode* p = CL->next;
        while (p) {
            arr[i++] = p;
            p = p->next;
        }
    }

    SortCartArrayByPayDesc(arr, count);

    PrintCartHeader();
    show = (count < 10) ? count : 10;
    for (i = 0; i < show; i++) {
        char* gname = GetGoodsNameByID(GL, arr[i]->cart.goods_id);
        if (!gname) gname = "未知商品";
        printf("%-8s%d\t%-16s\t%d\t%.2f\n",
            arr[i]->cart.username,
            arr[i]->cart.goods_id,
            gname,
            arr[i]->cart.num,
            arr[i]->cart.pay);
    }

    free(arr);
    Pause();
}

/* ===================== 管理员菜单 ===================== */
// 管理员商品管理子菜单
void AdminGoodsMenu(GoodsList GL, CartList CL) {
    int c;

    while (1) {
        ClearScreen();
        printf("管理员 - 商品数据维护\n");
        printf("1. 添加单个商品\n");
        printf("2. 批量添加商品\n");
        printf("3. 修改商品\n");
        printf("4. 删除商品\n");
        printf("5. 根据商品编号查询商品\n");
        printf("6. 返回上级菜单\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) AddGoodsSingle(GL);
        else if (c == 2) AddGoodsBatch(GL);
        else if (c == 3) ModifyGoodsByID(GL, CL);
        else if (c == 4) DeleteGoodsByID(GL, CL);
        else if (c == 5) SearchGoodsByID(GL);
        else if (c == 6) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

// 管理员用户管理子菜单
void AdminUserMenu(UserList UL, CartList CL) {
    int c;

    while (1) {
        ClearScreen();
        printf("管理员 - 用户维护管理\n");
        printf("1. 添加顾客账号\n");
        printf("2. 删除顾客账号\n");
        printf("3. 返回上级菜单\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) AddCustomer(UL);
        else if (c == 2) DeleteCustomer(UL, CL);
        else if (c == 3) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

// 管理员查询子菜单
void AdminQueryMenu(GoodsList GL, CartList CL) {
    int c;
    char name[MAX_USER_NAME];

    while (1) {
        ClearScreen();
        printf("管理员 - 查询商品 / 购物车\n");
        printf("1. 全体查询商品（分页）\n");
        printf("2. 按商品名称查询\n");
        printf("3. 查询指定用户购物车\n");
        printf("4. 返回上级菜单\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) PrintAllGoodsPage(GL);
        else if (c == 2) SearchGoodsByName(GL);
        else if (c == 3) {
            ClearScreen();
            printf("请输入用户名：");
            scanf("%19s", name);
            AdminSearchUserCart(GL, CL, name);
        }
        else if (c == 4) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

// 管理员统计子菜单
void AdminStatsMenu(GoodsList GL, CartList CL) {
    int c;

    while (1) {
        ClearScreen();
        printf("管理员 - 统计功能\n");
        printf("1. 商品统计（按单价降序前10）\n");
        printf("2. 购物车价格统计（按金额降序前10）\n");
        printf("3. 返回上级菜单\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) GoodsTop10Stats(GL);
        else if (c == 2) CartTop10Stats(GL, CL);
        else if (c == 3) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

// 管理员主菜单
void AdminMenu(GoodsList GL, UserList UL, CartList CL) {
    int c;

    while (1) {
        ClearScreen();
        printf("========================================\n");
        printf("            管理员操作菜单             \n");
        printf("========================================\n");
        printf("1. 数据维护管理\n");
        printf("2. 用户维护管理\n");
        printf("3. 查询商品功能\n");
        printf("4. 统计功能\n");
        printf("5. 退出登录\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) AdminGoodsMenu(GL, CL);
        else if (c == 2) AdminUserMenu(UL, CL);
        else if (c == 3) AdminQueryMenu(GL, CL);
        else if (c == 4) AdminStatsMenu(GL, CL);
        else if (c == 5) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

/* ===================== 顾客菜单 ===================== */
// 顾客操作菜单
void CustomerMenu(GoodsList GL, CartList CL, const char* username) {
    int c;

    while (1) {
        ClearScreen();
        printf("========================================\n");
        printf("             顾客操作菜单              \n");
        printf("========================================\n");
        printf("1. 全体查询商品（分页）\n");
        printf("2. 按商品名称查询\n");
        printf("3. 添加商品到购物车\n");
        printf("4. 按商品编号移除购物车\n");
        printf("5. 查询当前购物车\n");
        printf("6. 退出登录\n");
        printf("请输入操作：");
        scanf("%d", &c);

        if (c == 1) PrintAllGoodsPage(GL);
        else if (c == 2) SearchGoodsByName(GL);
        else if (c == 3) AddToCart(GL, CL, username);
        else if (c == 4) RemoveFromCart(CL, username);
        else if (c == 5) ViewMyCart(GL, CL, username);
        else if (c == 6) break;
        else { printf("输入错误。\n"); Pause(); }
    }
}

/* ===================== 主函数 ===================== */
int main(void) {
    // 初始化三大链表
    GoodsList GL = InitGoodsList();
    UserList UL = InitUserList();
    CartList CL = InitCartList();

    char u[MAX_USER_NAME];  // 登录用户名
    char p[MAX_PWD];        // 登录密码
    Identity idt;           // 用户身份

    // 从文件加载所有数据
    LoadGoodsFromFile(GL);
    LoadUsersFromFile(UL);
    LoadCartFromFile(CL);

    // 添加默认管理员（不存在则添加）
    if (AddDefaultAdmin(UL)) {
        SaveUsersToFile(UL);
    }

    RefreshAllCartPay(CL, GL); // 刷新购物车金额

    // 主登录循环
    while (1) {
        ClearScreen();
        printf("========================================\n");
        printf("          商城管理系统登录页           \n");
        printf("========================================\n");
        printf("用户名：");
        scanf("%19s", u);
        printf("密码：");
        scanf("%19s", p);

        // 登录验证
        if (Login(UL, u, p, &idt)) {
            printf("登录成功！\n");
            Pause();

            // 根据身份进入对应菜单
            if (idt == ADMIN) {
                AdminMenu(GL, UL, CL);
            }
            else {
                CustomerMenu(GL, CL, u);
            }

            SaveAll(GL, UL, CL); // 退出菜单保存所有数据
        }
        else {
            printf("登录失败，用户名或密码错误。\n");
            Pause();
        }
    }

    // 释放内存
    FreeGoodsList(GL);
    FreeUserList(UL);
    FreeCartList(CL);
    return 0;
}