#ifndef CPP_LOG_H
#define CPP_LOG_H
#include "LogExport.hpp"
#include <memory>
namespace Log {
	class Switch;
	class LOG_EXPORT CppLog {
	public:
		~CppLog();
		static std::shared_ptr<CppLog> GetInstance();
		//������� ÿһ�Ժ�����Ӧһ��std�еı�׼����ӿ�
		void onCout();
		void offCout();
		void onCerr();
		void offCerr();

		//�ر��������
		void onAll();
		void offAll();
	private:
		CppLog();
	private:
		static std::shared_ptr<CppLog> instance;
		//�������
		std::shared_ptr<Switch> coutSwitch, cerrSwitch;
	};

}

#endif
