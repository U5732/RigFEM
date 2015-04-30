#pragma once
#include "StatusRecorder.h"

// ���岻ͬ�Ĺ����ݶ��㷨
#define FR_CG		0			// fletcher-reeves  ��
#define PR_CG		1			// polak-ribiere	��
#define CG_METHOD	FR_CG
namespace RigFEM
{
	// ����һά�����ĺ�������
	class ObjectFunction
	{
	public:
		virtual ~ObjectFunction(){}
		virtual bool computeValueAndGrad(const EigVec& x, const EigVec& param, double* v = NULL, EigVec* grad = NULL) = 0;

	protected:
	};

	// һά���������ڲ���
	class SimpleFunction:public ObjectFunction
	{
	public:
		virtual bool computeValueAndGrad(const EigVec& x, double* v = NULL, EigVec* grad = NULL);
	private:
		double computeFuncVal(double x);
	};
	
	class LineSearcher
	{
	public:
		LineSearcher(ObjectFunction* objFun = NULL, double initStep = 1.0, double maxStep = 10);
		~LineSearcher(void);
		enum Status
		{
			LS_NONE = 0,
			LS_WOLFE_DESCENT	= (0x1) << 1,
			LS_WOLFE_CURVATURE	= (0x1) << 2
		};
	
		bool setC(double c1, double c2);
		
		// һά��������
		// param ��Ŀ�꺯����һЩ����������ʱ�䡣�����������в������ֲ���
		// ������ֵ����0�� �ɹ��ҵ�����wolfe�����ĺ���
		// ������ֵ������0�� û���ҵ�����wolfe�����ĺ������ɸ���Status����ö�ٱ��������������
		// f0 df0Ϊ��ʼ��ĺ���ֵ�͵����������ṩ�������м���
		int lineSearch( const EigVec& x0, const EigVec& dx, const EigVec& param, double& aFinal,
						double* f0 = NULL, double *df0 = NULL);
		void setInitStep(double initStep){m_initStep = initStep;}
		void setMaxStep(double maxStep){m_maxStep = maxStep;}
		void setMaxZoomIter(int maxZoomIter){m_maxZoomIter = maxZoomIter;}
	private:

		void setOriginAndDir(const EigVec& x0, const EigVec& dx);

		// ���㺯��ֵ�ͷ�����ֵ��
		// ��f��dfΪNULL�������ж�Ӧ����
		bool computeValueAndDeri( const EigVec& x, double* f, double* df );
		// ��׼ȷ�ķŴ���
		// ��al��ahȷ��������֮�䣨al��һ��С��ah����һ������wolfe������a,���ҵ�������true
		// al Ϊһά���������У��������½������ĵ��к���ֵ��С��һ��
		// ah ʹ�� f'(al)(ah - al) < 0
		// dfal,dfah Ϊal ah��ĵ���������ǰû�У�������ΪNULL
		// ������ֵ����0�� �ɹ��ҵ�����wolfe�����ĺ���
		// ������ֵ����1�� û���ҵ�����wolfe�����ĺ���
		int zoom(	double al, double fal, double* dfal, 
					double ah, double fah, double* dfah, 
					double& a );
		// ������[a0,a1]��[a1,a0]�й���һ����Сֵ��
		// ������0����ʾ�ɹ��������ҵ�һ����Сֵ��
		// ������1����ʾʧ�ܣ���ʱ�����е�
		int guessMinPnt( double a0, double fa0, double *dfa0, double a1, double fa1, double *dfa1, double& aMin );

		EigVec			m_param;			// Ŀ�꺯������
		EigVec			m_dx;				// ��������
		EigVec			m_x0;				// ������ʼ��
		double			m_f0;				// ������ʼ��ĺ���ֵ
		double			m_df0;				// ������ʼ�㺯���ķ�����

		double			m_c1;				// wolfe����½�����ϵ��
		double			m_c2;				// wolfe��������ϵ��

		double			m_initStep;			// ��ʼ����
		double			m_maxStep;			// ��󲽳�
		int				m_maxZoomIter;

		ObjectFunction* m_objFunc;			// Ŀ�꺯������
	};

	class RiggedMesh;
	class RiggedSkinMesh;
	enum  RigControlType;

	class NewtonSolver
	{
	public:
		NewtonSolver(RiggedMesh* fem);
		virtual ~NewtonSolver(){}

