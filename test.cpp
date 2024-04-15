// 基本思路
// 1. 解析xml文件 2. 建立索引并存储 3. 序列化到文件
// 用xml库解析文件，unordered_map存储索引，序列化使用cereal库
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


/**
 * 序列化存储索引(作者->文章)到文件
 * 
 * @param map 索引。这是一个unordered_map，包含要存储的索引信息。
 * 
 * @param filename 文件名。这是一个字符串，表示要将索引存储到的文件名。
 * 
 * @return 无返回值。此函数会将给定的索引信息序列化并存储到文件中。
 * 
 * @throws invalid_argument 如果文件名为空。
 */
void saveAuthorToTitles(const unordered_map<string, vector<string>>& map, const string& filename) {
    if (filename.empty()) 
        throw invalid_argument("File path cannot be empty.");
    ofstream file(filename, ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(map);
    cout<<"saveAuthorToTitles success"<<endl;
}

/**
 * 序列化存储索引(标题->文章)到文件
 * 
 * @param map 索引。这是一个unordered_map，包含要存储的索引信息。
 * 
 * @param filename 文件名。这是一个字符串，表示要将索引存储到的文件名。
 * 
 * @return 无返回值。此函数会将给定的索引信息序列化并存储到文件中。
 * 
 * @throws invalid_argument 如果文件名为空。
 */
void saveTitleToArticle(const unordered_map<string, Article>& map, const string& filename) {
    if (filename.empty()) 
        throw invalid_argument("File path cannot be empty.");
    ofstream file(filename, ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(map);
    cout<<"saveTitleToArticle success"<<endl;
}

/**
 * 添加新的文章到索引中
 * 
 * @param article 文章信息。是一个Article对象，包含文章的所有信息，包括标题、作者、期刊、年份和EE等。
 * 
 * @return 无返回值。此函数会将给定的文章信息添加到索引中，包括titleToArticle和authorToTitles。
 * 
 * @throws invalid_argument 如果文章的标题或作者为空。
 */
void addArticle(const Article& article) {
    // 检查文章的标题和作者是否为空
    if (article.title.empty()) {
        throw invalid_argument("Article title can't be empty.");
    }
    if (article.author.empty()) {
        throw invalid_argument("Article must have at least one author.");
    }

    // 添加新的文章到titleToArticle
    titleToArticle[article.title] = article;
    // 循环                                                                                                                                                                                                                                                                                                                                                                      更新authorToTitles
    for (const string& author : article.author) {
        authorToTitles[author].push_back(article.title);
    }
}

/**
 * 按作者名搜索文章
 * 
 * @param author 作者名。这是一个字符串，表示要搜索的作者的名字。
 * 
 * @return 无返回值。此函数会打印出与给定作者名匹配的所有文章的信息，包括标题、作者、期刊、年份和EE。如果没有找到匹配的文章，会打印出提示信息。
 * 
 * @throws invalid_argument 如果作者名为空。 
 * */
void searchByAuthor(const string& author) {
    if (author.empty())
        throw invalid_argument("Author name cannot be empty.");

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

/**
 * 按文章标题搜索文章
 * 
 * @param title 文章标题。这是一个字符串，表示要搜索的文章的标题。
 * 
 * @return 无返回值。此函数会打印出与给定标题匹配的文章的信息，包括标题、作者、期刊、年份和EE。如果没有找到匹配的文章，会打印出提示信息。
 * 
 * @throws invalid_argument 如果标题为空。
 * */
void searchByTitle(const string& title) {
    if (title.empty())
        throw invalid_argument("Title cannot be empty.");

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