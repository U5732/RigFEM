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

	// ģ����غ���
	void				clearRig();
	bool				resetRig();
	bool				stepRig();
	MStatus				setParamPlug( const double* param, int nParam);

	// ���ڲ���
	void				testRig();
private:
	// ��ȡ��ǰ��Ч�Ĳ�������
	int					getNumParam();
	MMatrix				getMatrix();
	bool				getInitStatus(RigFEM::RigStatus& s);
	int					getCurFrame();

	MBoundingBox		m_box;

	static MObject		m_initParam;
	static MObject		m_param;
	static MObject		m_mesh;
	static MObject      m_transformMatrix;		// mesh transform matrix

	static const char*  m_initParamName[2];
	static const char*  m_paramName[2];
	static const char*  m_meshName[2];
	static const char*  m_transformMatrixName[2];

				
	RigFEM::GeneralRig*	m_rig;					// ��װ�˽ڵ���ֵ����	
	RigFEM::RiggedMesh*	m_rigMesh;				// fem ����
	RigFEM::NewtonSolver* m_solver;				// �����
	RigFEM::StatusRecorder m_recorder;			// ��¼���״̬		
};
