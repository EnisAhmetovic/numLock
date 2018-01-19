

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__



#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_ident=1,
		_broj=2
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void numLock();
	void Definition();
	void Ident();
	void ConstVal();
	void ArraySize();
	void Statement();
	void Initializator();
	void StatementExpression();
	void IfStatement();
	void LoopStatement();
	void CompoundStatement();
	void Expression();
	void Conditional();
	void AssignmentOperator();
	void LogANDExp();
	void EqualExp();
	void RelationExp();
	void AddExp();
	void MultExp();
	void PostFixExp();
	void Primary();

	void Parse();

}; // end Parser



#endif

