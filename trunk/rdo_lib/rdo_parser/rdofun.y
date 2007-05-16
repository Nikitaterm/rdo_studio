%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent_kw			258
%token Parameters			259
%token integer				260
%token real					261
%token End					262
%token temporary_kw			263
%token IDENTIF				264
%token INT_CONST			265
%token REAL_CONST			266
%token such_as				267
%token dblpoint				268
%token Resources			269
%token trace_keyword		270
%token no_trace				271
%token IDENTIF_COLON		272
%token Constant				273
%token Body					274
%token Function_keyword		275
%token Type_keyword			276
%token algorithmic			277
%token table_keyword		278
%token list_keyword			279
%token Exist				281
%token Not_Exist			282
%token For_All				283
%token Not_For_All			284
%token neq					285
%token leq					286
%token geq					287
%token NoCheck				288
%token Calculate_if			289
%token or_keyword			290
%token and_keyword			291
%token Sequence				292
%token uniform				293
%token exponential			294
%token normal_keyword		295
%token by_hist				296
%token enumerative			297

%token Pattern				298
%token operation_kw			299
%token irregular_event		300
%token rule_keyword			301
%token keyboard				302
%token Relevant_resources	303
%token Keep					304
%token Create_keyword		305
%token Erase				306
%token NonExist				307
%token IDENTIF_NoChange		308
%token Time					309
%token Choice				310
%token from_keyword			311
%token first_keyword		312
%token Convert_begin		313
%token Convert_end			314
%token Convert_rule			315
%token Convert_event		316
%token with_max				317
%token with_min				318
%token IDENTIF_set			319
%token IDENTIF_NoChange_NoChange	320
%token Operations			321
	
%token Results				322
%token watch_par			323
%token watch_state			324
%token watch_quant			325
%token watch_value			326
%token get_value			327

%token Model_name			328
%token Resource_file		329
%token OprIev_file			330
%token Frame_file			331
%token Statistic_file		332
%token Results_file			333
%token Trace_file			334
%token Show_mode			335
%token Frame_number			336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime		339
%token Trace_EndTime		340
%token Terminate_if			341
%token Break_point			342
%token Seed					343
%token NoShow				344
%token Monitor				345
%token Animation			346
%token NoChange				347

%token Decision_point		348
%token search_keyword		349
%token trace_stat			350
%token trace_tops			351
%token trace_all			352
%token Condition_keyword	353
%token Term_condition		354
%token Evaluate_by			355
%token Compare_tops			356
%token NO					357
%token YES					358
%token Activities			359
%token value_before			360
%token value_after			361
%token some					362
%token Process				363
%token SEIZE				364

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token transparent			406
%token bitmap				407
%token s_bmp				408
%token rect_keyword			409
%token r_rect				410
%token line					411
%token ellipse				412
%token triang				413
%token active				414
%token QUOTED_IDENTIF		415
%token Select				418
%token Size_kw				419
%token Empty_kw				420
%token not_keyword			421
%token UMINUS				422

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
#include <rdocalc.h>

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%start fun_list

%%

fun_list:	fun_consts fun_func_seq;
			| error {
				parser->lexer_loc_set( &(@1) );
				if ( parser->hasConstant() ) {
					parser->error( "��������� �������� ������� ��� ������������������" );
				} else {
					parser->error( "��������� �������� �������, ������������������ ��� ���������" );
				}
			};

fun_consts:	/* empty */
			| Constant fun_const_body End
			| Constant fun_const_body {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "����� �������� �������� ��������� �������� ����� $End" );
			};

fun_const_body:	/* empty */
				| fun_const_body fun_const_param_desc {
					RDORTPParamDesc* cons = (RDORTPParamDesc*)$2;
					parser->addConstant(cons);
				}
				| fun_const_body error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "��������� �������� ���������" );
				};

