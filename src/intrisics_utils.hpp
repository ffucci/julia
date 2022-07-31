#ifndef __INTRISICS_UTILS_HPP
#define __INTRISICS_UTILS_HPP

namespace JL_I {
#include "intrinsics.h"
}

STATISTIC(EmittedConstants, "Number of constants emitted");
STATISTIC(EmittedCoercedUnboxes, "Number of unbox coercions emitted");
STATISTIC(EmittedUnboxes, "Number of unboxes emitted");
STATISTIC(EmittedRuntimeCalls, "Number of runtime intrinsic calls emitted");
STATISTIC(EmittedIntrinsics, "Number of intrinsic calls emitted");
STATISTIC(Emitted_arraylen, "Number of arraylen calls emitted");
STATISTIC(Emitted_pointerref, "Number of pointerref calls emitted");
STATISTIC(Emitted_pointerset, "Number of pointerset calls emitted");
STATISTIC(Emitted_atomic_fence, "Number of atomic_fence calls emitted");
STATISTIC(Emitted_atomic_pointerref, "Number of atomic_pointerref calls emitted");
STATISTIC(Emitted_atomic_pointerop, "Number of atomic_pointerop calls emitted");
STATISTIC(Emitted_bitcast, "Number of bitcast calls emitted");
STATISTIC(Emitted_trunc_int, "Number of trunc_int calls emitted");
STATISTIC(Emitted_sext_int, "Number of sext_int calls emitted");
STATISTIC(Emitted_zext_int, "Number of zext_int calls emitted");
STATISTIC(Emitted_uitofp, "Number of uitofp calls emitted");
STATISTIC(Emitted_sitofp, "Number of sitofp calls emitted");
STATISTIC(Emitted_fptoui, "Number of fptoui calls emitted");
STATISTIC(Emitted_fptosi, "Number of fptosi calls emitted");
STATISTIC(Emitted_fptrunc, "Number of fptrunc calls emitted");
STATISTIC(Emitted_fpext, "Number of fpext calls emitted");
STATISTIC(Emitted_not_int, "Number of not_int calls emitted");
STATISTIC(Emitted_have_fma, "Number of have_fma calls emitted");
STATISTIC(EmittedUntypedIntrinsics, "Number of untyped intrinsics emitted");

// Here we need to declare this functions because these are still
// implemented in intrisics.cpp
static jl_cgval_t emit_pointerref(jl_codectx_t &ctx, jl_cgval_t *argv);
static jl_cgval_t emit_pointerset(jl_codectx_t &ctx, jl_cgval_t *argv);
static jl_cgval_t emit_atomicfence(jl_codectx_t &ctx, jl_cgval_t *argv);
static jl_cgval_t emit_atomic_pointerref(jl_codectx_t &ctx, jl_cgval_t *argv);
static jl_cgval_t emit_atomic_pointerop(jl_codectx_t &ctx, JL_I::intrinsic f, const jl_cgval_t *argv, int nargs, const jl_cgval_t *modifyop);

// Callbacks
static jl_cgval_t on_pointer_ref(jl_codectx_t &ctx, jl_cgval_t *argv, size_t nargs)
{
     ++Emitted_pointerref;
     assert(nargs == 3);
     return emit_pointerref(ctx, argv);
}

static jl_cgval_t on_pointer_set(jl_codectx_t &ctx, jl_cgval_t *argv, size_t nargs)
{
    ++Emitted_pointerset;
    assert(nargs == 4);
    return emit_pointerset(ctx, argv);
}

static jl_cgval_t on_atomic_fence(jl_codectx_t &ctx, jl_cgval_t *argv, size_t nargs)
{
    ++Emitted_atomic_fence;
    assert(nargs == 1);
    return emit_atomicfence(ctx, argv);
}

static jl_cgval_t on_atomic_pointerref(jl_codectx_t &ctx, jl_cgval_t *argv, size_t nargs)
{
    ++Emitted_atomic_pointerref;
    assert(nargs == 2);
    return emit_atomic_pointerref(ctx, argv);
}

static jl_cgval_t on_atomic_pointerop(jl_codectx_t &ctx, JL_I::intrinsic f, const jl_cgval_t *argv, int nargs)
{
    ++Emitted_atomic_pointerop;
    return emit_atomic_pointerop(ctx, f, argv, nargs, nullptr);
} 

// Callback types
typedef jl_cgval_t (*PointerRefFunc)(jl_codectx_t&,jl_cgval_t *, size_t);
typedef jl_cgval_t (*PointerAtomicOpFunc)(jl_codectx_t&,JL_I::intrinsic, const jl_cgval_t *, size_t);

static std::unordered_map<JL_I::intrinsic, PointerRefFunc> intrinsics_map = {{JL_I::pointerref, &on_pointer_ref},
                                                                             {JL_I::pointerset, &on_pointer_set},
                                                                             {JL_I::atomic_fence, &on_atomic_fence},
                                                                             {JL_I::atomic_pointerref, &on_atomic_pointerref}};


// !brief this map is for atomic operations
static std::unordered_map<JL_I::intrinsic, PointerAtomicOpFunc> intrinsics_atomics_map = {{JL_I::atomic_pointerset, &on_atomic_pointerop},
                                                                                         {JL_I::atomic_pointerswap, &on_atomic_pointerop},
                                                                                         {JL_I::atomic_pointermodify, &on_atomic_pointerop},
                                                                                         {JL_I::atomic_pointerreplace, &on_atomic_pointerop}};

#endif