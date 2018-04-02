/*************************************************************************/
/*									 */
/*	Source code for use with See5/C5.0 Release 1.11			 */
/*	-----------------------------------------------			 */
/*									 */
/*	This code is provided "as is" without warranty of any kind,	 */
/*	either express or implied.  All use is at your own risk.	 */
/*									 */
/*************************************************************************/


/*************************************************************************/
/*									 */
/*		General data for See5/C5.0				 */
/*									 */
/*************************************************************************/

#include "defns.h"

	Attribute	ClassAtt=0,	/* attribute to use as class */
			LabelAtt=0;	/* attribute used as case label */
	int		MaxAtt=0,	/* max att number */
			MaxClass=0,	/* max class number */
			ErrMsgs=0,      /* errors found */
			LineNo=0,	/* input line number */
			Delimiter;	/* character at end of name */
	DiscrValue	*MaxAttVal;	/* number of values for each att */
	char		*SpecialStatus;	/* special att treatment */
	Definition	*AttDef=0;	/* attribute definitions */
	MyString		*ClassName,	/* class names */
		  	*AttName,	/* att names */
		  	**AttValName;	/* att value names */
	float		Confidence,	/* set by classify() */
			*Vote=Nil,	/* total votes for classes */
			**MCost=Nil;	/* misclass cost [pred][real] */
	Tree		*Pruned=Nil;	/* decision trees */
	CRuleSet	*RuleSet;	/* rulesets */
	FILE		*TRf=0;		/* file pointer for tree and rule i/o */
	char		*FileStem="?",	/* file stem */
			Fn[500];	/* file name with extension */


/*************************************************************************/
/*									 */
/*		Global parameters for See5/C5.0				 */
/*									 */
/*************************************************************************/


	int		TRIALS=1;	/* number of classifiers */

	MyBoolean		RULES=0,	/* true if use rulesets */
			PROBTHRESH=0;	/* true if to use soft thresholds */


/*************************************************************************/
/*									 */
/*	  Global data for See5/C5.0 used for building decision trees	 */
/*									 */
/*************************************************************************/


int
	Trial;		/* trial number for boosting */

float
	*ClassSum;	/* class weights during classification */


/*************************************************************************/
/*									 */
/*	Global data for constructing and applying rules			 */
/*									 */
/*************************************************************************/


CRule	*Rule;		/* current rules */

RuleNo	NRules;		/* number of rules */

int	RuleSpace;	/* space currently allocated for rules */

ClassNo	Default;	/* default class associated with ruleset */
