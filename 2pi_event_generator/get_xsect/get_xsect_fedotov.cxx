#include <iomanip>
#include <string>
#include <stdio.h> 
#include <math.h>
#include <TLorentzVector.h>
#include <iostream>
#include "global.h"
#include "interpol_fedotov.h"


using namespace std;

Short_t getWbin_fed (Float_t W) {
Short_t bin;

if ((W>=1.3125)&&(W<=1.3375)) bin = 0;
if ((W>=1.3375)&&(W<=1.3625)) bin = 1;
if ((W>=1.3625)&&(W<=1.3875)) bin = 2;
if ((W>=1.3875)&&(W<=1.4125)) bin = 3;
if ((W>=1.4125)&&(W<=1.4375)) bin = 4;
if ((W>=1.4375)&&(W<=1.4625)) bin = 5;
if ((W>=1.4625)&&(W<=1.4875)) bin = 6;
if ((W>=1.4875)&&(W<=1.5125)) bin = 7;

if ((W>=1.5125)&&(W<=1.5375)) bin = 8;
if ((W>=1.5375)&&(W<=1.5625)) bin = 9;
if ((W>=1.5625)&&(W<=1.5875)) bin = 10;

if ((W<1.3125)||(W>1.5875)) {
cout << "Error, wrong W range, Fedotov" << "\n";
bin = -100;
};
return bin;
};
//-------
Short_t getQ2bin_fed (Float_t Q2) {

Short_t bin;
if ((Q2 >= 0.225) && (Q2 <= 0.275)) bin = 0;
if ((Q2 >= 0.275) && (Q2 <= 0.325)) bin = 1;
if ((Q2 >= 0.325) && (Q2 <= 0.425)) bin = 2;

if ((Q2 >= 0.425) && (Q2 <= 0.475)) bin = 3;
if ((Q2 >= 0.475) && (Q2 <= 0.525)) bin = 4;
if ((Q2 >= 0.525) && (Q2 <= 0.575)) bin = 5;

if ((Q2 < 0.225) || (Q2 > 0.575)) {
cout << "Error, wrong Q2 range, Fedotov" << "\n";
bin = -100;
};
return bin;
};

//---------
Short_t getsbin_fed (Short_t Wbin, Float_t sgen, Float_t Smax, Float_t Smin) {

Short_t bin;
if ((sgen>=Smin)&&(sgen<=Smax)) bin = int((sgen-Smin)/((Smax - Smin)/9.));
if (sgen<Smin) bin = 0;
if (sgen>Smax) bin = 8;

return bin;
};
//--------------------

Short_t getanglebin_fed (Float_t anglegen, Float_t anglemax) {

Short_t bin;
if ((anglegen < 0.01)) bin = 0;
if ((anglegen > anglemax - 0.01)) bin = 6;
if ((anglegen >= 0.01) && (anglegen <= anglemax - 0.01)) bin = int((anglegen - 0.01)/((anglemax  - 0.02)/7.));

return bin;
};


//This subroutine gets Fedotov cross sections on the tabuted grid and interpolates them to a desired point within the region covered by Fedotov cross section
//This is the grid and xsect arrays for Fedotov cross sections:
//W_ARR_FED[12];
//Q2_ARR_FED[7];
//S12_ARR_FED[10][12];
//S23_ARR_FED[10][12];
//THETA_ARR_FED[8]; 
//ALPHA_ARR_FED[8];
//SIGMA_ARR_FED[6][7][12][10][10][8][8];

//The subrouting is doing the following:
//1 - using auxiliary functions (getWbin, getQ2bin, getsbin, getanglebin) we identify the number of left and right point 
//2 - then we are doing 4d-interpolation for each (Wleft_bin, Q2_left_bin),  (Wright_bin, Q2_left_bin), (Wright_bin, Q2_right_bin) and (Wleft_bin, Q2_right_bin) and obtain cross-secton in that points (4 GLOBAL 6dim arrays)
//3 - then we are doing 2d-interpolation for the points written above and obtain sigma_final[6]



