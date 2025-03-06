# StickyNote-Plus-Plus

### 这好像是一个便笺小软件。。

### 懒癌犯了，所以开源

### 如何编译？

使用cmake，设置好QT路径即可
```
-DQT_QMAKE_EXECUTABLE:FILEPATH=%{Qt:qmakeExecutable}
-DCMAKE_PREFIX_PATH:PATH=%{Qt:QT_INSTALL_PREFIX}
-DCMAKE_C_COMPILER:FILEPATH=%{Compiler:Executable:C}
-DCMAKE_CXX_COMPILER:FILEPATH=%{Compiler:Executable:Cxx}
```
### 第三方开源代码

项目中使用到了一些第三方的开源代码，并没有打包为动态库使用而是直接使用的源代码。

pugiXML： [pugiXML](https://github.com/zeux/pugixml)

Qxt qxtglobalshortcut qt5：[Qxt qxtglobalshortcut qt5](https://github.com/ddqd/qxtglobalshortcut5)

### 最后

