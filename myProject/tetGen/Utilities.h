#pragma once

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#define CLAMP_INT(minValue,maxValue,value) (MAX(int(minValue),MIN(int(value),int(maxValue))))
#define CLAMP_FLOAT(minValue,maxValue,value) (MAX(float(minValue),MIN(float(value),float(maxValue))))
#define CLAMP_DOUBLE(minValue,maxValue,value) (MAX(double(minValue),MIN(double(value),double(maxValue))))

namespace RigFEM
{

class Utilities
{
private:
	typedef Eigen::Triplet<double> Tri;
public:
	// matrix 
	// ��vega����ת��Eigen����
	static void vegaSparse2Eigen(const SparseMatrix& src, EigSparse& tar, int nCols = -1);	
	// ��vega�����ĳ���Ӿ���ת��Eigen����rowID��colIDָ��ѡ�е��к���
	static void vegaSparse2Eigen( const SparseMatrix& src, const std::vector<int>& rowID, const std::vector<int>& colID, EigSparse& tar);
	static void double2EigenDiag(const double* m, int n, EigSparse& diag);
	template<class MatrixType>
	static double maxError(const MatrixType& m0, const MatrixType& m1)
	{
		MatrixType dm = m0 - m1;
		double error = 0;
		for (int r = 0; r < dm.rows(); ++r)
		{
			for (int c = 0; c < dm.cols(); ++c)
			{
				double absVal = abs(dm.coeff(r,c));
				error = error > absVal ? error : absVal;
			}
		}
		return error;
	}

	// ��Eigen����д���ַ���
	template<class MatrixType>
	static std::string matToString(const MatrixType& mat, const char* name)
	{
		std::string str;
		str += (string(name) + "=[\n");
		char digitStr[50];
		for (int ithRow = 0; ithRow < mat.rows(); ++ithRow)
		{
			for (int ithCol = 0; ithCol < mat.cols(); ++ithCol)
			{
				sprintf(digitStr, "%lf ", mat.coeff(ithRow, ithCol));
				str += digitStr;
			}
			str += "\n";
		}
		str += "];\n";
		return str;
	}

	// parse matlab matrix
	// actual valid str range is [begPos, endPos)
	static bool stringToDense(const std::string& str, int begPos, int& endPos, EigDense& mat, std::string& matName);
	// parse matlab matrix file(.m)
	static bool fileToDense(const char* fileName, std::map<std::string, EigDense>& denseMap);
	static bool eigDense2Sparse(const EigDense& denseMat, EigSparse& sparseMat);
	static bool kronecker3X( EigSparse& src, EigSparse& dst );
	static bool eigDense2Vec(const EigDense& denseMat, EigVec& vec);

	
	// vector functions
	template<class VectorType>
	static std::string vecToString( const VectorType&vec, const char*name )
	{
		std::string str;
		str += (string(name) + "=[");
		char digitStr[50];
		for (int i = 0; i < vec.size(); ++i)
		{
			double val = vec[i];
			sprintf(digitStr, "%lf ", val);
			str += digitStr;
		}
		str += "];\n";
		return str;
	}

	
	template<class VectorType>
	static bool	saveVector(const VectorType& vec, const char* fileName)
	{
		std::ofstream file(fileName);
		if (file)
		{
			int length = vec.size();
			file << length;
			for (int i = 0; i < length; ++i)
			{
				file << ' ' << vec[i];
			}
			file.close();
			return true;
		}
		return false;
	}
	template<class VectorType>
	static bool	loadVector(VectorType& vec, const char* fileName)
	{
		std::ifstream file(fileName);
		if (file)
		{
			int length = 0;
			file >> length;
			if (length <= 0)
				return false;

			vec.resize(length);
			for (int i = 0; i < length; ++i)
				file >> vec[i];
			return true;
		}
		return false;
	}
	// v = [v1 v2]
	static void mergeVec(const EigVec& v1, const EigVec& v2, EigVec& v);

