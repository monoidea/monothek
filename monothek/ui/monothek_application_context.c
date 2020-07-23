/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2020 Joël Krähemann
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

#include <monothek/ui/monothek_application_context.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/audio/monothek_rack.h>

#include <monothek/audio/thread/monothek_export_thread.h>

#include <monothek/audio/file/monothek_audio_file_manager.h>

#include <monothek/ui/model/monothek_jukebox_playlist_model.h>

#include <pango/pango.h>

#include <libxml/parser.h>
#include <libxml/xlink.h>
#include <libxml/xpath.h>
#include <libxml/valid.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlsave.h>

#ifndef __APPLE__
#include <pango/pangofc-fontmap.h>
#endif

#include <monothek/ui/view/monothek_load_view.h>
#include <monothek/ui/view/monothek_start_view.h>

#include <sys/types.h>

#ifndef MONOTHEK_W32API
#include <pwd.h>
#endif

#include <stdbool.h>

#include <monothek/i18n.h>

void monothek_application_context_class_init(MonothekApplicationContextClass *monothek_application_context);
void monothek_application_context_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_application_context_concurrency_provider_interface_init(AgsConcurrencyProviderInterface *concurrency_provider);
void monothek_application_context_service_provider_interface_init(AgsServiceProviderInterface *service_provider);
void monothek_application_context_sound_provider_interface_init(AgsSoundProviderInterface *sound_provider);
void monothek_application_context_init(MonothekApplicationContext *monothek_application_context);
void monothek_application_context_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec);
void monothek_application_context_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec);
void monothek_application_context_dispose(GObject *gobject);
void monothek_application_context_finalize(GObject *gobject);

void monothek_application_context_connect(AgsConnectable *connectable);
void monothek_application_context_disconnect(AgsConnectable *connectable);

AgsThread* monothek_application_context_get_main_loop(AgsConcurrencyProvider *concurrency_provider);
void monothek_application_context_set_main_loop(AgsConcurrencyProvider *concurrency_provider,
						AgsThread *main_loop);
AgsTaskLauncher* monothek_application_context_get_task_launcher(AgsConcurrencyProvider *concurrency_provider);
void monothek_application_context_set_task_launcher(AgsConcurrencyProvider *concurrency_provider,
						    AgsTaskLauncher *task_launcher);
AgsThreadPool* monothek_application_context_get_thread_pool(AgsConcurrencyProvider *concurrency_provider);
void monothek_application_context_set_thread_pool(AgsConcurrencyProvider *concurrency_provider,
						  AgsThreadPool *thread_pool);
GList* monothek_application_context_get_worker(AgsConcurrencyProvider *concurrency_provider);
void monothek_application_context_set_worker(AgsConcurrencyProvider *concurrency_provider,
					     GList *worker);

gboolean monothek_application_context_is_operating(AgsServiceProvider *service_provider);

AgsServerStatus* monothek_application_context_server_status(AgsServiceProvider *service_provider);

void monothek_application_context_set_registry(AgsServiceProvider *service_provider,
					       AgsRegistry *registry);
AgsRegistry* monothek_application_context_get_registry(AgsServiceProvider *service_provider);

void monothek_application_context_set_server(AgsServiceProvider *service_provider,
					     GList *server);
GList* monothek_application_context_get_server(AgsServiceProvider *service_provider);

GObject* monothek_application_context_get_default_soundcard(AgsSoundProvider *sound_provider);
void monothek_application_context_set_default_soundcard(AgsSoundProvider *sound_provider,
							GObject *soundcard);
GObject* monothek_application_context_get_default_soundcard_thread(AgsSoundProvider *sound_provider);
void monothek_application_context_set_default_soundcard_thread(AgsSoundProvider *sound_provider,
							       GObject *soundcard_thread);
GList* monothek_application_context_get_soundcard(AgsSoundProvider *sound_provider);
void monothek_application_context_set_soundcard(AgsSoundProvider *sound_provider,
						GList *soundcard);
GList* monothek_application_context_get_sequencer(AgsSoundProvider *sound_provider);
void monothek_application_context_set_sequencer(AgsSoundProvider *sound_provider,
						GList *sequencer);
GList* monothek_application_context_get_audio(AgsSoundProvider *sound_provider);
void monothek_application_context_set_audio(AgsSoundProvider *sound_provider,
					    GList *audio);
GList* monothek_application_context_get_sound_server(AgsSoundProvider *sound_provider);
void monothek_application_context_set_sound_server(AgsSoundProvider *sound_provider,
						   GList *sound_server);
GList* monothek_application_context_get_osc_server(AgsSoundProvider *sound_provider);
void monothek_application_context_set_osc_server(AgsSoundProvider *sound_provider,
						 GList *osc_server);

void monothek_application_context_load_config(AgsApplicationContext *application_context);

void monothek_application_context_prepare(AgsApplicationContext *application_context);
void monothek_application_context_setup(AgsApplicationContext *application_context);

void monothek_application_context_register_types(AgsApplicationContext *application_context);

void monothek_application_context_quit(AgsApplicationContext *application_context);

void monothek_application_context_launch(AgsFileLaunch *launch, MonothekApplicationContext *application_context);

void* monothek_application_context_server_main_loop_thread(GMainLoop *main_loop);
void* monothek_application_context_audio_main_loop_thread(GMainLoop *main_loop);

/**
 * SECTION:monothek_application_context
 * @short_description: The monothek application context
 * @title: MonothekApplicationContext
 * @section_id:
 * @include: ags/X/monothek_application_context.h
 *
 * #MonothekApplicationContext is a application context providing
 * the main window and sets up a functional audio layer.
 */

enum{
     PROP_0,
     PROP_WINDOW,
};

static gpointer monothek_application_context_parent_class = NULL;
static AgsConnectableInterface* monothek_application_context_parent_connectable_interface;

MonothekApplicationContext *monothek_application_context;

extern AgsApplicationContext *ags_application_context;

//TODO:JK: implement get functions
#ifndef AGS_USE_TIMER
static sigset_t ags_wait_mask;
struct sigaction ags_sigact;
#else
static sigset_t ags_timer_mask;

struct sigaction ags_sigact_timer;

struct sigevent ags_sev_timer;
struct itimerspec its;
#endif

GType
monothek_application_context_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_application_context = 0;

    static const GTypeInfo monothek_application_context_info = {
								sizeof (MonothekApplicationContextClass),
								NULL, /* base_init */
								NULL, /* base_finalize */
								(GClassInitFunc) monothek_application_context_class_init,
								NULL, /* class_finalize */
								NULL, /* class_data */
								sizeof (MonothekApplicationContext),
								0,    /* n_preallocs */
								(GInstanceInitFunc) monothek_application_context_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
								       (GInterfaceInitFunc) monothek_application_context_connectable_interface_init,
								       NULL, /* interface_finalize */
								       NULL, /* interface_data */
    };

    static const GInterfaceInfo monothek_concurrency_provider_interface_info = {
										(GInterfaceInitFunc) monothek_application_context_concurrency_provider_interface_init,
										NULL, /* interface_finalize */
										NULL, /* interface_data */
    };

    static const GInterfaceInfo monothek_service_provider_interface_info = {
									    (GInterfaceInitFunc) monothek_application_context_service_provider_interface_init,
									    NULL, /* interface_finalize */
									    NULL, /* interface_data */
    };
    
    static const GInterfaceInfo monothek_sound_provider_interface_info = {
									  (GInterfaceInitFunc) monothek_application_context_sound_provider_interface_init,
									  NULL, /* interface_finalize */
									  NULL, /* interface_data */
    };

    monothek_type_application_context = g_type_register_static(MONOTHEK_TYPE_APPLICATION_CONTEXT,
							       "MonothekApplicationContext",
							       &monothek_application_context_info,
							       0);

    g_type_add_interface_static(monothek_type_application_context,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_type_add_interface_static(monothek_type_application_context,
				AGS_TYPE_CONCURRENCY_PROVIDER,
				&monothek_concurrency_provider_interface_info);

    g_type_add_interface_static(monothek_type_application_context,
				AGS_TYPE_SERVICE_PROVIDER,
				&monothek_service_provider_interface_info);

    g_type_add_interface_static(monothek_type_application_context,
				AGS_TYPE_SOUND_PROVIDER,
				&monothek_sound_provider_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_application_context);
  }

  return g_define_type_id__volatile;
}

void
monothek_application_context_class_init(MonothekApplicationContextClass *monothek_application_context)
{
  GObjectClass *gobject;
  AgsApplicationContextClass *application_context;
  GParamSpec *param_spec;

  monothek_application_context_parent_class = g_type_class_peek_parent(monothek_application_context);

  /* GObjectClass */
  gobject = (GObjectClass *) monothek_application_context;

  gobject->set_property = monothek_application_context_set_property;
  gobject->get_property = monothek_application_context_get_property;

  gobject->dispose = monothek_application_context_dispose;
  gobject->finalize = monothek_application_context_finalize;

  /* properties */
  /**
   * MonothekApplicationContext:window:
   *
   * The assigned window.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_object("window",
				   i18n_pspec("window of monothek application context"),
				   i18n_pspec("The window which this monothek application context assigned to"),
				   MONOTHEK_TYPE_WINDOW,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_WINDOW,
				  param_spec);

  /* MonothekApplicationContextClass */
  application_context = (AgsApplicationContextClass *) monothek_application_context;
  
  application_context->load_config = monothek_application_context_load_config;

  application_context->prepare = monothek_application_context_prepare;
  application_context->setup = monothek_application_context_setup;

  application_context->register_types = monothek_application_context_register_types;
  
  application_context->quit = monothek_application_context_quit;

  application_context->write = NULL;
  application_context->read = NULL;
}

