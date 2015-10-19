FIFO与PIPE的读写：
(1)对于PIPE或FIFO的write总是往末尾添加数据，对他们的read则总是从开头返回数据。如果对PIPE或FIFO调用lseek,那就返回ESPIPE错误
(2)一个文件描述符能以2中方式设置成非阻塞：(默认为阻塞)
? 调用open是可指定O_NONBLOCK标志
? 如果文件描述符已经打开，那么可以调用fcntl设置O_NONBLOCK标志(PIPE只能采用这种方式)
(3)读写规则：
阻塞(缺省设置):
只读open
? FIFO已经被只写打开：成功返回
? FIFO没有被只写打开：阻塞到FIFO被打开来写
只写open
? FIFO已经被只读打开：成功返回
? FIFO没有被只读打开：阻塞到FIFO被打开来读
从空PIPE或空FIFO中read
? FIFO或PIPE已经被只写打开：阻塞到PIPE或FIFO中有数据或者不再为写打开着
? FIFO或PIPE没有被只写打开：返回0(文件结束符)
write
? FIFO或PIPE已经被只读打开：
写入数据量不大于PIPE_BUF(保证原子性):有足够空间存放则一次性全部写入,没有则进入睡眠，直到当缓冲区中有能够容纳要写入的全部字节数时，才开始进行一次性写操作
写入数据量大于PIPE_BUF(不保证原子性):缓冲区一有空闲区域，进程就会试图写入数据，函数在写完全部数据后返回
? FIFO或PIPE没有被只读打开：给线程产生SIGPIPE(默认终止进程)

O_NONBLOCK设置:
只读open
? FIFO已经被只写打开：成功返回
? FIFO没有被只写打开：成功返回
只写open
? FIFO已经被只读打开：成功返回
? FIFO没有被只读打开：返回ENXIO错误
从空PIPE或空FIFO中read
? FIFO或PIPE已经被只写打开：返回EAGAIN错误
? FIFO或PIPE没有被只写打开：返回0(文件结束符)
write
? FIFO或PIPE已经被只读打开：
写入数据量不大于PIPE_BUF(保证原子性):有足够空间存放则一次性全部写入,没有则返回EAGAIN错误(不会部分写入)
写入数据量大于PIPE_BUF(不保证原子性):有足够空间存放则全部写入,没有则部分写入,函数立即返回
? FIFO或PIPE没有被只读打开：给线程产生SIGPIPE(默认终止进程)

PIPE或FIFO若干额外的规则：
? 如果请求读取的数据量多余当前可用的数据量，那么返回这些可用的数据
? 如果请求写入的数据字节数小于或等于PIPE_BUF,那么write操作保证是原子的(O_NONBLOCK标志的设置对原子性没有影响)
? 当对PIPE或FIFO最后一个关闭时，仍在该PIPE或FIFO上的数据将被丢弃

FIFO与PIPE的限制：
? 它们是半双工的(单向性),即数据只能在一个方向上流动。由进程A流向进程B或由进程B流向进程A。
? PIPE的读写端通过打开的文件描述符来传递,因此要通信的两个进程必须从它们的公共祖先那里继承PIPE文件描述符。FIFO可以实现无关进程间的通信。
? 一个进程在任意时刻打开的最大文件描述符个数OPEN_MAX(通过调用sysconf(_SC_OPEN_MAX)获得)
? 可原子地写往PIPE或FIFO的最大数据量PIPE_BUF(通常定义在limits.h)