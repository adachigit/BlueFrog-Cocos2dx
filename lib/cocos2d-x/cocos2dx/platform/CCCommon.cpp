
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "CCCommon.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include <Foundation/NSString.h>
#endif

NS_CC_BEGIN

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

static char logbuff[sizeof(char) * (kMaxLogLen + 1)];

void CCLog(const char * pszFormat, ...)
{
    printf("Cocos2d: ");
    va_list ap;
    va_start(ap, pszFormat);
    memset(logbuff, 0, sizeof(logbuff));
    vsnprintf(logbuff, kMaxLogLen, pszFormat, ap);
    va_end(ap);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    NSString* log = [NSString stringWithUTF8String:logbuff];
    printf("%s\n", [log UTF8String]);
#else
    printf("%s", logbuff);
    printf("\n");
#endif
    fflush(stdout);
}

void CCLuaLog(const char * pszLog)
{
    printf("Cocos2d: ");
    if (strlen(pszLog) > 65536)
    {
        printf("Cocos2d: [WARNING] log too long, max size is %0.2fKB, actual size is %0.2fKB\n", (float)kMaxLogLen / 1024.0f, (float)strlen(pszLog) / 1024.0f);
        memset(logbuff, 0, sizeof(logbuff));
        strncpy(logbuff, pszLog, kMaxLogLen);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        NSString* log = [NSString stringWithUTF8String:logbuff];
        printf("%s\n", [log UTF8String]);
#else
        puts(logbuff);
#endif
}
    else
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    NSString* log = [NSString stringWithUTF8String:pszLog];
    printf("%s\n", [log UTF8String]);
#else
        puts(pszLog);
#endif
}
}

#endif

NS_CC_END
