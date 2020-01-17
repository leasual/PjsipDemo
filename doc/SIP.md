## SIP

SIP使用统一资源定位符 URI 来表示用户，通常由一个域名加上用户名组成，如 sip:John@eastcom.com

SIP URI只是用来唯一标示用户的逻辑标记符，用户注册时它将与用户的IP地址绑定并记录到服务器。



## voip呼叫过程

### INVITE

```
INVITE sip:marconi@radio.org SIP/2.0
    <＝ 请求方法、请求地址（Request－URI）、SIP 版本号（目前都是 SIP/2.0）
        <＝ 请求地址一般就是被叫方地址，跟 MSN 中好友 eMail 地址类似

Via: SIP/2.0/UDP lab.high-voltage.org:5060;branch=z9hG4bKfw19b
    <＝SIP 版本号（2.0）、传输类型（UDP）、呼叫地址、
        <＝branch是一随机码，它被看作传输标识
        <＝Via 字段中地址是消息发送方或代理转发方设备地址，一般由主机地址和端口号组成
        <＝传输类型可以为 UDP、TCP、TLS、SCTP

Max-Forwards: 70
    <＝最大跳跃数，就是经过 SIP 服务器的跳跃次数，主要是防止循环跳跃
    <＝每尽管一台代理服务器，该整数减一

To: G. Marconi <sip:Marconi@radio.org>
From: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341
    <＝表示请求消息的发送方和目标方
        <＝如果里面有用户名标签，地址要求用尖括号包起来
        <＝对于 INVITE 消息，可以在 From 字段中包含 tag，它也是个随机码

Call-ID: 123456789@lab.high-voltage.org
    <＝呼叫ID是由本地设备生成的，全局唯一值。每次呼叫该值唯一不变
        <＝对于用户代理发送 INVITE 消息，本地将生成 From tag 和 Call-ID 全局唯一码，被叫方代理则生成 To tag 全局唯一码。这三个随机码做为整个对话中对话标识（dialog indentifier）在通话双方使用。

 

CSeq: 1 INVITE
    <＝CSeq，又叫命令队列（Command Seqence），每发送一个新的请求，该数自动加1
* 以上几个字段是所有 SIP 消息体所必须的，其它头字段有些是可选的，有些在特定请求也是必须

Subject: About That Power Outage...
Contact: <sip:n.tesla@lab.high-voltage.org>
    <＝Contact 是 INVITE 消息所必须的，它用来路由到被叫设备地址，也称为用户代理（UA）
Content-Type: application/sdp
Content-Length: 158
    <＝最后两位附属字段说明消息体类型以及字段长度

 

v=0    <＝SDP版本号，目前都是 0
o=Tesla 2890844526 2890844526 IN IP4 lab.high-voltage.org    <＝主叫源地址，类型等
s=Phone Call    <＝主题
c=IN IP4 100.101.102.103    <＝连接
t=0 0    <＝ 时间戳
m=audio 49170 RTP/AVP 0   <＝媒体
a=rtpmap:0 PCMU/8000    <＝媒体属性

    <＝从上面 SDP 消息体我们可以得出下面信息
        <＝连接 IP 地址：100.101.102.103
        <＝媒体格式：audio
        <＝端口号：49170
        <＝媒体传输类型：RTP
        <＝媒体编码：PCM u Law
        <＝采样率：8000 Hz
```

### 180 Ringing

```
SIP/2.0 180 Ringing
Via: SIP/2.0/UDP lab.high-voltage.org:5060;branch=z9hG4bKfw19b
;received=100.101.102.103    <＝这里增加一个 received 参数，标识接收方 IP 地址
To: G. Marconi <sip:marconi@radio.org>;tag=a53e42    <＝上已提到，To tag 做为被叫方标识
From: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341  <＝要求很发送方 From tag 一致
Call-ID: 123456789@lab.high-voltage.org
CSeq: 1 INVITE
Contact: <sip:marconi@tower.radio.org>
Content-Length: 0
    <＝对于 180 Ringing 响应，基本上就是将 INVITE 的 Via、To、From、Call-ID 和 CSeq 内容复制过来，对于首行标出 SIP 版本号，响应代码（180）和动作原因（reason phrase）
    <＝注意这里 From 和 To 地址，因为它们用来指定呼叫方向，因此这里的 200 OK 响应并没有将地址对调，仍然保持原样。一点不同的是 To 头字段添加了由被叫方 Marconi 生成的 tag 标识
```

