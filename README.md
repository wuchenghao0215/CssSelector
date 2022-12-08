# CSS Selector 实验报告

实现一个 CSS 选择器 (HTML CSS Selector)

## 需求分析

1. 通过树数据结构对 HTML DOM 层次结构进行建模
2. 实现常用 HTML CSS Selector 以及对应的获取文本, HTML, 链接等操作
3. 开发测试或者演示程序, 展示 HTML CSS Selector 的使用方法, 验证正确性

<div STYLE="page-break-after: always;"></div>

## 概要设计

![流程图](images/流程图-导出.jpg)

<div STYLE="page-break-after: always;"></div>

## 详细设计

1. Node 类

    ```cpp
    class Node {
    public:
        friend class DomTree;

        Node();

        // 含参构造函数
        Node(std::string tag_name, std::string class_name, std::string id_name,
             std::string open_tag);

        Node(std::string tag_name, std::string class_name, std::string id_name,
             std::string open_tag, std::string close_tag, std::string text);

        // 输入: indent, 缩进层数. 目的是打印符合缩进的 outer HTML
        void print_outer_html(int indent) const;

        // 打印 inner text
        void print_text() const;

        // 打印 tag
        void print_open_tag() const;

        // 判断是否是 inner text
        bool is_text() const;

        // 打印 href
        void print_href() const;

    private:
        std::string _tag_name;
        std::string _class_name;
        std::string _id_name;
        std::map<std::string, std::string> _attributes;
        std::string _open_tag;
        std::string _close_tag;
        std::string _text;
        Node *_parent;
        Node *_left_child;
        Node *_right_sibling;
    };
    ```

2. DomTree 类

    ```cpp
    class DomTree {
    public:
        DomTree();

        // 生成树
        void build_tree(const std::string &content);

        ~DomTree();

        // 遍历树
        std::vector<Node *> traversal() const;

        // 各个选择函数
        std::vector<Node *> select_1(const std::string &tag_name, const std::string &class_name, const std::string &id_name) const;

        std::vector<Node *> select_2(const std::string &class_1, const std::string &class_2) const;

        std::vector<Node *> select_3(const std::string &class_1, const std::string &class_2) const;

        std::vector<Node *> select_4(const std::string &tag_1, const std::string &tag_2) const;

        std::vector<Node *> select_5(const std::string &tag_1, const std::string &tag_2) const;

        std::vector<Node *> select_6(const std::string &tag_1, const std::string &tag_2) const;

        std::vector<Node *> select_7(const std::string &tag_1, const std::string &tag_2) const;

        std::vector<Node *> select_8(const std::string &tag_1, const std::string &tag_2) const;

        std::vector<Node *> select_9(const std::string &attribute) const;

        std::vector<Node *> select_10(const std::string &attribute, const std::string &value) const;

        std::vector<Node *> select_11(const std::string &attribute, const std::string &value) const;

        std::vector<Node *> select_12(const std::string &attribute, const std::string &value) const;

        std::vector<Node *> select_13(const std::string &attribute, const std::string &value) const;

    private:
        Node *_root;
    };
    ```

<div STYLE="page-break-after: always;"></div>

## 用户手册

1. 本程序是 Unix 可执行文件, 可在 Windows/MacOS/Linux 系统上运行
2. 本程序按照命令行的形式, 采用标准输入输出与用户交互
3. 进入程序后, 首先按照提示输入 HTML 文件的路径, 随后程序会读取文件生成 DOM Tree
4. 然后输入对应的 CSS Selector 指令, 进行选择
5. 输入格式为 ```css.selector("#your_order#")[#index#].#func#()```
6. 其中 ```#your_order#``` 是必要的, ```[#index#]``` 和 ```.#func#()``` 是可选的
7. 程序会读取用户的指令并返回相应结果
8. 输入 ```[#index#]``` 可以选择指定找到的第几个节点, 如果未输入 ```[#index#]```, 程序会返回结果的列表
9. 输入 ```.#func#()``` 可以指定返回的信息, 如 ```.text()``` 返回 inner_text, ```.html()``` 返回 outer_html, ```.href()``` 返回 \<a> 标签的 href 链接; 如果未输入 ```.#func#()```, 程序会返回找到的节点的 tag 信息 (如果 tag 过长, 输出前 80 个字符)

<div STYLE="page-break-after: always;"></div>

## 测试样例

选择 lab3_news.html 用作测试

### 测试选择器

