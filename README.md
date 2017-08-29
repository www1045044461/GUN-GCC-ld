# GUN-GCC-ld
Those file teach you the GUN-GCC how to find function from lib*.so!
<!!根据一个bolg写的，在此声明>
目的：测试Linux GUN GCC环境中，使用动态库的寻找路径。
动态库主要遵循以下规则 在文件中的几个库 libA.so和libB2.so，并且当前目录下的libB.so就是libB1.so.libB2.so copy 到/usr/lib 目录下，并且更名
libB.so。
main.c中显式的调用了libA.so中的一个函数，libA.so在生成过程中指定引用本地目录下的libB.so。链接main时使用不同的方式观察，输出的结果。
执行以下指令：
1.gcc -shared -fPIC libb2.c -o libB2.so
2.sudo mv libB2.so /usr/lib/libB.so\
3.gcc -shared -fPIC libb1.c -o libB.so
#生成A的动态库文件
4.gcc -shared -fPIC liba.c -o libA.so -L. -lB
#链接主文件阶段
5 <1>gcc main.c -ldl -o ./a1.out
      ./a1.out 
  <2>gcc main.c -ldl -Wl，--rpath=. -o ./a2.out
      ./a2.out
  <3>gcc main.c -ldl -Wl，--rpath=.,--enable-new-dtags -o a3.out
      ./a3.out
  结果是第一次执行的是/usr/lib下的libB。
        第二次是 本地下的libB。
        第三次是 /usr/lib下的libB。
        
   结果分析：调用过程 main-->libA->libB
        main中默认是本地目录下的A，所以没有疑问，关键是两个不同的libB.so位置。
        通过一下几个命令观察elf文件的头信息。
        readelf libA.so -d
        readelf -d a1.out
        readelf -d a2.out
        readelf -d a3.out
        以下是规则原文：
        The linker uses the following search paths to locate required shared libraries:

           1.  Any directories specified by -rpath-link options.

           2.  Any directories specified by -rpath options.  The difference between -rpath and -rpath-link is that directories specified by -rpath
               options are included in the executable and used at runtime, whereas the -rpath-link option is only effective at link time.
               Searching -rpath in this way is only supported by native linkers and cross linkers which have been configured with the
               --with-sysroot option.

           3.  On an ELF system, for native linkers, if the -rpath and -rpath-link options were not used, search the contents of the environment
               variable "LD_RUN_PATH".

           4.  On SunOS, if the -rpath option was not used, search any directories specified using -L options.

           5.  For a native linker, search the contents of the environment variable "LD_LIBRARY_PATH".

           6.  For a native ELF linker, the directories in "DT_RUNPATH" or "DT_RPATH" of a shared library are searched for shared libraries needed
               by it. The "DT_RPATH" entries are ignored if "DT_RUNPATH" entries exist.

           7.  The default directories, normally /lib and /usr/lib.

           8.  For a native linker on an ELF system, if the file /etc/ld.so.conf exists, the list of directories found in that file.

           If the required shared library is not found, the linker will issue a warning and continue with the link.
