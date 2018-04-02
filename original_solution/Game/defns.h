/*************************************************************************/
/*									 */
/*	Source code for use with See5/C5.0 Release 1.11			 */
/*	-----------------------------------------------			 */
/*									 */
/*	This code is provided "as is" without warranty of any kind,	 */
/*	either express or implied.  All use is at your own risk.	 */
/*									 */
/*************************************************************************/


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


/*************************************************************************/
/*									 */
/*		Constants, macros etc.					 */
/*									 */
/*************************************************************************/


#define	 Nil	   0		/* null pointer */
#define	 false	   0
#define	 true	   1
#define	 None	   -1 
#define	 Epsilon   1E-4

#define  EXCLUDE   1		/* special attribute status: do not use */
#define  DISCRETE  2		/* ditto: collect values as data read */
#define  ORDERED   3		/* ditto: ordered discrete values */
#define  DATEVAL   4		/* ditto: YYYY/MM/DD */

				/* unknown and N/A values are represented by
				   unlikely floating-point numbers
				   (octal 01600000000 and 01) */
#define	 UNKNOWN   01600000000	/* 1.5777218104420236e-30 */
#define	 NA	   01		/* 1.4012984643248171e-45 */

#define	 BrDiscr   1
#define	 BrThresh  2
#define	 BrSubset  3

#define  Alloc(N,T)		(T *) Pmalloc((N)*sizeof(T))
#define  AllocZero(N,T)		(T *) Pcalloc(N, sizeof(T))
#define  Realloc(V,N,T)		V = (T *) Prealloc(V, (N)*sizeof(T))

#define	 Bit(b)			(1 << (b))
#define	 In(b,s)		((s[(b) >> 3]) & Bit((b) & 07))
#define	 SetBit(b,s)		(s[(b) >> 3] |= Bit((b) & 07))

#define	 ForEach(v,f,l)		for(v=f ; v<=l ; ++v) 

#define  Discrete(a)		(MaxAttVal[a])
#define  Continuous(a)		(! MaxAttVal[a])
#define  Ordered(a)		(SpecialStatus[a] == ORDERED)

#define  Space(s)		(s == ' ' || s == '\n' || s == '\t')
#define  SkipComment		while ( ( c = getc(f) ) != '\n' && c != EOF )

#define	 FreeUnlessNil(p)	if((p)!=Nil) free(p)

#ifdef WIN32
#define  rint(x)		((x)<0 ? (double)((int)((x)-0.5)) :\
					 (double)((int)((x)+0.5)))
#endif

#define	 P1(x)		(rint((x)*10) / 10)
#define	 Of		stdout
#define	 Goodbye(x)	exit(x)


#define	 NOFILE		 0
#define	 BADATTNAME	 1
#define	 EOFINATT	 2
#define	 SINGLEATTVAL	 3
#define	 BADATTVAL	 4
#define	 BADCLASS	 5
#define	 BADCOSTCLASS	 6
#define	 BADCOST	 7
#define	 NOMEM		 8
#define	 TOOMANYVALS	 9
#define	 BADDISCRETE	10
#define	 NOTARGET	11
#define	 BADTARGET	12
#define	 LONGNAME	13
#define	 HITEOF		14
#define	 MISSNAME	15
#define	 BADDATE	16
#define	 BADDEF1	20
#define	 BADDEF2	21
#define	 BADDEF3	22
#define	 SAMEATT	23


/*************************************************************************/
/*									 */
/*		Type definitions					 */
/*									 */
/*************************************************************************/


typedef  unsigned char	MyBoolean, BranchType, *MySet;
typedef	 char		*MyString;

typedef  int	ItemNo;			/* data item number */
typedef  float	ItemCount;		/* count of (partial) items */

typedef  int	ClassNo,		/* class number, 1..MaxClass */
		DiscrValue,		/* discrete attribute value (0 = ?) */
		Attribute;		/* attribute number, 1..MaxAtt */

#ifdef USEDOUBLE
typedef	 double	ContValue;	/* continuous attribute value */
#define	 PREC	14		/* precision */
#else
typedef	 float	ContValue;	/* continuous attribute value */
#define	 PREC	 7		/* precision */
#endif


