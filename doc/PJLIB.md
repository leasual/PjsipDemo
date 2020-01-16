## PJLIB

要理解好PJSIP，就不得不先说说PJLIB，PJLIB算的上是这个库中最基础的库，正是这个库的优美实现，才让PJSIP变得如此优越。

PJLIB提供了一系列特征，涉及到：

- 非动态内存分配（No Dynamic Memory Allocations）
- OS抽象[Operating System Abstraction]
- 底层的网络相关IO(Low-Level Network I/O)
- 时间管理[Timer Management]
- 各种数据结构(Various Data Structures)
- 异常处理(Exception Construct)
- LOG机制(Logging Facility)
- 随机数以及GUID的产生(Random and GUID Generation)

### 特征

#### 非动态内存分配

实现了内存池，获取内存是从与分配的内存池中获取，高性能程序多会自己构造内存池，后面我们会解释该内存池的使用以及基本的原理。根据作者的比较，是常规的 malloc()/free()函数的30倍。

#### OS抽象

涉及到：

- 线程(Threads)
- 线程本地存储[Thread Local Storage.]
- 互斥[Mutexes.]
- 信号灯[Semaphores.]
- 原子变量[Atomic Variables.]
- 临届区[Critical sections.]
- 锁对象[Lock Objects.]
- 事件对象[Event Object.]
- 时间管理[Time Data Type and Manipulation.]
- 高解析的时间戳[High Resolution Timestamp.]

#### 低层的网络相关IO

涉及到：

- Socket抽象[Socket Abstraction.]
- 网络地址解析[Network Address Resolution.]
- 实现针对Socket的select API[Socket select() API.]

#### 时间管理

主要涉及到两个部分，一个时定时器的管理，还有就是时间解析的精度(举例说来，就是能精确到哪个时间等级，比如 POSIX sleep(),就只能以秒为单位，而使用select()则可以实现毫秒级别的计时)

#### 各种数据结构

- 针对字符串的操作[String Operations]
- 数组辅助[Array helper]
- Hash表[Hash Tabl]
- 链表[Linked List]
- 红黑平衡树[Red/Black Balanced Tree]

#### 异常处理

使用的是TRY/CATCH。

#### LOG机制



#### 随机数以及GUID的产生

GUID指的是"globally unique identifier"，一个标识。



### PJLIB的使用

#### 引入头文件

​	#include <pjlib.h>

​    当然，也可以选择：

​    \#include <pj/log.h>

​    \#include <pj/os.h>

#### 调用 pj_init()

#### 使用建议

- 不要使用ANSI C[Do NOT Use ANSI C]

  ANSI C并不会让程序具有最大的移植性，应该使用PJSIP库所提供的响应机制来实现你所需要的功能

- 使用pj_str_t取代C风格的字符串[Use pj_str_t instead of C Strings]

  原因之一是移植性，之二则是PJLIB内置的pj_str_t相关操作会更快(性能)

- 从内存池分配内存[Use Pool for Memory Allocations]

- 使用PJLIB的LOG机制做文字显示[Use Logging for Text Display]

#### 原理

##### 快速内存池(Fast Memory Pool)

**优点:**

- 不像其它内存池,允许分配不同尺寸的chunks.

- 快速

  内存chunks拥有O(1)的复杂度,并且操作仅仅是指针的算术运算,其间不需要使用锁住任何互斥量.

- 有效使用内存

  除了可能因为内存对齐的原因会浪费很少的内存外,内存的使用效率非常高.

- 可预防内存泄漏

  内存都是从内存池中获取的,就算你没有释放你获取的内存,只要你记得把内存池destroy,那么内存还是会还给系统.

- 内存泄漏更容易被跟踪

  内存是在指定的内存池中分配的,只要能很快定位到内存池,内存泄漏的侦测就方便多了.

- 设计上从内存池中获取内存这一操作是非线程安全的

  内存池被上层对象所拥有,线程安全应该由上层对象去保证

- 内存池的行为像C++中的new的行为,当内存池获取内存chunks会抛出PJ_NO_MEMORY_EXCEPTION异常,当然,因为支持异常处理,也可以使用其它方式让上层程序灵活的定义异常的处理

- 可以在后端使用任何的内存分配器.

**告诫：**

- 使用合适的大小来初始化内存池

- 内存池只能增加,而不能被缩小(shrink)

  因为内存池没有函数把内存chunks释放还给系统,这就要去内存池的构造者和使用者明确使用内存

##### 内存池的使用(Using Memory Pool)

- 创建内存池工厂[Create Pool Factory]

  Caching Pool Factory，内存池工厂的初始化使用函数 pj_caching_pool_init()

- 创建内存池(Create The Pool)

  使用 pj_pool_create()，其参数分别为**内存工厂**(Pool Factory),**内存池的名字**(name),**初始时的大小**以及**增长时的大小**.

- 根据需要分配内存(Allocate Memory as Required)

  可以使用pj_pool_alloc(), pj_pool_calloc(), 或pj_pool_zalloc()从指定的内存池根据需要去获取内存

- Destroy内存池(Destroy the Pool)

  预分配的内存还给系统

- Destroy 内存池工厂(Destroy the Pool Factory)

#### 示例

```c
#include <pjlib.h> 
#define THIS_FILE    "pool_sample.c" 

static void my_perror(const char *title, pj_status_t status) 
{
    char errmsg[PJ_ERR_MSG_SIZE];
    
    pj_strerror(status, errmsg, sizeof(errmsg)); 
    PJ_LOG(1,(THIS_FILE, "%s: %s [status=%d]", title, errmsg, status)); 
}

static void pool_demo_1(pj_pool_factory *pfactory) 
{
    unsigned i; 
    pj_pool_t *pool;
    
    // Must create pool before we can allocate anything
    pool = pj_pool_create(pfactory,  // the factory
                          "pool1",   // pool's name
                          4000,      // initial size
                          4000,      // increment size
                          NULL);     // use default callback. 
    if (pool == NULL) {
        my_perror("Error creating pool", PJ_ENOMEM);
        return;
    }
    
    // Demo: allocate some memory chunks
    for (i=0; i<1000; ++i) {
        void *p;
        
        p = pj_pool_alloc(pool, (pj_rand()+1) % 512);
        // Do something with p 
        ...
        // Look! No need to free p!! 
    }
    
    // Done with silly demo, must free pool to release all memory.
    pj_pool_release(pool);
}

int main() 
{
    pj_caching_pool cp; 
    pj_status_t status;
    
    // Must init PJLIB before anything else
    status = pj_init();
    if (status != PJ_SUCCESS) {
        my_perror("Error initializing PJLIB", status);
        return 1; 
    } 
    
    // Create the pool factory, in this case, a caching pool, 
    // using default pool policy.
    pj_caching_pool_init(&cp, NULL, 1024*1024 ); 
    
    // Do a demo
    pool_demo_1(&cp.factory); 
    
    // Done with demos, destroy caching pool before exiting app. 
    pj_caching_pool_destroy(&cp); 
    
    return 0;
} 
```