1. selector: **.class**
    > \> css.selector(".more")
    > [\<a href="/" class="homepage more">,
    > \<a href="http://www.ruc.edu.cn" target="_blank" class="more">,
    > \<a href="http://portal.ruc.edu.cn" target="_blank" class="more">,
    > \<a href="mailto:leader@ruc.edu.cn" class="more">]

    ![scrapy](images/2022-12-06%2023.18.15.png)
2. selector: **.class1.class2**
    > \> css.selector(".aligncenter.size-full")  
    > [\<img class="aligncenter size-full wp-image-407864" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407865" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407860" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407855" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407856" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407857" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407881" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407873" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407874" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407882" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407856" src="http://news.ruc.edu.cn/wp...>]

    ![scrapy](images/2022-12-06%2023.18.45.png)
3. selector: **.class1 .class2**
    > \> css.selector(".toolbox_fontsize .f1")
    > [\<span class="f1 cur">]

    ![scrapy](images/2022-12-06%2023.19.21.png)
4. selector: **#id**
    > \> css.selector("#header")
    > [\<div id="header">]

    ![scrapy](images/2022-12-06%2023.19.40.png)
5. selector: **\***
    > \> css.selector("*")
    > [\<html xmlns="http://www.w3.org/1999/xhtml">,
    > \<head>,
    > \<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />,
    > \<body>,
    > \<meta name="viewport" content="width=device-width, initial-scale=1.0">,
    > \<div id="wrapper">,
    > \<meta name="format-detection" content="telephone=no">,
    > \<div id="header_top_nav">,
    > ...]
    // 所有结点, 由于太多不全部展示

    ![scrapy](images/2022-12-06%2023.20.12.png)
6. selector: **element**
    > \> css.selector("meta")
    > [\<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />,
    > \<meta name="viewport" content="width=device-width, initial-scale=1.0">,
    > \<meta name="format-detection" content="telephone=no">]

    ![scrapy](images/2022-12-06%2023.20.52.png)
7. selector: **element.class**
    > \> css.selector("a.inews")
    > [\<a href="/archives/category/important_news/campus" class="inews dist">,
    > \<a href="/archives/category/important_news/affairs" class="inews">,
    > \<a href="/archives/category/important_news/exchange" class="inews">,
    > \<a href="/archives/category/important_news/scholars" class="inews">,
    > \<a href="/archives/category/important_news/students" class="inews">,
    > \<a href="/archives/category/important_news/academic" class="inews">]

    ![scrapy](images/2022-12-06%2023.21.08.png)
8. selector: **element, element**
    > \> css.selector("meta, input")
    > [\<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />,
    > \<meta name="viewport" content="width=device-width, initial-scale=1.0">,
    > \<meta name="format-detection" content="telephone=no">,
    > \<input type="text" id="s" class="inputtext" name="s" value="请输入搜索内容...>,
    . \<input type="image" id="search_btn" name="commit" class="submit" value=""  src="/...>]

    ![scrapy](images/2022-12-06%2023.21.34.png)
9. selector: **element element**
    > \> css.selector("div ul")
    > [\<ul>,
    > \<ul>,
    > \<ul>,
    > \<ul class="ca1_hot_item_t3">,
    > \<ul class="ca1_hot_item_s3">,
    > \<ul>]

    ![scrapy](images/2022-12-07%2000.03.13.png)
10. selector: **element > element**
    > \> css.selector("div > ul")
    > [\<ul>,
    > \<ul>,
    > \<ul>,
    > \<ul class="ca1_hot_item_t3">,
    > \<ul>,
    > \<ul class="ca1_hot_item_s3">]

    ![scrapy](images/2022-12-07%2000.06.43.png)
11. selector: **element + element**
    > \> css.selector("link + script")
    > [\<script type="text/javascript" src="/wp-content/themes/rucnews/js/jquery-1.7.2.mi...>,
    > \<script type="text/javascript">]

    ![scrapy](images/2022-12-07%2000.07.19.png)
12. selector: **element ~ element**
    > \> css.selector("link ~ script")
    > [\<script type="text/javascript" src="/wp-content/themes/rucnews/js/jquery-1.7.2.mi...>,
    > \<script type="text/javascript" src="/wp-content/themes/rucnews/js/backgroundposit...>,
    > \<script type="text/javascript" src="/wp-content/themes/rucnews/js/jquery.fancybox...>,
    > \<script type="text/javascript">,
    > \<script type="text/javascript">]

    ![scrapy](images/2022-12-07%2000.07.37.png)
13. selector: **[attribute]**
    > \> css.selector("[rel]")
    > [\<link rel="shortcut icon" href="/wp-content/themes/rucnews/images/ruc.ico" type="...>,
    > \<link rel="stylesheet" type="text/css" href="/wp-content/themes/rucnews/style.css...>,
    > \<link rel="stylesheet" type="text/css" href="/wp-content/themes/rucnews/style-spe...>,
    > \<link rel="stylesheet" type="text/css" href="/wp-content/themes/rucnews/css/jquer...>,
    > \<link rel="stylesheet" type="text/css" href="/wp-content/themes/rucnews/css/mobil...>,
    > \<link rel="stylesheet" type="text/css" href="/wp-content/themes/rucnews/css/slide...>,
    > \<link rel="EditURI" type="application/rsd+xml" title="RSD" href="https://news.ruc...>,
    > \<link rel="wlwmanifest" type="application/wlwmanifest+xml" href="https://news.ruc...>,
    > \<link rel="canonical" href="https://news.ruc.edu.cn/archives/407847" />,
    > \<link rel="alternate" type="application/json+oembed" href="https://news.ruc.edu.c...>,
    > \<link rel="alternate" type="text/xml+oembed" href="https://news.ruc.edu.cn/wp-jso...>]

    ![scrapy](images/2022-12-07%2000.07.55.png)
14. selector: **[attribute=value]**
    > \> css.selector("[href=/]")
    > [\<a href="/" class="homepage more">,
    > \<a href="/">,
    > \<a class="back" href="/">,
    > \<a href="/">,
    > \<a class="link2home" href="/">,
    > \<a href="/">]

    ![scrapy](images/2022-12-07%2000.09.03.png)
15. selector: **[attribute~=value]**
    > \> css.selector("[srcset~=1500,]")
    > [\<img class="aligncenter size-full wp-image-407864" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407865" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407855" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407856" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407857" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407881" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407882" src="http://news.ruc.edu.cn/wp...>,
    > \<img class="aligncenter size-full wp-image-407856" src="http://news.ruc.edu.cn/wp...>]

    ![scrapy](images/2022-12-07%2000.13.29.png)
16. selector: **[attribute^=value]**
    > \> css.selector("[href^=https]")
    > [\<a href="https://news.ruc.edu.cn/archives/category/special_news/20th">,
    > \<link rel="EditURI" type="application/rsd+xml" title="RSD" href="https://news.ruc...>,
    > \<link rel="wlwmanifest" type="application/wlwmanifest+xml" href="https://news.ruc...>,
    > \<a href="https://news.ruc.edu.cn/archives/408586" >,
    > \<a href="https://news.ruc.edu.cn/archives/408414" >,
    > \<a href="https://news.ruc.edu.cn/archives/408352">,
    > \<a href="https://news.ruc.edu.cn/archives/408399" class="last">,
    > \<a href="https://news.ruc.edu.cn/archives/408291">,
    > \<link rel="canonical" href="https://news.ruc.edu.cn/archives/407847" />,
    > \<a href="https://news.ruc.edu.cn/archives/408197">,
    > \<a href="https://news.ruc.edu.cn/archives/407334">,
    > \<link rel="alternate" type="application/json+oembed" href="https://news.ruc.edu.c...>,
    > \<link rel="alternate" type="text/xml+oembed" href="https://news.ruc.edu.cn/wp-jso...>,
    > \<a href="https://mp.weixin.qq.com/s/v8NE0Sk_vlQJNLtOuFsjRQ">]
    // 这里和 scrapy 的结果有出入, 比之少了2个结果. 推断原因是编码格式不同, 导致含有汉字的链接无法被正确读取

    ![scrapy](images/2022-12-07%2000.16.13.png)
17. selector: **[attribute$=value]**
    > \> css.selector("[src$=.jpg]")
    > [\<img class="aligncenter size-full wp-image-407860" src="http://news.ruc.edu.cn/wp...>]

    ![scrapy](images/2022-12-07%2000.27.54.png)

<div STYLE="page-break-after: always;"></div>

### 测试函数

1. inner text
    ![css](images/2022-12-07%2017.33.46.png)
2. outer HTML
    ![css](images/2022-12-07%2017.33.14.png)
3. href
    ![css](images/2022-12-07%2017.52.17.png)

### 测试对返回的element再次调用选择器进行查询

输入格式: **result[i].function()**

![css](image/../images/2022-12-07%2021.58.10.png)