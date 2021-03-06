#if defined(_MSC_VER_)
#pragma once
#endif

#ifndef PERSISTENT_H
#define PERSISTENT_H

#include <map>
#include <regex>

using namespace std;

template<class Db>
class Persistent
{
	typedef Db db_t;
	typedef typename db_t::result_set_t db_res_t;
	typedef typename db_t::db_row_t db_row_t;
	
public:
	Persistent(db_t *db)
		:_db(db)
	{}

	template<class Obj>
	bool Load(Obj* obj, string id)
	{
		string select_params = obj->get_params();
		string table = obj->get_table();
		string id_name = obj->get_idname();
		string sqlstr = "select " + select_params +" from " +table +" where " + id_name +"='"+id+"'";

		db_res_t res( _db->ExecuteQuery(sqlstr.c_str()) );
		db_row_t row = res->Begin();
		if(row != res->End() )
		{
			obj->Load(row);
			return true;
		}

		return false;
	}

	//@filter where conditons
	template<class Obj>
	bool Load(const Obj* objtemp, typename Obj::objlist_t &objs, const string& filter)
	{
		string select_params = objtemp->get_params();
		string table = objtemp->get_table();

		string sqlstr = "";
		if( filter.empty()
		{
			sqlstr = "select " + select_params +" from " +table;
		}
		else
		{
			regx rgx("(=|<|>|\s+like\s+|\s+in\s+)");
			if( regex_match(filter, rgx) )
			{
				sqlstr = "select " + select_params +" from " +table + " where "+filter;
			}
			else 
				sqlstr = "select " + select_params +" from " +table + filter;
		}

		db_res_t res( _db->ExecuteQuery(sqlstr) );
		db_row_t row = res->Begin();
		size_t count = res->RowsCount();
		if( count == 0 ) 
			return false;

		for(; row != res->End(); ++row )
		{
			Obj *obj = Obj::Create(objtemp->get_type());
			obj->Load(row);
			objs.push_back( obj );
		}

		return true;
	}
	
	template<class Obj>
	bool Load(const Obj* objtemp, typename Obj::objlist_t &objs, const char* filter)
	{
		string str(filter);
		return Load(objtem, objs, str);
	}
	
	template<class Obj>
	bool Save(const Obj* obj)
	{
		string params = obj->get_params();
		string values = obj->get_values();
		string table = obj->get_table();

		string sqlstr = "insert into "+table+"("+params+") values("+values+")";
		if(_db->Update(sqlstr.c_str()) < 0 )
			return false;

		return true;
	}

	template<class T>
	bool Update(string& key, T value)
	{
		_key_values[key] = t2string(value);

		return true;
	}

	template<class T>
	bool Update(const char* key, T value)
	{
		return Update(string(key), value);
	}

	//@where conditons without the where word
	bool UpdateCommit(const string& table, const string& where)
	{
		if(_key_values.size() > 0)
		{
			string sqlstr = "update "+table+" set ";
			map<string, string>::iterator i = _key_values.begin();
			for(; i!= --_key_values.end(); ++i)
				sqlstr += i->first + "='" + i->second +"',";

			sqlstr += i->first + "='" + i->second+"'";

			if( !where.empty() )
				sqlstr += " where " + where;
			
			if( _db->Update( sqlstr.c_str() ) < 0 )
				return false;

			_key_values.clear();
		}
		
		return true;
	}

	bool Delete(const string& table, const string& where)
	{
		string sqlstr = "delete from "+table;

		if( !where.empty() )
				sqlstr += " where " + where;

		if( _db->Update( sqlstr.c_str() ) < 0 )
				return false;

		return true;
	}

private:
	db_t *_db;
	map<string, string> _key_values;
};

#endif /*PERSISTENT_H*/