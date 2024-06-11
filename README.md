1、移植步骤

```bash
tar xzf flac-1.4.3.tar.gz
cd flac-1.4.3/
./autogen.sh
./configure --prefix=$PWD/_install --enable-static --disable-shared
make -j8
make install
tree _install/ -h
```

2、demo使用方法

编译

```bash
$ make clean && make
rm -rf pcm2flac flac2pcm out*
gcc main_pcm2flac.c -I./include -lFLAC -L./libs -lm -o pcm2flac
gcc main_flac2pcm.c -I./include -lFLAC -L./libs -lm -o flac2pcm
```

编码

```bash
$ ./pcm2flac 
Usage: 
         ./pcm2flac <in-pcm-file> <sample-rate> <bits per sample> <channels> <out-flac-file>
Examples: 
         ./pcm2flac ./audio/test_16000_16_1.pcm 16000 16 1 out1.flac
         ./pcm2flac ./audio/test_44100_16_2.pcm 44100 16 2 out2.flac
```

解码

```bash

```

3、参考文章

- [FLAC 格式详解_flac格式解析-CSDN博客](https://blog.csdn.net/ffgamelife/article/details/7893747) 
- [FLAC 格式详解 - 刘文涛 - 博客园](https://www.cnblogs.com/liuwt0911/articles/3730378.html) 

