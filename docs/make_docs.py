import os
import shutil
import subprocess
import glob
import jinja2
import re

DIRS = [
    "procs",
    "maze",
    "family",
    "avl_tree",
    "huffman",
    "subway",
    "bus",
    "b_tree",
    "sort",
    "json_parser_tree",
]
SRC_EXTENSIONS = ["c", "cpp", "h"]
DIST_DIR = "dist"
OUTPUT = "report.pdf"
OUTPUT_DOCX = "report.docx"
SOURCES = []


def run_command(command):
    print(f"Running command: {command}")
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        raise Exception(f"Command failed: {command}")


def format_cpp_code(code: str) -> str:
    process = subprocess.Popen(
        ["clang-format"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    formatted_code, error = process.communicate(input=code)
    if error:
        raise RuntimeError(f"Error formatting code: {error}")
    return formatted_code


def create_dirs():
    if not os.path.exists(DIST_DIR):
        os.makedirs(DIST_DIR)
    with open(os.path.join(DIST_DIR, "newpage.md"), "w", encoding="utf-8") as f:
        f.write("\\newpage\n")
    for dir in DIRS:
        dir_path = os.path.join(DIST_DIR, dir)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)


def count_lines_in_file(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        return sum(1 for line in file if line.strip())


args = {}


def generate_sources():
    SOURCES.append(os.path.join(DIST_DIR, "begin.md"))
    for dir in DIRS:
        SOURCES.append(os.path.join(DIST_DIR, dir, "README.md"))
        SOURCES.append(os.path.join(DIST_DIR, dir, "source_header.md"))
        for ext in SRC_EXTENSIONS:
            for file in glob.glob(f"../{dir}/*.{ext}"):
                if os.path.isfile(file):
                    filename = os.path.basename(file)
                    SOURCES.append(os.path.join(DIST_DIR, dir, f"{filename}.md"))
        SOURCES.append(os.path.join(DIST_DIR, "newpage.md"))
    SOURCES.append(os.path.join(DIST_DIR, "end.md"))


def count_lines():
    args["total_lines"] = 0
    for dir in DIRS:
        args[f"{dir}_lines"] = 0
        for ext in SRC_EXTENSIONS:
            for file in glob.glob(f"../{dir}/*.{ext}"):
                if os.path.isfile(file):
                    filelines = count_lines_in_file(file)
                    args[f"{dir}_lines"] += filelines
                    args["total_lines"] += filelines


def render_markdown():
    for dir in DIRS:
        for ext in SRC_EXTENSIONS:
            for file in glob.glob(f"../{dir}/*.{ext}"):
                if os.path.isfile(file):
                    filename = os.path.basename(file)
                    with open(
                        os.path.join(DIST_DIR, dir, f"{filename}.md"),
                        "w",
                        encoding="utf-8",
                    ) as f:
                        f.write(
                            f"### {filename} (共 {count_lines_in_file(file)} 行)\n\n```{ext}\n"
                        )
                        with open(file, "r", encoding="utf-8") as src:
                            f.write(src.read())
                        f.write("\n```\n")
        with open(
            os.path.join(DIST_DIR, dir, "source_header.md"), "w", encoding="utf-8"
        ) as f:
            f.write(f"## 源代码 (共 {args[f'{dir}_lines']} 行)\n\n")
        readme_file = f"../{dir}/README.md"
        readme_template = jinja2.Template(
            open(readme_file, "r", encoding="utf-8").read()
        )
        rendered_content = readme_template.render(args)  # 变量填充后的md内容
        # 复制图片文件
        image_paths = re.findall(r"!\[(.*?)\]\((.*?)\)", rendered_content)
        for _, image_path in image_paths:  # 找到所有引用的图片名
            src_image_path = os.path.join("..", dir, image_path)
            if os.path.isfile(
                src_image_path
            ):  # 确保图片文件存在，如果是网络图片就不复制了
                dest_image_path = os.path.join("dist", dir, image_path)
                os.makedirs(os.path.dirname(dest_image_path), exist_ok=True)
                shutil.copyfile(src_image_path, dest_image_path)
        rendered_content = re.sub(
            r"!\[(.*?)\]\((?!http)(.*?)\)", rf"![\1](./dist/{dir}/\2)", rendered_content
        )  # 替换md内容中本地图片的路径为复制之后的

        with open(os.path.join(DIST_DIR, dir, "README.md"), "w", encoding="utf-8") as f:
            f.write(rendered_content)
    with open(os.path.join(DIST_DIR, "begin.md"), "w", encoding="utf-8") as f:
        f.write(open("begin.md", "r", encoding="utf-8").read())
    end_template = jinja2.Template(open("end.md", "r", encoding="utf-8").read())
    with open(os.path.join(DIST_DIR, "end.md"), "w", encoding="utf-8") as f:
        f.write(end_template.render(args))


def render_summary():
    args["code_line_summary"] = ""
    for dir in DIRS:
        args["code_line_summary"] += f"{dir}: {args[f'{dir}_lines']} Lines\n"
    args["code_line_summary"] += f"\n{args['total_lines']} Lines in Total\n"
    print(args["code_line_summary"])

def build_pdf():
    sources = " ".join(
        [f'"{source}"' for pattern in SOURCES for source in glob.glob(pattern)]
    )
    command = (
        f"pandoc -o {OUTPUT} {sources} --pdf-engine=xelatex --highlight-style=tango"
    )
    run_command(command)


def clean():
    shutil.rmtree(DIST_DIR, ignore_errors=True)


def main():
    create_dirs()
    count_lines()
    render_summary()
    render_markdown()
    generate_sources()
    build_pdf()
    clean()
    print("Done!")


if __name__ == "__main__":
    main()