### 200 ok

```
SIP/2.0 200 OK
Via: SIP/2.0/UDP lab.high-voltage.org:5060;branch=z9hG4bKfw19b
;received=100.101.102.103
To: G. Marconi <sip:marconi@radio.org>;tag=a53e42
From: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341
Call-ID: 123456789@lab.high-voltage.org
CSeq: 1 INVITE
Contact: <sip:marconi@tower.radio.org>
Content-Type: application/sdp
Content-Length: 155
    <＝头字段部分基本同上

v=0
o=Marconi 2890844528 2890844528 IN IP4 tower.radio.org
s=Phone Call
c=IN IP4 200.201.202.203
t=0 0
m=audio 60000 RTP/AVP 0
a=rtpmap:0 PCMU/8000

    <＝从上面 SDP 消息体我们可以得出下面信息
        <＝终端 IP 地址：200.201.202.203
        <＝媒体格式：audio
        <＝端口号：60000
        <＝媒体传输类型：RTP
        <＝媒体编码：PCM u Law
        <＝采样率：8000 Hz
```

### ACK

```
ACK sip:marconi@tower.radio.org SIP/2.0
Via: SIP/2.0/UDP lab.high-voltage.org:5060;branch=z9hG4bK321g
Max-Forwards: 70
To: G. Marconi <sip:marconi@radio.org>;tag=a53e42
From: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341
Call-ID: 123456789@lab.high-voltage.org
CSeq: 1 ACK
Content-Length: 0
```

### BYE

```
BYE sip:n.tesla@lab.high-voltage.org SIP/2.0
Via: SIP/2.0/UDP tower.radio.org:5060;branch=z9hG4bK392kf
Max-Forwards: 70
To: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341
From: G. Marconi <sip:marconi@radio.org>;tag=a53e42
Call-ID: 123456789@lab.high-voltage.org
CSeq: 1 BYE
Content-Length: 0
```

### 200 OK

```
SIP/2.0 200 OK
Via: SIP/2.0/UDP tower.radio.org:5060;branch=z9hG4bK392kf
;received=200.201.202.203
To: Nikola Tesla <sip:n.tesla@high-voltage.org>;tag=76341
From: G. Marconi <sip:marconi@radio.org>;tag=a53e42
Call-ID: 123456789@lab.high-voltage.org
CSeq: 1 BYE
Content-Length: 0
```



## 路由

一个SIP消息每经过一个Proxy（包括主叫），都会被加上一个Via头域，当消息到达被叫后，Via头域就记录了请求消息经过的完整路径。被叫将这些Via头域原样copy到响应消息中（包括各Via的参数，以及各Via的顺序），然后下发给第一个Via中的URI，每个Proxy转发响应消息前都会把第一个Via（也就是它自己添加的Via）删除，然后将消息转发给新的第一个Via中的URI，直到消息到达主叫。



总的来说，如果有Route，request就应该根据Route发送，如果没有就根据Contact头域发送，如果连Contact都没有，就根据From头域发送。

#### 严格路由

这种路由机制在SIP协议的前身RFC 2534中定义，其机制非常简单。

要求接收方接收到的消息的request-URI必须是自己的URI(即发送方的Request-URI always contained URI of the next hop)，然后它会把第一个Route头域“弹”出来，并把其中的URI作为新的request-RUI，然后把该消息路由给该URI。

#### 松散路由

该路由机制较为灵活，也是SIP路由机制的灵魂所在，在RFC 3261中定义。

松散路由的Proxy的路由决策过程：

1. Proxy首先会检查消息的request-URI是不是自己属于自己所负责的域。如果是，它就会通过定位服务将该地址“翻译”成具体的联系地址并以此替换掉原来的request-URI；否则，它不会动request-URI。
2. Proxy检查第一个Route头域中的URI是不是自己的，如果是，则移除之。
3. Loose Router首先会检查Request URI是否为自己：如果不是，则不作处理；如果是，则取出Route字段的最后一个地址作为Request URI地址，并从Route字段中删去最后一个地址。
4. Loose Router其次会检查下一跳是否为Strict Router：如果不是，则不作处理；如果是，则将Request URI添加为Route的最后一个字段，并用下一跳Strict Router的地址更新Request URI。



