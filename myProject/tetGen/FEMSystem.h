#pragma once

using namespace std;
//#define SHOW_TETGEN_RESULT		// �Ƿ���ʾtetgen���ɵ�out����
#define SHOW_TETMESH_RESULT			// �Ƿ���ʾVega������������
namespace RigFEM
{

enum RigControlType
{
	CONTROL_NONE			= (0x1),			// �������κο�����
	CONTROL_EXPLICIT_FORCE	= (0x1) << 1,		// ��ÿһ֡���Ĺ���������ͬ�Ŀ�����
	CONTROL_IMPLICIT_FORCE	= (0x1) << 2		// ��ÿһ֡���Ĺ������ÿɱ�Ŀ�����
};
enum AxisType
{
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2
};
struct MeshDispConfig
{
	MeshDispConfig();
	void setDefault();

	static void beginColorMaterial();
	static void endColorMaterial();

	double m_extForceDispFactor;

	bool   m_showEdge;
	bool   m_showPoint;
	bool   m_showBBox;
	bool   m_showMaterial;

	double m_minMaterialFactor;
	double m_maxMaterialFactor;
	AxisType m_axis;
	double m_axisFactor;

	static unsigned char  s_colorRuler[];
};
class RiggedMesh :public ObjectFunction
{
public:
	RiggedMesh(void);
	~RiggedMesh(void);
 
	void init();
	// �ñ��������rig�����ʼ����rig���ⲿ�����ͷ�
	bool init( tetgenio& surfMesh, RigBase* rig, double maxVolume = 1, double edgeRatio = 2 , double youngModulus = 1e6, double nu = 0.45, double density = 1000);
	void clear();
	void show();
	bool showStatus( RigStatus& s, const MeshDispConfig& config , double* bbox = NULL);
	void computeRig();

	// ���ּ��㺯��
	// �������״̬x = [n��p] �Լ�ʱ�����param�µĺ���ֵ���Լ��ݶ�
	bool computeValueAndGrad(const EigVec& x, const EigVec& param, double* v, EigVec* grad);
	// ���ݸ������ڲ����ƫ����n�Լ�����ֵp���㺯��ֵ���ݶ��Լ�Hessian����
	double computeValue(const EigVec& n, const EigVec& p, double t);
	bool computeGradient(const EigVec& n, const EigVec& p, double t, EigVec& gn, EigVec& gp);
	bool computeHessian(const EigVec& n, const EigVec& p, double t, EigSparse& Hnn, EigDense& Hnp, EigDense& Hpn, EigDense* pHpp = NULL);		// pHpp ��ΪNULL��������
	// ����������ȷ�������λ�ã������ڲ�������ƽ���λ��
	// initN����ָ����ʼ����λ��
	bool computeStaticPos(const EigVec& p, double t, EigVec& q, int maxIter = 20, const EigVec* initQ = NULL);

	// ���� A = [T*f1, T*f2, ..., T*fn]
	// ���� T �ǲ���ָ���Ľ�ά���� �ߴ�Ϊ��ά��x����Ԫ�������ɶ���
	// f1 ... fn �Ǹ�������Ԫ������������Ĺ���
	// ����q�Ǹ������λ��
	void computeReducedForceMatrix(	const EigVec& q, const EigDense& T, EigDense& A);
	void computeReducedHessianMatrix(const EigVec& q, const EigDense& T, EigDense& A);
	bool loadElementMaterialFactor(EigVec& factor);
	bool clearElementMaterialFactor();
	typedef vector<int> NeighIdx;
	const vector<NeighIdx>& getElementAdjList()const{return m_eleAdjList;}

	// ����״̬����
	// ��õ�ǰ�������ɶȵ�״̬
	void getDof(EigVec& n, EigVec& p);
	// ���¸������ɶȵ�״̬��ͬʱ���µ��ж���λ�á��ٶȡ����ٶ�
	void setDof(EigVec&n, EigVec&p, bool proceedTime = true);
	// ��������ÿһʱ�̣�ÿ���ڵ��������Ϊ�㶨
	bool setExternalForce(const EigVec&f);
	const EigVec& getExternalForce()const;
	// ����
	const EigVec& getMass()const;
	// ʱ��
	double getCurTime(){return m_t;}
	void setStepTime(double dt){m_h = dt;}
	double getStepTime()const{return m_h;}
	// ��������
	void getVertexPosition(EigVec& pos);
	// �����������������������������������б��ֲ���
	bool updateExternalAndControlForce();
	// ����
	void setRigControlType(RigControlType type);
	const EigVec& getControlTargetParam()const;
	const EigVec& getControlTargetVelocity()const;
	bool getControlTargetFromRigNode(EigVec& target);
	void setControlTarget(const EigVec& targetParam, const EigVec& targetParamVelocity);

	RigBase* getRigObj(){return m_rigObj;}

	// ���ط�װ��״̬
	RigStatus getStatus()const;
	bool setStatus(const RigStatus& s);

	const vector<int> getInternalPntIdx()const;
	const vector<int> getSurfacePntIdx()const;
	void getMeshPntPos(vector<double>& pnts)const;
	int getNTotPnt()const{return m_nTotPnt;}
	int getNSurfPnt()const{return m_nSurfPnt;}
	int getNIntPnt()const{return m_nIntPnt;}
	int getNParam()const{return m_rigObj->getNFreeParam();}

	// ���ֲ��Ժ���������ר��
	// ������ǰ������n,p,���Hessian�Ƿ���ȷ�ƽ�
	bool testHessian();
	// ���Ե�ǰ����������λ�Ƶ��ݶ��Ƿ��������
	bool testElasticEnergy();
	// ���Ժ���ֵ�����Ƿ���ȷ
	bool testValue();
	void testStep(double dt);

