// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"


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
#include "Eigen/QR"
#include "Eigen/SuperLUSupport"

typedef Eigen::SparseMatrix<double> EigSparse;
typedef Eigen::MatrixXd				EigDense;
typedef Eigen::VectorXd				EigVec;

// ����������
#define TETLIBRARY
#include "tetgen.h"

#define PRINT_F(format,...)					 printf(format, ##__VA_ARGS__)

// �����ļ�
#include "Utilities.h"
#include "CorotationalLinearFEMWrapper.h"
#include "rig.h"
#include "NewtonSolver.h"
#include "StatusRecorder.h"
#include "FEMSystem.h"

