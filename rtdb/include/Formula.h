/////////////////////////////////////////////////////////////////////////////
// Formula.h: �������ڹ�ʽ(���ʽ+IF_THEN_ELSE���)�������CFormula
/////////////////////////////////////////////////////////////////////////////
#ifndef	__AFX_RTDB_FORMULA_H_2004_10_07__
#define	__AFX_RTDB_FORMULA_H_2004_10_07__
/////////////////////////////////////////////////////////////////////////////
#define MAX_PAIR_TYPE_SUM	5

#define MAX_EXPRESSION_LEN	240// ���ʽ��󳤶�
#define MAX_STACK_NUMBER	100// ��ջ���������
#define MAX_TOKEN_LEN		10 // һ���������ŵ���󳤶�

#define MAX_VARIABLE_SUM	30 // ����������
#define MAX_CONSTANT_SUM	30 // ���������

#include "SYSTEM.h"
/////////////////////////////////////////////////////////////////////////////
typedef struct FORMULA_VARIABLE
{
	UINT	m_uTableID;	  //���ID
	UINT	m_uRecordID;  //��¼ID
	INT		m_iFieldIndex;//�ֶ�ID
	int		m_iDataType;  //ֵ����
	char	m_cValue[4];  //����ֵ
} CVariable;
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CFormula
{
public:
	CFormula();
	~CFormula();

public:
	//��ʽ���ʽ
	char		m_strExpression[MAX_EXPRESSION_LEN];
	//��������λʹ�ñ�־
	bool		m_VariFlag[MAX_VARIABLE_SUM];
	//��������
	CVariable	m_Variable[MAX_VARIABLE_SUM];

public:
	//��ϵͳ��ʽ���ĳ����¼��ȡ���㹫ʽ�����ȫ��������Ϣ
	void	ReadFormula(const SYSTEM_Formula & Formula);
	//�����㹫ʽ��ȫ��������Ϣд��ϵͳ��ʽ���ĳ����¼��
	void	WriteFormula(SYSTEM_Formula & Formula);
	//�ж�ָ���Ĺ�ʽ�Ƿ���(����)��ֵ���
	static int IsAssignSentence(const char * pSentence);
	//�ж�ָ���ı����Ƿ��Ѿ�����
	bool	IsVariableExist(CVariable * pVar);
	//���빫ʽ
	bool	Translate();
	//ִ�й�ʽ����(���ȵ���Translate���빫ʽ)
	bool	RunFormula(CDataBase * pDataBase);
	//ȡ�ù�ʽ������(����)
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
