# CSS Selector 实验报告

实现一个 CSS 选择器 (HTML CSS Selector)

## 需求分析

1. 通过树数据结构对 HTML DOM 层次结构进行建模
2. 实现常用 HTML CSS Selector 以及对应的获取文本, HTML, 链接等操作
3. 开发测试或者演示程序, 展示 HTML CSS Selector 的使用方法, 验证正确性

<div STYLE="page-break-after: always;"></div>

## 概要设计

// TODO 流程图

<div STYLE="page-break-after: always;"></div>

## 详细设计

1. Node 类

    ```cpp
    class Node {
    public:
        // 声明友元类
        friend class DomTree;

        // 构造函数
        Node();

        // 含参构造函数
        Node(std::string tag_name, std::string class_name, std::string id_name,
             std::string open_tag);

        // 含参构造函数
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

1. selector: **.class**
    > \> css.selector(".more")
    > [\<a href="/" class="homepage more">,
    > \<a href="http://www.ruc.edu.cn" target="_blank" class="more">,
    > \<a href="http://portal.ruc.edu.cn" target="_blank" class="more">,
    > \<a href="mailto:leader@ruc.edu.cn" class="more">]

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
3. selector: **.class1 .class2**
    > \> css.selector(".toolbox_fontsize .f1")
    > [\<span class="f1 cur">]
4. selector: **#id**
    > \> css.selector("#header")
    > [\<div id="header">]
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
6. selector: **element**
    > \> css.selector("meta")
    > [\<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />,
    > \<meta name="viewport" content="width=device-width, initial-scale=1.0">,
    > \<meta name="format-detection" content="telephone=no">]
7. selector: **element.class**
    > \> css.selector("a.inews")
    > [\<a href="/archives/category/important_news/campus" class="inews dist">,
    > \<a href="/archives/category/important_news/affairs" class="inews">,
    > \<a href="/archives/category/important_news/exchange" class="inews">,
    > \<a href="/archives/category/important_news/scholars" class="inews">,
    > \<a href="/archives/category/important_news/students" class="inews">,
    > \<a href="/archives/category/important_news/academic" class="inews">]
8. selector: **element, element**

9.  selector: **element element**
    
10. selector: **element > element**
    
11. selector: **element + element**
    
12. selector: **element ~ element**