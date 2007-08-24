#include "pch.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void rtperror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - �������� ��-���������, ������� �����
// ----------------------------------------------------------------------------
RDORTPDefVal::RDORTPDefVal( const RDORTPDefVal& dv ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( dv.src_info() ),
	exist( dv.exist )
{
}

RDORTPDefVal::RDORTPDefVal( bool _exist ):
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( _exist )
{
}

RDORTPDefVal::RDORTPDefVal( bool _exist, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( _exist )
{
}

int RDORTPDefVal::getIntValue() const
{
	parser->error( src_info(), "�������� �������� ��-���������" );
//	parser->error( "Invalid default value" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::getRealValue() const
{
	parser->error( src_info(), "�������� �������� ��-���������" );
	return 0.;	// unreachable code...
}

const std::string& RDORTPDefVal::getEnumValue() const
{
	parser->error( src_info(), "�������� �������� ��-���������" );
#pragma warning( disable: 4172 )
	return "";	// unreachable code...
#pragma warning( default: 4172 )
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( getType() ) {
		case RDORTPParamType::pt_int: {
			if ( action->getType() == RDORTPParamType::pt_real ) {
				parser->warning( action->src_info(), "������� ������������� ����� � �����, �������� ������ ������" );
			} else if ( action->getType() != RDORTPParamType::pt_int ) {
				parser->error( action->src_info(), "�������������� �����. ��������� ����� �����" );
			} else {
				const rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(action->getCalc());
				if ( calc_const ) {
					rdoRuntime::RDOValue value = calc_const->calcValueBase( parser->runtime );
					checkRSSIntValue( value, action->src_info() );
				}
			}
			break;
		}
		case RDORTPParamType::pt_real: {
			if ( action->getType() != RDORTPParamType::pt_real && action->getType() != RDORTPParamType::pt_int ) {
				parser->error( action->src_info(), "�������������� �����. ��������� ������������ �����" );
			} else {
				const rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(action->getCalc());
				if ( calc_const ) {
					rdoRuntime::RDOValue value = calc_const->calcValueBase( parser->runtime );
					checkRSSRealValue( value, action->src_info() );
				}
			}
			break;
		}
		case RDORTPParamType::pt_enum: {
			if ( action->getType() == RDORTPParamType::pt_unknow ) {
				if ( static_cast<const RDORTPEnumParamType*>(this)->enu->findEnumValueWithoutThrow( action->str ) == -1 ) {
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun ) {
						parser->error( action->src_info(), rdo::format("�������� '%s' �� ����� �������� ����������� �������: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					} else {
						parser->error( action->src_info(), rdo::format("�������� '%s' �� �������� ��������� ������������� ���������: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			} else if ( action->getType() != RDORTPParamType::pt_enum ) {
				parser->error( action->src_info(), rdo::format("�������������� �����. ��������� ������������ ���: %s", src_text().c_str()) );
			} else if ( action->enu != static_cast<const RDORTPEnumParamType*>(this)->enu ) {
				if ( action->enu->enumVals == static_cast<const RDORTPEnumParamType*>(this)->enu->enumVals ) {
					parser->error_push_only( action->src_info(), "������������ ��������� ������������ ���� � ����������� ����������" );
					parser->error_push_only( action->enu->src_info(), action->enu->src_text() );
					parser->error_push_only( action->src_info(), "�" );
					parser->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
					parser->error( "��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������" );
				} else {
					parser->error_push_only( action->src_info(), "�������������� ������������ �����" );
					parser->error_push_only( action->enu->src_info(), action->enu->src_text() );
					parser->error_push_only( action->src_info(), "�" );
					parser->error( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
				}
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( NULL )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

int RDORTPParam::writeModelStructure() const
{
	parser->modelStructure << getName() << " ";
	getType()->writeModelStructure();
//	parser->modelStructure << " ";
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
RDORTPResType::RDORTPResType( RDOParser* _parser, const std::string& _name, const bool _permanent ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	name( _name ),
	number( parser->getRTP_id() ),
	permanent( _permanent )
{
	setSrcText( name );
	parser->insertRTPResType( this );
}

void RDORTPResType::addParam( const RDORTPParam* const param )
{
	if ( findRTPParam( param->getName() ) ) {
		parser->lexer_loc_restore();
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_PARAM_NAME, param->getName().c_str() );
	}
	params.push_back( param );
}

const RDORTPParam* RDORTPResType::findRTPParam( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDORTPParam>(param) );
	return it != params.end() ? *it : NULL;
}

int RDORTPResType::getRTPParamNumber( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName2<RDORTPParam>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

int RDORTPResType::writeModelStructure() const
{
	parser->modelStructure << getNumber() << " " << getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		parser->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - ��������, ��������, [1 .. 4]
// ----------------------------------------------------------------------------
RDORTPIntDiap::RDORTPIntDiap():
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( const RDORTPIntDiap& diap ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	minVal( diap.minVal ),
	maxVal( diap.maxVal )
{
}

RDORTPIntDiap::RDORTPIntDiap( const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( int _minVal, int _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	minVal( _minVal ),
	maxVal( _maxVal )
{
	if ( minVal > maxVal ) {
		parser->error( _max_value_pos, "����� ������� ��������� ������ ���� ������ ������" );
	}
	setSrcText( rdo::format("[%d..%d]", minVal, maxVal) );
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPIntDefVal(0) ),
	diap( new RDORTPIntDiap() )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPIntParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "integer";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	if ( dv->isExist() ) {
		getRSSIntValue( static_cast<RDORTPIntDefVal*>(dv)->getIntValue(), dv->src_info() );
	}
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( *static_cast<RDORTPIntDefVal*>(dv) );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent, _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( defVal, defVal_info );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������ ����: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������ ����: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPIntParamType::writeModelStructure() const
{
	parser->modelStructure << "I" << std::endl;
	return 0;
}

void RDORTPIntParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	parser->error( _src_info, rdo::format("��������� ����� �����, ������� '%s'", val.c_str()) );
}

void RDORTPIntParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	if ( diap->isExist() ) {
		if ( val < diap->minVal || val > diap->maxVal ) {
			if ( _src_info.src_filetype() == diap->src_filetype() && _src_info.src_pos().last_line == diap->src_pos().last_line ) {
				parser->error( _src_info, rdo::format("������������� �������� ������� �� ���������� �������� [%d..%d]: %d", diap->minVal, diap->maxVal, val) );
			} else {
				parser->error_push_only( _src_info, rdo::format("������������� �������� ������� �� ���������� �������� [%d..%d]: %d", diap->minVal, diap->maxVal, val) );
				parser->error( diap->src_info(), rdo::format("��. �������� ���������") );
			}
//			parser->error( ("integer value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str() );
		}
	}
}

void RDORTPIntParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	parser->error( _src_info, rdo::format("��������� ����� �����, ������� '%f'", val) );
}

rdoRuntime::RDOValue RDORTPIntParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		parser->error( _src_info, "��� �������� ��-���������" );
//		parser->error( "No default value" );
	}
	return rdoRuntime::RDOValue( dv->getIntValue() );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return NULL; // �� ����������� ��-�� checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return NULL; // �� ����������� ��-�� checkRSSRealValue
}

int RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !diap->isExist() ) {
		parser->error( src_info(), "��� ��������� ��������� ������� ������ ���� ����� ���������� ��������" );
//		parser->error("integer table function parameter must have range");
	}
	if ( diap->minVal != 1 ) {
		parser->error( src_info(), rdo::format("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]", diap->minVal, diap->maxVal) );
//		parser->error("integer table function parameter must have minimum value = 1");
	}
	return diap->maxVal - diap->minVal + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - ��������, ��������, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
RDORTPRealDiap::RDORTPRealDiap():
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( const RDORTPRealDiap& diap ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	minVal( diap.minVal ),
	maxVal( diap.maxVal )
{
}

RDORTPRealDiap::RDORTPRealDiap( const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( double _minVal, double _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	minVal( _minVal ),
	maxVal( _maxVal )
{
	if ( minVal > maxVal ) {
		parser->lexer_loc_set( _max_value_pos.last_line, _max_value_pos.last_column );
		parser->error( "����� ������� ��������� ������ ���� ������ ������" );
	}
	setSrcText( rdo::format("[%f..%f]", minVal, maxVal) );
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPRealDefVal(0) ),
	diap( new RDORTPRealDiap() )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPRealParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "real";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	if ( dv->isExist() ) {
		getRSSRealValue( static_cast<RDORTPRealDefVal*>(dv)->getRealValue(), dv->src_info() );
	}
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( *static_cast<RDORTPRealDefVal*>(dv) );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPRealParamType::writeModelStructure() const
{
	parser->modelStructure << "R" << std::endl;
	return 0;
}

void RDORTPRealParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	parser->error( _src_info, rdo::format("��������� ������������ �����, ������� '%s'", val.c_str()) );
}

void RDORTPRealParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
}

void RDORTPRealParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	if ( diap->isExist() ) {
		if ( val < diap->minVal || val > diap->maxVal ) {
			if ( _src_info.src_filetype() == diap->src_filetype() && _src_info.src_pos().last_line == diap->src_pos().last_line ) {
				parser->error( _src_info, rdo::format("������������ �������� ������� �� ���������� �������� [%f..%f]: %f", diap->minVal, diap->maxVal, val) );
			} else {
				parser->error_push_only( _src_info, rdo::format("������������ �������� ������� �� ���������� �������� [%f..%f]: %f", diap->minVal, diap->maxVal, val) );
				parser->error( diap->src_info(), rdo::format("��. �������� ���������") );
			}
//			parser->error(("real value " + toString(*val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
		}
	}
}

rdoRuntime::RDOValue RDORTPRealParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		parser->error( _src_info, "��� �������� ��-���������" );
	}
	return rdoRuntime::RDOValue( dv->getRealValue() );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return NULL; // �� ����������� ��-�� checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSRealValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

int RDORTPRealParamType::getDiapTableFunc() const
{
	parser->error( src_info(), "�������� ��������� ������� ����� ���� ������ ��� ������������� ����" );
//	parser->error( "unexpected real table function parameter" );
	return 0;		// unreachable code...
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
void RDORTPEnum::add( const std::string& next, const YYLTYPE& _pos )
{
	if ( std::find( enumVals.begin(), enumVals.end(), next ) != enumVals.end() ) {
		parser->lexer_loc_set( _pos.last_line, _pos.last_column );
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_ENUM_VALUE, next.c_str() );
	}
	enumVals.push_back( next );
}

int RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& _src_info, const std::string& val ) const
{
	int result = findEnumValueWithoutThrow( val );
	if ( result == -1 ) {
		parser->error_push_only( _src_info, rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val.c_str() );
		parser->error( src_info(), rdo::format("��������� ��������: %s", src_text().c_str()) );
	}
	return result;
}

int RDORTPEnum::findEnumValueWithoutThrow( const std::string& val ) const
{
	std::vector< std::string >::const_iterator it = std::find( enumVals.begin(), enumVals.end(), val );
	return it != enumVals.end() ? it - enumVals.begin() : -1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
RDORTPEnumParamType::RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPEnumDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	enu( _enu ),
	enum_name( "" ),
	enum_fun( false )
{
	enu->reparent( this );
	init_src_info();
	parser->insertEnum( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = enu->src_text();
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	setSrcPos( enu->getPosAsYY(), dv->getPosAsYY() );
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( *static_cast<RDORTPEnumDefVal*>(dv) );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent, _enu, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %d", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const 
{
	enu->findEnumValueWithThrow( defVal_info, defVal );
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( defVal );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent, _enu, _dv, _src_info );
	_dv->setSrcInfo( defVal_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

int RDORTPEnumParamType::writeModelStructure() const
{
	parser->modelStructure << "E " << enu->enumVals.size() << std::endl;
	for(int i = 0; i < enu->enumVals.size(); i++)
		parser->modelStructure << "    " << i << " " << enu->enumVals.at(i) << std::endl;
	return 0;
}

void RDORTPEnumParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	enu->findEnumValueWithThrow( _src_info, val );
}

void RDORTPEnumParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	parser->error( _src_info, rdo::format("��������� ������������ ���, ������� '%d'", val) );
}

void RDORTPEnumParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	parser->error( _src_info, rdo::format("��������� ������������ ���, ������� '%f'", val) );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		parser->error( _src_info, "��� �������� ��-���������" );
	}
	return rdoRuntime::RDOValue( enu->findEnumValueWithThrow( _src_info, dv->getEnumValue() ) );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	return enu->findEnumValueWithThrow( _src_info, val );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return NULL; // �� ����������� ��-�� checkRSSIntValue
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return NULL; // �� ����������� ��-�� checkRSSRealValue
}

int RDORTPEnumParamType::getDiapTableFunc() const 
{
	return enu->enumVals.size();
}

} // namespace rdoParse
