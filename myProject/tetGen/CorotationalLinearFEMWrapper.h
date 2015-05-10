#pragma once

// ���� corotational ģ�ͼ��㵯�������ķ���
#define ENERGY_FROM_STIFFNESSMAT	1	// �������
#define ENERGY_FROM_MODEL			2	// ��������ȶ���ԭ��δ֪
#define ENERGY_METHOD				ENERGY_FROM_STIFFNESSMAT

class ModelWrapper
{
public:
	virtual ~ModelWrapper(){}
	virtual double	computeElasticEnergy(const double* u) = 0;
	// ���� A = [T*f1, T*f2, ..., T*fn]
	// ���� T �ǲ���ָ���Ľ�ά���� �ߴ�Ϊ��ά��x����Ԫ�������ɶ���
	// f1 ... fn �Ǹ�������Ԫ������������Ĺ���
	virtual void	computeReducedForceMatrix(	const double * vertexDisplacements, const EigDense& T, EigDense& A) = 0;
	
	// ���� A = [RH1, RH2, ..., RHn]
	// ���� RHi Ϊ R * Hi * R^T �󣬽�����������ų�һ����
	// HiΪÿ��Ԫ�ض�����նȾ���Ĺ���
	// ��Ҫ��stiffnessMatrixBuffer��������ʽԤ�ȸ����նȾ���ķ���Ԫ�ֲ�
	virtual void	computeReducedHessianMatrix(const double * vertexDisplacements, const EigDense& R, EigDense& A, SparseMatrix * stiffnessMatrixBuffer)=0;

	virtual bool	setElementMaterialFactor(EigVec& factor)=0;
	virtual double	getElementMaterialFactor(int ithElement)=0;
	virtual void	clearElementMaterialFactor()=0;
};

class CorotationalLinearFEMWrapper: public CorotationalLinearFEM, public ModelWrapper
{
public:
	CorotationalLinearFEMWrapper(TetMesh * tetMesh, int wrap = 2):m_wrap(wrap),CorotationalLinearFEM(tetMesh)
	{
	}

	~CorotationalLinearFEMWrapper(void);

	bool	setElementMaterialFactor(EigVec& factor);
	double	getElementMaterialFactor(int ithElement);
	void	clearElementMaterialFactor(){m_eleMatFactor = EigVec();}
	void   setWrap(int wrap){wrap = wrap;}

	virtual double computeElasticEnergy(const double* u);
	virtual void   ComputeForceAndStiffnessMatrix(double * u, double * f, SparseMatrix * stiffnessMatrix, int warp);

	// ���� A = [T*f1, T*f2, ..., T*fn]
	// ���� T �ǲ���ָ���Ľ�ά���� �ߴ�Ϊ��ά��x����Ԫ�������ɶ���
	// f1 ... fn �Ǹ�������Ԫ������������Ĺ���
	virtual void computeReducedForceMatrix(	const double * vertexDisplacements, 
											const EigDense& T, 
											EigDense& A);

	virtual void computeReducedHessianMatrix(	const double * vertexDisplacements, 
												const EigDense& reduceMat, 
												EigDense& reducedEleHessianMat, 
												SparseMatrix * stiffnessMatrixBuffer);

private:
	void ComputeForceAndStiffnessMatrixOfSubmesh(double * u, double * f, SparseMatrix * stiffnessMatrix, int warp, int elementLo, int elementHi);

	int				m_wrap;
	EigVec			m_eleMatFactor;			// ����ÿ��������Ԫ�ص�Ӳ��
};
