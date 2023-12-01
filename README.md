# 哈夫曼编解码文件，并进行加密
# 原理
![压缩原理](https://github.com/zxuu/Haffuman-compressed-files/blob/master/%E5%8E%9F%E7%90%86.png)
# 软件主界面

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/zhuye.png)

**选择一个纯英文文本进行压缩，压缩前的文件大小是2262kb**

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/selectfile.png)

### 压缩后文件大小是1285kb
* .带有reducecode的文件是压缩后的文件.
* .bin文件是加密后的文件（一不小心设置成了.bin文件，尽量改成其他的后缀文件，随意）
* .带code的文件是文本中出现的ASCII字符所对应的哈夫曼编码
* .带hfcode的文件是文本中所有的字符所对应的哈夫曼编码
* .把带hfcode的文件中的01每8个重新生成新的ASCII字符,就是压缩后的文本文件（乱码）.

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/yasuohou.png)

### 选择.bin文件进行解码，解码后多了一个new文件

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/jieya.png)

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/newfile.png)

### 解压后的文件内容和原先的一样！

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/newfilenei.png)

>**注**：中文文本压缩率不如纯英文文本高.

![image](https://github.com/zxuu/Haffuman-compressed-files/blob/master/images/Chinese.png)
