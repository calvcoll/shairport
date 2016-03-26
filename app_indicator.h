#ifdef __unix__

#ifndef _APP_INDICATOR_H
#define _APP_INDICATOR_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libappindicator/app-indicator.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "common.h"
#include "pthread.h"
#include "metadata.h"
#include <string.h>

struct stat info;

void startup_gtk(void);
void shutdown_gtk(void);
void indicator_set_image_data(const char *buffer, int length, metadata *data);

#endif
#endif
