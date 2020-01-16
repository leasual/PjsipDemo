## Buddy

class pj::Buddy

Buddy

### 公有函数

- Buddy()

  ​	构造函数.

- virtual ~Buddy() 

  ​	析构函数

  ​	注意，如果 Buddy 实例被删除，它也将删除PJSUA-LIB中的对应伙伴

- void create([Account& *acc*, *const* BuddyConfig& *cfg*)

  ​	创建好友并将好友注册给PJSUA-LIB 

  ​	**参数**

  ​		acc -这个好友的帐号

  ​		cfg -好友配置.

- bool isValid() const 检查这个好友是否有效

  ​	**返回**     

  ​		如果有效返回true

- BuddyInfo getInfo() const 获取详细的好友信息

  ​	**返回**  

  ​		Buddy info. 

- void subscribePresence(bool *subscribe*)

  ​	启用/禁用好友的状态监控。一旦好友状态（存在）被订阅，应用程序将通过*onBuddyState（）*回调通知好友的状态改变。

  ​	**参数**

  ​		subscribe - 指定true以激活状态订阅 

- void updatePresence(void)

  ​	更新伙伴的在线信息。虽然动态库会定期刷新所有好友的存在订阅，但某些应用程序可能立即刷新好友的呈现（状态）订阅，这种情况下，可使用此功能来完成此操作。

  ​	注意，只有启用了好友的状态监视，才能启动好友的呈现（状态）订阅。请参阅 subscribePresence() 了解更多信息。此外，如果好友的状态订阅已经处于活动状态，则此函数将不执行任何操作。

  ​	一旦对于好友成功激活了呈现（状态）订阅，应用程序将在*onBuddyState（）*回调中通知好友的状态。

- void sendInstantMessage(*const* SendInstantMessageParam& *prm*)

  ​	对外发送即时消息，使用该伙伴指定的帐户进行路由集和身份验证。 

  ​	**参数** 

  ​		prm - 发送即时消息的参数 

- void sendTypingIndication(*const* SendTypingIndicationParam& *prm*)

  ​	对外发送打字提示 

  ​	**参数**

  ​		prm - 发送打字提示的参数 

- virtual void onBuddyState()

  ​	当Buddy的状态改变时通知应用程序。应用程序可以通过查询buddy的状态获得详细信息

## PresenceStatus

状态

struct pj::PresenceStatus

描述呈现状态

## BuddyInfo

信息

struct pj::BuddyInfo

该结构体描述buddy的信息，可以通过函数 Buddy::getInfo() 获取该信息

## BuddyConfig

配置

struct pj::BuddyConfig

当 Buddy::create() 将好友添加到好友列表时，此结构描述好友配置

继承 pj::PersistentObject

