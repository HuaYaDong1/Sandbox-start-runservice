#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <time.h>

#include <gio/gio.h>
#include <gio-unix-2.0/gio/gunixfdlist.h>

char app[1024];

static gint
get_server_gvariant_stdout (GDBusConnection  *connection,
                   const gchar      *name_owner,
                   GError          **error)
{
  GDBusMessage *method_call_message;
  GDBusMessage *method_reply_message;
  gint fd;
  const gchar * response;
  
  fd = -1;
  method_call_message = NULL;
  method_reply_message = NULL;

  method_call_message = g_dbus_message_new_method_call (name_owner,
                                                        "/org/gtk/GDBus/TestObject",
                                                        "org.gtk.GDBus.TestInterface",
                                                        "ExampleTuple");
 
  g_dbus_message_set_body (method_call_message, g_variant_new ("(s)",app));  
  method_reply_message = g_dbus_connection_send_message_with_reply_sync (connection,
                                                                       method_call_message,
                                                                       G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                       -1,
                                                                       NULL, /* out_serial */
                                                                       NULL, /* cancellable */
                                                                       error);
  if (method_reply_message == NULL)
      goto out;

  if (g_dbus_message_get_message_type (method_reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
      g_dbus_message_to_gerror (method_reply_message, error);
      goto out;
    }

fd = 1;
response = g_dbus_message_get_arg0(method_reply_message);

puts("-------------------------");
g_printf("Response: %s\n",response);
puts("-------------------------");


 out:
  g_object_unref (method_call_message);
  g_object_unref (method_reply_message);

  return fd;
}



static void
on_name_appeared (GDBusConnection *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data)
{
  gint fd;
  GError *error;

  error = NULL;
  fd = get_server_gvariant_stdout (connection, name_owner, &error);
  if (fd == -1)
    {
      g_printerr ("Error invoking ExampleTuple: %s\n",
                  error->message);
      g_error_free (error);
      exit (1);
    }
  else
    {

//---------------------------------客户端运行处--------------------------------------------------------

//-----------------------------------------------------------------------------------------------------



      close (fd);
      exit (0);
    }   
}

static void
on_name_vanished (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  g_printerr ("Failed to get name owner for %s\n"
              "Is ./gdbus-example-server running?\n",
              name);
  exit (1);
}

int
main (int argc, char *argv[])
{
  guint watcher_id;
  GMainLoop *loop;

  printf("%d\n",argc);
  if(argc < 2)
  { 
         puts("please input appname");
         return 0;
  }
  strcpy(app,argv[1]);
  puts(app);


  watcher_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                 "org.gtk.GDBus.kylin_runing_Server",
                                 G_BUS_NAME_WATCHER_FLAGS_NONE,
                                 on_name_appeared,
                                 on_name_vanished,
                                 NULL,
                                 NULL);

  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_bus_unwatch_name (watcher_id);
  return 0;
}
