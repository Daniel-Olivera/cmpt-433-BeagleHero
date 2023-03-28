#ifndef TIME_H_
#define TIME_H_

//Timer Module:
//provides a number of useful timing functions.
//1 - the ability to record a time and check the time elapsed since
//2 - access to a simple nanosleep measured in milliseconds.

void Timer_init(void);

void Timer_startTimer(void);
long long Timer_checkTimerInMS(void);
void Timer_stopTimer(void);

void Timer_sleepForMs(long long delayInMs);

long long Timer_getTimeInMs(void);

#endif