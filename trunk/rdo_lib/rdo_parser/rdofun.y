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
%token if_keyword			369
%token result_keyword		370

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

// ----------------------------------------------------------------------------
// ---------- ���������
// ----------------------------------------------------------------------------
fun_consts:	/* empty */
			| Constant fun_const_body End
			| Constant fun_const_body {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "����� �������� �������� ��������� �������� ����� $End" );
			};

fun_const_body:	/* empty */
				| fun_const_body fun_const_param_desc {
					RDORTPParam* cons = reinterpret_cast<RDORTPParam*>($2);
					parser->addConstant( cons );
				}
				| fun_const_body error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "��������� �������� ���������" );
				};

fun_const_param_desc:	IDENTIF_COLON param_type {
							std::string name = *reinterpret_cast<std::string*>($1);
							if ( parser->findFUNConst( name ) ) {
								parser->lexer_loc_set( &(@1) );
								parser->error( rdo::format("��������� � ����� ������ ��� ����������: %s", name.c_str()) );
//								parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
							}
							RDORTPParamType* parType = reinterpret_cast<RDORTPParamType*>($2);
							if ( !parType->dv->isExist() ) {
								parser->lexer_loc_set( &(@2) );
								parser->error( "��������� ������ ����� ��������" );
//								parser->error( "Constant must have value" );
							}
							RDORTPParam* param = new RDORTPParam( parser, name, parType, RDOParserSrcInfo( @1, @2 ) );
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
// ---------- �������� ���� ���������
// ----------------------------------------------------------------------------
param_type:		integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum* enu      = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPEnumDefVal* dv = reinterpret_cast<RDORTPEnumDefVal*>($2);
					if ( dv->isExist() ) {
						enu->findEnumValueWithThrow( dv->src_pos(), dv->getEnumValue() ); // ���� �� �������, �� ����� ��������� �� ������, �.�. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( parser->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( src_info );
				}
				| param_such_as '=' INT_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( (int)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' REAL_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(double*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' IDENTIF {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(std::string*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' error {
					parser->error( "��������� ������� ��-���������" );
				}
				| param_such_as error {
					parser->error( "��������� ��������� �������� ���������-������, ��������, ������� ��-���������" );
				};
/*
				| integer error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� integer. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� real. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ������������� ����. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( pos );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
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
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( pos );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
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
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_int_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPIntDefVal(pos));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal($2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' REAL_CONST {
						// ����� ����� ���������������� ������������: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "�� ������� �������� ��-��������� ��� ������ ����" );
					}
					| '=' error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "�������� �������� ��-��������� ��� ������ ����" );
					};

param_real_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPRealDefVal(pos));
					}
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->src_text() + ")" );
				$$ = $2;
			}
			| '(' param_enum_list {
				parser->lexer_loc_set( &(@2) );
				parser->error( "������������ ������ ������������� �������" );
			};

param_enum_list: IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( parser->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						std::string next = *reinterpret_cast<std::string*>($3);
						enu->add( next, @3 );
						enu->setSrcText( enu->src_text() + ", " + next );
						$$ = (int)enu;
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("��������� ������� �����: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "������ � �������� �������� ������������� ����" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "�������� �������� ������������� ����: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| param_enum_list ',' REAL_CONST {
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

param_enum_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPEnumDefVal(pos));
					}
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal( *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string type  = *reinterpret_cast<std::string*>($2);
					std::string param = *reinterpret_cast<std::string*>($4);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = parser->findFUNConst( constName );
					if ( !cons ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("������ �� �������������� ���������: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| such_as IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "�� ������ ���������" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "������ ��� �������� ���������" );
					}
				}
				| such_as IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "����� ����� ���� ������ ���� ������ ��������� ����� �����" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "����� ��������� ����� such_as ���������� ������� ��� � ��������� ������� ��� ������" );
				};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- ������������������ � �������
// ----------------------------------------------------------------------------
fun_func_seq:	/* empty */
			| fun_func_seq fun_func_descr
			| fun_func_seq fun_seq_descr
			| fun_func_seq Constant {
				parser->lexer_loc_set( &(@2) );
				parser->error( "��������� ����� ���� ������� �������, ����� ��������� � ��������������������" );
			};

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
fun_func_descr:	fun_func_header fun_func_footer
				| fun_func_header error {
					RDOFUNFunction* fun = reinterpret_cast<RDOFUNFunction*>($1);
					parser->error( @2, rdo::format("��������� �������� ����� $Type � ��������� ���� ������� '%s'", fun->getName().c_str()) );
				};

fun_func_header:	Function_keyword IDENTIF_COLON param_type {
						std::string name = *reinterpret_cast<std::string*>($2);
						if ( parser->findFUNConst( name ) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "���������� ��������� � ����� �� ������: %s", name.c_str() ) );
						}
						if ( parser->findSequence( name ) ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( rdo::format( "���������� ������������������ � ����� �� ������: %s", name.c_str() ) );
						}
						if ( parser->findFunction( name ) ) {
							parser->error( @2, rdo::format( "������� ��� ����������: %s", name.c_str() ) );
//							parser->error( ("Second appearance of the same function: " + name).c_str() );
						}
						RDORTPParamType* retType = reinterpret_cast<RDORTPParamType*>($3);
						RDOParserSrcInfo src_info;
						src_info.setSrcPosAndTextByLength( @2, name );
						RDOFUNFunction* fun = new RDOFUNFunction( parser, src_info, retType );
						if ( retType->getType() == RDORTPParamType::pt_enum && static_cast<RDORTPEnumParamType*>(retType)->enum_name.empty() ) {
							static_cast<RDORTPEnumParamType*>(retType)->enum_name = name;
							static_cast<RDORTPEnumParamType*>(retType)->enum_fun  = true;
						}
						$$ = (int)fun;
					}
					| Function_keyword IDENTIF_COLON error {
						std::string name = *reinterpret_cast<std::string*>($2);
						parser->error( @3, rdo::format("��������� ��� ������������� �������� ������� '%s'", name.c_str()) );
					}
					| Function_keyword error {
						parser->error( @2, "����� ��������� ����� $Function ��������� ��� ������" );
					};

fun_func_parameters:	/* empty */
					| Parameters fun_func_params {
					};

fun_func_params:	/* empty */
				| fun_func_params IDENTIF_COLON param_type {
					std::string      name = *reinterpret_cast<std::string*>($2);
					RDORTPParamType* type = reinterpret_cast<RDORTPParamType*>($3);
					RDOParserSrcInfo src_info;
					src_info.setSrcPosAndTextByLength( @2, name );
					RDOFUNFunctionParam* param = new RDOFUNFunctionParam( parser->getLastFUNFunction(), src_info, type );
					parser->getLastFUNFunction()->add( param );
				}
				| fun_func_params IDENTIF_COLON error {
					parser->error( @3, "��������� ��� ��������� �������" );
				}
				| fun_func_params error {
					parser->error( @2, "��������� �������� ��������� ������� � ������� <���>: <���>" );
				};

fun_func_footer:	Type_keyword '=' algorithmic fun_func_parameters Body fun_func_algorithmic_body End {
						RDOFUNFunction* currFunc = parser->getLastFUNFunction();
						currFunc->createAlgorithmicCalc( @5 );
					}
					| Type_keyword '=' list_keyword fun_func_parameters Body fun_func_list_body End {
						RDOFUNFunction* currFunc = parser->getLastFUNFunction();
						currFunc->createListCalc();
					}
					| Type_keyword '=' table_keyword fun_func_parameters Body fun_func_list_body End {
						RDOFUNFunction *currFunc = parser->getLastFUNFunction();
						currFunc->createTableCalc();
					}
					| Type_keyword '=' algorithmic fun_func_parameters Body fun_func_algorithmic_body error {
						parser->error( @7, "��������� �������� ����� $End" );
					}
					| Type_keyword '=' list_keyword fun_func_parameters Body fun_func_list_body error {
						parser->error( @7, "��������� �������� ����� $End" );
					}
					| Type_keyword '=' table_keyword fun_func_parameters Body fun_func_list_body error {
						parser->error( @7, "��������� �������� ����� $End" );
					}
					| Type_keyword '=' algorithmic error {
						parser->error( @4, "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' list_keyword error {
						parser->error( @4, "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' table_keyword error {
						parser->error( @4, "��������� �������� ����� $Parameters" );
					}
					| Type_keyword '=' error {
						parser->error( @3, "����������� ��� �������" );
					}
					| Type_keyword error {
						parser->error( @2, "����� ��������� ����� $Type ��������� ��� �������" );
					};

fun_func_algorithmic_body:	/* empty */
							| fun_func_algorithmic_body fun_func_algorithmic_calc_if {
								RDOFUNCalculateIf* calculateIf = reinterpret_cast<RDOFUNCalculateIf*>($2);
								parser->getLastFUNFunction()->add( calculateIf );
							};

fun_func_calc_if:	Calculate_if {
					}
					| if_keyword {
					};

fun_func_calc_name:	result_keyword {
					}
					| IDENTIF {
						std::string name = *reinterpret_cast<std::string*>($1);
						if ( name != parser->getLastFUNFunction()->getName() ) {
							parser->error( @1, rdo::format("��������� ��� ������� '%s'", parser->getLastFUNFunction()->getName().c_str()) );
						}
					};

fun_func_algorithmic_calc_if:	fun_func_calc_if fun_logic fun_func_calc_name '=' fun_arithm {
									RDOFUNLogic*  logic  = reinterpret_cast<RDOFUNLogic*>($2);
									logic->setSrcText( "Calculate_if " + logic->src_text() );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($5);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( parser->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_name '=' fun_arithm {
									rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
									RDOParserSrcInfo logic_src_info( "Calculate_if 1 = 1" );
									logic_src_info.setSrcPos( @1.first_line, @1.first_column, @1.first_line, @1.first_column );
									calc_cond->setSrcInfo( logic_src_info );
									RDOFUNLogic*  logic  = new RDOFUNLogic( calc_cond, true );
									logic->setSrcInfo( logic_src_info );
									RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
									RDOFUNCalculateIf* calculateIf = new RDOFUNCalculateIf( parser->getLastFUNFunction(), logic, arithm );
									$$ = (int)calculateIf;
								}
								| fun_func_calc_if fun_logic fun_func_calc_name error {
									parser->error( @4, "��������� '='" );
								}
								| fun_func_calc_name error {
									parser->error( @2, "��������� '='" );
								}
								| fun_func_calc_if fun_logic error {
									parser->error( @3, "��������� <���_�������> = <���������_�������>" );
								};
								| error {
									parser->error( @1, "��������� �������� ����� Calculate_if" );
								};

fun_func_list_body:	/* empty */
			| fun_func_list_body fun_func_list_value;

fun_func_list_value: IDENTIF {
						std::string str = *reinterpret_cast<std::string*>($1);
						RDOFUNFunctionListElementIdentif* value = new RDOFUNFunctionListElementIdentif( parser->getLastFUNFunction(), RDOParserSrcInfo( @1, str ) );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| REAL_CONST {
						RDOFUNFunctionListElementReal* value = new RDOFUNFunctionListElementReal( parser->getLastFUNFunction(), @1, *(double *)$1 );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| INT_CONST {
						RDOFUNFunctionListElementInt* value = new RDOFUNFunctionListElementInt( parser->getLastFUNFunction(), @1, (int)$1 );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					}
					| '=' {
						RDOFUNFunctionListElementEq* value = new RDOFUNFunctionListElementEq( parser->getLastFUNFunction(), @1 );
						parser->getLastFUNFunction()->add( value );
						$$ = (int)value;
					};

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
fun_seq_descr:	fun_seq_uniform
				| fun_seq_exponential
				| fun_seq_normal
				| fun_seq_by_hist
				| fun_seq_enumerative;

fun_seq_header:	Sequence IDENTIF_COLON param_type Type_keyword '=' {
					std::string name = *reinterpret_cast<std::string*>($2);
					if ( parser->findFUNConst( name ) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "���������� ��������� � ����� �� ������: %s", name.c_str() ) );
					}
					if ( parser->findSequence( name ) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "������������������ ��� ����������: %s", name.c_str() ) );
					}
					if ( parser->findFunction( name ) ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "���������� ������� � ����� �� ������: %s", name.c_str() ) );
					}
					$$ = (int)(new RDOFUNSequence::RDOFUNSequenceHeader( name, (RDORTPParamType *)$3, RDOParserSrcInfo( @2, name ) ));
				}
				| Sequence IDENTIF_COLON param_type Type_keyword '=' error {
					parser->lexer_loc_set( &(@5), &(@6) );
					parser->error( "��������� ��� ������������������" );
				}
				| Sequence IDENTIF_COLON param_type Type_keyword error {
					parser->lexer_loc_set( &(@4), &(@5) );
					parser->error( "��������� ��� ������������������ � ������� '=' <���>" );
				}
				| Sequence IDENTIF_COLON param_type error {
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
						$$ = (int)(new RDOFUNSequenceUniform( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1) );
					}
					| fun_seq_header uniform INT_CONST End {
						$$ = (int)(new RDOFUNSequenceUniform( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1, $3) );
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
							$$ = (int)(new RDOFUNSequenceExponential( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1) );
						}
						| fun_seq_header exponential INT_CONST End {
							$$ = (int)(new RDOFUNSequenceExponential( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1, $3) );
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
					$$ = (int)(new RDOFUNSequenceNormal( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1) );
				}
				| fun_seq_header normal_keyword INT_CONST End {
					$$ = (int)(new RDOFUNSequenceNormal( parser, (RDOFUNSequence::RDOFUNSequenceHeader*)$1, $3) );
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
							$$ = (int)(new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( (RDOFUNSequence::RDOFUNSequenceHeader*)$1) );
						}
						| fun_seq_header by_hist INT_CONST Body {
							$$ = (int)(new RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader( (RDOFUNSequence::RDOFUNSequenceHeader*)$1, $3) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
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
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, (RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*)$1, $2, $3, *((double*)$4)) );
							}
							| fun_seq_by_hist_header REAL_CONST INT_CONST INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( "�������� �� ������������� ���� ������������������" );
								}
								if ( header->header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_real && header->header->type->getType() != RDORTPParamType::pt_int ) {
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
								$$ = (int)(new RDOFUNSequenceByHistReal( parser, (RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*)$1, $2, $3, $4));
							}
							| fun_seq_by_hist_body_real REAL_CONST REAL_CONST REAL_CONST {
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@2) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequence::RDOFUNSequenceHeader* header = reinterpret_cast<RDOFUNSequenceByHistReal*>($1)->header;
								if ( header->type->getType() == RDORTPParamType::pt_int ) {
									parser->lexer_loc_set( &(@3) );
									parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������", header->name.c_str()) );
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
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_enum ) {
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
								$$ = (int)(new RDOFUNSequenceByHistEnum(parser, header, *(std::string*)$2, *((double*)$3)));
								parser->lexer_loc_restore();
							}
							| fun_seq_by_hist_header IDENTIF INT_CONST {
								RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader* header = reinterpret_cast<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader*>($1);
								if ( header->header->type->getType() != RDORTPParamType::pt_enum ) {
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
								$$ = (int)(new RDOFUNSequenceByHistEnum(parser, header, *(std::string*)$2, $3));
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
								((RDOFUNSequenceByHistEnum *)$1)->addEnum(*(std::string*)$2, *((double*)$3));
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
								((RDOFUNSequenceByHistEnum *)$1)->addEnum(*(std::string*)$2, $3);
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
								$$ = (int)(new RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader((RDOFUNSequence::RDOFUNSequenceHeader*)$1));
							}
							| fun_seq_header enumerative INT_CONST Body {
								$$ = (int)(new RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader((RDOFUNSequence::RDOFUNSequenceHeader*)$1, $3));
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
									RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPParamType::pt_int ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPParamType::pt_real: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� �������������", header->header->name.c_str()) );
												break;
											}
											case RDORTPParamType::pt_enum: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� ������������� ����", header->header->name.c_str()) );
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
									RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPParamType::pt_real ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPParamType::pt_int: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � �������� ���� ������ ���� ��������������", header->header->name.c_str()) );
												break;
											}
											case RDORTPParamType::pt_enum: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� ������������� ����", header->header->name.c_str()) );
												break;
											}
										}
									}
									$$ = (int)(new RDOFUNSequenceEnumerativeReal( parser, header, *(double*)$2) );
								}
								| fun_seq_enumerative_body_real REAL_CONST {
									((RDOFUNSequenceEnumerativeReal *)$1)->addReal(*(double*)$2); $$ = $1;
								}
								| fun_seq_enumerative_body_real error {
									parser->lexer_loc_set( &(@1), &(@2) );
									parser->error( "��������� ������������ ����� ��� �������� ����� $End" );
								};

