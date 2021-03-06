#ifndef PYCOMPS_ENVS_H
#define PYCOMPS_ENVS_H

#include <Python.h>
#include "libcomps/comps.h"
#include "libcomps/comps_list.h"
#include "structmember.h"
#include "pycomps_sequence.h"
#include "pycomps_ids.h"
#include "pycomps_utils.h"
#include "pycomps_23macros.h"
#include "pycomps_dict.h"
#include "pycomps_ctopy_seq.h"
#include "pycomps_lang.h"

/*typedef struct {
    PyObject_HEAD
    COMPS_List *list;
} PyCOMPS_Envs;
*/

typedef struct {
    PyObject_HEAD
    PyCOMPS_CtoPy_PItem_HEAD
    //COMPS_DocEnv * env;

    /*PyCOMPS_CtoPy_CItem *group_list_citem;
    PyCOMPS_CtoPy_CItem *option_list_citem;
    PyCOMPS_CtoPy_CItem *name_by_lang_citem;
    PyCOMPS_CtoPy_CItem *desc_by_lang_citem;
    */
    PyObject *group_list_pobj;
    PyObject *option_list_pobj;
    PyObject *packages_pobj;
    PyObject *name_by_lang_pobj;
    PyObject *desc_by_lang_pobj;
} PyCOMPS_Env;

typedef struct COMPS_DocEnvExtra {
    PyCOMPS_CtoPy_CItem *citem;
    PyCOMPS_CtoPy_CItem *group_list_citem;
    PyCOMPS_CtoPy_CItem *option_list_citem;
    PyCOMPS_CtoPy_CItem *name_by_lang_citem;
    PyCOMPS_CtoPy_CItem *desc_by_lang_citem;
} COMPS_DocEnvExtra;
COMPS_DocEnvExtra * comps_docenv_extra_create();

COMPS_DocEnv* pycomps_env_get(PyObject *pyenv);
COMPS_DocEnv** pycomps_env_getp(PyObject *pyenv);
COMPS_DocEnvExtra* pycomps_env_get_extra(PyObject *pyenv);
COMPS_DocEnvExtra* comps_env_get_extra(void *docenv);
void pycomps_env_destroy(void * env);
void pycomps_env_decref(void * env);
void comps_env_incref(void * env);
PyObject* comps_env_str(void * env);

COMPS_List* comps_envs_union(COMPS_List *envs1, COMPS_List *envs2);
char comps_env_cmp(void *e1, void *e2);

PyObject* PyCOMPSEnv_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int PyCOMPSEnvs_init(PyCOMPS_CtoPySeq *self, PyObject *args, PyObject *kwds);

int PyCOMPSEnv_init(PyCOMPS_Env *self, PyObject *args, PyObject *kwds);
PyObject* PyCOMPSEnv_get_id(PyCOMPS_Env *self, void *closure);
PyObject* PyCOMPSEnv_get_name(PyCOMPS_Env *self, void *closure);
PyObject* PyCOMPSEnv_get_desc(PyCOMPS_Env *self, void *closure);
int PyCOMPSEnv_set_id(PyCOMPS_Env *self, PyObject *value, void *closure);
int PyCOMPSEnv_set_name(PyCOMPS_Env *self, PyObject *value, void *closure);
int PyCOMPSEnv_set_desc(PyCOMPS_Env *self, PyObject *value, void *closure);

PyObject* PyCOMPSEnv_get_name_by_lang(PyObject *self, void *closure);
int PyCOMPSEnv_set_name_by_lang(PyObject *self, PyObject *value, void *closure);
PyObject* PyCOMPSEnv_get_desc_by_lang(PyObject *self, void *closure);
int PyCOMPSEnv_set_desc_by_lang(PyObject *self, PyObject *value, void *closure);

extern PyTypeObject PyCOMPS_EnvType;
extern PyTypeObject PyCOMPS_EnvsType;

#endif
