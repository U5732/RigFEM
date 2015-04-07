#pragma once


class RigSimulationNode: public MPxLocatorNode
{
public:
	enum SimulationType
	{
		SIM_STANDARD = 0,
		SIM_SKIN     = 1
	};
	enum DisplayType
	{
		DISP_SIM	 = 0,
		DISP_INIT    = 1
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
	bool				stepWithEleGF();			// ��̬ģ�⣬������ÿ��Ԫ�صĹ�����
	bool				saveGFResult(const char* fileName);
	bool				loadElementMaterial();
	bool				resetElementMaterial();

	MStatus				setParamPlug( const double* param, int nParam);

	// ���ڲ���
	void				testRig();	
	bool				testHessian(double noiseN, double noiseP);					// ���Ե�ǰ֡��Hessian
	bool				testGrad(double noiseN, double noiseP);

	// ��������
	bool				saveSimulationData(const char* fileName);
	bool				computeExternalForce( const EigVec& pos, const EigVec& vel, const EigVec& m, double time, EigVec& extForce , EigVec& surfForce);
	bool				getControlParams(EigVec& targetParam, EigVec& propGain, EigVec& deriGain);
	bool				getControlGain(EigVec& propGain, EigVec& deriGain);
	bool				getControlTarget(EigVec& targetParam);

	int					getNumInternalPnt();
	int					getNumSurfacePnt();
private:
	
	int					getNumParam();			// ��ȡ��ǰ��Ч�Ĳ�������
	MMatrix				getMatrix();
	bool				getInitStatus(RigFEM::RigStatus& s);
	bool				getInitParam(EigVec& p);
	int					getCurFrame();
	MStatus				setParamToInit();		// ���ò���Ϊ��ʼ����
	void				drawIcon();
	DisplayType			getDispType();
	
	bool				updateDeriStepSize();	// �������޲��̵�������
	bool				updateTerminationCond();// ������ֹ����

	// �ڵ�����
	void				printFieldData();
	MStatus				getInputForce( EigVec& fieldForce, EigVec& surfForce );
	MStatus				testField();

	// ��ʾ����
	RigFEM::MeshDispConfig getDisplayConfig();

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
	static MObject		m_dispType;				// ��ʾ��ʽ,ֱ����ʾģ�������ǳ�ʼ����
	static MObject		m_dispFemMesh;			// �Ƿ���ʾfem����
	static MObject		m_dispVertex;			// �Ƿ���ʾ����
	static MObject		m_dispEdge;				// �Ƿ���ʾ��
	static MObject		m_dispBBox;				// �Ƿ���ʾ��Χ��
	static MObject      m_fieldForceFactor;		// ���߳�������
	static MObject		m_time;					// �����ʱ�䣬���������ڵ���ֵ

	// ţ�ٷ�����
	static MObject		m_deriStep;				// ������ʱ�����޲��̴�С
	static MObject		m_maxIter;				// ����������
	static MObject		m_minStepSize;			// ��������С������������С�ڴ�ֵ����ֹ����
	static MObject		m_minGradSize;			// ��С���ݶ�ֵ�����ݶ�С�ڴ�ֵ����ֹ����
	static MObject		m_simType;				// ģ���㷨ѡ��
	static MObject		m_weightPath;			// Ȩ��·��
	static MObject		m_resultPath;			// ���·��
	static MObject		m_maxParamStep;			// ÿ�ε����������������

	// ����Ӧ����Ӳ��
	static MObject		m_GFResultPath;			// �������ı���·��
	static MObject		m_materialPath;			// �������Ӳ���ļ�
	static MObject		m_displayMaterial;		// �Ƿ���ʾ����Ӳ��
	static MObject		m_minFactor;			// ��ɫ��Χ
	static MObject		m_maxFactor;			// 
	static MObject		m_cutAxis;				// �и�����
	static MObject		m_cutRatio;				// �и����

	// �����ݶȷ�����
	static MObject		m_cgMinStepSize;		// ��������С������������С�ڴ�ֵ����ֹ����
	static MObject		m_maxCGIter;			// �����ݶȷ�����������

	// maya������
	static MObject		m_inputForce;			// ���������������ڲ������ڵ㣩
	static MObject		m_fieldData;			// ���������Ԫ�ڵ�״̬
	static MObject		m_fieldDataPosition;	// λ��������
	static MObject		m_fieldDataVelocity;	// �ٶ�������
	static MObject		m_fieldDataMass;		// ����������
	static MObject		m_fieldDataDeltaTime;	// ʱ��������

	// ��ײ
	static MObject		m_surfaceForce;			// �����ܵ�����
	static MObject		m_surfaceForceBitmap;	// ��¼������������Ƿ���Ч��λͼ
	static MObject      m_surfaceForceFactor;	// ������ǿ������
	static MObject		m_collisionParticle;	// ���ڴ�����ײ����n������״�ڵ�
	static MObject		m_collisionMesh;		// ����׷�ٵ���ײ����

	// PD�Զ����Ʋ���
	static MObject		m_controlType;			// �Զ����ƿ���
	static MObject		m_targetParam;			// ������״̬	
	static MObject		m_proportionalGain;		// ��������ǿ��
	static MObject		m_derivativeGainRatio;	// ΢�ֿ���ǿ�ȱ���

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
	static const char*  m_resultPathName[2];			// ���·��
	static const char*  m_GFResultPathName[2];			// ����������·��
	static const char*	m_maxParamStepName[2];			// ÿ�ε����������������
	static const char*  m_dispTypeName[2];
	static const char*  m_dispFemMeshName[2];			// �Ƿ���ʾfem����
	static const char*	m_dispVertexName[2];			// �Ƿ���ʾ����
	static const char*	m_dispEdgeName[2];				// �Ƿ���ʾ��
	static const char*	m_dispBBoxName[2];				// �Ƿ���ʾ��Χ��
	static const char*  m_cgMinStepSizeName[2];
	static const char*  m_maxCGIterName[2];				// �����ݶȷ�����������
	static const char*  m_inputForceName[2];			// ����������
	static const char*  m_fieldDataName[2];				// ���������Ԫ�ڵ�״̬
	static const char*  m_fieldDataPositionName[2];		// ������
	static const char*  m_fieldDataVelocityName[2];
	static const char*  m_fieldDataMassName[2];
	static const char*  m_fieldDataDeltaTimeName[2];
	static const char*  m_fieldForceFactorName[2];
	static const char*	m_controlTypeName[2];			// �Զ����ƿ���
	static const char*	m_targetParamName[2];			// ������״̬	
	static const char*	m_proportionalGainName[2];		// ��������ǿ��
	static const char*	m_derivativeGainRatioName[2];	// ΢�ֿ���ǿ��
	static const char*  m_surfaceForceName[2];
	static const char*  m_collisionParticleName[2];
	static const char*  m_collisionMeshName[2];
	static const char*	m_surfaceForceBitmapName[2];	// ��¼������������Ƿ���Ч��λͼ
	static const char*  m_surfaceForceFactorName[2];	// ������ǿ������
	static const char*  m_timeName[2];
	static const char*	m_displayMaterialName[2];		// �Ƿ���ʾ����Ӳ��
	static const char*	m_minFactorName[2];				// ��ɫ��Χ
	static const char*	m_maxFactorName[2];				// 
	static const char*	m_cutAxisName[2];				// �и�����
	static const char*	m_cutRatioName[2];				// �и����
	static const char*  m_materialPathName[2];			// �������Ӳ���ļ�


	SimulationType		m_simTypeFlag;

	RigFEM::SimulatorBase*	m_simulator;				// ģ����
};
