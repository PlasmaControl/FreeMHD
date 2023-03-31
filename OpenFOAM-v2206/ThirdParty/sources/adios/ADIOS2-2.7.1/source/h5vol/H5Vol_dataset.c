#ifndef __H5VOL_DATASET_FUNC
#define __H5VOL_DATASET_FUNC

#include "H5Vol_def.h"

void *H5VL_adios2_dataset_create(void *obj, const H5VL_loc_params_t *loc_params,
                                 const char *name, hid_t lcpl_id, hid_t type_id,
                                 hid_t space_id, hid_t dcpl_id, hid_t dapl_id,
                                 hid_t dxpl_id, void **req)
{
    REQUIRE_NOT_NULL_ERR(loc_params, NULL);
    REQUIRE_NOT_NULL_ERR(obj, NULL);

    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)obj;

    if (H5I_FILE == loc_params->obj_type)
    {
        REQUIRE_SUCC((ROOT == vol->m_ObjType), NULL);
        H5VL_FileDef_t *handle = (H5VL_FileDef_t *)(vol->m_ObjPtr);
        H5VL_VarDef_t *varDef =
            gCreateVarDef(name, handle->m_Engine, NULL, space_id, type_id);
        gADIOS2DefineVar(vol->m_FileIO, varDef);
        return gVarToVolObj(varDef, vol);
    }
    else if (H5I_GROUP == loc_params->obj_type)
    {
        REQUIRE_SUCC((GROUP == vol->m_ObjType), NULL);
        size_t pathSize = strlen(vol->m_Path) + 1 + strlen(name);
        char fullPath[pathSize + 1];
        if (vol->m_Path[strlen(vol->m_Path) - 1] == '/')
        {
            pathSize = pathSize - 1;
            sprintf(fullPath, "%s%s", vol->m_Path, name);
        }
        else
            sprintf(fullPath, "%s/%s", vol->m_Path, name);

        fullPath[pathSize] = '\0';

        H5VL_ObjDef_t *curr = vol;
        while (curr)
        {
            if (curr->m_Parent == NULL)
            {
                H5VL_VarDef_t *varDef = gCreateVarDef(
                    fullPath, ((H5VL_FileDef_t *)(curr->m_ObjPtr))->m_Engine,
                    NULL, space_id, type_id);
                gADIOS2DefineVar(vol->m_FileIO, varDef);
                return gVarToVolObj(varDef, vol);
            }
            else
            {
                curr = (H5VL_ObjDef_t *)(curr->m_Parent);
            }
        }
    }
    return NULL;
}

void *H5VL_adios2_dataset_open(void *obj, const H5VL_loc_params_t *loc_params,
                               const char *name, hid_t dapl_id, hid_t dxpl_id,
                               void **req)
{
    REQUIRE_NOT_NULL_ERR(loc_params, NULL);
    REQUIRE_NOT_NULL_ERR(obj, NULL);

    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)obj;

    if (H5I_FILE == loc_params->obj_type)
    {
        REQUIRE_SUCC((ROOT == vol->m_ObjType), NULL);
        // return gGetVarObjDef(name, vol);
    }

    if (H5I_GROUP == loc_params->obj_type)
    {
        REQUIRE_SUCC((GROUP == vol->m_ObjType), NULL);
        // return gGetVarObjDef(name, vol);
    }

    H5VL_ObjDef_t *result = gGetVarObjDef(name, vol);
    if (NULL == result)
        SHOW_ERROR_MSG("H5VL_ADIOS2: Error: No such variable: %s in file.\n ",
                       name);
    return result;
}

herr_t H5VL_adios2_dataset_read(void *dset, hid_t mem_type_id,
                                hid_t mem_space_id, hid_t file_space_id,
                                hid_t plist_id, void *buf, void **req)
{
    REQUIRE_NOT_NULL_ERR(dset, -1);
    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)dset;

    H5VL_VarDef_t *var = (H5VL_VarDef_t *)(vol->m_ObjPtr);

    var->m_HyperSlabID = file_space_id;
    var->m_MemSpaceID = mem_space_id;
    var->m_Data = buf;
    return gADIOS2ReadVar(var);
}

herr_t H5VL_adios2_dataset_get(void *dset, H5VL_dataset_get_t get_type,
                               hid_t dxpl_id, void **req, va_list arguments)
{
    REQUIRE_NOT_NULL_ERR(dset, -1);
    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)dset;
    H5VL_VarDef_t *varDef = (H5VL_VarDef_t *)(vol->m_ObjPtr);

    switch (get_type)
    {
    case H5VL_DATASET_GET_SPACE:
    {
        hid_t *ret_id = va_arg(arguments, hid_t *);
        *ret_id = H5Scopy(varDef->m_ShapeID);
        REQUIRE_SUCC_MSG((*ret_id >= 0), -1,
                         "H5VOL-ADIOS2: Unable to get space id.");
        break;
    }
    case H5VL_DATASET_GET_TYPE:
    {
        hid_t *ret_id = va_arg(arguments, hid_t *);
        *ret_id = H5Tcopy(varDef->m_TypeID);
        break;
    }
    default:
    {
        return -1;
    }
    }
    return 0;
}

herr_t H5VL_adios2_dataset_write(void *dset, hid_t mem_type_id,
                                 hid_t mem_space_id, hid_t file_space_id,
                                 hid_t plist_id, const void *buf, void **req)
{
    REQUIRE_NOT_NULL_ERR(dset, -1);
    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)dset;
    H5VL_VarDef_t *varDef = (H5VL_VarDef_t *)(vol->m_ObjPtr);

    // H5VL_VarDef_t *varDef = (H5VL_VarDef_t *)dset;
    varDef->m_Data = (void *)buf;
    varDef->m_MemSpaceID = mem_space_id;
    varDef->m_HyperSlabID = file_space_id;
    varDef->m_PropertyID = plist_id;

    gADIOS2CreateVar(vol->m_FileIO, varDef);
    return 0;
}

herr_t H5VL_adios2_dataset_close(void *dset, hid_t dxpl_id, void **req)
{
    if (NULL == dset)
        return 0;

    H5VL_ObjDef_t *vol = (H5VL_ObjDef_t *)dset;
    REQUIRE_SUCC((VAR == vol->m_ObjType), -1);

    H5VL_VarDef_t *varDef = (H5VL_VarDef_t *)(vol->m_ObjPtr);

    // H5VL_VarDef_t *varDef = (H5VL_VarDef_t *)dset;
    SAFE_FREE(varDef->m_Name);
    H5Sclose(varDef->m_ShapeID);

    SAFE_FREE(varDef);

    gFreeVol(vol);

    return 0;
}

#endif
