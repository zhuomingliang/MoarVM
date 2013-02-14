#include "moarvm.h"

/* This representation's function pointer table. */
static MVMREPROps *this_repr;

/* Creates a new type object of this representation, and associates it with
 * the given HOW. */
static MVMObject * type_object_for(MVMThreadContext *tc, MVMObject *HOW) {
    MVMSTable *st;
    MVMObject *obj;
    
    st = MVM_gc_allocate_stable(tc, this_repr, HOW);
    MVM_gc_root_temp_push(tc, (MVMCollectable **)&st);
    
    obj = MVM_gc_allocate_type_object(tc, st);
    st->WHAT = obj;
    st->size = sizeof(MVMIter);
    
    MVM_gc_root_temp_pop(tc);
    
    return st->WHAT;
}

/* Creates a new instance based on the type object. */
static MVMObject * allocate(MVMThreadContext *tc, MVMSTable *st) {
    return MVM_gc_allocate_object(tc, st);
}

/* Initialize a new instance. */
static void initialize(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data) {
}

/* Copies the body of one object to another. */
static void copy_to(MVMThreadContext *tc, MVMSTable *st, void *src, MVMObject *dest_root, void *dest) {
    MVMIterBody *src_body  = (MVMIterBody *)src;
    MVMIterBody *dest_body = (MVMIterBody *)dest;
}

/* Adds held objects to the GC worklist. */
static void gc_mark(MVMThreadContext *tc, MVMSTable *st, void *data, MVMGCWorklist *worklist) {
    MVMIterBody  *body  = (MVMIterBody *)data;
}

/* Called by the VM in order to free memory associated with this object. */
static void gc_free(MVMThreadContext *tc, MVMObject *obj) {
    MVMIter *iter = (MVMIter *)obj;
}

/* Gets the storage specification for this representation. */
static MVMStorageSpec get_storage_spec(MVMThreadContext *tc, MVMSTable *st) {
    MVMStorageSpec spec;
    spec.inlineable      = MVM_STORAGE_SPEC_REFERENCE;
    spec.boxed_primitive = MVM_STORAGE_SPEC_BP_NONE;
    spec.can_box         = 0;
    return spec;
}

