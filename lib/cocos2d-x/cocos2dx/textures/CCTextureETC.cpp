/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCTextureETC.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCFileUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif


NS_CC_BEGIN

CCTextureETC::CCTextureETC()
: _name(0)
, _width(0)
, _height(0)
, _alphaTex(NULL) // add by weixin
{}

CCTextureETC::~CCTextureETC()
{
    // add by weixin begin
    if (_alphaTex)
    {
        _alphaTex->release();
        _alphaTex = NULL;
    }
    // add by weixin end
}

bool CCTextureETC::initWithFile(const char *file)
{
    // Only Android supports ETC file format
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    bool ret = loadTexture(CCFileUtils::sharedFileUtils()->fullPathForFilename(file).c_str()); // comment by weixin
    bool ret = loadTexture(CCFileUtils::sharedFileUtils()->fullPathForFilename(file).c_str(), file);
    return ret;
#else
    return false;
#endif
}

unsigned int CCTextureETC::getName() const
{
    return _name;
}

unsigned int CCTextureETC::getAlphaName() const
{
    if (_alphaTex)
        return _alphaTex->getName();

    return 0;
}

unsigned int CCTextureETC::getWidth() const
{
    return _width;
}

unsigned int CCTextureETC::getHeight() const
{
    return _height;
}

// add by weixin begin
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define  LOG_TAG    "CCTextureETC.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif
// add by weixin end

// Call back function for java
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#define  LOG_TAG    "CCTextureETC.cpp"
//#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

static unsigned int sWidth = 0;
static unsigned int sHeight = 0;
static unsigned char *sData = NULL;
static unsigned int sLength = 0;

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxETCLoader_nativeSetTextureInfo(JNIEnv* env, jobject thiz, jint width, jint height, jbyteArray data, jint dataLength)
    {
        sWidth = (unsigned int)width;
        sHeight = (unsigned int)height;
        sLength = dataLength;
        sData = new unsigned char[sLength];
        env->GetByteArrayRegion(data, 0, sLength, (jbyte*)sData);
    }
}
#endif

// comment by weixin
//bool CCTextureETC::loadTexture(const char* file)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxETCLoader", "loadTexture", "(Ljava/lang/String;)Z"))
//    {
//        jstring stringArg1 = t.env->NewStringUTF(file);
//        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);
//        
//        t.env->DeleteLocalRef(stringArg1);
//        t.env->DeleteLocalRef(t.classID);
//        
//        if (ret)
//        {
//            _width = sWidth;
//            _height = sHeight;
//            
//            
//            glGenTextures(1, &_name);
//            glBindTexture(GL_TEXTURE_2D, _name);
//            
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            
//            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, sLength, sData);
//            
//            glBindTexture(GL_TEXTURE_2D, 0);
//            
//            delete [] sData;
//            sData = NULL;
//            
//            GLenum err = glGetError();
//            if (err != GL_NO_ERROR)
//            {
//                LOGD("width %d, height %d, lenght %d", _width, _height, sLength);
//                LOGD("cocos2d: TextureETC: Error uploading compressed texture %s glError: 0x%04X", file, err);
//                return false;
//            }
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//#else
//    return false;
//#endif
//}

// add by weixin begin
//bool CCTextureETC::loadTexture(const char* fullPath, const char* fileName)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxETCLoader", "loadTexture", "(Ljava/lang/String;)Z"))
//    {
//        jstring stringArg1 = t.env->NewStringUTF(fullPath);
//        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);
//        
//        t.env->DeleteLocalRef(stringArg1);
//        t.env->DeleteLocalRef(t.classID);
//        
//        if (ret)
//        {
//            LOGD("Trying to load etc1 texture: %s", fullPath); // add by weixin
//            _width = sWidth;
//            _height = sHeight;
//            
//            
//            glGenTextures(1, &_name);
//            glBindTexture(GL_TEXTURE_2D, _name);
//            
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            
//            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, sLength, sData);
//            
//            glBindTexture(GL_TEXTURE_2D, 0);
//            
//            delete [] sData;
//            sData = NULL;
//            
//            GLenum err = glGetError();
//            if (err != GL_NO_ERROR)
//            {
//                LOGD("width %d, height %d, lenght %d", _width, _height, sLength);
//                LOGD("cocos2d: TextureETC: Error uploading compressed texture %s glError: 0x%04X", fullPath, err);
//                return false;
//            }
//            
//            // add by weixin start
//            size_t len = strlen(fileName);
//            size_t len2 = len + strlen("_alpha") + 1;
//            char* alpha_file = new char[len2];
//            strcpy(alpha_file, fileName);
//            strcpy(alpha_file+len-4, "_alpha.pkm");
//            if (CCFileUtils::sharedFileUtils()->isFileExist(alpha_file))
//            {
//                _alphaTex = new CCTextureETC;
//                if (!_alphaTex->initWithFile(alpha_file))
//                {
//                    _alphaTex->release();
//                    _alphaTex = NULL;
//                }
//            }
//            
//            delete [] alpha_file;
//            
//            LOGD("Trying to load etc1 texture success: %s", fullPath);
//            // add by weixin end
//            
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//#else
//    return false;
//#endif
//}
// add by weixin end


