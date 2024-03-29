//
// Created by 服部篤樹 on 2019/08/18.
//
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

//  与えられた文字がトークンを構成する文字、すなわち英数字かアンダースコアかどうかを判定する関数
int is_alnum(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           (c == '_');
}

//  与えられた文字がトークンを構成する文字、すなわち英字かアンダースコアかどうかを判定する関数（変数の頭文字の判定）
int is_alnum_init(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           (c == '_');
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    // curの後ろに次のトークンを繋げる
    cur->next = tok;
    // 次のトークンをcurとして、さらにその後ろに新たなトークンをつなげていく
    return tok;
}

// 入力文字列pをトークナイズしてそれを返すで
// Tokenのポインタを返す関数
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        //  returnを判定
        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])){
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        // 2文字演算子
        if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 || strncmp(p, "<=", 2) == 0 ||
            strncmp(p, ">=", 2) == 0) {

            cur = new_token(TK_RESERVED, cur, p++, 2);
            p++;    // ２文字読み込んだので、２回インクリメント
            continue;
        }

        // 1文字演算子
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>' || *p == ';' || *p == '=') {
            // インクリメント演算子を変数の後に付けた場合、変数の値を評価してから、変数の値に1を加えます。
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        //  変数名（a -> z）
        if (is_alnum_init(*p)) {    //  変数の最初は数字が使えない
            char *init_p = p;     //  変数名
            p++;
            while(is_alnum(*p)){    //  is_alnum()で変数に使える文字を定義
                p++;
            }
            int len = p - init_p;
            char *var = NULL;
            var = (char*)malloc(len+1); //  変数の長さは実行中に決まるので動的割付
            strncpy(var, init_p, len);
            cur = new_token(TK_IDENT, cur, var, len);
            //free(var);    //  解放してはいけない、ここに変数名が格納され、そのポインタを渡しているので、ここを解放すると未定義
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 1);
            cur->val = strtol(p, &p, 10);   // strtol()は第2引数のポインタをアップデートして、読み込んだ最後の文字の次の文字を指すように値を更新
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 1);
    // 最後にheadの頭をなくすとまとまって繋がった構造が得られる
    return head.next;
}
