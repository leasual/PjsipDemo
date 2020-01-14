## Account

### Account帐号

*class* pj::Account

帐户 

#### 公有函数

- Account（）

  ​	构造函数

- virtual ~Account（）

  ​	析构器

  ​	请注意，如果该帐户被删除，它也将删除PJSUA-LIB中的相应帐户。

- void create（const AccountConfig＆cfg，bool make_default = false ）

  ​	创建帐户

  ​	参数cfg -帐号配置

  ​	make_default -将其设为默认帐户。

- void modify（const AccountConfig＆cfg ）

  ​	修改帐户以使用指定的帐户配置。根据变更，这可能会导致帐户的注销或重新注册。

  ​	参数cfg -要应用于帐户的新帐户配置。

- bool isValid（） const

  ​	检查此帐户是否仍然有效。

  ​	返回 true如果有效

- void setDefault（）

  ​	将此设置为默认帐户以在传入和传出请求与任何帐户不匹配时使用。

  ​	返回 PJ_SUCCESS成功。

- bool isDefault（） const

  ​	检查这个帐户是否是默认帐户。默认帐户将用于与任何其他帐户不匹配的传入和传出请求。

  ​	返回 如果这是默认帐户，则为true。

- INT getId（） const

  ​	获取与此帐户相关联的PJSUA-LIB帐户ID或索引。

  ​	返回 整数大于或等于零。

- AccountInfo getInfo（） const

  ​	获取帐户信息。

  ​	返回 帐户信息。

- void setRegistration（ bool renew ） 

  ​	更新注册或执行注销。如果要手动更新注册或从服务器注销，通常只需要调用此函数。

  ​	参数renew - 如果是false，将执行注销。

- void setOnlineStatus（const PresenceStatus＆pres_st ）

  ​	设置或修改要发布给远程/现场订阅帐户的在线状态。如果此帐户有服务器端存在订阅，和/或出站PUBLISH（如果此帐户启用了存在发布），则会触发发送传出的NOTIFY请求。

  ​	参数 pres_st - 在线状态。

- void setTransport（TransportId tp_id ）

  ​	将此帐户锁定/绑定到特定的传输/侦听器。

  ​	通常，应用程序不需要这样做，因为库将根据目的地自动选择传输。当帐户被锁定/绑定到特定的传输时，来自该帐户的所有传出请求将使用指定的传输（包括SIP注册，对话（呼叫和事件订阅）以及诸如MESSAGE之类的对话外请求）。注意，传输ID也可以在AccountConfig中指定。

  ​	参数 tp_id - 传输ID。

- void presNotify（const PresNotifyParam＆prm ）

  ​	发送NOTIFY以通知帐户存在状态或终止服务器端存在订阅

  ​	如果应用程序想要拒绝传入的请求，它应该将PresNotifyParam.state的参数设置为PJSIP_EVSUB_STATE_TERMINATED。

  ​	参数prm -发送NOTIFY参数。

- const BuddyVector＆enumBuddies（） const

  ​	枚举帐户的所有好友。

  ​	返回 好友列表。

- buddy * findBuddy（string URI，FindBuddyMatch * buddy_match = NULL ） const

  ​	在具有指定URI的好友列表中找到一个好友。

  ​	异常：如果没有找到好友，PJ_ENOTFOUND将被抛出。

  ​	返回 指向好友的指针

  ​	参数 uri -好友URI。

  ​	buddy_match -好友匹配算法

- void addBuddy（Buddy * buddy ）

  ​	一个内部功能，将一个好友添加到帐户好友列表。应用程序不能使用此功能。

- void removeBuddy（Buddy * buddy ）

  ​	从帐户好友列表中删除好友的内部功能。应用程序不能使用此功能。

- virtual void onIncomingCall（OnIncomingCallParam＆prm ）来电时通知应用程序。

  ​	参数prm -回调参数

- virtual void onRegStarted（OnRegStartedParam＆prm ）

  ​	注册或注销登记时通知申请。

  ​	注意，这只会通知初始注册和注销。一旦注册会话处于活动状态，后续刷新将不会导致此回调被调用。

  ​	参数prm - 回调参数

- virtual void onRegState（OnRegStateParam＆prm ）

  ​	注册状态发生变化时通知申请。应用程序可能会查询帐户信息以获取注册详细信息。

  ​	参数prm -回调参数

- virtual void onIncomingSubscribe（OnIncomingSubscribeParam＆prm ）

  ​	收到传入SUBSCRIBE请求时的通知。

  ​	应用程序可以使用此回调来授权传入的订阅请求（例如，如果请求被授予，请求用户许可）。

  ​	如果未实现此回调，则将接受所有传入的存在订阅请求。

  ​	如果实现这个回调，在处理传入的请求时应用程序有几个选择：

  ​		它可以通过在IncomingSubscribeParam.code参数中指定非200类最终响应立即拒绝该请求。

  ​		它可以通过指定200作为IncomingSubscribeParam.code参数来立即接受该请求。如果应用程序未设置任何值给IncomingSubscribeParam.code参数，则这是默认值。在这种情况下，库将从该回调返回时自动发送NOTIFY请求。

  ​		它可能延迟处理请求，例如要求用户许是接受还是拒绝请求。在这种情况下，该应用程序必须设置IncomingSubscribeParam.code参数202，然后立即调用presNotify（）与状态PJSIP_EVSUB_STATE_PENDING，并在之后调用presNotify（）再次接受或拒绝订阅请求。

  ​		200和202以外的任何IncomingSubscribeParam.code将被视为200。

  ​		应用程序必须立即从此回调中返回（例如，在等待用户确认时不得阻止此回调）。

  ​	参数prm - 回调参数