void
monothek_application_context_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_application_context_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_application_context_connect;
  connectable->disconnect = monothek_application_context_disconnect;
}

void
monothek_application_context_concurrency_provider_interface_init(AgsConcurrencyProviderInterface *concurrency_provider)
{
  concurrency_provider->get_main_loop = monothek_application_context_get_main_loop;
  concurrency_provider->set_main_loop = monothek_application_context_set_main_loop;

  concurrency_provider->get_task_launcher = monothek_application_context_get_task_launcher;
  concurrency_provider->set_task_launcher = monothek_application_context_set_task_launcher;

  concurrency_provider->get_thread_pool = monothek_application_context_get_thread_pool;
  concurrency_provider->set_thread_pool = monothek_application_context_set_thread_pool;

  concurrency_provider->get_worker = monothek_application_context_get_worker;
  concurrency_provider->set_worker = monothek_application_context_set_worker;
}

void
monothek_application_context_service_provider_interface_init(AgsServiceProviderInterface *service_provider)
{
  service_provider->is_operating = monothek_application_context_is_operating;

  service_provider->server_status = monothek_application_context_server_status;

  service_provider->set_registry = monothek_application_context_set_registry;
  service_provider->get_registry = monothek_application_context_get_registry;
  
  service_provider->set_server = monothek_application_context_set_server;
  service_provider->get_server = monothek_application_context_get_server;
}

void
monothek_application_context_sound_provider_interface_init(AgsSoundProviderInterface *sound_provider)
{
  sound_provider->get_default_soundcard = monothek_application_context_get_default_soundcard;
  sound_provider->set_default_soundcard = monothek_application_context_set_default_soundcard;

  sound_provider->get_default_soundcard_thread = monothek_application_context_get_default_soundcard_thread;
  sound_provider->set_default_soundcard_thread = monothek_application_context_set_default_soundcard_thread;

  sound_provider->get_soundcard = monothek_application_context_get_soundcard;
  sound_provider->set_soundcard = monothek_application_context_set_soundcard;

  sound_provider->get_sequencer = monothek_application_context_get_sequencer;
  sound_provider->set_sequencer = monothek_application_context_set_sequencer;

  sound_provider->get_sound_server = monothek_application_context_get_sound_server;
  sound_provider->set_sound_server = monothek_application_context_set_sound_server;

  sound_provider->get_audio = monothek_application_context_get_audio;
  sound_provider->set_audio = monothek_application_context_set_audio;

  sound_provider->get_osc_server = monothek_application_context_get_osc_server;
  sound_provider->set_osc_server = monothek_application_context_set_osc_server;
}

void
monothek_application_context_init(MonothekApplicationContext *monothek_application_context)
{
  AgsApplicationContext *application_context;
  AgsConfig *config;

#ifndef MONOTHEK_W32API
  struct passwd *pw;
  
  uid_t uid;
#endif

  gchar *wdir;
  gchar *config_file;
#if defined MONOTHEK_W32API
  gchar *app_dir;
  gchar *path;
#endif

  application_context = monothek_application_context;
  
  if(ags_application_context == NULL){
    ags_application_context = monothek_application_context;
  }

  g_atomic_int_set(&(monothek_application_context->gui_ready),
		   FALSE);
  
  /* fundamental instances */  
  config = ags_config_get_instance();
  AGS_APPLICATION_CONTEXT(monothek_application_context)->config = config;
  g_object_ref(config);
  g_object_set(config,
	       "application-context", monothek_application_context,
	       NULL);
  
  AGS_APPLICATION_CONTEXT(monothek_application_context)->log = ags_log_get_instance();
  g_object_ref(AGS_APPLICATION_CONTEXT(monothek_application_context)->log);
  
  /* Monothek application context */
  monothek_application_context->thread_pool = NULL;

  monothek_application_context->worker = NULL;
  
  monothek_application_context->is_operating = FALSE;

  monothek_application_context->server_status = NULL;

  monothek_application_context->registry = NULL;

  monothek_application_context->server = NULL;

  monothek_application_context->default_soundcard = NULL;

  monothek_application_context->default_soundcard_thread = NULL;
  monothek_application_context->default_export_thread = NULL;
    
  monothek_application_context->soundcard = NULL;
  monothek_application_context->sequencer = NULL;

  monothek_application_context->sound_server = NULL;
  
  monothek_application_context->audio = NULL;

  monothek_application_context->osc_server = NULL;

  monothek_application_context->gui_ready = FALSE;
  monothek_application_context->show_animation = TRUE;

  monothek_application_context->window = NULL;
}

