#pragma once
/***********************************************************************
   MEL ����: rigSimulate
   -init			-i	��ʼ��ģ������
   -step			-s	����һ��ģ�⣬����1����������0�����д���
   -stepStatic		-ss ���о�̬ģ�⣬����1����������0�����д���
   -stepEleGF		-rgf����ģ�⣬����¼ÿ��Ԫ�صĹ�����
   -saveGF			-sgf ���ֹ�����
   -loadEleMat		-lm ����Ԫ��Ӳ��
   -resetEleMat	    -rsm����Ĭ��Ӳ��
   -name			-n  ָ���ڵ�����
   -hessian			-h  ����Hessian����ָ������noiseN, noiseP
   -grad			-g  �����ݶ��Ƿ��뺯��ֵ�����Ǻ�
   -save			-sa  ����ģ����
   -numInternalPnt	-nip	����ڲ�����Ŀ
   -numSurfacePnt	-nsp    ��ñ������Ŀ
   -create			-c		����ģ��ڵ�
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
	static const char*				m_stepStaticFlag[2];
	static const char*				m_recordEleGFFlag[2];
	static const char*				m_saveFlag[2];
	static const char*				m_saveEleGFFlag[2];
	static const char*				m_loadEleMatFlag[2];
	static const char*				m_resetEleMatFlag[2];

	static const char*				m_nameFlag[2];
	static const char*				m_hessianFlag[2];
	static const char*				m_gradFlag[2];
	static const char*				m_intPntFlag[2];
	static const char*				m_surfPntFlag[2];
	static const char*				m_createFlag[2];
};
