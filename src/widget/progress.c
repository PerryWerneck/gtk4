/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2025 Perry Werneck <perry.werneck@gmail.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 // References: https://docs.gtk.org/gobject/tutorial.html

 #include <config.h>
 #include <gtk4/progress.h>
 #include <gtk/gtk.h>

 enum {
	PROP_TEXT = 1,
	PROP_CURRENT,
	PROP_TOTAL,
	PROP_PULSE,
	PROP_STEP_CURRENT,
	PROP_STEP_TOTAL,
	N_PROPERTIES
 };

 static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

 struct _PwProgressClass {
	GtkGridClass parent_class;

 };

 struct _PwProgress {
	GtkGrid parent_instance;

	GtkProgressBar *progress_bar;
	guint timesource;
	uint8_t flags;
	uint32_t idle;
	uint64_t current;
	uint64_t total;

 };

 G_DEFINE_TYPE(PwProgress, pw_progress, GTK_TYPE_GRID);
 
 static void pw_progress_class_init(PwProgressClass *klass) {
 
 }

 static void pw_progres_init(PwProgress *self) {

 }

