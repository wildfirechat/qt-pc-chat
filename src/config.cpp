#include "config.h"


//应用服务地址，默认是8888端口的。上线时建议添加https支持。
//const QString APP_SERVER_ADDRESS = "http://wildfirechat.net:8888";
//NSString *APP_SERVER_ADDRESS = @"http://[2409:8a00:32c0:1ee0:702d:f0c0:2e1b:4d10]:8888"; //ipv6地址要用这种方式
const QString APP_SERVER_ADDRESS = "https://app.wildfirechat.net";

//二维码地址的前缀，注意需要和移动端同步修改。确保PC生成的二维码移动端能够识别。
const QString QR_CODE_PREFIX_PC_SESSION = "wildfirechat://pcsession/";
