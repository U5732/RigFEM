#pragma once

// ���� corotational ģ�ͼ��㵯�������ķ���
#define ENERGY_FROM_STIFFNESSMAT	1	// �������
#define ENERGY_FROM_MODEL			2	// ��������ȶ���ԭ��δ֪
#define ENERGY_METHOD				ENERGY_FROM_STIFFNESSMAT

class ModelWrapper
{
public:
	virtual ~ModelWrapper(){}
	virtual double computeElasticEnergy(const double* u) = 0;
};

class CorotationalLinearFEMWrapper: public CorotationalLinearFEM, public ModelWrapper
{
public:
	CorotationalLinearFEMWrapper(TetMesh * tetMesh, int wrap = 2):m_wrap(wrap),CorotationalLinearFEM(tetMesh)
	{
	}

	~CorotationalLinearFEMWrapper(void);

	void   setWrap(int wrap){wrap = wrap;}
	double computeElasticEnergy(const double* u);

private:
	int m_wrap;
};
