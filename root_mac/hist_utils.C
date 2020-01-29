using namespace std;

void	print_hist_info( TH1D *h, char c[])
{
	Double_t	entries = h->GetEntries();
	Double_t	mean = h->GetMean();
	Double_t	rms = h->GetRMS();

	cout << "\n#########################\n\n"
		<< c << "\n"
		<< setw(15) << entries
		<< setw(15) << mean
		<< setw(15) << rms << endl;
}

void	fprint_hist_info(FILE *f, TH1D *h, char *name)
{
	Double_t	entries = h->GetEntries();
	Double_t	mean = h->GetMean();
	Double_t	rms = h->GetRMS();

	fprintf(f, "%s: %15.0lf %15.2lf %15.2lf\n", name, entries, mean, rms);
}

