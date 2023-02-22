#include "time_funcs.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>

bool isLeapYear(int year) {
   int modFour = year % 4;
   if(modFour != 0) {
      // Must be dvisible by 4
      return false;
   }

   int modHundred = year % 100;
   if(modHundred != 0) {
      // Any year not divisible by 100 is lea year
      return true;
   }
   
   // If divisible by 100, must also be divisible by 400
   int modFourHunded = year % 400;
   return (modFourHunded == 0);
}

int dayOfYear(int year, int month, int day) {
   if(month == 1) {
      return day;
   }

   if(month == 2) {
      return 31 + day;
   }

   int m = (int) floor((month + 1) * 30.6);
   if(isLeapYear(year)) {
      m = m - 62;
   }
   else {
      m = m - 63;
   }
   return m + day;
}

int daysSinceEpoch() {
   struct timeval currTime;
   gettimeofday(&currTime, NULL);
   struct tm *timeInfo = localtime(&currTime.tv_sec);
   return daysSinceEpoch(timeInfo, currTime.tv_usec);
}

int daysSinceEpoch(struct tm* timeInfo, long usec) {
   int year = timeInfo->tm_year + 1900;
   int days = timeInfo->tm_yday+1;

   if(year < EPOCH_YEAR) {
      for(int yr = year; yr < EPOCH_YEAR; yr++) {
         if(isLeapYear(yr)) {
            days = days - 366;
         }
         else {
            days = days - 365;
         }
      }
   }
   else {
      for(int yr = EPOCH_YEAR; yr < year; yr++) {
         if(isLeapYear(yr)) {
            days = days + 366;
         }
         else {
            days = days + 365;
         }
      }
   }

   return days;
}
