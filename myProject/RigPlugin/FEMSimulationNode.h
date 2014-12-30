#pragma once


class RigSimulationNode: public MPxLocatorNode
{
public:
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
	bool				stepRig();
	MStatus				setParamPlug( const double* param, int nParam);

	// ���ڲ���
	void				testRig();

private:
	
	int					getNumParam();			// ��ȡ��ǰ��Ч�Ĳ�������
	MMatrix				getMatrix();
	bool				getInitStatus(RigFEM::RigStatus& s);
	int					getCurFrame();
	MStatus				setParamToInit();		// ���ò���Ϊ��ʼ����
	void				drawIcon();
	MBoundingBox		m_box;

	static MObject		m_initParam;
	static MObject		m_param;
	static MObject		m_mesh;
	static MObject      m_transformMatrix;		// mesh transform matrix
	static MObject		m_tetEdgeRatio;			// �����񻯲�����������߱���
	static MObject		m_tetMaxVolume;			// �����񻯲�����������������
	static MObject		m_youngModulus;			// ����ģ��
	static MObject		m_nu;					// ���Ʋ�ͬ��������໥Ӱ��Ĳ���
	static MObject		m_density;				// �ܶ�
	static MObject		m_timeStep;				// ʱ�䲽��
	static MObject		m_deriStep;				// ������ʱ�����޲��̴�С

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
	static const char*  m_deriStepName[2];					// ������ʱ�����޲��̴�С
				
	RigFEM::GeneralRig*	m_rig;					// ��װ�˽ڵ���ֵ����	
	RigFEM::RiggedMesh*	m_rigMesh;				// fem ����
	RigFEM::NewtonSolver* m_solver;				// �����
	RigFEM::StatusRecorder m_recorder;			// ��¼���״̬		
};