	static void transformBBox(	const double srcMin[3], 
								const double srcMax[3], 
								const double mat[4][4], 
								double dstMin[3], 
								double dstMax[3]);

private:
	static int  skipMatlabComment(const std::string& str, int begPos);
	static std::string s_whiteSpace;
};

class MathUtilities
{
public:
	static double absSin(double t)
	{
		return 3 * abs(sin(t));
	}
	static double zero(double t)
	{
		return 0;
	}
	static double linear(double t)
	{
		return t;
	}

	// �ҳ���ֵ���κ���f����ֵ��x,xδ����������
	// a   > 0
	// f0  = f(0)
	// df0 = f'(0) < 0 
	// fa  = f(a)
	static inline bool findQuadricMin(double f0, double df0, double a, double fa, double& xMin, double& fxMin)
	{
		double coefA = (fa - f0 - a * df0) / (a * a);
		if (coefA < 0)
			return false;
		double coefB = df0;
		double coefC = f0;
		double x = -coefB / (2 * coefA);
		xMin = x;
		fxMin= coefA * x * x + coefB * x + coefC;
		return true;
	}

	// �ҳ���ֵ���κ���f�ļ�Сֵ��
	// a0,a1 > 0
	// f0  = f(0)
	// df0 = f'(0)  < 0
	// fa0 = f(a0)
	// fa1 = f(a1)
	// �����������ĵ㣬����true�����򷵻�false
	static inline bool findCubicMin0(	double f0, double df0, double a0, double fa0, double a1, double fa1, 
										double& xMin, double& fxMin)
	{
		double a02 = a0 * a0;
		double a12 = a1 * a1;
		double a03 = a02 * a0;
		double a13 = a12 * a1;

		double devisor = a02*a12*(a1 - a0);
		double v0 = fa1 - f0 - df0 * a1;
		double v1 = fa0 - f0 - df0 * a0;

		double coefA = (a02 * v0 - a12 * v1) / devisor;
		double coefB = (-a03* v0 + a13 * v1) / devisor;
		double coefC = df0;
		double coefD = f0;

		double r    = coefB * coefB - 3 * coefA * df0;
		if (r < 0)
			return false;
		xMin  = (-coefB + sqrt(r)) / (3 * coefA);
		fxMin = coefD + xMin * (coefC + xMin * (coefB + xMin * coefA));
		return true;
	}

	// �ҳ���ֵ���κ���f�ļ�Сֵ��
	// a0,a1 > 0
	// fa0  = f(a0)
	// fa1  = f(a1)
	// dfa0 = f'(a0)
	// dfa1 = f'(a1)
	static inline bool findCubicMin( double a0, double fa0, double dfa0, 
									 double a1, double fa1, double dfa1,
									 double& xMin, double& fxMin)
	{
		// ӳ�䵽��һ������g(x), �˺����������ص�
		// f(x) = g((x-a1)/(a0 - a1))
		// f(a1)   = g(0)
		// f(a0)   = g(1)
		// f'(a1)  = g'(0) / (a0 - a1)
		// f'(a0)  = g'(1) / (a0 - a1)
		double scale = a0 - a1;
		double dg0   = dfa1 * scale;
		double dg1   = dfa0 * scale;
		double g0    = fa1;
		double g1    = fa0;

		// ����g(x) = coefA * x^3 + coefB * x^2 + coefC * x + coefD
		double coefA = dg1 - 2*g1 + dg0 + 2 * g0;
		double coefB = 3*g1 - 3*g0 - 2*dg0 - dg1;
		double coefC = dg0;
		double coefD = g0;

		// ����g(x) �ļ�Сֵ��
		double delta = coefB * coefB - 3 * coefA * coefC;
		if (delta < 0)
			return false;
		double gxMin = (-coefB + sqrt(delta)) / (3 * coefA);
		double gMin  = coefD + gxMin*(coefC + gxMin*(coefB + gxMin* coefA));

		// ӳ���ԭ����ֵ
		xMin = a1 + gxMin * scale;
		fxMin= gMin;
		return true;
	}

	// ���������ţ�ʹ��Ԫ��������ֵ������maxElement
	static void clampVector( EigVec& v, double maxElement );

	static Vec3d computeNormal(const Vec3d& v0, const Vec3d& v1, const Vec3d& v2);

	static void testMath();
};

}