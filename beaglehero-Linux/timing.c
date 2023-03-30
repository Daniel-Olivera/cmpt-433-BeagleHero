#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

static long long timerStartedAtTime;
static bool timerRunning;

void Timer_init() 
{
    timerStartedAtTime = 0;
    timerRunning = false;
}

long long Timer_getTimeInMs(void) 
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoseconds = spec.tv_nsec;
    long long milliseconds = seconds * 1000 + nanoseconds / 1000000;

    return milliseconds;
}


void Timer_startTimer()
{
    timerStartedAtTime = Timer_getTimeInMs(); 
    timerRunning = true;
}

void Timer_startTimerFrom(long long startTimeInMs)
{
    timerStartedAtTime = startTimeInMs; 
    timerRunning = true;
}

long long Timer_checkTimerInMS()
{
    if(!timerRunning) {
        printf("Timer checked but not started.");
        exit(1);
    }
    long long currentTime = Timer_getTimeInMs();

    return currentTime - timerStartedAtTime;
}

void Timer_stopTimer()
{
    timerRunning = false;
}


void Timer_sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep (&reqDelay, (struct timespec *) NULL);
}

