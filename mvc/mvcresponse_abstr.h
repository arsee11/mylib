//file: mvcresponse_abstr.h

#ifndef MVCRESPONSE_ABSTR_H
#define MVCRESPONSE_ABSTR_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

class IView;
class IObject;

template<class OBJ>
class IFilter
{
public:
	OBJ obj_t;
public:
	virtual bool Filter(IView *base, OBJ *obj)=0;
	virtual ~IFilter(){}
};

class IResponse
{
public:
	virtual int Push() = 0;
	virtual ~IResponse(){}
};

NAMESP_END;
#endif /*MVCRESPONSE_ABSTR_H*/
