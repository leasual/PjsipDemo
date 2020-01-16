## Endpoint

Endpoint（端点）表示pjsua库的一个实例。在应用程序中只能有一个pjsua库的实例，因此这个类是一个单例。

### 公共功能（函数）

- Endpoint()

  ​	默认构造函数 

- virtual ~Endpoint()

  ​	虚析构函数

- Version libVersion() const

  ​	得到库的版本

- void libCreate()

  ​	实例化pjsua应用程序。调用任何其他函数之前，应用程序必须调用此函数，以确保底层库被正确初始化。一旦此函数返回成功，应用程序必须在退出前调用 libDestroy()。

- pjsua_state libGetState() const //获取库状态。**返回库状态** 

- void libInit(*const* EpConfig) 

  ​	使用指定的设置初始化pjsua。所有设置都是可选的，并且在未指定配置时将使用默认值。请注意，在调用此函数之前必须调用create（）。 

  ​	**参数**  prm：EpConfig -端点配置

- void libStart() 

  ​	在所有的初始化完成后，调用该函数，以便可以做其他检查设置。应用程序可以在init（）之后的任何时间调用此函数。

- void libRegisterThread(*const* string &*name*)

  ​	将由外部或自身API创建的线程注册到库。请注意，每次调用此函数时，它将分配一些内存来存储线程描述，这只会在库被销毁时被释放。 

  ​	**参数** name 要分配给线程的可选名称

- bool libIsThreadRegistered()

  ​	检查该线程是否已经被注册到库中。需要注意的是此功能只适用于使用libRegisterThread()注册的库的主线程与工作线程和外部/自身线程。

- void libStopWorkerThreads()

  ​	停止所有工作线程

- int libHandleEvents(unsigned *msec_timeout*)

  ​	对pjsua进行事件轮询，如果需要，可以阻塞调用者线程指定的最大间隔（以毫秒为单位）。如果它在pjsua_config结构里配置了工作线程（*thread_cnt**域），*应用程序通常不需要调用这个函数，因为轮询将由这些工作线程来完成。如果EpConfig :: UaConfig :: mainThreadOnly启用，并从主线程调用此函数（默认情况下，主线程是调用 **libCreate()** 的线程），此功能也将扫描并在列表中运行任何挂起的作业。

  ​	**返回**		投票期间处理的事件数。负值表示错误，应用程序可以以（status = -return_value）方式检索错误。

  ​	**参数**		msec_timeout - 最长时间等待，以毫秒为单位。 

- void libDestroy(unsigned *prmFlags* = 0) 

  ​	销毁pjsua. 建议使用应用程序在调用此功能之前执行正常shutdown（例如从SIP服务器注销帐户，终止预订订阅和挂断主动调用）.但是如果发现有活动会话，此函数将执行所有这些操作以终止活动会话。此功能将等待（block）几秒钟等待远程的回复。如果没有跟踪它的状态，Application.可以多次安全地调用此函数。 

  ​	**参数**		prmFlags，  pjsua_destroy_flag 枚举值的组合

- string utilStrError(pj_status_t *prmErr*) 

  ​	检索指定状态代码的错误字符串。

  ​	参数 prmErr -错误代码。

- void utilLogWrite(int *prmLevel*, *const* string &*prmSender*, *const* string &*prmMsg*)

  ​	写一个日志消息

  ​	参数 prmLevel -日志详细程度（1-5）

  ​	prmSender -日志发送方。

  ​	prmMsg -日志消息。

- void utilLogWrite(LogEntry &*e*) 

  ​	写一个日志条目。

  ​	参数 e - 日志条目