void
monothek_application_context_set_property(GObject *gobject,
					  guint prop_id,
					  const GValue *value,
					  GParamSpec *param_spec)
{
  MonothekApplicationContext *monothek_application_context;

  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(gobject);

  switch(prop_id){
  case PROP_WINDOW:
    {
      MonothekWindow *window;
      
      window = (MonothekWindow *) g_value_get_object(value);

      if(window == monothek_application_context->window){
	return;
      }

      if(monothek_application_context->window != NULL){
	g_object_unref(monothek_application_context->window);
      }
      
      if(window != NULL){
	g_object_ref(G_OBJECT(window));
      }
      
      monothek_application_context->window = window;
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_application_context_get_property(GObject *gobject,
					  guint prop_id,
					  GValue *value,
					  GParamSpec *param_spec)
{
  MonothekApplicationContext *monothek_application_context;

  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(gobject);

  switch(prop_id){
  case PROP_WINDOW:
    {
      g_value_set_object(value, monothek_application_context->window);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_application_context_dispose(GObject *gobject)
{
  MonothekApplicationContext *monothek_application_context;

  GList *list;

  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(gobject);

  /* thread pool */
  if(monothek_application_context->thread_pool != NULL){
    g_object_unref(monothek_application_context->thread_pool);
    
    monothek_application_context->thread_pool = NULL;
  }

  /* worker thread */
  if(monothek_application_context->worker != NULL){    
    g_list_free_full(monothek_application_context->worker,
		     g_object_unref);

    monothek_application_context->worker = NULL;
  }

  /* server status */
  if(monothek_application_context->server_status != NULL){
    g_object_unref(monothek_application_context->server_status);
    
    monothek_application_context->server_status = NULL;
  }

  /* server thread */
  if(monothek_application_context->server != NULL){    
    g_list_free_full(monothek_application_context->server,
		     g_object_unref);

    monothek_application_context->server = NULL;
  }

  /* default soundcard */
  if(monothek_application_context->default_soundcard != NULL){
    g_object_unref(monothek_application_context->default_soundcard);

    monothek_application_context->default_soundcard = NULL;
  }

  /* soundcard and export thread */
  if(monothek_application_context->default_soundcard_thread != NULL){
    g_object_unref(monothek_application_context->default_soundcard_thread);

    monothek_application_context->default_soundcard_thread = NULL;
  }

  if(monothek_application_context->default_export_thread != NULL){
    g_object_unref(monothek_application_context->default_export_thread);

    monothek_application_context->default_export_thread = NULL;
  }

  /* soundcard and sequencer */
  if(monothek_application_context->soundcard != NULL){    
    g_list_free_full(monothek_application_context->soundcard,
		     g_object_unref);

    monothek_application_context->soundcard = NULL;
  }

  if(monothek_application_context->sequencer != NULL){
    g_list_free_full(monothek_application_context->sequencer,
		     g_object_unref);

    monothek_application_context->sequencer = NULL;
  }

  /* audio */
  if(monothek_application_context->audio != NULL){
    g_list_free_full(monothek_application_context->audio,
		     g_object_unref);

    monothek_application_context->audio = NULL;
  }

  /* sound server */
  if(monothek_application_context->sound_server != NULL){
    g_list_free_full(monothek_application_context->sound_server,
		     g_object_unref);

    monothek_application_context->sound_server = NULL;
  }

  /* osc server */
  if(monothek_application_context->osc_server != NULL){
    g_list_free_full(monothek_application_context->osc_server,
		     g_object_unref);

    monothek_application_context->osc_server = NULL;
  }
  
  /* window */
  if(monothek_application_context->window != NULL){
    gtk_widget_destroy(monothek_application_context->window);

    monothek_application_context->window = NULL;
  }  
  
  /* call parent */
  G_OBJECT_CLASS(monothek_application_context_parent_class)->dispose(gobject);
}

void
monothek_application_context_finalize(GObject *gobject)
{
  MonothekApplicationContext *monothek_application_context;

  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(gobject);

  /* thread pool */
  if(monothek_application_context->thread_pool != NULL){
    g_object_unref(monothek_application_context->thread_pool);
  }

  /* worker thread */
  if(monothek_application_context->worker != NULL){    
    g_list_free_full(monothek_application_context->worker,
		     g_object_unref);
  }

  /* server status */
  if(monothek_application_context->server_status != NULL){
    g_object_unref(monothek_application_context->server_status);
  }

  /* server thread */
  if(monothek_application_context->server != NULL){    
    g_list_free_full(monothek_application_context->server,
		     g_object_unref);
  }

  /* default soundcard */
  if(monothek_application_context->default_soundcard != NULL){
    g_object_unref(monothek_application_context->default_soundcard);
  }

  /* soundcard and export thread */
  if(monothek_application_context->default_soundcard_thread != NULL){
    g_object_unref(monothek_application_context->default_soundcard_thread);
  }

  if(monothek_application_context->default_export_thread != NULL){
    g_object_unref(monothek_application_context->default_export_thread);
  }

  /* soundcard and sequencer */
  if(monothek_application_context->soundcard != NULL){    
    g_list_free_full(monothek_application_context->soundcard,
		     g_object_unref);
  }

  if(monothek_application_context->sequencer != NULL){
    g_list_free_full(monothek_application_context->sequencer,
		     g_object_unref);
  }

  /* audio */
  if(monothek_application_context->audio != NULL){
    g_list_free_full(monothek_application_context->audio,
		     g_object_unref);
  }

  /* sound server */
  if(monothek_application_context->sound_server != NULL){
    g_list_free_full(monothek_application_context->sound_server,
		     g_object_unref);
  }

  /* osc server */
  if(monothek_application_context->osc_server != NULL){
    g_list_free_full(monothek_application_context->osc_server,
		     g_object_unref);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_application_context_parent_class)->finalize(gobject);
}

void
monothek_application_context_connect(AgsConnectable *connectable)
{
  MonothekApplicationContext *monothek_application_context;

  GList *soundcard, *sequencer;
  
  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(connectable);

  if((AGS_APPLICATION_CONTEXT_CONNECTED & (AGS_APPLICATION_CONTEXT(monothek_application_context)->flags)) != 0){
    return;
  }

  monothek_application_context_parent_connectable_interface->connect(connectable);

  /* soundcard */
  soundcard = monothek_application_context->soundcard;

  while(soundcard != NULL){
    ags_connectable_connect(AGS_CONNECTABLE(soundcard->data));

    soundcard = soundcard->next;
  }
  
  ags_connectable_connect(AGS_CONNECTABLE(monothek_application_context->window));
}

void
monothek_application_context_disconnect(AgsConnectable *connectable)
{
  MonothekApplicationContext *monothek_application_context;

  monothek_application_context = MONOTHEK_APPLICATION_CONTEXT(connectable);

  if((AGS_APPLICATION_CONTEXT_CONNECTED & (AGS_APPLICATION_CONTEXT(monothek_application_context)->flags)) == 0){
    return;
  }

  monothek_application_context_parent_connectable_interface->disconnect(connectable);
}

AgsThread*
monothek_application_context_get_main_loop(AgsConcurrencyProvider *concurrency_provider)
{
  AgsThread *main_loop;

  AgsApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  main_loop = (AgsThread *) application_context->main_loop;

  if(main_loop != NULL){
    g_object_ref(main_loop);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(main_loop);
}

void
monothek_application_context_set_main_loop(AgsConcurrencyProvider *concurrency_provider,
					   AgsThread *main_loop)
{
  AgsApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->main_loop == main_loop){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  if(application_context->main_loop != NULL){
    g_object_unref(application_context->main_loop);
  }
  
  if(main_loop != NULL){
    g_object_ref(main_loop);
  }
  
  application_context->main_loop = (GObject *) main_loop;
  
  g_rec_mutex_unlock(application_context_mutex);
}

AgsTaskLauncher*
monothek_application_context_get_task_launcher(AgsConcurrencyProvider *concurrency_provider)
{
  AgsTaskLauncher *task_launcher;

  AgsApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  task_launcher = (AgsThread *) application_context->task_launcher;

  if(task_launcher != NULL){
    g_object_ref(task_launcher);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(task_launcher);
}

void
monothek_application_context_set_task_launcher(AgsConcurrencyProvider *concurrency_provider,
					       AgsTaskLauncher *task_launcher)
{
  AgsApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->task_launcher == task_launcher){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  if(application_context->task_launcher != NULL){
    g_object_unref(application_context->task_launcher);
  }
  
  if(task_launcher != NULL){
    g_object_ref(task_launcher);
  }
  
  application_context->task_launcher = (GObject *) task_launcher;
  
  g_rec_mutex_unlock(application_context_mutex);
}

AgsThreadPool*
monothek_application_context_get_thread_pool(AgsConcurrencyProvider *concurrency_provider)
{
  AgsThreadPool *thread_pool;
  
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get thread pool */
  g_rec_mutex_lock(application_context_mutex);

  thread_pool = application_context->thread_pool;

  if(thread_pool != NULL){
    g_object_ref(thread_pool);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(thread_pool);
}

void
monothek_application_context_set_thread_pool(AgsConcurrencyProvider *concurrency_provider,
					     AgsThreadPool *thread_pool)
{
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->thread_pool == thread_pool){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  if(application_context->thread_pool != NULL){
    g_object_unref(application_context->thread_pool);
  }
  
  if(thread_pool != NULL){
    g_object_ref(thread_pool);
  }
  
  application_context->thread_pool = (GObject *) thread_pool;
  
  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_worker(AgsConcurrencyProvider *concurrency_provider)
{
  MonothekApplicationContext *application_context;
  
  GList *worker;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get worker */
  g_rec_mutex_lock(application_context_mutex);

  worker = g_list_copy_deep(application_context->worker,
			    (GCopyFunc) g_object_ref,
			    NULL);
  
  g_rec_mutex_unlock(application_context_mutex);

  return(worker);
}

void
monothek_application_context_set_worker(AgsConcurrencyProvider *concurrency_provider,
					GList *worker)
{
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set worker */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->worker == worker){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  g_list_free_full(application_context->worker,
		   g_object_unref);
  
  application_context->worker = worker;

  g_rec_mutex_unlock(application_context_mutex);
}

gboolean
monothek_application_context_is_operating(AgsServiceProvider *service_provider)
{
  MonothekApplicationContext *application_context;

  gboolean is_operating;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get gui ready */
  g_rec_mutex_lock(application_context_mutex);

  is_operating = application_context->is_operating;
   
  g_rec_mutex_unlock(application_context_mutex);

  return(is_operating);
}

AgsServerStatus*
monothek_application_context_server_status(AgsServiceProvider *service_provider)
{
  AgsServerStatus *server_status;
  
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get thread pool */
  g_rec_mutex_lock(application_context_mutex);

  server_status = application_context->server_status;

  if(server_status != NULL){
    g_object_ref(server_status);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(server_status);
}

AgsRegistry*
monothek_application_context_get_registry(AgsServiceProvider *service_provider)
{
  AgsRegistry *registry;
  
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get thread pool */
  g_rec_mutex_lock(application_context_mutex);

  registry = application_context->registry;

  if(registry != NULL){
    g_object_ref(registry);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(registry);
}

void
monothek_application_context_set_registry(AgsServiceProvider *service_provider,
					  AgsRegistry *registry)
{
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get main loop */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->registry == registry){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  if(application_context->registry != NULL){
    g_object_unref(application_context->registry);
  }
  
  if(registry != NULL){
    g_object_ref(registry);
  }
  
  application_context->registry = (GObject *) registry;
  
  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_server(AgsServiceProvider *service_provider)
{
  MonothekApplicationContext *application_context;
  
  GList *server;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get server */
  g_rec_mutex_lock(application_context_mutex);

  server = g_list_copy_deep(application_context->server,
			    (GCopyFunc) g_object_ref,
			    NULL);
  
  g_rec_mutex_unlock(application_context_mutex);

  return(server);
}

void
monothek_application_context_set_server(AgsServiceProvider *service_provider,
					GList *server)
{
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(service_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set server */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->server == server){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  g_list_free_full(application_context->server,
		   g_object_unref);
  
  application_context->server = server;

  g_rec_mutex_unlock(application_context_mutex);
}

GObject*
monothek_application_context_get_default_soundcard_thread(AgsSoundProvider *sound_provider)
{
  GObject *soundcard_thread;
  
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get default soundcard thread */
  g_rec_mutex_lock(application_context_mutex);

  soundcard_thread = (GObject *) MONOTHEK_APPLICATION_CONTEXT(sound_provider)->default_soundcard_thread;

  if(soundcard_thread != NULL){
    g_object_ref(soundcard_thread);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(soundcard_thread);
}

void
monothek_application_context_set_default_soundcard(AgsSoundProvider *sound_provider,
						   GObject *soundcard)
{
  AgsMessageDelivery *message_delivery;
  GList *start_message_queue;

  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set default soundcard */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard == soundcard){
    g_rec_mutex_unlock(application_context_mutex);

    return;
  }

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard != NULL){
    g_object_unref(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard);
  }
  
  if(soundcard != NULL){
    g_object_ref(soundcard);
  }
  
  MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard = (GObject *) soundcard;

  g_rec_mutex_unlock(application_context_mutex);

  /* emit message */
  message_delivery = ags_message_delivery_get_instance();

  start_message_queue = ags_message_delivery_find_sender_namespace(message_delivery,
								   "libags-audio");

  if(start_message_queue != NULL){
    AgsMessageEnvelope *message;

    xmlDoc *doc;
    xmlNode *root_node;

    /* specify message body */
    doc = xmlNewDoc("1.0");

    root_node = xmlNewNode(NULL,
			   "ags-command");
    xmlDocSetRootElement(doc, root_node);    

    xmlNewProp(root_node,
	       "method",
	       "AgsSoundProvider::set-default-soundcard");

    /* add message */
    message = ags_message_envelope_new(G_OBJECT(sound_provider),
				       NULL,
				       doc);
    
    /* set parameter */
    message->n_params = 1;

    message->parameter_name = (gchar **) malloc(2 * sizeof(gchar *));
    message->value = g_new0(GValue,
			    1);

    /* audio channels */
    message->parameter_name[0] = "default-soundcard";
    
    g_value_init(&(message->value[0]),
		 G_TYPE_OBJECT);
    g_value_set_object(&(message->value[0]),
		       soundcard);

    /* terminate string vector */
    message->parameter_name[1] = NULL;
    
    /* add message */
    ags_message_delivery_add_message_envelope(message_delivery,
					      "libags-audio",
					      message);

    g_list_free_full(start_message_queue,
		     (GDestroyNotify) g_object_unref);
  }
}

GObject*
monothek_application_context_get_default_soundcard(AgsSoundProvider *sound_provider)
{
  GObject *soundcard;
  
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get default soundcard */
  g_rec_mutex_lock(application_context_mutex);

  soundcard = (GObject *) MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard;

  if(soundcard != NULL){
    g_object_ref(soundcard);
  }
  
  g_rec_mutex_unlock(application_context_mutex);
  
  return(soundcard);
}

void
monothek_application_context_set_default_soundcard_thread(AgsSoundProvider *sound_provider,
							  GObject *soundcard_thread)
{
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set default soundcard thread */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard_thread == (AgsThread *) soundcard_thread){
    g_rec_mutex_unlock(application_context_mutex);
  
    return;
  }

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard_thread != NULL){
    g_object_unref(MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard_thread);
  }
  
  if(soundcard_thread != NULL){
    g_object_ref(soundcard_thread);
  }
  
  MONOTHEK_APPLICATION_CONTEXT(application_context)->default_soundcard_thread = (AgsThread *) soundcard_thread;

  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_soundcard(AgsSoundProvider *sound_provider)
{
  AgsApplicationContext *application_context;

  GList *soundcard;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get soundcard */
  g_rec_mutex_lock(application_context_mutex);
  
  soundcard = g_list_copy_deep(MONOTHEK_APPLICATION_CONTEXT(application_context)->soundcard,
			       (GCopyFunc) g_object_ref,
			       NULL);

  g_rec_mutex_unlock(application_context_mutex);
  
  return(soundcard);
}

void
monothek_application_context_set_soundcard(AgsSoundProvider *sound_provider,
					   GList *soundcard)
{
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set soundcard */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->soundcard == soundcard){
    g_rec_mutex_unlock(application_context_mutex);

    return;
  }
  
  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->soundcard != NULL){
    g_list_free(MONOTHEK_APPLICATION_CONTEXT(application_context)->soundcard);
  }

  MONOTHEK_APPLICATION_CONTEXT(application_context)->soundcard = soundcard;

  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_sequencer(AgsSoundProvider *sound_provider)
{
  AgsApplicationContext *application_context;

  GList *sequencer;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get sequencer */
  g_rec_mutex_lock(application_context_mutex);
  
  sequencer = g_list_copy_deep(MONOTHEK_APPLICATION_CONTEXT(application_context)->sequencer,
			       (GCopyFunc) g_object_ref,
			       NULL);

  g_rec_mutex_unlock(application_context_mutex);
  
  return(sequencer);
}

void
monothek_application_context_set_sequencer(AgsSoundProvider *sound_provider,
					   GList *sequencer)
{
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set sequencer */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->sequencer == sequencer){
    g_rec_mutex_unlock(application_context_mutex);

    return;
  }

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->sequencer != NULL){
    g_list_free(MONOTHEK_APPLICATION_CONTEXT(application_context)->sequencer);
  }
  
  MONOTHEK_APPLICATION_CONTEXT(application_context)->sequencer = sequencer;

  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_sound_server(AgsSoundProvider *sound_provider)
{
  AgsApplicationContext *application_context;

  GList *sound_server;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get sound server */
  g_rec_mutex_lock(application_context_mutex);
  
  sound_server = g_list_copy_deep(MONOTHEK_APPLICATION_CONTEXT(application_context)->sound_server,
				  (GCopyFunc) g_object_ref,
				  NULL);

  g_rec_mutex_unlock(application_context_mutex);
  
  return(sound_server);
}

void
monothek_application_context_set_sound_server(AgsSoundProvider *concurrency_provider,
					      GList *sound_server)
{
  MonothekApplicationContext *application_context;
  
  GRecMutex *application_context_mutex;

  application_context = MONOTHEK_APPLICATION_CONTEXT(concurrency_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set sound_server */
  g_rec_mutex_lock(application_context_mutex);

  if(application_context->sound_server == sound_server){
    g_rec_mutex_unlock(application_context_mutex);
    
    return;
  }

  g_list_free_full(application_context->sound_server,
		   g_object_unref);
  
  application_context->sound_server = sound_server;

  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_audio(AgsSoundProvider *sound_provider)
{
  AgsApplicationContext *application_context;

  GList *audio;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get audio */
  g_rec_mutex_lock(application_context_mutex);
  
  audio = g_list_copy_deep(MONOTHEK_APPLICATION_CONTEXT(application_context)->audio,
			   (GCopyFunc) g_object_ref,
			   NULL);

  g_rec_mutex_unlock(application_context_mutex);
  
  return(audio);
}

void
monothek_application_context_set_audio(AgsSoundProvider *sound_provider,
				       GList *audio)
{
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set audio */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->audio == audio){
    g_rec_mutex_unlock(application_context_mutex);

    return;
  }

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->audio != NULL){
    g_list_free(MONOTHEK_APPLICATION_CONTEXT(application_context)->audio);
  }

  MONOTHEK_APPLICATION_CONTEXT(application_context)->audio = audio;

  g_rec_mutex_unlock(application_context_mutex);
}

GList*
monothek_application_context_get_osc_server(AgsSoundProvider *sound_provider)
{
  AgsApplicationContext *application_context;

  GList *osc_server;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* get osc_server */
  g_rec_mutex_lock(application_context_mutex);
  
  osc_server = g_list_copy_deep(MONOTHEK_APPLICATION_CONTEXT(application_context)->osc_server,
				(GCopyFunc) g_object_ref,
				NULL);

  g_rec_mutex_unlock(application_context_mutex);
  
  return(osc_server);
}

void
monothek_application_context_set_osc_server(AgsSoundProvider *sound_provider,
					    GList *osc_server)
{
  AgsApplicationContext *application_context;

  GRecMutex *application_context_mutex;

  application_context = AGS_APPLICATION_CONTEXT(sound_provider);
  
  /* get mutex */
  application_context_mutex = AGS_APPLICATION_CONTEXT_GET_OBJ_MUTEX(application_context);

  /* set osc_server */
  g_rec_mutex_lock(application_context_mutex);

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->osc_server == osc_server){
    g_rec_mutex_unlock(application_context_mutex);

    return;
  }

  if(MONOTHEK_APPLICATION_CONTEXT(application_context)->osc_server != NULL){
    g_list_free(MONOTHEK_APPLICATION_CONTEXT(application_context)->osc_server);
  }

  MONOTHEK_APPLICATION_CONTEXT(application_context)->osc_server = osc_server;

  g_rec_mutex_unlock(application_context_mutex);
}

void
monothek_application_context_load_config(AgsApplicationContext *application_context)
{
  //TODO:JK: implement me
}

void
monothek_application_context_prepare(AgsApplicationContext *application_context)
{
  MonothekApplicationContext *monothek_application_context;
  MonothekWindow *window;

#ifdef AGS_WITH_QUARTZ
  GtkosxApplication *app;
#endif
  
  AgsThread *audio_loop;
  AgsTaskLauncher *task_launcher;
  AgsMessageDelivery *message_delivery;
  AgsMessageQueue *message_queue;
  AgsMessageQueue *audio_message_queue;

  GMainContext *server_main_context;
  GMainContext *audio_main_context;
  GMainContext *osc_server_main_context;
  GMainLoop *main_loop;
  GThread *main_loop_thread;
  
  GList *start_queue;
  
  gchar *filename;

  guint i;

  static const guint staging_program[] = {
					  (AGS_SOUND_STAGING_AUTOMATE | AGS_SOUND_STAGING_RUN_INTER | AGS_SOUND_STAGING_FX),
  };
  
  monothek_application_context = (MonothekApplicationContext *) application_context;

  /* call parent */
  //  AGS_APPLICATION_CONTEXT_CLASS(monothek_application_context_parent_class)->prepare(application_context);
  
  /* register types */
  ags_application_context_register_types(monothek_application_context);

  /*
   * fundamental thread setup
   */
  /* server main context and main loop */
  server_main_context = g_main_context_new();
  g_main_context_ref(server_main_context);

  monothek_application_context->server_main_context = server_main_context;

  main_loop = g_main_loop_new(server_main_context,
			      TRUE);

  g_thread_new("Advanced Gtk+ Sequencer - server main loop",
	       monothek_application_context_server_main_loop_thread,
	       main_loop);

  /* audio main context and main loop */
#if 1
  audio_main_context = g_main_context_new();
  g_main_context_ref(audio_main_context);

  monothek_application_context->audio_main_context = audio_main_context;

  main_loop = g_main_loop_new(audio_main_context,
			      TRUE);

  g_thread_new("Advanced Gtk+ Sequencer - audio main loop",
	       monothek_application_context_audio_main_loop_thread,
	       main_loop);
#else
  monothek_application_context->audio_main_context = NULL;  
#endif
  
  /* message delivery */
  message_delivery = ags_message_delivery_get_instance();

  message_queue = ags_message_queue_new("libags");
  g_object_set(message_queue,
	       "recipient-namespace", "libgsequencer",
	       NULL);
  ags_message_delivery_add_message_queue(message_delivery,
					 (GObject *) message_queue);

  audio_message_queue = ags_message_queue_new("libags-audio");
  g_object_set(audio_message_queue,
	       "recipient-namespace", "libgsequencer",
	       NULL);
  ags_message_delivery_add_message_queue(message_delivery,
					 (GObject *) audio_message_queue);
  
  /* OSC server main context and main loop */
  osc_server_main_context = g_main_context_new();
  g_main_context_ref(osc_server_main_context);

  monothek_application_context->osc_server_main_context = osc_server_main_context;

  g_main_loop_new(osc_server_main_context,
		  TRUE);

  /* AgsAudioLoop */
  audio_loop = (AgsThread *) ags_audio_loop_new();
  ags_audio_loop_set_do_fx_staging(audio_loop, TRUE);
  ags_audio_loop_set_staging_program(audio_loop,
				     staging_program,
				     1);
  
  g_object_ref(audio_loop);
  
  application_context->main_loop = (GObject *) audio_loop;
  ags_connectable_connect(AGS_CONNECTABLE(audio_loop));

  /* AgsTaskLauncher */
  task_launcher = ags_task_launcher_new();
  g_object_ref(task_launcher);

  application_context->task_launcher = (GObject *) task_launcher;
  ags_connectable_connect(AGS_CONNECTABLE(task_launcher));  

#if 0  
  ags_task_launcher_attach(task_launcher,
			   audio_main_context);
#endif
  
  /* start audio loop and thread pool*/
  ags_thread_start(audio_loop);
  
  /* wait for audio loop */
  g_mutex_lock(AGS_THREAD_GET_START_MUTEX(audio_loop));

  if(ags_thread_test_status_flags(audio_loop, AGS_THREAD_STATUS_START_WAIT)){
    ags_thread_unset_status_flags(audio_loop, AGS_THREAD_STATUS_START_DONE);
      
    while(ags_thread_test_status_flags(audio_loop, AGS_THREAD_STATUS_START_WAIT) &&
	  !ags_thread_test_status_flags(audio_loop, AGS_THREAD_STATUS_START_DONE)){
      g_cond_wait(AGS_THREAD_GET_START_COND(audio_loop),
		  AGS_THREAD_GET_START_MUTEX(audio_loop));
    }
  }
    
  g_mutex_unlock(AGS_THREAD_GET_START_MUTEX(audio_loop));

  /* start gui */
  g_timeout_add((guint) (1000.0 / 30.0),
		(GSourceFunc) monothek_application_context_message_monitor_timeout,
		(gpointer) monothek_application_context);
  
  g_timeout_add((guint) (1000.0 / 30.0),
		(GSourceFunc) monothek_application_context_task_timeout,
		(gpointer) monothek_application_context);

  /* MonothekWindow */
  window = g_object_new(MONOTHEK_TYPE_WINDOW,
			NULL);
  g_object_set(monothek_application_context,
	       "window", window,
	       NULL);

  //  gtk_window_set_default_size((GtkWindow *) window, 1920, 1080);
  gtk_window_fullscreen((GtkWindow *) window);

  ags_connectable_connect(AGS_CONNECTABLE(window));
  gtk_widget_show_all(window);

  /* set initial view */
#if !defined(MONOTHEK_NO_LOAD_VIEW)
  monothek_window_change_view(window,
			      MONOTHEK_TYPE_LOAD_VIEW, G_TYPE_NONE);
#else
  monothek_window_change_view(window,
			      MONOTHEK_TYPE_START_VIEW, G_TYPE_NONE);
#endif
  
  /* start gui thread */
  g_atomic_int_set(&(monothek_application_context->gui_ready),
		   TRUE);

  gtk_main();
}

void
monothek_application_context_setup(AgsApplicationContext *application_context)
{
  MonothekApplicationContext *monothek_application_context;

  MonothekRack *rack;

  MonothekAudioFileManager *audio_file_manager;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GObject *soundcard;
  GObject *sequencer;
  AgsJackServer *jack_server;
  AgsPulseServer *pulse_server;
  AgsCoreAudioServer *core_audio_server;

  AgsOscServer *osc_server;
  
  AgsLadspaManager *ladspa_manager;
  AgsDssiManager *dssi_manager;
  AgsLv2Manager *lv2_manager;
  AgsLv2uiManager *lv2ui_manager;
  AgsLv2WorkerManager *lv2_worker_manager;

  AgsServer *server;
  
  AgsThread *main_loop;
  AgsThread *soundcard_thread;
  AgsThread *export_thread;
  AgsThread *sequencer_thread;
  AgsDestroyWorker *destroy_worker;
  AgsTaskLauncher *task_launcher;

  AgsLog *log;
  AgsConfig *config;

  GList *list;  
  GSList *slist;
  
#ifndef AGS_W32API
  struct passwd *pw;
  
  uid_t uid;
#endif

  gchar *blacklist_path;
  gchar *blacklist_filename;
  gchar *filename;
  gchar *server_group;
  gchar *soundcard_group;
  gchar *sequencer_group;
  gchar *osc_server_group;
  gchar *str;
  gchar *capability;
#if defined AGS_W32API
  gchar *app_dir;
#endif
  
  guint i, j;
  gboolean has_core_audio;
  gboolean has_pulse;
  gboolean has_jack;
  gboolean is_output;

  GValue *value;
  
  monothek_application_context = (MonothekApplicationContext *) application_context;

  config = ags_config_get_instance();
  
  /* call parent */
  //  AGS_APPLICATION_CONTEXT_CLASS(monothek_application_context_parent_class)->setup(application_context);

  /* main loop */
  main_loop = ags_concurrency_provider_get_main_loop(AGS_CONCURRENCY_PROVIDER(application_context));

  log = ags_log_get_instance();
    
  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* AgsSoundcard */
  monothek_application_context->soundcard = NULL;
  soundcard = NULL;

  /* sound server */
  monothek_application_context->sound_server = NULL;

  /* core audio server */
  core_audio_server = ags_core_audio_server_new(NULL);
  monothek_application_context->sound_server = g_list_append(monothek_application_context->sound_server,
							     core_audio_server);
  g_object_ref(G_OBJECT(core_audio_server));

  has_core_audio = FALSE;

  /* pulse server */
  pulse_server = ags_pulse_server_new(NULL);
  monothek_application_context->sound_server = g_list_append(monothek_application_context->sound_server,
							     pulse_server);
  g_object_ref(G_OBJECT(pulse_server));

  has_pulse = FALSE;
  
  /* jack server */
  jack_server = ags_jack_server_new(NULL);
  monothek_application_context->sound_server = g_list_append(monothek_application_context->sound_server,
							     jack_server);
  g_object_ref(G_OBJECT(jack_server));

  has_jack = FALSE;

  /* AgsSoundcard */
  monothek_application_context->soundcard = NULL;
  soundcard = NULL;

  soundcard_group = g_strdup("soundcard");
  
  for(i = 0; ; i++){
    guint pcm_channels, buffer_size, samplerate, format;
    guint cache_buffer_size;
    gboolean use_cache;
    
    if(!g_key_file_has_group(config->key_file,
			     soundcard_group)){
      if(i == 0){
	g_free(soundcard_group);    
	soundcard_group = g_strdup_printf("%s-%d",
					  AGS_CONFIG_SOUNDCARD,
					  i);
    	
	continue;
      }else{
	break;
      }
    }
    
    str = ags_config_get_value(config,
			       soundcard_group,
			       "backend");

    capability = ags_config_get_value(config,
				      soundcard_group,
				      "capability");

    is_output = TRUE;

    if(capability != NULL &&
       !g_ascii_strncasecmp(capability,
			    "capture",
			    8)){
      is_output = FALSE;
    }

    /* change soundcard */
    if(str != NULL){
      if(!g_ascii_strncasecmp(str,
			      "core-audio",
			      11)){
	soundcard = ags_sound_server_register_soundcard(AGS_SOUND_SERVER(core_audio_server),
							is_output);

	has_core_audio = TRUE;
      }else if(!g_ascii_strncasecmp(str,
				    "pulse",
				    6)){
	soundcard = ags_sound_server_register_soundcard(AGS_SOUND_SERVER(pulse_server),
							is_output);

	has_pulse = TRUE;
      }else if(!g_ascii_strncasecmp(str,
				    "jack",
				    5)){
	if(!is_output){
	  AgsJackClient *input_client;

	  g_object_get(jack_server,
		       "input-jack-client", &input_client,
		       NULL);

	  if(input_client == NULL){
	    input_client = ags_jack_client_new((GObject *) jack_server);
	    g_object_set(jack_server,
			 "input-jack-client", input_client,
			 NULL);
	    ags_jack_server_add_client(jack_server,
				       (GObject *) input_client);
    
	    ags_jack_client_open((AgsJackClient *) input_client,
				 "ags-input-client");	    
	  }else{
	    g_object_unref(input_client);
	  }
	}
	
	soundcard = ags_sound_server_register_soundcard(AGS_SOUND_SERVER(jack_server),
							is_output);
	
	has_jack = TRUE;
      }else if(!g_ascii_strncasecmp(str,
				    "alsa",
				    5)){
	if(is_output){
	  soundcard = (GObject *) ags_devout_new((GObject *) monothek_application_context);
	  
	  AGS_DEVOUT(soundcard)->flags &= (~AGS_DEVOUT_OSS);
	  AGS_DEVOUT(soundcard)->flags |= AGS_DEVOUT_ALSA;
	}else{
	  soundcard = (GObject *) ags_devin_new((GObject *) monothek_application_context);
	  
	  AGS_DEVIN(soundcard)->flags &= (~AGS_DEVIN_OSS);
	  AGS_DEVIN(soundcard)->flags |= AGS_DEVIN_ALSA;
	}
      }else if(!g_ascii_strncasecmp(str,
				    "wasapi",
				    7)){
	gchar *str;
	
	if(is_output){
	  soundcard = (GObject *) ags_wasapi_devout_new((GObject *) monothek_application_context);	  

	  str = ags_config_get_value(config,
				     soundcard_group,
				     "wasapi-share-mode");

	  if(str != NULL &&
	     !g_ascii_strncasecmp(str,
				  "exclusive",
				  10)){
	    ags_wasapi_devout_set_flags(AGS_WASAPI_DEVOUT(soundcard),
					AGS_WASAPI_DEVOUT_SHARE_MODE_EXCLUSIVE);
	  }else{
	    ags_wasapi_devout_unset_flags(AGS_WASAPI_DEVOUT(soundcard),
					  AGS_WASAPI_DEVOUT_SHARE_MODE_EXCLUSIVE);
	  }

	  g_free(str);
	  
	  str = ags_config_get_value(config,
				     soundcard_group,
				     "wasapi-buffer-size");

	  if(str != NULL){
	    AGS_WASAPI_DEVOUT(soundcard)->wasapi_buffer_size = g_ascii_strtoull(str,
										NULL,
										10);
	    
	    g_free(str);
	  }
	}else{
	  soundcard = (GObject *) ags_wasapi_devin_new((GObject *) monothek_application_context);

	  str = ags_config_get_value(config,
				     soundcard_group,
				     "wasapi-share-mode");

	  if(str != NULL &&
	     !g_ascii_strncasecmp(str,
				  "exclusive",
				  10)){
	    ags_wasapi_devin_set_flags(AGS_WASAPI_DEVIN(soundcard),
				       AGS_WASAPI_DEVIN_SHARE_MODE_EXCLUSIVE);
	  }else{
	    ags_wasapi_devin_unset_flags(AGS_WASAPI_DEVIN(soundcard),
					 AGS_WASAPI_DEVIN_SHARE_MODE_EXCLUSIVE);
	  }

	  g_free(str);
	  
	  str = ags_config_get_value(config,
				     soundcard_group,
				     "wasapi-buffer-size");

	  if(str != NULL){
	    AGS_WASAPI_DEVIN(soundcard)->wasapi_buffer_size = g_ascii_strtoull(str,
									       NULL,
									       10);
	    
	    g_free(str);
	  }
	}
      }else if(!g_ascii_strncasecmp(str,
				    "oss",
				    4)){
	if(is_output){
	  soundcard = (GObject *) ags_devout_new((GObject *) monothek_application_context);

	  AGS_DEVOUT(soundcard)->flags &= (~AGS_DEVOUT_ALSA);
	  AGS_DEVOUT(soundcard)->flags |= AGS_DEVOUT_OSS;
	}else{
	  soundcard = (GObject *) ags_devin_new((GObject *) monothek_application_context);

	  AGS_DEVIN(soundcard)->flags &= (~AGS_DEVIN_ALSA);
	  AGS_DEVIN(soundcard)->flags |= AGS_DEVIN_OSS;	  
	}
      }else{
	g_warning(i18n("unknown soundcard backend - %s"), str);

	g_free(soundcard_group);    
	soundcard_group = g_strdup_printf("%s-%d",
					  AGS_CONFIG_SOUNDCARD,
					  i);
    
	continue;
      }
    }else{
      g_warning(i18n("unknown soundcard backend - NULL"));

      g_free(soundcard_group);    
      soundcard_group = g_strdup_printf("%s-%d",
					AGS_CONFIG_SOUNDCARD,
					i);
          
      continue;
    }
    
    monothek_application_context->soundcard = g_list_append(monothek_application_context->soundcard,
							    soundcard);
    g_object_ref(soundcard);

    /* device */
    str = ags_config_get_value(config,
			       soundcard_group,
			       "device");

    if(str != NULL){
      ags_soundcard_set_device(AGS_SOUNDCARD(soundcard),
			       str);
      g_free(str);
    }
    
    /* presets */
    pcm_channels = AGS_SOUNDCARD_DEFAULT_PCM_CHANNELS;
    buffer_size = AGS_SOUNDCARD_DEFAULT_BUFFER_SIZE;
    samplerate = AGS_SOUNDCARD_DEFAULT_SAMPLERATE;
    format = AGS_SOUNDCARD_DEFAULT_FORMAT;

    str = ags_config_get_value(config,
			       soundcard_group,
			       "pcm-channels");

    if(str != NULL){
      pcm_channels = g_ascii_strtoull(str,
				      NULL,
				      10);
      g_free(str);
    }

    str = ags_config_get_value(config,
			       soundcard_group,
			       "buffer-size");

    if(str != NULL){
      buffer_size = g_ascii_strtoull(str,
				     NULL,
				     10);
      g_free(str);
    }

    str = ags_config_get_value(config,
			       soundcard_group,
			       "samplerate");

    if(str != NULL){
      samplerate = g_ascii_strtoull(str,
				    NULL,
				    10);
      g_free(str);
    }

    str = ags_config_get_value(config,
			       soundcard_group,
			       "format");

    if(str != NULL){
      format = g_ascii_strtoull(str,
				NULL,
				10);
      g_free(str);
    }

    ags_soundcard_set_presets(AGS_SOUNDCARD(soundcard),
			      pcm_channels,
			      samplerate,
			      buffer_size,
			      format);

    use_cache = TRUE;
    str = ags_config_get_value(config,
			       soundcard_group,
			       "use-cache");

    if(str != NULL &&
       !g_strncasecmp(str,
		      "false",
		      5)){
      use_cache = FALSE;
    }

    cache_buffer_size = 4096;
    str = ags_config_get_value(config,
			       soundcard_group,
			       "cache-buffer-size");

    if(str != NULL){
      cache_buffer_size = g_ascii_strtoull(str,
					   NULL,
					   10);
    }

    if(AGS_IS_PULSE_DEVOUT(soundcard)){
      GList *start_port, *port;

      g_object_get(soundcard,
		   "pulse-port", &start_port,
		   NULL);

      port = start_port;

      while(port != NULL){
	ags_pulse_port_set_samplerate(port->data,
				      samplerate);
	ags_pulse_port_set_pcm_channels(port->data,
					pcm_channels);
	ags_pulse_port_set_buffer_size(port->data,
				       buffer_size);
	ags_pulse_port_set_format(port->data,
				  format);
	ags_pulse_port_set_cache_buffer_size(port->data,
					     buffer_size * ceil(cache_buffer_size / buffer_size));
	
	port = port->next;
      }

      g_list_free_full(start_port,
		       g_object_unref);
    }else if(AGS_IS_CORE_AUDIO_DEVOUT(soundcard)){
      GList *start_port, *port;

      g_object_get(soundcard,
		   "core_audio-port", &start_port,
		   NULL);

      port = start_port;

      while(port != NULL){
	ags_core_audio_port_set_samplerate(port->data,
					   samplerate);
	ags_core_audio_port_set_pcm_channels(port->data,
					     pcm_channels);
	ags_core_audio_port_set_buffer_size(port->data,
					    buffer_size);
	ags_core_audio_port_set_format(port->data,
				       format);
	ags_core_audio_port_set_cache_buffer_size(port->data,
						  buffer_size * ceil(cache_buffer_size / buffer_size));
	
	port = port->next;
      }

      g_list_free_full(start_port,
		       g_object_unref);
    }
    
    g_free(soundcard_group);    
    soundcard_group = g_strdup_printf("%s-%d",
				      AGS_CONFIG_SOUNDCARD,
				      i);
  }

  if(monothek_application_context->soundcard != NULL){
    soundcard = monothek_application_context->soundcard->data;
  }  

  g_free(soundcard_group);

  ags_sound_provider_set_default_soundcard(AGS_SOUND_PROVIDER(monothek_application_context),
					   soundcard);
  
  /* AgsSequencer */
  monothek_application_context->sequencer = NULL;
  
  /* MonothekRack */
  value = g_hash_table_lookup(session->value,
			      "rack");

  rack = g_value_get_object(value);
  
  g_object_set(rack,
	       "output-soundcard", soundcard,
	       NULL);
  monothek_rack_setup_tree(rack);
  
  /* AgsServer */
  monothek_application_context->server = NULL;
  server = NULL;

  server_group = g_strdup("server");
  
  for(i = 0; ; i++){
    gchar *ip4, *ip6;

    guint server_port;
    gboolean auto_start;
    gboolean any_address;
    gboolean enable_ip4, enable_ip6;
    
    if(!g_key_file_has_group(config->key_file,
			     server_group)){
      if(i == 0){
	g_free(server_group);    
	server_group = g_strdup_printf("%s-%d",
				       AGS_CONFIG_SERVER,
				       i);
    	
	continue;
      }else{
	break;
      }
    }

    server = ags_server_new();

    monothek_application_context->server = g_list_append(monothek_application_context->server,
							 server);
    g_object_ref(server);

    /* realm */
    str = ags_config_get_value(config,
			       server_group,
			       "realm");
    
    if(str != NULL){
      g_object_set(server,
		   "realm", str,
		   NULL);
      
      g_free(str);
    }

    /* any address */
    any_address = FALSE;

    str = ags_config_get_value(config,
			       server_group,
			       "any-address");
    
    if(str != NULL){
      any_address = (!g_ascii_strncasecmp(str,
					  "true",
					  5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(any_address){
      ags_server_set_flags(server,
			   (AGS_SERVER_ANY_ADDRESS));
    }

    /* enable ip4 and ip6 */
    enable_ip4 = FALSE;
    enable_ip6 = FALSE;

    str = ags_config_get_value(config,
			       server_group,
			       "enable-ip4");
    
    if(str != NULL){
      enable_ip4 = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    str = ags_config_get_value(config,
			       server_group,
			       "enable-ip6");

    if(str != NULL){
      enable_ip6 = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(enable_ip4){
      ags_server_set_flags(server,
			   (AGS_SERVER_INET4));
    }

    if(enable_ip6){
      ags_server_set_flags(server,
			   (AGS_SERVER_INET6));
    }

    /* ip4 and ip6 address */
    str = ags_config_get_value(config,
			       server_group,
			       "ip4-address");

    if(str != NULL){
      g_object_set(server,
		   "ip4", str,
		   NULL);
      
      g_free(str);
    }

    str = ags_config_get_value(config,
			       server_group,
			       "ip6-address");

    if(str != NULL){
      g_object_set(server,
		   "ip6", str,
		   NULL);
      
      g_free(str);
    }

    /* server port */
    str = ags_config_get_value(config,
			       server_group,
			       "server-port");

    if(str != NULL){
      server_port = (guint) g_ascii_strtoull(str,
					     NULL,
					     10);

      g_object_set(server,
		   "server-port", server_port,
		   NULL);
    }
    
    /* auto-start */
    auto_start = FALSE;
    
    str = ags_config_get_value(config,
			       server_group,
			       "auto-start");

    if(str != NULL){
      auto_start = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(auto_start){
      ags_server_set_flags(server, AGS_SERVER_AUTO_START);
    }

    g_free(server_group);    
    server_group = g_strdup_printf("%s-%d",
				   AGS_CONFIG_SERVER,
				   i);
  }

  if(server == NULL){
    server = ags_server_new();
    ags_server_set_flags(server,
			 (AGS_SERVER_INET4));

    monothek_application_context->server = g_list_append(monothek_application_context->server,
							 server);
    g_object_ref(server);
  }

  /* AgsSoundcardThread and AgsExportThread */
  monothek_application_context->default_soundcard_thread = NULL;
  list = monothek_application_context->soundcard;
    
  while(list != NULL){
    guint soundcard_capability;

    soundcard_capability = ags_soundcard_get_capability(AGS_SOUNDCARD(list->data));
    
    soundcard_thread = (AgsThread *) ags_soundcard_thread_new(list->data,
							      soundcard_capability);
    ags_thread_add_child_extended(main_loop,
				  soundcard_thread,
				  TRUE, TRUE);

    /* export thread */
    export_thread = NULL;
    
    /* export thread */
    if(AGS_IS_DEVOUT(list->data) ||
       AGS_IS_WASAPI_DEVOUT(list->data) ||
       AGS_IS_JACK_DEVOUT(list->data) ||
       AGS_IS_PULSE_DEVOUT(list->data) ||
       AGS_IS_CORE_AUDIO_DEVOUT(list->data)){
      export_thread = (AgsThread *) ags_export_thread_new(list->data,
							  NULL);
      ags_thread_add_child_extended(main_loop,
				    (AgsThread *) export_thread,
				    TRUE, TRUE);
    }    

    /* default soundcard thread */
    if(monothek_application_context->default_soundcard_thread == NULL){
      ags_sound_provider_set_default_soundcard_thread(AGS_SOUND_PROVIDER(monothek_application_context),
						      (GObject *) soundcard_thread);
    }

    /* default export thread */
    if(export_thread != NULL){
      if(monothek_application_context->default_export_thread == NULL){
	monothek_application_context->default_export_thread = export_thread;
      }else{
	g_object_unref(export_thread);
      }
    }

    /* iterate */
    list = list->next;      
  }
  
  /* AgsSequencerThread */
  list = monothek_application_context->sequencer;
    
  while(list != NULL){
    sequencer_thread = (AgsThread *) ags_sequencer_thread_new(list->data);
    ags_thread_add_child_extended(main_loop,
				  (AgsThread *) sequencer_thread,
				  TRUE, TRUE);

    list = list->next;      
  }

  /* AgsOscServer */
  monothek_application_context->osc_server = NULL;
  osc_server = NULL;

  osc_server_group = g_strdup("osc-server");
  
  for(i = 0; ; i++){
    gchar *ip4, *ip6;

    guint server_port;
    gboolean auto_start;
    gboolean any_address;
    gboolean enable_ip4, enable_ip6;
    
    if(!g_key_file_has_group(config->key_file,
			     osc_server_group)){
      if(i == 0){
	g_free(osc_server_group);    
	osc_server_group = g_strdup_printf("%s-%d",
					   AGS_CONFIG_OSC_SERVER,
					   i);
    	
	continue;
      }else{
	break;
      }
    }

    osc_server = ags_osc_server_new();
    ags_osc_server_set_flags(osc_server,
			     (AGS_OSC_SERVER_TCP));

    monothek_application_context->osc_server = g_list_append(monothek_application_context->osc_server,
							     osc_server);
    g_object_ref(osc_server);

    /* any address */
    any_address = FALSE;

    str = ags_config_get_value(config,
			       osc_server_group,
			       "any-address");
    
    if(str != NULL){
      any_address = (!g_ascii_strncasecmp(str,
					  "true",
					  5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(any_address){
      ags_osc_server_set_flags(osc_server,
			       (AGS_OSC_SERVER_ANY_ADDRESS));
    }

    /* enable ip4 and ip6 */
    enable_ip4 = FALSE;
    enable_ip6 = FALSE;

    str = ags_config_get_value(config,
			       osc_server_group,
			       "enable-ip4");
    
    if(str != NULL){
      enable_ip4 = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    str = ags_config_get_value(config,
			       osc_server_group,
			       "enable-ip6");

    if(str != NULL){
      enable_ip6 = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(enable_ip4){
      ags_osc_server_set_flags(osc_server,
			       (AGS_OSC_SERVER_INET4));
    }

    if(enable_ip6){
      ags_osc_server_set_flags(osc_server,
			       (AGS_OSC_SERVER_INET6));
    }
    
    ags_osc_server_add_default_controller(osc_server);

    /* ip4 and ip6 address */
    str = ags_config_get_value(config,
			       osc_server_group,
			       "ip4-address");

    if(str != NULL){
      g_object_set(osc_server,
		   "ip4", str,
		   NULL);
      
      g_free(str);
    }

    str = ags_config_get_value(config,
			       osc_server_group,
			       "ip6-address");

    if(str != NULL){
      g_object_set(osc_server,
		   "ip6", str,
		   NULL);
      
      g_free(str);
    }

    /* server port */
    str = ags_config_get_value(config,
			       osc_server_group,
			       "server-port");

    if(str != NULL){
      server_port = (guint) g_ascii_strtoull(str,
					     NULL,
					     10);

      g_object_set(osc_server,
		   "server-port", server_port,
		   NULL);
    }
    
    /* auto-start */
    auto_start = FALSE;
    
    str = ags_config_get_value(config,
			       osc_server_group,
			       "auto-start");

    if(str != NULL){
      auto_start = (!g_ascii_strncasecmp(str,
					 "true",
					 5)) ? TRUE: FALSE;
      g_free(str);
    }

    if(auto_start){
      ags_osc_server_set_flags(osc_server, AGS_OSC_SERVER_AUTO_START);
    }

    g_free(osc_server_group);    
    osc_server_group = g_strdup_printf("%s-%d",
				       AGS_CONFIG_OSC_SERVER,
				       i);
  }

  if(osc_server == NULL){
    osc_server = ags_osc_server_new();
    ags_osc_server_set_flags(osc_server,
			     (AGS_OSC_SERVER_INET4 |
			      AGS_OSC_SERVER_TCP));

    ags_osc_server_add_default_controller(osc_server);

    monothek_application_context->osc_server = g_list_append(monothek_application_context->osc_server,
							     osc_server);
    g_object_ref(osc_server);
  }
  
  /* AgsWorkerThread */
  monothek_application_context->worker = NULL;

  /* AgsDestroyWorker */
  destroy_worker = ags_destroy_worker_get_instance();
  ags_thread_add_child_extended(main_loop,
				(AgsThread *) destroy_worker,
				TRUE, TRUE);
  monothek_application_context->worker = g_list_prepend(monothek_application_context->worker,
							destroy_worker);
  ags_thread_start((AgsThread *) destroy_worker);
  
  /* AgsThreadPool */
  monothek_application_context->thread_pool = NULL;

  /* launch */
  if(has_core_audio){
    ags_core_audio_server_connect_client(core_audio_server);
  }

  if(has_pulse){
    ags_pulse_server_connect_client(pulse_server);

    ags_pulse_server_start_poll(pulse_server);
  }

  if(has_jack){
    ags_jack_server_connect_client(jack_server);
  }
  
  /* audio file manager */
  audio_file_manager = monothek_audio_file_manager_get_instance();

  slist = NULL;
  
  {
    xmlDoc *doc;
    xmlNode *current_node;
    xmlXPathContext *xpath_context;
    xmlXPathObject *xpath_object;
    xmlNode **node;

    gchar *xpath;
    gchar *playlist_filename;

    /* read from XML */  
    playlist_filename = MONOTHEK_JUKEBOX_PLAYLIST_MODEL_FILENAME;

    doc = xmlReadFile(playlist_filename, NULL, 0);

    xpath = g_strdup_printf("/playlist/song/file");

    xpath_context = xmlXPathNewContext(doc);
    xpath_object = xmlXPathEval((xmlChar *) xpath,
				xpath_context);

    if(xpath_object->nodesetval != NULL){
      node = xpath_object->nodesetval->nodeTab;

      for(i = 0; i < xpath_object->nodesetval->nodeNr; i++){
	if(node[i]->type == XML_ELEMENT_NODE){
	  gchar *str;

	  str = xmlNodeGetContent(node[i]);

	  slist = g_slist_prepend(slist,
				  g_strdup(str));
	}
      }
    }

    slist = g_slist_reverse(slist);

    g_free(xpath);
  
    /* free XML doc */
    xmlFreeDoc(doc);
  }
  
  monothek_audio_file_manager_load_playlist(audio_file_manager,
					    slist);
  g_slist_free_full(slist,
		    g_free);

  /* unref */
  g_object_unref(main_loop);
}

void
monothek_application_context_register_types(AgsApplicationContext *application_context)
{
  ags_complex_get_type();

  /* */
  ags_connectable_get_type();

  /*  */
  ags_audio_loop_get_type();
  ags_soundcard_thread_get_type();

  monothek_export_thread_get_type();

  /*  */
  ags_audio_file_get_type();
  ags_audio_file_link_get_type();

  /* register tasks */
  ags_cancel_audio_get_type();
  ags_cancel_channel_get_type();

  //TODO:JK: extend me
  
  /* register recalls */
  ags_generic_recall_channel_run_get_type();

  ags_play_audio_get_type();
  ags_play_channel_get_type();
  ags_play_channel_run_get_type();
  ags_play_channel_run_master_get_type();

  ags_stream_channel_get_type();
  ags_stream_channel_run_get_type();

  ags_loop_channel_get_type();
  ags_loop_channel_run_get_type();

  ags_copy_channel_get_type();
  ags_copy_channel_run_get_type();

  ags_volume_channel_get_type();
  ags_volume_channel_run_get_type();

  ags_peak_channel_get_type();
  ags_peak_channel_run_get_type();

  ags_recall_ladspa_get_type();
  ags_recall_ladspa_run_get_type();

  ags_recall_dssi_get_type();
  ags_recall_dssi_run_get_type();

  ags_recall_lv2_get_type();
  ags_recall_lv2_run_get_type();

  ags_delay_audio_get_type();
  ags_delay_audio_run_get_type();

  ags_count_beats_audio_get_type();
  ags_count_beats_audio_run_get_type();

  ags_copy_pattern_audio_get_type();
  ags_copy_pattern_audio_run_get_type();
  ags_copy_pattern_channel_get_type();
  ags_copy_pattern_channel_run_get_type();

  ags_buffer_channel_get_type();
  ags_buffer_channel_run_get_type();

  ags_play_notation_audio_get_type();
  ags_play_notation_audio_run_get_type();

  ags_route_dssi_audio_get_type();
  ags_route_dssi_audio_run_get_type();

  ags_route_lv2_audio_get_type();
  ags_route_lv2_audio_run_get_type();
}

void
monothek_application_context_quit(AgsApplicationContext *application_context)
{  
  gtk_main_quit();
}

void*
monothek_application_context_server_main_loop_thread(GMainLoop *main_loop)
{
  AgsApplicationContext *application_context;

  GList *start_list, *list;

  g_main_context_push_thread_default(g_main_loop_get_context(main_loop));
  
  application_context = ags_application_context_get_instance();
  
  while(ags_ui_provider_get_show_animation(AGS_UI_PROVIDER(application_context))){
    g_usleep(G_USEC_PER_SEC / 30);
  }

  list = 
    start_list = ags_service_provider_get_server(AGS_SERVICE_PROVIDER(application_context));

  while(list != NULL){
    if(ags_server_test_flags(list->data, AGS_SERVER_AUTO_START)){
      ags_server_start(AGS_SERVER(list->data));
    }
    
    list = list->next;
  }

  g_list_free_full(start_list,
		   g_object_unref);
  
  g_main_loop_run(main_loop);

  g_thread_exit(NULL);

  return(NULL);
}

void*
monothek_application_context_audio_main_loop_thread(GMainLoop *main_loop)
{
  AgsApplicationContext *application_context;

  GList *start_list, *list;

#ifdef AGS_WITH_RT
  AgsPriority *priority;

  struct sched_param param;

  gchar *str;
#endif

  g_main_context_push_thread_default(g_main_loop_get_context(main_loop));
  
  application_context = ags_application_context_get_instance();

  while(ags_ui_provider_get_show_animation(AGS_UI_PROVIDER(application_context))){
    g_usleep(G_USEC_PER_SEC / 30);
  }

  /* real-time setup */
#ifdef AGS_WITH_RT
  priority = ags_priority_get_instance();  

  param.sched_priority = 95;

  str = ags_priority_get_value(priority,
			       AGS_PRIORITY_RT_THREAD,
			       AGS_PRIORITY_KEY_AUDIO_MAIN_LOOP);

  if(str != NULL){
    param.sched_priority = (int) g_ascii_strtoull(str,
						  NULL,
						  10);
  }

  if(str == NULL ||
     ((!g_ascii_strncasecmp(str,
			    "0",
			    2)) != TRUE)){
    if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
      perror("sched_setscheduler failed");
    }
  }
    
  g_free(str);
#endif
  
  list = 
    start_list = ags_sound_provider_get_osc_server(AGS_SERVICE_PROVIDER(application_context));

  while(list != NULL){
    if(ags_osc_server_test_flags(list->data, AGS_OSC_SERVER_AUTO_START)){
      ags_osc_server_start(AGS_OSC_SERVER(list->data));
    }
    
    list = list->next;
  }

  g_list_free_full(start_list,
		   g_object_unref);

  g_main_loop_run(main_loop);

  g_thread_exit(NULL);

  return(NULL);
}

gboolean
monothek_application_context_message_monitor_timeout(MonothekApplicationContext *monothek_application_context)
{
  ags_ui_provider_check_message(AGS_UI_PROVIDER(monothek_application_context));
  ags_ui_provider_clean_message(AGS_UI_PROVIDER(monothek_application_context));

  return(TRUE);
}

gboolean
monothek_application_context_task_timeout(MonothekApplicationContext *monothek_application_context)
{
  AgsTaskLauncher *task_launcher;

  if(monothek_application_context->task == NULL){
    return(TRUE);
  }
  
  task_launcher = ags_concurrency_provider_get_task_launcher(AGS_CONCURRENCY_PROVIDER(monothek_application_context));

  ags_task_launcher_add_task_all(task_launcher,
				 g_list_reverse(monothek_application_context->task));

  g_list_free_full(monothek_application_context->task,
		   g_object_unref);
  
  monothek_application_context->task = NULL;
  
  g_object_unref(task_launcher);

  return(TRUE);
}

MonothekApplicationContext*
monothek_application_context_new()
{
  MonothekApplicationContext *monothek_application_context;

  monothek_application_context = (MonothekApplicationContext *) g_object_new(MONOTHEK_TYPE_APPLICATION_CONTEXT,
									     NULL);

  return(monothek_application_context);
}