fun_const_param_desc:	IDENTIF_COLON fun_param_type {
							std::string* name = (std::string*)$1;
							if ( parser->findFUNConst( name ) ) {
								parser->lexer_loc_set( &(@1) );
								parser->error( rdo::format("��������� � ����� ������ ��� ����������: %s", name->c_str()) );
//								parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
							}
							RDORTPResParam*  parType = (RDORTPResParam*)$2;
							RDORTPParamDesc* param   = new RDORTPParamDesc( name, parType );
							if ( !parType->dv->exist ) {
								parser->lexer_loc_set( &(@2) );
								parser->error( "��������� ������ ����� ��������" );
//								parser->error( "Constant must have value" );
							}
							$$ = (int)param;
						}
						| IDENTIF_COLON {
							parser->lexer_loc_set( &(@1) );
							parser->error( "��������� ��� ���������" );
						}
						| IDENTIF_COLON error {
							parser->lexer_loc_set( &(@2) );
							parser->error( "�������� ��� ���������" );
						};

// ----------------------------------------------------------------------------
// ---------- �������� ����������
// ----------------------------------------------------------------------------
fun_param_type: integer fun_int_diap fun_int_default_val {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPIntResParam *rp = new RDORTPIntResParam( parser, diap, dv );
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| integer fun_int_diap {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = new RDORTPIntDefVal();
					RDORTPIntResParam *rp = new RDORTPIntResParam( parser, diap, dv );
					$$ = (int)rp;
				}
				| real fun_real_diap fun_real_default_val {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPRealResParam *rp = new RDORTPRealResParam( parser, diap, dv );
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| real fun_real_diap {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = new RDORTPRealDefVal();
					RDORTPRealResParam *rp = new RDORTPRealResParam( parser, diap, dv );
					$$ = (int)rp;
				}
				| fun_enum fun_enum_default_val {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
					RDORTPEnumResParam *rp = new RDORTPEnumResParam( parser, enu, dv );
					$$ = (int)rp;
				}
				| fun_enum {
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = new RDORTPEnumDefVal();
					RDORTPEnumResParam *rp = new RDORTPEnumResParam( parser, enu, dv );
					$$ = (int)rp;
				}
				| fun_such_as {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs();
				}
				| fun_such_as fun_int_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$2;
					$$ = (int)desc->getType()->constructSuchAs((int)dv->val);
				}
				| fun_such_as	fun_real_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$2;
					if(!dv->exist)
						$$ = (int)desc->getType()->constructSuchAs();
					else
						$$ = (int)desc->getType()->constructSuchAs((double *)&(dv->val));
				}
				| fun_such_as fun_enum_default_val {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					if ( !dv->exist ) {
						$$ = (int)desc->getType()->constructSuchAs();
					} else {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@2) );
						$$ = (int)desc->getType()->constructSuchAs((std::string *)dv->value);
						parser->lexer_loc_restore();
					}
				}
				| fun_such_as '=' error {
					parser->error( "��������� ������� ��-���������" );
				}
				| fun_such_as error {
					parser->error( "��������� ��������� �������� ������, ��������, ������� ��-���������" );
				}
				| integer error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� integer. ��������, �� ������� �������� ��-���������." );
				}
				| real error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� real. ��������, �� ������� �������� ��-���������." );
				}
				| fun_enum error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ������������� ����. ��������, �� ������� �������� ��-���������." );
				};

fun_int_diap:	/* empty */ {
					RDORTPIntDiap *diap = new RDORTPIntDiap();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@4) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "�������� ����� �������" );
				};

fun_real_diap: /* empty */ {
					RDORTPRealDiap *diap = new RDORTPRealDiap();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = *((double *)$2);
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					double min = $2;
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					parser->lexer_loc_restore();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "�������� ����� �������" );
				};

fun_enum:	'(' fun_enum_item ')' {
				$$ = $2;
			}
			| '(' fun_enum_item {
				parser->lexer_loc_set( &(@2) );
				parser->error( "������������ ������ ������������� �������" );
			};

