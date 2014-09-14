#include <stdlib.h>
#include <string.h>
#include "thread_sort.h"

void *th_sort(void *th_msg) {
    pthread_t th_lo, th_hi;
    msg *m = (msg *)th_msg;
    size_t lo = m->lo;
    size_t hi = m->hi;
    size_t mid = (lo + hi) / 2;

    if (hi - lo > 2000) {
        msg *lo_msg = malloc(sizeof(msg));
        lo_msg->lo = lo;
        lo_msg->hi = mid;
        lo_msg->fs = m->fs;

        pthread_create(&th_lo, null, th_sort, lo_msg);

        msg *hi_msg = malloc(sizeof(msg));
        hi_msg->lo = mid + 1;
        hi_msg->hi = hi;
        hi_msg->fs = m->fs;

        pthread_create(&th_hi, null, th_sort, lo_msg);

        pthread_join(th_lo, null);
        pthread_join(th_hi, null);

        fraction **fs = malloc((hi - lo + 1) * f_ptr);
        memcpy(fs, &(m->fs[lo]), (hi - lo + 1) * f_ptr);

        size_t curr = lo;
        size_t i = 0;
        size_t j = mid - lo;
        for (curr; curr <= hi; curr++) {
            if (i <= mid - lo && f_lt(fs[i], fs[j])) {
                m->fs[curr] = fs[i];
                i++;
            } else {
                m->fs[curr] = fs[j];
                j++;
            }
        }

        free(fs);
    } else {
        fraction **fs = malloc((hi - lo + 1) * f_ptr);
        memcpy(fs, &(m->fs[lo]), (hi - lo + 1) * f_ptr);

        msg *lo_msg = malloc(sizeof(msg));
        lo_msg->lo = lo;
        lo_msg->hi = mid;
        lo_msg->fs = fs;

        pthread_create(&th_lo, null, ins_sort, lo_msg);

        msg *hi_msg = malloc(sizeof(msg));
        hi_msg->lo = mid + 1;
        hi_msg->hi = hi;
        hi_msg->fs = fs;

        pthread_create(&th_hi, null, ins_sort, lo_msg);

        pthread_join(th_lo, null);
        pthread_join(th_hi, null);

        size_t curr = lo;
        size_t i = 0;
        size_t j = mid - lo;
        while (i <= mid - lo && j <= hi - mid) {
            if (f_lt(fs[i], fs[j])) {
                m->fs[curr++] = fs[i++];
            } else {
                m->fs[curr++] = fs[j++];
            }
        }

        free(fs);
    }
}

void *ins_sort(void *th_msg) {
    size_t lo = ((msg *)th_msg)->lo;
    size_t hi = ((msg *)th_msg)->hi;
    fraction **fs = ((msg *)th_msg)->fs;

    size_t curr;
    for (curr = lo; curr <= hi; curr++) {
        size_t i;
        size_t min = curr;
        for (i = curr + 1; i < hi; i++) {
            min = f_lt(fs[i], fs[min]) ? i : min;
        }

        if (min != curr) {
            fraction *f = fs[min];
            fs[min] = fs[curr];
            fs[curr] = f;
        }
    }
}
