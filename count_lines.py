import os

exts = ['.cpp', '.c', '.h']

def count_lines_in_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return sum(1 for line in file if line.strip())

def count_lines_in_directory(directory):
    total_lines = 0
    for root, _, files in os.walk(directory):
        # print(root)
        for file in files:
            if os.path.splitext(file)[1] in exts:
                file_path = os.path.join(root, file)
                file_lines = count_lines_in_file(file_path)
                print(f"{file_path}: {file_lines}")
                total_lines += file_lines
    return total_lines

if __name__ == "__main__":
    directory = os.path.dirname(os.path.abspath(__file__))
    total_lines = count_lines_in_directory(directory)
    print(f"Total lines of code: {total_lines}")
