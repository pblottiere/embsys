#ifndef DECODER_H
#define DECODER_H

int is_gll(char * frame);
int get_time(char * frame);
float get_latitude(char * frame);
float get_longitude(char * frame);

#endif
