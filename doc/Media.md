## Media

class pj::Media

由PJ :: AudioMedia子类化

### 公有函数

- Virtual ~Media（）

  ​	虚拟析构函数

- pjmedia_type getType（） const

  ​	获取媒体类型。

  ​	**返回**

  ​		媒体类型。

## AudioMedia

Class pj::**AudioMedia**

音频媒体。

从 PJ ::Media 继承

子类 PJ :: AudioMediaPlayer，PJ :: AudioMediaRecorder，PJ :: ToneGenerator

### 公有函数

- ConfPortInfo getPortInfo（） const

  ​	获取有关指定会议端口的信息。

- INT getPortId（） const

  ​	获取端口ID。

- void startTransmit（*const*  AudioMedia＆*sink*） const

  ​	建立单向媒体流（下沉）。

  ​	该媒体端口将作为源，它可以传输到多个目的地/汇。并且如果多个源传输到同一个接收器，则媒体将被混合在一起。源和接收器可以指同一媒体，有效地循环播放媒体。

  ​	如果需要双向媒体流，应用程序需调用此方法两次，第二个来自相对的源媒体。

  ​	**参数**sink -目标媒体。

- void stopTransmit（*const*  AudioMedia＆ *sink*） const

  ​	停止媒体流到目的地/汇聚端口。

  ​	**参数**sink -目的地媒体。

- void adjustRxLevel (float *level*) 

  ​	通过调整从媒体桥传输到介质端口的信号电平，使其更大或安静，。

  ​	**参数**  level - 信号电平调整。值1.0表示无电平调整，值0表示使端口静音。

- void adjustTxLevel(float *level*)

  ​	通过调整从该媒体端口（到桥接器）接收的信号电平，使其更大或更安静

  ​	**参数**level -信号电平调整。值1.0表示无电平调整，值0表示使端口静音。

- unsigned getRxLevel（） const

  ​	获取最后收到的信号电平。

  ​	**返回**信号电平百分比。

- unsigned getTxLevel（） const

  ​	获取最后传输的信号电平。

  ​	**返回**信号电平百分比。

- Virtual ~AudioMedia（）

  ​	虚拟析构函数

  ​	**公共静态功能**

- static  ConfPortInfo getPortInfoFromId（ INT *port_id* ）

  ​	从特定端口ID获取信息。

- static  AudioMedia * typecastFromMedia（媒体* *媒体*）

  ​	从基类媒体的类型转换。

  ​	这对于不支持向下转换的语言诸如Python编写的应用程序很有用。

  ​	**返回** 对象为 AudioMedia 实例

  ​	**参数**media -要被对待的对象

## AudioMediaPlayer

**Class** **pj::****AudioMediaPlayer**

音频媒体播放器。

继承PJ :: AudioMedia

### 公有函数

- AudioMediaPlayer（）

  ​	构造函数

- void createPlayer（*const* string &*file_name*, unsigned *options* = 0)

  ​	创建文件播放器，并自动将此播放器添加到会议桥。

  ​	**参数**file_name -要播放的文件名。目前仅支持WAV文件，WAV文件必须格式化为16bit PCM单声道/单声道（支持任何时钟频率）。	options -可选选项标志。应用程序可以指定PJMEDIA_FILE_NO_LOOP以防止回放循环。

- void createPlaylist（*const* StringVector &*file_names*, *const* string &*label* = "", unsigned *options* = 0)

  ​	创建文件播放列表媒体端口，并自动将端口添加到会议桥。

  ​	**参数**

  ​		file_names -要添加到播放列表的文件名数组。请注意，文件必须具有相同的时钟速率，通道数和每个样本的位数。

  ​		label -要为媒体端口设置的可选标签。

  ​		options -可选选项标志。应用程序可以指定PJMEDIA_FILE_NO_LOOP以防止循环。

- AudioMediaPlayerInfo getInfo（） const

  ​	获取有关播放器的其他信息。

  ​	此操作仅适用于播放器。对于播放列表，将抛出错误。

  ​	**返回**对应信息

- pj_uint32_t getPos（） const

  ​	获取当前播放位置。

  ​	此操作对播放列表无效。

  ​	**返回**当前播放位置

