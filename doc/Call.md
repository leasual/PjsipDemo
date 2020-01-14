## Call

### 公有函数

- Call（Account＆ *ACC*，INT *CALL_ID* = PJSUA_INVALID_ID ）

  ​	构造函数

- virtual  ~Call（）

  ​	析构

- CallInfo getInfo（） const

  ​	获取有关此呼叫的详细信息。

  ​	**返回**

  ​		呼叫信息。

- bool **isActive**（） const

  ​	检查此呼叫是否具有活动的INVITE会话，并且INVITE会话未被断开。

  ​	**返回**

  ​		如果call处于活动状态，则为真。

- INT **getId**（） const

  ​	获取与此呼叫相关联的PJSUA-LIB呼叫ID或索引。

  ​	**返回**

  ​		整数大于或等于零。

- bool **hasMedia**（） const

  ​	检查呼叫是否有活动的媒体会话。

  ​	**返回**

  ​		如果是，则为真。

- Media* **getMedia**（unsigned *med_idx* ） const

  ​	获取指定介质索引的介质。

  ​	**返回**

  ​		Media 或NULL（如果invalid 或inactive）

  ​	**参数**

  ​		med_idx - 媒体索引号

- pjsip_dialog_cap_status **remoteHasCap**（ INT *HTYPE*，const string *hname*，*const string token* ） const

  ​	检查远端（远程对等体）是否支持指定的功能。

  ​	**返回**

  ​		PJSIP_DIALOG_CAP_SUPPORTED如果显式支持指定的功能，请参阅pjsip_dialog_cap_status以获取更多信息。

  ​	**参数**

  ​		htype - 要检查的标题类型（pjsip_hdr_e），该值可以是：

  ​			PJSIP_H_ACCEPT

  ​			PJSIP_H_ALLOW

  ​			PJSIP_H_SUPPORTED

  ​		hname -如果htype指定了PJSIP_H_OTHER，则必须在此参数中提供头名称。否则，该值必须设置为空字符串（“”）。

  ​		token -检查的能力标记。例如，如果*HTYPE*是PJSIP_H_ALLOW，则*令牌*指定方法名称; 如果*HTYPE*是PJSIP_H_SUPPORTED，则*令牌*指定诸如“100rel”扩展名。

- void setUserData（Token *user_data* ）

  ​	将应用程序特定数据附加到呼叫。

  ​	然后，应用程序可以通过调用检查此数据 getUserData()。

  ​	**参数**

  ​		user_data -要附加到任意数据。

- Token getUserData（） const

  ​	获取附接至该呼叫，先前已经设置与用户数据 setUserData() 来。

  ​	**返回**  

  ​		用户数据

- pj_stun_nat_type getRemNatType（）

  ​	获取远程端点的NAT类型。

  ​	当*natTypeInSdp* 在 UaConfig 中设置时，这是pjsua-lib的专有特征，在SDP中发送NAT类型。

  ​	只有从远程接收到SDP之后才能调用此功能。这意味着对于来电，只要来电包含SDP，就可以在收到呼叫时立即调用此功能；对于拨出的呼叫，此功能只能在接收SDP后（通常为200 / OK响应INVITE）才能调用。一般情况下，应用程序应在调用*onCallMediaState（）*时或调用该回调之后才能使用该函数。

  ​	**返回**

  ​		NAT类型。

  ​	**参考**

  ​		Endpoint:: natGetType（)，natTypeInSdp

- void makeCall（*const string* ＆*dst_uri, const*  CallOpParam＆ *PRM* ）

  ​	拨出电话给指定的URI。

  ​	**参数**

  ​		dst_uri -要放入To头的URI（通常与目标URI相同）。

  ​		prm.opt -可选呼叫设置。

  ​		prm.txOption -可选标题等待添加到发出的INVITE请求。