typedef  union	 _def_val
	 {
	    MyString	_s_val;		/* att val for comparison */
	    ContValue	_n_val;		/* number for arith */
	 }
	 DefVal;

typedef  struct  _def_elt
	 {
	    short	_op_code;	/* type of element */
	    DefVal	_operand;	/* string or numeric value */
	 }
	 DefElt, *Definition;

typedef  struct  _elt_rec
	 {
	    int		Fi,		/* index of first char of element */
			Li;		/* last ditto */
	    char	Type;		/* 'B', 'S', or 'N' */
	 }
	 EltRec;

#define	 DefOp(DE)	DE._op_code
#define	 DefSVal(DE)	DE._operand._s_val
#define	 DefNVal(DE)	DE._operand._n_val

#define	 OP_ATT			 0	/* opcodes */
#define	 OP_NUM			 1
#define	 OP_STR			 2
#define	 OP_MISS		 3
#define	 OP_AND			10
#define	 OP_OR			11
#define	 OP_EQ			20
#define	 OP_NE			21
#define	 OP_GT			22
#define	 OP_GE			23
#define	 OP_LT			24
#define	 OP_LE			25
#define	 OP_SEQ			26
#define	 OP_SNE			27
#define	 OP_PLUS		30
#define	 OP_MINUS		31
#define	 OP_UMINUS		32
#define	 OP_MULT		33
#define	 OP_DIV			34
#define	 OP_MOD			35
#define	 OP_POW			36
#define	 OP_SIN			40
#define	 OP_COS			41
#define	 OP_TAN			42
#define	 OP_LOG			43
#define	 OP_EXP			44
#define	 OP_INT			45
#define	 OP_END			99


typedef  union  _attribute_value
	 {
	    DiscrValue	_discr_val;
	    ContValue	_cont_val;
	    MyString	_string_val;
	 }
	 AttValue, *Description;

#define  CVal(Case,Attribute)   Case[Attribute]._cont_val
#define  DVal(Case,Attribute)   Case[Attribute]._discr_val
#define  XDVal(Case,Att)	(Case[Att]._discr_val & 077777777)
#define  SVal(Case,Attribute)   Case[Attribute]._string_val
#define  Class(Case)		(*Case)._discr_val
#define  Weight(Case)		(*(Case-1))._cont_val

#define	 Unknown(Case,Att)	(DVal(Case,Att)==UNKNOWN)
#define	 UnknownVal(AV)		(AV._discr_val==UNKNOWN)
#define	 NotApplic(Case,Att)	(DVal(Case,Att)==NA)
#define	 NotApplicVal(AV)	(AV._discr_val==NA)


typedef  struct _treerec	*Tree;
typedef  struct _treerec
	 {
	    BranchType	NodeType;
	    ClassNo	Leaf;		/* best class at this node */
	    ItemCount	Items,		/* no of items at this node */
			*ClassDist,	/* class distribution of items */
	    		Errors;		/* no of errors at this node */
	    Attribute	Tested; 	/* attribute referenced in test */
	    int		Forks;		/* number of branches at this node */
	    ContValue	Cut,		/* threshold for continuous attribute */
		  	Lower,		/* lower limit of soft threshold */
		  	Upper,		/* upper limit ditto */
			Mid;		/* 50% point */
	    MySet         *Subset;	/* subsets of discrete values  */
	    Tree	*Branch;	/* Branch[x] = subtree for outcome x */
	 }
	 TreeRec;


typedef  int	RuleNo;			/* rule number */

typedef  struct _condrec
	 {
	    BranchType	NodeType;	/* test type (see tree nodes) */
	    Attribute	Tested;		/* attribute tested */
	    int		Forks;		/* possible branches */
	    ContValue	Cut;		/* threshold (if relevant) */
	    MySet		Subset;		/* subset (if relevant) */
	    int		TestValue;	/* specified outcome of test */
	 }
	 CondRec, *Condition;


typedef  struct _rulerec
	 {
	    RuleNo	RNo;		/* rule number */
	    int		TNo,		/* trial number */
	    		Size;		/* number of conditions */
	    Condition	*Lhs;		/* conditions themselves */
	    ClassNo	Rhs;		/* class given by rule */
	    ItemCount	Cover,		/* number of cases covered by rule */
			Correct;	/* number on which correct */
	    float	Prior;		/* prior probability of RHS */
	    int		Vote;		/* unit = 0.001 */
	 }
	 RuleRec, *CRule;


