# 定义源文件和输出文件
DIRS = procs maze family avl_tree huffman subway bus b_tree sort json_parser

SRC_EXTENSIONS = c cpp h
DIST_DIR = dist

OUTPUT = report.pdf
SOURCES = ./begin.md $(foreach dir,$(DIRS),../$(dir)/README.md $(DIST_DIR)/source_header.md $(wildcard $(DIST_DIR)/$(dir)/*.md))

all: $(DIRS) pdf clean
	@echo "Done!"

pdf:
	pandoc --highlight-style=tango -o $(OUTPUT) $(SOURCES) --pdf-engine=xelatex

clean:
	del $(DIST_DIR)

$(DIRS):
	mkdir -p $(DIST_DIR);
	mkdir -p $(DIST_DIR)/$@;
	echo -e "## 源代码" > $(DIST_DIR)/source_header.md;
	@for ext in $(SRC_EXTENSIONS); do \
		for file in ../$@/*.$$ext; do \
			if [ -f $$file ]; then \
				filename=$$(basename $$file); \
				echo -e "$$file\n\`\`\`cpp" > $(DIST_DIR)/$@/$$filename.md; \
				cat $$file >> $(DIST_DIR)/$@/$$filename.md; \
				echo -e "\n\`\`\`" >> $(DIST_DIR)/$@/$$filename.md; \
			fi \
		done \
	done


