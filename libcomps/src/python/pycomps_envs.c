#include "pycomps_23macros.h"
#include "pycomps_envs.h"

inline COMPS_DocEnvExtra * comps_docenv_extra_create() {
    COMPS_DocEnvExtra *ret;
    ret = malloc(sizeof(*ret));
    return ret;
}

char __compsenv_id_cmp(void *e1, void *e2) {
    return (strcmp(((COMPS_DocEnv*)e1)->id,
                  ((COMPS_DocEnv*)e2)->id) == 0);
}

inline COMPS_DocEnv* pycomps_env_get(PyObject *pyenv) {
    return (COMPS_DocEnv*)((PyCOMPS_Env*)pyenv)->citem->data;
}
inline COMPS_DocEnv** pycomps_env_getp(PyObject *pyenv) {
    return (COMPS_DocEnv**)&((PyCOMPS_Env*)pyenv)->citem->data;
}
inline COMPS_DocEnvExtra* pycomps_env_get_extra(PyObject *pyenv) {
    return (COMPS_DocEnvExtra*)
           ((COMPS_DocEnv*)((PyCOMPS_Env*)pyenv)->citem->data)
           ->reserved;
}
inline COMPS_DocEnvExtra* comps_env_get_extra(void *docenv) {
    return (COMPS_DocEnvExtra*)((COMPS_DocEnv*)(docenv))->reserved;
}

inline void comps_env_incref(void * env) {
    ctopy_citem_incref(comps_env_get_extra(env)->name_by_lang_citem);
    ctopy_citem_incref(comps_env_get_extra(env)->desc_by_lang_citem);
    ctopy_citem_incref(comps_env_get_extra(env)->group_list_citem);
    ctopy_citem_incref(comps_env_get_extra(env)->option_list_citem);
    ctopy_citem_incref(comps_env_get_extra(env)->citem);
}

inline void pycomps_env_incref(PyObject * pyenv) {
    comps_env_incref((void*)pycomps_env_get(pyenv));
}


inline void pycomps_env_decref(void * env) {
    ctopy_citem_decref(comps_env_get_extra(env)->name_by_lang_citem);
    ctopy_citem_decref(comps_env_get_extra(env)->desc_by_lang_citem);
    ctopy_citem_decref(comps_env_get_extra(env)->group_list_citem);
    ctopy_citem_decref(comps_env_get_extra(env)->option_list_citem);
    ctopy_citem_decref(comps_env_get_extra(env)->citem);
}

void pycomps_env_destroy(void * env) {
    free(((COMPS_DocEnv*)env)->id);
    free(((COMPS_DocEnv*)env)->name);
    free(((COMPS_DocEnv*)env)->desc);
    free(((COMPS_DocEnv*)env)->reserved);
    free(env);
}

PyObject* PyCOMPSEnv_convert(void *c) {
    PyObject *ret;
    ret = PyCOMPSEnv_new(&PyCOMPS_EnvType, NULL, NULL);
    PyCOMPSEnv_init((PyCOMPS_Env*)ret, NULL, NULL);

    ctopy_citem_decref(pycomps_env_get_extra(ret)->group_list_citem);
    ctopy_citem_decref(pycomps_env_get_extra(ret)->option_list_citem);
    ctopy_citem_decref(pycomps_env_get_extra(ret)->name_by_lang_citem);
    ctopy_citem_decref(pycomps_env_get_extra(ret)->desc_by_lang_citem);
    ctopy_citem_decref(((PyCOMPS_Env*)ret)->citem);
    

    ((PyCOMPS_Env*)ret)->citem = comps_env_get_extra(c)->citem;
    pycomps_env_get_extra(ret)->group_list_citem =
                                        comps_env_get_extra(c)->group_list_citem;
    pycomps_env_get_extra(ret)->option_list_citem =
                                        comps_env_get_extra(c)->option_list_citem;
    pycomps_env_get_extra(ret)->name_by_lang_citem =
                                     comps_env_get_extra(c)->name_by_lang_citem;
    pycomps_env_get_extra(ret)->desc_by_lang_citem =
                                     comps_env_get_extra(c)->desc_by_lang_citem;

    ctopy_citem_incref(((PyCOMPS_Env*)ret)->citem);
    ctopy_citem_incref(pycomps_env_get_extra(ret)->option_list_citem);
    ctopy_citem_incref(pycomps_env_get_extra(ret)->group_list_citem);
    ctopy_citem_incref(pycomps_env_get_extra(ret)->name_by_lang_citem);
    ctopy_citem_incref(pycomps_env_get_extra(ret)->desc_by_lang_citem);
    
    return ret;
}

