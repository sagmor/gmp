#ifndef _RUBY_GMP_H_
#define _RUBY_GMP_H_

#include <stdio.h>
#include <ruby.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <gmp.h>

#ifdef MPFR

#ifdef HAVE_MPFR_H
#include <mpfr.h>
#endif /* HAVE_MPFR_H */

#ifdef HAVE_MPF2MPFR_H
#include <mpf2mpfr.h>
#endif /* HAVE_MPF2MPFR_H */

#endif /* MPFR */

#include <stdlib.h>

/*
  MP_INT*, MP_RAT* and MP_FLOAT* are used because they don't have side-effects
  of single-element arrays mp*_t

  MP_FLOAT is defined here, as it's commented out in gmp.h
*/
#if defined(MPFR) && defined(HAVE_MPFR_H)
typedef __mpfr_struct MP_FLOAT;
#else
typedef __mpf_struct MP_FLOAT;
#endif /* HAVE_MPF2MPFR_H */

#define mpz_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_INT, c_var); }
#define mpq_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_RAT, c_var); }
#define mpf_get_struct(ruby_var,c_var) { Data_Get_Struct(ruby_var, MP_FLOAT, c_var); }
#define mpf_get_struct_prec(ruby_var,c_var,prec) { mpf_get_struct(ruby_var,c_var); prec = mpf_get_prec(c_var); }
#define mpz_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Z, MP_INT, 0, r_gmpz_free, c_var); }
#define mpq_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_Q, MP_RAT, 0, r_gmpq_free, c_var); }
#define mpf_make_struct(ruby_var,c_var) { ruby_var = Data_Make_Struct(cGMP_F, MP_FLOAT, 0, r_gmpf_free, c_var); }
#define mpz_make_struct_init(ruby_var,c_var) { mpz_make_struct(ruby_var,c_var); mpz_init (c_var); }
#define mpq_make_struct_init(ruby_var,c_var) { mpq_make_struct(ruby_var,c_var); mpq_init (c_var); }
#define mpf_make_struct_init(ruby_var,c_var,prec) { mpf_make_struct(ruby_var,c_var); mpf_init2 (c_var,prec); }
#define BIGNUM_P(value) (TYPE(value) == T_BIGNUM)
#define FLOAT_P(value)  (TYPE(value) == T_FLOAT)
#define STRING_P(value) (TYPE(value) == T_STRING)
#define GMPZ_P(value)   (rb_obj_is_instance_of(value, cGMP_Z) == Qtrue)
#define GMPQ_P(value)   (rb_obj_is_instance_of(value, cGMP_Q) == Qtrue)
#define GMPF_P(value)   (rb_obj_is_instance_of(value, cGMP_F) == Qtrue)
#define mpz_set_bignum(var_mpz,var_bignum) \
  mpz_set_str (var_mpz, STR2CSTR (rb_funcall (var_bignum, rb_intern ("to_s"), 0)), 0);
#define mpz_temp_alloc(var) { var=malloc(sizeof(MP_INT)); }
#define mpz_temp_init(var) { mpz_temp_alloc(var); mpz_init(var); }
#define mpz_temp_from_bignum(var,var_bignum)  \
  { mpz_temp_alloc(var); mpz_init_set_str (var, STR2CSTR (rb_funcall (var_bignum, rb_intern ("to_s"), 0)), 0); }
#define mpz_temp_free(var) { mpz_clear(var); free(var); }
#define mpf_temp_alloc(var) { var=malloc(sizeof(MP_FLOAT)); }
#define mpf_temp_init(var,prec) { mpf_temp_alloc(var); mpf_init2(var,prec); }
#define mpf_temp_free(var) { mpf_clear(var); free(var); }
#define prec_max(prec,var) {if(mpf_get_prec(var) > prec) prec = mpf_get_prec(var); }

#define EXPECTED_ZQFXBD "Expected GMP::Z, GMP::Q, GMP::F, FixNum, BigNum or Float"
#define EXPECTED_ZQFXB "Expected GMP::Z, GMP::Q, GMP::F, FixNum or BigNum"
#define EXPECTED_ZXB "Expected GMP::Z, FixNum or BigNum"
#define EXPECTED_ZX "Expected GMP::Z or FixNum"
#define EXPECTED_X "Expected FixNum"
#define typeerror(expected) rb_raise(rb_eTypeError, EXPECTED_##expected)
#define typeerror_as(expected, argname) rb_raise(rb_eTypeError, EXPECTED_##expected " as " argname)

//should change exception type
#define not_yet rb_raise(rb_eTypeError,"Not implemented yet")

extern VALUE mGMP, cGMP_Z, cGMP_Q, cGMP_F;

extern void r_gmpz_free(void *ptr);
extern void r_gmpq_free(void *ptr);
extern void r_gmpf_free(void *ptr);


/* from gmpz.h */