Record-Route头域不用来路由，而只是起到传递信息的作用。

#### 示例

两个UE间有两个Proxy，U1 -> P1 -> P2 -> U2，并且两个Proxy都乐意添加Record-Route头域。

p1：p1.example.com

u2：u2.domain.com

- U1发出一个INVITE请求给P1（P1是U1的外拨代理服务器）

  ```
  INVITE sip:callee@domain.com SIP/2.0
  Contact: sip:caller@u1.example.com
  ```

- P1->P2

  P1不负责域domain.com，消息中也没有Route头域，因此通过DNS查询得到负责该域的Proxy的地址并且把消息转发过去。这里P1在转发前就添加了一个Record-Route头域，里面有一个lr参数，说明P1是一个松散路由器，遵循RFC3261中的路由机制。

  ```
  INVITE sip:callee@domain.com SIP/2.0
  Contact: sip:caller@u1.example.com
  Record-Route: <sip:p1.example.com;lr>
  ```

- P2->U2

  P2负责域domain.com，因此它通过定位服务得到callee@domain.com 对应的设备地址是callee@u2.domain.com ，因此用新的URI重写request-URI。消息中没有Route头域，因此它就把该消息转发给request-URI中的URI，转发前它也增加了一个Record-Route头域，并且也有lr参数。

  ```
  INVITE sip:callee@u2.domain.com SIP/2.0
  Contact: sip:caller@u1.example.com
  Record-Route: <sip:p2.domain.com;lr>
  Record-Route: <sip:p1.example.com;lr>
  ```

- U2响应

  位于u2.domain.com的被叫收到了该INVITE消息，并且返回一个200 OK响应。其中就包括了INVITE中的Record-Route头域。

  ```
  SIP/2.0 200 OK
  Contact: sip:callee@u2.domain.com
  Record-Route: <sip:p2.domain.com;lr>
  Record-Route: <sip:p1.example.com;lr>
  ```

  被叫此时也就有了自己的路由集：

  ​	 (<sip:p2.domain.com;lr>,<sip:p1.example.com;lr>)

  主叫也有了自己的路由集，只是跟被叫的是反序的：

  ​	(<sip:p1.example.com;lr>,<sip:p2.domain.com;lr>)

- 通话完毕

  - U1-->P1

    主叫先挂机，则主叫发出BYE请求：

    ```
    BYE sip:callee@u2.domain.com SIP/2.0
    Route: <sip:p1.example.com;lr>,<sip:p2.domain.com;lr>
    ```

  - p1-->p2

    P1收到该消息后，发现request-URI中的URI不属于自己负责的域，而消息有Route头域，并且第一个Route头域中的URI正是自己，因此删除之，并且把消息转发给新的第一个Route头域中的URI，也就是P2：

    ```
    BYE sip:callee@u2.domain.com SIP/2.0
    Route: <sip:p2.domain.com;lr>
    ```

  - p2->u2

    P2收到该消息后，发现request-URI中的URI不属于自己负责的域（P2负责的是domain.com，而不是u2.domain.com），第一个Route头域中的URI正是自己，因此删除之，此时已经没有Route头域了，因此就转发给了request-URI中的URI。

    被叫就会收到BYE消息：

    ```
    BYE sip:callee@u2.domain.com SIP/2.0
    ```



## 认证

Sip 可以采用 Basic 模式和摘要模式（Digest schema）。RFC 2617 专门对两种认证模式做了规定。RFC 1321 是MD5标准。

### 基本认证模式

客户向服务器发送请求，服务器返回401（未授权），要求认证。401消息的头里面带了challenge信息。realm用以区分要不同认证的部分。客户端收到401后，将用户名密码和challenge信息用BASE64加密形成证书，发送回服务器认证。

语法如下：

```
challenge   = "Basic" 	realm
credentials = "Basic" 	basic-credentials
```

