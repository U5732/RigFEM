#pragma once

class RigSimulationNode;
namespace RigFEM
{

	class RigException
	{
	public:
		enum Type
		{
			MESH_TOPOLOGY_CHANGED = 0x1,	// �������˷����ı�
		};
		RigException(Type type, const MString& info = ""): m_type(type), m_info(info){}
		Type getType()const{return m_type;}
		const MString& getInfo()const{return m_info;}
	private:
		Type m_type;
		MString m_info;
	};

	class GeneralRig : public RigBase
	{
	public:
		GeneralRig(RigSimulationNode* node, int nParam, int nPnts);
		~GeneralRig(void);

		virtual int getNPoints()const{return m_nPnts;}
		virtual int getNFreeParam()const{return m_nParam;}

		// ���㺯��
		virtual bool computeValue(double* result, const double* params = 0);
		
		// �ӽڵ��ȡ����ֵ��һ�����ڳ�ʼ���ڵ�
		MStatus		fetchParamFromNode();

		MStatus		getMesh(MObject& meshObj);
	private:
		// ��ֹ�ؼ�֡����
		void keyParam(int ithParam, KeyFrameFunc func);
		void unKeyParam(int ithParam);

		// ��ȡ���񶥵�,����Ԥ�ȷ���ÿռ�
		MStatus getMeshPoints(double* points);
		// ���ò���ֵ
		MStatus setParamPlug();

		RigSimulationNode*			m_node;
		int							m_nPnts;
	};
}