PyObject* PyCOMPSEnv_union(PyObject *self, PyObject *other) {
    COMPS_DocEnv *e;
    PyObject *res;

    if (Py_TYPE(other) != &PyCOMPS_EnvType) {
        PyErr_SetString(PyExc_TypeError, "Not Environment instance");
        return NULL;
    }
    if (pycomps_env_get(self)->group_list == NULL) {
        pycomps_env_get(self)->group_list = comps_list_create();
        comps_list_init(pycomps_env_get(self)->group_list);
        pycomps_env_get_extra(self)->group_list_citem->data =
                                              pycomps_env_get(self)->group_list;
    }
    if (pycomps_env_get(self)->option_list == NULL) {
        pycomps_env_get(self)->option_list = comps_list_create();
        comps_list_init(pycomps_env_get(self)->option_list);
        pycomps_env_get_extra(self)->option_list_citem->data =
                                             pycomps_env_get(self)->option_list;
    }

    if (pycomps_env_get(other)->group_list == NULL) {
        pycomps_env_get(other)->group_list = comps_list_create();
        comps_list_init(pycomps_env_get(other)->group_list);
        pycomps_env_get_extra(other)->group_list_citem->data =
                                             pycomps_env_get(other)->group_list;
    }
    if (pycomps_env_get(other)->option_list == NULL) {
        pycomps_env_get(other)->option_list = comps_list_create();
        comps_list_init(pycomps_env_get(other)->option_list);
        pycomps_env_get_extra(other)->option_list_citem->data =
                                             pycomps_env_get(other)->option_list;
    }
    e = comps_docenv_union(pycomps_env_get(self), pycomps_env_get(other));

    res = PyCOMPSEnv_new(&PyCOMPS_EnvType, NULL, NULL);
    PyCOMPSEnv_init((PyCOMPS_Env*)res, NULL, NULL);
    e->reserved = pycomps_env_get_extra(res);
    comps_docenv_destroy(pycomps_env_get(res));
    ((PyCOMPS_Env*)res)->citem->data = e;
    pycomps_env_get_extra(res)->option_list_citem->data = e->option_list;
    pycomps_env_get_extra(res)->group_list_citem->data = e->group_list;
    pycomps_env_get_extra(res)->name_by_lang_citem->data = e->name_by_lang;
    pycomps_env_get_extra(res)->desc_by_lang_citem->data = e->desc_by_lang;
    return res;
}


void PyCOMPSEnv_dealloc(PyObject *self)
{
    ctopy_citem_destroy(pycomps_env_get_extra(self)->name_by_lang_citem);
    ctopy_citem_destroy(pycomps_env_get_extra(self)->desc_by_lang_citem);
    ctopy_citem_destroy(pycomps_env_get_extra(self)->group_list_citem);
    ctopy_citem_destroy(pycomps_env_get_extra(self)->option_list_citem);
    ctopy_citem_destroy(((PyCOMPS_Env*)self)->citem);
    if (((PyCOMPS_Env*)self)->group_list_pobj)
        Py_XDECREF(((PyCOMPS_Env*)self)->group_list_pobj);
    if (((PyCOMPS_Env*)self)->option_list_pobj)
        Py_XDECREF(((PyCOMPS_Env*)self)->option_list_pobj);
    if (((PyCOMPS_Env*)self)->name_by_lang_pobj)
        Py_XDECREF(((PyCOMPS_Env*)self)->name_by_lang_pobj);
    if (((PyCOMPS_Env*)self)->desc_by_lang_pobj)
        Py_XDECREF(((PyCOMPS_Env*)self)->desc_by_lang_pobj);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* PyCOMPSEnv_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyCOMPS_Env *self;
    COMPS_DocEnv *e;

    (void)args;
    (void)kwds;

    self = (PyCOMPS_Env*) type->tp_alloc(type, 0);
    if (self != NULL) {
        e = comps_docenv_create();
        e->reserved = (void*)comps_docenv_extra_create();
        ((COMPS_DocEnvExtra*)(e->reserved))->citem =
                                  ctopy_citem_create(e, &pycomps_env_destroy);
        self->citem = ((COMPS_DocEnvExtra*)e->reserved)->citem;
        ((COMPS_DocEnvExtra*)(e->reserved))->group_list_citem =
                                    ctopy_citem_create(e->group_list,
                                                       &comps_list_destroy_v);
        self->group_list_pobj = NULL;
        ((COMPS_DocEnvExtra*)(e->reserved))->option_list_citem =
                                    ctopy_citem_create(e->option_list,
                                                       &comps_list_destroy_v);
        self->option_list_pobj = NULL;
        ((COMPS_DocEnvExtra*)(e->reserved))->name_by_lang_citem =
                                    ctopy_citem_create(e->name_by_lang,
                                                       &comps_dict_destroy_v);
        self->name_by_lang_pobj = NULL;
        ((COMPS_DocEnvExtra*)(e->reserved))->desc_by_lang_citem =
                                    ctopy_citem_create(e->desc_by_lang,
                                                       &comps_dict_destroy_v);
        self->desc_by_lang_pobj = NULL;
    }
    return (PyObject*) self;
}

