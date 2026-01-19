#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

struct Scholar {
    string name;
    string dynasty;
    string contribution;
    string works;
    Scholar() = default;
    Scholar(string n, string d, string c, string w)
        : name(n), dynasty(d), contribution(c), works(w) {
    }
};

struct Book {
    string title;
    string category;
    string author;
    string intro;
    int dynastyYear;
    Book() = default;
    Book(string t, string c, string a, string i, int y)
        : title(t), category(c), author(a), intro(i), dynastyYear(y) {
    }
};

struct Question {
    string content;
    vector<string> options;
    char answer;
    string analysis;
    Question() = default;
    Question(string c, vector<string> o, char a, string an)
        : content(c), options(o), answer(a), analysis(an) {
    }
};

vector<Scholar> scholars;
vector<Book> books;
vector<Question> questions;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(4096, '\n');
}

void saveQuestions() {
    ofstream qFile("questions.txt");
    if (!qFile.is_open()) {
        cerr << "无法创建 questions.txt" << endl;
        return;
    }
    for (const auto& q : questions) {
        qFile << q.content << endl;
        qFile << q.options.size() << endl;
        for (const auto& opt : q.options) qFile << opt << endl;
        qFile << q.answer << endl;
        qFile << q.analysis << endl;
    }
    qFile.close();
    cout << "题库已保存到 questions.txt" << endl;
}

void loadData() {
    ifstream scholarFile("scholars.txt");
    if (scholarFile.is_open()) {
        string name, dynasty, contribution, works;
        while (getline(scholarFile, name) && getline(scholarFile, dynasty) &&
            getline(scholarFile, contribution) && getline(scholarFile, works)) {
            scholars.emplace_back(name, dynasty, contribution, works);
        }
        scholarFile.close();
        cout << "名人数据加载成功，共加载 " << scholars.size() << " 位汉学名人" << endl;
    }
    else {
        cout << "未找到 scholars.txt，退出时自动创建" << endl;
    }

    ifstream bookFile("books.txt");
    if (bookFile.is_open()) {
        string title, category, author, intro;
        int year;
        while (getline(bookFile, title) && getline(bookFile, category) &&
            getline(bookFile, author) && getline(bookFile, intro) &&
            bookFile >> year && bookFile.ignore(4096, '\n')) {
            books.emplace_back(title, category, author, intro, year);
        }
        bookFile.close();
        cout << "著作数据加载成功，共加载 " << books.size() << " 部汉学著作" << endl;
    }
    else {
        cout << "未找到 books.txt，退出时自动创建" << endl;
    }

    ifstream qFile("questions.txt");
    if (qFile.is_open()) {
        string content, analysis, opt;
        char answer;
        int optCount;
        while (getline(qFile, content)) {
            qFile >> optCount; qFile.ignore(4096, '\n');
            vector<string> options;
            for (int i = 0; i < optCount; i++) {
                getline(qFile, opt);
                options.push_back(opt);
            }
            qFile >> answer; qFile.ignore(4096, '\n');
            getline(qFile, analysis);
            questions.emplace_back(content, options, answer, analysis);
        }
        qFile.close();
        cout << "题库加载成功，共加载 " << questions.size() << " 道题目" << endl;
    }
    else {
        cout << "未找到 questions.txt，无法加载题库" << endl;
    }
}

void saveData() {
    ofstream scholarFile("scholars.txt");
    if (scholarFile.is_open()) {
        for (const auto& s : scholars) {
            scholarFile << s.name << endl;
            scholarFile << s.dynasty << endl;
            scholarFile << s.contribution << endl;
            scholarFile << s.works << endl;
        }
        scholarFile.close();
        cout << "名人数据已保存" << endl;
    }
    else {
        cerr << "无法保存 scholars.txt" << endl;
    }

    ofstream bookFile("books.txt");
    if (bookFile.is_open()) {
        for (const auto& b : books) {
            bookFile << b.title << endl;
            bookFile << b.category << endl;
            bookFile << b.author << endl;
            bookFile << b.intro << endl;
            bookFile << b.dynastyYear << endl;
        }
        bookFile.close();
        cout << "著作数据已保存" << endl;
    }
    else {
        cerr << "无法保存 books.txt" << endl;
    }

    saveQuestions();
}

