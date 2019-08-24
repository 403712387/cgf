import os
import stat
import shutil
import time
import sys

currentTime = time.localtime()
strTime = "%d-%02d-%02d %02d:%02d:%02d" % (currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min,currentTime.tm_sec)

# 服务名称
serviceName = "cfg"

# git信息
gitBranch = "unknown"
gitCommitId = "unknown"

# 编译类型，编译debug版本还是release版本
compileType = "release"

# 编译器，支持g++和clang
compiler = "g++"

#------------------------函数的定义-------------------------#

#清理
def cleanFiles(path):
    if not os.path.exists(path):
        return

    shutil.rmtree(path)

#解析参数
def parseArgs():
    global compileType, compiler

    if "debug" in sys.argv:
        compileType = "debug"
    else:
        compileType = "release"

    if "clang++" in sys.argv:
        compiler = "clang++"
    else:
        compiler = "g++"

#获取git的信息（获取当前分支以及commit id）
def getGitInfo():
    global gitBranch, gitCommitId

    gitDir = "../.git"

    #获取分支信息
    branchFile = os.path.join(gitDir, "HEAD")
    if os.path.exists(branchFile):
        with open(branchFile, "r") as f:
            line = f.readline()
            line = line.strip()
            splits = line.split("/")
            if len(splits) > 0:
                gitBranch = splits[-1]

    # 获取commit id
    commitIdFile = os.path.join(gitDir + "/refs/heads", gitBranch)
    if os.path.exists(commitIdFile):
        with open(commitIdFile) as f:
            line = f.readline()
            line = line.strip()
            gitCommitId = line

#编译各个模块
def compileModules():
    global serviceName

    compileSuccessful = True

    # 创建软连接
    libraryPath = ["../thirdparty/lib/linux64/Qt"]
    for library in libraryPath:
        createSymbolLink(library)

    try:
        projectFile = "../src/" + serviceName + ".pro"
        if not os.path.exists(projectFile):
            print("not find project file %s"%projectFile)
            raise Exception("not find project file %s"%projectFile)

        with open(projectFile, "r") as file:
            for lineData in file:
                if lineData.find("./") <= 0:
                    continue

                lineData = lineData.replace("\\", "")
                lineData = lineData.strip()
                (path, file) = os.path.split(lineData)
                path = path.replace("./", "../src/")
                if not compileOneModule(path, file):
                    print("compile %s %s fail" % (path,projectFile))
                    raise Exception("compile  module fail")

    except:
        compileSuccessful = False
    finally:
        # 删除软连接
        for library in libraryPath:
            removeSymbolLink(library)

    return compileSuccessful

#编译一个模块
def compileOneModule(modulePath, module):
    global gitBranch, gitCommitId, compileType, compiler

    print("\n------------compile module %s-------------"%(module))
    gitBranchMacro = "GIT_BRANCH=%s"%(gitBranch)
    gitCommitMacro = "GIT_COMMIT_ID=%s"%(gitCommitId)
    if not os.path.exists(modulePath):
        print("compile module fail, not find module directory, module path:%s, module name:%s"%(modulePath, module))
        return False

    moduleProFile = os.path.join(modulePath, module + ".pro")
    if not os.path.exists(moduleProFile):
        print("compile module fail, not find pro file, module path:%s, module name:%s"%(modulePath, module))
        return False

    # 删除原来的makefile
    makefileName = os.path.join(modulePath, "Makefile")
    if os.path.exists(makefileName):
        os.remove(makefileName)

    # 生成新的makefile
    os.system("qmake DEFINES+=%s DEFINES+=%s QMAKE_CXX=%s QMAKE_LINK=%s CONFIG+=%s -o %s %s"%(gitBranchMacro, gitCommitMacro, compiler, compiler, compileType, makefileName, moduleProFile))
    if not os.path.exists(makefileName):
        print("qmake fail, module name:" + module + ", not find makefile")
        return False

    currentPath = os.getcwd()
    os.chdir(modulePath)
    os.system("make clean")
    if os.system("make -j 16 ") != 0:
        os.chdir(currentPath)
        print("---------compile module " + module + ", please check code---------")
        return False

    os.chdir(currentPath)
    return True

#拷贝第三方库
def copyThirdpartyLibrarys():
    global baseModuleName, businessModuleName, serviceName

    sourceDir = "../thirdparty/lib/linux64"
    destinationDir = "../src/bin/" + serviceName + "/lib"
    if not os.path.exists(destinationDir):
        os.makedirs(destinationDir)

    copyFiles(sourceDir, destinationDir)

#拷贝配置文件/脚本文件
def copyConfigFile():
    global serviceName

    sourceDir = "../config"
    destinationDir = "../src/bin/" + serviceName
    copyFiles(sourceDir, destinationDir)

    # config.json文件放在config目录下
    sourceFile = "../src/bin/" + serviceName + "/config.json"
    destinationDir = "../src/bin/" + serviceName + "/config"
    copyFiles(sourceFile, destinationDir)
    os.remove(sourceFile)

