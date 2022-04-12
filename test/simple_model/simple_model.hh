// $Author: ahamez $
// $Revision: 997 $
// $Date: 2007-06-26 20:55:40 +0200 (Tue, 26 Jun 2007) $

// A very simple states space :-)
//
//	------->0
//	|	____|____
//  |   |   |   |
//	|  (a) (b) (c)
//  |   |   |   |
//	|	v	v	v
//	|	1	2	3<---
//	|___|	|___|	|
//				|	|
//				v	|
//				4	|
//			____|	|
//          |   |   |
//         (d) (e) (f)
//			|	|	|
//			v	v	|
//			5	6	|
//				|___|
//              |
//             (g)
//              |
//              v
//              7

#ifndef SIMPLE_MODEL_H_
#define SIMPLE_MODEL_H_

#include "dmc.hh"
#include "simple_model/state.hh"

#define NO_MORE_EVENTS '$'
#define SINGLE_EVENT '-'
#define FIRST_EVENT '#'

namespace simple_model {
	
//////////////////////////////////////////////////////////////////////
	
class simple_model : public rg::abstract_model
{

// attributes
private:
	
	static int cost_;
	static bool verbose_;
	
	
// methods
public:
	
	simple_model( int cost,
				  bool verbose);
	
	rg::dmc_state* 
	get_initial_state();
    
	dmc::rg::abstract_state_succ_iterator*
	get_succ_iterator(rg::dmc_state* s) ;
	
	static 
	state* 
	fire(state* s , char e);
	
	static 
	char 
	get_next_event( state* s , char e );
};

//////////////////////////////////////////////////////////////////////

} // namespace model


#endif