fun_seq_enumerative_body_enum:	fun_seq_enumerative_header IDENTIF {
									RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader* header = reinterpret_cast<RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader*>($1);
									if ( header->header->type->getType() != RDORTPParamType::pt_enum ) {
										parser->lexer_loc_set( &(@2) );
										switch ( header->header->type->getType() ) {
											case RDORTPParamType::pt_int: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� �������������, � �������� ���� ������ ���� ��������������", header->header->name.c_str()) );
												break;
											}
											case RDORTPParamType::pt_real: {
												parser->error( rdo::format("������������������ '%s' ���������� ��� ������������, � �������� ���� ������ ���� �������������", header->header->name.c_str()) );
												break;
											}
										}
									}
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									$$ = (int)(new RDOFUNSequenceEnumerativeEnum(parser, (RDOFUNSequenceEnumerative::RDOFUNSequenceEnumerativeHeader*)$1, *(std::string*)$2));
									parser->lexer_loc_restore();
								}
								| fun_seq_enumerative_body_enum IDENTIF {
									parser->lexer_loc_backup();
									parser->lexer_loc_set( &(@2) );
									((RDOFUNSequenceEnumerativeEnum *)$1)->addEnum(*(std::string*)$2); $$ = $1;
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
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| not_keyword fun_logic				{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group							{
				$$ = $1;
			}
			| fun_select_logic					{
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
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call			{
				$$ = $1;
			}
			| fun_select_arithm				{
				$$ = $1;
			}
			| IDENTIF '.' IDENTIF			{
				$$ = (int)new RDOFUNArithm( parser, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( parser, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( parser, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( parser, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec UMINUS	{
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runtime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			}
			| error							{
				parser->lexer_loc_set( &(@1) );
				if ( @1.first_line = @1.last_line ) {
					parser->error( rdo::format("����������� �������������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				} else {
					parser->error( "������ � �������������� ���������" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($3);
							fun->name_error_pos.setSrcPos( @1 );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( *(std::string*)$1 + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( *(std::string*)$1 );
							$$ = (int)arithm;
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
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
								fun->setSrcText( arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
								fun->setSrcText( fun->src_text() + ", " + arithm->src_text() );
								fun->addParameter( arithm );
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
						$$ = (int)(new RDOFUNGroupLogic( parser, $1, *(std::string *)$3) );
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
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic((RDOFUNLogic *)$2);
					}
					| fun_group_header NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst( parser->runtime, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	Select '(' IDENTIF_COLON {
						RDOFUNSelect* select = new RDOFUNSelect(parser, *(std::string*)$3);
						select->setSrcText( "Select(" + *(std::string*)$3 + ": " );
						$$ = (int)select;
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
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						RDOFUNLogic* logic = select->createFunSelect( flogic );
						logic->setSrcPos( @2 );
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcText( select->src_text() + "NoCheck)" );
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect();
						logic->setSrcPos( @2 );
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
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, (RDOFUNLogic*)$5 );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOFUNLogic* logic = select->createFunSelectEmpty();
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
						select->setSrcPos( @1, @5 );
						RDOFUNArithm* arithm = select->createFunSelectSize();
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
