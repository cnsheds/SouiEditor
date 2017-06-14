#pragma once


class ResManger
{
public:
	ResManger();
	~ResManger();

	void LoadUIResFromFile(SStringT strPath);
	void ReleaseUIRes();
	void SaveRes();

	pugi::xml_node GetResFirstNode(const SStringT tagname);

	void LoadUIRes();

	void LoadSkinFile();
	void LoadStringFile();
	void LoadColorFile();
	void LoadStyleFile();
	void LoadObjattrFile();

	void GetSubNodes(pugi::xml_node & parentNode, SStringT parentNodeName);

	static SStringT RemoveResTypename(const SStringT & resname);

	SStringT GetResPathByName(const SStringT & resname);

protected:
	void LoadResFileEx(SStringT & filepath, pugi::xml_document &xmlDoc, SStringT tagname);

public:
	SStringT m_strProPath;			// ������Դ�ļ��ĸ�Ŀ¼
	SStringT m_strUIResFile;		// uires.idx �����ļ���
	SStringT m_strInitFile;			// Init.xml �����ļ���

	SStringT m_strSkinFile;			// skin�����ļ���
	SStringT m_strStringFile;		// string�����ļ���
	SStringT m_strColorFile;		// color�����ļ���
	SStringT m_strStyleFile;		// Style�����ļ���
	SStringT m_strObjattrFile;		// Objattr�����ļ���

	pugi::xml_document m_xmlDocUiRes;			// uires.idx�ļ�xml doc
	pugi::xml_node m_xmlNodeUiRes;				// uires.idx�ļ��ĸ����
	pugi::xml_document m_xmlDocSkin;			// skin�����ļ�xml doc
	pugi::xml_document m_xmlDocColor;			// Color�����ļ�xml doc
	pugi::xml_document m_xmlDocString;			// String�����ļ�xml doc
	pugi::xml_document m_xmlDocStyle;			// Style�����ļ�xml doc
	pugi::xml_document m_xmlDocObjattr;			// Objattr�����ļ�xml doc


	SMap<SStringT, SStringT> m_mapResFile;
	SMap<SStringT, SStringT> m_mapXmlFile;		// ����XML�ļ���Ϣ
};

