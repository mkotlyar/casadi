// NOLINT(legal/copyright)
// SYMBOL "sum_viol"
template<typename T1>
T1 casadi_sum_viol(casadi_int n, const T1* x, const T1* lb, const T1* ub) {
  T1 r = 0;
  const T1 zero = 0;
  casadi_int i;
  for (i=0; i<n; ++i) {
    T1 x_i = x ? *x++ : zero;
    T1 lb_i = lb ? *lb++ : zero;
    T1 ub_i = ub ? *ub++ : zero;
    r += fmax(x_i-ub_i, zero);
    r += fmax(lb_i-x_i, zero);
  }
  return r;
}