#处理启动脚本中的库的路径
def processStartScript():
    global serviceName

    startScript = "../src/bin/" + serviceName + "/start.sh"
    if not os.path.exists(startScript):
        print("process start.sh fail, not find start.sh")
        return

    # 遍历lib库下的所有目录
    libPath = "../src/bin/" + serviceName + "/lib"
    files = os.listdir(libPath)
    libDirs = []
    for file in files:
        if os.path.isdir(os.path.join(libPath, file)):
            libDirs.append("./lib/" + file)

            #添加子目录
            for subFile in os.listdir(libPath + "/" + file):
                if os.path.isdir(os.path.join(libPath + "/" + file, subFile)):
                    libDirs.append("./lib/" + file + "/" + subFile)

    libsPath = ":".join(libDirs)
    libsPath = "export LD_LIBRARY_PATH=" + libsPath + "\n"

    #读取启动脚本的内容，并替换库路径
    startScriptData = ""
    with open(startScript, "r")  as file:
        for lineData in file:
            if lineData.find("LD_LIBRARY_PATH") > 0:
                startScriptData += libsPath
                continue

            startScriptData += lineData

    # 向启动脚本中写入内容
    with open(startScript, "w") as file:
            file.write(startScriptData)

#修改文件的权限
def processFilePromission(path):
    files = os.listdir(path)
    for file in files:
        fileName = os.path.join(path, file)
        if not os.path.isfile(fileName):
            continue

        #对于sh结束的文件，修改权限
        if fileName.endswith(".sh"):
            os.chmod(fileName, stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)

#拷贝文件或者目录
def copyFiles(source, destination):

    #复制文件(要注意权限和软连接这种情况)
    def copyFile(sourceFile, destDir):
        if not os.path.exists(sourceFile):
            return

        if not os.path.exists(destDir):
            os.makedirs(destDir)

        if os.path.islink(sourceFile):  #复制软连接
            currentPath = os.getcwd()
            symbolLink = os.readlink(sourceFile)
            os.chdir(destDir)
            os.symlink(symbolLink, os.path.basename(sourceFile))
            os.chdir(currentPath)
        elif os.path.isfile(sourceFile):    #复制文件
            with open(sourceFile, "rb") as input:
                with open(os.path.join(destDir, os.path.basename(sourceFile)), "wb") as output:
                    output.write(input.read())
        os.chmod(os.path.join(destDir, os.path.basename(sourceFile)), os.stat(sourceFile).st_mode)

    if not os.path.exists(source):
        print("copy %s to %s fail, not find %s"%(source, destination, source))
        return

    # 目标文件夹一定要存在
    if not os.path.exists(destination):
        os.makedirs(destination)

    if os.path.isdir(source):   #复制整个目录下的文件
        for path, directorys, files in os.walk(source):
            subPath = path[len(source): ]

            # 创建目录
            if subPath.startswith("/"):
                subPath = subPath[1:]
            destinationPath = os.path.join(destination, subPath)
            if not os.path.exists(destinationPath):
                os.makedirs(destinationPath)

            # 复制目录下中的文件
            for file in files:
                copyFile(os.path.join(path, file), destinationPath)
    elif os.path.isfile(source):    # 复制单个文件
        copyFile(source, destination)

#创建符号链接
def createSymbolLink(path):
    if not os.path.exists(path):
        return

    files = os.listdir(path)
    for file in files:
        filename = os.path.join(path, file)
        if os.path.isdir(filename) or os.path.islink(filename) or filename.endswith(".so"):
            continue

        index = file.find(".so")
        if index <= 0:
            continue

        symbolName = file[:index + 3]
        if os.path.exists(os.path.join(path, symbolName)):
            continue

        currentPath = os.getcwd()
        os.chdir(path)
        os.symlink(file, symbolName)
        os.chdir(currentPath)

#删除符号链接
def removeSymbolLink(path):
    if not os.path.exists(path):
        return

    files = os.listdir(path)
    for file in files:
        if os.path.islink(os.path.join(path, file)):
            os.remove(os.path.join(path, file))

#修改脚本中的结束符，\r\n换为\n
def formatLineBrak():
    global serviceName

    fileNames = ["../src/bin/" + serviceName + "/start.sh", "../src/bin/" + serviceName + "/stop.sh"]
    for fileName in fileNames:
        if not os.path.exists(fileName):
            continue

        fileData = ""
        with open(fileName, "r") as file:
            for lineData in file:
                lineData = lineData.replace("\r\n", "\n")
                fileData += lineData

            # 向启动脚本中写入内容
        with open(fileName, "w")  as file:
            file.write(fileData)

#构建服务
def buildService():
    global serviceName
    outputDir = "../src/bin/" + serviceName
    serviceDir = "./" + serviceName
    parseArgs()
    cleanFiles(outputDir)
    cleanFiles(serviceDir)
    getGitInfo()

    #编译各个模块
    if not compileModules():
        print("\n--------------compile fail at %s--------------" % (strTime))
        return -1

    #拷贝文件
    copyThirdpartyLibrarys()
    copyConfigFile()

    #处理启动脚本中
    processStartScript()
    formatLineBrak()

    #修改文件的权限
    processFilePromission(outputDir)

    #移动到当前目录
    print("move dir %s to %s"%(outputDir, serviceDir))
    copyFiles(outputDir, serviceDir)
    print("\n--------------compile successful at %s--------------"%(strTime))
    return 0

#------------------------函数的调用-------------------------#
buildService()