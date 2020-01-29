#ifndef SENSITIVEDETECTOR
#define SENSITIVEDETECTOR
#include<G4VSensitiveDetector.hh>
#include<stdio.h>

class G4Step;
class G4TouchableHistory;


class SensitiveDetector: public G4VSensitiveDetector
{
private:
		char		detname[30];
		double		energy_event;
		double		Gtime_sum;
		int			N_Hit;
		int 		stepnumber;

public:   
	SensitiveDetector(G4String name);
	~SensitiveDetector();
	G4bool ProcessHits(G4Step *step, G4TouchableHistory *hist);
	void EndOfEvent(G4HCofThisEvent*);

};




#endif /* SENSITIVEDETECTOR */
