#pragma once

/*
This class handles Timer functionality. The maximum time is set 
by SetMaxElapseTime(), and this ElapseTime() reduces time.
*/
class Timer
{
public:
    Timer(void);
    ~Timer(void);
    void SetMaxElapseTime( const int nMaxElapseTime_i,
                           const bool bAppendTime_i = false );
    void ElapseTime( const int nElapseTime = 1 );
    bool IsEnabled()
    {
        return ( 0 < m_nTimeRemaining );
    }
    int GetRemainingTime()
    {
        return m_nTimeRemaining;
    }

private:

    int m_nTimeRemaining;
};
