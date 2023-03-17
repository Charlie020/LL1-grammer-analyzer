#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <stack>
using namespace std;
const int N = 110;

char start;                           // 起始符
map<string, vector<string>> pro;      // 每个终结符所对应的产生式集
map<string, set<string>> FIRST, FOLLOW;
map<pair<char, char>, string> M;      // M[A, a]=A->α
set<string> alpha, Vn, Vt;            // 字符表，非终结符，终结符
stack<char> stk, stk_tmp;             // 分析栈

bool isOperator(char ch) {
    if (ch == '+' || ch == '-'
        || ch == '*' || ch == '/'
        || ch == '(' || ch == ')') return true;
    return false;
}

bool check(string vn, string pstr) {    // 判断是否是非终结符开头
    for (int i = 0; i < vn.size(); i++) {
        if (vn[i] != pstr[i]) return false;
    }
    return true;
}

void disp(bool exist) {
    cout << endl << "Vn: ";
    for (auto it : Vn) cout << it << ' ';
    cout << endl << "Vt: ";
    for (auto it : Vt) cout << it << ' ';
    cout << endl;
    if (exist) cout << "存在左递归，消除后如下：" << endl;
    for (auto it : pro) {
        for (auto i : it.second) {
            cout << it.first << "->" << i << endl;
        }
    }
}

void Sol_First() {     // 求FIRST集
    for (auto vt : Vt) FIRST[vt].insert(vt);     // 规则一
    while (true) {
        bool change = false;
        for (auto vn : Vn) {
            int siz = FIRST[vn].size();
            for (auto istr : pro[vn]) {
                string tmp;
                tmp = istr[0];
                if (Vt.count(tmp)) FIRST[vn].insert(tmp);    // 规则二
                else if (Vn.count(tmp)) {                    // 规则三
                    for (auto i : FIRST[tmp]) {
                        if (i != "e") FIRST[vn].insert(i);
                    }
                }
            }
            if (siz != FIRST[vn].size()) change = true;
        }
        if (!change) break;
    }

    cout << endl << "FIRST集:" << endl;
    for (auto vn : Vn) {
        cout << "FIRST( " << vn << " ): ";
        for (auto i : FIRST[vn]) cout << i << ' ';
        cout << endl;
    }
    for (auto vt : Vt) {
        cout << "FIRST( " << vt << " ): ";
        for (auto i : FIRST[vt]) cout << i << ' ';
        cout << endl;
    }
}

void Sol_Follow() {       // 求FOLLOW集
    while (true) {
        bool change = false;
        for (auto vn : Vn) {
            for (auto p : pro[vn]) {
                for (int i = 0; i < p.size(); ++i) {
                    string tmp;
                    tmp = p[i];
                    if (Vn.count(tmp)) {
                        ++i;
                        int siz = FOLLOW[tmp].size();
                        if (i < p.size()) {  //规则二
                            string fir;
                            fir = p[i];
                            for (auto f : FIRST[fir]) {
                                if (f != "e") FOLLOW[tmp].insert(f);
                            }
                        } else if (tmp != vn) {       // 规则三
                            for (auto i : FOLLOW[vn]) FOLLOW[tmp].insert(i);
                        } else if (tmp == vn) FOLLOW[tmp].insert("#");
                        if (siz != FOLLOW[tmp].size()) change = true;
                        --i;
                    }
                }
                int i = p.size() - 1;
                while (true) {
                    string tmp;
                    tmp = p[i];
                    if (Vt.count(tmp)) break;        // 规则三：广义推导出空集
                    if (find(pro[tmp].begin(), pro[tmp].end(), "e") != pro[tmp].end()) { 
                        int j = i - 1;
                        if (j >= 0) {
                            string t;
                            t = p[j];
                            for (auto i : FOLLOW[vn]) FOLLOW[t].insert(i);
                        }
                    } else break;
                    --i;
                }
            }
        }
        if (!change) break;
    }
    cout << endl << "FOLLOW集:" << endl;
    for (auto vn : Vn) {
        cout << "FOLLOW( " << vn << " ): ";
        for (auto i : FOLLOW[vn]) cout << i << ' ';
        cout << endl;
    }
    cout << endl;
}

void Build_LL1() {
    for (auto vn : Vn) {
        for (auto p : pro[vn]) {
            bool flag = false;
            string tmp;
            tmp = p[0];
            for (auto i : FIRST[tmp]) {  // 构造LL(1)的规则一
                if (i == "e") flag = true;
                else M[{vn[0], i[0]}] = p;
            }
            if (flag) {   // 构造LL(1)的规则二
                for (auto i : FOLLOW[vn]) {
                    M[{vn[0], i[0]}] = p;
                }
            }
        }
    }
    for (auto it : M) {
       cout << "M[" << it.first.first << ',' << it.first.second << "] = ";
       cout << it.first.first << "->" << it.second << endl;
    }
    cout << endl;
}