- void answer（*const*  CallOpParam＆ *PRM* ）

  ​	通过呼叫设置参数发送对INVITE请求的响应。

  ​	根据指定为参数的状态码，此功能可能发送临时响应，建立呼叫或终止呼叫。有关通话设置的注意事项：

  ​	如果在此功能的后续呼叫中更改呼叫设置，则仅应用提供的第一个呼叫设置。所以一般来说，应用程序在获取用户的确认之前不会提供呼叫设置。

  ​	如果在SDP已发送未提供通话设置，即：答案与状态码183或者2xx，默认调用的设置将被使用，检查 CallSetting 它的默认值。

  ​	**参数**

  ​		prm.opt -可选呼叫设置。

  ​		prm.statusCode -状态码，（100-699）。

  ​		prm.reason -可选原因短语。如果为空，将使用默认文本。

  ​		prm.txOption -要添加到传出响应消息的标题等的可选列表。请注意，此消息数据将在此INVITE请求的所有下一个答案/响应中持续存在。

- void hangup（*const* CallOpParam＆ *PRM* ）

  ​	通过使用根据呼叫状态适当的方法挂断呼叫。

  ​	此功能比3XX-6xx应答应答呼叫（使用不同的答案() ) ，在这个函数会挂断电话，无论状态和呼叫的角色，而 答案() 只与早期国家来电工程。

  ​	**参数**

  ​		prm.statusCode -我们拒绝来电时可以发送的可选状态码。如果值为零，将发送“603 / Decline”。

  ​		prm.reason -我们拒绝来电时可以选择发送的原因短语。如果为空，将使用默认文本。

  ​		prm.txOption -要添加到传出请求/响应消息的标题等的可选列表。

- void setHold（ *const*  CallOpParam＆ *PRM* ）

  ​	将指定的呼叫置于保持状态。

  ​	这将发送re-INVITE与适当的SDP通知遥控器呼叫被搁置。请求本身的最终地位将在报告*onCallMediaState（）*回调，这表示该呼叫的媒体状态已经改变了应用程序。

  ​	**参数**

  ​		prm.options -pjsua_call_flag常量的位掩码。目前，只能使用标志PJSUA_CALL_UPDATE_CONTACT。

  ​		prm.txOption -与请求一起发送的可选消息组件。

- void reinvite（ *const*  CallOpParam＆ *PRM* ）

  ​	发送re-INVITE。

  ​	请求本身的最终地位将在报告*onCallMediaState（）*回调，这表示该呼叫的媒体状态已经改变了应用程序。

  ​	**参数**

  ​		prm.opt -可选呼叫设置，如果为空，当前呼叫设置将保持不变。

  ​		prm.opt.flag -pjsua_call_flag常量的位掩码。在此指定PJSUA_CALL_UNHOLD将释放呼叫保持。

  ​		prm.txOption -与请求一起发送的可选消息组件。

- void update（ *const*  CallOpParam＆ *PRM* ）

  ​	发送UPDATE请求。

  ​	**参数**

  ​		prm.opt -可选呼叫设置，如果为空，当前呼叫设置将保持不变。

  ​		prm.txOption -与请求一起发送的可选消息组件。

- void xfer（*const string* ＆*DEST*， *const* CallOpParam＆ *PRM* ）

  ​	启动呼叫转移到指定的地址。

  ​	该功能将发送REFER请求，指示远程呼叫方向指定的目的地/目标发起新的INVITE会话。

  ​	如果应用程序有兴趣监视成功性和传输请求的进步，它可以实现*onCallTransferStatus（）*回调，这将报告该呼叫转移请求的进度。

  ​	**参数**

  ​		dest -要联系的新目标的URI。URI可以是名称地址或addr-spec格式。

  ​		prm.txOption -与请求一起发送的可选消息组件。

- void xferReplaces（ *const* call & *dest_call*， *const* CallOpParam& *PRM* ）

  ​	发起参加呼叫转移。

  ​	此功能会发送REFER请求指示远程呼叫方发起新的INVITE会话的URL *destCall*。在党*dest_call*则应从REFER接受者新来电与我们“取代”的号召。

  ​	**参数**

  ​		dest_call -呼叫被替换。

  ​		prm.options -应用程序可以指定PJSUA_XFER_NO_REQUIRE_REPLACES以阻止在REFER请求创建的传出INVITE请求中包含“Require：replace”。

  ​		prm.txOption -与请求一起发送的可选消息组件。

