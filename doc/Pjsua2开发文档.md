# PJSUA2

PJSUA2是PJSUA API以上的面向对象抽象。

它将信令，媒体和NAT穿越功能结合到易于使用的呼叫控制API，帐户管理，好友列表管理，在线状态和即时消息中，以及多媒体功能，如本地会议，文件流，本地播放和语音录制和强大的NAT穿越技术，利用STUN，TURN和ICE。

## 概述

### 主类

- 终端 Endpoint

  实例化这个类中的一个，并从实例中可以初始化并启动库，单例类。

- 账号 Account

  帐户指定SIP会话一侧的人员（或端点）的身份。至少需要创建一个帐户实例，并且可以从帐户实例开始创建/接收电话以及添加好友。

- 媒体 Media

  抽象基类，表示能够生成媒体或传播媒体的媒体元素。将 `AudioMedia `子类化，然后将其子类实例化成具体类，如 `AudioMediaPlayer `和 `AudioMediaRecorder`

- 呼叫 Call

  类表示正在进行的呼叫（或者说技术上是INVITE会话），并且可以用于操纵它，例如应答呼叫，挂断呼叫，保持呼叫，转接呼叫等。

- 搭档 Buddy

  该类代表一个远程伙伴（一个人或一个SIP端点）。您可以订阅好友的状态来了解好友是否在线/离线等等，您可以向/从伙伴发送和接收即时消息。

### 一般概念

- 类使用模式

  从对应的类（Endpoint，Call，Account或Buddy）派生一个类，并实现/重载相关的方法（取决于想要处理的事件）

- 错误处理

  Error类有几个成员来解释错误，例如引发错误的操作名称，错误代码和错误消息本身。

- 异步操作

  在PJSIP中，涉及发送和接收SIP消息的所有操作都是异步的，这意味着调用该操作的功能将立即完成，您将在回调中获得完成状态。

- 线程

  PJSUA2模块本身是线程安全的。

- 垃圾收集问题

  - 在Java 和Python空间中创建的PJSUA2对象的过早析构，并传递给本机空间，而不保留对对象的引用

    应用程序需要维护在其应用程序中创建的对象的显式引用，而不是依赖于PJSUA2本机库来跟踪这些对象。

  - 它延迟了对象（包括PJSUA2对象）的析构，导致对象的析构函数中的代码无序执行

  - GC的销毁操作可以在**之前未注册到PJLIB的不同线程上**运行，从而导致断言**assertion**

    问题 2 、3 必须立即销毁 PJSUA2 对象，而不是依靠 GC 来清理对象。

- 对象持久化

  PJSUA2包括 PersistentObject（持久对象） 类，用于提供从文档（字符串或文件）读取/写入数据的功能。数据可以是简单的数据类型，如布尔值，数字，字符串和字符串数组，或用户定义的对象。

## Endpoint

Endpoint类是一个单例类，应用程序必须在此类实例之前创建一个并且最多只能创建一个，然后才能执行任何操作。这个类是PJSUA2的核心类，它提供了以下功能：

- 启动和关机
- 配置的定制，如核心 UA(用户代理)SIP配置，媒体配置和日志配置

#### 基本使用

1. 实例化端点
2. 创建库
3. 初始化并配置设置
4. 创建一个或多个传输
5. 创建安全传输（TLS）
6. 启动库
7. 关闭库

##### 实例化端点

在其他任何事情之前，必须实例化Endpoint类：

```c
Endpoint *ep = new Endpoint;
```

##### 创建库

通过调用其libCreate（）方法来创建库：

```c
try 
{
    ep->libCreate();
}
catch(Error& err) 
{
    cout << "Startup error: " << err.info() << endl;
}
```

如果发生错误，libCreate（）方法将引发异常，因此我们需要使用try / catch子句来捕获异常。

##### 初始化库并配置设置

EpConfig类提供端点配置，允许自定义以下设置：

- UAConfig，指定核心SIP用户代理设置。
- MediaConfig，指定各种媒体*全局*设置
- LogConfig来自定义日志设置。

请注意，可以在AccountConfig中根据每个帐户进一步指定一些设置。

要自定义设置，请创建EpConfig类的实例，并在端点初始化期间指定它们（稍后将对此进行说明），例如： 

```c
EpConfig ep_cfg;
ep_cfg.logConfig.level = 5;
ep_cfg.uaConfig.maxCalls = 4;
ep_cfg.mediaConfig.sndClockRate = 16000;
```

接下来，通过调用libInit（）初始化库：