void show(string in, string exc, int turn) {
    string show_exc;
    if (exc == "Success") {
        show_exc = exc;
    } else if (exc == "Error") {
        show_exc = exc;
    } else if (exc != " ") {
        show_exc = stk.top();
        show_exc += "->";
        show_exc += exc;
    }

    cout << turn << '\t';
    while (stk.size()) {
        stk_tmp.push(stk.top());
        stk.pop();
    }
    while (stk_tmp.size()) {
        cout << stk_tmp.top();
        stk.push(stk_tmp.top());
        stk_tmp.pop();
    }
    cout << '\t' << in << '\t';
    cout << show_exc << endl;
}

void Sol_LL1(string in) {
    stk.push('#');
    stk.push(start);               // 分析栈初始状态
    int turn = 0;                  // 记录步骤数
    while (true && ++turn) {
        if (stk.top() == '#' && in.front() == '#') {        // 终止
            show(in, "Success", turn);                      // 打印过程
            break;
        }
        if (stk.top() == in[0]) {  // 消除
            show(in, " ", turn);
            stk.pop();
            in.erase(in.begin());
            continue;
        }
        if (M.find({ stk.top(), in.front() }) == M.end()) { // 检测到错误
            show(in, "Error", turn);
            cout << "Detect error!" << endl;
            exit(0);
        }

        string exc = M[{stk.top(), in.front()}];            // 所用产生式
        show(in, exc, turn); 

        stk.pop();
        for (int i = exc.size() - 1; i >= 0; --i)           // 倒序入栈
            if (exc[i] != 'e') stk.push(exc[i]);
    }
}

int main() {
    cout << "Input the grammer : " << endl;
    bool startflag = true;
    string production;
    while (getline(cin, production)) {    // 处理终结符和非终结符，以及每个终结符对应的产生式
        while (production.back() == ' ') production.pop_back();
        int idx = 0;
        string vn;
        while (production[idx] != '-') {  // "->"的左部
            if (production[idx] != ' ') vn += production[idx];
            ++idx;
        }
        Vn.insert(vn), alpha.insert(vn);
        if (startflag) FOLLOW[vn].insert("#"), start = vn[0], startflag = false;  // 求FOLLOW集的规则一
        while (production[idx] != '>') ++idx;
        idx++;

        string str, alp;                        // str存放产生式右部，alp存放每个字符
        for (idx; idx < production.size(); ++idx) {
            if (production[idx] == ' ') continue;          // 过滤空格
            if (production[idx] == '|') {       // 遇到'|'则记录产生式
                if (str.size()) pro[vn].push_back(str), str = "";
                if (alp.size()) alpha.insert(alp), alp = "";
                continue;
            }
            str += production[idx];

            if (isOperator(production[idx])) {  // 如果是运算符就将之前的符号和运算符都加入符号表中
                if (alp.size()) alpha.insert(alp), alp = "";
                alp += production[idx], alpha.insert(alp), alp = "";
            }
            else alp += production[idx];

            if (idx == production.size() - 1) {
                if (str.size()) pro[vn].push_back(str), str = "";;
                if (alp.size()) alpha.insert(alp), alp = "";
            }
        }
    }

    for (auto it : alpha) {     // 处理终结符
        if (!Vn.count(it)) Vt.insert(it);
    }
    cin.clear();
    cout << "Input the sentence : ";
    string str;
    cin >> str;

    // 判断并消除直接左递归
    bool exist = false;
    for (auto vn : Vn) {        // 每个非终结符
        bool flag = false;
        string str, subpstr;
        for (auto it = pro[vn].begin(); it != pro[vn].end(); ) {
            if (check(vn, *it)) {                  // 判断是否有左递归
                exist = flag = true;
                subpstr = (*it).substr(vn.size()); // 记录左递归后面的内容
                it = pro[vn].erase(it);
                break;
            }
            else ++it;
        }
        if (flag) {
            for (auto it = pro[vn].begin(); it != pro[vn].end(); ) {
                if (!check(vn, *it)) {
                    str = *it;                     // 记录一个不是左递归的产生式右部
                    it = pro[vn].erase(it);
                    break;
                }
                else ++it;
            }
            string newvn = vn;
            for (char ch = 'A'; ch <= 'Z'; ++ch) {
                string tmp;
                tmp = ch;
                if (!Vn.count(tmp)) {
                    newvn = ch;
                    Vn.insert(newvn);
                    break;
                }
            }
            pro[vn].push_back(str + newvn);
            pro[newvn].push_back(subpstr + newvn);
            Vt.insert("e");                      // 空集
            pro[newvn].push_back("e");
        }
    }
    disp(exist);

    Sol_First();      // 求FIRST集
    Sol_Follow();     // 求FOLLOW集

    Build_LL1();      // 构建LL(1)文法分析表

    for (auto it = str.begin(); it != str.end();) {  // 处理空格
        if (*it == ' ') it = str.erase(it);
        else ++it;
    }
    string str_tmp;
    for (auto it : str) {         // 将句子转化为分析栈能够处理的格式
        if ('0' <= it && it <= '9') {
            if (!str_tmp.size()) str_tmp.push_back('i');
            else if (str_tmp.back() == 'i') continue;
            else str_tmp.push_back('i');
        } else str_tmp.push_back(it);
    }
    str = str_tmp;
    cout << "Format : " << str << endl;
    Sol_LL1(str + "#");
    return 0;
}