		// ���ò���������ʱ�Ĳ���ֵ
		void setRestParam(EigVec& restParam);
		// ��ֹ����
		void setTerminateCond( int maxIter, double minStepSize, double minGradSize, int maxCGIter, double minCGStepSize);

		bool setInitStatus(const RigStatus&s);
		const RigStatus& getFinalStatus()const{return m_finalStatus;}
		virtual bool step()=0;
		virtual bool staticSolve(const EigVec& curParam){return false;}
		virtual bool staticSolveWithEleGF(const EigVec& curParam){return false;}

		void setIterationMaxStepSize(double maxStep);
		
		virtual void setControlType(RigControlType type){m_controlType = type;}

		StatusRecorder& getRecorder(){return m_recorder;}

		void setCurrentFrame(int curFrame){m_curFrame = curFrame;}
		int  getCurrentFrame(){return m_curFrame;}

		void setStaticSolveMaxIter(int maxIter);

		bool getRestStatus( RigStatus& status );

		// ���⴫�ݵ�״̬��¼��
		static const char*  const	s_dPName;					// ����ǰ��֡��ֵ����
		static const char*	const	s_initStepName;				// ������ʼ����
		static const char*	const	s_reducedElementGFName;			// ÿ��Ԫ�ضԲ����ռ�����Ĺ�����
	protected:
		// ��õ�ǰ֡��Ŀ����Ʋ�����Ŀ����Ʋ������ٶ�
		bool getCurCtrlParam(EigVec& tarParam, EigVec& tarParamVelocity);

		EigVec			m_restParam;

		RiggedMesh*		m_femBase;

		RigStatus	m_initStatus;		// ģ���ʼ״̬
		RigStatus	m_finalStatus;		// ģ�����״̬
		StatusRecorder	m_recorder;		// ״̬��¼��

		int			m_curFrame;			// ��ǰ֡��

		RigControlType m_controlType;	// �Ƿ�������

		// �����ǵ�������ֹ����
		int			m_maxIter;			// ����������			
		double		m_minStepSize;		// ��С��������������С�ڴ�ֵʱ������ֹ
		double		m_minGradSize;		// �ݶȳ���С�ڴ�ֵʱ������ֹ
		int			m_maxCGIter;		// �����ݶȷ�����������
		double      m_minCGStepSize;	// �����ݶȷ��У��ݶȳ���С�ڴ�ֵʱ������ֹ

		double		m_iterMaxStepSize;	// ������������󲽳�,ÿ�β��������������������ó�����ֵ

		int			m_maxStaticSolveIter;

	};
	class PointParamSolver:public NewtonSolver
	{
	public:
		PointParamSolver(RiggedMesh* fem = NULL);
		~PointParamSolver();

		void setMesh(RiggedMesh* fem);
		// ���㺯��
		bool step();
		bool staticSolve(const EigVec& curParam);
		// ��̬���,�������������Ԫ�Ĺ�������¼����
		bool staticSolveWithEleGF(const EigVec& curParam);

		void clearResult(){m_paramResult.clear();}
		void saveResult(const char* fileName, const char* paramName = "param");

		virtual void setControlType(RigControlType type);
	private:
		// ��������ֵ��������ƽ������������ڲ���λ�ã�
		// ����ֵ�����仯ʱ���ѱ���㣬�ڲ��㶼���ɲ����ĺ���
		// ������������ֵ�������ſɱȾ���
		bool computeStaticJacobian(const EigVec& curParam, EigDense& J);

		RiggedMesh*		m_fem;
		LineSearcher	m_lineSearch;

		// ��¼ģ������Ĳ����仯
		vector<EigVec>			m_paramResult;				// ���������������ؼ�֡�����Ĳ�������ģ������Ĳ���
	};

	class ParamSolver:public NewtonSolver
	{
	public:
		enum HessianType
		{
			HESSIAN_TRUE,
			HESSIAN_CONST_JACOBIAN
		};
		ParamSolver(RiggedSkinMesh* fem = NULL, HessianType type = HESSIAN_CONST_JACOBIAN);

		bool step();
		bool staticSolveWithEleGF(const EigVec& curParam);

		virtual void setControlType(RigControlType type);
	private:
		RiggedSkinMesh*	m_fem;
		LineSearcher	m_lineSearch;


		// ��¼ģ������Ĳ����仯
		vector<EigVec>			m_paramResult;				// ���������������ؼ�֡�����Ĳ�������ģ������Ĳ���
		HessianType				m_hessianType;
	};
}
