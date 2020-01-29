 #include "G4HCofThisEvent.hh"
 #include "G4Step.hh"
 #include "G4ThreeVector.hh"
 #include "G4SDManager.hh"
 #include "G4ios.hh"
 #include "G4UnitsTable.hh"
 #include "G4VVisManager.hh"
 #include "G4Circle.hh"
 #include "G4Colour.hh"
 #include "G4VisAttributes.hh"
 
 virtual void G4VVisManager::Draw (const G4Circle&, ...) ;
 virtual void G4VVisManager::Draw (const G4Square&, ...) ;
 void MyTrackerHit::Draw()
 {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
      // define a circle in a 3D space
      G4Circle circle(pos);
      circle.SetScreenSize(0.3);
      circle.SetFillStyle(G4Circle::filled);

      // make the circle red
      G4Colour colour(1.,0.,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);

      // make a 3D data for visualization 
      pVVisManager->Draw(circle);
    }
  }
  
  void MyTrackerHitsCollection::DrawAllHits()
  {
    G4int n_hit = theCollection.entries();
    for(G4int i=0;i< n_hit;i++)
    { 
      theCollection[i].Draw(); 
    }
  }
  
   void MyEventAction::EndOfEventAction()
  {
    const G4Event* evt = fpEventManager->get_const_currentEvent();

    G4SDManager * SDman = G4SDManager::get_SDMpointer();
    G4String colNam;
    G4int trackerCollID = SDman->get_collectionID(colNam="TrackerCollection");
    G4int calorimeterCollID = SDman->get_collectionID(colNam="CalCollection");

    G4TrajectoryContainer * trajectoryContainer = evt->get_trajectoryContainer();
    G4int n_trajectories = 0;
    if(trajectoryContainer)
    { n_trajectories = trajectoryContainer->entries(); }

    G4HCofThisEvent * HCE = evt->get_HCofThisEvent();
    G4int n_hitCollection = 0;
    if(HCE)
    { n_hitCollection = HCE->get_capacity(); }
 
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

    if(pVVisManager)
    {
 
      // Declare begininng of visualization
      G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");

      // Draw trajectories
      for(G4int i=0; i< n_trajectories; i++)
      { 
          (*(evt->get_trajectoryContainer()))[i]->DrawTrajectory(); 
      }

      // Construct 3D data for hits
      MyTrackerHitsCollection* THC 
        = (MyTrackerHitsCollection*)(HCE->get_HC(trackerCollID));
      if(THC) THC->DrawAllHits();
      MyCalorimeterHitsCollection* CHC
        = (MyCalorimeterHitsCollection*)(HCE->get_HC(calorimeterCollID));
      if(CHC) CHC->DrawAllHits();

      // Declare end of visualization
      G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    
    } 

  }