```c
try 
{
    EpConfig ep_cfg;
    // Specify customization of settings in ep_cfg
    ep->libInit(ep_cfg);
}
catch(Error& err)
{
    cout << "Initialization error: " << err.info() << endl;
}
```

上面的代码片段使用默认设置初始化库。

##### 创建一个或多个传输

在发送或接收SIP消息之前，应用程序需要创建一个或多个传输：

```c
try {
    TransportConfig tcfg;
    tcfg.port = 5060;
    TransportId tid = ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
} 
catch(Error& err) 
{
    cout << "Transport creation error: " << err.info() << endl;
}
```

transportCreate（）方法返回新创建的传输ID，它传递 传输类型和TransportConfig对象 来定制传输设置，如绑定地址和侦听端口号。没有这个，默认情况下，传输将绑定到INADDR_ANY和任何可用的端口。

除了创建无用户帐户（使用Account.create（））以外，没有真正使用传输ID，稍后将对此进行说明），也许可以在应用程序想要的时候向用户显示传输列表。

##### 创建安全传输（TLS）[ ](http://www.pjsip.org/docs/book-latest/html/endpoint.html#creating-a-secure-transport-tls)

要创建TLS传输，您可以使用与上述相同的方法。您可以通过修改字段TransportConfig.tlsConfig来进一步自定义TLS传输，例如设置证书文件或选择使用的密码。

```c
try {
    TransportConfig tcfg;
    tcfg.port = 5061;
    // Optional, set CA/certificate/private key files.
    // tcfg.tlsConfig.CaListFile = "ca.crt";
    // tcfg.tlsConfig.certFile = "cert.crt";
    // tcfg.tlsConfig.privKeyFile = "priv.key";
    // Optional, set ciphers. You can select a certain cipher/rearrange the order of ciphers here.
    // tcfg.ciphers = ep->utilSslGetAvailableCiphers();
    TransportId tid = ep->transportCreate(PJSIP_TRANSPORT_TLS, tcfg);
} 
catch(Error& err) 
{
    cout << "Transport creation error: " << err.info() << endl;
}
```

##### 启动库

现在我们启动库了。我们需要启动库完成初始化阶段，例如完成初始的STUN地址解析，初始化/启动声音设备等。要启动库，请调用libStart（）方法：

```c
try
{
    ep->libStart();
}
catch(Error& err) 
{
    cout << "Startup error: " << err.info() << endl;
}
```

##### 关闭库

应用程序退出后，库需要关闭，以便将资源释放回操作系统。虽然可以通过删除端点实例（内部调用libDestroy（））来完成，最好是手动调用它，因为在Java或Python中，垃圾回收有问题，如前所述：

```c
ep->libDestroy();
delete ep;
```

## 账户

一个帐户有一个与之相关的SIP统一资源标识符（URI）。该URI用作该人的记录地址（ Address of Record，AOR），并且用作传出请求中的From头。

帐户也与路由集和一些认证凭证相关联，这些**凭证**在使用该帐户发送SIP请求消息时使用。帐户还**具有状态**，当它们订阅该帐户时，该状态将被报告给远端（远程对等体），或者如果为该帐户启用了发布，则该状态发布到呈现（Presence）服务器。

必须在应用程序中创建至少一个帐户，因为任何外发请求都需要一个帐户上下文。如果不需要用户关联，应用程序可以通过调用Account.create（）来创建一个**无用户帐户**。无用户帐户识别本地端点而不是特定用户，并且它对应于特定的传输ID。

还有一个帐户必须设置为**默认帐户**，当pjsua使用更严格的匹配规则无法将传入请求与任何帐户匹配时，用作帐户身份。

### 使用

#### 子类化Account类

要使用Account类，通常应用程序应该创建自己的子类，以便接收该帐户的通知。

```c++
class MyAccount : public Account
{
	public:
    	MyAccount() {}
    	~MyAccount() {}

    virtual void onRegState(OnRegStateParam &prm)
    {
    	AccountInfo ai = getInfo();
		cout << (ai.regIsActive? "*** Register: code=" : "*** Unregister: code=")<< prm.code << endl;
	}

	virtual void onIncomingCall(OnIncomingCallParam &iprm){
		Call *call = new MyCall(*this, iprm.callId);
		// Just hangup for now
        CallOpParam op;
        op.statusCode = PJSIP_SC_DECLINE;
        call->hangup(op);
		
		// And delete the call
		delete call;
    }
};
```