typedef struct _rulesetrec
	 {
	    RuleNo	SNRules;	/* number of rules */
	    CRule	*SRule;		/* rules */
	    ClassNo	SDefault;	/* default class for this ruleset */
	 }
	 RuleSetRec, *CRuleSet;


/*************************************************************************/
/*									 */
/*		Function prototypes					 */
/*									 */
/*************************************************************************/

MyBoolean	    ReadName(FILE *f, MyString s, int n);
void	    GetNames(FILE *Nf);
void	    DefinedAtt(FILE *Nf);
int	    Which(MyString Val, MyString *List, int First, int Last);
MyString	    CopyString(MyString S);
int	    InChar(FILE *f);

Description GetDescription(FILE *Df, MyBoolean Train);

void	    ImplicitAtt(FILE *Nf);
void	    ReadDefinition(FILE *f);
void	    Append(char c);
MyBoolean	    Expression();
MyBoolean	    Conjunct();
MyBoolean	    SExpression();
MyBoolean	    AExpression();
MyBoolean	    Term();
MyBoolean	    Factor();
MyBoolean	    Primary();
MyBoolean	    Atom();
MyBoolean	    Find(MyString S);
int	    FindOne(MyString *Alt);
Attribute   FindAttName();
void	    DefSyntaxError(MyString Msg);
void	    DefSemanticsError(int Fi, MyString Msg, int OpCode);
void	    Dump(char OpCode, ContValue F, MyString S, int Fi);
void	    DumpOp(char OpCode, int Fi);
MyBoolean	    UpdateTStack(char OpCode, ContValue F, MyString S, int Fi);
AttValue    EvaluateDef(Definition D, Description Case);

void	    ReadFilePrefix(MyString Extension);
void	    ReadHeader();
Tree	    GetTree(MyString Extension);
Tree	    InTree();
CRuleSet    GetRules(MyString Extension);
CRuleSet    InRules();
CRule	    InRule();
Condition   InCondition();
int	    ReadProp(char *Delim);
MyString	    RemoveQuotes(MyString S);
MySet	    MakeSubset(Attribute Att);
void	    BinRecoverDiscreteNames();
Tree	    BinInTree();
CRuleSet    BinInRules();
void	    StreamIn(MyString S, int n);
Tree	    Leaf(ItemCount *Freq, ClassNo NodeClass, ItemCount Items,
		 ItemCount Errors);

void	    GetMCosts(FILE *f);

ClassNo	    TreeClassify(Description CaseDesc, Tree DecisionTree);
void	    FollowAllBranches(Description CaseDesc, Tree T, float Fraction);
void	    FindLeaf(Description CaseDesc, Tree T, Tree PT, float Wt);
ClassNo	    RuleClassify(Description CaseDesc, CRuleSet RS);
MyBoolean	    Satisfies(Description CaseDesc, Condition OneCond);
MyBoolean	    Matches(CRule R, Description Case);
ClassNo	    BoostClassify(Description CaseDesc, int MaxTrial, MyBoolean UseRules);
ClassNo	    SelectClass(ClassNo Default, MyBoolean UseCosts);
ClassNo	    Classify(Description CaseDesc);
float	    Interpolate(Tree T, ContValue Val);

FILE *	    GetFile(MyString Extension, MyString RW);
void	    CheckFile(MyString Extension, MyBoolean Write);

char	    ProcessOption(int Argc, char *Argv[], char *Options);
void	    *Pmalloc(unsigned Bytes);
void	    *Prealloc(void *Present, unsigned Bytes);
void	    *Pcalloc(unsigned Number, unsigned Size);
double	    KRandom();
void	    Error(int ErrNo, MyString S1, MyString S2);
int	    Denominator(ContValue Val);
int	    GetInt(MyString S, int N);
int	    DateToDay(MyString DS);

void	    FreeGlobals();
void	    FreeCosts();
void	    FreeNames();
void	    FreeTree(Tree T);
void	    FreeRules(CRuleSet RS);
void	    FreeCase(Description DVec);
void	    FreeVector(void **V, int First, int Last);
