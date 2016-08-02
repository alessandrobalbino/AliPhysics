#ifndef ALIPHOSTENDERSUPPLY_H
#define ALIPHOSTENDERSUPPLY_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  PHOS tender, apply corrections to PHOS clusters                   //
//  and do track matching                                             //
//  Author : Dmitri Peressounko (RRC KI)                              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <AliTenderSupply.h>

class TVector3;
class AliPHOSGeometry;
class AliPHOSCalibData ;
class TH2I ;
class AliVCluster ;
class AliVCaloCells ;
class AliAnalysisTaskSE ;

class AliPHOSTenderSupply: public AliTenderSupply {
  
public:
  AliPHOSTenderSupply();
  AliPHOSTenderSupply(const char *name, const AliTender *tender=NULL);
  virtual ~AliPHOSTenderSupply();

  virtual void   Init(){}
  virtual void   ProcessEvent();
  
  void SetTask(AliAnalysisTaskSE * task){fTask=task;} //if work with AOD and special task
  
  void  SetNonlinearityVersion(const char * ver="Gustavo2005"){fNonlinearityVersion=ver;}
  void  SetNonlinearityParams(Int_t n, const Double_t * par){
            if(n>10){printf("Only 10 parameters allowed \n"); return ;}
            for(Int_t i=0;i<n;i++)fNonlinearityParams[i]=par[i]; }
  void  SetReconstructionPass(Int_t ipass=2){fRecoPass=ipass;}
  
  //Use this function to let tender know that it works with MC production
  //should read calibration from PHOSMCCalibration file and use object for specified production
  //By defaul real data is assumed.
  void SetMCProduction(const char * name ="LHC13_b2"){fIsMC=kTRUE ; fMCProduction=name ;}
  
  //If you want to override automatic choise of bad maps and calibration
  void ForceUsingBadMap(const char * filename="alien:///alice/cern.ch/user/p/prsnko/BadMaps/BadMap_LHC10b.root") ;
  void ForceUsingCalibration(const char * filename="alien:///alice/cern.ch/user/p/prsnko/Recalibrations/LHC10b_pass1.root") ;
  void SetAddCellNoise(Double_t rms=0.008){fAddNoiseMC=kTRUE; fNoiseMC=rms;} //Add some noise to MC data 

  void   InitTender();

protected:
  AliPHOSTenderSupply(const AliPHOSTenderSupply&c);
  AliPHOSTenderSupply& operator= (const AliPHOSTenderSupply&c);
  Int_t   FindTrackMatching(Int_t mod,TVector3 *locpos,Double_t &dx, Double_t &dz, Double_t &pttrack, Int_t &charge); 
  Float_t CorrectNonlinearity(Float_t en) ;
  Double_t TestCPV(Double_t dx, Double_t dz, Double_t pt, Int_t charge) ;
  Double_t TestCoreLambda(Double_t pt,Double_t l1,Double_t l2) ;
  Double_t TestFullLambda(Double_t pt,Double_t l1,Double_t l2) ;
  Bool_t IsGoodChannel(Int_t mod, Int_t ix, Int_t iz) ;
  void   CorrectPHOSMisalignment(TVector3 & globalPos, Int_t module);
  void   EvalLambdas(AliVCluster * clu, Double_t &m02, Double_t &m20) ;
  Double_t CoreEnergy(AliVCluster * clu) ;  
  Double_t EvalEcross(AliVCluster * clu) ;  
  Double_t EvalTOF(AliVCluster * clu,AliVCaloCells * cells); 
  Double_t CalibrateTOF(Double_t tof, Int_t absId, Bool_t isHG); 
  void DistanceToBadChannel(Int_t mod, TVector3 * locPos, Double_t &minDist) ;

 
private:

  TString fOCDBpass ;                        // Pass to OCDB recalibration object, local or alien
  TString fNonlinearityVersion;              // Version of non-linearity correction to aaply
  AliPHOSGeometry   *fPHOSGeo;               // PHOS geometry
  Double_t fNonlinearityParams[10] ;         // Parameters for non-linearity calculation
  TH2I * fPHOSBadMap[6] ;                    // Bad channels map
  Int_t fL1phase[15] ;                       // L1phases for PHOS DDLs (run2 only)
  Int_t fRecoPass ;                          // Reconstruction pass
  Int_t fRunNumber ;                         // run number
  Bool_t fUsePrivateBadMap ;
  Bool_t fUsePrivateCalib ;
  Bool_t fAddNoiseMC ;                       //Should we add cell-by-cell noise in MC simulations
  Double_t fNoiseMC  ;                       //RMS of cell-by-cell noise (in GeV)

  AliPHOSCalibData *fPHOSCalibData;          // PHOS calibration object
  AliAnalysisTaskSE     *fTask;              // analysis task

  Bool_t fIsMC;                              //True if work with MC data
  TString fMCProduction ;                    //Name of MC production
 
  ClassDef(AliPHOSTenderSupply, 4); // PHOS tender task
};


#endif