- void processRedirect（ pjsip_redirect_op *CMD* ）

  ​	接受或拒绝重定向响应。

  ​	应用程序必须调用此函数后，它在信号PJSIP_REDIRECT_PENDING *onCallRedirected（）*回调，以通知呼叫是否接受或拒绝重定向到当前目标。应用程序可以使用PJSIP_REDIRECT_PENDING命令的组合*onCallRedirected（）*回调此功能将呼叫重定向之前要求用户权限。

  ​	请注意，如果应用程序选择拒绝或停止重定向（通过分别使用PJSIP_REDIRECT_REJECT或PJSIP_REDIRECT_STOP），则此函数返回之前将调用呼叫断开回调。如果应用程序拒绝对目标的*onCallRedirected（）*回调也可称为前如果有另一个目标试试这个函数返回。

  ​	**参数**

  ​		cmd -要应用于当前目标的重定向操作。这种说法的语义相似，在说明书*onCallRedirected（）*回调，除了PJSIP_REDIRECT_PENDING这里不接受。

- void dialDtmf（ *const* string &*digits*）

  ​	使用RFC 2833有效载荷格式将DTMF数字发送到远程。

  ​	**参数**

  ​		digits -要发送的DTMF字符串数字。

- void sendInstantMessage（ *const*  SendInstantMessageParam ＆*PRM* ）

  ​	在INVITE会话中发送即时消息。

  ​	**参数**

  ​		prm.contentType -MIME类型。

  ​		prm.content -消息内容。

  ​		prm.txOption -要包括在传出请求中的标题等的可选列表。txOption中的body描述符被忽略。

  ​		prm.userData -可选的用户数据，在调用IM回调时将返回。

- void sendTypingIndication（ *const*  SendTypingIndicationParam＆ *PRM* ）

  ​	在INVITE会话中发送IM打字指示。

  ​	**参数**

  ​		prm.isTyping -确实是为了向远方指示当地人当前正在键入IM。

  ​		prm.txOption -要包括在传出请求中的标题等的可选列表。

- void sendRequest（ *const*  CallSendRequestParam＆ *PRM* ）

  ​	发送任意请求与呼叫。

  ​	这对于发送INFO请求是有用的。请注意，应用程序不应该使用此函数发送将更改邀请会话状态的请求，例如re-INVITE，UPDATE，PRACK和BYE。

  ​	**参数**

  ​		prm.method -请求的SIP方法。

  ​		prm.txOption -可选消息体和/或要包括在传出请求中的头的列表。

  ​		字符串**dump**（布尔*with_media*，*const string* *缩进*）

  ​		将呼叫和媒体统计信息转储为字符串。

  ​	**返回**

  ​		呼叫转储和媒体统计字符串。

  ​	**参数**

  ​		with_media -真的也包括媒体信息。

  ​		indent -左缩进的空格。

- INT vidGetStreamIdx（） const

  ​	获取通话中默认视频流的媒体流索引。

  ​	通常这将只是检索呼叫中第一个激活的视频流的流索引。如果没有活动，它将返回第一个不活动的视频流。

  ​	**返回**

  ​		Media 流索引为-1，如果没有视频流存在于呼叫中。

- bool vidStreamIsRunning（ INT *med_idx*，pjmedia_dir *DIR* ） const

  ​	确定指定呼叫的视频流当前是否正在运行（即，

  ​	已被创建，启动，不被暂停）。

  ​	**返回**

  ​		如果目前正在为指定的方向运行流，则为真。

  ​	**参数**

  ​		med_idx -媒体流索引，或-1指定默认的视频媒体。

  ​		dir -要检查的方向。

- void vidSetStream（pjsua_call_vid_strm_op *op*, *const* CallVidSetStreamParam& *param*)

  ​	为指定的呼叫添加，删除，修改和/或操纵视频媒体流。

  ​	这可能会触发重新启动或更新以发送呼叫。

  ​	**参数**

  ​		op -要执行的视频流操作，可能的值是pjsua_call_vid_strm_op。

  ​		param -视频流操作的参数（见CallVidSetStreamParam）。

- StreamInfo getStreamInfo（unsigned *med_idx* ） const

  ​	获取指定媒体索引的媒体流信息。

  ​	**返回**

  ​		流信息。

  ​	**参数**

  ​		med_idx -媒体流索引。