示例：

```
认证头		WWW-Authenticate: Basic realm= "wang.guoliang2@zte.com.cn"
证书		Authorization: Basic QsdfgWGHffuIcaNlc2FtZQ
```

### 摘要访问认证

在客户发送请求后，收到一个401（未授权）消息，包含一个Challenge。消息里面有一个唯一的字符串：nonce，每次请求都不一样。客户将用户名密码和401消息返回的Challenge一起加密后传给服务器。

**响应头语法如下：**

```
challenge = "Digest" 	digest-challenge
digest-challenge = 1#( realm | [ domain ] | nonce | [opaque] |[stale] | [algorithm] | [qop-options] | [auth-param] )
domain            = "domain" "=" <"> URI ( 1*SP URI ) <">
URI               = absoluteURI | abs_path
nonce             = "nonce" "=" nonce-value
nonce-value       = quoted-string
opaque            = "opaque" "=" quoted-string
stale             = "stale" "=" ( "true" | "false" )
algorithm         = "algorithm" "=" ( "MD5" | "MD5-sess" | token )
qop-options       = "qop" "=" <"> 1#qop-value <">
qop-value         = "auth" | "auth-int" | token
```

realm：让客户知道使用哪个用户名和密码的字符串。不同的领域可能密码不一样。至少告诉用户是什么主机做认证，他可能会提示用哪个用户名登录，类似一个Email。
domain：一个URI列表，指示要保护的域。可能是一个列表。提示用户这些URI采用一样的认证。如果为空或忽略则为整个服务器。
nonce：随机字符串，每次401都不一样。跟算法有关。算法类似Base64加密：time-stamp H(time-stamp ":" ETag ":" private-key) 。time-stamp为服务器时钟，ETag为请求的Etag头。private-key为服务器知道的一个值。
opaque：服务器产生的由客户下去请求时原样返回。最好是Base64串或十六进制字符串。
auth-param：为扩展用的，现阶段忽略。

**授权头语法：**

```
credentials      = "Digest" digest-response
digest-response = 1#( username | realm | nonce | digest-uri | response | [ algorithm ] | [cnonce] |
                       [opaque] | [message-qop] | [nonce-count] | [auth-param] )
username         = "username" "=" username-value
username-value   = quoted-string
digest-uri       = "uri" "=" digest-uri-value
digest-uri-value = request-uri   ; As specified by HTTP/1.1
message-qop      = "qop" "=" qop-value
cnonce           = "cnonce" "=" cnonce-value
cnonce-value     = nonce-value
nonce-count      = "nc" "=" nc-value
nc-value         = 8LHEX
response         = "response" "=" request-digest
request-digest = <"> 32LHEX <">
LHEX             = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" | "a" | "b" | "c" | "d" | "e" | "f"
```

#### 示例

**401响应**

```
HTTP/1.1 401 Unauthorized
WWW-Authenticate: Digest
realm="testrealm@host.com",
qop="auth,auth-int",
nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093",
opaque="5ccc069c403ebaf9f0171e9517f40e41"
```

**再次请求**

```
Authorization: Digest username="Mufasa",
realm="testrealm@host.com",
nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093",
uri="/dir/index.html",
qop=auth,
nc=00000001,
cnonce="0a4f113b",
response="6629fae49393a05397450978507c4ef1",
opaque="5ccc069c403ebaf9f0171e9517f40e41"
```

### 认证和加密

#### 认证

认证（Authorization）的作用在于表明自己是谁，即向别人证明自己是谁。

相关的概念是MD5，用于认证安全。注意MD5仅仅是个hash函数而已，并不是用于加密。因为hash函数处理后的数据没法进行反向恢复，这样子的话别人没法盗取你认证身份的口令。

#### 加密

加密（Encryption）的作用在于对想传输的数据进行处理，在网络中即使被窃取也难以破解。加密的信息可以被破解，这需要一把钥匙——“密钥”。通过密钥，我们可以对数据进行加密和解密。

最有名的专用密钥加密系统就是数据加密标准(DES), 这个标准现在由美国国家安全局和国家标准与技术局来管理。另一个系统是国际数据加密算法(IDEA), 它比DES的加密性好, 而且需要的计算机功能也不怎么强。

