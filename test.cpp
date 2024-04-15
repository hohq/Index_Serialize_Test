// 基本思路
// 1. 解析xml文件 2. 建立索引并存储 3. 序列化到文件
// 用xml库解析文件(编写接口)，unordered_map存储索引，序列化使用cereal库
// 读取文件，反序列化，查询索引，返回结果

#include <unordered_map>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>

using namespace std;

struct Article {
    string element;
    string key;
    vector<string> author;
    string editor;
    string title;
    string booktitle;
    int pages;
    int year;
    string address;
    string journal;
    string volume;
    string number;
    string month;
    string url;
    string ee;
    string cdrom;
    string cite;
    string publisher;
    string note;
    string crossref;
    string isbn;
    string series;
    string school;
    string chapter;
    string publnr;
    string stream;
    string rel;
    
    template <class Archive>
    void serialize(Archive& ar) {
        ar(element, key, author, editor, title, booktitle, pages, year, address, journal, volume, number, month, url, ee, cdrom, cite, publisher, note, crossref, isbn, series, school, chapter, publnr, stream, rel);
    }
};

unordered_map<string, vector<string>> authorToTitles;  // 存储作者名到他们的文章标题的映射
unordered_map<string, Article> titleToArticle;  // 存储文章标题到文章信息的映射


// 序列化存储索引
void saveAuthorToTitles(const unordered_map<string, vector<string>>& map, const string& filename) {
    ofstream file(filename, ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(map);
    cout<<"saveAuthorToTitles success"<<endl;
}
void saveTitleToArticle(const unordered_map<string, Article>& map, const string& filename) {
    ofstream file(filename, ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(map);
    cout<<"saveTitleToArticle success"<<endl;
}


void addArticle(const Article& article) {
    // 添加新的文章到titleToArticle
    titleToArticle[article.title] = article;
    // 更新authorToTitles
    for (const string& author : article.author) {
        authorToTitles[author].push_back(article.title);
    }
}

// // 删除
// void removeArticle(const string& author, const string& title) {
//     auto& titles = authorToTitles[author];
//     titles.erase(remove(titles.begin(), titles.end(), title), titles.end());
//     titleToArticle.erase(title);
// }


// 按作者名和文章标题搜索文章
void searchByAuthor(const string& author) {
    if (authorToTitles.find(author) != authorToTitles.end()) {
        const vector<string>& titles = authorToTitles[author];
        for (const string& title : titles) {
            const Article& article = titleToArticle[title];
            // 显示文章信息，还是一大坨以后再优化
            cout << "Title: " << article.title << '\n';
            cout << "Authors: ";
        for (const string& author : article.author) {
            cout << author << ' ';
        }
        cout << '\n';
        cout << "Journal: " << article.journal << '\n';
        cout << "Year: " << article.year << '\n';
        cout << "EE: " << article.ee << '\n';
        }
    } else {
        cout << "No articles found for author: " << author << '\n';
    }
}
void searchByTitle(const string& title) {
    if (titleToArticle.find(title) != titleToArticle.end()) {
        const Article& article = titleToArticle[title];
        // 显示文章的信息，以后再优化
        cout << "Title: " << article.title << '\n';
        cout << "Authors: ";
        for (const string& author : article.author) {
            cout << author << ' ';
        }
        cout << '\n';
        cout << "Journal: " << article.journal << '\n';
        cout << "Year: " << article.year << '\n';
        cout << "EE: " << article.ee << '\n';
    } else {
        cout << "No article found with title: " << title << '\n';
    }
}



int main() {

// 解析XML文件并填充article的字段
Article article1;
Article article2;

// 测试数据
article1.element = "article";
article1.key = "tr/meltdown/s18";
article1.author = {"Paul Kocher", "Daniel Genkin", "Daniel Gruss", "Werner Haas", "Mike Hamburg",
                     "Moritz Lipp", "Stefan Mangard", "Thomas Prescher", "Michael Schwarz"};
article1.title = "Spectre Attacks: Exploiting Speculative Execution";
article1.journal = "meltdownattack.com";
article1.year = 2018;
article1.ee = "https://meltdownattack.com/spectre.pdf";
addArticle(article1);

article2.element = "article";
article2.key = "tr/meltdown/m18";
article2.author = {"Moritz Lipp", "Michael Schwarz", "Daniel Gruss", "Thomas Prescher", "Werner Haas",
                     "Stefan Mangard", "Paul Kocher", "Daniel Genkin", "Mike Hamburg"};
article2.title = "Meltdown";
article2.journal = "meltdownattack.com";
article2.year = 2018;
article2.ee = "https://meltdownattack.com/meltdown.pdf";
addArticle(article2);

// 测试搜索
string author = "Daniel Genkin";
searchByAuthor(author);
cout<<"--------------------------------------"<<endl;
string title = "Meltdown";
searchByTitle(title);
cout<<"--------------------------------------"<<endl;

// 序列化索引并存储：作者->文章标题，文章标题->文章信息
saveAuthorToTitles(authorToTitles, "Q:\\dataStructures\\authorToTitles.bin");
saveTitleToArticle(titleToArticle, "Q:\\dataStructures\\titleToArticle.bin");
return 0;
}