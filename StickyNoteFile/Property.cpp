#include "Property.h"

/*
* type : Ҫ�洢����������,������������
* name : �����ĺ�׺
* xmlType : ��Ӧxml���� to_xxx()���������ͣ����ڶ�ȡ
*/
#define PROPERTY_CLASS_SOURCE(type,className,xmlType)\
Property##className::Property##className(const std::string& name,const type v)\
{\
	this->name = name;\
	this->value = v;\
};\
void Property##className::saveValue(pugi::xml_node& node)\
{\
	node.append_attribute(name.c_str()) = value;\
};\
void Property##className::loadValue(pugi::xml_node& node)\
{\
		auto attr = node.attribute(name.c_str());\
	if(attr.empty())\
		return;\
	value = attr.as_##xmlType(value);\
};\
void Property##className::operator =(const  type& v)\
{\
	value = v;\
};\
void Property##className::setValue(const  type& v)\
{\
	value = v;\
};\
type Property##className::getValue()\
{\
	return value;\
};
/*
* PROPERTY_CLASS_SOURCE �ĸĽ��� ��Ҫ������Ͳ���ֱ������xml�洢����Ҫ��ת���������ʹ��
* trName : ת���������ƣ�������ʵ�����std::string ת char д��
*/
#define PROPERTY_CLASS_SOURCE_TR(type,className,xmlType,trName)\
Property##className::Property##className(const std::string& name,const type v)\
{\
	this->name = name;\
	this->value = v;\
};\
void Property##className::saveValue(pugi::xml_node& node)\
{\
	node.append_attribute(name.c_str()) = value.##trName();\
};\
void Property##className::loadValue(pugi::xml_node& node)\
{\
	auto attr = node.attribute(name.c_str());\
	if(attr.empty())\
		return;\
	value = attr.as_##xmlType(value.##trName());\
};\
void Property##className::operator =(const  type& v)\
{\
	value = v;\
};\
void Property##className::setValue(const  type& v)\
{\
	value = v;\
};\
type Property##className::getValue()\
{\
	return value;\
};

namespace NF {
	PROPERTY_CLASS_SOURCE(int,Int,int);
	PROPERTY_CLASS_SOURCE(float,Float,float);
	PROPERTY_CLASS_SOURCE(double,Double, double);
	PROPERTY_CLASS_SOURCE(unsigned int, UInt, uint);
	PROPERTY_CLASS_SOURCE_TR(std::string, String,string,c_str);
	PROPERTY_CLASS_SOURCE(bool, Bool, bool);
}
