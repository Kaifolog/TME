/*
    SVWM - simple vector without macroses
*/

#ifndef SVWM_DIFFERENCE
#define SVWM_DIFFERENCE 5
#endif

void *svwm_vector(int size, int size_of_instance);

int svwm_free(void *ptr);

int svwm_bytesize(void *ptr);

int svwm_size(void *ptr);

int svwm_maxsize(void *ptr);

int svwm_capacity(void *ptr);

void *svwm_insert(void *ptr, int position, void *instance);

int svwm_erase(void *ptr, int position);

void *svwm_pushback(void *ptr, void *instance);

void svwm_popback(void *ptr);

/****************************************************************/ //standart functions for vector class

void *svwm_pb_int(void *ptr, int instance);

void *svwm_pb_char(void *ptr, char instance);

void *svwm_pb_charptr3(void *ptr, char **instance);
