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

#define LAYS 15
#define EVENTS 10000
#define EPS   0.000001
#define THRESHOLD 0.0

using namespace std;

void	ndet_lays_energy(string inFile = "EnergyDeposit.dat")
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

	TH1D *h1= new TH1D(
		/* name */ "E_lay",
		/* title */ "E_lay",
		/* X-dimension */ LAYS + 1, -1, LAYS);


	int		rId,
			eId;
	char	detname[30];
	double	energy;

	int		old_eId = 0;
	int			xc, yc, lc;
	char		mod[10];

	int			c = 0;
	double_t	gtime;
	
	char	line[201];
	char	*estr;
	double	lay_energy[LAYS];
	int		ret = 0;
	
	for (int i = 0; i < LAYS; i++)
	{
		lay_energy[i] = 0.0;
	}

	while(1)
	{
		estr = fgets(line, 200, in);
		if (!estr)
		{
			write(1, "\n", 1);
			break ;
		}
		ret = sscanf(line, "%d %d %s %lf %lf", &rId, &eId, detname, &energy, &gtime);
		if (ret == 5 && strncmp(detname, "spdCal", 6) == 0)
		{
			sscanf(detname, "%[a-zA-Z]_%d_%d_%d", mod, &lc, &yc, &xc);
			lay_energy[lc] += energy / EVENTS;
		}
	}
	fclose(in);
	
	for (int i = 0; i < LAYS; i++)
	{
		lay_energy[i] = (lay_energy[i] > THRESHOLD ? lay_energy[i] : 0.0);
		h1->Fill(i, lay_energy[i]);
		//~ cout << setw(15) << lay_energy[i] << endl;
	}

	TCanvas *c1 =new TCanvas("LaysEnergy", "LaysEnergy", 0, 0, 600, 600);

	h1->GetXaxis()->SetTitle("Lays");
	h1->GetYaxis()->SetTitle("E, MeV");
	h1->SetBarWidth(0.8);
	h1->SetFillColor(4);
	h1->SetBarOffset(-0.35);
	//~ h1->GetXaxis()->SetNdivisions(120);
	h1->Draw("BARHIST");

	char canname[40];
	sprintf(canname, "pic/NDET/lay_energy_%s.png", fname);
	c1 -> Print(canname);
}
