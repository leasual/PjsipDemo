## OPENSIPs

OpenSIPS是一个成熟的开源SIP服务器，除了提供基本的SIP代理及SIP路由功能外，还提供了一些应用级的功能。OpenSIPS的结构非常 灵活，其核心路由功能完全通过脚本来实现，可灵活定制各种路由策略，可灵活应用于语音、视频通信、IM以 及Presence等多种应用。同时OpenSIPS性能上是目前最快的SIP服务器之一，可用于电信级产品构建。

### 功能特点

凭借其可扩展、模块化的系统架构，OpenSIPS提供了一个高度灵活的、用户可配置的路由引擎，可以为voice、video、IM和 presence等服务提供强大高效的路由、鉴权、NAT、网关协议转化等功能。由于其稳定高效等特点，OpenSIPS已经被诸多电信运营商应用在自己的网络体系中。其 主要功能如下：

- SIP注册服务器/代理服务器（lcr、dynamic routing、dialplan）/重定向服务器
- SIP presence agent
- SIP B2BUA
- SIP IM Server
- SIP to SMS/XMPP网关
- SIP to XMPP网关
- SIP 负载均衡
- SIP NAT traversal

### 基本应用配置

OpenSIPS不但提供了丰富的功能，还具有操作简单的特点。所有OpenSIPS的应用功能都可以通过一个配置文件opensips.cfg来 实现的。该配置文件主要分为三个部分：

- 第一部分主要是全局变量的设置
- 第二部分主要是加载模块，并设置模块的相应参数
- 第三部分主要是路由的策略和功能应 用。

### 系统架构

OpenSIPS的架构开放灵活，其核心功能控制均可通过脚本控制实现，各个功能也通过模块加载的方式来构建。采用lex和yacc工具构建的配置 文件分析器是其架构设计中的重要部分之一。通过这个分析器，opensips设计了自己的语法规则，使得我们可以适合SIP规 范的语言来进行配置文件中的脚本编写，从而达到简化程序以及方便代码阅读的目的。同时这样的设计也使opensips.cfg配置文件的执行速度达到了C 语言的级别。

![](.\png\opensips系统架构.jpg)

框架的最上层是用于实现sip消息路由逻辑的opensips.cfg脚本配置，在配置文件中，可以使用Core提供的Parameter和 Function，也可以使用众多Modules提供的Function。比如在之前的负载均衡示例中，is_method(“INVITE”)就属于 textops模块提供的功能，src_ip和src_port都属于Core提供的参数。下层，提供了网络传输、sip消息解析等基本功能。在左侧，通过相应的数据库适配器，可以使用多种数据库存取数据。在这样的 体系结构下，我们就可以方便地通过增加功能module来添加我们需要的功能，而不会对原有系统造成影响。

OpenSIPS还提供了一系列的管理维护命令的接口。我们可以通过Core和Module提供的MI管理 接口，方便的监控系统以及模块的状态。比如，通过Core的fifo ps命令，可以获取当前进程的状态；通过Core的fifo get_statistics命令，可以获得当前共享内存以及各进程私有内存的使用情况等等。通过MI管理接口，我们还可以方便地在运行时修改部分参数， 比如，对于load_balancer模块，我们可以通过fifo lb_reload命令，更新目标组的配置信息，可以通过fifo lb_status命令激活或关闭某个目标，这些命令在实际应用中都非常实用。如果希望通过WEB图形界面管理OpenSIPS，OpenSIPS社区还 提供了OpenSIPS Control Panel 4.0产品。

### 与其他开源项目共同搭建VOIP服务

OpenSIPS并不具备一个媒体服务器(Media Server)的功能。媒体服务器主要提供了类似VoiceMail、呼叫中语音交换、会议服务、视频服务等一系列和语音、视频相关的服务；而 OpenSIPS的主要功能主要在于代理、路由和网关。因此，单独的OpenSIPS并不能够提供VOIP服务，只有和Asterisk等具备媒体功能的软件整合，才能构建可靠的语音服务体系。

对于媒体服务器，开源世界也提供了很多选择，如老牌的Asterisk，以及功能全面的sipXecs以及专注于IVR功 能的FreeSwitch等，他们都是非常优秀的开源项目。其中Asterisk功能全面、灵活，但主要面向企业应用，在性能上稍差。但Asterisk 提供了完善的PBX功能，可以连接多种不同的电话终端，支持多种主流的IP电话协议和系统接口。FreeSwtich专注于IVR功能，性能、可靠性非常高。近期 FreeSwitch已被sipXecs采用作为其IVR部分功能。sipXecs则是一个功能比较全面的产品，包括IVR、VoiceMail、人工坐 席等等，更难得的是SipXecs提供了良好的配置、管理界面，易于使用。

只要将OpenSIPS作为前置接入，将多个Asterisk、FreeSwitch、sipXecs挂接在其后，由OpenSIPS实现SIP消 息的转发和负载均衡，就可以轻松地实现各种语音业务以及规模扩展。

### 小结

OpenSIPS是一个成熟的电信级SIP Server平台，可广泛应用于SIP应用的路由分发、负载均衡，可用于搭建SIP代理，提供SIP注册服务等。而且目前OpenSIPS自身也提供 SIP Presence以及IM功能。同时，应该注意的是OpenSIPS本身并不提供媒体相关服务，如呼叫中心、VoiceMail等业务，该部分业务可通过 FreeSwtich、sipXecs等平台实现。



[OpenSIPS](http://www.opensips.org/)
[FreeSwitch wiki](http://wiki.freeswitch.org/wiki/Main_Page)
[sipXecs](http://www.sipfoundry.org/)

