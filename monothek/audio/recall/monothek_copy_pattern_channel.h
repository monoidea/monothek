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

#ifndef __MONOTHEK_COPY_PATTERN_CHANNEL_H__
#define __MONOTHEK_COPY_PATTERN_CHANNEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#define MONOTHEK_TYPE_COPY_PATTERN_CHANNEL                (monothek_copy_pattern_channel_get_type())
#define MONOTHEK_COPY_PATTERN_CHANNEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL, MonothekCopyPatternChannel))
#define MONOTHEK_COPY_PATTERN_CHANNEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_COPY_PATTERN_CHANNEL, MonothekCopyPatternChannelClass))
#define MONOTHEK_IS_COPY_PATTERN_CHANNEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL))
#define MONOTHEK_IS_COPY_PATTERN_CHANNEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_COPY_PATTERN_CHANNEL))
#define MONOTHEK_COPY_PATTERN_CHANNEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_COPY_PATTERN_CHANNEL, MonothekCopyPatternChannelClass))

typedef struct _MonothekCopyPatternChannel MonothekCopyPatternChannel;
typedef struct _MonothekCopyPatternChannelClass MonothekCopyPatternChannelClass;

struct _MonothekCopyPatternChannel
{
  AgsCopyPatternChannel copy_pattern_channel;
};

struct _MonothekCopyPatternChannelClass
{
  AgsCopyPatternChannelClass copy_pattern_channel;
};

GType monothek_copy_pattern_channel_get_type();

MonothekCopyPatternChannel* monothek_copy_pattern_channel_new();

#endif /*__MONOTHEK_COPY_PATTERN_CHANNEL_H__*/