fun_enum_item:	IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( parser, (std::string *)$1 );
					$$ = (int)enu;
					reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt = 1;
				}
				| fun_enum_item ',' IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@3) );
						RDORTPEnum *enu = (RDORTPEnum *)$1;
						enu->add((std::string *)$3);
						parser->lexer_loc_restore();
						$$ = (int)enu;
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| fun_enum_item IDENTIF {
					if ( reinterpret_cast<RDOLexerFUN*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("��������� ������� �����: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| fun_enum_item error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "������ � �������� �������� ������������� ����" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "�������� �������� ������������� ����: %s", str.c_str() ) );
					}
				}
				| fun_enum_item ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| fun_enum_item ',' REAL_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| INT_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| REAL_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "������ � �������� �������� ������������� ����" );
				};

fun_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string* type = (std::string *)$2;
					std::string* param = (std::string *)$4;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					}
					const RDORTPParamDesc *const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type->c_str(), param->c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string *constName = (std::string *)$2;
					const RDOFUNConstant *const cons = parser->findFUNConst(constName);
					if ( !cons ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("������ �� �������������� ���������: %s", constName->c_str()) );
					}
					$$ = (int)(cons->descr);
				}
				| such_as IDENTIF '.' {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "�� ������ ��������" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "������ ��� �������� ���������" );
					}
				}
				| such_as IDENTIF error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "����� ����� ���� ������ ���� ������ �������� ����� �����" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "����� ��������� ����� such_as ���������� ������� ��� � �������� ������� ��� ������" );
				};

fun_int_default_val:	'=' INT_CONST {
						RDORTPIntDefVal *dv = new RDORTPIntDefVal($2);
						$$ = (int)dv;
					}
					| '=' REAL_CONST {
						// ����� ����� ���������������� ������������: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ������ ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ������ ����" );
					};

fun_real_default_val:	'=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2)));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ������������� ����" );
					};

fun_enum_default_val:	'=' IDENTIF {
						std::string *val = (std::string *)$2;
						RDORTPEnumDefVal* dv = new RDORTPEnumDefVal(val);
						$$ = (int)dv;
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ������������� ����" );
					};

fun_func_seq:	/* empty */
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr
			| fun_func_seq Constant {
				parser->lexer_loc_set( &(@2) );
				parser->error( "��������� ����� ���� ������� �������, ����� ��������� � ��������������������" );
			};

fun_func_descr:	fun_func_header fun_func_footer;

fun_func_header:	Function_keyword IDENTIF_COLON fun_param_type {
						std::string* name = (std::string*)$2;
						if ( parser->findFUNConst(name) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "���������� ��������� � ����� �� ������: %s", name->c_str() ) );
						}
						if ( parser->findSequence(name) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "���������� ������������������ � ����� �� ������: %s", name->c_str() ) );
						}
						if ( parser->findFunction(name) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "������� ��� ����������: %s", name->c_str() ) );
//							parser->error(("Second appearance of the same function: " + *(name)).c_str());
						}
						RDORTPResParam* retType = (RDORTPResParam*)$3;
						RDOFUNFunction* fun = new RDOFUNFunction( parser, name, retType );
						if ( retType->getType() == RDORTPResParam::pt_enum && static_cast<RDORTPEnumResParam*>(retType)->enum_name.empty() ) {
							static_cast<RDORTPEnumResParam*>(retType)->enum_name = *name;
							static_cast<RDORTPEnumResParam*>(retType)->enum_fun  = true;
						}
						$$ = (int)fun;
					};

