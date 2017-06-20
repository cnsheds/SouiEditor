#pragma once


class ResManger
{
public:
	ResManger();
	~ResManger();

	struct SkinItem
	{
		SStringT class_name;
		SStringT name;
		SStringT src;
		pugi::xml_node attrdoc;
		SkinItem() { ; }
		SkinItem(SStringT _classname, SStringT _name, SStringT _src, pugi::xml_node xmlnode)
		{
			class_name = _classname;
			name = _name;
			src = _src;
			attrdoc.append_copy(xmlnode);
		}
	};

	struct StyleItem
	{
		SStringT class_name;
		SStringT name;
		pugi::xml_node attrdoc;
		StyleItem() { ; }
		StyleItem(SStringT _classname, SStringT _name, pugi::xml_node xmlnode)
		{
			class_name = _classname;
			name = _name;
			attrdoc.append_copy(xmlnode);
		}
	};

	struct ValueItem
	{
		SStringT class_name;
		SStringT value;

		ValueItem() { ; }
		ValueItem(SStringT _classname, SStringT _value)
		{
			class_name = _classname;
			value = _value;
		}
	};


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

	SkinItem GetSkinByImg(SStringT srcimg);

	SStringA GetSkinAutos();

	SStringA GetStyleAutos();

	SStringA GetStringAutos();

	SStringA GetColorAutos();


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

	SMap<SStringT, SkinItem> m_mapSkins;		//���ж����Skin��Ŀ
	SMap<SStringT, StyleItem> m_mapStyles;		//���ж����Style��Ŀ
	SMap<SStringT, ValueItem> m_mapStrings;		//���ж����String
	SMap<SStringT, ValueItem> m_mapColors;		//���ж����Color
};

