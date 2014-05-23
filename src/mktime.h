/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  time.h - time header file.                                       */
/*                                                                   */
/*********************************************************************/

#include <time.h>

time_t p_time(time_t *timer);
clock_t p_clock(void);
double p_difftime(time_t time1, time_t time0);
time_t p_mktime(struct tm *timeptr);
char *p_asctime(const struct tm *timeptr);
char *p_ctime(const time_t *timer);
struct tm *p_gmtime(const time_t *timer);
struct tm *p_localtime(const time_t *timer);
size_t p_strftime(char *s, size_t maxsize,
                const char *format, const struct tm *timeptr);

