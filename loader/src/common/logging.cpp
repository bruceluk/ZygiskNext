#include <android/log.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "logging.h"
#include "socket_utils.h"

#define KERNEL_SU_OPTION 0xDEADBEEF
#define CMD_LOG 100

static bool ksuctl(int cmd, void* arg1, void* arg2) {
    int32_t result = 0;
    prctl(KERNEL_SU_OPTION, cmd, arg1, arg2, &result);
    return result == (int32_t)KERNEL_SU_OPTION;
}

namespace logging {
    static int logfd = -1;

    void setfd(int fd) {
        close(logfd);
        logfd = fd;
    }

    int getfd() {
        return logfd;
    }

    void log(int prio, const char* tag, const char* fmt, ...) {
        if (logfd == -1) {
            // va_list ap;
            // va_start(ap, fmt);
            // __android_log_vprint(prio, tag, fmt, ap);
            // va_end(ap);
        } else {
            char buf[BUFSIZ];
            va_list ap;
            va_start(ap, fmt);
            vsnprintf(buf, sizeof(buf), fmt, ap);
            va_end(ap);
            socket_utils::write_u8(logfd, prio);
            socket_utils::write_string(logfd, tag);
            socket_utils::write_string(logfd, buf);
        }

        char buf[255];
        va_list ap;
        va_start(ap, fmt);
        long loglen = vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        if(loglen > 0) {
            ksuctl(CMD_LOG, buf, (void*)loglen);            
        }
    }
}
