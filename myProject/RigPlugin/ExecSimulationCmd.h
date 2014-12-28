#pragma once
/***********************************************************************
   MEL ����: rigSimulate
   -init			-i	��ʼ��ģ������
   -step			-s	����һ��ģ�⣬����1����������0�����д���
   -name			-n  ָ���ڵ�����
/************************************************************************/

class RigSimulateCmd: public MPxCommand
{
public:
	RigSimulateCmd(void);
	~RigSimulateCmd(void);

	MStatus doIt(const MArgList& args);

	static void* creator();

	static MSyntax newSyntax();
private:
	static const char*				m_initFlag[2];
	static const char*				m_stepFlag[2];
	static const char*				m_nameFlag[2];
};
