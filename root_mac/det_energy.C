//######################################################
//##	macro energy_layer.C
//##	make hists with mean energy in each sheet fnd each lengthwise layers
//##
//######################################################


//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <unistd.h>

#include <stdio.h>

#include "TH2D.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>

#include "status.C"
#include "hist_utils.C"

#define EPS 0.000001
#define THRESHOLD 0.0

using namespace std;

void	logger(TH1D *h, char *name)
{
	FILE *f;

	f = fopen("pic/NDET/hist_info.dat", "a");
	fprint_hist_info(f, h, name);
	fclose(f);
}

void	ndet_energy(string inFile = "EnergyDeposit.dat", double E0 = 1000.0)
{
	FILE	*in;

	char fname[50];
	int	i = -1;
	int	k = 0;
	while (++i < inFile.length())
	{
		fname[k++] = inFile[i];
	}
	fname[k] = '\0';

	i = inFile.length() - 4;
	k = 0;

	in = fopen(fname, "r");

	while (inFile[i] != '/' && i >= 0)
		i--;
	i = (i == 0 ? -1 : i);
	while (++i < inFile.length() - 4)
	{
		fname[k++] = inFile[i];
	}
	fname[k] = '\0';

	TH1D *h1es= new TH1D(
		/* name */ "E_scint",
		/* title */ "E_scint",
		/* X-dimension */ E0 , 0, E0);

	TH1D *h1el= new TH1D(
		/* name */ "Energy_leak",
		/* title */ "Energy_leak",
		/* X-dimension */ E0 , 0, E0);

	TH1D *h1ll= new TH1D(
		/* name */ "lengthwise_leak",
		/* title */ "lengthwise_leak",
		/* X-dimension */ E0 , 0, E0);


	int		rId,
			eId;
	char	detname[30];
	double	energy;

	int		old_eId = 0;
	double	energy_s = 0.0,
			energy_w = 0.0,
			energy_ll = 0.0;

	int			c = 0;
	double_t	gtime;
	
	char	line[201];
	char	*estr;
	int	ret = 0;

	while(1)
	{
/*		if(c % 1000 == 0)
		{
			//~ ft_clock(c / 1000);
			ft_line(c / 1000);
			//~ ft_ball(c / 1000);
			//~ write(1, "\n",1);
		}
*/
		estr = fgets(line, 200, in);
		if (!estr)
		{
			if (energy_s > THRESHOLD)
				h1es -> Fill(energy_s);
			h1el -> Fill(E0 - (energy_s + energy_w));
			if (energy_ll > 0.0)
				h1ll -> Fill(energy_ll);
			write(1, "\n", 1);
			break ;
		}
		ret = sscanf(line, "%d %d %s %lf %lf", &rId, &eId, detname, &energy, &gtime);
		if (ret == 5)
		{
			++c;
			if (c == 1)
				old_eId = eId;
			if (eId != old_eId)
			{
				if (energy_s > THRESHOLD)
					h1es -> Fill(energy_s);
				h1el -> Fill(E0 - (energy_s + energy_w));
				if (energy_ll > 0.0) h1ll -> Fill(energy_ll);
				//~ cout << setw(10) << old_eId 
						//~ << setw(10) << energy_s 
						//~ << setw(10) << E0 - (energy_s + energy_w) 
						//~ << setw(10) << (energy_s + energy_w) << endl;
				energy_s = 0.0;
				energy_w = 0.0;
				energy_ll = 0.0;
				old_eId = eId;
			}
			if (eId == old_eId)
			{
				if (strncmp(detname, "spdCal", 6) == 0)
					energy_s += energy;
				else if (strncmp(detname, "Pb", 2) == 0)
					energy_w += energy;
				else if (fabs(E0 - energy) > EPS)
					energy_ll += energy;
			}
		}
	}
	fclose(in);

	TCanvas *c1 =new TCanvas("CellEnergy", "CellEnergy", 0, 0, 1200, 600);
	c1->Divide(2, 1);

	c1->cd(1);
	h1es->GetXaxis()->SetTitle("MeV");
	h1es->GetYaxis()->SetTitle("N events");
	h1es->Draw();

	c1->cd(2);
	h1el->GetXaxis()->SetTitle("MeV");
	h1el->GetYaxis()->SetTitle("N events");
	h1el->Draw();

	char canname[40];
	sprintf(canname, "pic/NDET/energy_%s.png", fname);
	logger(h1es, fname);
	logger(h1el, fname);
	c1 -> Print(canname);


	TCanvas *c2 =new TCanvas("lengthwiseLeaks", "lengthwiseLeaks", 0, 0, 600, 600);

	h1ll->GetXaxis()->SetTitle("MeV");
	h1ll->GetYaxis()->SetTitle("N events");
	h1ll->Draw();

	sprintf(canname, "pic/NDET/ll_%s.png", fname);
	c2 -> Print(canname);
	
	logger(h1ll, fname);
}