- StreamStat getStreamStat（unsigned *med_idx* ） const

  ​	获取指定媒体索引的媒体流统计信息。

  ​	**返回**

  ​		流统计。

  ​	**参数**

  ​		med_idx -[媒体](http://www.pjsip.org/docs/book-latest/html/reference.html#pjsua2classpj_1_1Media)流索引。

- MediaTransportInfo getMedTransportInfo（unsigned *med_idx* ） const

  ​	获取指定媒体索引的媒体传输信息。

  ​	**返回**

  ​		交通信息。

  ​	**参数**

  ​		med_idx -媒体流索引。

- void processMediaUpdate（OnCallMediaStateParam＆ *PRM* ）

  ​	内部功能（由callled 端点（工艺更新调用媒体呼吁媒体状态发生变化时。

- void processStateChange（OnCallStateParam＆ *PRM* ）

  ​	内部功能（通过称为端点）来处理呼叫状态的变化。

- virtual void onCallState（OnCallStateParam＆ *PRM* ）

  ​	呼叫状态发生变化时通知应用程序。

  ​	然后，应用程序可以查询通话信息通过调用来获取详细呼叫状态[的 getInfo() 函数。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallTsxState（OnCallTsxStateParam＆ *PRM* ）

  ​	这是一个通用回调，当呼叫中的一个事务已经改变状态时被调用。

  ​	例如，应用程序可以实现此回调，以监视传出请求的状态，或者以最终响应回答未处理的传入请求（如INFO）。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallMediaState（OnCallMediaStateParam＆ *PRM* ）

  ​	当通话中的媒体状态发生变化时通知应用程序。

  ​	正常应用需要实现这种回调，例如将呼叫的媒体连接到声音设备。当使用ICE时，也会调用此回调来报告ICE协商失败。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallSdpCreated（OnCallSdpCreatedParam＆ *PRM* ）

  ​	当呼叫刚刚创建本地SDP（用于初始或随后的SDP报价/答案）时通知应用程序。

  ​	应用程序可以在远程SDP发送和/或协商之前实施此回调来修改SDP，例如，以每个帐户/呼叫基础编解码器优先级应用或添加自定义/专有SDP属性。

  ​	**参数**

  ​		prm -回调参数

- virtual void onStreamCreated（OnStreamCreatedParam＆ *PRM* ）

  ​	在创建媒体会话之前，注册到会议桥之前通知应用程序。

  ​	如果已将媒体处理端口添加到流中，应用程序可能会返回不同的媒体端口。然后，这个媒体端口将被添加到会议桥。

  ​	**参数**

  ​		prm -回调参数

- virtual void onStreamDestroyed（OnStreamDestroyedParam＆ *PRM* ）

  ​	Media 会议从会议桥被注销并被破坏时通知应用程序。

  ​	**参数**

  ​		prm -回调参数

- virtual void onDtmfDigit（OnDtmfDigitParam＆ *PRM* ）

  ​	传入DTMF数字时通知应用程序。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallTransferRequest（OnCallTransferRequestParam＆ *PRM* ）

  ​	通知申请被转移（即REFER被接收）。应用程序可以通过设置代码来决定接受/拒绝传输请求（默认为202）。当此回调未实现时，默认行为是接受转移。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallTransferStatus（OnCallTransferStatusParam＆ *PRM* ）

  ​	通知应用以前发送的呼叫转移请求的状态。

  ​	应用程序可以监视呼叫转移请求的状态，例如决定是否终止现有呼叫。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallReplaceRequest（OnCallReplaceRequestParam＆ *PRM* ）

  ​	通过替换标题通知有关INVITE的应用程序。

  ​	应用程序可以通过设置非2xx代码来拒绝该请求。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallReplaced（OnCallReplacedParam＆ *PRM* ）

  ​	通知应用程序现有呼叫已被新呼叫替换。

  ​	当PJSUA-API使用Replaces标头接收到INVITE请求时，会发生这种情况。

  ​	这种回调被调用后，通常PJSUA-API将断开此呼叫并建立一个新的呼叫*newCallId*。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallRxOffer（OnCallRxOfferParam＆ *PRM* ）

  ​	当通话接收到来自遥控器的新报价时通知应用程序（即

  ​	接收到使用SDP的re-INVITE / UPDATE）。应用程序可以通过设置代码来决定接受/拒绝报价（默认为200）。如果报价被接受，应用程序可以更新应答的通话设置。当此回调未实现时，默认行为是接受使用当前通话设置的报价。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallTxOffer（OnCallTxOfferParam＆ *PRM* ）

  ​	通话时通知应用程序接收到没有SDP报价的INVITE。

  ​	应用程序可以更新呼叫设置（例如：添加音频/视频），或启用/禁用编解码器，或者从回调内更新其他媒体会话设置，但是，如标准（RFC3261第14.2节）所规定的那样，它必须确保更新与需要对等体支持的现有媒体会话（编解码器，传输或其他参数）重叠，这是为了避免对等体拒绝该提议。

  ​	当不执行此回调时，默认行为是使用当前活动媒体会话发送SDP报价（每个媒体类型上都有所有启用的编解码器）。

  ​	**参数**

  ​		prm -回调参数

- virtual void onInstantMessage（OnInstantMessageParam＆ *PRM* ）

  ​	通知输入MESSAGE请求的应用程序。

  ​	**参数**

  ​		prm -回调参数

- virtual void onInstantMessageStatus（OnInstantMessageStatusParam＆ *PRM* ）

  ​	通知应用程序关于传出MESSAGE请求的传送状态。

  ​	**参数**

  ​		prm -回调参数

- virtual void onTypingIndication（OnTypingIndicationParam＆ *PRM* ）

  ​	通知应用程序打字指示。

  ​	**参数**

  ​		prm -回调参数

- virtual  pjsip_redirect_op onCallRedirected（OnCallRedirectedParam＆ *PRM* ）

  ​	当呼叫即将重新发送INVITE请求到指定的目标时，调用此回调，遵循先前接收到的重定向响应。

  ​	应用程序可以接受重定向到指定的目标，仅拒绝此目标，并使会话继续尝试列表中的下一个目标，如果此类目标存在，则完全停止整个重定向过程，并导致会话断开或延迟决定要求用户确认。

  ​	此回调是可选的，默认行为是不遵循重定向响应。

  ​	**返回**

  ​	为目标执行的行动。将此参数设置为以下值之一：

  ​		PJSIP_REDIRECT_ACCEPT：立即接受重定向。设置后，呼叫将立即重新发送INVITE请求给目标。

  ​		PJSIP_REDIRECT_ACCEPT_REPLACE：立即接受重定向，并将To标题替换为当前目标。设置后，呼叫将立即重新发送INVITE请求给目标。

  ​		PJSIP_REDIRECT_REJECT：立即拒绝此目标。如果存在，则呼叫将继续重试下一个目标，如果没有更多目标尝试，则断开该呼叫。

  ​		PJSIP_REDIRECT_STOP：停止整个重定向过程，并立即断开通话。该[onCallState（）](http://www.pjsip.org/docs/book-latest/html/reference.html#pjsua2classpj_1_1Call_1a6346fb46307057308e0fff0118306206)回调将PJSIP_INV_STATE_DISCONNECTED状态回调返回后立即调用。

  ​		PJSIP_REDIRECT_PENDING：如果不能立即作出决定（例如要求用户确认），则设置为此值。然后应用程序必须调用processRedirect()，以接受或在获取用户决定拒绝该重定向。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallMediaTransportState（OnCallMediaTransportStateParam＆ *PRM* ）

  ​	当媒体传输状态更改时，将调用此回调。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCallMediaEvent（OnCallMediaEventParam＆ *PRM* ）

  ​	有关媒体事件（例如视频通知）的通知。

  ​	这种回调很可能是从媒体线程调用的，所以应用程序不能在这个回调中执行大量的处理。特别是，应用程序不得在此回调中销毁呼叫或媒体。如果应用程序需要执行更复杂的任务来处理事件，则应将任务发布到另一个线程。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCreateMediaTransport（OnCreateMediaTransportParam＆ *PRM* ）

  ​	应用程序可以使用此回调来实现呼叫的自定义媒体传输适配器，或者使用完全全新的媒体传输替换媒体传输。

  ​	创建新呼叫时调用此回调。图书馆创造了该呼叫的媒体传输，它被设置为*mediaTp*回调的论据。回调可能会随媒体传输的实例而改变，以便呼叫使用。

  ​	**参数**

  ​		prm -回调参数

- virtual void onCreateMediaTransportSrtp（OnCreateMediaTransportSrtpParam＆ *PRM* ）

  ​	当创建SRTP媒体传输时，将调用此回调。

  ​	应用程序可以修改SRTP设置*srtpOpt*指定cryptos和密钥，其将被使用。请注意，应用程序不应该修改字段*pjmedia_srtp_setting.close_member_tp*，只能修改字段*pjmedia_srtp_setting.use*初始INVITE。

  ​	**参数**

  ​		prm -回调参数

### 公共静态功能

- *static* Call* lookup（ INT *CALL_ID* ）

  ​	获取呼叫的指定呼叫ID级别。

  ​	**返回**

  ​		该呼叫实例或NULL，如果没有找到。

  ​	**参数**

  ​		call_id -要查找的呼叫ID

## CallSetting

设置

struct pj::CallSetting

通话设置。

## CallInfo

信息和统计

struct pj::CallInfo

呼叫信息。

应用程序可以通过调用查询呼叫信息 call::getInfo()。

## CallMediaInfo

struct pj::CallMediaInfo

呼叫媒体信息。

## StreamInfo

struct pj::StreamInfo

媒体流信息。

## StreamStat

struct pj::StreamStat

媒体流的统计数据。

## JbufState

struct pj::JbufState

此结构描述抖动缓冲状态。

## RtcpStat

struct pj::RtcpStat

双向RTP流统计。

## RtcpStreamStat

struct pj::RtcpStreamStat

单向RTP流统计。

## MathStat

struct pj::MathStat

此结构描述统计状态。

## MediaTransportInfo

struct pj::MediaTransportInfo

这种结构描述媒体传输信息。

它对应于pjmedia_transport_info结构。



## 回调参数

- struct pj::OnCallStateParam

  ​	该结构包含参数用于 call:: onCallState() 回调。

- struct pj::OnCallTsxStateParam

  ​	该结构包含参数用于 call:: onCallTsxState() 回调。

- struct pj::OnCallMediaStateParam

  ​	该结构包含参数用于 call:: onCallMediaState() 回调。

- struct pj::OnCallSdpCreatedParam

  ​	该结构包含参数用于 call:: onCallSdpCreated() 回调。

- struct pj::OnStreamCreatedParam

  ​	该结构包含参数用于 call:: onStreamCreated() 回调。

- struct pj::OnStreamDestroyedParam

  ​	该结构包含参数用于 call:: onStreamDestroyed() 回调。

- struct pj::OnDtmfDigitParam

  ​	该结构包含参数用于 call:: onDtmfDigit() 回调。

- struct pj::OnCallTransferRequestParam

  ​	该结构包含参数用于 call:: onCallTransferRequest() 回调。

- struct pj::OnCallTransferStatusParam

  ​	该结构包含参数用于 call:: onCallTransferStatus() 回调。

- struct pj::OnCallReplaceRequestParam

  ​	该结构包含参数用于 call:: onCallReplaceRequest() 回调。

- struct pj::OnCallReplacedParam

  ​	该结构包含参数用于 call:: onCallReplaced() 回调。

- struct pj::OnCallRxOfferParam

  ​	该结构包含参数用于call:: onCallRxOffer()回调。

- struct pj::OnCallRedirectedParam

  ​	该结构包含参数用于 call:: onCallRedirected() 回调。

- struct pj::OnCallMediaEventParam

  ​	该结构包含参数用于 call:: onCallMediaEvent() 回调。

- struct pj::OnCallMediaTransportStateParam

  ​	该结构包含参数用于 call:: onCallMediaTransportState() 回调。

- struct pj::OnCreateMediaTransportParam

  ​	该结构包含参数用于 call:: onCreateMediaTransport() 回调。

- struct pj::CallOpParam

  ​	该结构包含参数用于 Call::answer(), Call::hangup(), Call::reinvite(), Call::update(), Call::xfer(), Call::xferReplaces(), Call::setHold().

- struct pj::CallSendRequestParam

  ​	该结构包含参数用于call:: sendRequest()

- struct pj::CallVidSetStreamParam

  ​	该结构包含参数用于 call:: vidSetStream()

## 其他

### MediaEvent

struct pj::MediaEvent

此结构描述媒体事件。

它对应于pjmedia_event结构。

### MediaFmtChangedEvent

struct pj::MediaFmtChangedEvent

此结构描述媒体格式更改事件。

### SdpSession

struct pj::SdpSession

该结构描述了SDP会话描述。

它对应于pjmedia_sdp_session结构。

### RtcpSdes

struct pj::RtcpSdes

RTCP SDES结构。