fun_func_footer:	Type_keyword '=' algorithmic Parameters fun_func_params Body fun_func_algorithmic_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createAlgorithmicCalc();
					}
					| Type_keyword '=' list_keyword Parameters fun_func_params Body fun_func_list_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createListCalc();
					}
					| Type_keyword '=' table_keyword Parameters fun_func_params Body fun_func_list_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createTableCalc();
					}
					| Type_keyword '=' algorithmic Parameters fun_func_params Body fun_func_algorithmic_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "��������� �������� ����� $End" );
					}
					| Type_keyword '=' list_keyword Parameters fun_func_params Body fun_func_list_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "��������� �������� ����� $End" );
					}
					| Type_keyword '=' table_keyword Parameters fun_func_params Body fun_func_list_body error {
						parser->lexer_loc_set( @8.first_line, @8.first_column );
						parser->error( "��������� �������� ����� $End" );
					}
					| Type_keyword '=' algorithmic Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "��������� ������ ����������" );
					}
					| Type_keyword '=' list_keyword Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "��������� ������ ����������" );
					}
					| Type_keyword '=' table_keyword Parameters error {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "��������� ������ ����������" );
					}
					| Type_keyword '=' algorithmic error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' list_keyword error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' table_keyword error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "����������� ��� �������" );
					}
					| Type_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ��� �������" );
					};

fun_func_params:	/* empty */
			|	fun_func_params IDENTIF_COLON fun_param_type	{
				std::string*         name  = (std::string*)$2;
				RDORTPResParam*      type  = (RDORTPResParam*)$3;
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam( name, type );
				param->setErrorPos( @2 );
				parser->getLastFUNFunction()->add(param);	// the function must check uniquness of parameters names
				$$ = (int)param;
			};

fun_func_list_body:	/* empty */
			| fun_func_list_body fun_std_value;

fun_std_value:	IDENTIF {
					RDOFUNFunctionListElementIdentif *value = new RDOFUNFunctionListElementIdentif( parser->getLastFUNFunction(), (std::string *)$1 );
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| REAL_CONST {
					RDOFUNFunctionListElementReal *value = new RDOFUNFunctionListElementReal( parser->getLastFUNFunction(), (double *)$1 );
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| INT_CONST {
					RDOFUNFunctionListElementInt *value = new RDOFUNFunctionListElementInt( parser->getLastFUNFunction(), (int)$1 );
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				}
				| '=' {
					RDOFUNFunctionListElementEq *value = new RDOFUNFunctionListElementEq( parser->getLastFUNFunction() );
					parser->getLastFUNFunction()->add(value);
					value->setErrorPos( @1 );
					$$ = (int)value;
				};

fun_func_algorithmic_body:	/* empty */
			| fun_func_algorithmic_body fun_func_algorithmic_calc_if {
				parser->getLastFUNFunction()->add((RDOFUNCalculateIf*)$2);
			};

fun_func_algorithmic_calc_if:	Calculate_if fun_logic IDENTIF '=' fun_arithm {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									$$ = (int)(new RDOFUNCalculateIf( parser->getLastFUNFunction(), (RDOFUNLogic *)$2, (std::string *)$3, (RDOFUNArithm *)$5 ));
									parser->lexer_loc_restore();
								}
								| Calculate_if fun_logic error {
									parser->lexer_loc_set( &(@2) );
									parser->error( "��������� <���_�������> = <���������_�������>" );
								}
								| error {
									parser->lexer_loc_set( &(@1) );
									parser->error( "��������� �������� ����� Calculate_if" );
								};

fun_seq_descr:	fun_seq_uniform
				| fun_seq_exponential
				| fun_seq_normal
				| fun_seq_by_hist
				| fun_seq_enumerative;

fun_seq_header:	Sequence IDENTIF_COLON fun_param_type Type_keyword '=' {
					std::string* name = (std::string*)$2;
					if ( parser->findFUNConst(name) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "���������� ��������� � ����� �� ������: %s", name->c_str() ) );
					}
					if ( parser->findSequence(name) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "������������������ ��� ����������: %s", name->c_str() ) );
					}
					if ( parser->findFunction(name) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "���������� ������� � ����� �� ������: %s", name->c_str() ) );
					}
					$$ = (int)(new RDOFUNSequenceHeader( name, (RDORTPResParam *)$3) );
				}
				| Sequence IDENTIF_COLON fun_param_type Type_keyword '=' error {
					parser->lexer_loc_set( &(@5), &(@6) );
					parser->error( "��������� ��� ������������������" );
				}
				| Sequence IDENTIF_COLON fun_param_type Type_keyword error {
					parser->lexer_loc_set( &(@4), &(@5) );
					parser->error( "��������� ��� ������������������ � ������� '=' <���>" );
				}
				| Sequence IDENTIF_COLON fun_param_type error {
					parser->lexer_loc_set( @4.first_line, @4.first_column );
					parser->error( "��������� �������� ����� $Type" );
				}
				| Sequence IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "��������� ��� ������������� ��������" );
				}
				| Sequence error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "��������� ��� ������������������" );
				};