int PyCOMPSEnv_init(PyCOMPS_Env *self, PyObject *args, PyObject *kwds)
{
    char *name, *id, *desc;
    unsigned disp_ord;

    (void)kwds;

    name = NULL;
    id = NULL;
    desc = NULL;
    disp_ord = 0;
    if (args && PyArg_ParseTuple(args, "|sssi", &id, &name, &desc, &disp_ord)) {
        comps_docenv_set_id(pycomps_env_get((PyObject*)self), id, 1);
        comps_docenv_set_name(pycomps_env_get((PyObject*)self), name, 1);
        comps_docenv_set_desc(pycomps_env_get((PyObject*)self), desc, 1);
        comps_docenv_set_displayorder(pycomps_env_get((PyObject*)self), disp_ord);
        return 0;
    } else {
        return -1;
    }
}

void pycomps_env_print(FILE *f, void *e) {
    COMPS_ListItem *it;
    COMPS_HSList *pairlist;
    COMPS_HSListItem *hsit;

    fprintf(f, "<COMPS_Environment: id='%s'"
               "name='%s' description=%s ",
                ((COMPS_DocEnv*)e)->id,
                ((COMPS_DocEnv*)e)->name,
                ((COMPS_DocEnv*)e)->desc);

    fprintf(f, ", name_by_lang={");
    pairlist = comps_rtree_pairs(((COMPS_DocEnv*)e)->name_by_lang);
    for (hsit = pairlist->first; hsit != pairlist->last; hsit = hsit->next) {
        printf("'%s': '%s', ", ((COMPS_RTreePair*)hsit->data)->key,
                               (char*)((COMPS_RTreePair*)hsit->data)->data);
    }
    printf("'%s': '%s'}", ((COMPS_RTreePair*)hsit->data)->key,
                           (char*)((COMPS_RTreePair*)hsit->data)->data);
    comps_hslist_destroy(&pairlist);

    fprintf(f, ", desc_by_lang={");
    pairlist = comps_rtree_pairs(((COMPS_DocEnv*)e)->desc_by_lang);
    for (hsit = pairlist->first; hsit != pairlist->last; hsit = hsit->next) {
        printf("'%s': '%s', ", ((COMPS_RTreePair*)hsit->data)->key,
                               (char*)((COMPS_RTreePair*)hsit->data)->data);
    }
    printf("'%s': '%s'}", ((COMPS_RTreePair*)hsit->data)->key,
                           (char*)((COMPS_RTreePair*)hsit->data)->data);
    comps_hslist_destroy(&pairlist);

    fprintf(f, ", group_list=[");
    if (((COMPS_DocEnv*)e)->group_list) {
        for (it = ((COMPS_DocEnv*)e)->group_list->first;
             it != ((COMPS_DocEnv*)e)->group_list->last; it = it->next){
            fprintf(f, "'%s', ", (char*)it->data);
        }
        if (it)
            fprintf(f, "'%s'", (char*)it->data);
    }
    fprintf(f, "]");
    fprintf(f, ", ");
    fprintf(f, ", option_list=[");
    if (((COMPS_DocEnv*)e)->option_list) {
        for (it = ((COMPS_DocEnv*)e)->option_list->first;
             it != ((COMPS_DocEnv*)e)->option_list->last; it = it->next){
            fprintf(f, "'%s', ", (char*)it->data);
        }
        if (it)
            fprintf(f, "'%s'", (char*)it->data);
    }
    fprintf(f, "]>");
}

int PyCOMPSEnv_print(PyObject *self, FILE *f, int flags) {
    (void) flags;
    pycomps_env_print(f, pycomps_env_get(self));
    return 0;
}

