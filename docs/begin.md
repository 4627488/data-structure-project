---
geometry: margin=1in
header-includes:
  - \usepackage{graphicx}
  - \usepackage{titling}
  - \usepackage{xeCJK}
  - \usepackage{fancyhdr}
  - \usepackage{tikz}
  - \pagestyle{fancy}
  - \fancyhead[L]{\leftmark}
  - \fancyhead[R]{《数据结构》课程设计报告}
  - \fancyfoot[L]{\href{https://github.com/4627488/data-structure-project}{github.com/4627488/data-structure-project}}
  - \fancyfoot[C]{\thepage}
  - \fancyfoot[R]{2025年1月}
  - \pretitle{\begin{center}\vspace{\bigskipamount}\Huge\bfseries}
  - \posttitle{\end{center}}
  - \renewcommand{\sectionmark}[1]{\markboth{#1}{}}
---

\begin{titlepage}
    \begin{tikzpicture}[remember picture, overlay]
        % 在右上角插入校徽
        \node[anchor=north east, xshift=-2cm, yshift=-1cm] at (current page.north east) {
            \includegraphics[width=7cm]{nuaa_logo.png}
        };
    \end{tikzpicture}
    \centering
    \vspace*{3cm}
    
    \fontsize{40}{50}\selectfont
    \textbf{数据结构课程设计报告}\\[0.5cm]

    \fontsize{20}{30}\selectfont
    \textbf{2024-2025 学年第一学期}\\[3cm]
    
    \begin{tabular}{rl}
        姓\hspace{1em}名： & 黄耘青 \\[0.5cm]
        学\hspace{1em}号： & 022330225 \\[0.5cm]
        指导教师： & 高航 \\[0.5cm]
        班\hspace{1em}级： & 1623301 \\[4cm]
    \end{tabular}

    \vfill
    \fontsize{15}{25}\selectfont
    南京航空航天大学 \hspace{0.2em} 计算机科学与技术学院/软件学院
    
\end{titlepage}


\newpage

<style>
  ul {
    line-height: 1.2; /* 调整行间距 */
  }
</style>

\renewcommand{\contentsname}{目录}
\tableofcontents

\newpage