fun_seq_uniform:	fun_seq_header uniform End {
						$$ = (int)(new RDOFUNSequenceUniform( parser, (RDOFUNSequenceHeader *)$1) );
					}
					| fun_seq_header uniform INT_CONST End {
						$$ = (int)(new RDOFUNSequenceUniform( parser, (RDOFUNSequenceHeader *)$1, $3) );
					}
					| fun_seq_header uniform INT_CONST error {
						parser->lexer_loc_set( @4.first_line, @4.first_column );
						parser->error( "��������� �������� ����� $End" );
					}
					| fun_seq_header uniform error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� ���� ����������" );
					};

fun_seq_exponential:	fun_seq_header exponential End {
							$$ = (int)(new RDOFUNSequenceExponential( parser, (RDOFUNSequenceHeader *)$1) );
						}
						| fun_seq_header exponential INT_CONST End {
							$$ = (int)(new RDOFUNSequenceExponential( parser, (RDOFUNSequenceHeader *)$1, $3) );
						}
						| fun_seq_header exponential INT_CONST error {
							parser->lexer_loc_set( @4.first_line, @4.first_column );
							parser->error( "��������� �������� ����� $End" );
						}
						| fun_seq_header exponential error {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "��������� ���� ����������" );
						};

fun_seq_normal:	fun_seq_header normal_keyword End {
					$$ = (int)(new RDOFUNSequenceNormal( parser, (RDOFUNSequenceHeader *)$1) );
				}
				| fun_seq_header normal_keyword INT_CONST End {
					$$ = (int)(new RDOFUNSequenceNormal( parser, (RDOFUNSequenceHeader *)$1, $3) );
				}
				| fun_seq_header normal_keyword INT_CONST error {
					parser->lexer_loc_set( @4.first_line, @4.first_column );
					parser->error( "��������� �������� ����� $End" );
				}
				| fun_seq_header normal_keyword error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "��������� ���� ����������" );
				};

fun_seq_by_hist_header:	fun_seq_header by_hist Body {
							$$ = (int)(new RDOFUNSequenceByHistHeader( (RDOFUNSequenceHeader *)$1) );
						}
						| fun_seq_header by_hist INT_CONST Body {
							$$ = (int)(new RDOFUNSequenceByHistHeader( (RDOFUNSequenceHeader *)$1, $3) );
						}
						| fun_seq_header by_hist error Body {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "��������� ���� ����������" );
						}
						| fun_seq_header by_hist INT_CONST error {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "��������� �������� ����� $Body" );
						}
						| fun_seq_header by_hist error {
							parser->lexer_loc_set( &(@2), &(@3) );
							parser->error( "��������� �������� ����� $Body" );
						};