- void setPos（ pj_uint32_t *samples*）

  ​	在*samples*中设置播放位置。

  ​	此操作对播放列表无效。

  ​	**参数**

  ​		samples -样品中所需的播放位置。

- Virtual ~AudioMediaPlayer（）

  ​	析构器

- Virtual bool onEof（）

  ​	当文件播放器读取到文件结束或读取到达播放列表的最后一个文件结束时，注册要调用的回调。如果文件或播放列表设置为重复播放，则将多次调用回调。

  ​	**返回**如果回调返回false，则播放将停止。注意，如果应用程序在回调中破坏播放器，则必须在此返回false。

 

### 公共静态功能

- static  AudioMediaPlayer* typecastFromAudioMedia（AudioMedia * *media*）

  ​	从基类AudioMedia的类型转换。

  ​	这对于不支持向下转换的语言诸如Python编写的应用程序很有用

  ​	**返回**

  ​		AudioMediaPlayer对象实例

  ​	**参数**

  ​		media -要被转换的对象

##  AudioMediaRecorder

**class** **pj::****AudioMediaRecorder**

音频媒体记录器。

继承PJ :: AudioMedia

### 公有函数

- AudioMediaRecorder（）

  ​	构造函数

- void createRecorder（*const* string &*file_name*, unsigned *enc_type* = 0, pj_ssize_t *max_size* = 0, unsigned *options* = 0)

  ​	创建文件记录器，并自动将此录音机连接到会议桥。录音机目前支持录制WAV文件。要使用的录像机的类型由文件的扩展名（例如“.wav”）决定。

  ​	**参数**

  ​		file_name -输出文件名。该功能将根据文件扩展名确定要使用的默认格式。目前所有平台都支持“.wav”。

  ​		enc_type -如果文件可以支持不同的编码，可选择用于压缩介质的编码器类型。现在该值必须为零。

  ​		max_size -最大文件大小。 指定零或-1以删除大小限制。此值现在必须为零或-1。

  ​		options -可选选项，可用于指定录制文件格式。支持的选项有PJMEDIA_FILE_WRITE_PCM，PJMEDIA_FILE_WRITE_ALAW和PJMEDIA_FILE_WRITE_ULAW。默认值为零或PJMEDIA_FILE_WRITE_PCM。

- Virtual ~AudioMediaRecorder（）

  ​	析构器

### 公共静态功能

- static  AudioMediaRecorder* typecastFromAudioMedia（AudioMedia* *media*）

  ​	从 AudioMedia 类的类型转换。

  ​	这对于不支持向下转换的语言诸如Python编写的应用程序很有用

  ​	**返回** AudioMediaRecorder 对象实例

  ​	**参数**

  ​		media -要被转换的对象

  

## 格式和信息

- struct pj::MediaFormat

  ​	此结构包含完全描述媒体所需的所有信息。

  ​	子类 PJ :: MediaFormatAudio，PJ :: MediaFormatVideo

- struct pj::MediaFormatAudio

  该结构描述了有关音频媒体的详细信息。

  继承 PJ :: MediaFormat

- struct pj::MediaFormatVideo

  ​	该结构描述了有关视频媒体的详细信息。

  ​	继承 PJ :: MediaFormat

- struct pj::ConfPortInfo

  ​	此结构描述了已注册到会议桥中的特定媒体端口的信息。

## 音频设备框架

### 设备管理器

**class** **pj::AudDevManager**

音频设备管理器

#### 公有函数

- INT getCaptureDev（） const

  ​	获取当前活动的捕捉声音设备。

  ​	如果没有创建声音设备，则该功能可能返回-1作为设备ID。

  ​	**返回**捕获设备的设备ID。

- AudioMedia＆ getCaptureDevMedia()

  ​	获取 AudioMedia 捕获音频设备。

  ​	**返回**用于捕获设备的音频媒体。

- INT getPlaybackDev（） const

  ​	获取当前正在播放的声音设备。

  ​	如果没有创建声音设备，则该功能可能返回-1作为设备ID。

  ​	**返回**播放设备的设备ID。

- AudioMedia＆ getPlaybackDevMedia（）

  ​	获取 AudioMedia 扬声器/播放音频设备。

  ​	**返回**扬声器/播放设备的音频媒体。

