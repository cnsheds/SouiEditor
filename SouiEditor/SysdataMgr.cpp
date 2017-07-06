#include "stdafx.h"
#include "SysdataMgr.h"
#include "CDebug.h"
#include <vector>
#include "helpapi.h"
#include "Global.h"

extern SStringT g_CurDir;

CSysDataMgr::CSysDataMgr()
{
}


CSysDataMgr::~CSysDataMgr()
{
}

bool CSysDataMgr::LoadSysData(LPCTSTR cfgDir)
{
	m_strConfigDir = cfgDir;
	InitProperty();

	return true;
}

void CSysDataMgr::InitProperty()   //��ʼ�������б�
{
	/*

	<ͨ����ʽ>
	<id style="proptext" name ="����ID(id)" value="" />
	<name style="proptext" name ="��������(name)" value="" />
	<skin style="proptext" name ="Ƥ��(skin)" value="" />
	</ͨ����ʽ>

	<Button>
	<���� name="����">
	<id/>
	<name/>
	<skin/>
	<pos/>
	<size/>
	<offset/>
	</����>

	<���� name="��չ">
	<accel style="proptext" name ="��ݼ�(accel)" value="ctrl+alt+f9" />
	<animate style="propoption" name ="����(animate)" value="0" options="��(0)|��(1)"/>
	</����>

	</Button>
	*/


	pugi::xml_parse_result result = m_xmlDocProperty.load_file(g_CurDir + L"Config\\property.xml");
	if (!result)
	{
		CDebug::Debug(_T("InitPropertyʧ��"));
	}

	pugi::xml_document xmlDocProperty;
	xmlDocProperty.append_copy(m_xmlDocProperty.document_element());
	pugi::xml_node NodeCom = xmlDocProperty.child(L"root").child(L"ͨ����ʽ");
	pugi::xml_node NodeComStyle = xmlDocProperty.child(L"root").child(L"������ʽ");
	pugi::xml_node NodeCtrlList = xmlDocProperty.child(L"root").child(L"�����б�");

	pugi::xml_node NodeCtrl = NodeCtrlList.first_child();  //NodeCtrl = Button�ڵ�
	while (NodeCtrl)
	{
		SStringT strCtrlname = NodeCtrl.name();
		m_mapControl.SetAt(strCtrlname, new CTRL_ATTR_VALUE());

		InitCtrlProperty(NodeCom, NodeComStyle, NodeCtrl, m_mapControl[strCtrlname]);

		NodeCtrl = NodeCtrl.next_sibling();
	}

	pugi::xml_node NodeBasicStyle = NodeComStyle.first_child();  //NodeCtrl = Button�ڵ�
	while (NodeBasicStyle)
	{
		if (!NodeBasicStyle.attribute(L"style"))
		{
			// û������style��Ϊͨ������, ��ͨ�����Խ���л�ȡ��Ϣ
			SStringT strName = NodeBasicStyle.name();
			pugi::xml_node N = NodeCom.child(strName);
			if (N)
			{
				pugi::xml_node NodeNew;

				// ��ͨ�����Խ����滻
				NodeNew = NodeBasicStyle.parent().insert_copy_before(N, NodeBasicStyle);
				NodeBasicStyle.parent().remove_child(NodeBasicStyle);

				NodeBasicStyle = NodeNew;
			}
			else
			{
				NodeBasicStyle.append_attribute(L"style").set_value(L"proptext");
				NodeBasicStyle.append_attribute(L"name").set_value(strName);
			}
		}
		pugi::xml_node ctrl_node;
		ctrl_node.append_copy(NodeBasicStyle);
		m_arrControlStyle.Add(CtrlAttrItem(NodeBasicStyle.name(), ctrl_node));

		NodeBasicStyle = NodeBasicStyle.next_sibling();
	}
}


