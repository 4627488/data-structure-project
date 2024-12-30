import os
import shutil
import subprocess
import glob
import jinja2

DIRS = ["procs", "maze", "family", "avl_tree", "huffman", "subway", "bus", "b_tree", "sort", "json-parser-tree"]
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
    with open(file_path, 'r', encoding='utf-8') as file:
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
                    with open(os.path.join(DIST_DIR, dir, f"{filename}.md"), "w", encoding="utf-8") as f:
                        f.write(f"### {filename} (共 {count_lines_in_file(file)} 行)\n\n```{ext}\n")
                        with open(file, "r", encoding="utf-8") as src:
                            f.write(src.read())
                        f.write("\n```\n")
        with open(os.path.join(DIST_DIR, dir, "source_header.md"), "w", encoding="utf-8") as f:
            f.write(f"## 源代码 (共 {args[f'{dir}_lines']} 行)\n\n")
        readme_file = f"../{dir}/README.md"
        readme_template = jinja2.Template(open(readme_file, "r", encoding="utf-8").read())
        with open(os.path.join(DIST_DIR, dir, "README.md"), "w", encoding="utf-8") as f:
            f.write(readme_template.render(args))
    with open(os.path.join(DIST_DIR, "begin.md"), "w", encoding="utf-8") as f:
        f.write(open("begin.md", "r", encoding="utf-8").read())
    end_template = jinja2.Template(open("end.md", "r", encoding="utf-8").read())
    with open(os.path.join(DIST_DIR, "end.md"), "w", encoding="utf-8") as f:
        f.write(end_template.render(args))

def build_pdf():
    sources = ' '.join([f'"{source}"' for pattern in SOURCES for source in glob.glob(pattern)])
    command = f"pandoc --highlight-style=tango -o {OUTPUT} {sources} --pdf-engine=xelatex --wrap=auto"
    run_command(command)

def build_docx():
    sources = ' '.join([f'"{source}"' for pattern in SOURCES for source in glob.glob(pattern)])
    command = f"pandoc --highlight-style=tango -o {OUTPUT_DOCX} {sources} --wrap=auto"
    run_command(command)

def clean():
    shutil.rmtree(DIST_DIR, ignore_errors=True)

def main():
    create_dirs()
    count_lines()
    render_markdown()
    generate_sources()
    build_pdf()
    build_docx()
    clean()
    print("Done!")

if __name__ == "__main__":
    main()