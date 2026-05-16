#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <conio.h>
#include <cstdlib>
using namespace std;

// ========== ESC 检测与安全输入工具 ==========

bool escFlag = false; // 全局ESC标志

// 在循环中调用，检测ESC按键
bool checkEsc() {
    if (_kbhit()) {
        if (_getch() == 27) {
            escFlag = true;
            cout << "\n[ESC] 返回菜单...\n";
            return true;
        }
    }
    return false;
}

// 安全读取整数，支持ESC退出
bool inputInt(int& val, const string& prompt = "") {
    if (escFlag) return false;
    if (!prompt.empty()) cout << prompt;
    string buf;
    while (!escFlag) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) {
                escFlag = true;
                cout << "\n[ESC] 返回菜单...\n";
                return false;
            }
            if (ch == '\r') {
                if (!buf.empty()) {
                    cout << endl;
                    val = atoi(buf.c_str());
                    return true;
                }
            }
            else if (ch == '\b') {
                if (!buf.empty()) {
                    buf.pop_back();
                    cout << "\b \b";
                }
            }
            else if ((ch >= '0' && ch <= '9') || (ch == '-' && buf.empty())) {
                buf += (char)ch;
                cout << (char)ch;
            }
        }
    }
    return false;
}

// 安全读取字符，支持ESC退出
bool inputChar(char& val, const string& prompt = "") {
    if (escFlag) return false;
    if (!prompt.empty()) cout << prompt;
    while (!escFlag) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) {
                escFlag = true;
                cout << "\n[ESC] 返回菜单...\n";
                return false;
            }
            if (ch != '\r') {
                val = (char)ch;
                cout << val << endl;
                return true;
            }
        }
    }
    return false;
}


// ========== 实验一(1): 求矩阵中的鞍点 ==========
void findSaddlePoint() {
    cout << "\n========== 求矩阵中的鞍点 ==========\n";
    escFlag = false;

    int rows, cols;
    cout << "请输入矩阵的行数和列数: ";
    if (!inputInt(rows)) return;
    if (!inputInt(cols)) return;

    vector<vector<int>> matrix(rows, vector<int>(cols));
    cout << "请输入矩阵元素 (" << rows << "x" << cols << "):\n";
    for (int i = 0; i < rows && !escFlag; i++) {
        for (int j = 0; j < cols && !escFlag; j++) {
            cout << "  matrix[" << i << "][" << j << "] = ";
            if (!inputInt(matrix[i][j])) return;
        }
    }
    if (escFlag) return;

    cout << "\n矩阵为:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }

    bool found = false;
    for (int i = 0; i < rows && !escFlag; i++) {
        int maxInRow = matrix[i][0];
        int colOfMax = 0;
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] > maxInRow) {
                maxInRow = matrix[i][j];
                colOfMax = j;
            }
        }

        bool isMinInCol = true;
        for (int k = 0; k < rows; k++) {
            if (matrix[k][colOfMax] < maxInRow) {
                isMinInCol = false;
                break;
            }
        }

        if (isMinInCol) {
            cout << "找到鞍点: matrix[" << i << "][" << colOfMax << "] = " << maxInRow << endl;
            found = true;
        }
    }

    if (!found && !escFlag) {
        cout << "该矩阵没有鞍点。\n";
    }
    cout << endl;
}


// ========== 实验一(2): 模拟停车场程序 ==========
struct Car {
    int plate;
    int time;
    Car(int p = 0, int t = 0) : plate(p), time(t) {}
};

