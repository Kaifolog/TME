#include "logging.h"

FILE *log_file;

void log_init(const char *log_file_name)
{
    if (log_file = fopen(log_file_name, "w"))
    {
        log_message("Log_started");
    }
    else
        error("Log file error", 0);
}

void error(char *s, int d)
{
    time_t t = time(NULL);
    struct tm *aTm = localtime(&t);
    if (d)
    {
        fprintf(log_file, "%02d:%02d:%02d :: ERROR %s %d\n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s, d);
        fprintf(stderr, "%02d:%02d:%02d :: ERROR %s %d\n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s, d);
    }
    else
    {
        fprintf(log_file, "%02d:%02d:%02d :: %s \n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s);
        fprintf(stderr, "%02d:%02d:%02d :: %s \n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s);
    }
    fclose(log_file);
    getchar();
    exit(0);
}

void log_message(char *s)
{
    time_t t = time(NULL);
    struct tm *aTm = localtime(&t);
    fprintf(log_file, "%02d:%02d:%02d :: %s \n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s);
    printf("%02d:%02d:%02d :: %s \n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec, s);
}