在其子类中，应用程序可以实现帐户回调，基本用于处理与该帐户相关的事件，如：

- SIP注册的状态
- 传入呼叫
- 传入存在订阅请求
- 不是从伙伴传入的即时消息

如果未处理事件，将调用默认操作：

- 来电将不被处理
- 传入的存在订阅请求将被接受
- 来自非伙伴的传入即时消息将被忽略

#### 创建无用户账户

无用户帐户标识特定SIP端点，而不是特定用户。一些其他SIP软电话可以称之为对等模式，这意味着我们通过其地址呼叫另一台计算机，而不是调用特定的用户ID。例如，我们可能会将自己标识为 **sip：192.168.0.15**（无用户帐户），而不是 **sip：alice@pjsip.org** 。

在下层PJSUA-LIB API中，无用户帐户与SIP传输相关联，并且使用pjsua_acc_add_local()API 创建。这个概念在PJSUA2中已经被弃用了，而且一个无用户帐号是一个没有用户名的ID URI（例如“ [sip：192.168.0.15](sip:192.168.0.15) ”），是一个没有注册的“正常”账户。因此，创建无用户帐户与创建“正常”帐户完全相同。

#### 创建账号

需要配置AccountConfig并调用Account.create（）来创建帐户。至少，pjsua只需要帐户的ID，这是一个URI来标识帐户（或者在SIP术语中，它被称为记录地址/ AOR）。

```c++
AccountConfig acc_cfg;
acc_cfg.idUri = "sip:test1@pjsip.org";
MyAccount *acc = new MyAccount;
try {
    acc->create(acc_cfg);
} catch(Error& err) {
    cout << "Account creation error: " << err.info() << endl;
}
```

以上创建的帐户不会执行任何操作，除非在发件人的“发件人”标题中提供身份。该帐户不会注册到SIP服务器或任何东西。

通常，将希望该帐户认证并注册到SIP服务器，以便可以接收来电。为此，需要在AccountConfig中配置一些更多设置，如下所示：

```c++
AccountConfig acc_cfg;
acc_cfg.idUri = "sip:test1@pjsip.org";
acc_cfg.regConfig.registrarUri = "sip:pjsip.org";
acc_cfg.sipConfig.authCreds.push_back( AuthCredInfo("digest", "*", "test1", 0, "secret1") );

MyAccount *acc = new MyAccount;
try {
    acc->create(acc_cfg);
} catch(Error& err) {
    cout << "Account creation error: " << err.info() << endl;
}
```

#### 账户配置

在AccountConfig中可以指定更多的设置，如：

- AccountRegConfig，指定注册设置，如注册服务器和重试间隔。
- AccountSipConfig，用于指定SIP设置，如凭证信息和代理服务器。
- AccountCallConfig，用于指定呼叫设置，例如是否需要可靠的临时响应（SIP 100rel）。
- AccountPresConfig来指定状态设置，例如是否启用在线发布（PUBLISH）。
- AccountMwiConfig，指定MWI（留言等待指示）设置。
- AccountNatConfig，用于指定NAT设置，例如是否使用STUN或ICE。
- AccountMediaConfig，用于指定媒体设置，如安全RTP（SRTP）相关设置。
- AccountVideoConfig，用于指定视频设置，如默认捕获和渲染设备。

#### 账户操作

Account对象的一些操作：

- 管理注册
- 管理好友/联系人
- 管理在线状态

## 媒体(Media)

媒体对象是能够产生媒体或接受媒体的对象。

Media的重要子类是AudioMedia，它代表音频媒体。PJSUA2支持多种类型的音频媒体对象：

- 捕获设备的AudioMedia，用于从声音设备捕获音频。
- 播放设备的AudioMedia，可以播放音频到声音设备。
- 呼叫音频媒体，用于向远程人员发送和接收音频。
- AudioMediaPlayer，播放WAV文件。
- AudioMediaRecorder将音频录制到WAV文件。

### 音频会议桥

会议桥提供了一种简单而又强大的概念来管理音频媒体之间的音频流。原理很简单，就是将音频源连接到音频目的地，桥接器会把音频流从源地址送到目的地，就这样。如果多个来源正在发送到同一个目的地，则来自源的音频被混合。如果一个信号源发送到多个目的地，则桥接器负责把来自源的音频复制到多个目的地。桥接器甚至会照顾不同时钟频率和时间的媒体。

