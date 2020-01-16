## SIP

SIP使用统一资源定位符 URI 来表示用户，通常由一个域名加上用户名组成，如 sip:John@eastcom.com

SIP URI只是用来唯一标示用户的逻辑标记符，用户注册时它将与用户的IP地址绑定并记录到服务器。

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

