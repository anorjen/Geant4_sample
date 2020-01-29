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

#define FIRST_LAYS		15
#define	SECOND_LAYS		0
#define	THIRD_LAYS		0

#define FIRST_LAYS_BOXD  20.0
#define	FIRST_LAYS_PBD   3.0

#define	SECOND_LAYS_BOXD 0.0
#define	SECOND_LAYS_PBD  0.0

#define THIRD_LAYS_BOXD  0.0
#define	THIRD_LAYS_PBD   0.0

#define CELL_MIN         30.0
#define CELL_MAX         30.0
#define ROWS 			  8
#define COLS 			  8

#define LAYS 	(FIRST_LAYS + SECOND_LAYS + THIRD_LAYS)
#define	LENGTH  (FIRST_LAYS * (FIRST_LAYS_BOXD + FIRST_LAYS_PBD) + SECOND_LAYS * (SECOND_LAYS_BOXD + SECOND_LAYS_PBD) + THIRD_LAYS * (THIRD_LAYS_BOXD + THIRD_LAYS_PBD) - THIRD_LAYS_PBD)
#define ADDSIZE ((CELL_MAX - CELL_MIN) / LENGTH)
#define EPS				  0.000001

//~ #define EVENTS 10000

using namespace std;

double	get_length(int l)
{
	if (l == 0)
		return 0;
	if (l < FIRST_LAYS)
		return (l - 1) * (FIRST_LAYS_BOXD + FIRST_LAYS_PBD) + FIRST_LAYS_BOXD / 2;
	if (l < SECOND_LAYS)
		return FIRST_LAYS * (FIRST_LAYS_BOXD + FIRST_LAYS_PBD) + (l - FIRST_LAYS) * (SECOND_LAYS_BOXD + SECOND_LAYS_PBD) + SECOND_LAYS_BOXD / 2; 
	return FIRST_LAYS * (FIRST_LAYS_BOXD + FIRST_LAYS_PBD) + (SECOND_LAYS) * (SECOND_LAYS_BOXD + SECOND_LAYS_PBD) + (l - FIRST_LAYS - SECOND_LAYS) * (THIRD_LAYS_BOXD + THIRD_LAYS_PBD) + THIRD_LAYS_BOXD / 2; 

}

double	get_coord(int l, int x)
{
	double	xc;

	xc = (COLS - 1) / 2.0;
	return (((double)x - xc) * (CELL_MIN + get_length(l) * ADDSIZE));
}

int	getCoordinate(double cell_energy[LAYS][COLS][ROWS], double start_point, double *result)
{

	int		i = 0,
			j = 0,
			k = 0;

	double	num_det[2],
			denom_det = 0.0;

	for(int i = 0; i < 2; i++)
	{
		num_det[i] = 0.0;
	}
	for(int i = 0; i < LAYS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			for(int k = 0; k < ROWS; k++)
			{
				num_det[0] += get_coord(i, j) * cell_energy[i][j][k];
				//~ num_det[1] += get_coord(i, k) * cell_energy[i][j][k];
				denom_det += cell_energy[i][j][k];
			}
		}
	}
	*result = num_det[0] / denom_det - start_point;
	if (denom_det > EPS)
	{
		//~ cout << setw(15) << *result
			//~ << setw(15) << start_point << endl;
		return (0);
	}
	return (-1);
}

void	free_array(double arr[LAYS][COLS][ROWS])
{
	int	i = 0,
		j = 0,
		k = 0;

	for(int i = 0; i < LAYS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			for(int k = 0; k < ROWS; k++)
			{
				arr[i][j][k] = 0.0;
			}
		}
	}
}


void	ndet_start_point(string inFile = "EnergyDeposit.dat")
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

	TH1D *h1r= new TH1D(
		/* name */ "R_coordinate",
		/* title */ "R_coordinate",
		/* X-dimension */ 100, -50, 50);

	int			rId,
				eId;
	char		detname[30];
	double_t	energy;
	double_t	gtime;

	double_t	cell_energy[LAYS][COLS][ROWS];
	int			c = 0;
	int			xc, yc, lc;
	char		mod[10];

	int			eId_old = 0;
	double		r;
	int			coord_status = 0;

	char	line[201];
	char	*estr;
	int		ret;
	double	start_point = 0.0,
		old_point = 0.0;

	free_array(cell_energy);
	while(1)
	{
		// <status 
/*
		if(c % 1000 == 0)
		{
			//~ ft_clock(c / 1000);
			ft_line(c / 1000);
			//~ ft_ball(c / 1000);
			//~ write(1, "\n",1);
		}
		// status/> 
*/
		estr = fgets(line, 200, in);
		if (!estr)
		{
			if ((coord_status = getCoordinate(cell_energy, start_point, &r)) == 0)
				h1r->Fill(r);
			write(1, "\n", 1);
			break ;
		}
		ret = sscanf(line, "%d %d %s %lf %lf", &rId, &eId, detname, &energy, &gtime);
		if (ret < 5)
		{
			old_point = start_point;
			sscanf(line, " coordinate %lf ", &start_point);
			//cout << setw(30) << line
			//	<< setw(15) << start_point << endl;
			continue ;
		}
		if (strncmp(detname, "spdCal", 6) != 0)
			continue ;
		c++;
		if (c == 1)
			eId_old = eId;
		if (eId != eId_old)
		{
			if ((coord_status = getCoordinate(cell_energy, old_point, &r)) == 0)
				h1r->Fill(r);
			free_array(cell_energy);
			eId_old = eId;
		}
		sscanf(detname, "%[a-zA-Z]_%d_%d_%d", mod, &lc, &yc, &xc);
		cell_energy[lc][xc][yc] += energy;
	}
	fclose(in);

	char	canname[50];

	TCanvas *c1 =new TCanvas("cell_lays", "cell_lays", 0, 0, 1200, 800);
	h1r->GetXaxis()->SetTitle("R, mm");
	h1r->GetYaxis()->SetTitle("N");
	h1r->Draw();

	sprintf(canname, "pic/NDET/sp_%s.png", fname);
	c1 -> Print(canname);

	FILE *f;
	f = fopen("pic/NDET/hist_info.dat", "a");
	fprint_hist_info(f, h1r, fname);
	fprintf(f, "\n");
	fclose(f);

}
