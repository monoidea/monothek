/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2019 Joël Krähemann
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

#ifndef __MONOTHEK_EXPORT_OUTPUT_H__
#define __MONOTHEK_EXPORT_OUTPUT_H__

#include <glib.h>
#include <glib-object.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#define MONOTHEK_TYPE_EXPORT_OUTPUT                (monothek_export_output_get_type())
#define MONOTHEK_EXPORT_OUTPUT(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_EXPORT_OUTPUT, MonothekExportOutput))
#define MONOTHEK_EXPORT_OUTPUT_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_EXPORT_OUTPUT, MonothekExportOutputClass))
#define MONOTHEK_IS_EXPORT_OUTPUT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_EXPORT_OUTPUT))
#define MONOTHEK_IS_EXPORT_OUTPUT_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_EXPORT_OUTPUT))
#define MONOTHEK_EXPORT_OUTPUT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_EXPORT_OUTPUT, MonothekExportOutputClass))

#if MONOTHEK_DEVEL_MODE
#define MONOTHEK_EXPORT_PATH SRCDIR "/monothek.home/monothek/export"
#else
#define MONOTHEK_EXPORT_PATH "/home/monothek/export"
#endif

typedef struct _MonothekExportOutput MonothekExportOutput;
typedef struct _MonothekExportOutputClass MonothekExportOutputClass;

struct _MonothekExportOutput
{
  AgsExportOutput export_output;

  struct timespec *duration;
};

struct _MonothekExportOutputClass
{
  AgsExportOutputClass export_output;
};

GType monothek_export_output_get_type();

MonothekExportOutput* monothek_export_output_new(AgsExportThread *export_thread,
						 GObject *soundcard,
						 struct timespec *duration);

#endif /*__MONOTHEK_EXPORT_OUTPUT_H__*/