- void setCaptureDev（ INT *capture_dev* ） const

  ​	选择或更改采集（捕获）声音的设备。

  ​	应用程序可以随时调用此功能来替换当前的声音设备。调用此方法不会更改声音设备的状态（打开/关闭）。注意，此方法将覆盖设定的模式 setSndDevMode()。

  ​	**参数**

  ​		capture_dev -捕获设备的设备ID。

- void setPlaybackDev（ INT *playback_dev* ） const

  ​	选择或更改播放声音设备

  ​	应用程序可以随时调用此功能来替换当前的声音设备。调用此方法不会更改声音设备的状态（打开/关闭）。注意，此方法将覆盖设定的模式 setSndDevMode()。

  ​	**参数**

  ​		playback_dev -播放设备的设备ID。

- const AudioDevInfoVector＆ enumDev（）

  ​	枚举系统中安装的所有音频设备。

  ​	**返回**

  ​	音频设备信息列表。

- void setNullDev（）

  ​	设置pjsua使用空声音设备。

  ​	零声音设备仅提供会议桥所需的时间，并且不会与任何硬件相互冲突。

- MediaPort * setNoDev（）

  ​	断开主会议桥与任何声音设备的连接，并让应用将网桥连接到自己的声卡/主端口。

  ​	**返回**

  ​	会议桥的端口接口，使应用程序可以将其连接到自己的声音设备或主端口。

- void setSndDevMode（unsigned *mode*) const

  ​	设置声音设备模式。

  ​	**参数**

  ​		mode -声音设备模式，作为#pjsua_snd_dev_mode的位掩码组合

- void setEcOptions（unsigned *tail_msec*, unsigned *options*)

  ​	更改回声消除设置。此功能的行为取决于声音设备当前是否处于活动状态，如果是，该设备或软件是否支持AEC。

  ​	如果声音设备当前处于活动状态，并且设备支持AEC，则此功能会将更改请求转发给设备，并且由设备决定是否支持该请求。如果使用软件AEC（如果设备不支持AEC，将使用软件EC），则此功能将更改软件EC设置。在所有情况下，将保存设置以备将来打开声音设备。

  ​	如果当前声音设备未处于活动状态，则仅会更改默认的AEC设置，并且下次打开声音设备时将进行设置。

  ​	**参数**

  ​		tail_msec -尾长度，以毫秒为单位。设置为零以禁用AEC。

  ​		options -要传递给pjmedia_echo_create（）的选项。通常值应为零。

- unsigned getEcTail（） const

  ​	获取当前的回声消除器尾部长度。

  ​	**返回**

  ​		EC尾部长度（以毫秒为单位），如果禁用AEC，则该值将为零。

- bool sndIsActive（） const

  ​	检查声音设备当前是否处于活动状态。

  ​	非活动状态的情况：

  ​	应用程序已经设置自动关闭功能（在sndAutoCloseTime设定声音设备可以是不活动 MediaConfig )

  ​	无声音设备

  ​	声音设备已经经由配置 setNoDev() 函数。

- void refreshDevs（）

  ​	刷新系统中安装的声音设备列表。

  ​	此方法只会刷新音频设备列表，因此所有活动音频流将不受影响。刷新设备列表后，在调用任何接受音频设备索引方法作为参数之前，应用程序必须确保更新对音频设备所有索引的更新。

- unsigned getDevCount（） const

  获取系统中安装的声音设备的数量。

  **返回**

  ​	安装在系统中的声音设备的数量。

- AudioDevInfo getDevInfo（ INT *ID* ） const

  ​	获取设备信息。

  ​	**返回**一旦返回成功，将通过此方法填写的设备信息。

  ​	**参数**

  ​		id -音频设备ID。

- int lookupDev（*const* string &*drv_name*, *const* string &*dev_name*) const

  ​	根据驱动和设备名称查找设备索引。

  ​	**返回**设备ID。如果未找到该设备，抛出错误。

  ​	**参数**

  ​		drv_name –驱动名称。

  ​		dev_name -设备名称。