// Initializing, Assigning Integers
extern VALUE r_gmpzsg_new(int argc, VALUE *argv, VALUE klass);
extern void mpz_set_value(MP_INT *target, VALUE source);
extern VALUE r_gmpmod_z(int argc, VALUE *argv, VALUE module);
extern VALUE r_gmpz_swap(VALUE self, VALUE arg);

// Converting Integers
extern VALUE r_gmpz_to_i(VALUE self);
extern VALUE r_gmpz_to_d(VALUE self);

// Integer Arithmetic
extern VALUE r_gmpz_add(VALUE self, VALUE arg);
extern VALUE r_gmpz_add_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub(VALUE self, VALUE arg);
extern VALUE r_gmpz_sub_self(VALUE self, VALUE arg);
extern VALUE r_gmpz_mul(VALUE self, VALUE arg);

// Number Theoretic Functions
extern VALUE r_gmpz_remove(VALUE self, VALUE arg);

// Integer Comparisons
extern VALUE r_gmpz_eq(VALUE self, VALUE arg);
extern VALUE r_gmpz_cmp(VALUE self, VALUE arg);
extern VALUE r_gmpz_cmpabs(VALUE self, VALUE arg);

// _unsorted_
extern VALUE r_gmpz_div(VALUE self, VALUE arg);
extern VALUE r_gmpz_is_probab_prime(int argc, VALUE* argv, VALUE self);
extern VALUE r_gmpz_popcount(VALUE self);
extern VALUE r_gmpz_jacobi(VALUE self);
extern VALUE r_gmpz_legendre(VALUE self);
extern VALUE r_gmpz_setbit(VALUE self, VALUE bitnr, VALUE set_to);
extern VALUE r_gmpz_getbit(VALUE self, VALUE bitnr);
extern VALUE r_gmpz_scan0(VALUE self, VALUE bitnr);
extern VALUE r_gmpz_scan1(VALUE self, VALUE bitnr);
extern VALUE r_gmpz_powm(VALUE self, VALUE exp, VALUE mod);
extern VALUE r_gmpz_sgn(VALUE self);
extern int mpz_cmp_value(MP_INT *OP, VALUE arg);

extern VALUE r_gmpzsg_pow(VALUE klass, VALUE base, VALUE exp);
extern VALUE r_gmpz_to_s(VALUE self);


/* from gmpq.h */

// Initializing Rationals
extern VALUE r_gmpmod_q(int argc, VALUE *argv, VALUE module);
extern VALUE r_gmpq_swap(VALUE self, VALUE arg);

// Rational Conversions
extern VALUE r_gmpq_to_d(VALUE self);

// Rational Arithmetic
extern VALUE r_gmpq_add(VALUE self, VALUE arg);
extern VALUE r_gmpq_sub(VALUE self, VALUE arg);
extern VALUE r_gmpq_mul(VALUE self, VALUE arg);
extern VALUE r_gmpq_div(VALUE self, VALUE arg);
extern VALUE r_gmpq_neg(VALUE self);
extern VALUE r_gmpq_neg_self(VALUE self);
extern VALUE r_gmpq_abs(VALUE self);
extern VALUE r_gmpq_abs_self(VALUE self);
extern VALUE r_gmpq_inv(VALUE self);
extern VALUE r_gmpq_inv_self(VALUE self);

// Comparing Rationals
extern VALUE r_gmpq_eq(VALUE self, VALUE arg);
extern VALUE r_gmpq_cmp(VALUE self, VALUE arg);
extern int mpq_cmp_value(MP_RAT *OP, VALUE arg);
extern VALUE r_gmpq_sgn(VALUE self);

// Applying Integer Functions
extern VALUE r_gmpq_num(VALUE self);
extern VALUE r_gmpq_den(VALUE self);

// _unsorted_


/* from gmpf.h */

// Initializing, Assigning Floats
extern void mpf_set_value(MP_FLOAT *self_val, VALUE arg);
extern VALUE r_gmpmod_f(int argc, VALUE *argv, VALUE module);

// Converting Floats
extern VALUE r_gmpf_to_d(VALUE self);
extern VALUE r_gmpf_to_s(VALUE self);

// Float Arithmetic
extern VALUE r_gmpf_add(VALUE self, VALUE arg);
extern VALUE r_gmpf_sub(VALUE self, VALUE arg);
extern VALUE r_gmpf_mul(VALUE self, VALUE arg);
extern VALUE r_gmpf_div(VALUE self, VALUE arg);

// Float Comparison
extern VALUE r_gmpf_eq(VALUE self, VALUE arg);
extern VALUE r_gmpf_cmp(VALUE self, VALUE arg);
extern int mpf_cmp_value(MP_FLOAT *OP, VALUE arg);

// _unsorted_
extern VALUE r_gmpf_sgn(VALUE self);
extern VALUE r_gmpf_get_prec(VALUE self);


extern void init_gmpz();
extern void init_gmpq();
extern void init_gmpf();

#endif
