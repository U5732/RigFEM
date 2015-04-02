#pragma once

class Simple1DNoiseNode :
	public MPxNode
{
public:
	Simple1DNoiseNode(void);
	~Simple1DNoiseNode(void);

	static void* creator(){return new Simple1DNoiseNode;}
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  MStatus		initialize();
	
	static  MTypeId			m_id;
	static	const char*		m_nodeName;
private:
	double				computeNoise(double x, double minVal, double maxVal, double period, double phase);
	double				randDouble(unsigned x);

	// ������������,��Ϊ�������
	static MObject			m_minVal;
	static MObject			m_maxVal;
	static MObject			m_period;
	static MObject			m_phase;
	static MObject			m_outputVal;		// ���	
	// ����
	static MObject			m_input;			// �Ա���

	static const char*			m_minValName[2];
	static const char*			m_maxValName[2];
	static const char*			m_periodName[2];
	static const char*			m_phaseName[2];
	static const char*			m_outputValName[2];	// ���
	static const char*			m_inputName[2];		// �Ա���
};
