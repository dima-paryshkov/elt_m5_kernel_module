### Kernel module

This second practice is about writing a simple kernel module. After building and inserting it into the kernel, you can write the information to the buffer, and then read it in the field.

Compile: 

    make

After compiling, you must load it into the kernel using

    $ sudo insmod hello.ko 

After that you can check last step using 

    lsmod | grep hello

If it was successful, than you should see "hello" module in list


Finally, you can write in proc/hello using 

    $ echo "your_text" > /proc/hello

And you can read the data back using 

    $ cat /proc/hello

You can also clean your working directory of kernel files using

    make clean