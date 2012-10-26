const int FILE_BUFFER=256;
struct runopts{
	int calc_shannon;
	int calc_metric;
	char* input_file;
};
struct runresults{
	double shannon_entropy;
	double metric_entropy;
};
int calculateEntropy(FILE*, struct runresults*);
double lg2(double);