void parkingLotSimulation() {
    cout << "\n========== 模拟停车场程序 ==========\n";
    escFlag = false;

    int N;
    cout << "请输入停车场容量 N: ";
    if (!inputInt(N)) return;

    stack<Car> parking;
    queue<Car> sidewalk;

    cout << "请输入操作 (A 车牌 时刻 / D 车牌 时刻 / E 0 0 结束):\n";
    cout << "  例如: A 1 5  表示1号车在5时刻到达\n";
    cout << "        D 1 20 表示1号车在20时刻离开\n";
    cout << "        E 0 0  表示结束\n";
    cout << "  按 ESC 可随时返回菜单\n";

    char flag;
    int plate, time;
    double totalFee = 0;

    while (!escFlag) {
        cout << "> ";
        if (checkEsc()) return;

        if (!inputChar(flag)) return;
        if (escFlag) return;

        if (flag == 'E') {
            int zero1, zero2;
            cout << "0 0" << endl;
            break;
        }

        if (!inputInt(plate)) return;
        if (!inputInt(time)) return;

        if (flag == 'A') {
            if ((int)parking.size() < N) {
                parking.push(Car(plate, time));
                cout << "  车辆 " << plate << " 进入停车场 (时刻 " << time << ")\n";
            }
            else {
                sidewalk.push(Car(plate, time));
                cout << "  停车场已满，车辆 " << plate << " 在便道等候 (时刻 " << time << ")\n";
            }
        }
        else if (flag == 'D') {
            stack<Car> temp;
            bool found = false;

            while (!parking.empty()) {
                Car top = parking.top();
                parking.pop();
                if (top.plate == plate) {
                    double fee = (time - top.time) * 0.5;
                    totalFee += fee;
                    cout << "  车辆 " << plate << " 离开停车场，停车费: " << fee
                        << " 元 (停留 " << (time - top.time) << " 单位时间)\n";
                    found = true;
                    break;
                }
                else {
                    temp.push(top);
                }
            }

            while (!temp.empty()) {
                parking.push(temp.top());
                temp.pop();
            }

            if (!found) {
                cout << "  错误: 车辆 " << plate << " 不在停车场内！\n";
            }
            else {
                if (!sidewalk.empty()) {
                    Car next = sidewalk.front();
                    sidewalk.pop();
                    next.time = time;
                    parking.push(next);
                    cout << "  便道上车辆 " << next.plate << " 进入停车场 (时刻 " << time << ")\n";
                }
            }
        }
        else {
            cout << "  无效操作!\n";
        }

        if (checkEsc()) return;
    }

    cout << "\n模拟结束。总停车费收入: " << totalFee << " 元\n\n";
}


// ========== 实验一(3): Josephus问题 ==========
struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

void josephusProblem() {
    cout << "\n========== Josephus问题 ==========\n";
    escFlag = false;

    int n, m, k;
    cout << "请输入总人数 n: ";
    if (!inputInt(n)) return;
    cout << "请输入报数值 m: ";
    if (!inputInt(m)) return;
    cout << "请输入起始编号 k: ";
    if (!inputInt(k)) return;

    if (n <= 0 || m <= 0 || k < 1 || k > n) {
        cout << "输入参数无效!\n";
        return;
    }

    // 构建不带头结点的单循环链表
    Node* head = new Node(1);
    Node* prev = head;
    for (int i = 2; i <= n; i++) {
        Node* node = new Node(i);
        prev->next = node;
        prev = node;
    }
    prev->next = head;

    // 定位起始结点
    Node* current = head;
    Node* before = prev;
    for (int i = 1; i < k; i++) {
        before = current;
        current = current->next;
    }

    cout << "出列序列: ";
    while (n > 0 && !escFlag) {
        for (int i = 1; i < m; i++) {
            before = current;
            current = current->next;
        }

        cout << current->data;
        if (n > 1) cout << ", ";

        before->next = current->next;
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
        n--;

        if (checkEsc()) return;
    }
    if (!escFlag) cout << "\n";
    cout << endl;
}


// ========== 文件读取功能 ==========

void findSaddlePointFromFile() {
    cout << "\n========== 从文件读取：求矩阵中的鞍点 ==========\n";
    escFlag = false;

    string filename;
    cout << "请输入文件名: ";
    if (!escFlag) {
        // 文件名输入使用 cin 即可（简单场景）
        cin >> filename;
    }
    if (escFlag) return;

    ifstream fin(filename);
    if (!fin) {
        cout << "无法打开文件: " << filename << "\n\n";
        return;
    }

    int rows, cols;
    fin >> rows >> cols;
    if (fin.fail()) {
        cout << "文件格式错误！\n\n";
        return;
    }

    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> matrix[i][j];
        }
    }
    fin.close();

    cout << "矩阵为 (" << rows << "x" << cols << "):\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }

    bool found = false;
    for (int i = 0; i < rows && !escFlag; i++) {
        int maxInRow = matrix[i][0];
        int colOfMax = 0;
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] > maxInRow) {
                maxInRow = matrix[i][j];
                colOfMax = j;
            }
        }

        bool isMinInCol = true;
        for (int k = 0; k < rows; k++) {
            if (matrix[k][colOfMax] < maxInRow) {
                isMinInCol = false;
                break;
            }
        }

        if (isMinInCol) {
            cout << "找到鞍点: matrix[" << i << "][" << colOfMax << "] = " << maxInRow << endl;
            found = true;
        }
        if (checkEsc()) return;
    }

    if (!found && !escFlag) {
        cout << "该矩阵没有鞍点。\n";
    }
    cout << endl;
}

