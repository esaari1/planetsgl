#ifndef _TIME_FUNCS_H
#define _TIME_FUNCS_H

/**
 * Epoch is January 0, 1990.
 */
const int EPOCH_YEAR = 1990;

/**
 * Return true if year is a leap yesr, else false.
 */
bool isLeapYear(int year);
/**
 * Return day of year.
 * month and day are both 1 based (1 = January).
 */
int dayOfYear(int year, int month, int day);
/**
 * Returns number of days since epoch for current date.
 */
int daysSinceEpoch();
/**
 * Return number of days since epoch for specified time.
 */
int daysSinceEpoch(struct tm* timeInfo, long usec);

#endif
