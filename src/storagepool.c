#include "storagepool.h"
#include "util.h"

#include <libvirt/libvirt.h>

static virStoragePoolPtr
virtDBusStoragePoolGetVirStoragePool(virtDBusConnect *connect,
                                     const gchar *objectPath,
                                     GError **error)
{
    virStoragePoolPtr storagePool;

    if (!virtDBusConnectOpen(connect, error))
        return NULL;

    storagePool = virtDBusUtilVirStoragePoolFromBusPath(connect->connection,
                                                        objectPath,
                                                        connect->storagePoolPath);
    if (!storagePool) {
        virtDBusUtilSetLastVirtError(error);
        return NULL;
    }

    return storagePool;
}

static void
virtDBusStoragePoolGetActive(const gchar *objectPath,
                             gpointer userData,
                             GVariant **value,
                             GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    gint active;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    active = virStoragePoolIsActive(storagePool);
    if (active < 0)
        return virtDBusUtilSetLastVirtError(error);

    *value = g_variant_new("b", !!active);
}

static void
virtDBusStoragePoolGetAutostart(const gchar *objectPath,
                                gpointer userData,
                                GVariant **value,
                                GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    gint autostart = 0;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolGetAutostart(storagePool, &autostart) < 0)
        return virtDBusUtilSetLastVirtError(error);

    *value = g_variant_new("b", !!autostart);
}

static void
virtDBusStoragePoolGetName(const gchar *objectPath,
                           gpointer userData,
                           GVariant **value,
                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    const gchar *name;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    name = virStoragePoolGetName(storagePool);
    if (!name)
        return virtDBusUtilSetLastVirtError(error);

    *value = g_variant_new("s", name);
}

static void
virtDBusStoragePoolGetPersistent(const gchar *objectPath,
                                 gpointer userData,
                                 GVariant **value,
                                 GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    gint persistent;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    persistent = virStoragePoolIsPersistent(storagePool);
    if (persistent < 0)
        return virtDBusUtilSetLastVirtError(error);

    *value = g_variant_new("b", !!persistent);
}

static void
virtDBusStoragePoolGetUUID(const gchar *objectPath,
                           gpointer userData,
                           GVariant **value,
                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    gchar uuid[VIR_UUID_STRING_BUFLEN] = "";

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolGetUUIDString(storagePool, uuid) < 0)
        return virtDBusUtilSetLastVirtError(error);

    *value = g_variant_new("s", uuid);
}

