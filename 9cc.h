//
// Created by 服部篤樹 on 2019/08/18.
//

#ifndef INC_9CC_9CC_H
#define INC_9CC_9CC_H


// トークンの種類
typedef enum {
    TK_RESERVED, // 記号
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
// 1 -> + -> 10 -> - -> 5 -> ...
// のように繋がっていく
struct Token {
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークン文字列
    int len;        // トークンの長さ
};
// 抽象構文木のノードの種類
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_NUM, // 整数
    ND_EQ, // ==
    ND_NE, // !=
    ND_LT, // <     >も兼用
    ND_LE, // <=    >=も兼用
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind; // ノードの型
    Node *lhs;     // 左辺
    Node *rhs;     // 右辺
    int val;       // kindがND_NUMの場合のみ使う
};

// parse.c
extern Token *tokenize(char *p);

// codegen.c
extern void gen(Node *node);

// container.c
extern void error(char *fmt, ...);
extern void error_at(char *loc, char *fmt, ...);
extern Node *expr();

// Global variables
extern Token *token;    // 現在着目しているトークン
extern char *user_input;    // 入力プログラム

#endif //INC_9CC_9CC_H