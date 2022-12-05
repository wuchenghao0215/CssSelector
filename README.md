# CSS Selector 实验报告

实现一个 CSS 选择器 (HTML CSS Selector)

## 一、需求分析

1. 通过树数据结构对 HTML DOM 层次结构进行建模
2. 实现常用 HTML CSS Selector 以及对应的获取文本, HTML, 链接等操作
3. 开发测试或者演示程序, 展示 HTML CSS Selector 的使用方法, 验证正确性

<div STYLE="page-break-after: always;"></div>

## 二、概要设计

1. 抽象数据类型: DomNode 节点

    ```C
    ADT DomNode {
        数据对象 V
            V 是 HTML 文本中的元素节点组成的集合

        数据关系 R
            R = {H1, H2, H3}
            H1 = <v, v_parent>      表示 v_parent 是 v 的父亲节点
            H2 = <v, v_child>       表示 v_child 是 v 的最左孩子节点
            H3 = <v, v_sibling>     表示 v_sibling 是 v 的右兄弟节点

        基本操作 P
            打印 Outer HTML
            print_outer_html();

            打印 Text
            print_text();

            打印 attr 参数的 value
            print_attribution(attr);
    };
    ```

2. 抽象数据类型: DomTree 树

    ```C
    ADT DomTree {
        数据对象 V
            V 是所有 DomNode 按一定逻辑结构组成的集合

        数据关系 R
            R = {H}
            H = <v, v_l, v_r>       表示 v_l 是 v 的左子树, v_r 是 v 的右子树

        基本操作 P
            根据读取的 HTML 文本生成一棵树
            build_tree(content);

            先序遍历整棵树
            traversal();

            CSS 选择 (根据给出的限定条件进行筛选)
            select(tag_name, class_name, id_name, ...);
    };
    ```

3. 主程序

    ```cpp
    int main() {
        // 读取 HTML 文件
        std::string HTML_content;
        {...}

        // 解析 HTML 文本, 生成 DOM Tree
        DomTree tree;
        {...}

        // CSS 选择
        {
            // 读取命令
            std::string input; 
            {...}

            // 模式匹配
            std::string selector;
            std::string operation;
            {...}

            // 执行选择
            std::vector(Node *) results;
            switch(selector) {...}

            // 执行操作
            switch(operation) {...}
        }

        return 0;
    }
    ```

<div STYLE="page-break-after: always;"></div>

## 详细设计

1. Node 类

    ```C
        
    ```

<div STYLE="page-break-after: always;"></div>

## 用户手册

1. 本程序是 Unix 可执行文件, 可在 Windows/MacOS 系统上运行
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
6. selector: **element**
   
7. selector: **element.class**
   
8. selector: **element, element**
   
9.  selector: **element element**
    
10. selector: **element > element**
    
11. selector: **element + element**
    
12. selector: **element ~ element**