- pj_status_t utilVerifySipUri(*const* string &*prmUri*) 

  ​	这是一个通用用函数，用于验证是否给出了有效的SIP URL。如果URL是有效的SIP / SIPS方案，则将返回PJ_SUCCESS

  ​	返回 PJ_SUCCESS成功，或相应的错误代码

  ​	**参考** [utilVerifyUri()

- pj_status_t utilVerifyUri(*const* string &*prmUri*) 

  ​	这是一个通用用函数，用于验证是否给出了有效的URL。与utilVerifySipUri（）不同，如果给出tel：URI，此函数将返回PJ_SUCCESS

  ​	**返回** PJ_SUCCESS成功，或相应的错误代码

  ​	PJ_SUCCESS on success, or the appropriate error code.

  ​	**参考** pjsua_verify_sip_url()

  ​	**参数** prmUri - URL字符串

- Token utilTimerSchedule(unsigned *prmMsecDelay*, Token *prmUserData*) 

  ​	设定具有指定间隔和用户数据的定时器。当间隔时间到时，将调用onTimer（）回调。注意，回调可能由不同的线程执行，具体取决于是否启用了工作线程

  ​	**返回** 令牌识别定时器，可以用utilTimerCancel（）取消定时器

  ​	**参数** prmMsecDelay -时间间隔，单位为毫秒

  ​	prmUserData - 任意用户数据，被回馈给应用程序在回调。

- void utilTimerCancel(Token *prmToken*) 

  ​	使用指定的定时器令牌取消先前指定的定时器

  ​	**参数** prmToken -从以前的utilTimerSchedule（）调用返回的计时器令牌。 

- void utilAddPendingJob(PendingJob **job*)  

  ​	注册要由主线程执行的待处理作业的实用程序。如果EpConfig :: UaConfig :: mainThreadOnly为false，该作业将立即执行。

  ​	参数 job 工作类 

- IntVector utilSslGetAvailableCiphers() 

  ​	获取SSL / TLS后端支持的密码列表。

- void natDetectType(void) 

  ​	这是一个在这个端点前面检测NAT类型的通用函数，一旦成功调用，此函数将异步完成，并在onNatDetectionComplete（）中报告结果.在检测到NAT并调用回调后，应用程序可以通过调用natGetType（）获取检测到的NAT类型。应用程序还可以在稍后再次调用natDetectType（）来执行NAT检测。注意，必须启用STUN才能成功运行此功能。

- pj_stun_nat_type natGetType() 

  ​	获取natDetectType（）函数检测到的NAT类型。

  ​	natDetectType()已成功完成并且已调用 onNatDetectionComplete() 回调函数后，此函数将仅返回有用的NAT类型。

  ​	异常：如果在检测过程中调用此函数，将引发PJ_EPENDING异常。

- void natUpdateStunServers(*const* StringVector &*prmServers*, bool *prmWait*)

  ​	更新STUN服务器列表。libInit() 必须在调用这个函数之前调用。

  ​	**参数**prmServers - STUN服务器数组尝试。端点将尝试解析并联系每个STUN服务器条目，直到找到可用的条目。每个条目可能是域名，主机名，IP地址，并且可能包含可选的端口号。例如：

  ​	“pjsip.org”（域名）

  ​	“sip.pjsip.org”（主机名）

  ​	“pjsip.org:33478”（域名和非标准端口号）

  ​	“10.0.0.1:3478”（IP地址和端口号）

  ​	prmWait - 指定函数是否应该阻塞，直到得到结果。在这种情况下，函数将在分辨率完成时阻塞，并且在该函数返回之前调用onNatCheckStunServersComplete()

- void natCheckStunServers(*const* StringVector &*prmServers*, bool *prmWait*, Token *prmUserData*)

  ​	辅助函数，用于解析和联系每个STUN服务器条目（依次）以查找哪个可用。须先调用libInit()

  ​	**参考**   natCancelCheckStunServers()

  ​	**参数** prmServers - STUN服务器数组尝试。端点将尝试解析并联系每个STUN服务器条目，直到找到可用的条目。每个条目可能是域名，主机名，IP地址，并且可能包含可选的端口号。例如：

  ​	*“pjsip.org”**（域名）***

  ​	*“sip.pjsip.org”**（主机名）*

  ​	*“pjsip.org:33478”**（域名和非标准端口号）*

  ​	*“10.0.0.1:3478”**（**IP**地址和端口号）*

  ​	 *prmWait* *-* *指定函数是否应该阻塞，直到得到结果。在这种情况下，函数将在分辨率完成时阻塞，并且在该函数返回之前调用回调函数。**prmUserData* *-* 任意用户数据要在回调中传回应用程序。 

- void **natCancelCheckStunServers**(Token *token*, bool *notify_cb* = false)

  ​	取消与指定令牌匹配的待决STUN分辨率。

  ​	异常：如果没有匹配的PJ_ENOTFOUND或其他错误。

  ​	**参数**token -令牌匹配。 这个令牌给了 natCheckStunServers()

  ​	notify_cb -布尔值，用于控制是否为已取消的方案调用回调。 当调用回调时，结果中的状态将被设置为PJ_ECANCELLED。

- TransportId **transportCreate** （ pjsip_transport_type_e *type* ， *const* TransportConfig ＆ *cfg* ） 

  ​	根据指定的设置创建并启动新的SIP传输。

  ​	**返回**传输ID。

  ​	**参数**type -传输类型。cfg -传输配置。

- IntVector **transportEnum** （ ） 

  ​	枚举系统中当前创建的所有传输。

  ​	此函数将返回所有传输ID，然后应用程序可以调用 transportGetInfo() 函数来检索有关传输的详细信息。

  ​	**返回** 传输ID数组。

- TransportInfo **transportGetInfo** （ TransportId *id* ） 

  ​	获取有关传输的信息。

  ​	**返回** 交通信息

  ​	**参数** id - 运输ID。

- void **transportSetEnable** （ TransportId *id* ，bool *enabled* ） 

  ​	禁用传输或重新启用它。

  ​	默认情况下，传输始终在创建后启用。 禁用传输不一定关闭套接字，它只会丢弃传入的消息，并阻止传输被用于发送传出的消息。

  ​	**参数**id -传输ID。

  ​	enabled -启用或禁用传输。

- void **transportClose** （ TransportId *id* ） 

  ​	关闭传输。

  ​	系统将等待所有事务关闭，同时防止新用户使用传输，并在其使用次数达到零时关闭传输。

  ​	**参数**id -传输ID。

- void **transportShutdown**(TransportHandle *tp*)

  ​	启动此传输句柄的正常关闭程序。

  ​	在正常关闭初始化后，传输获取不到新的引用。 然而，当前使用传输的现有对象仍然可以使用该传输来发送和接收数据包。 所有的对象释放对该引用后传输将立即销毁。

  ​	注意：从回调 onTransportState() 获取句柄后，应用程序通常会使用此API。

  ​	**参数**tp -传输句柄。

- void **hangupAllCalls** （ void ） 

  ​	终止所有呼叫

  ​	这将启动所有当前活动呼叫的呼叫挂断。

- void **mediaAdd**(AudioMedia ＆ *media*)

  ​	将媒体添加到媒体列表。

  ​	**参数**media -要添加的媒体

- void **mediaRemove**(AudioMedia ＆ *media*)

  ​	从媒体列表中删除媒体。

  ​	**参数**media -要移除的媒体

- bool **mediaExists**(*const* AudioMedia ＆ *media*)const

  ​	检查媒体是否已添加到媒体列表。

  ​	**返回** 如果添加了媒体，则为真，否则为false。

  ​	**参数**media -要检查的媒体

- unsigned  mediaMaxPorts() mediaMaxPorts

  ​	获取媒体端口的最大数量

  ​	**返回** 会议桥中媒体端口的最大数量。

- unsigned  mediaActivePorts （ ） mediaActivePorts

  ​	获取桥活性介质端口的当前数目

  ​	**返回**会议桥中媒体端口的最大数量

- *const* AudioMediaVector ＆ **mediaEnumPorts**() **mediaEnumPorts**

  ​	枚举所有媒体端口。

  ​	**返回** 媒体端口列表。

- AudDevManager ＆ **audDevManager** （ ） 

  ​	获取音频设备管理器的实例。

  ​	**返回** 音频设备管理器。

- VidDevManager ＆ **vidDevManager** （ ） 

  ​	获取视频设备管理器的实例。

  ​	**返回** 视频设备管理器。

- *const* CodecInfoVector ＆ **codecEnum** （ ） 

  ​	枚举所有支持的编解码器在系统中。

  ​	**返回** 编解码器信息数组。

- void **codecSetPriority**(*const* string＆ *codec_id* ，pj_uint8_t *priority*)

  ​	更改编解码优先级

  ​	**参数**codec_id -编解码器ID，它是唯一标识编解码器的字符串（如“speex / 8000”）。

  ​	priority -编解码器优先级0-255，其中0表示禁用编解码器。

- CodecParam **codecGetParam** （ *const* string＆ *codec_id* ） *const*

  ​	获取编解码器参数

  ​	**返回** 编解码器参数。 如果没有找到编解码器，则会抛出错误 。

  ​	**参数**codec_id - 编解码器ID。

- void **codecSetParam** （ *const* string＆ *codec_id* ， *const* CodecParam *param* ） 

  ​	设置编解码器参数

  ​	**参数** codec_id - 编解码器ID。

  ​	param -编解码器参数设置。 设置为NULL将编解码器参数重置为库默认设置。

- *const* CodecInfoVector ＆ **videoCodecEnum**()

  ​	枚举所有支持的视频编解码器在系统中。

  ​	**返回**  视频编解码器信息数组。

- void **videoCodecSetPriority**(*const* string＆ *codec_id* ，pj_uint8_t *priority*)

  ​	更改视频编解码优先级

  ​	**参数**codec_id - 编解码器ID，其是唯一标识编解码器的字符串（例如“H263 / 90000”）。 有关详细信息，请参阅pjsua手册或pjmedia编解码器参考。

  ​	priority - 编解码器优先级0-255，其中0表示禁用编解码器。

- VidCodecParam **getVideoCodecParam** （ *const* string＆ *codec_id* ） *const*

  ​	获取视频编解码器参数。

  ​	**返回**

  ​	编解码器参数。 如果没有找到编解码器，则会抛出错误。

  ​	**参数**

  ​	codec_id -编解码器ID。

- void **setVideoCodecParam** （ *const* string＆ *codec_id* ， *const* VidCodecParam ＆ *param* ） 

  ​	设置视频编解码器参数。

  ​	**参数**

  ​	codec_id -编解码器ID。

  ​	param -编解码器参数设置。

- void **resetVideoCodecParam** （ *const* string＆ *codec_id* ） 

  ​	将视频编解码器参数重置为库默认设置。

  ​	**参数**

  ​	codec_id -编解码器ID。

- virtual void **onNatDetectionComplete** （ *const* OnNatDetectionCompleteParam ＆ *prm* ） 

  ​	端点完成使用 natDetectType() 启动的NAT类型检测时的回调。

  ​	**参数**

  ​	prm -包含检测结果的回调参数。

- virtual  void **onNatCheckStunServersComplete** （ *const* OnNatCheckStunServersCompleteParam ＆ *prm* ） 

  ​	当Endpoint完成执行调用 libInit() 或调用 natCheckStunServers() 或 natUpdateStunServers() 时启动的STUN服务器检查时的回调。

  ​	**参数**prm -回调参数

- virtual  void **onTransportState** （ *const* OnTransportStateParam ＆ *prm* ） 

  ​	传输状态发生变化时调用此回调。

  ​	**参数**prm -回调参数

- virtual  void **onTimer** （ *const* OnTimerParam ＆ *prm* ） 

  ​	计时器触发时回调。

  ​	计时器由 utilTimerSchedule() 调度。

  ​	**参数**prm -回调参数

- virtual  void **onSelectAccount**(OnSelectAccountParam ＆ *prm*)

  ​	应用程序可以使用此回调来覆盖用于处理传入消息的帐户。

  ​	最初，使用的帐户将由图书馆自动计算。 如果应用程序没有实现此回调，则该初始帐户将被使用，或者从此回调返回时应用程序设置无效的帐户。请注意，目前需要帐号分配的传入消息是INVITE，MESSAGE，SUBSCRIBE和未经请求的NOTIFY。 这个回调可以在SIP事件本身的回叫之前被调用，即：来电，寻呼机，订阅或者非请求事件。

  ​	**参数** prm - 回调参数

####  公共静态功能

- *static* Endpoint ＆ **instance** （ ） 

  检索端点的单例实例。

###  端点配置

Endpoint
struct pj::EpConfig
\#include <endpoint.hpp>

#### endpoint配置

##### 公共功能

- void readObject（const ContainerNode＆node）

  ​		从容器读取此对象。

  ​		参数

  ​		node - 要从中写入值的容器。

- void writeObject(ContainerNode＆node) 

  ​		将此对象写入容器。

  ​		参数

  ​		node - 要将值写入的容器。

##### 公有成员

- UaConfig uaConfig
  ​		UA配置
- LogConfig logConfig
  ​		记录配置。
- MediaConfig medConfig
  ​		媒体配置

###### 媒体

- `struct pj::``MediaConfig`

  此结构描述媒体配置，在调用Lib :: init（）时指定。从 pj :: PersistentObject 继承

###### 记录

- `struct pj::``LogConfig`

  记录配置，可以（可选）在调用Lib :: init（）时指定。从 pj :: PersistentObject 继承

- `*class* pj::``LogWriter`

  用于编写日志消息的界面应用程序可以继承此类并在 LogConfig 结构中提供它，以实现自定义日志写入工具。公共函数`virtual ~LogWriter`（）析构器virtual void `write`（*const* LogEntry＆*entry* ） = 写一个日志条目。

- `struct pj::``LogEntry`

  包含由 LogWriter 写入的日志条目的数据。

###### 用户代理

- `struct pj::``UaConfig`

  SIP用户代理相关设置。从pj :: PersistentObject继承

### 回调参数

- `struct pj::``OnNatDetectionCompleteParam`

  Endpoint :: onNatDetectionComplete()回调的参数。

- `struct pj::``OnNatCheckStunServersCompleteParam`

  对 Endpoint :: onNatCheckStunServersComplete() 回调的参数。

- `struct pj::``OnTimerParam`

  端点::的 OnTimer() 回调的参数 

- `struct pj::``OnTransportStateParam`

  端点:: onTransportState()回调的参数

- `struct pj::``OnSelectAccountParam`

  端点:: onSelectAccount() 回调的参数