#### SIP认证方式

Basic 模式和摘要（Digest）模式。

Basic方法使用的口令原文验证的方式，易被盗取，所以SIP已经摒弃这种方式。

Digest认证方案可以对口令进行MD5包装。一般来说，获取口令有两种方式：1.字典攻击，即使用轮询进行口令猜测的方法，如果口令简单比较危险；另一个方法是攻击服务器来获得口令，如果服务器把密码存储起来那样的话就可能被盗取。所以方法是服务器端不再存储密码原文，而是使用MD5包装起来，这样的话当经过MD5包装的认证信息过来后，比较存储的MD5数据则可知道用户的身份了。

#### SIP认证过程

##### UA/Proxy之间

首先当UAC给UAS/Proxy发请求时；如果UAS/Proxy需要认证信息，则回复401/407；这时UAC**通过回复信息来计算认证消息**，然后重新发送 请求；如果认证不通过的话则会继续收到401/407或403，这时UAC**必须不能再次使用刚才被拒绝的信任书进行尝试，需要重新生成请求**直至UA/Proxy认证通过。注意也可以第一次请求时就已经带有认证信息。

当UAC在接收到401（Unauthorized）或者407（ProxyAuthenticationRequired）应答之后，重新用它的信任书来提交请求，它必须增加Cseq头域的值，就像发送一个正常的新请求一样。（**新请求**）

如果**认证通过**的话，**UA应当把这个给特定To头域和”realm”字段的信任书cache起来**，以备给这个地址下一个请求时候使用。（**cache**）

**cache一个proxy的信任书**：

如果UA在给特定Call-ID的请求的401/407应答中，接收到一个Proxy-Authenticate头域，它应当合并对这个realm的信任书，并且为以后具有相同Call-ID的请求发送这个信任书。这些信任书必须在对话中被cache住；不过如果UA配置的是它自己的本地外发proxy，那么如果出现要求认证的情况，那么UA应当cache住跨对话的信任书。注意，这个意味着在一个对话中的请求可以包含在Route头域中所经过proxy都不需要的信任书。
    当服务器可以正确处理绝大部分SIP请求，有本文档约定了两类请求要求特别的认证处理：ACK和CANCEL。在某一个认证方案下，并且这个认证方案是使用应答来放置计算nonces（比如Digest），那么对于某些没有应答的情况，就会出现问题，比如ACK。所以，基于这个原因，一个服务器接受在INVITE请求中的信任书，也必须同样接收对应ACK的信任书。UAC通过赋值所有的INVITE请求中的Authorization和Proxy-Authorization头域值来创建一个相关的ACK消息。服务器必须接收这个ACK请求。
    虽然CANCEL方法具有应答（2xx），服务器必须不能拒绝CANCEL请求，因为这些请求不能被重新提交。通常，如果CANCEL请求和被CANCEL的请求来自同一个节点（假设某种通讯协议，或者网络层有安全关系26.2.1节描述），服务器应当接收CANCEL请求。
     可见SIP为认证系统提供了一个无状态的，试错机制，这个认证机制式基于HTTP的认证机制的。通过请求和回复来验证用户身份。

#### SIP认证消息解析

具体为401中的WWW-Authenticate应答头域，相对应的为请求的Authorization头域；407中的Proxy-Authenticate头域，相对应的是请求的Proxy-Authorization头域。

- WWW-Authenticate/Proxy-Authenticate头域

  这个头域值包含了至少一个表明认证方式和适用realm的参数的拒绝原因。如：

  ```
  WWW-Authenticate: Digest
  realm="biloxi.com",
  qop="auth,auth-int",
  nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093",
  opaque="5ccc069c403ebaf9f0171e9517f40e41"
  ```

  - Realm

    realm字串单独定义被保护的区域。Realm字串必须是全局唯一的。我们强调这个realm字串必须包含一个主机名或者域名。Realm字串应当是一个可读的能够展示给用户的字串。
  通常，SIP认证对于特定realm（一个保护区域）是有意义的。因此，对于Digest认证来说，每一个类似的保护区域都有自己的用户名和密码集合。
  
