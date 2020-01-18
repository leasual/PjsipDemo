## 技术要点

- SIP
- WebRTC
- TCP
- UDP
- RTP
- RTSP
- TLS
- NAT
- STUN
- TURN
- ICE(interactive Connectivity Establishment)
- OPENH264
- MediaCodec

## 开发流程

- pjsip 客户端

- 搭建 VOIP 服务器

  配置 server ip

- 搭建 turn 服务器

  stun 无法穿透对称型 NAT，需要用 turn



## SIP 学习

- 抓包
- [IETF](https://www.ietf.org/)



## SIP开源软件产品

服务端软件

- Asterisk

  标准 C 程序实现，代码清晰。

- SipXecs

- FreeSWITCH

- OpenSIPS

- MiniSipServer

客户端软件

- X-lite
- LinPhone
- eysBeam



## STUN TURN Server

### 免费的

- [restund](http://www.creytiv.com/restund.html)

- [rfc5766-turn-server](https://github.com/coturn/rfc5766-turn-server/)

  支持 STUN、TURN

- [PJSIP](http://www.pjsip.org)

  支持 STUN、TURN、ICE

- [libnice](https://nice.freedesktop.org.wiki/)

  支持 ICE 和 STUN

- [STUNMAN](http://www.stunprotocol.org)

### 开源的

- [https://github.com/webrtc/samples](https://yq.aliyun.com/go/articleRenderRedirect?spm=a2c4e.11153940.0.0.75b66f10OM0EBe&url=https%3A%2F%2Fgithub.com%2Fwebrtc%2Fsamples)

对应的demo在这里（Chrome打开可以看效果）：

- [https://webrtc.github.io/samples](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fwebrtc.github.io%2Fsamples)

回退一级，还有更多资源：

- [https://github.com/webrtc](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fgithub.com%2Fwebrtc)

还有一个，SimpleWebRTC：

- [https://github.com/andyet/SimpleWebRTC](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fgithub.com%2Fandyet%2FSimpleWebRTC)

还有：

- [https://github.com/mgechev/angular-webrtc](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fgithub.com%2Fmgechev%2Fangular-webrtc)

嗯，接下来这个更赞，支持多人视频会议，开源的：

- [https://github.com/jitsi](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fgithub.com%2Fjitsi)，对应的演示地址[https://meet.jit.si/](https://yq.aliyun.com/go/articleRenderRedirect?spm=a2c4e.11153940.0.0.75b66f10OM0EBe&url=https%3A%2F%2Fmeet.jit.si%2F)

世界上第一个基于[HTML5](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Flib.csdn.net%2Fbase%2Fhtml5)的SIP客户端：[https://www.doubango.org/sipml5/](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fwww.doubango.org%2Fsipml5%2F)。他们的GitHub主页：[https://github.com/DoubangoTelecom/doubango](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fgithub.com%2FDoubangoTelecom%2Fdoubango)。

# 在线演示

搜集了一些在线演示的示例：

- [https://apprtc.appspot.com](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fapprtc.appspot.com%2F)
- [http://www.simpl.info/getusermedia](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Fwww.simpl.info%2Fgetusermedia)
- [https://webrtc.github.io/samples](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fwebrtc.github.io%2Fsamples)
- [http://webcamtoy.com/app/](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Fwebcamtoy.com%2Fapp%2F)
- [http://www.shinydemos.com/facekat/](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Fwww.shinydemos.com%2Ffacekat%2F)
- [http://idevelop.ro/ascii-camera/](https://yq.aliyun.com/go/articleRenderRedirect?url=http%3A%2F%2Fidevelop.ro%2Fascii-camera%2F)
- [https://meet.jit.si/](https://yq.aliyun.com/go/articleRenderRedirect?url=https%3A%2F%2Fmeet.jit.si%2F)，多人的视频会议



## VoIP应用程序和资源

### H.323客户端

#### 多平台

- FreeSWITCH
- YATE

#### Linux

- Ekiga

#### MacOS X

- XMeeting

#### Windows

- OpenH323 Project

  

### H.323 Gatekeeper

- OpenH323 Gatekeeper



### H.323 Radius Platform

- RSDRadius

### SIP客户端

#### 多平台

- SFLphone
- Linphone
- Minisip
- OpenWengo
- PhoneGaim
- sipXtapi
- OpenZoep

#### Linux

- Cockatoo
- YeaPhone
- Twinkle

#### Windows

- 1videoConference

### Sip代理

- Open Source SIP
- Partysip
- MjSip
- OpenSER
- SIP Express Router
- Siproxd

### Sip协议栈和库

- OpenSIPStack
- The GUN oSIP library
- The eXtended osip Library
- Vovida SIP Stack
- reSIProcate
- Twisted
- PJSIP

### SIP Test Tool

- Callflow
- SipBomber 0.8
- SIP Proxy
- sipsak
- SIPp
- PROTOS Test-Suite:c07-sip
- Vovida.org Load Balancer

### IAX Clients

- IAXComm
- Kiax
- QtIAX
- MozIAX
- YakaSoftware

### IAX/PBX Library

- IAXClient

### RTP Proxies

- Maxim Sobolev's RTPproxy

### RTP Protocol Stacks

- JRTPLIB
- oRTP
- GUN ccRTP
- Vovida RTP Stack
- RTPlib

### PBX Platform

- Asterisk
- OpenPBX.org 1.2 RC3
- Open Source Software
- PBX4Linux
- SIPxchange
- sipX

### IVR Platform

- GUN Bayonne
- CT Server

### Voicemail Apps

- lintad
- Linux Voicemail/OpenUMS
- VOCP System
- OpenVXI

### Speech Software

- The Festival Speech Synthesis System
- OpenSALT
- CMU Sphinx Projects

### Fax Servers

- HylaFAX
- AstFax

### Development Stacks

- OpenSS7
- ooh323c
- ++Skype Library
- OpenBlox

### Middleware

- MobiCent
- Ernie
- SIP Thor







### FreeSWITCH

H.323客户端。

可以作为一个简单的切换引擎、媒体网关或媒体服务器来托管 IVR 应用程序，使用简单的脚本或 XML 来控制调用流。 Freeswitch 运行在几个操作系统上，包括 Windows、 Max OS x、 Linux、 BSD 和 Solaris，都运行在32位和64位平台上。 注意:FreeSWITCH 也是多协议的，因为它可以与 SIP、 IAX2和 GoogleTalk 协同工作，从而可以轻松地与其他开源 PBX 系统进行接口。