void CSysDataMgr::InitCtrlProperty(pugi::xml_node NodeCom, pugi::xml_node NodeComStyle, pugi::xml_node NodeCtrl, CTRL_ATTR_VALUE* arr_attr)
{
	/*
	<ͨ����ʽ>
	<id style="proptext" name ="����ID(id)" value="" />
	<name style="proptext" name ="��������(name)" value="" />
	<skin style="proptext" name ="Ƥ��(skin)" value="" />
	</ͨ����ʽ>

	<Button>
	<���� name="����">
	<id/>
	<name/>
	<skin/>
	<pos/>
	<size/>
	<offset/>
	</����>

	<���� name="��չ">
	<accel style="proptext" name ="��ݼ�(accel)" value="ctrl+alt+f9" />
	<animate style="propoption" name ="����(animate)" value="0" options="��(0)|��(1)"/>
	</����>

	</Button>

	<propgroup name="group1" description="desc of group1">
	<proptext name="text1.1" value="value 1.1">

	*/

	pugi::xml_node NodeChild = NodeCtrl.first_child();

	while (NodeChild)
	{
		SStringT nodeName = NodeChild.name();
		if (_wcsicmp(nodeName, L"����") == 0)
		{
			SStringT nameAttr = NodeChild.attribute(L"name").as_string();
			if (nameAttr.CompareNoCase(L"������ʽ") == 0)
			{
				pugi::xml_node parentNode = NodeChild.parent();
				pugi::xml_node nodeCopy = parentNode.insert_copy_after(NodeComStyle, NodeChild);
				parentNode.remove_child(NodeChild);
				NodeChild = nodeCopy;
			}
			NodeChild.set_name(L"propgroup");
			InitCtrlProperty(NodeCom, NodeComStyle, NodeChild, arr_attr);
		}
		else
		{
			if (!NodeChild.attribute(L"style"))
			{
				// û������style��Ϊͨ������, ��ͨ�����Խ���л�ȡ��Ϣ
				SStringT strName = NodeChild.name();
				pugi::xml_node N = NodeCom.child(strName);
				pugi::xml_node NodeNew;

				// ��ͨ�����Խ����滻
				NodeNew = NodeChild.parent().insert_copy_before(N, NodeChild);
				NodeChild.parent().remove_child(NodeChild);

				NodeChild = NodeNew;
			}
			pugi::xml_node ctrl_node;
			ctrl_node.append_copy(NodeChild);
			arr_attr->Add(CtrlAttrItem(NodeChild.name(), ctrl_node));
		}

		NodeChild = NodeChild.next_sibling();
	}
}

SStringA CSysDataMgr::GetCtrlAutos()
{
	std::vector<SStringT> vecTemp;
	SPOSITION pos = m_mapControl.GetStartPosition();
	while (pos)
	{
		const SMap<SStringT, CTRL_ATTR_VALUE *>::CPair* item = m_mapControl.GetAt(pos);
		vecTemp.push_back(item->m_key);

		m_mapControl.GetNext(pos);
	}
	std::sort(vecTemp.begin(), vecTemp.end(), SortSStringNoCase);

	SStringT strAuto;
	std::vector<SStringT>::iterator it = vecTemp.begin();
	for (; it != vecTemp.end(); it++)
	{
		strAuto += *it + _T(" ");
	}
	strAuto.TrimRight(' ');

	SStringA str = S_CW2A(strAuto, CP_UTF8);
	return str;
}

SStringA CSysDataMgr::GetCtrlAttrAutos(SStringT ctrlname)
{
	ctrlname.MakeLower();
	if (ctrlname.CompareNoCase(_T("root")) == 0)
		ctrlname = _T("window");

	SMap<SStringT, CTRL_ATTR_VALUE*>::CPair* pNode = m_mapControl.Lookup(ctrlname);
	if (!pNode)
	{
		pNode = m_mapControl.Lookup(_T("window"));
	}
	if (!pNode)
		return "";

	SStringT strAuto;
	CTRL_ATTR_VALUE* ctrl_attr = pNode->m_value;
	SArray<CtrlAttrItem> allAttr;
	allAttr.Append(*ctrl_attr);
	allAttr.Append(m_arrControlStyle);

	SStringT strLastWord;
	qsort(allAttr.GetData(), allAttr.GetCount(), sizeof(CtrlAttrItem), CtrlAttrCmpNoCase);
	for (int i = 0; i < allAttr.GetCount(); i++)
	{
		if (allAttr.GetAt(i).attrname.CompareNoCase(uiedit_SpecAttr) == 0)
			continue;
		if (strLastWord != allAttr.GetAt(i).attrname)
		{
			strLastWord = allAttr.GetAt(i).attrname;
			strAuto += strLastWord + _T(" ");
		}
	}

	strAuto.TrimRight(' ');
	SStringA str = S_CW2A(strAuto, CP_UTF8);
	return str;
}