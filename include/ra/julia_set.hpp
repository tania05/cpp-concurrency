namespace ra :: fractal {
template <class Real >
void compute_julia_set (const std :: complex < Real >& bottom_left ,
const std :: complex < Real >& top_right , const std :: complex < Real >& c ,
int max_iters , boost :: multi_array <int, 2 >& a , int num_threads );
}