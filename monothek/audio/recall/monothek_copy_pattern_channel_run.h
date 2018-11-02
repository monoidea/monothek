/* Monothek - monoidea's monothek
 * Copyright (C) 2018 Joël Krähemann
 *
 * This file is part of Monothek.
 *
 * Monothek is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Monothek is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Monothek.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONOTHEK_COPY_PATTERN_CHANNEL_RUN_H__
#define __MONOTHEK_COPY_PATTERN_CHANNEL_RUN_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN                (monothek_copy_pattern_channel_run_get_type())
#define MONOTHEK_COPY_PATTERN_CHANNEL_RUN(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN, MonothekCopyPatternChannelRun))
#define MONOTHEK_COPY_PATTERN_CHANNEL_RUN_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN, MonothekCopyPatternChannelRunClass))
#define MONOTHEK_IS_COPY_PATTERN_CHANNEL_RUN(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN))
#define MONOTHEK_IS_COPY_PATTERN_CHANNEL_RUN_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN))
#define MONOTHEK_COPY_PATTERN_CHANNEL_RUN_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_COPY_PATTERN_CHANNEL_RUN, MonothekCopyPatternChannelRunClass))

typedef struct _MonothekCopyPatternChannelRun MonothekCopyPatternChannelRun;
typedef struct _MonothekCopyPatternChannelRunClass MonothekCopyPatternChannelRunClass;

struct _MonothekCopyPatternChannelRun
{
  AgsCopyPatternChannelRun copy_pattern_channel_run;
};

struct _MonothekCopyPatternChannelRunClass
{
  AgsCopyPatternChannelRunClass copy_pattern_channel_run;
};

GType monothek_copy_pattern_channel_run_get_type();

MonothekCopyPatternChannelRun* monothek_copy_pattern_channel_run_new();

#endif /*__MONOTHEK_COPY_PATTERN_CHANNEL_RUN_H__*/
