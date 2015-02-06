#pragma once


class RigSimulationNode: public MPxLocatorNode
{
public:
	enum SimulationType
	{
		SIM_STANDARD = 0,
		SIM_SKIN     = 1
	};
	RigSimulationNode(void);
	virtual ~RigSimulationNode(void);
	void postConstructor();

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const{return true;}
	virtual MBoundingBox boundingBox() const;

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator();
	static  MStatus		initialize();

	static MTypeId		m_id;
	static const char*  m_nodeName;

	// ����Plug������
	static const char*	paramLongName(){return m_paramName[0];}
	static const char*	meshLongName(){return m_meshName[0];}
	static const char*  transformLongName(){return m_transformMatrixName[0];}
	static const char*  initParamLongName(){return m_initParamName[0];}

	// ģ����غ���
	void				clearRig();
	bool				resetRig();
	bool				stepRig();					// ��̬ģ��
	bool				staticStepRig();			// ��̬ģ��
	MStatus				setParamPlug( const double* param, int nParam);


	// ���ڲ���
	void				testRig();	
	bool				testHessian(double noiseN, double noiseP);					// ���Ե�ǰ֡��Hessian
	bool				testGrad(double noiseN, double noiseP);

	// ��������
	bool				saveSimulationData(const char* fileName);
private:
	
	int					getNumParam();			// ��ȡ��ǰ��Ч�Ĳ�������
	MMatrix				getMatrix();
	bool				getInitStatus(RigFEM::RigStatus& s);
	bool				getInitParam(EigVec& p);
	int					getCurFrame();
	MStatus				setParamToInit();		// ���ò���Ϊ��ʼ����
	void				drawIcon();
	
	bool				updateDeriStepSize();	// �������޲��̵�������
	bool				updateTerminationCond();// ������ֹ����

	MBoundingBox		m_box;

	static MObject		m_initParam;			// ������ʼֵ
	static MObject		m_param;				// ����ֵ
	static MObject		m_mesh;					// ��������
	static MObject      m_transformMatrix;		// mesh transform matrix
	static MObject		m_tetEdgeRatio;			// �����񻯲�����������߱���
	static MObject		m_tetMaxVolume;			// �����񻯲�����������������
	static MObject		m_youngModulus;			// ����ģ��
	static MObject		m_nu;					// ���Ʋ�ͬ��������໥Ӱ��Ĳ���
	static MObject		m_density;				// �ܶ�
	static MObject		m_timeStep;				// ʱ�䲽��

	// ţ�ٷ�����
	static MObject		m_deriStep;				// ������ʱ�����޲��̴�С
	static MObject		m_maxIter;				// ����������
	static MObject		m_minStepSize;			// ��������С������������С�ڴ�ֵ����ֹ����
	static MObject		m_minGradSize;			// ��С���ݶ�ֵ�����ݶ�С�ڴ�ֵ����ֹ����
	static MObject		m_simType;				// ģ���㷨ѡ��
	static MObject		m_weightPath;			// Ȩ��·��
	static MObject		m_maxParamStep;			// ÿ�ε����������������


	static const char*  m_initParamName[2];
	static const char*  m_paramName[2];
	static const char*  m_meshName[2];
	static const char*  m_transformMatrixName[2];
	static const char*	m_tetEdgeRatioName[2];			// �����񻯲�����������߱���
	static const char*	m_tetMaxVolumeName[2];			// �����񻯲�����������������
	static const char*	m_youngModulusName[2];			// ����ģ��
	static const char*	m_nuName[2];					// ���Ʋ�ͬ�������Ӱ��̶ȵĲ���
	static const char*	m_densityName[2];				// �ܶ�
	static const char*	m_timeStepName[2];				// ʱ�䲽��
	static const char*  m_deriStepName[2];				// ������ʱ�����޲��̴�С
	static const char*  m_maxIterName[2];				// ����������
	static const char*  m_minStepSizeName[2];			// ��������С������������С�ڴ�ֵ����ֹ����
	static const char*  m_minGradSizeName[2];			// ��С���ݶ�ֵ�����ݶ�С�ڴ�ֵ����ֹ����
	static const char*  m_simTypeName[2];				// ģ���㷨ѡ��
	static const char*	m_weightPathName[2];			// Ȩ��·��
	static const char*	m_maxParamStepName[2];			// ÿ�ε����������������

	SimulationType		m_simTypeFlag;

// 	RigFEM::GeneralRig*	m_rig;					// ��װ�˽ڵ���ֵ����	
// 	RigFEM::RiggedMesh*	m_rigMesh;				// fem ����
// 	RigFEM::NewtonSolver* m_solver;				// �����
// 	RigFEM::StatusRecorder m_recorder;			// ��¼���״̬

	RigFEM::SimulatorBase*	m_simulator;				// ģ����
};
