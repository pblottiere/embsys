#ifndef UTIL_H
#define UTIL_H

/**
 * @param [in] frame  NMEA frame to analyse.
 *
 * @retval 0   The NMEA frame is a GLL frame.
 * @retval 1   The NMEA frame is not a GLL frame.
 *
 * Example Usage:
 * @code
 *    char frame[37] = "$GPGLL,4916.45,N,12311.12,W,225444,A";
 *    int rc = is_gll(frame); // Returns 0
 * @endcode
 */
int is_gll(char * frame);

/**
 * @param [in] frame  GLL NMEA frame to analyse.
 * @param [inout] time_str  Buffer where time (7 bytes) is stored.
 *
 * @retval time   The time as an integer.
 *
 * Example Usage:
 * @code
 *    char time_str[7];
 *    char frame[37] = "$GPGLL,4916.45,N,12311.12,W,225444,A";
 *    int time_value = get_time(frame, time_str); // Returns 225444
 * @endcode
 */
int get_time(char * frame, char * time_str);

#endif
