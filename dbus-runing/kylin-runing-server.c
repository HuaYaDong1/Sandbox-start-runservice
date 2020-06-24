#include <stdlib.h>
#include <gio/gio.h>
#include <glib.h>
#include <glib/gprintf.h>

/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;

static const gchar introspection_xml[] =
  "<node>"
  " <interface name='org.gtk.GDBus.TestInterface'>"
  "    <annotation name='org.gtk.GDBus.Annotation' value='OnInterface'/>"
  "    <annotation name='org.gtk.GDBus.Annotation' value='AlsoOnInterface'/>"
  "  <method name='ExampleTuple'>"
  "      <annotation name='org.gtk.GDBus.Annotation' value='OnMethod'/>"
  "      <arg type='s' name='data' direction='in'/>"
  "      <arg type='s' name='response' direction='out'/>"
  "  </method>"
  " </interface>"
  "</node>";


/* ---------------------------------------------------------------------------------------------------- */

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
   if (g_strcmp0 (method_name, "ExampleTuple") == 0)
    {
      //const int value;
      const gchar *cadena;
      char mount[1024];
      char umount[1024];
      gchar *response;

      g_variant_get(parameters, "(&s)", &cadena);
      
        memset(umount, 0, sizeof(umount));
        memset(mount, 0, sizeof(mount));

        sprintf(umount, "umount /opt/%s",cadena);
        system(umount);
        puts(umount);

        sprintf(mount, "mount -t aufs -o  dirs=/opt/%s=rw:/opt/app_runtime=ro:/opt/base_runtime=ro none /opt/%s", cadena, cadena);
        system(mount);
        puts(mount);
      

      response = g_strdup_printf ("挂载 '%s' 完成!", cadena);
      g_dbus_method_invocation_return_value (invocation,g_variant_new ("(s)", response));
      g_printf("%s\n",response);
      g_free (response);
    }
}


/* for now */
static const GDBusInterfaceVTable interface_vtable =
{
  handle_method_call,
};

/* ---------------------------------------------------------------------------------------------------- */

static gboolean
on_timeout_cb (gpointer user_data)
{
  return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  guint registration_id;

  registration_id = g_dbus_connection_register_object (connection,
                                                       "/org/gtk/GDBus/TestObject",
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       NULL,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */
  g_assert (registration_id > 0);

  /* swap value of properties Foo and Bar every two seconds */
  g_timeout_add_seconds (2,
                         on_timeout_cb,
                         connection);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    g_print("Lost\n");
    exit (1);
}

int
main (int argc, char *argv[])
{
  guint owner_id;
  GMainLoop *loop;

  introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
  g_assert (introspection_data != NULL);

  owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                             "org.gtk.GDBus.kylin_runing_Server",
                             G_BUS_NAME_OWNER_FLAGS_NONE,
                             on_bus_acquired,
                             on_name_acquired,
                             on_name_lost,
                             NULL,
                             NULL);

  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_bus_unown_name (owner_id);

  g_dbus_node_info_unref (introspection_data);

  return 0;
}

