1、移植步骤

源码下载地址：[https://github.com/xiph/flac/releases](https://github.com/xiph/flac/releases) 

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
Usage: 
         ./flac2pcm <in-flac-file> <out-pcm-file>
Examples: 
         ./flac2pcm ./audio/out1.flac out_16000_16_1.pcm
         ./flac2pcm ./audio/out2.flac out_44100_16_2.pcm
```

3、参考文章

- [FLAC 格式详解_flac格式解析-CSDN博客](https://blog.csdn.net/ffgamelife/article/details/7893747) 
- [FLAC 格式详解 - 刘文涛 - 博客园](https://www.cnblogs.com/liuwt0911/articles/3730378.html) 

4、demo使用日志

编码

如果是双声道，那么一个采样点就包含16bit*2channels等于4个字节，单通道就是2个字节（16bit），那么，文件大小/一个采样点的字节数=总的采样点数。

另外，从日志里看，这里1帧（frame）是由4096个采样点编码而成，那么我们程序里每次读取1024个采样点传递进去并不是直接进行编码的。

```
$ ls -l audio/
total 6088
-rw-rw-r-- 1 lin lin  437838 6月  11 21:10 out1.flac
-rw-rw-r-- 1 lin lin 1621374 6月  11 21:10 out2.flac
-rw-rw-r-- 1 lin lin  640000 6月  11 16:27 test_16000_16_1.pcm
-rw-rw-r-- 1 lin lin 3528000 6月  11 20:23 test_44100_16_2.pcm
$ 
$ ./pcm2flac ./audio/test_16000_16_1.pcm 16000 16 1 out1.flac
It will encode a PCM file as [file size: 640000] [total samples: 320000] [sample rate: 16000] - [bits per sample: 16] - [channels: 1] !
wrote 5318 bytes, 4096/320000 samples, 2/79 frames
wrote 9345 bytes, 8192/320000 samples, 2/79 frames
wrote 13453 bytes, 12288/320000 samples, 3/79 frames
wrote 17821 bytes, 16384/320000 samples, 4/79 frames
wrote 22198 bytes, 20480/320000 samples, 5/79 frames
wrote 27175 bytes, 24576/320000 samples, 6/79 frames
wrote 33137 bytes, 28672/320000 samples, 7/79 frames
wrote 39056 bytes, 32768/320000 samples, 8/79 frames
wrote 44756 bytes, 36864/320000 samples, 9/79 frames
wrote 50595 bytes, 40960/320000 samples, 10/79 frames
wrote 56557 bytes, 45056/320000 samples, 11/79 frames
wrote 63043 bytes, 49152/320000 samples, 12/79 frames
wrote 69089 bytes, 53248/320000 samples, 13/79 frames
wrote 75846 bytes, 57344/320000 samples, 14/79 frames
wrote 81958 bytes, 61440/320000 samples, 15/79 frames
wrote 87419 bytes, 65536/320000 samples, 16/79 frames
wrote 92602 bytes, 69632/320000 samples, 17/79 frames
wrote 97864 bytes, 73728/320000 samples, 18/79 frames
wrote 103342 bytes, 77824/320000 samples, 19/79 frames
wrote 108532 bytes, 81920/320000 samples, 20/79 frames
wrote 113797 bytes, 86016/320000 samples, 21/79 frames
wrote 120079 bytes, 90112/320000 samples, 22/79 frames
wrote 125751 bytes, 94208/320000 samples, 23/79 frames
wrote 131919 bytes, 98304/320000 samples, 24/79 frames
wrote 137624 bytes, 102400/320000 samples, 25/79 frames
wrote 143249 bytes, 106496/320000 samples, 26/79 frames
wrote 149302 bytes, 110592/320000 samples, 27/79 frames
wrote 155021 bytes, 114688/320000 samples, 28/79 frames
wrote 160210 bytes, 118784/320000 samples, 29/79 frames
wrote 165215 bytes, 122880/320000 samples, 30/79 frames
wrote 170107 bytes, 126976/320000 samples, 31/79 frames
wrote 174884 bytes, 131072/320000 samples, 32/79 frames
wrote 179605 bytes, 135168/320000 samples, 33/79 frames
wrote 184281 bytes, 139264/320000 samples, 34/79 frames
wrote 188811 bytes, 143360/320000 samples, 35/79 frames
wrote 193357 bytes, 147456/320000 samples, 36/79 frames
wrote 197303 bytes, 151552/320000 samples, 37/79 frames
wrote 201058 bytes, 155648/320000 samples, 38/79 frames
wrote 207450 bytes, 159744/320000 samples, 39/79 frames
wrote 214081 bytes, 163840/320000 samples, 40/79 frames
wrote 220053 bytes, 167936/320000 samples, 41/79 frames
wrote 225452 bytes, 172032/320000 samples, 42/79 frames
wrote 230626 bytes, 176128/320000 samples, 43/79 frames
wrote 237118 bytes, 180224/320000 samples, 44/79 frames
wrote 242759 bytes, 184320/320000 samples, 45/79 frames
wrote 249391 bytes, 188416/320000 samples, 46/79 frames
wrote 255973 bytes, 192512/320000 samples, 47/79 frames
wrote 261699 bytes, 196608/320000 samples, 48/79 frames
wrote 267705 bytes, 200704/320000 samples, 49/79 frames
wrote 273581 bytes, 204800/320000 samples, 50/79 frames
wrote 279489 bytes, 208896/320000 samples, 51/79 frames
wrote 285062 bytes, 212992/320000 samples, 52/79 frames
wrote 291205 bytes, 217088/320000 samples, 53/79 frames
wrote 297068 bytes, 221184/320000 samples, 54/79 frames
wrote 302728 bytes, 225280/320000 samples, 55/79 frames
wrote 307865 bytes, 229376/320000 samples, 56/79 frames
wrote 313116 bytes, 233472/320000 samples, 57/79 frames
wrote 318420 bytes, 237568/320000 samples, 58/79 frames
wrote 323566 bytes, 241664/320000 samples, 59/79 frames
wrote 328424 bytes, 245760/320000 samples, 60/79 frames
wrote 333121 bytes, 249856/320000 samples, 61/79 frames
wrote 337973 bytes, 253952/320000 samples, 62/79 frames
wrote 344265 bytes, 258048/320000 samples, 63/79 frames
wrote 350878 bytes, 262144/320000 samples, 64/79 frames
wrote 357328 bytes, 266240/320000 samples, 65/79 frames
wrote 363638 bytes, 270336/320000 samples, 66/79 frames
wrote 370207 bytes, 274432/320000 samples, 67/79 frames
wrote 375974 bytes, 278528/320000 samples, 68/79 frames
wrote 381269 bytes, 282624/320000 samples, 69/79 frames
wrote 386678 bytes, 286720/320000 samples, 70/79 frames
wrote 392988 bytes, 290816/320000 samples, 71/79 frames
wrote 400124 bytes, 294912/320000 samples, 72/79 frames
wrote 406608 bytes, 299008/320000 samples, 73/79 frames
wrote 413317 bytes, 303104/320000 samples, 74/79 frames
wrote 419545 bytes, 307200/320000 samples, 75/79 frames
wrote 425148 bytes, 311296/320000 samples, 76/79 frames
wrote 430510 bytes, 315392/320000 samples, 77/79 frames
wrote 437052 bytes, 319488/320000 samples, 78/79 frames
   state: FLAC__STREAM_ENCODER_OK
wrote 437838 bytes, 320000/320000 samples, 79/79 frames
encoding: succeeded
   state: FLAC__STREAM_ENCODER_UNINITIALIZED
./audio/test_16000_16_1.pcm -> out1.flac: Success!
$ 
$  ./pcm2flac ./audio/test_44100_16_2.pcm 44100 16 2 out2.flac
It will encode a PCM file as [file size: 3528000] [total samples: 882000] [sample rate: 44100] - [bits per sample: 16] - [channels: 2] !
wrote 6858 bytes, 4096/882000 samples, 2/216 frames
wrote 11921 bytes, 8192/882000 samples, 2/216 frames
wrote 18879 bytes, 12288/882000 samples, 3/216 frames
wrote 24793 bytes, 16384/882000 samples, 4/216 frames
wrote 31126 bytes, 20480/882000 samples, 5/216 frames
wrote 37532 bytes, 24576/882000 samples, 6/216 frames
wrote 43425 bytes, 28672/882000 samples, 7/216 frames
wrote 50210 bytes, 32768/882000 samples, 8/216 frames
wrote 56439 bytes, 36864/882000 samples, 9/216 frames
wrote 62129 bytes, 40960/882000 samples, 10/216 frames
wrote 69469 bytes, 45056/882000 samples, 11/216 frames
wrote 75834 bytes, 49152/882000 samples, 12/216 frames
wrote 82229 bytes, 53248/882000 samples, 13/216 frames
wrote 89065 bytes, 57344/882000 samples, 14/216 frames
wrote 95800 bytes, 61440/882000 samples, 15/216 frames
wrote 102536 bytes, 65536/882000 samples, 16/216 frames
wrote 109214 bytes, 69632/882000 samples, 17/216 frames
wrote 116265 bytes, 73728/882000 samples, 18/216 frames
wrote 124616 bytes, 77824/882000 samples, 19/216 frames
wrote 132645 bytes, 81920/882000 samples, 20/216 frames
wrote 140175 bytes, 86016/882000 samples, 21/216 frames
wrote 147662 bytes, 90112/882000 samples, 22/216 frames
wrote 154911 bytes, 94208/882000 samples, 23/216 frames
wrote 162076 bytes, 98304/882000 samples, 24/216 frames
wrote 169290 bytes, 102400/882000 samples, 25/216 frames
wrote 176480 bytes, 106496/882000 samples, 26/216 frames
wrote 183555 bytes, 110592/882000 samples, 27/216 frames
wrote 190913 bytes, 114688/882000 samples, 28/216 frames
wrote 198080 bytes, 118784/882000 samples, 29/216 frames
wrote 206022 bytes, 122880/882000 samples, 30/216 frames
wrote 213876 bytes, 126976/882000 samples, 31/216 frames
wrote 222047 bytes, 131072/882000 samples, 32/216 frames
wrote 230677 bytes, 135168/882000 samples, 33/216 frames
wrote 238791 bytes, 139264/882000 samples, 34/216 frames
wrote 246886 bytes, 143360/882000 samples, 35/216 frames
wrote 254563 bytes, 147456/882000 samples, 36/216 frames
wrote 263643 bytes, 151552/882000 samples, 37/216 frames
wrote 273869 bytes, 155648/882000 samples, 38/216 frames
wrote 282268 bytes, 159744/882000 samples, 39/216 frames
wrote 290151 bytes, 163840/882000 samples, 40/216 frames
wrote 297752 bytes, 167936/882000 samples, 41/216 frames
wrote 305124 bytes, 172032/882000 samples, 42/216 frames
wrote 312254 bytes, 176128/882000 samples, 43/216 frames
wrote 319121 bytes, 180224/882000 samples, 44/216 frames
wrote 325896 bytes, 184320/882000 samples, 45/216 frames
wrote 332538 bytes, 188416/882000 samples, 46/216 frames
wrote 339206 bytes, 192512/882000 samples, 47/216 frames
wrote 345880 bytes, 196608/882000 samples, 48/216 frames
wrote 352383 bytes, 200704/882000 samples, 49/216 frames
wrote 359824 bytes, 204800/882000 samples, 50/216 frames
wrote 367211 bytes, 208896/882000 samples, 51/216 frames
wrote 373769 bytes, 212992/882000 samples, 52/216 frames
wrote 381000 bytes, 217088/882000 samples, 53/216 frames
wrote 387656 bytes, 221184/882000 samples, 54/216 frames
wrote 394393 bytes, 225280/882000 samples, 55/216 frames
wrote 401500 bytes, 229376/882000 samples, 56/216 frames
wrote 408441 bytes, 233472/882000 samples, 57/216 frames
wrote 415171 bytes, 237568/882000 samples, 58/216 frames
wrote 423435 bytes, 241664/882000 samples, 59/216 frames
wrote 432680 bytes, 245760/882000 samples, 60/216 frames
wrote 440701 bytes, 249856/882000 samples, 61/216 frames
wrote 448100 bytes, 253952/882000 samples, 62/216 frames
wrote 455401 bytes, 258048/882000 samples, 63/216 frames
wrote 462701 bytes, 262144/882000 samples, 64/216 frames
wrote 472414 bytes, 266240/882000 samples, 65/216 frames
wrote 481221 bytes, 270336/882000 samples, 66/216 frames
wrote 489509 bytes, 274432/882000 samples, 67/216 frames
wrote 497310 bytes, 278528/882000 samples, 68/216 frames
wrote 504814 bytes, 282624/882000 samples, 69/216 frames
wrote 512669 bytes, 286720/882000 samples, 70/216 frames
wrote 520577 bytes, 290816/882000 samples, 71/216 frames
wrote 528375 bytes, 294912/882000 samples, 72/216 frames
wrote 536032 bytes, 299008/882000 samples, 73/216 frames
wrote 544172 bytes, 303104/882000 samples, 74/216 frames
wrote 552246 bytes, 307200/882000 samples, 75/216 frames
wrote 559632 bytes, 311296/882000 samples, 76/216 frames
wrote 566922 bytes, 315392/882000 samples, 77/216 frames
wrote 574093 bytes, 319488/882000 samples, 78/216 frames
wrote 580970 bytes, 323584/882000 samples, 79/216 frames
wrote 587300 bytes, 327680/882000 samples, 80/216 frames
wrote 593719 bytes, 331776/882000 samples, 81/216 frames
wrote 600181 bytes, 335872/882000 samples, 82/216 frames
wrote 606396 bytes, 339968/882000 samples, 83/216 frames
wrote 612662 bytes, 344064/882000 samples, 84/216 frames
wrote 618967 bytes, 348160/882000 samples, 85/216 frames
wrote 625201 bytes, 352256/882000 samples, 86/216 frames
wrote 631440 bytes, 356352/882000 samples, 87/216 frames
wrote 637761 bytes, 360448/882000 samples, 88/216 frames
wrote 643816 bytes, 364544/882000 samples, 89/216 frames
wrote 650384 bytes, 368640/882000 samples, 90/216 frames
wrote 656575 bytes, 372736/882000 samples, 91/216 frames
wrote 662745 bytes, 376832/882000 samples, 92/216 frames
wrote 669712 bytes, 380928/882000 samples, 93/216 frames
wrote 675969 bytes, 385024/882000 samples, 94/216 frames
wrote 682037 bytes, 389120/882000 samples, 95/216 frames
wrote 688092 bytes, 393216/882000 samples, 96/216 frames
wrote 694048 bytes, 397312/882000 samples, 97/216 frames
wrote 700636 bytes, 401408/882000 samples, 98/216 frames
wrote 706679 bytes, 405504/882000 samples, 99/216 frames
wrote 712382 bytes, 409600/882000 samples, 100/216 frames
wrote 717892 bytes, 413696/882000 samples, 101/216 frames
wrote 723301 bytes, 417792/882000 samples, 102/216 frames
wrote 728847 bytes, 421888/882000 samples, 103/216 frames
wrote 734156 bytes, 425984/882000 samples, 104/216 frames
wrote 739274 bytes, 430080/882000 samples, 105/216 frames
wrote 746880 bytes, 434176/882000 samples, 106/216 frames
wrote 756767 bytes, 438272/882000 samples, 107/216 frames
wrote 766891 bytes, 442368/882000 samples, 108/216 frames
wrote 775996 bytes, 446464/882000 samples, 109/216 frames
wrote 784668 bytes, 450560/882000 samples, 110/216 frames
wrote 792922 bytes, 454656/882000 samples, 111/216 frames
wrote 801492 bytes, 458752/882000 samples, 112/216 frames
wrote 809356 bytes, 462848/882000 samples, 113/216 frames
wrote 816866 bytes, 466944/882000 samples, 114/216 frames
wrote 824192 bytes, 471040/882000 samples, 115/216 frames
wrote 831114 bytes, 475136/882000 samples, 116/216 frames
wrote 837942 bytes, 479232/882000 samples, 117/216 frames
wrote 844678 bytes, 483328/882000 samples, 118/216 frames
wrote 852778 bytes, 487424/882000 samples, 119/216 frames
wrote 861523 bytes, 491520/882000 samples, 120/216 frames
wrote 870124 bytes, 495616/882000 samples, 121/216 frames
wrote 877961 bytes, 499712/882000 samples, 122/216 frames
wrote 885433 bytes, 503808/882000 samples, 123/216 frames
wrote 892823 bytes, 507904/882000 samples, 124/216 frames
wrote 900258 bytes, 512000/882000 samples, 125/216 frames
wrote 908603 bytes, 516096/882000 samples, 126/216 frames
wrote 918817 bytes, 520192/882000 samples, 127/216 frames
wrote 928288 bytes, 524288/882000 samples, 128/216 frames
wrote 937156 bytes, 528384/882000 samples, 129/216 frames
wrote 945565 bytes, 532480/882000 samples, 130/216 frames
wrote 953499 bytes, 536576/882000 samples, 131/216 frames
wrote 960935 bytes, 540672/882000 samples, 132/216 frames
wrote 968090 bytes, 544768/882000 samples, 133/216 frames
wrote 975740 bytes, 548864/882000 samples, 134/216 frames
wrote 985130 bytes, 552960/882000 samples, 135/216 frames
wrote 993246 bytes, 557056/882000 samples, 136/216 frames
wrote 1001069 bytes, 561152/882000 samples, 137/216 frames
wrote 1008631 bytes, 565248/882000 samples, 138/216 frames
wrote 1016261 bytes, 569344/882000 samples, 139/216 frames
wrote 1024244 bytes, 573440/882000 samples, 140/216 frames
wrote 1032388 bytes, 577536/882000 samples, 141/216 frames
wrote 1039786 bytes, 581632/882000 samples, 142/216 frames
wrote 1047190 bytes, 585728/882000 samples, 143/216 frames
wrote 1055030 bytes, 589824/882000 samples, 144/216 frames
wrote 1062862 bytes, 593920/882000 samples, 145/216 frames
wrote 1070573 bytes, 598016/882000 samples, 146/216 frames
wrote 1078334 bytes, 602112/882000 samples, 147/216 frames
wrote 1085813 bytes, 606208/882000 samples, 148/216 frames
wrote 1093283 bytes, 610304/882000 samples, 149/216 frames
wrote 1100145 bytes, 614400/882000 samples, 150/216 frames
wrote 1108373 bytes, 618496/882000 samples, 151/216 frames
wrote 1116195 bytes, 622592/882000 samples, 152/216 frames
wrote 1123450 bytes, 626688/882000 samples, 153/216 frames
wrote 1130454 bytes, 630784/882000 samples, 154/216 frames
wrote 1137457 bytes, 634880/882000 samples, 155/216 frames
wrote 1144441 bytes, 638976/882000 samples, 156/216 frames
wrote 1151327 bytes, 643072/882000 samples, 157/216 frames
wrote 1157846 bytes, 647168/882000 samples, 158/216 frames
wrote 1164613 bytes, 651264/882000 samples, 159/216 frames
wrote 1171889 bytes, 655360/882000 samples, 160/216 frames
wrote 1179003 bytes, 659456/882000 samples, 161/216 frames
wrote 1186194 bytes, 663552/882000 samples, 162/216 frames
wrote 1192913 bytes, 667648/882000 samples, 163/216 frames
wrote 1199511 bytes, 671744/882000 samples, 164/216 frames
wrote 1206029 bytes, 675840/882000 samples, 165/216 frames
wrote 1212386 bytes, 679936/882000 samples, 166/216 frames
wrote 1218772 bytes, 684032/882000 samples, 167/216 frames
wrote 1225041 bytes, 688128/882000 samples, 168/216 frames
wrote 1231422 bytes, 692224/882000 samples, 169/216 frames
wrote 1237651 bytes, 696320/882000 samples, 170/216 frames
wrote 1245053 bytes, 700416/882000 samples, 171/216 frames
wrote 1254712 bytes, 704512/882000 samples, 172/216 frames
wrote 1263045 bytes, 708608/882000 samples, 173/216 frames
wrote 1273075 bytes, 712704/882000 samples, 174/216 frames
wrote 1282054 bytes, 716800/882000 samples, 175/216 frames
wrote 1291524 bytes, 720896/882000 samples, 176/216 frames
wrote 1302024 bytes, 724992/882000 samples, 177/216 frames
wrote 1311828 bytes, 729088/882000 samples, 178/216 frames
wrote 1321254 bytes, 733184/882000 samples, 179/216 frames
wrote 1330171 bytes, 737280/882000 samples, 180/216 frames
wrote 1338987 bytes, 741376/882000 samples, 181/216 frames
wrote 1348433 bytes, 745472/882000 samples, 182/216 frames
wrote 1358436 bytes, 749568/882000 samples, 183/216 frames
wrote 1367462 bytes, 753664/882000 samples, 184/216 frames
wrote 1375580 bytes, 757760/882000 samples, 185/216 frames
wrote 1383306 bytes, 761856/882000 samples, 186/216 frames
wrote 1390483 bytes, 765952/882000 samples, 187/216 frames
wrote 1397478 bytes, 770048/882000 samples, 188/216 frames
wrote 1404200 bytes, 774144/882000 samples, 189/216 frames
wrote 1410618 bytes, 778240/882000 samples, 190/216 frames
wrote 1416946 bytes, 782336/882000 samples, 191/216 frames
wrote 1423099 bytes, 786432/882000 samples, 192/216 frames
wrote 1431069 bytes, 790528/882000 samples, 193/216 frames
wrote 1439434 bytes, 794624/882000 samples, 194/216 frames
wrote 1447284 bytes, 798720/882000 samples, 195/216 frames
wrote 1455480 bytes, 802816/882000 samples, 196/216 frames
wrote 1465232 bytes, 806912/882000 samples, 197/216 frames
wrote 1476564 bytes, 811008/882000 samples, 198/216 frames
wrote 1486541 bytes, 815104/882000 samples, 199/216 frames
wrote 1495732 bytes, 819200/882000 samples, 200/216 frames
wrote 1504701 bytes, 823296/882000 samples, 201/216 frames
wrote 1513423 bytes, 827392/882000 samples, 202/216 frames
wrote 1522877 bytes, 831488/882000 samples, 203/216 frames
wrote 1532922 bytes, 835584/882000 samples, 204/216 frames
wrote 1541847 bytes, 839680/882000 samples, 205/216 frames
wrote 1550105 bytes, 843776/882000 samples, 206/216 frames
wrote 1557923 bytes, 847872/882000 samples, 207/216 frames
wrote 1565203 bytes, 851968/882000 samples, 208/216 frames
wrote 1572329 bytes, 856064/882000 samples, 209/216 frames
wrote 1579009 bytes, 860160/882000 samples, 210/216 frames
wrote 1585557 bytes, 864256/882000 samples, 211/216 frames
wrote 1592689 bytes, 868352/882000 samples, 212/216 frames
wrote 1601002 bytes, 872448/882000 samples, 213/216 frames
wrote 1610714 bytes, 876544/882000 samples, 214/216 frames
wrote 1618694 bytes, 880640/882000 samples, 215/216 frames
   state: FLAC__STREAM_ENCODER_OK
wrote 1621374 bytes, 882000/882000 samples, 216/216 frames
encoding: succeeded
   state: FLAC__STREAM_ENCODER_UNINITIALIZED
./audio/test_44100_16_2.pcm -> out2.flac: Success!
```

解码

```
$ ls -l audio/
total 6088
-rw-rw-r-- 1 lin lin  437838 6月  11 21:10 out1.flac
-rw-rw-r-- 1 lin lin 1621374 6月  11 21:10 out2.flac
-rw-rw-r-- 1 lin lin  640000 6月  11 16:27 test_16000_16_1.pcm
-rw-rw-r-- 1 lin lin 3528000 6月  11 20:23 test_44100_16_2.pcm
$ 
$ ./flac2pcm ./audio/out1.flac out_16000_16_1.pcm
sample rate    : 16000 Hz
channels       : 1
bits per sample: 16
total samples  : 320000
decoding: succeeded
   state: FLAC__STREAM_DECODER_END_OF_STREAM
$ 
$ ./flac2pcm ./audio/out2.flac out_44100_16_2.pcm
sample rate    : 44100 Hz
channels       : 2
bits per sample: 16
total samples  : 882000
decoding: succeeded
   state: FLAC__STREAM_DECODER_END_OF_STREAM
```

