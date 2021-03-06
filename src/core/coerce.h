/* Boolification. */
MVMint64 MVM_coerce_istrue_s(MVMThreadContext *tc, struct _MVMString *str);
void MVM_coerce_istrue(MVMThreadContext *tc, MVMObject *obj, MVMRegister *res_reg,
    MVMuint8 *true_addr, MVMuint8 *false_addr, MVMuint8 flip);

/* Stringification. */
struct _MVMString * MVM_coerce_i_s(MVMThreadContext *tc, MVMint64 i);
struct _MVMString * MVM_coerce_n_s(MVMThreadContext *tc, MVMnum64 n);
void MVM_coerce_smart_stringify(MVMThreadContext *tc, MVMObject *obj, MVMRegister *res_reg);

/* Numification. */
MVMint64 MVM_coerce_s_i(MVMThreadContext *tc, struct _MVMString *s);
MVMnum64 MVM_coerce_s_n(MVMThreadContext *tc, struct _MVMString *s);
void MVM_coerce_smart_numify(MVMThreadContext *tc, MVMObject *obj, MVMRegister *res_reg);
MVMint64 MVM_coerce_simple_intify(MVMThreadContext *tc, MVMObject *obj);
MVMObject* MVM_radix(MVMThreadContext *tc, MVMint64 radix, struct _MVMString *str, MVMint64 offset, MVMint64 flag);