	// ���º�����maya mel�������
	// ���Ե�ǰ֡��Hessian
	bool testCurFrameHessian( RigStatus& lastFrame, RigStatus& curFrame, double noiseN = 1.0, double noiseP=1.0);
	bool testCurFrameGrad( RigStatus& lastFrame, RigStatus& curFrame, double noiseN = 1.0, double noiseP = 1.0);


protected:
	struct PntPair
	{
		Vec3d m_pnt;
		int   m_idx;
		bool  operator<(const PntPair& other)const;
	};

	struct OrderedPair
	{
		int   m_k1, m_k2, m_k3;
		OrderedPair(int k1, int k2, int k3):m_k1(k1), m_k2(k2), m_k3(k3){}
		bool  operator<(const OrderedPair& o)const;
	};

	bool findOriPoints(tetgenio& in, tetgenio& out);
	bool buildVegaData(double E = 1e6, double nu = 0.45, double density = 1000);
	bool allocateTempData();
	bool freeTempData();

	// ���ڲ���ƫ��n�Ͳ���p�������е��ƫ��q
	bool computeQ(const double* n, const double* p, double t, double* q);
	// ֻ���±�����pӰ��ĵ��λ��
	bool computeQ(const double* p, double t, double* q);
	// ���������ƫ����
	bool computeSurfOffset(const double* p, double t, EigVec& s);

	double computeValue(const EigVec& q, const EigVec& p);

	bool getN(RigStatus& s, EigVec& n);

	// ����ʩ�ӵ�ÿ���ڵ�Ŀ���������������������
	// �˺���������ʽ���ƣ�����ݸ�������ֵ�Ͳ����ٶ�ֱ�Ӽ����������
	bool computeControlForce( EigVec& controlForce, const EigVec& param, const EigVec& paramVelocity);
	// ����ʩ�ӵ�ÿ���ڵ�Ŀ�����������������������
	// �˺���������ʽ����,���Ե�ǰ�Ĳ���ֵΪ��һ֡�Ĳ���ֵ���Դ˼�������ٶ�
	bool computeControlForce( EigVec& generalForce, const EigVec& param );
	bool computeControlEnergy(const EigVec& param, double& energy);

	// ����Ӳ�����
	bool computeElementLaplacian(vector<NeighIdx>& adjList);

	// Tengen���ɵ���������������,��Щ����һ�����ɣ���ģ������г�����ֵ�ⲻ���޸�
	tetgenio		m_out;
	int*			m_surfPntIdx;						// ����m_in�Ķ��㣨��Ϊ����㣩��m_out������
	int*			m_intPntIdx;						// �¼���Ķ���(��Ϊ�ڲ���)��m_out������
	int*			m_surfDofIdx;						// �����������ɶȣ�xyzxyzxyz������
	int*			m_intDofIdx;						// �ڲ���������ɶȣ�xyzxyzxyz������
	int				m_nTotPnt;							// �ܵ���
	int				m_nIntPnt;							// �ڲ��㣨Ҳ���������˶��ĵ㣩����
	int				m_nSurfPnt;							// ����㣨Ҳ���Ǳ��������Ƶĵ㣩����
	int				m_nIntDof, m_nSurfDof;				// ���ɶ���
	int				m_nParam;							// ���ɲ�������
	TetMesh*		m_tetMesh;							// ����������
	vector<NeighIdx>m_eleAdjList;						// �����������У�������Ԫ�ص�������˹����

	// ����Vega���ݽṹ
	ModelWrapper*			m_modelwrapper;
	ForceModel*				m_forceModel;

	// rig����
	RigBase*				m_rigObj;					// rig ����

	// ����״̬��������Ϊ����*3, 
	// �ڵ��������У���Щֵ����Ϊ��һ֡��ֵ��ֱ����һ֡��ֵ���������Ÿ���
	EigVec					m_q,m_v,m_a;				// ÿ�������ƫ����,�ٶȣ����ٶ�
	EigVec					m_force;					// �ܵĵ���
	EigVec					m_extForce;					// �ܵ�����
	EigVec					m_param;					// ����rig �����ųɵ�����
	EigVec					m_paramVelocity;			// ����rig �����ٶ�

	EigVec					m_mass;						// ����
	double					m_h;						// ʱ�䲽��
	double					m_t;						// ��ǰʱ��

	// ���±���������״̬������������Ϊ�˱��ⷴ������ռ���������ʱ����
	SparseMatrix*			m_tangentStiffnessMatrix;	// ����նȾ���
	EigSparse				m_dFss, m_dFsn, m_dFns, m_dFnn;// ����նȾ�������ֿ�
	EigVec					m_intDofBuf, m_surfDofBuf, m_totDofBuf;	// �ڲ������ɶȣ���������ɶȣ������ɶ�

	// ���Ʋ���
	RigControlType			m_controlType;				// �Ƿ����������
	EigVec					m_controlForce;				// ���������������Ϊ������*3,
														// ����ʽ���Ƶ�����£���������������ǰ�ɵ�ǰ״̬����
														// ����ʽ���Ƶ�����£������������ڽ⣬���������в��ϱ仯
	// �ڵ��������У���Щֵ����Ϊ��һ֡��ֵ��ֱ����һ֡��ֵ���������Ÿ���
	EigVec					m_targetParam;				// Ŀ�����ֵ
	EigVec					m_targetParamVelocity;		// Ŀ������ٶ�
	EigVec					m_propGain;					// ������������
	EigVec					m_deriGain;					// ΢�ֿ�������
};



class FEMSystem
{
public:
	void init();
	void show(){m_mesh.show();}
	void saveResult(const char* fileName);
	void clearResult();
	void step();
	RiggedMesh				m_mesh;						// ���������
	PointParamSolver			m_solver;					// ţ�ٷ������
};

}