- string capName（pjmedia_aud_dev_cap *cap*) const

  ​	获取指定功能的字符串信息。

  ​	**返回**Capability 名称

  ​	**参数**

  ​		cap -Capability ID。

- void setExtFormat（*const* MediaFormatAudio& *format*, bool *keep* = true)

  ​	配置音频格式（PCM除外）到正在使用的声音设备。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在设备具备 AudioDevInfo.caps 标志的PJMEDIA_AUD_DEV_CAP_EXT_FORMAT能力时才起作用。否则抛出错误

  ​	注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		format - 音频格式。

  ​		keep -指定设置是否保留以备将来使用。

- MediaFormatAudio getExtFormat() const

  ​	获取正在使用的声音设备的音频格式功能（PCM除外）。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备标志 AudioDevInfo.caps 的PJMEDIA_AUD_DEV_CAP_EXT_FORMAT时有效，否则抛出错误

  ​	音频格式。

- void setInputLatency（unsigned *latency_msec*, bool *keep* = true)

  ​	为正在使用的声音设备配置音频输入延迟控制或查询功能。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	仅在设备具有PJMEDIA_AUD_DEV_CAP_INPUT_LATENCY标志位（ AudioDevInfo.caps 标志）时，此方法有效，否则抛出错误

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		latency_msec -输入延迟。

  ​		keep -指定设置是否保留以备将来使用。

- unsigned getInputLatency（） const

  ​	获取正在使用的声音设备的音频输入延迟控制或查询功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_INPUT_LATENCY能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	**返回**

  ​		音频输入延迟。

- void setOutputLatency（unsigned *latency_msec*, bool *keep* = true)

  ​	为正在使用的声音设备配置音频输出延迟控制或查询功能。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在设备的 AudioDevInfo.caps 标志中包含PJMEDIA_AUD_DEV_CAP_OUTPUT_LATENCY时有效，否则抛出错误

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		latency_msec -输出延迟。

  ​		keep -指定设置是否保留以备将来使用。

- unsigned getOutputLatency（） const

  ​	获取正在使用的声音设备的音频输出延迟控制或查询功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备的 AudioDevInfo.caps 标志中包含PJMEDIA_AUD_DEV_CAP_OUTPUT_LATENCY时有效，否则抛出错误

  ​	**返回**

  ​		音频输出延迟。

- void setInputVolume（unsigned *volume*, bool *keep* = true)

  ​	为正在使用的声音设备配置音频输入音量级别。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		volume -输入音量级别，以百分比表示。

  ​		keep -指定设置是否保留以备将来使用。

- unsigned getInputVolume（） const

  ​	获取正在使用的声音设备的音频输入音量电平能力。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	**返回**

  ​		音频输入音量级别，以百分比表示。

- void setOutputVolume（unsigned *volume*, bool *keep* = true)[¶](http://www.pjsip.org/docs/book-latest/html/media.html#_CPPv2N2pj13AudDevManager15setOutputVolumeEjb)

  ​	为正在使用的声音设备配置音频输出音量级别。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_OUTPUT _VOLUME_SETTING能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		volume -输出音量级别，以百分比表示。

  ​		keep -指定设置是否保留以备将来使用。

- unsigned getOutputVolume（） const

  ​	获取正在使用的声音设备的音频输出音量级别功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_OUTPUT _VOLUME_SETTING能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	**返回**

  ​		音频输出音量级别，以百分比表示。

- unsigned getInputSignal（） const

  ​	获取正在使用的声音设备的音频输入信号电平能力。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_INPUT_SIGNAL_METER能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

​			**返回**

​				音频输入信号电平，以百分比表示。

- unsigned getOutputSignal（） const

  ​	获取正在使用的声音设备的音频输出信号电平能力。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备具有PJMEDIA_AUD_DEV_CAP_OUTPUT_SIGNAL_METER能力时有效（在 AudioDevInfo.caps 标志中），否则抛出错误。

  ​	**返回**

  ​		音频输出信号电平，以百分比表示。

- void setInputRoute（pjmedia_aud_dev_route *route*, bool *keep* = true)

  ​	这将为正在使用的声音设备配置音频输入路由功能。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在PJMEDIA_AUD_DEV_CAP_INPUT_ROUTE AudioDevInfo.caps 标志时有效，否则错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		route -音频输入路由。

  ​		keep -指定设置是否保留以备将来使用。

