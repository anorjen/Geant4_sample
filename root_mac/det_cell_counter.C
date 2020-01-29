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

#define CELLS 400 //(36 * 25)
#define EPS 0.000001
#define THRESHOLD 0.2

using namespace std;

void	logger(TH1D *h, char *name)
{
	FILE *f;

	f = fopen("pic/SPD/cell_info.dat", "a");
	fprint_hist_info(f, h, name);
	fclose(f);
}

void	ndet_cell_counter(string inFile = "EnergyDeposit.dat")
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
		/* name */ "Cell_number",
		/* title */ "Cell_number",
		/* X-dimension */ CELLS, 0, CELLS);


	int		rId,
			eId;
	char	detname[30];
	double	energy;

	int		old_eId = 0;
	int		cells = 0;

	int			c = 0;
	double		gtime;
	
	char	line[201];
	char	*estr;
	int		ret = 0;

	while(1)
	{
		estr = fgets(line, 200, in);
		if (!estr)
		{
			if (cells > 0)
				h1 -> Fill(cells);
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
				if (cells > 0)
					h1 -> Fill(cells);
				cells = 0;
				old_eId = eId;
			}
			if (eId == old_eId && strncmp(detname, "spdCal", 6) == 0)
			{
				if (energy > THRESHOLD)
					cells += 1;
			}
		}
	}
	fclose(in);

	TCanvas *c1 =new TCanvas("CellNumber", "CellNumber", 0, 0, 600, 600);

	h1->GetXaxis()->SetTitle("N cells");
	h1->GetYaxis()->SetTitle("N events");
	h1->Draw();

	char canname[40];
	sprintf(canname, "pic/SPD/cell_%s.png", fname);
	logger(h1, fname);
	c1 -> Print(canname);

}
