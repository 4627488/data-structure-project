# 《数据结构》 课程设计 2024秋

[![Build Documentation](https://github.com/4627488/data-structure-project/actions/workflows/docs.yml/badge.svg)](https://github.com/4627488/data-structure-project/actions/workflows/docs.yml)

## 课程设计题目和要求

[课程设计题目](./problems.md)

## 目录

### 必做题

[1、系统进程统计（必做）（链表）](./procs/)

[2、迷宫问题（必做）（栈与队列）](./maze/)

[3、家谱管理系统（必做）（树）](./family/)

[4、 平衡二叉树编程](./avl_tree/)

[5、Huffman编码与解码 (必做)（Huffman编码、二叉树）](./huffman/)

[6、地铁修建 (必做) （图）](./subway/)

[7、公交线路提示 (必做) （图）](./bus/)

[8、B-树应用 （必做） （查找）](./b_tree/)

[9、排序算法比较 （必做）（排序）](./sort/)

### 选做题

[【18】树的应用 (选做)（树）](./json_parser/)


## 编译课程设计报告

报告基于 markdown 编写，使用 pandoc 生成 PDF。`docs` 目录下包含了报告的部分 markdown 源文件.

使用 Python 脚本编译：

```shell
cd docs
python make_docs.py
```

亦可在 [Actions - Build Documentation](https://github.com/4627488/data-structure-project/actions/workflows/docs.yml) 中的 Artifacts 下载。

## 许可证

[MIT with additional terms](./LICENSE)

> Additional Terms:
> 1. This software is provided for educational purposes only.
> 2. You may not submit this software or any derivative works in any form to 
>    fulfill the requirements of any course or academic program during the same 
>    semester in which the original author is enrolled in the same course or 
>    academic program.
> 3. Any violation of these terms will be considered academic dishonesty.

> 附加条款：
> 1. 代码仅供教育目的使用。
> 2. 您不得以任何形式提交此软件或任何衍生作品用于完成原作者在同一学期内所在课程或学术计划的任何要求。
> 3. 任何违反这些条款的行为将被视为学术不诚信。

