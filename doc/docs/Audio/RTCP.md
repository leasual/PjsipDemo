## RTCP

RTCP全称是Real-time Control Protocol（实时控制协议），它也是IETF提出的一个标准，对应的RFC文档为RFC3551。

#### 功能

服务质量的监视与反馈、媒体间的同步。在RTP会话期间，各参与者周期性地（一般是5秒，应用层可以配）传送RTCP包，RTCP包中含有已发送的数据包的数量、丢失的数据包的数量、数据包到达的平均时间间隔等统计信息。

### 协议

#### 报文类型

RTCP协议处理机定义了五种类型的报文，它们完成接收、分析、产生和发送控制报文的功能，如下表所示：

| 类型 |            缩写表示            | 用途       |
| :--: | :----------------------------: | ---------- |
| 200  |       SR(Sender Report)        | 发送端报告 |
| 201  |      RR(Receiver Report)       | 接收端报告 |
| 202  | SDES(Source Description Items) | 源点描述   |
| 203  |              BYE               | 结束传输   |
| 204  |              APP               | 特定应用   |

**SR**用来使发送端周期的向所有接收端用多播方式进行报告。

**RR**用来使接收端周期性的向所有的点用多播方式进行报告。当参加者既发又收时就发SR，只收不发时就发RR。

**SDES**给出会话中参加者的描述，包括参加者的规范名(CNAME)。

**BYE**用来关闭一个数据流。

**APP**能够定义新的分组类型。前四种类型经常用到，APP类型很少用到。

#### 封装格式

![](.\png\RTCP封装格式.png)

1. 算length时，计算公式是length = size/4 -1。其中size是SR包的真实大小（单位是字节）。
2. 算周期内丢包率（fraction lost）时，是以定点小数形式表示，即 fraction lost = (周期内丢包数 << 8) / 周期内期望接收包数。
3. 算DLSR时，是以1/65536秒为单位。

软件实现RTCP协议时，一般是几种类型的RTCP包组成组合包，所以一般先要判断要发几种类型的包。

1. 当处于SendReceive模式时，要发SR/SDES包，如果要停止通话，还要发BYE包；

2. 当处于ReceiveOnly模式时，要发RR/SDES包；如果要停止通话，还要发BYE包。

3. RTCP中有RTP包的统计，所以实现RTCP前要先在RTP中把相关的统计做好，同时还要做好sequence number的管理等。

实现RTCP时发送方先要实现SR或者RR包，然后是SDES包，如果是停止通话，还要加上BYE包。实现每种RTCP包时是把相应的字段值填好，然后再把包头填好。这些包都实现后拼在一起形成组合包，然后通过UDP socket发送到网络中去。接收方收到RTCP组合包后也是一个包一个包的去解析，然后把相应的信息报告给上层。