- pjmedia_aud_dev_route getInputRoute（） const

  ​	获取正在使用的声音设备的音频输入路由功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	仅在有PJMEDIA_AUD_DEV_CAP_INPUT_ROUTE标识时有效有效（AudioDevInfo.caps 标志），否则错误。

  ​	**返回**

  ​		音频输入路由。

- void setOutputRoute（pjmedia_aud_dev_route *route*, bool *keep* = true)

  ​	这将对正在使用的声音设备配置音频输出路由功能。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在具有PJMEDIA_AUD_DEV_CAP_OUTPUT_ROUTE标识时有效（AudioDevInfo.caps 标志），否则抛出错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		route -音频输出路由。

  ​		keep -指定设置是否保留以备将来使用。

- pjmedia_aud_dev_route getOutputRoute（） const

  ​	获取正在使用的声音设备的音频输出路由。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备有PJMEDIA_AUD_DEV_CAP_OUTPUT_ROUTE能力时有效（AudioDevInfo.caps 标志），否则抛出错误。

  ​	**返回**

  ​		音频输出路由。

- void `setVad`(bool *enable*, bool *keep* = true)

  ​	对正在使用的声音设备，设置音频语音活动检测能力。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在设备有PJMEDIA_AUD_DEV_CAP_VAD能力时起作用（AudioDevInfo.caps 标志），否则抛出错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于所有设备。

  ​	**参数**

  ​		enable -启用/禁用语音活动检测功能。设置为true以启用。

  ​		keep -指定设置是否保留以备将来使用。

- bool getVad（） const

  ​	获取正在使用的声音设备的音频声音活动检测功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	此方法仅在设备有PJMEDIA_AUD_DEV_CAP_VAD能力时起作用（AudioDevInfo.caps 标志），否则抛出错误。

  ​	**返回**

  ​		音频声音活动检测功能。

- void `setCng`(bool *enable*, bool *keep* = true)

  ​	这将为正在使用的声音设备配置舒适噪声生成的（comfort noise generation）能力。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	具有 AudioDevInfo.caps 的PJMEDIA_AUD_DEV_CAP_CNG 标志时有效，否则抛出错误 。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		enable -启用/禁用舒适噪声生成的能力。设置为true以启用。

  ​		keep -指定设置是否保留以备将来使用。

- bool `getCng`() const

  ​	获取正在使用的声音设备的音频降噪（comfort noise generation）能力。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	具有 AudioDevInfo.caps 的PJMEDIA_AUD_DEV_CAP_CNG 标志时有效，否则抛出错误。

  ​	**返回**

  ​		是否具备音频舒适噪声生成的功能。

- void `setPlc`(bool *enable*, bool *keep* = true)

  ​	为正在使用的声音设备配置音频包丢失隐藏（packet loss concealment ）功能。

  ​	如果声音设备当前处于活动状态，则该方法会将设置转发到要立即应用的声音设备实例（如果它支持）。

  ​	此方法仅在PJMEDIA_AUD_DEV_CAP_PLC配置时有效（AudioDevInfo.caps 标志中），否则抛出错误。

  ​	请注意，如果设置保留将来使用，即使应用程序更改了要使用的声音设备，也将应用于任何设备。

  ​	**参数**

  ​		enable 		-启用/禁用丢包隐藏功能。设置为true以启用。

  ​		keep 			-指定设置是否保留以备将来使用。

- bool getPlc（） const

  ​	获取正在使用的声音设备的音频包丢失隐藏功能。

  ​	如果声音设备当前处于活动状态，则该方法会将该请求转发到声音设备。如果声音设备当前处于非活动状态，并且如果应用程序以前设置了设置并将设置标记为保留，则该设置将被返回。否则，这种方法会引起错误。

  ​	设备需具备PJMEDIA_AUD_DEV_CAP_PLC能力，才能起作用（AudioDevInfo.caps 标志），否否则抛出错误。

  ​	**返回**

  ​		音频丢包隐藏功能。

 

## AudioDevInfo 

设备信息

struct pj::AudioDevInfo

音频设备信息结构。