#pragma once

#include "../SPropertyItemBase.h"

namespace SOUI
{
    class SPropertyItemText : public SPropertyItemBase
    {
        SOUI_CLASS_NAME(SPropertyGroup,L"proptext")
    public:
        virtual void DrawItem(IRenderTarget *pRT,CRect rc);
        
        virtual void SetString(const SStringT & strValue);
        virtual SStringT GetString() const {return m_strValue;}
		virtual BOOL HasButton() const;

		//add
	    virtual void SetStringOnly( const SStringT & strValue );
       
        SOUI_ATTRS_BEGIN()
            ATTR_STRINGT(L"value",m_strValue,TRUE)
            ATTR_STRINGT(L"buttonType",m_strButtonType,TRUE)
        SOUI_ATTRS_END()

    protected:
        virtual void OnInplaceActive(bool bActive);
        virtual void OnButtonClick();
    protected:
        SStringT m_strValue;
		SStringT m_strButtonType;
        
        SEdit  * m_pEdit;
    public:
        static IPropertyItem * CreatePropItem(SPropertyGrid *pOwner)
        {
            return new SPropertyItemText(pOwner);
        }
    protected:
        SPropertyItemText(SPropertyGrid *pOwner):SPropertyItemBase(pOwner),m_pEdit(NULL)
        {
        }
    };
}