- Nonce
  
  服务器端指定的数据字符，它应在每个401回应产生时，被唯一地创建。建议该字符以base64方式或16进制方式出现。另外，该字符在标题行中传递时是在引号内的，因此允许使用双引号字符。
  
- Stale
  
    一个标志，用来指示客户端先前的请求因其nonce值过期而被拒绝。如果stale是TRUE（大小写敏感），客户端可能希望用新的加密回应重新进行请求，而不用麻烦用户提供新的用户名和口令。服务器端只有在收到的请求nonce值不合法，而该nonce对应的摘要（digest）是合法的情况下（即客户端知道正确的用户名/口令），才能将stale置成TRUE值。如果stale是FALSE或其它非TRUE值，或者其stale域不存在，说明用户名、口令非法，要求输入新的值。
  
  - Algorithm
  
    Algorithm是个字符串，用来指示用来产生摘要及校验和的算法对。如果该域没指定，则认为是“MD5“算法。如果该域指定的算法无法理解，该质询（challenge）将被忽略。
  
  - qop
  
    “auth”表示鉴别方式；“auth-int”表示鉴别保护的完整性。
  
  - opaque
  
    由服务器指定的字符串，客户端不能改动它，如果并发请求的URI也指向同一个受保护区间，则该信息将被加在这些请求的授权标题域中返给服务器。建议采用base64或16进制的字符串。
  
- Authorization/Proxy-Authorization头域

  该头域包含了具有这个UA到请求的资源所在的realm（区域）的信任书和所需要的认证支持的参数和重现保护的参数。例如：

  ```
  Authorization: Digest username="bob",
  		realm="biloxi.com",
  		nonce="dcd98b7102dd2f0e8b11d0f600bfb0c093",
  		uri="sip:bob@biloxi.com",
  		qop=auth,
  		nc=00000001,
  		cnonce="0a4f113b",
  		response="6629fae49393a05397450978507c4ef1",
  		opaque="5ccc069c403ebaf9f0171e9517f40e41"
  ```

  如果服务器对特定请求没有要求认证或对于特定realm没有对应的认证信息，那么使用缺省的用户名,”anonymous”,并且这个用户名没有密码（密码是””）。

  根据RFC2617，合法的回应包含对用户名、口令、给定nonce值、SIP方法、请求URI的校验和（checksum，缺省是MD5的校验和）。

  - Response

    是个字符串，由32个经过计算的16进制数字组成，用来证明用户是否知道口令。

  - Cnonce

    当qop指示发送了，该指示必须要指定，而当服务器端没有在WWW-鉴别（WWW- Authenticate）标题域中添加qop指示时，该指示一定不能指定。cnonce-value是客户端提供的字符串，它由客户端和服务器共同使用，用来避免选择纯文本攻击、提供共同鉴别、提供某些消息的完整性保护。

#### 摘要的计算方法

128位的MD5摘要由32个可打印的ASCII码字符表示。128位摘要中的位按其重要性由高到低转换，在某个时刻每4位可用下面的ASCII表示。每4位都可用16进制字符‘0123456789abcdef’表示，也就是说，二进制0000由字符‘0’表示；0001由字符‘1’表示，以后如此类推，1111用‘f’表示。

##### KD(secret,data)

KD（secret,data）来表示摘要算法，其中data指数据，secret表示采用的方法.如果表示校验和算法时，data要写成H（data）；而unq（X）表示将带引号字符串的引号去掉。
简单来说:

```
response = H(H(username:realm:password)：nonce：cnonce：H(requestMothod:request-URI))
```

以下为详细的计算规则.
对于"MD5" 和"MD5-sess" 算法：
	H()：hash函数
	H(data) = MD5(data)
和
	KD(secret, data) = H(concat(secret, ":", data))
也就是说，摘要（digest）就是对secret与data通过冒号连接一起的结果进行MD5运算。而"MD5-sess"算法则允许其它第三方服务器参与鉴别。

#### 示例

```
hallenge = "Basic" realm 
credentials = "Basic" basic-credentials
```



```
认证头：WWW-Authenticate: Basic realm="zhouhh@mydomain.com" 
证书：Authorization: Basic QsdfgWGHffuIcaNlc2FtZQ==
```