static void at_pos(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMint64 index, MVMRegister *value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void bind_pos(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMint64 index, MVMRegister value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static MVMint64 elems(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void set_elems(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMint64 count) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void push(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void pop(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister *value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void unshift(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVM_exception_throw_adhoc(tc, "Invalid operation on iterator");
}

static void shift(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister *value, MVMuint16 kind) {
    MVMIterBody *body = (MVMIterBody *)data;
    MVMObject *target = body->target;
    switch (body->mode) {
        case MVM_ITER_MODE_ARRAY:
            body->array_state.index++;
            if (body->array_state.index >= body->array_state.limit)
                MVM_exception_throw_adhoc(tc, "Iteration past end of iterator");
            REPR(target)->pos_funcs->at_pos(tc, STABLE(target), target, OBJECT_BODY(target), body->array_state.index, value, 0);
            return;
        case MVM_ITER_MODE_HASH:
            if (!body->hash_state.curr) {
                if (body->hash_state.next) {
                    body->hash_state.curr = body->hash_state.next;
                    body->hash_state.next = body->hash_state.next->hash_handle.next;
                }
                else {
                    MVM_exception_throw_adhoc(tc, "Iteration past end of iterator");
                }
            }
            else {
                body->hash_state.curr = body->hash_state.curr->hash_handle.next;
                body->hash_state.next = body->hash_state.curr->hash_handle.next;
            }
            value->o = root;
            return;
        default:
            MVM_exception_throw_adhoc(tc, "Unknown iteration mode");
    }
}

/* This whole splice optimization can be optimized for the case we have two
 * MVMIter representation objects. */
static void splice(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *from, MVMint64 offset, MVMint64 count) {
    MVMIterBody *body = (MVMIterBody *)data;
}

static MVMStorageSpec get_elem_storage_spec(MVMThreadContext *tc, MVMSTable *st) {
    MVMStorageSpec spec;
    spec.inlineable      = MVM_STORAGE_SPEC_REFERENCE;
    spec.boxed_primitive = MVM_STORAGE_SPEC_BP_NONE;
    spec.can_box         = 0;
    return spec;
}

/* Compose the representation. */
static void compose(MVMThreadContext *tc, MVMSTable *st, MVMObject *info) {
    /* XXX element type supplied through this... */
}

/* Initializes the representation. */
MVMREPROps * MVMIter_initialize(MVMThreadContext *tc) {
    /* Allocate and populate the representation function table. */
    this_repr = malloc(sizeof(MVMREPROps));
    memset(this_repr, 0, sizeof(MVMREPROps));
    this_repr->type_object_for = type_object_for;
    this_repr->allocate = allocate;
    this_repr->initialize = initialize;
    this_repr->copy_to = copy_to;
    this_repr->gc_mark = gc_mark;
    this_repr->gc_free = gc_free;
    this_repr->get_storage_spec = get_storage_spec; 
    this_repr->pos_funcs = malloc(sizeof(MVMREPROps_Positional));
    this_repr->pos_funcs->at_pos = at_pos;
    this_repr->pos_funcs->bind_pos = bind_pos;
    this_repr->pos_funcs->elems = elems;
    this_repr->pos_funcs->set_elems = set_elems;
    this_repr->pos_funcs->push = push;
    this_repr->pos_funcs->pop = pop;
    this_repr->pos_funcs->unshift = unshift;
    this_repr->pos_funcs->shift = shift;
    this_repr->pos_funcs->splice = splice;
    this_repr->pos_funcs->get_elem_storage_spec = get_elem_storage_spec;
    this_repr->compose = compose;
    return this_repr;
}

MVMObject * MVM_iter(MVMThreadContext *tc, MVMObject **target_addr) {
    MVMObject *target = *target_addr, *iterator;
    MVMIterBody *body;
    if (REPR(target)->ID == MVM_REPR_ID_MVMArray) {
        iterator = MVM_repr_alloc_init(tc, tc->instance->boot_types->BOOTIter);
        /* must re-grab target from the register in case the GC ran */
        target = *target_addr;
        body = &((MVMIter *)iterator)->body;
        body->mode = MVM_ITER_MODE_ARRAY;
        body->array_state.index = -1;
        body->array_state.limit = REPR(target)->pos_funcs->elems(tc, STABLE(target), target, OBJECT_BODY(target));
        body->target = target;
    }
    else if (REPR(target)->ID == MVM_REPR_ID_MVMHash) {
        iterator = MVM_repr_alloc_init(tc, tc->instance->boot_types->BOOTIter);
        /* must re-grab target from the register in case the GC ran */
        target = *target_addr;
        body = &((MVMIter *)iterator)->body;
        body->mode = MVM_ITER_MODE_HASH;
        body->hash_state.next = ((MVMHash *)target)->body.hash_head;
        body->target = target;
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot iterate this");
    }
    return iterator;
}

MVMString * MVM_iterkey_s(MVMThreadContext *tc, MVMIter *iterator) {
    if (REPR(iterator)->ID != MVM_REPR_ID_MVMIter
            || iterator->body.mode != MVM_ITER_MODE_HASH)
        MVM_exception_throw_adhoc(tc, "This is not a hash iterator");
    if (!iterator->body.hash_state.curr)
        MVM_exception_throw_adhoc(tc, "You have not advanced to the first item of the hash iterator, or have gone past the end");
    return (MVMString *)iterator->body.hash_state.curr->key;
}

MVMObject * MVM_iterval(MVMThreadContext *tc, MVMIter *iterator) {
    MVMIterBody *body;
    MVMObject *target;
    MVMRegister result;
    if (REPR(iterator)->ID != MVM_REPR_ID_MVMIter)
        MVM_exception_throw_adhoc(tc, "This is not an iterator");
    if (iterator->body.mode == MVM_ITER_MODE_ARRAY) {
        body = &iterator->body;
        if (body->array_state.index == -1)
            MVM_exception_throw_adhoc(tc, "You have not yet advanced in the array iterator");
        target = body->target;
        REPR(target)->pos_funcs->at_pos(tc, STABLE(target), target, OBJECT_BODY(target), body->array_state.index, &result, 0);
    }
    else if (iterator->body.mode == MVM_ITER_MODE_HASH) {
        if (!iterator->body.hash_state.curr)
        MVM_exception_throw_adhoc(tc, "You have not advanced to the first item of the hash iterator, or have gone past the end");
        result.o = iterator->body.hash_state.curr->value;
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot iterate this");
    }
    return result.o;
}