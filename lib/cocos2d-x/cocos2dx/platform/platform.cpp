/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#include "platform.h"

#include "CCStdC.h"

NS_CC_BEGIN

int CCTime::gettimeofdayCocos2d(struct cc_timeval *tp, void *tzp)
{
    CC_UNUSED_PARAM(tzp);
    if (tp)
    {
        gettimeofday((struct timeval *)tp,  0);
    }
    return 0;
}

double CCTime::timersubCocos2d(struct cc_timeval *start, struct cc_timeval *end)
{
    if (! start || ! end)
    {
        return 0;
    }
    
    return ((end->tv_sec*1000.0+end->tv_usec/1000.0) - (start->tv_sec*1000.0+start->tv_usec/1000.0));
}

// add by weixin begin
TimeElapse::TimeElapse(const char* tag, float printLimit)
{
    m_tag = tag;
	m_printLimit = printLimit;
    reset();
}

TimeElapse::~TimeElapse()
{
    pause();
	if (m_totalMs >= m_printLimit)
	{
		printElapse();
	}
}

void TimeElapse::pause()
{
    if (m_isPause)
        return;
    struct cc_timeval tend;
    CCTime::gettimeofdayCocos2d(&tend, NULL);
    m_totalMs += (1000000*(tend.tv_sec-m_time.tv_sec)+(tend.tv_usec-m_time.tv_usec))/1000.0;
}

void TimeElapse::resume()
{
    if (!m_isPause)
        return;
    CCTime::gettimeofdayCocos2d(&m_time, NULL);
    m_isPause = false;
}

void TimeElapse::reset()
{
    m_totalMs = 0.0;
    m_isPause = true;
    resume();
}

void TimeElapse::printElapse()
{
    CCLog("%s elapse time: %f ms", m_tag, m_totalMs);
}
// add by weixin end

NS_CC_END