static void
virtDBusStoragePoolSetAutostart(GVariant *value,
                                const gchar *objectPath,
                                gpointer userData,
                                GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    gboolean autostart;

    g_variant_get(value, "b", &autostart);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolSetAutostart(storagePool, autostart) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolBuild(GVariant *inArgs,
                         GUnixFDList *inFDs G_GNUC_UNUSED,
                         const gchar *objectPath,
                         gpointer userData,
                         GVariant **outArgs G_GNUC_UNUSED,
                         GUnixFDList **outFDs G_GNUC_UNUSED,
                         GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    guint flags;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolBuild(storagePool, flags) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolCreate(GVariant *inArgs,
                          GUnixFDList *inFDs G_GNUC_UNUSED,
                          const gchar *objectPath,
                          gpointer userData,
                          GVariant **outArgs G_GNUC_UNUSED,
                          GUnixFDList **outFDs G_GNUC_UNUSED,
                          GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    guint flags;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolCreate(storagePool, flags) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolDelete(GVariant *inArgs,
                          GUnixFDList *inFDs G_GNUC_UNUSED,
                          const gchar *objectPath,
                          gpointer userData,
                          GVariant **outArgs G_GNUC_UNUSED,
                          GUnixFDList **outFDs G_GNUC_UNUSED,
                          GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    guint flags;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolDelete(storagePool, flags) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolDestroy(GVariant *inArgs G_GNUC_UNUSED,
                           GUnixFDList *inFDs G_GNUC_UNUSED,
                           const gchar *objectPath,
                           gpointer userData,
                           GVariant **outArgs G_GNUC_UNUSED,
                           GUnixFDList **outFDs G_GNUC_UNUSED,
                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolDestroy(storagePool) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolGetInfo(GVariant *inArgs G_GNUC_UNUSED,
                           GUnixFDList *inFDs G_GNUC_UNUSED,
                           const gchar *objectPath,
                           gpointer userData,
                           GVariant **outArgs,
                           GUnixFDList **outFDs G_GNUC_UNUSED,
                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autofree virStoragePoolInfoPtr info = NULL;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    info = g_new0(virStoragePoolInfo, 1);
    if (virStoragePoolGetInfo(storagePool, info) < 0)
        return virtDBusUtilSetLastVirtError(error);

    *outArgs = g_variant_new("((ittt))", info->state,
                             info->capacity, info->allocation,
                             info->available);
}

static void
virtDBusStoragePoolGetXMLDesc(GVariant *inArgs,
                              GUnixFDList *inFDs G_GNUC_UNUSED,
                              const gchar *objectPath,
                              gpointer userData,
                              GVariant **outArgs,
                              GUnixFDList **outFDs G_GNUC_UNUSED,
                              GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autofree gchar *xml = NULL;
    guint flags;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    xml = virStoragePoolGetXMLDesc(storagePool, flags);
    if (!xml)
        return virtDBusUtilSetLastVirtError(error);

    *outArgs = g_variant_new("(s)", xml);
}

static void
virtDBusStoragePoolListStorageVolumes(GVariant *inArgs,
                                      GUnixFDList *inFDs G_GNUC_UNUSED,
                                      const gchar *objectPath,
                                      gpointer userData,
                                      GVariant **outArgs,
                                      GUnixFDList **outFDs G_GNUC_UNUSED,
                                      GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autoptr(virStorageVolPtr) storageVols = NULL;
    guint flags;
    gint nVols;
    GVariantBuilder builder;
    GVariant *gstorageVols;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    nVols = virStoragePoolListAllVolumes(storagePool, &storageVols, flags);
    if (nVols < 0)
        return virtDBusUtilSetLastVirtError(error);

    g_variant_builder_init(&builder, G_VARIANT_TYPE("ao"));

    for (gint i = 0; i < nVols; i++) {
        g_autofree gchar *path = NULL;
        path = virtDBusUtilBusPathForVirStorageVol(storageVols[i],
                                                   connect->storageVolPath);

        g_variant_builder_add(&builder, "o", path);
    }

    gstorageVols = g_variant_builder_end(&builder);
    *outArgs = g_variant_new_tuple(&gstorageVols, 1);
}

static void
virtDBusStoragePoolRefresh(GVariant *inArgs,
                           GUnixFDList *inFDs G_GNUC_UNUSED,
                           const gchar *objectPath,
                           gpointer userData,
                           GVariant **outArgs G_GNUC_UNUSED,
                           GUnixFDList **outFDs G_GNUC_UNUSED,
                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    guint flags;

    g_variant_get(inArgs, "(u)", &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolRefresh(storagePool, flags) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static void
virtDBusStoragePoolStorageVolCreateXML(GVariant *inArgs,
                                       GUnixFDList *inFDs G_GNUC_UNUSED,
                                       const gchar *objectPath,
                                       gpointer userData,
                                       GVariant **outArgs,
                                       GUnixFDList **outFDs G_GNUC_UNUSED,
                                       GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autoptr(virStorageVol) storageVol = NULL;
    gchar *xml;
    guint flags;
    g_autofree gchar *path = NULL;

    g_variant_get(inArgs, "(&su)", &xml, &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    storageVol = virStorageVolCreateXML(storagePool, xml, flags);
    if (!storageVol)
        return virtDBusUtilSetLastVirtError(error);

    path = virtDBusUtilBusPathForVirStorageVol(storageVol,
                                               connect->storageVolPath);

    *outArgs = g_variant_new("(o)", path);
}

static void
virtDBusStoragePoolStorageVolCreateXMLFrom(GVariant *inArgs,
                                           GUnixFDList *inFDs G_GNUC_UNUSED,
                                           const gchar *objectPath,
                                           gpointer userData,
                                           GVariant **outArgs,
                                           GUnixFDList **outFDs G_GNUC_UNUSED,
                                           GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autoptr(virStorageVol) storageVol = NULL;
    g_autoptr(virStorageVol) storageVolOld = NULL;
    const gchar *key;
    const gchar *xml;
    guint flags;
    g_autofree gchar *path = NULL;

    g_variant_get(inArgs, "(&s&su)", &xml, &key, &flags);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    storageVolOld = virStorageVolLookupByKey(connect->connection, key);
    if (!storageVolOld)
        return virtDBusUtilSetLastVirtError(error);

    storageVol = virStorageVolCreateXMLFrom(storagePool, xml, storageVolOld,
                                            flags);
    if (!storageVol)
        return virtDBusUtilSetLastVirtError(error);

    path = virtDBusUtilBusPathForVirStorageVol(storageVol,
                                               connect->storageVolPath);

    *outArgs = g_variant_new("(o)", path);
}

static void
virtDBusStoragePoolStorageVolLookupByName(GVariant *inArgs,
                                          GUnixFDList *inFDs G_GNUC_UNUSED,
                                          const gchar *objectPath,
                                          gpointer userData,
                                          GVariant **outArgs,
                                          GUnixFDList **outFDs G_GNUC_UNUSED,
                                          GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;
    g_autoptr(virStorageVol) storageVol = NULL;
    g_autofree gchar *path = NULL;
    const gchar *name;

    g_variant_get(inArgs, "(&s)", &name);

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    storageVol = virStorageVolLookupByName(storagePool, name);

    if (!storageVol)
        return virtDBusUtilSetLastVirtError(error);

    path = virtDBusUtilBusPathForVirStorageVol(storageVol,
                                               connect->storageVolPath);

    *outArgs = g_variant_new("(o)", path);
}

static void
virtDBusStoragePoolUndefine(GVariant *inArgs G_GNUC_UNUSED,
                            GUnixFDList *inFDs G_GNUC_UNUSED,
                            const gchar *objectPath,
                            gpointer userData,
                            GVariant **outArgs G_GNUC_UNUSED,
                            GUnixFDList **outFDs G_GNUC_UNUSED,
                            GError **error)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePool) storagePool = NULL;

    storagePool = virtDBusStoragePoolGetVirStoragePool(connect, objectPath,
                                                       error);
    if (!storagePool)
        return;

    if (virStoragePoolUndefine(storagePool) < 0)
        virtDBusUtilSetLastVirtError(error);
}

static virtDBusGDBusPropertyTable virtDBusStoragePoolPropertyTable[] = {
    { "Active", virtDBusStoragePoolGetActive, NULL },
    { "Autostart", virtDBusStoragePoolGetAutostart,
                   virtDBusStoragePoolSetAutostart },
    { "Name", virtDBusStoragePoolGetName, NULL },
    { "Persistent", virtDBusStoragePoolGetPersistent, NULL },
    { "UUID", virtDBusStoragePoolGetUUID, NULL },
    { 0 }
};

static virtDBusGDBusMethodTable virtDBusStoragePoolMethodTable[] = {
    { "Build", virtDBusStoragePoolBuild },
    { "Create", virtDBusStoragePoolCreate },
    { "Delete", virtDBusStoragePoolDelete },
    { "Destroy", virtDBusStoragePoolDestroy },
    { "GetInfo", virtDBusStoragePoolGetInfo },
    { "GetXMLDesc", virtDBusStoragePoolGetXMLDesc },
    { "ListStorageVolumes", virtDBusStoragePoolListStorageVolumes },
    { "Refresh", virtDBusStoragePoolRefresh },
    { "StorageVolCreateXML", virtDBusStoragePoolStorageVolCreateXML },
    { "StorageVolCreateXMLFrom", virtDBusStoragePoolStorageVolCreateXMLFrom },
    { "StorageVolLookupByName", virtDBusStoragePoolStorageVolLookupByName },
    { "Undefine", virtDBusStoragePoolUndefine },
    { 0 }
};

static gchar **
virtDBusStoragePoolEnumerate(gpointer userData)
{
    virtDBusConnect *connect = userData;
    g_autoptr(virStoragePoolPtr) storagePools = NULL;
    gint num = 0;
    gchar **ret = NULL;

    if (!virtDBusConnectOpen(connect, NULL))
        return NULL;

    num = virConnectListAllStoragePools(connect->connection, &storagePools, 0);
    if (num < 0)
        return NULL;

    if (num == 0)
        return NULL;

    ret = g_new0(gchar *, num + 1);

    for (gint i = 0; i < num; i++) {
        ret[i] = virtDBusUtilBusPathForVirStoragePool(storagePools[i],
                                                      connect->storagePoolPath);
    }

    return ret;
}

static GDBusInterfaceInfo *interfaceInfo;

void
virtDBusStoragePoolRegister(virtDBusConnect *connect,
                            GError **error)
{
    connect->storagePoolPath = g_strdup_printf("%s/storagepool",
                                               connect->connectPath);

    if (!interfaceInfo) {
        interfaceInfo = virtDBusGDBusLoadIntrospectData(VIRT_DBUS_STORAGEPOOL_INTERFACE,
                                                        error);
        if (!interfaceInfo)
            return;
    }

    virtDBusGDBusRegisterSubtree(connect->bus,
                                 connect->storagePoolPath,
                                 interfaceInfo,
                                 virtDBusStoragePoolEnumerate,
                                 virtDBusStoragePoolMethodTable,
                                 virtDBusStoragePoolPropertyTable,
                                 connect);
}
