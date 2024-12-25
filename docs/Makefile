# 定义源文件和输出文件
SOURCES = ./report.md ../procs/README.md ../maze/README.md ../family/README.md ../avl_tree/README.md ../huffman/README.md ../subway/README.md ../bus/README.md ../b_tree/README.md ../sort/README.md ../json_parser/README.md
OUTPUT = ./report.pdf

# 生成PDF文件的规则
$(OUTPUT): $(SOURCES)
	pandoc --highlight-style=tango -o $(OUTPUT) $(SOURCES) --pdf-engine=xelatex

# 清理生成的PDF文件
clean:
	rm -f $(OUTPUT)

# 默认目标
all: $(OUTPUT)
