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

#ifndef __MONOTHEK_JUKEBOX_TRACK_VIEW_H__
#define __MONOTHEK_JUKEBOX_TRACK_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW                (monothek_jukebox_track_view_get_type())
#define MONOTHEK_JUKEBOX_TRACK_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, MonothekJukeboxTrackView))
#define MONOTHEK_JUKEBOX_TRACK_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, MonothekJukeboxTrackViewClass))
#define MONOTHEK_IS_JUKEBOX_TRACK_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW))
#define MONOTHEK_IS_JUKEBOX_TRACK_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW))
#define MONOTHEK_JUKEBOX_TRACK_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, MonothekJukeboxTrackViewClass))

typedef struct _MonothekJukeboxTrackView MonothekJukeboxTrackView;
typedef struct _MonothekJukeboxTrackViewClass MonothekJukeboxTrackViewClass;

struct _MonothekJukeboxTrackView
{
  MonothekView monothek_view;

  //cover
  cairo_surface_t *cover_surface;
  
  gdouble cover_box_line_width;

  gdouble cover_box_x0;
  gdouble cover_box_y0;

  gdouble cover_box_width;
  gdouble cover_box_height;

  //progress
  GtkAdjustment *progress;
  
  gdouble progress_box_line_width;

  gdouble progress_box_x0;
  gdouble progress_box_y0;

  gdouble progress_box_width;
  gdouble progress_box_height;
};

struct _MonothekJukeboxTrackViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_track_view_get_type(void);

MonothekJukeboxTrackView* monothek_jukebox_track_view_new();

#endif /*__MONOTHEK_JUKEBOX_TRACK_VIEW_H__*/