在PJSUA2中，所有音频媒体对象都插入中央会议桥，方便操作。首先，插入式音频媒体将不会连接到任何东西，因此媒体不会流出/入任何对象。音频媒体源可以使用API AudioMedia.startTransmit（）/ AudioMedia.stopTransmit（））启动/停止到目的地的传输。

插入式的会议桥的音频媒体对象将被赋予识别桥对象的端口ID号。 应用程序可以使用API AudioMedia.getPortId（）来检索端口ID。 通常，应用程序不需要担心会议桥及其端口ID（所有这些都将由Media类负责），除非应用程序要创建自己的定制音频媒体。

### 使用

#### 播放WAV文件

```c++
AudioMediaPlayer player;
AudioMedia& play_med = Endpoint::instance().audDevManager().getPlaybackDevMedia();
try 
{
    player.createPlayer("file.wav");
    player.startTransmit(play_med);
} 
catch(Error& err) 
{
}
```

默认情况下，WAV文件将以循环模式播放。要禁用循环，创建播放器时，需指定 `PJMEDIA_FILE_NO_LOOP`：

```c++
player.createPlayer("file.wav", PJMEDIA_FILE_NO_LOOP);
```

没有循环，一旦回放到达WAV文件的末尾，就会播放静音。

完成播放后，只需停止播放即可停止播放：

```c++
try
{
    player.stopTransmit(play_med);
}
catch(Error& err) 
{
}
```

在停止播放时，将从上次播放位置恢复播放，恢复传输。使用player.setPos（）到播放位置，设定到所需的位置。

#### 录制为WAV文件

```c++
AudioMediaRecorder recorder;
AudioMedia& cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();
try {
    recorder.createRecorder("file.wav");
    cap_med.startTransmit(recorder);
} catch(Error& err) {
}
```

媒体将从声音设备流向WAV录像文件。像前面一样，要停止或暂停录制，只需停止传输：

```c++
try {
   cap_med.stopTransmit(recorder);
} catch(Error& err) {
}
```

请注意，如上所述停止向WAV录像机的传输并不会关闭WAV文件，可通过将源连接到WAV录像机来重新开始录制。关闭之前无法播放录制的WAV文件。要关闭WAV录音机，只需删除它：

```c++
delete recorder;
```

#### 本地音频环回

通过将捕获设备中的音频直接发送到播放设备（即本地环回），检查本地声音设备（捕获和播放设备）是否正常工作的有用测试。可以这样做：

```c++
cap_med.startTransmit(play_med);
```

#### 循环音频

如果需要，可以将音频媒体对象的音频循环到自身（即，从对象接收的音频将被传输到自身）。只要对象具有双向介质，就可以从任何对象中回放音频。这意味着可以循环呼叫音频媒体，以便远程人员收到的音频将被传回给他/他。但是不能循环WAV播放器或录音机，因为这些对象只能播放或录制，而不能同时播放和录制。

#### 正常呼叫

单个呼叫可以有多个媒体（例如音频和视频）。应用程序可以使用API Call.getMedia（）检索音频媒体。然后，对于正常的呼叫，我们想要与远程人员建立双向音频，可以通过连接声音设备和呼叫音频媒体轻松完成，反之亦然：

```c++
CallInfo ci = call.getInfo();
AudioMedia *aud_med = NULL;

// Find out which media index is the audio
for (unsigned i=0; i<ci.media.size(); ++i) {
    if (ci.media[i].type == PJMEDIA_TYPE_AUDIO) {
        aud_med = (AudioMedia *)call.getMedia(i);
        break;
    }
}

if (aud_med) {
    // This will connect the sound device/mic to the call audio media
    cap_med.startTransmit(*aud_med);

    // And this will connect the call audio media to the sound device/speaker
    aud_med->startTransmit(play_med);
}
```

#### 第二通电话

假设我们想同时与两个伙伴谈话。由于我们已经与一方进行双向媒体连接，我们只需要使用以下代码向对方添加双向连接：

```c++
AudioMedia *aud_med2 = (AudioMedia *)call2.getMedia(aud_idx);
if (aud_med2) {
    cap_med->startTransmit(*aud_med2);
    aud_med2->startTransmit(play_med);
}
```

现在我们可以同时与双方交谈，我们会听到任何一方的声音。但是在这个阶段，远程方不能说话或者听不到对方（即我们还没有在全会议模式下）。

#### 电话会议

为了让双方互相交流，只需建立双向媒体：

```c++
aud_med->startTransmit(*aud_med2);
aud_med2->startTransmit(*aud_med);
```

现在三方（我们和这两个伙伴）都能够相互交流。

#### 录制会议

