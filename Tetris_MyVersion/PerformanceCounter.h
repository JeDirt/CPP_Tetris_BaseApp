#include <windows.h>

class CStopwatch
{

public:


	CStopwatch();


public:


	void Start();
	int Now();
	int Time();


private:


	LARGE_INTEGER m_liPerfFreq;
	LARGE_INTEGER m_liPerfStart;


};

#ifdef MY_PERFORMENCE_COUNTER


CStopwatch::CStopwatch()
{

    QueryPerformanceFrequency(&m_liPerfFreq);
    Start();

}
void CStopwatch::Start()
{
    QueryPerformanceCounter(&m_liPerfStart);
}


// Returns the number of miliseconds after Start call
int CStopwatch::Now()
{

    LARGE_INTEGER liPerfNow;
    QueryPerformanceCounter(&liPerfNow);

	return (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart);

}


int CStopwatch::Time()
{

    LARGE_INTEGER liPerfNow;
	QueryPerformanceCounter(&liPerfNow);

	return ((liPerfNow.QuadPart * 1000) / m_liPerfFreq.QuadPart);

}


#endif