PyObject* comps_env_str(void * env) {
    PyObject *ret, *tmp, *tmp2, *emptytmp;
    const char *id, *name, *desc;
    char *empty;
    COMPS_ListItem *it;

    emptytmp = Py_TYPE(Py_None)->tp_str(Py_None);
    if (PyUnicode_Check(emptytmp)) {
        __pycomps_PyUnicode_AsString(emptytmp, &empty);
    } else {
        empty = PyBytes_AsString(emptytmp);
    }

    id = (((COMPS_DocEnv*)env)->id)?((COMPS_DocEnv*)env)->id:empty;
    name = (((COMPS_DocEnv*)env)->name)?((COMPS_DocEnv*)env)->name:empty;
    desc = (((COMPS_DocEnv*)env)->desc)?((COMPS_DocEnv*)env)->desc:empty;

    ret = PyUnicode_FromFormat("<COMPS_Env: id='%s', name='%s', description='%s',"
                              "name_by_lang=",
                              /*, description_by_lang=%U, %U>",*/
                              id, name, desc);
    if (PyUnicode_Check(emptytmp)) {
        free(empty);
    }
    Py_XDECREF(emptytmp);

    tmp2 = comps_lang_str(((COMPS_DocEnv*)env)->name_by_lang);
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;

    tmp2 = PyUnicode_FromString(", desc_by_lang=");
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;

    tmp2 = comps_lang_str(((COMPS_DocEnv*)env)->desc_by_lang);
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;

    tmp2 = PyUnicode_FromString(", group_list=[");
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(ret);
    Py_DECREF(tmp2);
    ret = tmp;
    it = (((COMPS_DocEnv*)env)->group_list)?
         ((COMPS_DocEnv*)env)->group_list->first:NULL;
    for (; it != NULL && it != ((COMPS_DocEnv*)env)->group_list->last;
         it = it->next){
        tmp2 =PyUnicode_FromFormat("'%s', ", (char*)it->data);
        tmp = PyUnicode_Concat(ret,tmp2);
        Py_DECREF(ret);
        Py_DECREF(tmp2);
        ret = tmp;
    }
    if (it) {
        tmp2 =PyUnicode_FromFormat("'%s', ", (char*)it->data);
    } else {
        tmp2 = PyUnicode_FromString("]");
    }
    tmp = PyUnicode_Concat(ret,tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;

    tmp2 = PyUnicode_FromString(", option_list=[");
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(ret);
    Py_DECREF(tmp2);
    ret = tmp;
    it = (((COMPS_DocEnv*)env)->option_list)?
         ((COMPS_DocEnv*)env)->option_list->first:NULL;
    for (; it != NULL && it != ((COMPS_DocEnv*)env)->option_list->last;
         it = it->next){
        tmp2 =PyUnicode_FromFormat("'%s', ", (char*)it->data);
        tmp = PyUnicode_Concat(ret,tmp2);
        Py_DECREF(ret);
        Py_DECREF(tmp2);
        ret = tmp;
    }
    if (it) {
        tmp2 =PyUnicode_FromFormat("'%s', ", (char*)it->data);
    } else {
        tmp2 = PyUnicode_FromString("]");
    }
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;

    tmp2 = PyUnicode_FromString(">");
    tmp = PyUnicode_Concat(ret, tmp2);
    Py_DECREF(tmp2);
    Py_DECREF(ret);
    ret = tmp;
    return ret;
}

PyObject* PyCOMPSEnv_str(PyObject *self) {
    return comps_env_str(pycomps_env_get(self));
}

PyObject* PyCOMPSEnv_cmp(PyObject *self, PyObject *other, int op) {
    char ret;
    CMP_OP_EQ_NE_CHECK(op)

    if (other == NULL || ( Py_TYPE(other) != &PyCOMPS_EnvType
                           && other != Py_None)) {
        PyErr_SetString(PyExc_TypeError, "Not Environment instance");
        return NULL;
    }
    CMP_NONE_CHECK(op, self, other)

    ret = comps_env_cmp((void*)pycomps_env_get(self),
                          (void*)pycomps_env_get(other));
    if (op == Py_EQ) {
        if (!ret) Py_RETURN_FALSE;
    } else {
        if (ret) Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

char comps_env_cmp(void *e1, void *e2) {
    COMPS_ListItem *it;
    COMPS_Set *set;

    if (__pycomps_strcmp(((COMPS_DocEnv*)e1)->id,
                         ((COMPS_DocEnv*)e2)->id) != 0) {
        return 0;
    }
    if (__pycomps_strcmp(((COMPS_DocEnv*)e1)->name,
                         ((COMPS_DocEnv*)e2)->name) != 0) {
        return 0;
    }
    if (__pycomps_strcmp(((COMPS_DocEnv*)e1)->desc,
                         ((COMPS_DocEnv*)e2)->desc) != 0) {
        return 0;
    }
    set = comps_set_create();
    comps_set_init(set, NULL, NULL, NULL, &__pycomps_strcmp_v);

    it = (((COMPS_DocEnv*)e1)->group_list)
         ?((COMPS_DocEnv*)e1)->group_list->first: NULL;
    for (; it!= NULL; it = it->next) {
        comps_set_add(set, it->data);
    }
    it = (((COMPS_DocEnv*)e2)->group_list)
          ?((COMPS_DocEnv*)e2)->group_list->first: NULL;
    for (; it!= NULL; it = it->next) {
        if (!comps_set_in(set, it->data)) {
                comps_set_destroy(&set);
                return 0;
        }
    }
    comps_set_clear(set);

    it = (((COMPS_DocEnv*)e1)->option_list)
         ?((COMPS_DocEnv*)e1)->option_list->first: NULL;
    for (; it!= NULL; it = it->next) {
        comps_set_add(set, it->data);
    }
    it = (((COMPS_DocEnv*)e2)->option_list)
          ?((COMPS_DocEnv*)e2)->option_list->first: NULL;
    for (; it!= NULL; it = it->next) {
        if (!comps_set_in(set, it->data)) {
            comps_set_destroy(&set);
            return 0;
        }
    }
    comps_set_destroy(&set);

    return 1;
}

PyObject* PyCOMPSEnv_get_groupids(PyCOMPS_Env *self, void *closure) {
    (void) closure;
    PyObject *ret;
    if (!self->group_list_pobj) {
        ret = PyCOMPSSeq_new(&PyCOMPS_IDsType, NULL, NULL);
        PyCOMPSIDs_init((PyCOMPS_Sequence*)ret, NULL, NULL);
        if (pycomps_env_get((PyObject*)self)->group_list == NULL) {
            pycomps_env_get((PyObject*)self)->group_list = comps_list_create();
            comps_list_init(pycomps_env_get((PyObject*)self)->group_list);
            pycomps_env_get_extra((PyObject*)self)->group_list_citem->data =
                                   pycomps_env_get((PyObject*)self)->group_list;
        }
        ctopy_citem_destroy(((PyCOMPS_Sequence*)ret)->citem);
        ((PyCOMPS_Sequence*)ret)->citem =
                       pycomps_env_get_extra((PyObject*)self)->group_list_citem;
        self->group_list_pobj = ret;
        ctopy_citem_incref(((PyCOMPS_Sequence*)ret)->citem);
        Py_INCREF(ret);
    } else {
        ret = self->group_list_pobj;
        Py_INCREF(ret);
    }
    return  ret;
}

int PyCOMPSEnv_set_groupids(PyCOMPS_Env *self,
                                  PyObject *value, void *closure) {
    (void) closure;
    if (value->ob_type != &PyCOMPS_IDsType) {
        PyErr_SetString(PyExc_TypeError, "Not GroupIds instance");
        return -1;
    }
    ctopy_citem_decref(pycomps_env_get_extra((PyObject*)self)->group_list_citem);
    pycomps_env_get_extra((PyObject*)self)->group_list_citem =
                                              ((PyCOMPS_Sequence*)value)->citem;
    ctopy_citem_incref(pycomps_env_get_extra((PyObject*)self)->group_list_citem);
    pycomps_env_get((PyObject*)self)->group_list =
                    (COMPS_List*)((PyCOMPS_Sequence*)value)->citem->data;
    if (self->group_list_pobj) {
        Py_XDECREF(self->group_list_pobj);
    }
    self->group_list_pobj = value;
    Py_INCREF(value);
    return 0;
}

PyObject* PyCOMPSEnv_get_optionids(PyCOMPS_Env *self, void *closure) {
    (void) closure;
    PyObject *ret;
    if (!self->option_list_pobj) {
        ret = PyCOMPSSeq_new(&PyCOMPS_IDsType, NULL, NULL);
        PyCOMPSIDs_init((PyCOMPS_Sequence*)ret, NULL, NULL);
        if (pycomps_env_get((PyObject*)self)->option_list == NULL) {
            pycomps_env_get((PyObject*)self)->option_list= comps_list_create();
            comps_list_init(pycomps_env_get((PyObject*)self)->option_list);
            pycomps_env_get_extra((PyObject*)self)->option_list_citem->data =
                                   pycomps_env_get((PyObject*)self)->option_list;
        }
        ctopy_citem_destroy(((PyCOMPS_Sequence*)ret)->citem);
        ((PyCOMPS_Sequence*)ret)->citem =
                       pycomps_env_get_extra((PyObject*)self)->option_list_citem;
        self->option_list_pobj = ret;
        ctopy_citem_incref(((PyCOMPS_Sequence*)ret)->citem);
        Py_INCREF(ret);
    } else {
        ret = self->option_list_pobj;
        Py_INCREF(ret);
    }
    return  ret;
}

int PyCOMPSEnv_set_optionids(PyCOMPS_Env *self,
                                  PyObject *value, void *closure) {
    (void) closure;
    if (value->ob_type != &PyCOMPS_IDsType) {
        PyErr_SetString(PyExc_TypeError, "Not GroupIds instance");
        return -1;
    }
    ctopy_citem_decref(pycomps_env_get_extra((PyObject*)self)->option_list_citem);
    pycomps_env_get_extra((PyObject*)self)->option_list_citem =
                                              ((PyCOMPS_Sequence*)value)->citem;
    ctopy_citem_incref(pycomps_env_get_extra((PyObject*)self)->option_list_citem);
    pycomps_env_get((PyObject*)self)->option_list =
                    (COMPS_List*)((PyCOMPS_Sequence*)value)->citem->data;
    if (self->option_list_pobj) {
        Py_XDECREF(self->option_list_pobj);
    }
    self->option_list_pobj = value;
    Py_INCREF(value);
    return 0;
}

inline PyObject* PyCOMPSEnv_get_name_by_lang(PyObject *self, void *closure) {
    (void) closure;
    return pycomps_lang_get_dict(pycomps_env_get_extra(self)->name_by_lang_citem,
                                 &((PyCOMPS_Env*)self)->name_by_lang_pobj);
}

inline int PyCOMPSEnv_set_name_by_lang(PyObject *self,
                                       PyObject *value, void *closure) {
    (void) closure;
    return pycomps_lang_set_dict(&pycomps_env_get_extra(self)->name_by_lang_citem,
                                 &((PyCOMPS_Env*)self)->name_by_lang_pobj,
                                 value,
                       (void**)&pycomps_env_get(self)->name_by_lang);
}

inline PyObject* PyCOMPSEnv_get_desc_by_lang(PyObject *self, void *closure) {
    (void) closure;
    return pycomps_lang_get_dict(pycomps_env_get_extra(self)->desc_by_lang_citem,
                                 &((PyCOMPS_Env*)self)->desc_by_lang_pobj);
}

inline int PyCOMPSEnv_set_desc_by_lang(PyObject *self, PyObject *value,
                                                          void *closure) {
    (void) closure;
    return pycomps_lang_set_dict(&pycomps_env_get_extra(self)->desc_by_lang_citem,
                                 &((PyCOMPS_Env*)self)->desc_by_lang_pobj,
                                 value,
                       (void**)&pycomps_env_get(self)->desc_by_lang);
}

int pycomps_env_strattr_setter(PyObject *self, PyObject *val, void *closure) {
    char *tmp;
    if (__pycomps_stringable_to_char(val, &tmp) < 0) {
        return -1;
    }
    __comps_doc_char_setter((void**)&GET_FROM(pycomps_env_get(self), (size_t)closure),
                                      tmp, 0);
    return 0;
}

PyObject* pycomps_env_strattr_getter(PyObject *self, void *closure) {

    char *tmp;
    tmp = GET_FROM(pycomps_env_get(self), (size_t)closure);
    if (tmp)
        return PyUnicode_FromString(tmp);
    else
        Py_RETURN_NONE;
}

PyMemberDef PyCOMPSEnv_members[] = {
    {NULL}};

PyMethodDef PyCOMPSEnv_methods[] = {
    {NULL}  /* Sentinel */
};

PyGetSetDef PyCOMPSEnv_getset[] = {
    {"id",
     (getter)pycomps_env_strattr_getter, (setter)pycomps_env_strattr_setter,
     "Env id",
     (void*)offsetof(COMPS_DocEnv, id)},
    {"name",
     (getter)pycomps_env_strattr_getter, (setter)pycomps_env_strattr_setter,
     "Env name",
     (void*)offsetof(COMPS_DocEnv, name)},
    {"desc",
     (getter)pycomps_env_strattr_getter, (setter)pycomps_env_strattr_setter,
     "Env description",
     (void*)offsetof(COMPS_DocEnv, desc)},
    {"group_ids",
     (getter)PyCOMPSEnv_get_groupids, (setter)PyCOMPSEnv_set_groupids,
     "Env group ids",
     NULL},
    {"option_ids",
     (getter)PyCOMPSEnv_get_optionids, (setter)PyCOMPSEnv_set_optionids,
     "Env group ids",
     NULL},
    {"name_by_lang",
    (getter)PyCOMPSEnv_get_name_by_lang, (setter)PyCOMPSEnv_set_name_by_lang,
     "Env name locales", NULL},
    {"desc_by_lang",
    (getter)PyCOMPSEnv_get_desc_by_lang, (setter)PyCOMPSEnv_set_desc_by_lang,
     "Env description locales", NULL},
    {NULL}  /* Sentinel */
};

PyNumberMethods PyCOMPSEnv_Nums = {
    .nb_add = PyCOMPSEnv_union
};

PyCOMPS_CtoPySeqItemMan PyCOMPSEnv_ItemMan = {
    .item_type = &PyCOMPS_EnvType,
    .ctopy_convert = &PyCOMPSEnv_convert,
    .data_decref = &pycomps_env_decref,
    .data_incref = &comps_env_incref,
    .data_cmp = &comps_env_cmp,
    .fprint_f = &pycomps_env_print,
    .str_f = &comps_env_str
};
PyTypeObject PyCOMPS_EnvType = {
    PY_OBJ_HEAD_INIT
    "libcomps.Env",   /*tp_name*/
    sizeof(PyCOMPS_Env), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyCOMPSEnv_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,//PyCOMPSEnv_cmp,            /*tp_compare*/
    0,                         /*tp_repr*/
    &PyCOMPSEnv_Nums,          /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    PyCOMPS_hash,              /*tp_hash */
    0,                         /*tp_call*/
    &PyCOMPSEnv_str,            /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,        /*tp_flags*/
    "Comps Env",          /* tp_doc */
    0,                    /* tp_traverse */
    0,                     /* tp_clear */
    &PyCOMPSEnv_cmp,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    PyCOMPSEnv_methods,        /* tp_methods */
    PyCOMPSEnv_members,        /* tp_members */
    PyCOMPSEnv_getset,         /* tp_getset */
    0,//&PyCOMPS_SeqItemType,        /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyCOMPSEnv_init,      /* tp_init */
    0,                               /* tp_alloc */
    PyCOMPSEnv_new,                 /* tp_new */};

int PyCOMPSEnvs_init(PyCOMPS_CtoPySeq *self, PyObject *args, PyObject *kwds)
{
    (void) args;
    (void) kwds;
    self->item_man = &PyCOMPSEnv_ItemMan;
    return 0;
}

inline PyObject* PyCOMPSEnvs_append(PyObject * self, PyObject *item) {
    if (item == NULL || Py_TYPE(item) != &PyCOMPS_EnvType) {
        PyErr_SetString(PyExc_TypeError, "Not Env instance");
        return NULL;
    }
    PyCOMPSSeq_append(self, item);
    Py_RETURN_NONE;
}
COMPS_List* comps_envs_union(COMPS_List *envs1, COMPS_List *envs2) {
    COMPS_ListItem *it, *newit;
    COMPS_HSListItem *hsit;
    COMPS_Set *set;
    COMPS_List *res;
    COMPS_DocEnv *tmpenv, *tmpenv2, *unienv;

    res = comps_list_create();
    comps_list_init(res);

    set = comps_set_create();
    comps_set_init(set, NULL, NULL, NULL, &__compsenv_id_cmp);

    it = (envs1)?envs1->first:NULL;
    for (; it != NULL; it = it->next) {
        if (((COMPS_DocEnv*)it->data)->group_list == NULL) {
            ((COMPS_DocEnv*)it->data)->group_list = comps_list_create();
            comps_list_init(((COMPS_DocEnv*)it->data)->group_list);
            comps_env_get_extra(it->data)->group_list_citem->data =
                                      ((COMPS_DocEnv*)it->data)->group_list;
        }
        if (((COMPS_DocEnv*)it->data)->option_list == NULL) {
            ((COMPS_DocEnv*)it->data)->option_list = comps_list_create();
            comps_list_init(((COMPS_DocEnv*)it->data)->option_list);
            comps_env_get_extra(it->data)->option_list_citem->data =
                                      ((COMPS_DocEnv*)it->data)->option_list;
        }
        comps_env_incref(it->data);
        comps_set_add(set, it->data);
    }
    it = (envs2)?envs2->first:NULL;
    for (; it != NULL; it = it->next) {
        tmpenv = it->data;
        if (tmpenv->group_list == NULL) {
            tmpenv->group_list = comps_list_create();
            comps_list_init(tmpenv->group_list);
            comps_env_get_extra(tmpenv)->group_list_citem->data =
                                                         tmpenv->group_list;
        }
        if (tmpenv->option_list == NULL) {
            tmpenv->option_list = comps_list_create();
            comps_list_init(tmpenv->option_list);
            comps_env_get_extra(tmpenv)->option_list_citem->data =
                                                         tmpenv->option_list;
        }
        if (comps_set_in(set, tmpenv)) {
            tmpenv2 = (COMPS_DocEnv*)comps_set_data_at(set, tmpenv);
            unienv = comps_docenv_union(tmpenv, tmpenv2);
            unienv->reserved = comps_docenv_extra_create();
            ((COMPS_DocEnvExtra*)(unienv->reserved))->citem =
                               ctopy_citem_create(unienv, &pycomps_env_destroy);
            ((COMPS_DocEnvExtra*)(unienv->reserved))->group_list_citem =
                                        ctopy_citem_create(unienv->group_list,
                                                           &comps_list_destroy_v);
            ((COMPS_DocEnvExtra*)(unienv->reserved))->option_list_citem =
                                        ctopy_citem_create(unienv->option_list,
                                                           &comps_list_destroy_v);
            ((COMPS_DocEnvExtra*)(unienv->reserved))->name_by_lang_citem =
                                        ctopy_citem_create(unienv->name_by_lang,
                                                           &comps_dict_destroy_v);
            ((COMPS_DocEnvExtra*)(unienv->reserved))->desc_by_lang_citem =
                                        ctopy_citem_create(unienv->desc_by_lang,
                                                           &comps_dict_destroy_v);
            pycomps_env_decref(tmpenv2);
            comps_set_remove(set, tmpenv2);
            comps_set_add(set, unienv);
        } else {
            comps_env_incref((void*)tmpenv);
            comps_set_add(set, tmpenv);
        }
    }
    for (hsit = set->data->first; hsit != NULL; hsit = hsit->next) {
        newit = comps_list_item_create(hsit->data, NULL, &pycomps_env_decref);
        comps_list_append(res, newit);
    }
    comps_set_destroy(&set);
    return res;


}
PyObject* PyCOMPSEnvs_union(PyObject *self, PyObject *other) {
    PyCOMPS_CtoPySeq *res;
    COMPS_List *res_list;

    if (other == NULL || Py_TYPE(other) != &PyCOMPS_EnvsType) {
        PyErr_SetString(PyExc_TypeError, "Not EnvironmentList instance");
        return NULL;
    }

    res = (PyCOMPS_CtoPySeq*) Py_TYPE(self)->tp_new(Py_TYPE(self), NULL, NULL);
    PyCOMPSEnvs_init(res, NULL, NULL);
    res_list = comps_envs_union(((PyCOMPS_CtoPySeq*)self)->citem->data,
                               ((PyCOMPS_CtoPySeq*)other)->citem->data);
    comps_list_destroy((COMPS_List**)&res->citem->data);
    res->citem->data = res_list;

    return (PyObject*)res;
}

PyMemberDef PyCOMPSEnvs_members[] = {
    {NULL}};

PyMethodDef PyCOMPSEnvs_methods[] = {
    //{"append", (PyCFunction)PyCOMPSEnvs_append, METH_O,
    //"Append item to new of the list"},
    {NULL}  /* Sentinel */
};

PyNumberMethods PyCOMPSEnvs_Nums = {
    .nb_add = PyCOMPSEnvs_union
};


PyTypeObject PyCOMPS_EnvsType = {
    PY_OBJ_HEAD_INIT
    "libcomps.EnvList",   /*tp_name*/
    sizeof(PyCOMPS_Sequence), /*tp_basicsize*/
    sizeof(PyCOMPS_Env),   /*tp_itemsize*/
    (destructor)PyCOMPSCtoPySeq_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0, //&PyCOMPSEnvs_cmp,          /*tp_compare*/
    0,                         /*tp_repr*/
    &PyCOMPSEnvs_Nums,         /*tp_as_number*/
    0,//&PyCOMPSEnv_sequence,       /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    PyCOMPSCtoPySeq_str,           /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_TYPE_SUBCLASS,        /*tp_flags*/
    "Comps Env list",           /* tp_doc */
    0,                    /* tp_traverse */
    0,                     /* tp_clear */
    &PyCOMPSCtoPySeq_cmp,       /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    PyCOMPSEnvs_methods,        /* tp_methods */
    PyCOMPSEnvs_members,        /* tp_members */
    0,                         /* tp_getset */
    &PyCOMPS_CtoPySeqType,           /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyCOMPSEnvs_init,      /* tp_init */
    0,                               /* tp_alloc */
    PyCOMPSCtoPySeq_new,                 /* tp_new */};

