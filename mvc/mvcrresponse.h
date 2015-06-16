//mvcresponse.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_RRESPONSE_H
#define MVC_RRESPONSE_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef STRINGEX_H
#include "../stringex.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>
#include <memory>
#include <list>

#ifndef MVC_VIEW_H
#include "mvcview.h"
#endif

NAMESP_BEGIN

/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class RResponse
{	
public:
	typedef std::string view_t;
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	RResponse(const std::string &name, view_t &view)
		:_name(name)
		,_view(view)
	{
	}
	
	RResponse(std::string &&name, view_t &view)
		:RResponse(name,view)
	{}
	
	RResponse(const std::string &name)
		:_name(name)
	{}
	
	RResponse(std::string &&name)
		:RResponse(name)
	{}
	
	void view	(const view_t&		val){ _view   = val; }
	void action	(const std::string& val){ _action = val; }
		
	template<class T>
	void add_param(std::string &&key, const T& value)
	{ 
		_pack_item[key] = StringBuilder(value); 
	}


	template<class T>
	void add_param(const char* key, const T value)
	{
		_pack_item[key] = StringBuilder(value); 
	}
	
	void append_param()
	{
		_params.push_back(_pack_item);
		_pack_item.clear();
	}

	pack_t* Reply()
	{
		if(_params.size() > 0)
		{
			pack_t pck=new pack_t(_name, _view, _action);
			for(auto &i:_params)
				pck->append_param(i);
			
			_params.clear();
			return pck;
		}
			
		return nullptr;
	}
				
protected:
	view_t _view;
	typename pack_t::param_item_t _pack_item;
	params_pack_t _params;
	std::string _name;
	std::string _action;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};


/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class PushResponse
{
public:
	typedef std::string view_t;
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	PushResponse(const std::string &name, view_t& view)
		:_name(name)
		,_view(view)
	{
	}

	PushResponse(const view_t& view)
		:_name("pusher")
		, _view(view)
	{
	}
	
	PushResponse(view_t&& view)
		:PushResponse(view)
	{
	}

	PushResponse(const char* view)
		:PushResponse(view_t(view))
	{
	}
	
	PushResponse(const char* name, view_t& view)
		:PushResponse(std::string(name), view)
	{
	}

	PushResponse(std::string &&name, view_t &&view)
		:PushResponse(name,view)
	{}
	
	template<class T>
	void add_param(std::string &&key, const T& value)
	{ 
		_pack_item[key] = StringBuilder(value); 
	}


	template<class T>
	void add_param(const char* key, const T value)
	{
		_pack_item[key] = StringBuilder(value); 
	}
	
	void append_param()
	{
		_params.push_back(_pack_item);
		_pack_item.clear();
	}
		
	template<class Session>	
	void Push(const std::string& ip, const int port)
	{
		typename Session::session_ptr_t ss =Session::instance().get(ip, port);
		typename pack_t::pack_ptr_t  pck(new pack_t(_name, _view, "push") );
		for(auto &i:_params)
				pck->append_param(i);
		
		_params.clear();
		ss->PostOutput(pck);
	}
	
				
protected:
	view_t _view;
	typename pack_t::param_item_t _pack_item;;
	params_pack_t _params;
	std::string _name;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};

NAMESP_END;

#endif /*MVC_RRESPONSE_H*/