void parkingLotSimulationFromFile() {
    cout << "\n========== 从文件读取：模拟停车场程序 ==========\n";
    escFlag = false;

    string filename;
    cout << "请输入文件名: ";
    if (!escFlag) cin >> filename;

    ifstream fin(filename);
    if (!fin) {
        cout << "无法打开文件: " << filename << "\n\n";
        return;
    }

    int N;
    fin >> N;
    cout << "停车场容量: " << N << endl;

    stack<Car> parking;
    queue<Car> sidewalk;
    double totalFee = 0;

    char flag;
    int plate, time;
    while (fin >> flag >> plate >> time && !escFlag) {
        cout << "> " << flag << " " << plate << " " << time << endl;

        if (flag == 'E' && plate == 0 && time == 0) break;

        if (flag == 'A') {
            if ((int)parking.size() < N) {
                parking.push(Car(plate, time));
                cout << "  车辆 " << plate << " 进入停车场 (时刻 " << time << ")\n";
            }
            else {
                sidewalk.push(Car(plate, time));
                cout << "  停车场已满，车辆 " << plate << " 在便道等候 (时刻 " << time << ")\n";
            }
        }
        else if (flag == 'D') {
            stack<Car> temp;
            bool found = false;

            while (!parking.empty()) {
                Car top = parking.top();
                parking.pop();
                if (top.plate == plate) {
                    double fee = (time - top.time) * 0.5;
                    totalFee += fee;
                    cout << "  车辆 " << plate << " 离开停车场，停车费: " << fee
                        << " 元 (停留 " << (time - top.time) << " 单位时间)\n";
                    found = true;
                    break;
                }
                else {
                    temp.push(top);
                }
            }

            while (!temp.empty()) {
                parking.push(temp.top());
                temp.pop();
            }

            if (!found) {
                cout << "  错误: 车辆 " << plate << " 不在停车场内！\n";
            }
            else {
                if (!sidewalk.empty()) {
                    Car next = sidewalk.front();
                    sidewalk.pop();
                    next.time = time;
                    parking.push(next);
                    cout << "  便道上车辆 " << next.plate << " 进入停车场 (时刻 " << time << ")\n";
                }
            }
        }
        if (checkEsc()) return;
    }
    fin.close();

    cout << "\n模拟结束。总停车费收入: " << totalFee << " 元\n\n";
}

void josephusProblemFromFile() {
    cout << "\n========== 从文件读取：Josephus问题 ==========\n";
    escFlag = false;

    string filename;
    cout << "请输入文件名: ";
    if (!escFlag) cin >> filename;

    ifstream fin(filename);
    if (!fin) {
        cout << "无法打开文件: " << filename << "\n\n";
        return;
    }

    int n, m, k;
    fin >> n >> m >> k;
    fin.close();

    cout << "n = " << n << ", m = " << m << ", k = " << k << endl;

    if (n <= 0 || m <= 0 || k < 1 || k > n) {
        cout << "输入参数无效!\n";
        return;
    }

    Node* head = new Node(1);
    Node* prev = head;
    for (int i = 2; i <= n; i++) {
        Node* node = new Node(i);
        prev->next = node;
        prev = node;
    }
    prev->next = head;

    Node* current = head;
    Node* before = prev;
    for (int i = 1; i < k; i++) {
        before = current;
        current = current->next;
    }

    cout << "出列序列: ";
    while (n > 0 && !escFlag) {
        for (int i = 1; i < m; i++) {
            before = current;
            current = current->next;
        }

        cout << current->data;
        if (n > 1) cout << ", ";

        before->next = current->next;
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
        n--;

        if (checkEsc()) return;
    }
    if (!escFlag) cout << "\n";
    cout << endl;
}


// ========== 主菜单 ==========
int main() {
    int choice;
    while (true) {
        escFlag = false;
        cout << "========================================\n";
        cout << "   《数据结构》实验一：线性表应用实验\n";
        cout << "========================================\n";
        cout << "   1. 求矩阵中的鞍点 (手动输入)\n";
        cout << "   2. 模拟停车场程序 (手动输入)\n";
        cout << "   3. Josephus问题 (手动输入)\n";
        cout << "   4. 求矩阵中的鞍点 (从文件读取)\n";
        cout << "   5. 模拟停车场程序 (从文件读取)\n";
        cout << "   6. Josephus问题 (从文件读取)\n";
        cout << "   0. 退出\n";
        cout << "========================================\n";
        cout << "  提示: 在任何子功能中按 ESC 可返回菜单\n";
        cout << "========================================\n";
        cout << "请选择: ";

        if (!inputInt(choice)) {
            escFlag = false;
            continue;
        }

        switch (choice) {
        case 1:
            findSaddlePoint();
            break;
        case 2:
            parkingLotSimulation();
            break;
        case 3:
            josephusProblem();
            break;
        case 4:
            findSaddlePointFromFile();
            break;
        case 5:
            parkingLotSimulationFromFile();
            break;
        case 6:
            josephusProblemFromFile();
            break;
        case 0:
            cout << "程序结束。\n";
            return 0;
        default:
            cout << "无效选择，请重新输入。\n";
        }
    }
    return 0;
}