fun_seq_by_hist_body_real:	fun_seq_by_hist_header REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, *((double*)$2), *((double*)$3), *((double*)$4)) );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, $2, *((double*)$3), *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, *((double*)$2), $3, *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, *((double*)$2), *((double*)$3), $4) );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, (RDOFUNSequenceByHistHeader *)$1, $2, $3, *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, *((double*)$2), $3, $4) );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name->c_str()) );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, header, $2, *((double*)$3), $4) );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_real && header->header->type->getType() != RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, (RDOFUNSequenceByHistHeader *)$1, $2, $3, $4));
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), *((double*)$3), *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, *((double*)$3), *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST REAL_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), $3, *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( *((double*)$2) > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), *((double*)$3), $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST REAL_CONST {
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( *((double*)$4) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$4) < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, $3, *((double*)$4)); $$ = $1;
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( *((double*)$2) != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( *((double*)$2) > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal(*((double*)$2), $3, $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST INT_CONST {
								RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPResParam::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name->c_str()) );
								}
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( $2 > *((double*)$3) ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, *((double*)$3), $4); $$ = $1;
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST INT_CONST {
								if ( $2 != ((RDOFUNSequenceByHistReal *)$1)->lastTo() ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "������ ��������� ������ ��������� � ���������� �����������" );
								}
								if ( $2 > $3 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������ ��������� ������ ���� ������ ��� �����" );
								}
								if ( $4 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@4) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $4 < 0 ) {
									parser->lexer_loc_set( &(@4) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								((RDOFUNSequenceByHistReal *)$1)->addReal($2, $3, $4); $$ = $1;
							}
							| fun_seq_by_hist_header REAL_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "��������� ����� ���������" );
							}
							| fun_seq_by_hist_header INT_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "��������� ����� ���������" );
							}
							| fun_seq_by_hist_header REAL_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_header INT_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_header INT_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_body_real REAL_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "��������� ����� ���������" );
							}
							| fun_seq_by_hist_body_real INT_CONST error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "��������� ����� ���������" );
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_body_real INT_CONST REAL_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_body_real REAL_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_body_real INT_CONST INT_CONST error {
								parser->lexer_loc_set( &(@3), &(@4) );
								parser->error( "��������� ������������� �����������" );
							}
							| fun_seq_by_hist_body_real IDENTIF error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "�������� �� ������������� ���� ������������������" );
							};

fun_seq_by_hist_body_enum:	fun_seq_by_hist_header IDENTIF REAL_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( *((double*)$3) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$3) < 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								$$ = (int)(new RDOFUNSequenceByHistEnum(parser, header, (std::string*)$2, *((double*)$3)));
								parser->lexer_loc_restore();
							}
							| fun_seq_by_hist_header IDENTIF INT_CONST {
								RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( $3 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $3 < 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								$$ = (int)(new RDOFUNSequenceByHistEnum(parser, header, (std::string*)$2, $3));
								parser->lexer_loc_restore();
							}
							| fun_seq_by_hist_body_enum IDENTIF REAL_CONST {
								if ( *((double*)$3) == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( *((double*)$3) < 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								((RDOFUNSequenceByHistEnum *)$1)->addEnum((std::string*)$2, *((double*)$3));
								parser->lexer_loc_restore();
								$$ = $1;
							}
							| fun_seq_by_hist_body_enum IDENTIF INT_CONST {
								if ( $3 == 0 ) {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@3) );
									parser->warning( "������������� ����������� ������ ���� ������ ����" );
									parser->lexer_loc_restore();
								} else if ( $3 < 0 ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( "������������� ����������� ������ ���� ������ ����" );
								}
								parser->lexer_loc_backup();
								parser->lexer_loc_set( &(@2) );
								((RDOFUNSequenceByHistEnum *)$1)->addEnum((std::string*)$2, $3);
								parser->lexer_loc_restore();
								$$ = $1;
							}
							| fun_seq_by_hist_header IDENTIF error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( rdo::format("��������� ������������� ����������� ��� ��������: %s", ((std::string*)$2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum IDENTIF error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( rdo::format("��������� ������������� ����������� ��� ��������: %s", ((std::string*)$2)->c_str()) );
							}
							| fun_seq_by_hist_body_enum REAL_CONST error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "�������� �� ������������� ���� ������������������" );
							}
							| fun_seq_by_hist_body_enum INT_CONST error {
								parser->lexer_loc_set( &(@2) );
								parser->error( "�������� �� ������������� ���� ������������������" );
							};

fun_seq_by_hist:	fun_seq_by_hist_body_real End {
						((RDOFUNSequenceByHist *)$1)->createCalcs();
					}
					| fun_seq_by_hist_body_enum End {
						((RDOFUNSequenceByHist *)$1)->createCalcs();
					}
					| fun_seq_by_hist_body_real error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "������ � �������� ������������������" );
					}
					| fun_seq_by_hist_body_enum error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "������ � �������� ������������������" );
					}
					| fun_seq_by_hist_body_real {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "��������� �������� ����� $End" );
					}
					| fun_seq_by_hist_body_enum {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "��������� �������� ����� $End" );
					};

