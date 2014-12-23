// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

// ϵͳ
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <math.h>
#include <GL/GLUT.H>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>

// Vega
#include "vec3d.h"
#include "tetMesh.h"
#include "polarDecomposition.h"
#include "volumetricMeshLoader.h"
#include "volumetricMeshENuMaterial.h"
#include "corotationalLinearFEM.h"
#include "corotationalLinearFEMForceModel.h"
#include "generateMassMatrix.h"
#include "implicitBackwardEulerSparse.h"
#include "centraldifferencessparse.h"
#include "neoHookeanIsotropicMaterial.h"
#include "isotropicHyperelasticFEMForceModel.h"

// Eigen
#include "Eigen/sparse"
#include "Eigen/LU"
#include "Eigen/SuperLUSupport"

typedef Eigen::SparseMatrix<double> EigSparse;
typedef Eigen::MatrixXd				EigDense;
typedef Eigen::VectorXd				EigVec;

// ����������
#include "tetgen.h"

// �����ļ�
#include "Utilities.h"
#include "CorotationalLinearFEMWrapper.h"
#include "rig.h"
#include "NewtonSolver.h"
#include "FEMSystem.h"
