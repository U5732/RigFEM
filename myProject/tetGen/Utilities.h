#pragma once


class Utilities
{
public:
	Utilities(void);
	~Utilities(void);

	static void vegaSparse2Eigen(const SparseMatrix& src, EigSparse& tar, int nCols = -1);
	
	// ��vega�����ĳ���Ӿ���ת��Eigen����rowID��colIDָ��ѡ�е��к���
	static void vegaSparse2Eigen( const SparseMatrix& src, const std::vector<int>& rowID, const std::vector<int>& colID, EigSparse& tar);

	// ��Eigen����д���ַ���
	template<class EigMatrix>
	static std::string toString(const EigMatrix& mat, const char* name)
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
			str += ";\n";
		}
		str += "\n];\n";
		return str;
	}

	// v = [v1 v2]
	static void mergeVec(const EigVec& v1, const EigVec& v2, EigVec& v);
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


	static void testMath();
};