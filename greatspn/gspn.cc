// $Author: ahamez $
// $Revision: 990 $
// $Date: 2007-06-25 18:43:15 +0200 (Mon, 25 Jun 2007) $

#include "gspndmclib.h"
#include "dmc.hh"
#include "greatspn/gspn.hh"
#include "greatspn/gspn_succiter.hh"

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

using namespace dmc;
using namespace dmc::rg;
using namespace std;

namespace greatspn {

//////////////////////////////////////////////////////////////////////
  
  greatspn::greatspn(const std::string &path, void *libHandle) 
  {
    char *error;
    /// link to distinct versions of libgreatspn
      my_dmcInitialize = (int (*)(int, char**)) dlsym(libHandle, "dmcInitialize");
    if ((error = dlerror()) != NULL) {
      fputs(error, stderr);
      exit(1);
    }
    my_dmcFinalize = (int (*)()) dlsym(libHandle, "finalize");
    if ((error = dlerror()) != NULL) {
      fputs(error, stderr);
      exit(1);
    }
    my_dmc_initial_state = (int (*)(dmcState**))dlsym(libHandle, "dmc_initial_state");
    if ((error = dlerror()) != NULL) {
      fputs(error, stderr);
      exit(1);
    }
    my_dmc_succ = (int (*)(dmcState*, dmcState**, size_t*))dlsym(libHandle, "dmc_succ");
    if ((error = dlerror()) != NULL) {
      fputs(error, stderr);
      exit(1);
    }
    //  extern long dmc_concrete_state_count ( const pdmcState s ) ;
    my_dmc_concrete_count =  (long (*) (dmcState *))dlsym(libHandle,"dmc_concrete_state_count");
    if ((error = dlerror()) != NULL) {
      fputs(error, stderr);
      exit(1);
    }

    my_dmc_print_state =  (int (*) (dmcState*, char**))dlsym(libHandle,"dmc_print_state");
    if ((error = dlerror()) != NULL) {
		fputs(error, stderr);
		exit(1);
    }
	

    const char * tab[2];
    tab[0] = "greatspn";
    tab[1] = path.c_str();
//    cerr << "launching greatsdpn on file : "<<tab[1] << " with pid :" <<getpid()<< endl;
    my_dmcInitialize(2,(char **)tab);
    pdmcState M0;
    my_dmc_initial_state(&M0);
  }
  
  //////////////////////////////////////////////////////////////////////
  
  rg::dmc_state*
  greatspn::get_initial_state()
  {
    pdmcState M0;
    my_dmc_initial_state(&M0);
    dmc_state * ret = new  dmc_state();
    ret->size = M0->length + sizeof(size_t);
    ret->data = new char [ret->size];
    * ((size_t*)ret->data) = M0->d_ptr;
    memcpy((char *)ret->data+sizeof(size_t),M0->data,M0->length);
    
    free (M0->data);
//    free(M0);

    return ret;
  }
  
  greatspn::~greatspn () {
    my_dmcFinalize();
  }

  dmc::rg::abstract_state_succ_iterator* greatspn::get_succ_iterator(dmc::rg::dmc_state* s) {
    pdmcState  tab_succ;
    size_t tab_succ_size;
    pdmcState cur = new dmcState;
    // cheating to avoid a copy, have the pdmcState point into the dmc_state
    cur->data = (char *)s->data + sizeof(size_t);
    cur->length = s->size - sizeof(size_t);
    cur->d_ptr = *( (size_t *) s->data );

//     cur->data = (char *) malloc(cur->length);
//     memcpy(cur->data,(char*) s->data + sizeof(size_t),cur->length);



    my_dmc_succ ( cur,&tab_succ,&tab_succ_size);
    
    delete cur;

    return new gspn_succ_iterator (tab_succ,tab_succ_size);

  }
  
}
