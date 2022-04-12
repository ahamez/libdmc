// $Author: ahamez $
// $Revision: 711 $
// $Date: 2006-08-29 15:05:32 +0200 (Tue, 29 Aug 2006) $

#include "gspn_fact.hh"
#include "gspn.hh"
#include <sstream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

using namespace std;

namespace greatspn
{

//////////////////////////////////////////////////////////////////////

void 
gspn_model_factory::set_options(const string & ppath)
{
  path = ppath;
  nb_model=0;
}

//////////////////////////////////////////////////////////////////////

dmc::rg::abstract_model* 
gspn_model_factory::create_model()
{
  stringstream mypath_stream ;
  char cmd[256];
  mypath_stream << "./thread" << ++nb_model << "/" ;
  
  std::string mystr = mypath_stream.str();
  

  const char * mypath = mystr.c_str();
  if ( access (mypath,F_OK) ) {
    // path does not exist : create
    mkdir(mypath,S_IREAD|S_IWRITE|S_IEXEC);   
  }
  sprintf(cmd,"cp ./libgspndmcSRG.so %s \n", mypath); 
//  printf ("run : %s \n",cmd);
  system(cmd);
  
  
  sprintf(cmd,"cp %s.def  %s \n", path.c_str(), mypath ); 
//  printf ("run : %s \n",cmd);
  system(cmd);
  sprintf(cmd,"cp %s.net  %s \n", path.c_str(), mypath ); 
//  printf ("run : %s \n",cmd);
  system(cmd);

  
  stringstream libpath ;
  libpath << mypath << "libgspndmcSRG.so" ;

  sprintf (cmd,"%s",path.c_str());
  char * cq = cmd;
  for (char * cp = cmd ; *cp ; cp ++) {
    if ( *cp == '/' ) 
      cq = cp +1;
  }
  char modelpath [256];
  sprintf (modelpath,"%s%s",mypath_stream.str().c_str(),cq);

  void * libHandle = dlopen(libpath.str().c_str(),RTLD_NOW);
  if (!libHandle) {
    fputs (dlerror(), stderr);
    exit(1);
  } 
 
  
  return new  greatspn::greatspn(modelpath,libHandle);
}

//////////////////////////////////////////////////////////////////////

}