fun_seq_enumerative_header:	fun_seq_header enumerative Body {
								$$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1));
							}
							| fun_seq_header enumerative INT_CONST Body {
								$$ = (int)(new RDOFUNSequenceEnumerativeHeader((RDOFUNSequenceHeader *)$1, $3));
							}
							| fun_seq_header enumerative Parameters {
								parser->lexer_loc_set( &(@3) );
								parser->error( "������������������ ���� enumerative �� ����� ����������" );
							}
							| fun_seq_header enumerative INT_CONST Parameters {
								parser->lexer_loc_set( &(@4) );
								parser->error( "������������������ ���� enumerative �� ����� ����������" );
							}
							| fun_seq_header enumerative error {
								parser->lexer_loc_set( &(@2), &(@3) );
								parser->error( "��������� ���� ����������" );
							};


fun_seq_enumerative_body_int:	fun_seq_enumerative_header INT_CONST {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_int ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_real: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� �������������", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_enum: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� ������������� ����", header->header->name->c_str()) );
												break;
											}
										}
									}
									$$ = (int)(new RDOFUNSequenceEnumerativeInt( parser, header, $2 ));
								}
								| fun_seq_enumerative_body_int INT_CONST {
									((RDOFUNSequenceEnumerativeInt *)$1)->addInt($2); $$ = $1;
								}
								| fun_seq_enumerative_body_int error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "��������� ����� ����� ��� �������� ����� $End" );
								};

fun_seq_enumerative_body_real:	fun_seq_enumerative_header REAL_CONST {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_real ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_int: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � �������� ���� ������ ���� ��������������", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_enum: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� ������������� ����", header->header->name->c_str()) );
												break;
											}
										}
									}
									$$ = (int)(new RDOFUNSequenceEnumerativeReal( parser, header, (double*)$2) );
								}
								| fun_seq_enumerative_body_real REAL_CONST {
									((RDOFUNSequenceEnumerativeReal *)$1)->addReal((double*)$2); $$ = $1;
								}
								| fun_seq_enumerative_body_real error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "��������� ������������ ����� ��� �������� ����� $End" );
								};

fun_seq_enumerative_body_enum:	fun_seq_enumerative_header IDENTIF {
									RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPResParam::pt_enum ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPResParam::pt_int: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � �������� ���� ������ ���� ��������������", header->header->name->c_str()) );
												break;
											}
											case RDORTPResParam::pt_real: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� �������������", header->header->name->c_str()) );
												break;
											}
										}
									}
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									$$ = (int)(new RDOFUNSequenceEnumerativeEnum(parser, (RDOFUNSequenceEnumerativeHeader *)$1, (std::string*)$2));
									parser->lexer_loc_restore();
								}
								| fun_seq_enumerative_body_enum IDENTIF {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									((RDOFUNSequenceEnumerativeEnum *)$1)->addEnum((std::string*)$2); $$ = $1;
									parser->lexer_loc_restore();
								}
								| fun_seq_enumerative_body_enum error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "��������� ������� ������������� ���� ��� �������� ����� $End" );
								};

fun_seq_enumerative:	fun_seq_enumerative_body_int End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						}
						| fun_seq_enumerative_body_real End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						}
						| fun_seq_enumerative_body_enum End {
							((RDOFUNSequenceEnumerative *)$1)->createCalcs();
						};

// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
// ���� �� �������������� RDOErrorPos, �� � ����. ���������� ��� �����������
fun_logic: fun_arithm '=' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| '(' fun_logic ')'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| not_keyword fun_logic				{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @2.last_line, @2.last_column );
				$$ = (int)logic->operator_not();
			}
			| fun_group							{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_logic					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
				$$ = $1;
			}
			| '[' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "��������� ������������� ������" );
			}
			| '(' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "��������� ������������� ������" );
			}
			| error								{
				parser->lexer_loc_set( &(@1) );
				parser->error( "������ � ���������� ���������" );
			};

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')'			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| fun_arithm_func_call			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_arithm				{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| IDENTIF '.' IDENTIF			{
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<std::string*>($1), reinterpret_cast<std::string*>($3), @1, @3 );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( parser, (int)$1, @1 );          }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( parser, (double*)$1, @1 );      }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( parser, (std::string*)$1, @1 ); }
			| '-' fun_arithm %prec UMINUS	{
				YYLTYPE error_pos;
				error_pos.first_line   = @1.first_line;
				error_pos.first_column = @1.first_column;
				error_pos.last_line    = @2.last_line;
				error_pos.last_column  = @2.last_column;
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runTime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), error_pos );
			}
			| error							{
				parser->lexer_loc_set( &(@1) );
				if ( @1.first_line = @1.last_line ) {
					parser->error( rdo::format("����������� �������������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				} else {
					parser->error( "������ � �������������� ���������" );
				}
			};

fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun = ((RDOFUNParams*)$3);
							fun->name_error_pos.setErrorPos( @1 );
							fun->setErrorPos( @1.first_line, @1.first_column, @4.last_line, @4.last_column );
							$$ = (int)fun->createCall((std::string *)$1);
						}
						| IDENTIF '(' error {
							parser->lexer_loc_set( &(@3) );
							parser->error( "������ � ���������� �������" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @2 );
								fun = fun->addParameter((RDOFUNArithm *)$2);
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @3 );
								fun = fun->addParameter((RDOFUNArithm *)$3);
								$$ = (int)fun;
							};

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( @3.first_line, @3.first_column + ((std::string*)$3)->length() );
						$$ = (int)(new RDOFUNGroupLogic(parser, $1, (std::string *)$3));
						parser->lexer_loc_restore();
					}
					| fun_group_keyword '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ��� ����" );
					}
					| fun_group_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ������������� ������" );
					};

fun_group:			fun_group_header fun_logic ')' {
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic((RDOFUNLogic *)$2));
					}
					| fun_group_header NoCheck ')' {
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst( parser->runTime, 1 ) );
						trueLogic->setErrorPos( @2 );
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic( trueLogic ));
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

fun_select_header:	Select '(' IDENTIF_COLON {
						$$ = (int)new RDOFUNSelect(parser, (std::string*)$3);
					}
					| Select '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ��� ����" );
					}
					| Select error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ������������� ������" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect((RDOFUNLogic*)$2);
						logic->setErrorPos( @2 );
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect();
						logic->setErrorPos( @2 );
						$$ = $1;
					}
					| fun_select_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, (RDOFUNLogic*)$5 );
						select->setErrorPos( @1.first_line, @1.first_column, @6.last_line, @6.last_column );
						logic->setErrorPos( select->error() );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic* logic = select->createFunSelectEmpty();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						logic->setErrorPos( select->error() );
						$$ = (int)logic;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� '.' (�����) ��� ������ ������ ������ ��������" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� ����� ������ ��������" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "������ � ���������� ���������" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "��������� ������������� ������" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNArithm* arithm = select->createFunSelectSize();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						arithm->setErrorPos( select->error() );
						$$ = (int)arithm;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� '.' (�����) ��� ������ ������ ������ ��������" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� ����� ������ ��������: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "��������� ������������� ������" );
					};

%%

}
