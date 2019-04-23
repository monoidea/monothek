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

#ifndef __MONOTHEK_JUKEBOX_MODE_VIEW_H__
#define __MONOTHEK_JUKEBOX_MODE_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_JUKEBOX_MODE_VIEW                (monothek_jukebox_mode_view_get_type())
#define MONOTHEK_JUKEBOX_MODE_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_MODE_VIEW, MonothekJukeboxModeView))
#define MONOTHEK_JUKEBOX_MODE_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_MODE_VIEW, MonothekJukeboxModeViewClass))
#define MONOTHEK_IS_JUKEBOX_MODE_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_MODE_VIEW))
#define MONOTHEK_IS_JUKEBOX_MODE_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_MODE_VIEW))
#define MONOTHEK_JUKEBOX_MODE_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_MODE_VIEW, MonothekJukeboxModeViewClass))

#define MONOTHEK_JUKEBOX_MODE_VIEW_PADDING_TOP (12.9)
#define MONOTHEK_JUKEBOX_MODE_VIEW_PADDING_LEFT (20.0)

typedef struct _MonothekJukeboxModeView MonothekJukeboxModeView;
typedef struct _MonothekJukeboxModeViewClass MonothekJukeboxModeViewClass;

typedef enum{
  MONOTHEK_JUKEBOX_MODE_VIEW_ALL_TESTS                = 1,
  MONOTHEK_JUKEBOX_MODE_VIEW_MORE_TESTS               = 1 <<  1,
  MONOTHEK_JUKEBOX_MODE_VIEW_ONE_TEST                 = 1 <<  2,
  MONOTHEK_JUKEBOX_MODE_VIEW_NO_TEST                  = 1 <<  3,
}MonothekJukeboxModeViewFlags;

struct _MonothekJukeboxModeView
{
  MonothekView monothek_view;

  guint flags;
  
  //test  
  gdouble test_box_line_width;

  gdouble test_box_x0;
  gdouble test_box_y0;

  gdouble test_box_width;
  gdouble test_box_height;

  //play
  gdouble play_box_line_width;

  gdouble play_box_x0;
  gdouble play_box_y0;

  gdouble play_box_width;
  gdouble play_box_height;

  //cancel
  gdouble cancel_box_line_width;

  gdouble cancel_box_x0;
  gdouble cancel_box_y0;

  gdouble cancel_box_width;
  gdouble cancel_box_height;

  gchar *current_test_message;
};

struct _MonothekJukeboxModeViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_jukebox_mode_view_get_type(void);

MonothekJukeboxModeView* monothek_jukebox_mode_view_new();

#endif /*__MONOTHEK_JUKEBOX_MODE_VIEW_H__*/
