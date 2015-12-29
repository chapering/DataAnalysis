/****************************************
 * File: Main.C
 * Last update:
 * Purpose: Test for Vilonava
 * Author: Cagatay Demiralp, >cad<
  ****************************************/
#include "BrownICurveDist.H"
#include "ICurveDist.H"
#include "points.H"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <limits>

using namespace std;

int
main(int argc, char** argv){

  std::cout<<"reading curves from "<<argv[1]<<"..."<<std::endl;
  std::ifstream cfile(argv[1]);

  if(!cfile){
    std::cout.flush();
    std::cout<<"cannot open curve file"<<argv[1]<<std::endl;
    exit(0);
  }

  cfile.setf(std::ios_base::fixed, std::ios_base::floatfield);
  cfile.precision(6);

  int nc=0;
  cfile>>nc;
  std::cout<<"number of curves: "<<nc<<std::endl;
  if(nc < 2){
    std::cout<<"at least two curves are needed for distance computation! \n"<<std::endl;
    exit(1);
  }

  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout.precision(6);

  std::vector< std::vector<Greal> > clist(nc);
  for(int i=0; i< nc; i++){
    int np = 0;
    cfile>>np;
    //cerr << "num of points: " << np << endl;
    std::vector<Greal> c(3*np);
    for(int j=0; j<np; j++){
      Greal x,y,z;
      cfile>>x>>y>>z;
      //cerr << "read: " << x << " " << y << " " << z <<  endl;
      c[3*j] =x; c[3*j+1] =y; c[3*j+2]=z;
      //ignore last three points
      //cfile>>x>>y>>z;
      //cerr << "*read: " << x << " " << y << " " << z <<  endl;
    }
    clist[i]=c;
  }
  cfile.close();
  std::cout<<"done!"<<std::endl;

  std::cout<<"computing distances between curves..."<<std::endl;
  std::vector< std::vector<Greal> > dmat(nc, std::vector<Greal>(nc, 0.0f));
  for(int i=0; i<nc; i++){
    for(int j=i+1; j<nc;j++){
      dmat[i][j]=  ICurveDist::MEANDIST(clist[i], clist[j], 0, 0);
      dmat[j][i] = dmat[i][j];
    }
  }
  std::cout<<"done!"<<std::endl;


  std::cout<<"writing distance matrix ("<<nc<<"x"<<nc<<") to "<<argv[2]<<"..."<<std::endl;
  std::ofstream dfile(argv[2]);
  if(!dfile){
    std::cout<<"cannot open distance file"<<argv[2]<<std::endl;
    exit(0);
  }
  dfile.setf(std::ios_base::fixed, std::ios_base::floatfield);
  dfile.precision(6);
  for(int i=0; i< nc; i++){
    for(int j=0; j< nc; j++){
      dfile<<dmat[i][j]<<" ";
    }
    dfile<<std::endl;
  }
  dfile.close();
  std::cout<<"done!"<<std::endl;
  return 0;
}