在进行会议时，想要将会议记录到WAV文件是非常有意义的，我们需要做的就是将麦克风和两个通话连接到WAV录音机：

```c++
cap_med.startTransmit(recorder);
aud_med->startTransmit(recorder);
aud_med2->startTransmit(recorder);
```

### 音频设备管理

[音频设备框架](https://www.pjsip.org/docs/book-latest/html/media.html#auddev)

## Call

呼叫由Call类处理

### 使用

#### 子类化 Call类

```c++
class MyCall : public Call
{
	public:
    	MyCall(Account &acc, int call_id = PJSUA_INVALID_ID): Call(acc, call_id)
    	{ }

    	~MyCall(){ }

    	// Notification when call's state has changed.
    	virtual void onCallState(OnCallStateParam &prm);

    	// Notification when call's media state has changed.
    	virtual void onCallMediaState(OnCallMediaStateParam &prm);
};
```

#### 呼出Making Outgoing Calls

拨出电话很简单，只需调用Call对象的makeCall（）方法即可。假设在“dest_uri”中将Account对象作为acc变量和目标URI字符串，可以使用下面的代码段发起呼出：

```c++
Call *call = new MyCall(*acc);
CallOpParam prm(true); // Use default call settings
try {
    call->makeCall(dest_uri, prm);
} catch(Error& err) {
    cout << err.info() << endl;
}
```

上面的代码段创建一个Call对象，并使用默认的呼叫设置启动到dest_uri的呼出。对呼叫的后续操作可以使用呼叫实例中的方法，并且将呼叫的事件报告给回调。

#### 接收来电 Receiving Incoming Calls

来电将被报告为Account类的onIncomingCall（）。必须从Account类派生一个类来处理来电。

```c++
void MyAccount::onIncomingCall(OnIncomingCallParam &iprm)
{
    Call *call = new MyCall(*this, iprm.callId);
    CallOpParam prm;
    prm.statusCode = PJSIP_SC_OK;
    call->answer(prm);
}
```

在回调函数中创建呼叫实例。应用程序应确保在呼叫生命周期内存储呼叫实例（直到呼叫断开连接）。

#### 呼叫属性

所有呼叫属性，如状态，媒体状态，远端（远程对等体）信息等都存储为CallInfo类，可以使用Call的getInfo（）方法获取。

#### 呼叫断开

呼叫断开事件是一个特殊的事件，因为一旦报告此事件的回调返回，该调用将不再有效，并且调用对象的任何操作将引发错误异常。因此，建议删除回调中的调用对象。

Call的onCallState（）方法报告呼叫断开，可以检测如下：

```c++
void MyCall::onCallState(OnCallStateParam &prm)
{
    CallInfo ci = getInfo();
    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        /* Delete the call */
        delete this;
    }
}
```

#### 与通话的音频媒体工作

当呼叫的音频媒体准备就绪（或激活）时，只能使用呼叫的音频媒体（例如，连接会议电话中的声音设备的呼叫）。呼叫媒体状态的更改在onCallMediaState（）回调中报告，如果呼叫音频媒体已准备就绪（或活动），则Call.getMedia（）函数将返回有效的音频媒体。

以下是当媒体处于活动状态时将呼叫连接到声音设备的示例代码：

```c++
void MyCall::onCallMediaState(OnCallMediaStateParam &prm)
{
    CallInfo ci = getInfo();
    // Iterate all the call medias
    for (unsigned i = 0; i < ci.media.size(); i++) {
        if (ci.media[i].type==PJMEDIA_TYPE_AUDIO && getMedia(i)) {
            AudioMedia *aud_med = (AudioMedia *)getMedia(i);

            // Connect the call audio media to sound device
            AudDevManager& mgr = Endpoint::instance().audDevManager();
            aud_med->startTransmit(mgr.getPlaybackDevMedia());
            mgr.getCaptureDevMedia().startTransmit(*aud_med);
        }
    }
}
```

当音频媒体变得不活动时（例如当呼叫被保持）时，不需要停止到/从声音设备的音频媒体传输，因为（当音频媒体不再有效时）呼叫的音频媒体将自动从会议桥被移除，这也将自动删除来自或连接到该呼叫的所有连接。

#### 呼叫操作

可以调用Call对象的操作，例如挂断，保持呼叫，发送re-INVITE等。

#### 即时消息(IM)

可以使用Call.sendInstantMessage（）在通话中发送IM。Call.onInstantMessageStatus（）回调方法报告传出即时消息的传输状态。

除了发送即时消息，还可以使用Call.sendTypingIndication（）发送打字指示。

在呼叫中收到的IM和打字指示将在回调函数Call.onInstantMessage（）和Call.onTypingIndication（）中报告。

或者，您可以通过使用Buddy.sendInstantMessage（）和Buddy.sendTypingIndication（）发送IM和打电话外的指示。

## Buddy 好友(存在)

PJSUA2的功能是围绕Buddy类为中心展开的。该类表示一个远端好友（伙伴，一个人或一个SIP端点）。

### 使用

#### 子类化Buddy类

要使用Buddy类，通常应创建子类，如：

```c++
class MyBuddy : public Buddy
{
	public:
		MyBuddy() {}
        ~MyBuddy() {}

    virtual void onBuddyState();
};
```

在子类中，应用程序可以实现好友回调，以获得好友状态更改的通知

#### 订阅好友的状态

要订阅好友的状态，需添加好友对象并订阅好友的状态。下面的代码片段实现这些示例：

```c++
BuddyConfig cfg;
cfg.uri = "sip:alice@example.com";
MyBuddy buddy;
try {
    buddy.create(*acc, cfg);
    buddy.subscribePresence(true);
} catch(Error& err) {
}
```

可以在onBuddyState（）回调中获得好友的状态改变：

```c++
void MyBuddy::onBuddyState()
{
    BuddyInfo bi = getInfo();
    cout << "Buddy " << bi.uri << " is " << bi.presStatus.statusText << endl;
}

```

#### 响应订阅请求

默认情况下，会自动接受对帐户传入的（存在）订阅。您可能想要更改此行为，例如仅仅好友列表中的一个好友时自动接受订阅，以及任何其他用户提示是否希望接受请求的消息。

这可以通过覆盖Account类的onIncomingSubscribe（）方法来实现。

#### 更改帐户的在线状态

要更改帐户的状态，可使用Account.setOnlineStatus（）函数设置帐户的在线状态（即可用或不可用）以及可选的一些扩展信息（例如忙碌，远离手机等），例如：

```c++
try {
    PresenceStatus ps;
    ps.status = PJSUA_BUDDY_STATUS_ONLINE;
    // Optional, set the activity and some note
    ps.activity = PJRPID_ACTIVITY_BUSY;
    ps.note = "On the phone";
    acc->setOnlineStatus(ps);
} catch(Error& err) {
}
```

当更改存在状态时，帐户将根据自身配置将新状态发布给所有在线用户，使用PUBLISH请求或NOTIFY请求，或同时使用上述两个请求。

#### 即时通讯(IM)

可以使用Buddy.sendInstantMessage（）发送IM。传出即时消息的传输状态在Account类的Account.onInstantMessageStatus（）回调方法中给出。

除了发送即时消息，还可使用Buddy.sendTypingIndication（）向远程伙伴发送打字提示。

接收IM和不在呼叫范围内的打字提示将在回调函数Account.onInstantMessage（）和Account.onTypingIndication（）中给出(报告)。

或者，可使用Call.sendInstantMessage（）和Call.sendTypingIndication（）在呼叫中发送IM和键入指示。

## 媒体质量(Media Quality)

### 音频质量

- 遵循指南：[使用pjsystest测试声音设备](http://trac.pjsip.org/repos/wiki/Testing_Audio_Device_with_pjsystest)

- 识别声音问题并使用以下步骤进行故障排除：[检查声音问题](http://trac.pjsip.org/repos/wiki/sound-problems)

### 视频质量

- 有关视频API的更多信息，请参见“ [视频用户指南”](http://trac.pjsip.org/repos/wiki/Video_Users_Guide)

- 使用PJSUA API检查本地视频预览，如“ [视频用户指南 - 视频预览API”](http://trac.pjsip.org/repos/wiki/Video_Users_Guide#VideopreviewAPI)中所述。

- 于视频需要较大的带宽，因此我们需要检查网络问题（损伤），如“ [检查网络问题（损伤）”中所述](http://trac.pjsip.org/repos/wiki/audio-check-packet-loss)。
- 检查CPU利用率。如果CPU利用率太高，可尝试使用不同的（减少CPU占用率（CPU-intensive））视频编解码器或降低分辨率/ fps。有关如何降低CPU利用率的文档，请参考：[FAQ-CPU利用率](http://trac.pjsip.org/repos/wiki/FAQ#cpu)。

## 常见API

https://www.cnblogs.com/mobilecard/p/6736026.html