// add by weixin begin
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class ETCHeader {
public:
    ETCHeader(unsigned char *data)
    {
        /*
         * Load from a ETC compressed pkm image file.
         * First 6 bytes are the name of the file format and version/packing type.
         * Bytes 7 and 8 are blank.
         */
        /* Beware endianess issues with most/least significant bits of the height and width. */
        paddedWidthMSB = data[8];
        paddedWidthLSB = data[9];
        paddedHeightMSB = data[10];
        paddedHeightLSB = data[11];
        widthMSB = data[12];
        widthLSB = data[13];
        heightMSB = data[14];
        heightLSB = data[15];
    }
    
    unsigned short getWidth(void)
    {
        return (widthMSB << 8) | widthLSB;
    }
    
    unsigned short getHeight(void)
    {
        return (heightMSB << 8) | heightLSB;
    }
    
    unsigned short getPaddedWidth(void)
    {
        return (paddedWidthMSB << 8) | paddedWidthLSB;
    }
    
    unsigned short getPaddedHeight(void)
    {
        return (paddedHeightMSB << 8) | paddedHeightLSB;
    }
    
    GLsizei getSize(GLenum internalFormat)
    {
        return (getPaddedWidth() * getPaddedHeight());
    }

private:
    unsigned char paddedWidthMSB;
    unsigned char paddedWidthLSB;
    unsigned char paddedHeightMSB;
    unsigned char paddedHeightLSB;
    unsigned char widthMSB;
    unsigned char widthLSB;
    unsigned char heightMSB;
    unsigned char heightLSB;
};

void getCompressedTextureFormats(GLint** textureFormats, int* numberOfTextureFormats)
{
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, numberOfTextureFormats);
    
    *textureFormats = (GLint *)calloc(*numberOfTextureFormats, sizeof(GLint));
    if(*textureFormats == NULL)
    {
        LOGD("Out of memory at %s:%i\n", __FILE__, __LINE__);
        return;
    }
    
    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, *textureFormats);
}

bool isETC1Supported()
{
    static bool hasDetected = false;
    static bool supportETC = false;
    if (hasDetected)
        return supportETC;

    GLint *textureFormat = NULL;
    GLint numberOfTextureFormats = 0;
    getCompressedTextureFormats(&textureFormat, &numberOfTextureFormats);

    for(int allTextureFormats = 0; allTextureFormats < numberOfTextureFormats; allTextureFormats++)
    {
        if (textureFormat[allTextureFormats] == GL_ETC1_RGB8_OES)
        {
            supportETC = true;
        }
    }
    
    free(textureFormat);
    textureFormat = NULL;
    
    if(!supportETC)
    {
        LOGD("Texture compression format GL_ETC1_RGB8_OES not supported\n");
    }
    
    hasDetected = true;
    
    return supportETC;
}

bool CCTextureETC::loadTexture(const char *fullPath, const char* fileName)
{
    LOGD("Trying to load etc1 texture: %s", fullPath);
    
    if (!isETC1Supported())
    {
        LOGD("ETC1 is not supported");
        return false;
    }
    
    unsigned long size;
    unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(fullPath, "rb", &size);
    if (data == NULL)
    {
        LOGD("Can not open the etc1 texture file: %s", fullPath);
        return false;
    }
    
    
    ETCHeader loadedETCHeader = ETCHeader(data);
    
    _width = loadedETCHeader.getWidth();
    _height = loadedETCHeader.getHeight();

    glGenTextures(1, &_name);
    glBindTexture(GL_TEXTURE_2D, _name);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, (loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight()) >> 1, data + 16);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    free(data);
    data = NULL;
    
    //
    size_t len = strlen(fileName);
    size_t len2 = len + strlen("_alpha") + 1;
    char* alpha_file = new char[len2];
    strcpy(alpha_file, fileName);
    strcpy(alpha_file+len-4, "_alpha.pkm");
    if (CCFileUtils::sharedFileUtils()->isFileExist(alpha_file))
    {
        _alphaTex = new CCTextureETC;
        if (!_alphaTex->initWithFile(alpha_file))
        {
            _alphaTex->release();
            _alphaTex = NULL;
        }
    }
    
    delete [] alpha_file;
    
    LOGD("Trying to load etc1 texture success: %s", fullPath);
    
    return true;
}

#endif

// add by weixin end

NS_CC_END