void scholarManager() {
    int choice;
    cout << "\n=== 汉学名人管理器 ===" << endl;
    cout << "1. 查询名人  2. 添加名人  3. 按朝代排序" << endl;
    cout << "请选择：";
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        clearInputBuffer();
        cout << "输入无效，请输入1-3：";
    }
    clearInputBuffer();

    if (choice == 1) {
        string name;
        cout << "输入姓名：";
        getline(cin, name);
        for (const auto& s : scholars) {
            if (s.name == name) {
                cout << "姓名：" << s.name << "\n朝代：" << s.dynasty << "\n贡献：" << s.contribution << "\n著作：" << s.works << endl;
                return;
            }
        }
        cout << "未找到该名人" << endl;
    }
    else if (choice == 2) {
        string name, dynasty, contribution, works;
        cout << "姓名：";
        getline(cin, name);
        cout << "朝代：";
        getline(cin, dynasty);
        cout << "贡献：";
        getline(cin, contribution);
        cout << "著作：";
        getline(cin, works);
        scholars.emplace_back(name, dynasty, contribution, works);
        saveData();
        cout << "添加成功！" << endl;
    }
    else if (choice == 3) {
        sort(scholars.begin(), scholars.end(),
            [](const Scholar& a, const Scholar& b) { return a.dynasty < b.dynasty; });
        cout << "排序结果：" << endl;
        for (const auto& s : scholars) {
            cout << s.dynasty << " - " << s.name << endl;
        }
    }
}

void bookCategoryStat() {
    string category;
    cout << "\n=== 著作分类统计 ===" << endl;
    cout << "输入类别：";
    clearInputBuffer();
    getline(cin, category);
    int count = 0;
    for (const auto& b : books) {
        if (b.category == category) {
            count++;
            cout << b.title << " - " << b.author << " (" << b.dynastyYear << "年)" << endl;
        }
    }
    cout << "该类别共 " << count << " 部著作" << endl;
}

void bookSearch() {
    string keyword;
    cout << "\n=== 著作模糊检索 ===" << endl;
    cout << "输入关键词：";
    clearInputBuffer();
    getline(cin, keyword);
    bool found = false;
    for (const auto& b : books) {
        if (b.title.find(keyword) != string::npos || b.author.find(keyword) != string::npos) {
            found = true;
            cout << "书名：" << b.title << "\n类别：" << b.category << "\n作者：" << b.author << "\n简介：" << b.intro << "\n年份：" << b.dynastyYear << endl << "----------------" << endl;
        }
    }
    if (!found) {
        cout << "未找到相关著作" << endl;
    }
}

void quiz() {
    int score = 0;
    cout << "\n=== 汉学小问答 ===" << endl;
    for (int i = 0; i < questions.size(); i++) {
        cout << "\n第" << i + 1 << "题：" << questions[i].content << endl;
        for (const auto& opt : questions[i].options) {
            cout << opt << endl;
        }
        char ans;
        cout << "答案：";
        while (!(cin >> ans)) {
            clearInputBuffer();
            cout << "输入无效，请输入字母：";
        }
        if (toupper(static_cast<unsigned char>(ans)) == toupper(static_cast<unsigned char>(questions[i].answer))) {
            cout << "正确！" << endl;
            score++;
        }
        else {
            cout << "错误！正确答案是" << questions[i].answer << endl;
        }
        cout << "解析：" << questions[i].analysis << endl;
    }
    cout << "\n得分：" << score << "/" << questions.size() << " 正确率：" << (double)score / questions.size() * 100 << "%" << endl;
}

int main() {
    loadData();
    int choice;
    while (true) {
        cout << "\n=== 汉学主题程序 V3.0 ===" << endl;
        cout << "1. 名人管理  2. 著作分类  3. 著作检索  4. 小问答  0. 退出" << endl;
        cout << "请选择：";
        while (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请输入数字：";
        }
        switch (choice) {
        case 1:
            scholarManager();
            break;
        case 2:
            bookCategoryStat();
            break;
        case 3:
            bookSearch();
            break;
        case 4:
            quiz();
            break;
        case 0:
            saveData();
            cout << "程序退出..." << endl;
            return 0;
        default:
            cout << "输入无效，请输入0-4" << endl;
        }
    }
}