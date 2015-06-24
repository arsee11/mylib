//file: jpack.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-10
//log	: modify 
//****************************

#include <utility>
#include "json/json.h"

#ifndef JPACK_H
#include "../jpack.h"
#endif

#ifndef STRINGEX_H
#include "../../stringex.h"
#endif

NAMESP_BEGIN

const char* Head0xff(const char *stream, size_t len, size_t *head_len)
{
	int head = 0;
	memset(&head, 0xff, 4);
	*head_len = 4;
	if( len > 4 )
	{
		for(size_t i=0; i<=len-4; ++i)
		{
			int tmp = *(int*)(stream+i);
			if(tmp == head)
				return stream+i+4;
		}
	}

	return nullptr;
}

JSerializer::stream_t JSerializer::Resolve(const pack_ptr_t &pck)
{
	Json::FastWriter wr;
	Json::Value root;
	root["source"] = pck->source();
	root["target"] = pck->target();
	root["action"] = pck->action();
	Json::Value param;
	Json::Value params;
	int k=0;
	for(auto &i : pck->params())
	{
		Json::Value param_item;
		for(auto &j : i)
		{
			Json::Value param;
			param["name"]  = j.first;
			param["value"] = j.second;
			param_item.append(param);
		}
		params["param"+t2str(k)] = param_item;
		++k;
	}
			
	root["params"] = params;
	return std::move(wr.write(root));
}

JSerializer::stream_t JSerializer::Resolve(const pack_t &pck)
{
	Json::FastWriter wr;
	Json::Value root;
	root["source"] = pck.source();
	root["target"] = pck.target();
	root["action"] = pck.action();
	Json::Value param;
	Json::Value params;
	int k = 0;
	for (auto &i : pck.params())
	{
		Json::Value param_item;
		for (auto &j : i)
		{
			Json::Value param;
			param["name"] = j.first;
			param["value"] = j.second;
			param_item.append(param);
		}
		params["param" + t2str(k)] = param_item;
		++k;
	}

	root["params"] = params;
	return std::move(wr.write(root));
}

size_t JSerializer::Header()
{
	return Head0xff(_head);
}



int JUnSerializer::Parse(pack_t &pck, stream_t &stream)
{
	Json::Reader rd;
	Json::Value root;
	
	if( rd.parse( stream, root, false) )
	{
		try{
			pck.source( root["source"].asString()	);
			pck.action( root["action"].asString()	);
			pck.target(  root["target"].asString()	);
			Json::Value& params = root["params"];
			for(int i=0; i<params.size(); i++)
			{
				Json::Value& param_item = params["param"+t2str(i)];
				pack_t::param_item_t ppitem;
				for(int j=0; j< param_item.size(); j++)
				{
					Json::Value& param = param_item[j];				
					ppitem[param["name"].asString()] = param["value"].asString();
				}
				pck.append_param(ppitem);
			}

			pck.status(true);
			return 1;
		}
		catch(exception& e){
			//pck.error(e.what());
			pck.status(false);
			return 0;
		}
	}
	
	return 0;
}

const char* JUnSerializer::Header(const char* stream, size_t len, size_t *head_len)
{
	return Head0xff(stream, len, head_len);
}

NAMESP_END

