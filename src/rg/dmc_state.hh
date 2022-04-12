// $Author: ahamez $
// $Revision: 1023 $
// $Date: 2008-03-29 00:52:26 +0100 (Sat, 29 Mar 2008) $

#ifndef DMC_dmc_state_H_
#define DMC_dmc_state_H_

#include <cstring>

namespace dmc { namespace rg {
	
//////////////////////////////////////////////////////////////////////
	
class dmc_state
{

// attributes
public :

    unsigned long distance_to_initial_;
	
	// compressed data of the state, declared public (this is a struct)
	void* data;
	size_t size;


// methods
public:
	
	/// empty constructor. PLEASE USE new char [] to allocate data !!!!
	dmc_state()
		:
        distance_to_initial_(0),
		data(nullptr),
		size(0)
	{}

	// Constructor copies data
	dmc_state(void* d, size_t s) 
		: 
		distance_to_initial_(0),
		data(nullptr),
		size(s)
	{ 
		data = new char[size];
		memcpy (data, d, size);
	};
   
	virtual ~dmc_state()
	{
		delete[] (char *)data;
	};
  	
	
	/// Provides a built-in function of hash that user can override
	virtual 
	size_t 
	hash() const;
	
	/// Provides built-in functions of localization that user can override
	virtual 
	int 
	localization() const;
	
	inline
	bool 
	operator==(const dmc_state& as) const 
	{
		return as.size == this->size
			&& !memcmp(as.data,this->data,this->size);
	};
	
	inline
	void
	get_content(const void** d , size_t* s) const 
	{ 
		*d = this->data ; 
		*s = this->size ; 
	};
	
	inline
    unsigned long
    get_distance() const
    {
        return distance_to_initial_;
    }
    
	inline
    void
    set_distance(unsigned long dist)
    {
        distance_to_initial_ = dist;
    }
    
private:
		
	dmc_state(const dmc_state&);
	dmc_state& operator=(const dmc_state& as);

}; 

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
