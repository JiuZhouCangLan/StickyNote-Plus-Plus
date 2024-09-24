#ifndef _LOG_BUFFER_H
#define _LOG_BUFFER_H

#include <streambuf>
namespace Log{
	class  LogBuffer :public std::streambuf{
	public:
		LogBuffer();
		~LogBuffer() = default;
	protected:
		int overflow(int c = EOF) override;
		int sync() override;
		virtual std::string getTag();
	private:
		std::string buffer;
	public:
		std::string getCurrentTime();
	};
	/*
		std::cerr �Ļ�������ͬ����������std::cout��һ�¡�
		std::cerr ÿһ�� << ��������ᴥ������ͬ������sync(),��������֮ǰ�������ȴ���cout�е�ͬ��������
		����������cout�����˴������������Ϊ�գ�����log�ļ�������κ����ݡ�
		** �����std::cerr ��ͬ����������ħ�ģ�Ī��Ϊ��ǰ�������� \n ����ʹ���� std::endl���Ž�����ͬ�����豸�С�
		
	*/
	class ErrorBuffer :public LogBuffer {
	public:
		ErrorBuffer();
		~ErrorBuffer() = default;

	protected:
		int overflow(int c = EOF) override;
		int sync() override;
		std::string getTag() override{
			return "Error";
		};
	private:
		//�жϻ��������Ƿ��пո�
		bool isEnd;
	};

	class Switch {
	public:
		Switch() = default;
		virtual ~Switch() {};
		
		virtual void on() = 0;
		virtual void off() = 0;
	protected:
		std::streambuf* oBuffer, * nBuffer;
	};
	class CerrSwitch:public Switch {
	public:
		CerrSwitch();
		~CerrSwitch();
		void on();
		void off();
	};

	class CoutSwitch :public Switch{
	public:
		CoutSwitch();
		~CoutSwitch();
		void on();
		void off();
	};
}
#endif
