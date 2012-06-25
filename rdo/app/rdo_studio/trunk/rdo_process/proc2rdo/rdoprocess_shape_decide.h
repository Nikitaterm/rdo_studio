#if !defined(AFX_RDOPROCESS_SHAPE_DECIDE_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_DECIDE_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"

class RPProcessShapeDecide : public RPProcessShape  
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeDecide( RPObject* parent );
	virtual ~RPProcessShapeDecide();

	virtual rp::string getClassName() const { return "RPProcessShapeDecide"; }
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// ���������������� ����������� ������� saveToXML � loadFromXML ��� RPProcessShapeDecide:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();
	double ptrue; 
	double pfalse; 
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
