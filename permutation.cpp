class qap_model : public mets::permutation_problem 
{
protected:
std::vector< std::vector<int> > f_m;
std::vector< std::vector<int> > d_m;
double compute_cost () const {
double sum = 0.0;
for(unsigned int ii = 0; ii != pi_m.size(); ++ii)
	for(unsigned int jj = 0; jj != pi_m.size(); ++jj) 
		sum += f_m[ii][jj] * d_m[pi_m[ii]][pi_m[jj]];
return sum; 
}
public:
qap_model() : permutation_problem(0), f_m(), d_m() {}; 
void copy_from(const mets::copyable& sol) {
const qap_model& o = dynamic_cast <const qap_model&>(sol);
permutation_problem::copy_from(sol);
f_m = o.f_m;
d_m = o.d_m;
}
double evaluate_swap(int i, int j) const {
double delta = 0.0;
for(unsigned int ii=0; ii != f_m.size(); ++ii) {
	delta -= f_m[i][ii] * d_m[pi_m[i]][pi_m[ii]]; 
	delta -= f_m[ii][i] * d_m[pi_m[ii]][pi_m[i]]; 
	delta -= f_m[j][ii] * d_m[pi_m[j]][pi_m[ii]]; 
	delta -= f_m[ii][j] * d_m[pi_m[ii]][pi_m[j]]; 
	int ni = ii;
	if(ii==i) ni = j; else if(ii==j) ni = i; 
	delta += f_m[i][ii] * d_m[pi_m[j]][pi_m[ni]]; 
	delta += f_m[ii][i] * d_m[pi_m[ni]][pi_m[j]]; 
	delta += f_m[j][ii] * d_m[pi_m[i]][pi_m[ni]]; 
	delta += f_m[ii][j] * d_m[pi_m[ni]][pi_m[i]];
}
return delta; 
}
};


int main()
{
qap_model problem_instance;
std::cin >> problem_instance;
unsigned int N = problem_instance.size();
qap_model best_solution(problem_instance); 
mets::best_ever_solution best_recorder(best_solution);
mets::swap_full_neighborhood neighborhood(N);
std::tr1::mt19937 rng(time(NULL));
 mets::random_shuffle(problem_instance , rng);
mets::local_search <mets::swap_full_neighborhood > algorithm(problem_instance ,
incumbent_recorder , neighborhood);
mets::iteration_logger g(clog); 
algorithm.attach(g); 
algorithm.search();
std::cout
<< std :: fixed
<< N << " "
<< best_solution.cost_function() << std::endl << best_solution << std::endl;
}