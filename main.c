#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"


void usage(){
	puts("Entropy1: Calculate stream or file entopy");
	puts("Usage:");
	puts("entropy1 [-m]|[-s]|[-b] [file|-]");
	puts("-m calculates metric entropy");
	puts("-s calculates shannon entropy");
	puts("-b calculates both metric and shannon entropy, output space separated, default");
}
int main(int argc, char *argv[])
{
    struct runopts opts={.calc_shannon=1, .calc_metric=1, .input_file=0};
	FILE *input;

	for (int i=1; i<argc; i++){
		if (strcmp(argv[i],"-m")==0){
			opts.calc_metric=1;
			opts.calc_shannon=0;
		}
		else if (strcmp(argv[i],"-s")==0){
			opts.calc_metric=0;
			opts.calc_shannon=1;
		}
		else if (strcmp(argv[i],"-b")==0){
			opts.calc_metric=1;
			opts.calc_shannon=1;
		}
		else {
			opts.input_file=argv[i];
		}

	}
	if (!opts.input_file){
		usage();
		exit(1);
	}
	if(!strcmp(opts.input_file,"-")) {
		input = stdin;
    } else {
      input = fopen(opts.input_file,"rb");
		if (!input){
			puts("Cannot open file");
			exit(2);
		}
    }
	struct runresults runresults;
	if (calculateEntropy(input, &runresults)){
		puts("Unexpected error");
		exit(2);
	}
	if (opts.calc_shannon)
	{
		printf("%f", runresults.shannon_entropy);
	}
	if (opts.calc_metric)
	{
		if (opts.calc_shannon){
			printf(" ");
		}
		printf("%f", runresults.metric_entropy);
	}
	printf("\n");

    return 0;
}

int calculateEntropy(FILE* input, struct runresults* runresults){
	long freqs[256];
	long total_chars=0;
	memset(freqs,0,sizeof(freqs));
	char buffer[FILE_BUFFER];
	int buffer_len=0;
	int buffer_pos=0;
	while(1){
		buffer_len=fread(buffer,1, sizeof(buffer), input);
		if (buffer_len<=0)
		{
			break;
		}
		buffer_pos=0;
		while (buffer_pos<buffer_len){
			freqs[buffer[buffer_pos]]++;
			buffer_pos++;
			total_chars++;
		}
	}
	double shannon_entropy = 0;
	for(int i=0;i<(sizeof(freqs)/sizeof(long));i++){
		if (freqs[i]>0){
			double p=((double)freqs[i])/total_chars;
			shannon_entropy+=p*lg2(p);
		}
	}
	runresults->shannon_entropy=-shannon_entropy;
	runresults->metric_entropy=-shannon_entropy/total_chars;
	return 0;
}
double lg2(double x){
	return log(x)/log(2);
}

