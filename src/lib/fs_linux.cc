/********************************************************************
 * livegrep -- fs_linux.cc
 * Copyright (c) 2011-2013 Nelson Elhage
 *
 * This program is free software. You may use, redistribute, and/or
 * modify it under the terms listed in the COPYING file.
 ********************************************************************/
#include "fs.h"

#include <limits.h>
#include <sys/inotify.h>
#include <unistd.h>

namespace {
    int fd = -1;
    int wd = -1;
}

fswatcher::fswatcher(const std::string &path) : path_(path) {
    fd = inotify_init();
    wd = inotify_add_watch(fd, path.c_str(), IN_ATTRIB | IN_CLOSE_WRITE | IN_MOVE_SELF);
}

fswatcher::~fswatcher() {
    if (fd >= 0) {
        close(fd);
        if (wd >= 0) {
            inotify_rm_watch(fd, wd);
        }
    }
}

void fswatcher::wait_for_event() {
    struct inotify_event event;
    int n = 0;

    // The read syscall is blocking; it returns after one eligible event (i.e., matching the mask) is received.
    while (n <= 0) {
        n = read(fd, &event, sizeof(struct inotify_event) + NAME_MAX + 1);
    }
}
