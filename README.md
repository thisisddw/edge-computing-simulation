## 运行环境
- Windows
- msys2
    - make
    - MinGW
- python
    - matplotlib
    - numpy

由于使用了[matplotlib-cpp](https://github.com/lava/matplotlib-cpp)进行绘图，必需保证python安装了matplotlib和numpy库。为了使g++在编译时能找到python相关的头文件和链接库，需要修改Makefile中的两行：
```
CPPFLAGS += -I$(INCLUDE_DIR) -I "C:\Program Files\Python38\include" -I "C:\Program Files\Python38\Lib\site-packages\numpy\core\include"
```
CPPFLAGS的两个include目录改成本地python目录下的相应位置。
```
LDFLAGS += -L "C:\Program Files\Python38\libs" -lpython38
```
LDFLAGS的链接目录也改为本地python目录的相应位置，"-lpython38"根据python版本修改。

## 如何使用

### 编译

在命令行使用make进行编译：

- make: 编译可执行文件
- make run: 编译并运行
- make clean: 清除编译产生的所有文件

### 修改模拟参数

/include/parameter.h包含所有参数。

### 如何试验新策略

- 从Agent类派生出新的子类，根据策略override act()和feedback()
- 从Experiment类派生出新的子类
    - 在构造函数中配置agents
    - override track()函数来追踪统计数据
- 参考/include/test/和/src/test.cpp