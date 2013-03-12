// Scintilla source code edit control
/** @file LexRdo.cxx
 ** Lexer for RDO.
 ** Written by Andrey Urusov.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "LexRdo.h"

#include "Platform.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "CharacterSet.h"

#include "utils/rdolocale.h"

namespace rdo { namespace gui { namespace lexer {

void SyntaxColor(unsigned int startPos, int length, int initStyle, WordList* keywordlists[], Accessor& styler)
{
	WordList& keywords  = *keywordlists[0];
	WordList& functions = *keywordlists[1];
	WordList& traces    = *keywordlists[2];
	WordList& colors    = *keywordlists[3];

	styler.StartAt(startPos);

	StyleContext sc(startPos, length, initStyle, styler);

	std::locale locale = rdo::locale::get().model();

	bool flag = sc.More();
	for (; flag; sc.Forward())
	{

		if (sc.state == SCE_RDO_OPERATOR)
		{
			sc.SetState(SCE_RDO_DEFAULT);
		}
		else if (sc.state == SCE_RDO_IDENTIFIER)
		{
			if (!isIdentifier(sc.ch, locale))
			{
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords.InList(s))
				{
					sc.ChangeState(SCE_RDO_KEYWORD);
				}
				else if (functions.InList(s))
				{
					sc.ChangeState(SCE_RDO_FUNCTION);
				}
				else if (traces.InList(s))
				{
					sc.ChangeState(SCE_RDO_TRACE);
				}
				else if (colors.InList(s))
				{
					sc.ChangeState(SCE_RDO_FRAME_COLOR);
				}
				sc.SetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_NUMBER)
		{
			if (!std::isdigit((rbyte)sc.ch, locale) && sc.ch != '.' && !(sc.ch == 'e' || sc.ch == 'E') && !((sc.ch == '+' || sc.ch == '-') && (sc.chPrev == 'e' || sc.chPrev == 'E')))
			{
				sc.SetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_STRING)
		{
			if (sc.ch == '\'')
			{
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_COMMENT_CPP)
		{
			if (sc.Match('*', '/'))
			{
				sc.Forward();
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_COMMENT_LINE)
		{
			if (sc.ch == '\r' || sc.ch == '\n')
			{
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}

		if (sc.state == SCE_RDO_DEFAULT)
		{
			if (sc.ch == '\'')
			{
				sc.SetState(SCE_RDO_STRING);
			}
			else if (sc.Match('/', '*'))
			{
				sc.SetState(SCE_RDO_COMMENT_CPP);
				sc.Forward();	// Eat the * so it isn't used for the end of the comment
			}
			else if (sc.Match('/', '/'))
			{
				sc.SetState(SCE_RDO_COMMENT_LINE);
			}
			else if (std::isdigit((rbyte)sc.ch, locale) || ((sc.ch == '-' || sc.ch == '+') && std::isdigit((rbyte)sc.chNext, locale)))
			{
				sc.SetState(SCE_RDO_NUMBER);
			}
			else if (isOperator(sc.ch))
			{
				sc.SetState(SCE_RDO_OPERATOR);
			}
			else if (isIdentifier(sc.ch, locale))
				sc.SetState(SCE_RDO_IDENTIFIER);
		}

		flag = sc.More();
	}
	sc.Complete();
}

void SyntaxFold(unsigned int startPos, int length, int initStyle, WordList *[], Accessor &styler)
{
	unsigned int endPos  = startPos + length;
	int     visibleChars = 0;
	int     lineCurrent  = styler.GetLine(startPos);
	int     levelPrev    = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int     levelCurrent = levelPrev;
	char    chNext       = styler[startPos];
	int     styleNext    = styler.StyleAt(startPos);
	int     style        = initStyle;
	tstring str;
	for (unsigned int i = startPos; i < endPos; i++)
	{
		char ch       = chNext;
		chNext        = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style         = styleNext;
		styleNext     = styler.StyleAt(i + 1);
		bool atEOL    = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == SCE_RDO_COMMENT_CPP && styler.GetPropertyInt("CommentFold") != 0)
		{
			if (style != stylePrev)
			{
				levelCurrent++;
			}
			else if ((style != styleNext) && !atEOL)
			{
				levelCurrent--;
			}
		}
		if (style == SCE_RDO_KEYWORD)
		{
			str += ch;
		}
		else
		{
			if (str.length())
			{
				if (str == "$Pattern"  || str == "$Resource_type"  || str == "$Resources" || str == "$Operations" || str == "$Frame" || str == "$Constant" || str == "$Sequence" || str == "$Function" || str == "$Decision_point" || str == "$Process" || str == "$Results"
				 || str == "$pattern"  || str == "$resource_type"  || str == "$resources" || str == "$operations" || str == "$frame" || str == "$constant" || str == "$sequence" || str == "$function" || str == "$decision_point" || str == "$Process" || str == "$results")
				{
					levelCurrent++;
				}
				else if (str == "$End")
				{
					levelCurrent--;
				}
				str = "";
			}
		}
		if (atEOL)
		{
			int lev = levelPrev;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
			{
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent))
			{
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
		{
			visibleChars++;
		}
	}
	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);
}

bool isOperator(char ch)
{
	return
		ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
		ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ',' ||
		ch == '<' || ch == '>' || ch == '=' || ch == ';' || ch == '!' ||
		ch == '{' || ch == '}' || ch == '.';
}

bool isIdentifier(char ch, const std::locale& locale)
{
	return 
		std::isalpha((rbyte)ch, locale) ||
		std::isdigit((rbyte)ch, locale) ||
		ch == '_' ||
		ch == '$' ||
		ch == '%';
}

}}} // namespace rdo::gui::lexer

LexerModule lexerRDOSyntax(SCLEX_RDO, rdo::gui::lexer::SyntaxColor, "rdo", rdo::gui::lexer::SyntaxFold);
