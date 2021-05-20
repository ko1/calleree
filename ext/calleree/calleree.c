#include "ruby/ruby.h"
#include "ruby/debug.h"
#include "ruby/st.h"

VALUE rb_mCalleree;

struct eree_data {
    VALUE tp; // TracePoint.new(:call)
    VALUE files; // name -> [ {[fname, 1], ... } ]
    struct st_table* caller_callee;  // [posnum | posnum] -> cnt
    int last_posnum;
} eree_data;

static int
posnum(struct eree_data *data, VALUE path, int line)
{
    VALUE lines, posnumv;

    if (NIL_P(lines = rb_hash_aref(data->files, path))) {
        rb_hash_aset(data->files, path, lines = rb_ary_new());
    }
    if (NIL_P(posnumv = rb_ary_entry(lines, line))) {
        rb_ary_store(lines, line, posnumv = INT2FIX(data->last_posnum++));
    }
    return FIX2INT(posnumv);
}

static int
increment_i(st_data_t *key, st_data_t *value, st_data_t arg, int existing)
{
    *value = (existing ? *value : 0) + 1;
    return ST_CONTINUE;
}

static void
eree_called(VALUE tpval, void *ptr)
{
    struct eree_data *data = ptr;
    VALUE frames[2];
    int lines[2];
    // int start, int limit, VALUE *buff, int *lines);
    rb_profile_frames(1, 2, frames, lines);
    int callee_posnum = posnum(data, rb_profile_frame_path(frames[0]), lines[0]);
    int caller_posnum = posnum(data, rb_profile_frame_path(frames[1]), lines[1]);
    // rb_p(rb_profile_frame_path(frames[1]));
    st_data_t eree_pair = (((VALUE)caller_posnum << 32) | ((VALUE)callee_posnum));

    st_update(data->caller_callee, eree_pair, increment_i, 0);
}

static VALUE
eree_start(VALUE self)
{
    if (!eree_data.tp) {
        eree_data.tp = rb_tracepoint_new(0, RUBY_EVENT_CALL, eree_called, &eree_data);
        rb_gc_register_mark_object(eree_data.tp);
        eree_data.caller_callee = st_init_numtable();
    }
    rb_tracepoint_enable(eree_data.tp);
    return self;
}

static VALUE
eree_stop(VALUE self)
{
    if (eree_data.tp) {
        rb_tracepoint_disable(eree_data.tp);
    }
    return self;
}

static int
raw_result_i(st_data_t key, st_data_t val, st_data_t data)
{
    VALUE ary = (VALUE)data;
    int callee_posnum = (int)(key & 0xffffffff);
    int caller_posnum = (int)(key >> 32);
    rb_ary_push(ary, rb_ary_new_from_args(3, INT2FIX(caller_posnum), INT2FIX(callee_posnum), INT2FIX((int)val)));
    return ST_CONTINUE;
}

static VALUE
eree_raw_result(VALUE self, VALUE clear_p)
{
    VALUE ary = rb_ary_new();
    if (eree_data.tp) {
        st_foreach(eree_data.caller_callee, raw_result_i, (st_data_t)ary);

        if (RTEST(clear_p)) {
            st_clear(eree_data.caller_callee);
        }
    }
    return ary;
}

static VALUE
eree_raw_posmap(VALUE self)
{
    return eree_data.files;
}

void
Init_calleree(void)
{
    eree_data.files = rb_hash_new();
    rb_funcall(eree_data.files, rb_intern("compare_by_identity"), 0);
    rb_gc_register_mark_object(eree_data.files);

    rb_mCalleree = rb_define_module("Calleree");
    rb_define_singleton_method(rb_mCalleree, "_start", eree_start, 0);
    rb_define_singleton_method(rb_mCalleree, "_stop", eree_stop, 0);
    rb_define_singleton_method(rb_mCalleree, "raw_result", eree_raw_result, 1);
    rb_define_singleton_method(rb_mCalleree, "raw_posmap", eree_raw_posmap, 0);
}
