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
 #include <glib/gi18n-lib.h>

 static void pw_progress_dispose(GObject *gobject);
 static void pw_progress_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
 static void pw_progress_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

 enum {
	PROP_TEXT = 1,
	PROP_LENGTH_CURRENT,
	PROP_LENGTH_TOTAL,
	PROP_STEP_CURRENT,
	PROP_STEP_TOTAL,
	N_PROPERTIES
 };

 enum {
	LABEL_LEFT,
	LABEL_RIGHT,
 };

 static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

 struct _PwProgressClass {
	GtkGridClass parent_class;

 };

 struct _PwProgress {
	GtkGrid parent_instance;

	GtkProgressBar *progress_bar;
	GtkLabel *labels[2];

	guint timesource;
	uint32_t idle;
	uint64_t current;
	uint64_t total;
	uint32_t step;
	uint32_t steps;

 };

 G_DEFINE_TYPE(PwProgress, pw_progress, GTK_TYPE_GRID);
 
 static void pw_progress_class_init(PwProgressClass *klass) {
 
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = pw_progress_dispose;

	/*
  	object_class->set_property = pw_progress_set_property;
  	object_class->get_property = pw_progress_get_property;

	obj_properties[PROP_TEXT] =
    	g_param_spec_string (
			"text",
			"Text",
			"The text for progress bar, usually the URL of the download.",
			"",  // default value
			G_PARAM_READWRITE
		);

  	obj_properties[PROP_LENGTH_CURRENT] =
		g_param_spec_uint ("current-length",
						"Current length",
						"The position for the progress operation.",
						0,  			// minimum value 
						G_MAXUINT,	// maximum value
						0, 			// default value
						G_PARAM_READWRITE
		);

	obj_properties[PROP_LENGTH_TOTAL] =
		g_param_spec_uint ("total-length",
							"Total length",
							"The length of the progress operation.",
							0,  			// minimum value 
							G_MAXUINT,	// maximum value
							0, 			// default value
							G_PARAM_READWRITE
		);

	obj_properties[PROP_STEP_CURRENT] =
		g_param_spec_uint ("step",
							"Current step",
							"The current step of the operation.",
							0,  			// minimum value 
							G_MAXUINT,	// maximum value
							0, 			// default value
							G_PARAM_READWRITE
		);

	obj_properties[PROP_LENGTH_TOTAL] =
		g_param_spec_uint ("steps",
							"Steps",
							"The number of steps for this operation.",
							0,  			// minimum value 
							G_MAXUINT,	// maximum value
							0, 			// default value
							G_PARAM_READWRITE
		);

	g_object_class_install_properties(
		object_class,
		N_PROPERTIES,
		obj_properties
	);
	*/

 }

 gchar * size_to_string(uint64_t size) {

	if(!size) {
		return g_strdup("");
	} else if(size < 1024) {
		return g_strdup_printf("%" G_GUINT64_FORMAT " B", size);
	} else if(size < 1024 * 1024) {
		return g_strdup_printf("%.1f KB", (double)size / 1024.0);
	} else if(size < 1024 * 1024 * 1024) {
		return g_strdup_printf("%.1f MB", (double)size / (1024.0 * 1024.0));
	} else {
		return g_strdup_printf("%.1f GB", (double)size / (1024.0 * 1024.0 * 1024.0));
	}

 }

 static int check_for_pulse(PwProgress *self) {

	if(self->idle == 0 && self->total > 0 && self->current > 0 && self->current <= self->total) {

		// Value has changed, update the progress bar
		double fraction = (double) self->current / (double) self->total;
		gtk_progress_bar_set_fraction(self->progress_bar, fraction);

		// Calculate sizes.
		g_autofree gchar *current_str = size_to_string(self->current);
		g_autofree gchar *total_str = size_to_string(self->total);
		g_autofree gchar *text = g_strdup_printf(_("%s of %s"), current_str, total_str);
		gtk_label_set_text(self->labels[LABEL_RIGHT], text);

		self->idle++;
	
	} else if (self->idle >= 100) {

		// No value change for a while, pulse the progress bar
		gtk_progress_bar_pulse(self->progress_bar);

	} else {

		self->idle++;

	}


	return TRUE;

 }

 static void pw_progress_init(PwProgress *self) {

	// https://gnome.pages.gitlab.gnome.org/libadwaita/doc/main/style-classes.html

	for(int label = 0; label < 2; label++) {
		self->labels[label] = GTK_LABEL(gtk_label_new(NULL));
		gtk_widget_set_hexpand(GTK_WIDGET(self->labels[label]), FALSE);
		gtk_widget_set_vexpand(GTK_WIDGET(self->labels[label]), FALSE);
		gtk_widget_set_valign(GTK_WIDGET(self->labels[label]), GTK_ALIGN_END);
		gtk_widget_set_visible(GTK_WIDGET(self->labels[label]),TRUE);
		GtkStyleContext *style = gtk_widget_get_style_context(GTK_WIDGET(self->labels[label]));
		gtk_style_context_add_class(style, "dim-label");
		gtk_style_context_add_class(style, "caption");
		gtk_style_context_add_class(style, "pw-progress-label");
	}

	gtk_widget_set_halign(GTK_WIDGET(self->labels[LABEL_LEFT]), GTK_ALIGN_START);
	gtk_widget_set_halign(GTK_WIDGET(self->labels[LABEL_RIGHT]), GTK_ALIGN_END);

#ifdef DEBUG
	gtk_label_set_text(self->labels[LABEL_LEFT], "Left Label");
	gtk_label_set_text(self->labels[LABEL_RIGHT], "Right Label");
#endif

	gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->labels[LABEL_LEFT]), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->labels[LABEL_RIGHT]), 2, 1, 1, 1);

	gtk_grid_set_column_homogeneous(GTK_GRID(self), FALSE);
	gtk_grid_set_column_spacing(GTK_GRID(self), 3);
	gtk_grid_set_row_spacing(GTK_GRID(self), 3);
	gtk_grid_set_row_homogeneous(GTK_GRID(self), FALSE);

	self->progress_bar = GTK_PROGRESS_BAR(gtk_progress_bar_new());
	gtk_progress_bar_set_show_text(self->progress_bar, TRUE);
	gtk_progress_bar_set_text(self->progress_bar, _("Please wait ..."));
	gtk_widget_set_hexpand(GTK_WIDGET(self->progress_bar), TRUE);
	gtk_widget_set_vexpand(GTK_WIDGET(self->progress_bar), FALSE);
	gtk_widget_set_halign(GTK_WIDGET(self->progress_bar), GTK_ALIGN_FILL);
	gtk_widget_set_valign(GTK_WIDGET(self->progress_bar), GTK_ALIGN_START);
	gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->progress_bar), 0, 0, 3, 1);
	gtk_widget_set_visible(GTK_WIDGET(self->progress_bar),TRUE);

	{
		GtkStyleContext *style = gtk_widget_get_style_context(GTK_WIDGET(self->progress_bar));
		gtk_style_context_add_class(style, "pw-progress-bar");
	}

	self->timesource = g_timeout_add(100, (GSourceFunc) check_for_pulse, self);
 }

 static void pw_progress_dispose(GObject *gobject) {

	PwProgress *self = PW_PROGRESS(gobject);

	if(self->timesource) {
		g_source_remove(self->timesource);
		self->timesource = 0;
	}

	G_OBJECT_CLASS(pw_progress_parent_class)->dispose(gobject);

 }

 static void pw_progress_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {

	PwProgress *self = PW_PROGRESS(object);

	switch(prop_id) {
	case PROP_TEXT:
		pw_progress_set_text(self, g_value_get_string(value));
		break;

	case PROP_LENGTH_CURRENT:
		pw_progress_set_progress(self, g_value_get_uint(value), self->total);
		break;

	case PROP_LENGTH_TOTAL:
		pw_progress_set_progress(self, self->current, g_value_get_uint(value));
		break;

	case PROP_STEP_CURRENT:
		pw_progress_set_step(self, g_value_get_uint(value), self->steps);
		break;

	case PROP_STEP_TOTAL:
		pw_progress_set_step(self, self->step, g_value_get_uint(value));
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;

	}

 }

 static void pw_progress_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {

	PwProgress *self = PW_PROGRESS(object);

	switch(prop_id) {
	case PROP_TEXT:
		g_value_set_string(value, gtk_progress_bar_get_text(self->progress_bar));
		break;

	case PROP_LENGTH_CURRENT:
		g_value_set_uint(value, self->current);
		break;

	case PROP_LENGTH_TOTAL:
		g_value_set_uint(value, self->total);
		break;

	case PROP_STEP_CURRENT:
		g_value_set_uint(value, self->step);
		break;

	case PROP_STEP_TOTAL:
		g_value_set_uint(value, self->steps);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);

	}

 }

 void pw_progress_set_progress(PwProgress *progress, uint64_t current, uint64_t total) {
	progress->current = current;
	progress->total = total;
	progress->idle = 0;
	if(!current) {
		gtk_progress_bar_set_text(progress->progress_bar, _("Starting ..."));
	}
 }

 void pw_progress_set_text(PwProgress *progress, const char *text) {
	gtk_progress_bar_set_text(progress->progress_bar, text);
 }

 void pw_progress_set_pulse(PwProgress *progress, gboolean pulse) {
	progress->idle = pulse ? 1000 : 1;
 }

 void pw_progress_set_step(PwProgress *progress, unsigned int current, unsigned int total) {
	progress->step = current;
	progress->steps = total;
	progress->idle = 0;	

	if(total > 1) {
		g_autofree gchar *text = g_strdup_printf(_("%u of %u"), current, total);
		gtk_label_set_text(progress->labels[LABEL_LEFT], text);
	} else {
		gtk_label_set_text(progress->labels[LABEL_LEFT], "");
	}
	
 }

 void pw_progress_set_steps(PwProgress *progress, unsigned int steps) {
	progress->steps = steps;
	progress->step = 0;
	progress->idle = 0;
	progress->current = 0;
	progress->total = 0;

	if(steps > 1) {
		g_autofree gchar *text = g_strdup_printf(_("%u of %u"), progress->step, progress->steps);
		gtk_label_set_text(progress->labels[LABEL_LEFT], text);
	} else {
		gtk_label_set_text(progress->labels[LABEL_LEFT], "");
	}

}

 void pw_progress_next_step(PwProgress *progress) {
	progress->step++;
	progress->idle = 0;
	progress->current = 0;
	progress->total = 0;

	gtk_progress_bar_set_text(progress->progress_bar, _("Please wait ..."));

	if(progress->current <= progress->total && progress->steps > 1) {
		g_autofree gchar *text = g_strdup_printf(_("%u of %u"), progress->step, progress->steps);
		gtk_label_set_text(progress->labels[LABEL_LEFT], text);
	} else {
		gtk_label_set_text(progress->labels[LABEL_LEFT], "");
	}

 }

 PwProgress * pw_progress_new() {
	return g_object_new(PW_TYPE_PROGRESS, NULL);
 }
