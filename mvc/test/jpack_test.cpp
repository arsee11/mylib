//test_jpack.cpp

#include "../jpack.h"
#include<iostream>



using namespace arsee;
using namespace std;

//-I/home/arsee_p/lib/jsoncpp-master/include/

//-l/home/arsee_p/lib/jsoncpp-master/lib/

//normal
void test_serializ1()
{
	cout<<"test_serializ1:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	pck.Param("ddd", "1111");
	pck.Param("eee", "2222");
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//no source
void test_serializ2()
{
	cout<<"test_serializ2:"<<endl;
	Jpack pck("", "bbb","ccc");
	pck.Param("ddd", "1111");
	pck.Param("eee", "2222");
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//unserial
void test_unserializ()
{
	cout<<"test_unserializ:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	pck.Param("ddd", "1111");
	pck.Param("eee", "2222");
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);

	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.Action()<<endl;
	cout<<"source:"<<npack.Source()<<endl;
	cout<<"target:"<<npack.Target()<<endl;
	for(auto &i:npack.Params())
		cout<<i.first<<"="<<i.second<<endl;
	
}

int main()
{
	test_serializ1();
	test_serializ2();
	test_unserializ();
	return 0;
}

