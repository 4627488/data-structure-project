import os
import shutil
import subprocess
import glob

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
    with open(os.path.join(DIST_DIR, "source_header.md"), "w", encoding="utf-8") as f:
            f.write("## 源代码\n")
    with open(os.path.join(DIST_DIR, "newpage.md"), "w", encoding="utf-8") as f:
            f.write("\\newpage\n")
    for dir in DIRS:
        dir_path = os.path.join(DIST_DIR, dir)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)
        

def generate_sources():
    SOURCES.append(f"./begin.md")
    for dir in DIRS:
        SOURCES.append(f"../{dir}/README.md")
        for ext in SRC_EXTENSIONS:
            for file in glob.glob(f"../{dir}/*.{ext}"):
                if os.path.isfile(file):
                    filename = os.path.basename(file)
                    with open(os.path.join(DIST_DIR, dir, f"{filename}.md"), "w", encoding="utf-8") as f:
                        f.write(f"### {filename}\n\n```cpp\n")
                        with open(file, "r", encoding="utf-8") as src_file:
                            f.write(src_file.read())
                        f.write("\n```\n")
                    SOURCES.append(f"{DIST_DIR}/{dir}/{filename}.md")
        SOURCES.append(f"{DIST_DIR}/newpage.md")
    SOURCES.append(f"./end.md")

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
    generate_sources()
    build_pdf()
    build_docx()
    clean()
    print("Done!")

if __name__ == "__main__":
    main()