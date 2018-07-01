/////////////////////////////////////////////////////////////////////////////
// Formula.h: 定义用于公式(表达式+IF_THEN_ELSE语句)计算的类CFormula
/////////////////////////////////////////////////////////////////////////////
#ifndef	__AFX_RTDB_FORMULA_H_2004_10_07__
#define	__AFX_RTDB_FORMULA_H_2004_10_07__
/////////////////////////////////////////////////////////////////////////////
#define MAX_PAIR_TYPE_SUM	5

#define MAX_EXPRESSION_LEN	240// 表达式最大长度
#define MAX_STACK_NUMBER	100// 堆栈的最大容量
#define MAX_TOKEN_LEN		10 // 一个完整符号的最大长度

#define MAX_VARIABLE_SUM	30 // 最大变量个数
#define MAX_CONSTANT_SUM	30 // 最大常量个数

#include "SYSTEM.h"
/////////////////////////////////////////////////////////////////////////////
typedef struct FORMULA_VARIABLE
{
	UINT	m_uTableID;	  //表的ID
	UINT	m_uRecordID;  //记录ID
	INT		m_iFieldIndex;//字段ID
	int		m_iDataType;  //值类型
	char	m_cValue[4];  //变量值
} CVariable;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CFormula
{
public:
	CFormula();
	~CFormula();

public:
	//公式表达式
	char		m_strExpression[MAX_EXPRESSION_LEN];
	//变量数组位使用标志
	bool		m_VariFlag[MAX_VARIABLE_SUM];
	//变量数组
	CVariable	m_Variable[MAX_VARIABLE_SUM];

public:
	//从系统公式表的某条记录读取计算公式所需的全部定义信息
	void	ReadFormula(const SYSTEM_Formula & Formula);
	//将计算公式的全部定义信息写到系统公式表的某条记录中
	void	WriteFormula(SYSTEM_Formula & Formula);
	//判断指定的公式是否是(包含)赋值语句
	static int IsAssignSentence(const char * pSentence);
	//判断指定的变量是否已经存在
	bool	IsVariableExist(CVariable * pVar);
	//编译公式
	bool	Translate();
	//执行公式计算(需先调用Translate编译公式)
	bool	RunFormula(CDataBase * pDataBase);
	//取得公式计算结果(变量)
	CVariable * GetResult();

	bool	WriteResultToRtdb(CDataBase * pDataBase,const CVariable & Result);

private:
	struct CConstVar
	{
		int		m_iDataType;
		char	m_cValue[4];
	};

private:
	char	m_pszResultTokens[MAX_EXPRESSION_LEN];

	int		m_nTokenID;
	bool	m_bTokenReaded;

	char	m_pszCurToken[MAX_TOKEN_LEN];
	int		m_nTokenEndPos;

	char	m_Char;
	int 	m_nCurCharPos;
	bool	m_bCharReaded;

	int		m_TokenPair[MAX_PAIR_TYPE_SUM][MAX_PAIR_TYPE_SUM];

	char	m_SymbolStack[MAX_STACK_NUMBER];
	int		m_nSymbolStackTop;

	CVariable 	m_StackData[MAX_STACK_NUMBER];
	int			m_nVariableStackTop;

	int			m_iConstantNumber;
	CConstVar  	m_Constant[MAX_CONSTANT_SUM];
	CVariable	m_Result;

private:
	char	GetChar();
	int		GetToken();

	bool	SkipDescribleChars();
	int		GetConstantToken(int& nTokenID);
	int		GetKeywordToken(int & nTokenID);
	int		GetVariableToken(int & nTokenID);

	int		GetTokenRunType(int nTokenID);
	int		GetTokenPairType(int nTokenID);
	bool	IsPairToken(int nFirstTokenID, int nSecondTokenID);

	bool	SymbolStackPush(int nTokenID);
	int		SymbolStackPop();
	int		GetStackToken();
	int		GetStackTopTokenPriority();
	bool	IsSymbolStackEmpty();
	void	ClearTokenChar();
	void	AddCharToToken(char Char);

	bool	DataStackPop(CVariable & vari);
	bool	DataStackPush(const CVariable & vari);
	bool	DataCompute(CVariable & Var1, CVariable & Var2, int iComputeType);
	bool	PrepareVariables(CDataBase * pDataBase);

	bool	TranslateConditionSentence();
	int		TranslateSentences(char & cLength, char * pSentence);
	int		TranslateExpression(char & cLength, char * pExpression, int nEndToken);
	int		TranslateParam(int nFuncToken, char & cLength, char * pExpression);

	int		GetTokenPriority(int nTokenID,bool bInStack);

	bool	RunFormulaExpression(char * pTokenID,int nTokenNum,CDataBase * pDataBase);
	void	GetKeyConstant(int nTokenID, CVariable & Result);
};
/////////////////////////////////////////////////////////////////////////////
#endif // __AFX_RTDB_FORMULA_H_2004_10_07__
