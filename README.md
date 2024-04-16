# Index_Serialize_Test

索引&amp;简单序列化测试

初步建立索引(unordered_map)，序列化存储(作者->文章标题，文章标题->文章信息)

每个文献大概占用0.5KB空间

```c++
void saveAuthorToTitles(const unordered_map<string, vector<string>>& map, const string& filename)
    
void saveTitleToArticle(const unordered_map<string, Article>& map, const string& filename)
    
void addArticle(const Article& article)

void searchByAuthor(const string& author)

void searchByTitle(const string& title)

void deSerialize(const string& path1,const string& path2, unordered_map<string, vector<string>>& authorToTitles, unordered_map<string, Article>& titleToArticle) 
```
