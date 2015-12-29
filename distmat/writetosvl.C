#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
//#include <limits>

using namespace std;

int
main(int argc, char** argv){

  cerr<<"reading lines from "<<argv[1]<<"..."<<std::endl;
  if(argc!=4) {
     cerr << "Usage: ./writetosvl [geom file] [color file] [svl file]"<<endl;
     exit(-1);
  };
  ifstream inf_point(argv[1]);
  ifstream inf_color(argv[2]);
  ofstream outf_svl(argv[3]);

  char line[225];

  if(!inf_point || !inf_color) {
    cerr << "ERROR: infile doesn't exist" << endl;
    cerr << endl;
    exit(-1);
  };
  if(!outf_svl) {
    cerr << "ERROR: outfile doesn't exist" << endl;
    cerr << endl;
    exit(-1);
  };

  int nc=0;
  inf_point>>nc;
  outf_svl<<nc<<endl;
  std::cout<<"number of curves: "<<nc<<std::endl;

  for(int i=0; i< nc; i++) {
    double dummy, dummy1, dummy2,dummy3,dummy4;
    int np = 0;
    inf_point>>np;
    outf_svl<<np<<endl;
    //cerr << "read: " << np << endl;

    inf_color.getline(line,225);

    //if(i>0) inf_point >> dummy; // skip
    for(int j=0; j<np; j++){
      double x,y,z;
      inf_point>>x>>y>>z;
     // >>dummy1>>dummy2>>dummy3>>dummy4;
      //cerr << "* read: " << x << " " << y << " " << z << endl;
      //outf_svl<<x<<" " << y<<" " << z << " " << line << " 1" << endl;
      outf_svl<<x<<" " << y<<" " << z << " " << line << endl;
      //cerr << "** read: " << line << endl;
    };
  }
  inf_point.close();
  outf_svl.close();

  std::cout<<"done!"<<std::endl;
}