- virtual void onInstantMessage（OnInstantMessageParam＆prm ）

  ​	调用上下文接收通知应用程序传入的即时消息或寻呼（即消息请求）

  ​	参数prm - 回调参数

- virtual void onInstantMessageStatus（OnInstantMessageStatusParam＆prm ）

  ​	通知应用程序传送状态，关于传出寻呼/即时消息（即，MESSAGE）请求的传送状态。

  ​	参数prm -回调参数

- virtual void onTypingIndication（OnTypingIndicationParam＆prm ）

  ​	通知应用程序打字指示。

  ​	参数prm -回调参数

- virtual void onMwiInfo（OnMwiInfoParam＆prm ）

  ​	关于MWI（消息等待指示Message Waiting Indication）状态变化的通知。

  ​	可以在SUBSCRIBE请求的状态更改（例如，202 /接收到SUBSCRIBE被接收）或接收到NOTIFY reqeust时调用此回调。

  ​	参数prm -回调参数 

#### 公共静态功能

- static Account * lookup（ int acc_id ）

  ​	获取指定帐户ID 的帐户类。

  ​	返回 该帐户实例或NULL（如果没有找到）。

  ​	参数acc_id -要查找的帐号

###  AccountInfo

struct pj::AccountInfo

\#include <account.hpp> 

帐户信息。

应用程序可以通过调用Account :: getInfo （）来查询帐户信息。

### 帐户设置

- AccountConfig

  ​	struct pj::AccountConfig

  ​	帐户配置，从pj :: PersistentObject继承

-  AccoutRegConfig

  ​	struct pj::AccountRegConfig

  ​	帐户注册配置，在AccountConfig中指定。从pj :: PersistentObject继承

- AccountSipConfig

  ​	struct pj::AccountSipConfig

  ​	帐户的各种SIP设置。在AccountConfig中指定。从pj :: PersistentObject继承

- AccountCallConfig

  ​	struct pj::AccountCallConfig

  ​	帐户的通话设置。在AccountConfig中指定。从pj :: PersistentObject继承

- AccountPresConfig

  ​	struct pj::AccountPresConfig

  ​	帐户存在配置。这将在AccountConfig中指定。从pj :: PersistentObject继承

- AccountMwiConfig

  ​	struct pj::AccountMwiConfig帐号 MWI（留言等待指示，Message Waiting Indication）设置。

  ​	在AccountConfig中指定。从pj :: PersistentObject继承

- AccountNatConfig

  ​	struct pj::AccountNatConfig帐户的NAT（网络地址转换）设置

  ​	在AccountConfig中指定。从pj :: PersistentObject继承

- AccountMediaConfig

  ​	struct pj::AccountMediaConfig帐户媒体配置（适用于音频和视频）

  ​	在AccountConfig中指定。从pj :: PersistentObject继承

- AccountVideoConfig

  ​	struct pj::AccountVideoConfig帐号视频配置

  ​	在AccountConfig中指定。从pj :: PersistentObject继承

### 回调参数

- struct pj::OnIncomingCallParam

  ​	此结构包含onIncomingCall（）帐户回调的参数。

- struct pj::OnRegStartedParam

  ​	此结构包含onRegStarted（）帐户回调的参数。

- struct pj::OnRegStateParam

  ​	此结构包含onRegState（）帐户回调的参数。

- struct pj::OnIncomingSubscribeParam

  ​	此结构包含onIncomingSubscribe（）回调的参数。

- struct pj::OnInstantMessageParam

  ​	onInstantMessage（）帐户回调的参数。

- struct pj::OnInstantMessageStatusParam

  ​	onInstantMessageStatus（）帐户回调的参数。

- struct pj::OnTypingIndicationParam

  ​	onTypingIndication（）帐户回调的参数。

- struct pj::OnMwiInfoParam

  ​	onMwiInfo（）帐户回调的参数。

- struct pj::PresNotifyParam

  ​	presNotify（）帐户方法的参数。

### 其他

class pj::FindBuddyMatch

Buddy匹配算法的包装类。

默认算法是Buddy URI 中搜索令牌的简单子字符串查找，区分大小写。应用程序可以通过覆盖此类并在Account :: findBuddy（）中指定其实例来实现自己的匹配算法。

### 公有函数

- virtual bool match（const string＆token，const Buddy＆buddy ）

  默认算法实现。

- virtual ~FindBuddyMatch（）

  析构器