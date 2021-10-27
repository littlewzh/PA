/*#include <am.h>
#include <nemu.h>
#include <amdev.h>
#include <sys/time.h>
#include <time.h>
//int gettimeofday(struct timeval *tv, void *tz);
static struct timeval boot_time = {};

void __am_timer_init() {

//gettimeofday(&boot_time, NULL);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  //uptime->us = 0;
  //uptime->us=getitimer();
 
  struct timeval now;
  gettimeofday(&now,NULL);
  //clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
  long seconds = now.tv_sec - boot_time.tv_sec;
  long useconds = now.tv_usec - boot_time.tv_usec;
  uptime->us = seconds * 1000000 + (useconds + 500);
}
  //now.tv_sec * 1000000 + now.tv_nsec / 1000;

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}*/
#include <am.h>
#include <amdev.h>
#include <nemu.h>
static uint64_t boot_time = 0;

#define taddr 0xa0000048

static uint64_t read_time() {
  uint32_t lo = inl(taddr);
  uint32_t hi = inl(taddr+4);
  uint64_t time = ((uint64_t)hi << 32) | lo;
  return time ;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = read_time();
}

void __am_timer_init() {
  boot_time = read_time();
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}