void get_xsect_fedotov(Float_t Q2gen, Float_t Wgen, Float_t s12gen,Float_t s23gen, Float_t thetagen, Float_t alphagen, Float_t phigen,Float_t &sigma_t_final, Float_t &sigma_l_final,Float_t  &sigma_c2f_final,Float_t  &sigma_s2f_final,Float_t &sigma_cf_final,Float_t  &sigma_sf_final){

Short_t Wleft_bin = getWbin_fed(Wgen);
Short_t Wright_bin = Wleft_bin+1;

//cout << Wgen <<" "<< Wleft_bin << " " <<Wright_bin <<"\n";

Short_t Q2left_bin = getQ2bin_fed(Q2gen);
Short_t Q2right_bin = Q2left_bin+1;

//cout << Q2gen <<" "<< Q2left_bin << " " <<Q2right_bin <<"\n";

Short_t s12left_wleft_bin = getsbin_fed(Wleft_bin, s12gen, S12_ARR_FED[9][Wleft_bin], S12_ARR_FED[0][Wleft_bin]);
Short_t s12right_wleft_bin = s12left_wleft_bin +1;

Short_t s12left_wright_bin = getsbin_fed(Wright_bin, s12gen, S12_ARR_FED[9][Wright_bin], S12_ARR_FED[0][Wright_bin]);
Short_t s12right_wright_bin = s12left_wright_bin +1;

Short_t s23left_wleft_bin = getsbin_fed(Wleft_bin, s23gen, S23_ARR_FED[9][Wleft_bin], S23_ARR_FED[0][Wleft_bin]);
Short_t s23right_wleft_bin = s23left_wleft_bin +1;

Short_t s23left_wright_bin = getsbin_fed(Wright_bin, s23gen, S23_ARR_FED[9][Wright_bin], S23_ARR_FED[0][Wright_bin]);
Short_t s23right_wright_bin = s23left_wright_bin +1;

//cout << s23left_wleft_bin <<" "<< Wgen << " "<<Wleft_bin<< " " << s23gen << " "<< S23_ARR[s23left_wleft_bin][Wleft_bin] << " "<< S23_ARR[s23right_wleft_bin][Wleft_bin] << "\n";


Short_t thetaleft_bin = getanglebin_fed(thetagen,THETA_ARR_FED[7]);
Short_t thetaright_bin = thetaleft_bin+1;

Short_t alphaleft_bin = getanglebin_fed(alphagen,ALPHA_ARR_FED[7]);
Short_t alpharight_bin = alphaleft_bin+1;

//cout << alphagen<<" "<< alphaleft_bin<<" "<< alpharight_bin<<" \n";

Float_t sigma_final[6];

for (Short_t i=0;i<6;i++){
interpol_fedotov(4,Q2left_bin,Wright_bin,s12left_wright_bin,s12right_wright_bin,s23left_wright_bin,s23right_wright_bin,thetaleft_bin,thetaright_bin,alphaleft_bin,alpharight_bin,s12gen,s23gen,thetagen,alphagen,sigma_wright_q2left[i],i);

interpol_fedotov(4,Q2right_bin,Wright_bin,s12left_wright_bin,s12right_wright_bin,s23left_wright_bin,s23right_wright_bin,thetaleft_bin,thetaright_bin,alphaleft_bin,alpharight_bin,s12gen,s23gen,thetagen,alphagen,sigma_wright_q2right[i],i);

interpol_fedotov(4,Q2right_bin,Wleft_bin,s12left_wleft_bin,s12right_wleft_bin,s23left_wleft_bin,s23right_wleft_bin,thetaleft_bin,thetaright_bin,alphaleft_bin,alpharight_bin,s12gen,s23gen,thetagen,alphagen,sigma_wleft_q2right[i],i);

interpol_fedotov(4,Q2left_bin,Wleft_bin,s12left_wleft_bin,s12right_wleft_bin,s23left_wleft_bin,s23right_wleft_bin,thetaleft_bin,thetaright_bin,alphaleft_bin,alpharight_bin,s12gen,s23gen,thetagen,alphagen,sigma_wleft_q2left[i],i);
};

for (Short_t i=0;i<6;i++){
interpol_fedotov(2,0,0, Wleft_bin, Wright_bin, Q2left_bin, Q2right_bin, 0,  0, 0,  0, Wgen, Q2gen, 0., 0.,sigma_final[i], i);
};

//We get explicitly different sigmas from the array
 sigma_t_final = sigma_final[0];
 sigma_l_final = sigma_final[1];
 sigma_c2f_final = sigma_final[2];
 sigma_s2f_final = sigma_final[3];
 sigma_cf_final = sigma_final[4];
 sigma_sf_final = sigma_final[5];

};
