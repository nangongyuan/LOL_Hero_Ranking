import os

# 设置源文件扩展名
source_extensions = [".cpp", ".cc", ".cxx", ".c", ".h", ".hpp"]

# 定义函数遍历目录并获取所有C++文件
def get_source_files(directory):
    source_files = {}
    for root, dirs, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in source_extensions):
                rel_dir = os.path.relpath(root, directory).replace("\\", "/")
                file_path = os.path.join(rel_dir, file).replace("\\", "/")
                if rel_dir == ".":
                    rel_dir = ""
                if rel_dir not in source_files:
                    source_files[rel_dir] = []
                source_files[rel_dir].append(file_path)
    return source_files

# 生成CMakeLists.txt文件
def generate_cmake(directory, project_name):
    source_files = get_source_files(directory)
    with open(os.path.join(directory, "CMakeLists.txt"), "w") as cmake_file:
        cmake_file.write(f"cmake_minimum_required(VERSION 3.10)\n")
        cmake_file.write(f"project({project_name})\n\n")
        
        all_files = []
        for group, files in source_files.items():
            cmake_file.write(f"source_group(\"{group}\" FILES\n")
            for source_file in files:
                cmake_file.write(f"    ${{CMAKE_CURRENT_LIST_DIR}}/{source_file}\n")
                all_files.append(source_file)
            cmake_file.write(")\n\n")
        
        cmake_file.write("set(SOURCE_FILES\n")
        for source_file in all_files:
            cmake_file.write(f"    ${{CMAKE_CURRENT_LIST_DIR}}/{source_file}\n")
        cmake_file.write(")\n\n")
        
        cmake_file.write("add_executable(${PROJECT_NAME} ${SOURCE_FILES})\n")

# 主程序
if __name__ == "__main__":
    directory = os.getcwd()  # 当前目录
    project_name = "MyProject"  # 项目名称
    generate_cmake(directory, project_name)
    print(f"CMakeLists.txt generated in {directory}")
