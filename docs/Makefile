# 定义源文件和输出文件
DIRS = procs maze family avl_tree huffman subway bus b_tree sort json-parser-tree

SRC_EXTENSIONS = c cpp h
DIST_DIR = dist

OUTPUT = report.pdf
OUTPUT_DOCX = report.docx
SOURCES = ./begin.md $(foreach dir,$(DIRS),../$(dir)/README.md $(DIST_DIR)/source_header.md $(wildcard $(DIST_DIR)/$(dir)/*.md)) ./end.md

all: $(DIRS) pdf docx clean
	@echo "Done!"

pdf:
	pandoc --highlight-style=tango -o $(OUTPUT) $(SOURCES) --pdf-engine=xelatex --wrap=auto

docx:
	pandoc --highlight-style=tango -o $(OUTPUT_DOCX) $(SOURCES) --wrap=auto

clean:
ifeq ($(OS),Windows_NT)
	powershell -Command "Remove-Item -Path $(DIST_DIR) -Recurse -Force"
else
	rm -rf $(DIST_DIR)
endif

$(DIRS):
	mkdir -p $(DIST_DIR);
	mkdir -p $(DIST_DIR)/$@;
	echo -e "## 源代码" > $(DIST_DIR)/source_header.md;
	@for ext in $(SRC_EXTENSIONS); do \
		for file in ../$@/*.$$ext; do \
			if [ -f $$file ]; then \
				filename=$$(basename $$file); \
				echo -e "### $$file\n\`\`\`cpp" > $(DIST_DIR)/$@/$$filename.md; \
				cat $$file >> $(DIST_DIR)/$@/$$filename.md; \
				echo -e "\n\`\`\`" >> $(DIST_DIR)/$@/$$filename.md; \
			fi \
		done \
	done


