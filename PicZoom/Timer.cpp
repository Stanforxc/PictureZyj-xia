#include "StdAfx.h"
#include "Timer.h"

Timer::Timer(void)
{
    m_nTimeRemaining = 0;
}

Timer::~Timer(void)
{
}
void Timer::SetMaxElapseTime( const int nMaxElapseTime_i,
                              const  bool bAppendTime_i )
{
    if( bAppendTime_i )
    {
        m_nTimeRemaining += nMaxElapseTime_i;
    }
    else
    {
        m_nTimeRemaining = nMaxElapseTime_i;
    }
}
void Timer::ElapseTime( const int nElapseTime )
{
    if( IsEnabled())
    {
        m_nTimeRemaining -= nElapseTime;
        if( 0 > m_nTimeRemaining )
        {
            // -Ve is not expected.
            m_nTimeRemaining = 0;
        }
    }
}
