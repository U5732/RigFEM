#pragma once
using namespace std;
namespace RigFEM
{
	// rig �ں�
	class RigBase
	{
	public:
		typedef double (*KeyFrameFunc)(double);
		RigBase(int nParam);
		virtual ~RigBase(void);

		// ����,
		virtual int  getNPoints()const=0;							// ���ض������

		int  getNParam()const{return m_nParam;}						// �����ܵĲ�������
		void getParam(double* params);

		// �ؼ�֡�����Ĳ���
		void keyParam(int ithParam, KeyFrameFunc func);
		void unKeyParam(int ithParam);

		// ���ɲ���
		virtual int  getNFreeParam()const;
		void setFreeParam(const double* params);
		void getFreeParam(double* params)const;;

		// ʱ��
		void setTime(double t);
		double		 getTime()const{return m_t;}

		// �������޲�����ʱ�Ĳ���
		void setDelta(double delta){m_delta = delta;}

		// �����Ǹ������㺯��, ���ݵ�ǰ�Ĳ�����ʱ�������ֽ��
		// ���㵱ǰ�����µ��������
		virtual bool computeValue(double* result, const double* params = 0) = 0;

		// ���޲��̼��㵱ǰ�����µ��ſɱȾ��� dS/dP 
		virtual bool computeJacobian(Eigen::MatrixXd& jacobian);

		// ���޲��̼��㵱ǰ�����µ� d2S / (dPi * dPj)  ����SΪ��������Pi��PjΪ��i��j������
		virtual bool computeJacobianDerivative(int i, int j, double* res);

		// ��������
		virtual bool computeExternalForce(const EigVec& pos, const EigVec& vel, const EigVec& m, 
			double time, EigVec& extForce, EigVec& surfForce){return false;}
		
		virtual bool getControlGain(EigVec& propGain, EigVec& deriGain){return false;}
		virtual bool getControlParams(EigVec& targetParam, EigVec& propGain, EigVec& deriGain){return false;}
		virtual bool getControlTarget(EigVec& targetParam){return false;}
	protected:
		double	m_delta;											// ���޲�����ʱ��΢Сλ��
		int     m_nParam;											// ��������
		double  m_t;												// ��ǰʱ��
		double* m_param;											// ��������
		KeyFrameFunc* m_keyFrameFunc;								// �ؼ�֡����
	};

	class TransformRig:public RigBase
	{
	public:
		TransformRig();
		void setInitPnts(double* pnts, int nPnts);

		int  getNPoints()const{return m_initPntList.size();}

		Vec3d getTranslation()const{return m_translation;}
		Vec3d getScale()const{return m_scale;}
		Vec3d getRotation()const{return m_rotate;}
		void setAllParam( const Vec3d& trans, const Vec3d& rotate, const Vec3d& scale );
		const vector<Vec3d>& getCurPnt()const{return m_curPntList;}
		const vector<Vec3d>& getInitPnt()const{return m_initPntList;}

		virtual bool computeValue(double* result, const double* params = 0);
		//void computeJacobian(Eigen::MatrixXd& jacobian);
		//void computeJacobianDerivative(int i, int j, double* res);

		static inline Vec3d rotateX(const Vec3d& v, double deg)
		{
			Vec3d res;
			res[0] = v[0];
			double c = cos(deg), s = sin(deg);
			res[1] = c * v[1] - s * v[2];
			res[2] = s * v[1] + c * v[2];
			return res;
		}
		static inline Vec3d rotateY(const Vec3d& v, double deg)
		{
			Vec3d res;
			res[1] = v[1];
			double c = cos(deg), s = sin(deg);
			res[2] = c * v[2] - s * v[0];
			res[0] = s * v[2] + c * v[0];
			return res;
		}
		static inline Vec3d rotateZ(const Vec3d& v, double deg)
		{
			Vec3d res;
			res[2] = v[2];
			double c = cos(deg), s = sin(deg);
			res[0] = c * v[0] - s * v[1];
			res[1] = s * v[0] + c * v[1];
			return res;
		}
	private:
		// ���ݵ�ǰ��ƽ�����Ų�������任��ĵ�
		void transform();

		void compute();
		void compute(const Vec3d& trans, const Vec3d& scl);

		vector<Vec3d>	m_initPntList;		// ��ʼ��λ�ã���ƽ�ƣ�ԭ��С��
		vector<Vec3d>   m_curPntList;		// ��ǰ�����µ��λ��

		// ��ǰrig ����
		Vec3d			m_translation;
		Vec3d			m_scale;
		Vec3d			m_rotate;
		Vec3d			m_localCenter;		// �ֲ�����ϵԭ��λ��
	};
}

