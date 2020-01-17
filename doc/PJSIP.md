# PJSIP

PJSIP开源库中主要包含两部分，一部分是SIP协议栈（SIP消息处理），另一部分媒体流处理模块（RTP包的处理）。

### SIP协议栈模块

SIP(Session Initiation Protocol)会话发起协议，负责发起会话，但不负责发起后的会话的控制和描述。

 SIP协议栈这个模块，开源库由底层往上层做了各个层次的封装。

![](.\png\pjsip.png)

- pjlib

  最底层的，最基础的库，它实现的是平台抽象与框架（数据结构、内存分配、文件I/O、线程、线程同步等），是SIP协议栈的基石。其他所有与SIP相关的模块都是基于PJLIB来实现的。

- pjlib-util

  封装了一些常用的算法，例如MD5、CRC32等，除此之外封装了一些涉及到字符串、文件格式解析操作的API，例如XML格式解析。

- pjnath

  开源的 NAT-T 辅助库，解决NAT 穿越问题

- PJMEDIA

  开源的媒体栈

- pjsip-core

  SIP协议栈的核心，在该库中，包含了三个非常重要的模块，分别是SIP endpoint、SIP transaction module、SIP dialog module、transport layer。

- pjsip-simple

  SIP事件与出席框架，实现 Presence 和即时消息

- pjsip-ua

  SIP用户代理库。是INVITE会话的高层抽象，使用该套API比较容易创建一个SIP会话。此外该库还实现了SIP client的注册API。

- pjsua

  pjsua是PJSIP开源库中能够使用到的最高层次抽象API，该库是基于pjsip-ua及以下库做了高层次的分装。

#### SIP消息处理

PJSIP 协议栈内部包含多个SIP 消息处理层，从下往上依次 TRANSPORT层、ENDPOINT层、TRANSACTION层、UA层 和 Dialog层。**每个消息处理层以模块的形式注册到协议栈中**，开发者也可以编写并添加自己的消息处理模块，对SIP 消息进行解析或修改。

当TRANSPORT MANAGER 收到SIP 消息包时，会把该SIP EVENT 通知上层的ENDPOINT，而ENDPOINT 会找到对应的接收者，先把EVENT 传给TRANSACTION LAYER，然后再传给UA LAYER（**传递的顺序由每个模块的优先权决定**），如果 UA LAYER指定要处理TRANSACTION 的EVENT，TRANSACTION LAYER 也会把解析后的EVENT传给UA LAYER。

##### UA处理事件注册方式

```c
static pjsip_module mod_simpleua =
{
	NULL, NULL, /* prev, next. */ 
	{ "mod-simpleua", 12 }, /* Name. */ 
	-1, /* Id */ 
	PJSIP_MOD_PRIORITY_APPLICATION, /* Priority */ 
	NULL, /* load() */ 
	NULL, /* start() */ 
	NULL, /* stop() */ 
	NULL, /* unload() */ 
	&on_rx_request, /* on_rx_request() */ 
	NULL, /* on_rx_response() */ 
	NULL, /* on_tx_request. */ 
	NULL, /* on_tx_response() */ 
	NULL, /* on_tsx_state() */ 
};
```



#### SIP endpoint

SIP endpoint是整个协议栈模块的核心，一般来说，一个进程内只能创建一个SIP endpoint。因此SIP endpoint是基于PJSIP开发的应用程序内所有SIP objects的管理与拥有者。SIP endpoint主要承担了一下角色：

- 为所有的SIP对象管理内存池的分配与释放（在基于pjsip的应用程序中，动态内存的分配是在内存池基础上进行的）

  Endpoint 提供了两个函数来分配和释放内存池

  ```
  pjsip_endpt_create_poll()
  ```

  

- 接收来自于传输层的消息，并将消息分配到上层，这里的上层指的是图中的**SIP transaction module**、**SIP dialog module**、**application module**。优先级顺序是SIP transaction module > SIP dialog module > application module。如果消息被上层接收处理，则消息由接收的那层继续往上传递处理。例如，SIP endpoint收到的SIP消息，会先较低给SIP transaction module，如果SIP transaction module在transaction hash table中找到消息所对应的transaction，则SIP transaction module在完成相应的处理后，会将消息尝试继续往上传递；如果SIP transaction module在transaction hash table中没有找到消息所对应的transaction，则该SIP消息由SIP endpoint继续往上传递。当SIP消息不能被上层所有module处理，则该消息由SIP endpoint来做默认处理。

- SIP endpoint负责管理模块（module），module在这里是对该库进行扩展的一种方法，在代码里代表的是一个结构体数据，上面会定义module名字、优先级、以及一些函数指针。开发者可以自己定义一些优先级高于SIP transaction module的module来截获对SIP消息的处理。

- 它为所有对象和分发事件提供单个轮询函数。

  提供了一个函数叫做 pjsip_endpt_handle_events() 来检查时间和网络时间。

- 时间管理

  Endpoint通过控制一个定时器堆来管理定时器。所有组件的定时器的创建和管理都由 endpoint 进行。

  提供的函数有

  ```
  pjsip_endpt_schedule_timer();
  pjsip_endpt_cancel_timer();
  ```

  当endpoint的轮询函数被调用的时候，endpoint来检查时间的终结。

- 还有一个单一的 PJLIB 的 io 队列，对处理不完的网络时间放到队列中。

- 通讯协议管理模块，包括通讯协议的管理，以及对消息的解析，分发消息到各个模块。

#### transport layer

transport layer是sip消息的接收与发送模块，目前支持TCP、UDP、TLS三种方式。



### 媒体流处理模块

该模块包含两部分：

- media transport，负责接收媒体流

  目前支持 RTP(UDP)、SRTP(加密)、ICE(NAT穿透)

- media port，媒体端口框架

  该框架实现了各种媒体端口，每一个media port上定义各种操作（创建、销毁、get/put等），常用媒体端口有：File writer（记录媒体文件），File player（播放媒体文件）、stream port 、conference port（可以实现多方通话）、master port等。

![](.\png\pjsip_media.png)

从右往左，以此是media transport、stream port、conference port、sound device port、sound device 。前四个需要自己在程序里创建，最后一个sound device 是与sound device port相关联的，创建sound device port的时候便会关联到sound device。

驱动媒体流由左往右流动的“驱动器是”sound device port，该端口是通过sound device的硬件驱动不停向与它连接的media port实施/get or put fram 操作，从而媒体流得以流动。在媒体流处理模块中，像sound device port的端口，我们称为主动型端口或者驱动型端口。

上图中最重要的是stream port ，如果你使用了pjmedia库，则必少不了stream port 。在stream port 中，从接收RTP包的角度讲，RTP包会被做一下处理：

**decode RTP into frame** ---> **put each frame into jitter buffer** ---> **decode frame into samples with codec**

从发送RTP包的角度讲，除了包含媒体流数据的RTP包外，还会存在**keep alive UDP pakcet**。

为了能正常接收RTP流，我们需要为media transport提供轮训机制，通常我们使用SIP endpoint的I\O queue即可，这个是在创建media transport时通过参数设置的。





### 相关技术

#### 协议栈定时器

pjsip 中管理定时器的数据结构是堆，是采用小顶堆来实现的，该堆为一棵完全二叉树，该完全二叉树采用顺序结构（即是数组）实现。而非 sleep 。

```
pj_timer_heap_create()			创建定时器堆
pj_timer_heap_schedule()		调度定时器，插入到定时器堆中
pj_timer_heap_poll()			轮询堆的根元素，做超时比较，超时则移除并调用回调函数，进行超时操作
```

