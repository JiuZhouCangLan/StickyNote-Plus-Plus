#pragma  once
#include "ExportConfig.hpp"
#include "xml/pugixml.hpp"
#include <string>

/*
* ��������ڴ������Դ洢������xml�ļ���������
* PROPERTY_CLASS_HEADER �����ڴ���ͷ�ļ�
* PROPERTY_CLASS_SOURCE ���ڴ���Դ�ļ��������ʵ����Property.cpp�ļ���
* ��������Ҫ���ʹ�ã��������뱨��
* type : Ҫ�洢����������
* className : �����ĺ�׺
* �������ʱ��Ҫ������������
* name : �������ƣ����������ͬһ��xml node������Ψһ�ģ������ڶ�ȡʱ���������֡�
* value :  Ĭ�ϵ�ֵ�����xml attribute ������ʱ����ȡʱ�Ὣ���ֵ��ΪĬ��ֵ��
*/
#define PROPERTY_CLASS_HEADER(type,className)\
class NOTE_FILE_EXPORT Property##className:public Property{\
public:	\
		Property##className(const std::string& name,const type value);\
		~Property##className() = default;\
		void loadValue(pugi::xml_node& node) override;\
		void saveValue(pugi::xml_node& node) override;\
		type getValue();\
		void setValue(const type& v);\
		void operator =(const type& v);\
private:\
	std::string name;\
	##type value;\
}\
/*
* �����������Ա���
*/
#define CREAT_PROPERTY(className,propertyName,propertyValue)\
	className propertyName(#propertyName,propertyValue);


namespace NF {
	class Property {
	public:
		virtual ~Property() {};
		virtual void loadValue(pugi::xml_node& node) =0;
		virtual void saveValue(pugi::xml_node& node) = 0;
	};

	PROPERTY_CLASS_HEADER(int,Int);
	PROPERTY_CLASS_HEADER(double,Double);
	PROPERTY_CLASS_HEADER(float,Float);
	PROPERTY_CLASS_HEADER(unsigned int, UInt);
	PROPERTY_CLASS_HEADER(std::string, String);
	PROPERTY_CLASS_HEADER(bool, Bool);
}