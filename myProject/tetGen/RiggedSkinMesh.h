#pragma once

using namespace std;

namespace RigFEM
{
	class RiggedSkinMesh:public RiggedMesh
	{
	public:
		void setWeight(EigSparse& sparse);
		// ������init֮���ٱ�����
		bool setWeight(const char* weightFile);

		void getDof( EigVec& p );
		void setDof( EigVec& p, bool proceedTime = true );

		// ���ּ��㺯��
		// �������״̬x = p �Լ�ʱ�����param�µĺ���ֵ���Լ��ݶ�
		bool computeValueAndGrad(const EigVec& x, const EigVec& param, double* v = NULL, EigVec* grad = NULL);
		bool computeHessian(const EigVec&x, const EigVec& param, EigDense& H);
		// ������Ƶ�Hessian����
		// �ٶ��ſɱȾ���Ϊ����
		bool computeApproxHessian(const EigVec&x, const EigVec& param, EigDense& H);

		bool testCurFrameGrad(RigStatus& lastFrame, RigStatus& curFrame, double noiseP = 1.0);
		bool testCurFrameHessian( RigStatus& lastFrame, RigStatus& curFrame, double noiseP=1.0);
	protected:
		bool computeSkinQ(const double* p, double t, double* q);

		EigSparse				m_weightMat;				// Ȩ�ؾ��󣬴�СΪ���ڲ�����*3, �������*3��
		EigSparse				m_weightMatTran;			// Ȩ�ؾ����ת��
	};
}