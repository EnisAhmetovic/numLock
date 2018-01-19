

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::numLock() {
		while (la->kind == _ident) {
			Definition();
		}
		Expect(_EOF);
}

void Parser::Definition() {
		while (!(la->kind == _EOF || la->kind == _ident)) {SynErr(28); Get();}
		Ident();
		if (StartOf(1)) {
			ArraySize();
		} else if (StartOf(2)) {
			Statement();
		} else SynErr(29);
}

void Parser::Ident() {
		Expect(_ident);
}

void Parser::ConstVal() {
		Expect(_broj);
}

void Parser::ArraySize() {
		if (la->kind == 3 /* "/," */) {
			Get();
			Expect(_broj);
			Expect(4 /* ",/" */);
		}
		if (la->kind == _ident || la->kind == _broj) {
			Initializator();
			while (la->kind == 5 /* "," */) {
				Get();
				Initializator();
			}
		}
		Expect(6 /* "\n" */);
}

void Parser::Statement() {
		while (!(StartOf(3))) {SynErr(30); Get();}
		if (la->kind == _ident || la->kind == _broj) {
			StatementExpression();
		} else if (la->kind == 8 /* "+" */) {
			IfStatement();
		} else if (la->kind == 10 /* "*" */) {
			LoopStatement();
		} else if (la->kind == 7 /* "/" */) {
			CompoundStatement();
		} else SynErr(31);
}

void Parser::Initializator() {
		if (la->kind == _broj) {
			ConstVal();
		} else if (la->kind == _ident) {
			Ident();
		} else SynErr(32);
}

void Parser::StatementExpression() {
		Expression();
		Expect(6 /* "\n" */);
}

void Parser::IfStatement() {
		Expect(8 /* "+" */);
		Expression();
		Statement();
		if (la->kind == 9 /* "++" */) {
			Get();
			Statement();
		}
}

void Parser::LoopStatement() {
		Expect(10 /* "*" */);
		if (la->kind == _broj) {
			ConstVal();
		} else if (la->kind == _ident) {
			Ident();
		} else SynErr(33);
		Statement();
}

void Parser::CompoundStatement() {
		Expect(7 /* "/" */);
		while (StartOf(2)) {
			Statement();
		}
		Expect(7 /* "/" */);
}

void Parser::Expression() {
		Conditional();
		while (StartOf(4)) {
			AssignmentOperator();
			Expression();
		}
}

void Parser::Conditional() {
		LogANDExp();
}

void Parser::AssignmentOperator() {
		if (la->kind == 22 /* "01" */) {
			Get();
		} else if (la->kind == 23 /* "014" */) {
			Get();
		} else if (la->kind == 24 /* "015" */) {
			Get();
		} else if (la->kind == 25 /* "012" */) {
			Get();
		} else if (la->kind == 26 /* "013" */) {
			Get();
		} else SynErr(34);
}

void Parser::LogANDExp() {
		EqualExp();
		while (la->kind == 11 /* "09" */) {
			Get();
			EqualExp();
		}
}

void Parser::EqualExp() {
		RelationExp();
		while (la->kind == 12 /* "011" */ || la->kind == 13 /* "018" */) {
			if (la->kind == 12 /* "011" */) {
				Get();
			} else {
				Get();
			}
			RelationExp();
		}
}

void Parser::RelationExp() {
		AddExp();
		while (StartOf(5)) {
			if (la->kind == 14 /* "06" */) {
				Get();
			} else if (la->kind == 15 /* "07" */) {
				Get();
			} else if (la->kind == 16 /* "016" */) {
				Get();
			} else {
				Get();
			}
			AddExp();
		}
}

void Parser::AddExp() {
		MultExp();
		while (la->kind == 18 /* "02" */ || la->kind == 19 /* "03" */) {
			if (la->kind == 18 /* "02" */) {
				Get();
			} else {
				Get();
			}
			MultExp();
		}
}

void Parser::MultExp() {
		PostFixExp();
		while (la->kind == 20 /* "04" */ || la->kind == 21 /* "05" */) {
			if (la->kind == 20 /* "04" */) {
				Get();
			} else {
				Get();
			}
			PostFixExp();
		}
}

void Parser::PostFixExp() {
		Primary();
		while (la->kind == 3 /* "/," */) {
			Get();
			Expression();
			Expect(4 /* ",/" */);
		}
}

void Parser::Primary() {
		if (la->kind == _ident) {
			Ident();
		} else if (la->kind == _broj) {
			ConstVal();
		} else SynErr(35);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	numLock();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 27;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[6][29] = {
		{T,T,T,x, x,x,x,T, T,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,T, x,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,x, x,x,x,T, T,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{T,T,T,x, x,x,x,T, T,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,T,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"ident expected"); break;
			case 2: s = coco_string_create(L"broj expected"); break;
			case 3: s = coco_string_create(L"\"/,\" expected"); break;
			case 4: s = coco_string_create(L"\",/\" expected"); break;
			case 5: s = coco_string_create(L"\",\" expected"); break;
			case 6: s = coco_string_create(L"\"\\n\" expected"); break;
			case 7: s = coco_string_create(L"\"/\" expected"); break;
			case 8: s = coco_string_create(L"\"+\" expected"); break;
			case 9: s = coco_string_create(L"\"++\" expected"); break;
			case 10: s = coco_string_create(L"\"*\" expected"); break;
			case 11: s = coco_string_create(L"\"09\" expected"); break;
			case 12: s = coco_string_create(L"\"011\" expected"); break;
			case 13: s = coco_string_create(L"\"018\" expected"); break;
			case 14: s = coco_string_create(L"\"06\" expected"); break;
			case 15: s = coco_string_create(L"\"07\" expected"); break;
			case 16: s = coco_string_create(L"\"016\" expected"); break;
			case 17: s = coco_string_create(L"\"017\" expected"); break;
			case 18: s = coco_string_create(L"\"02\" expected"); break;
			case 19: s = coco_string_create(L"\"03\" expected"); break;
			case 20: s = coco_string_create(L"\"04\" expected"); break;
			case 21: s = coco_string_create(L"\"05\" expected"); break;
			case 22: s = coco_string_create(L"\"01\" expected"); break;
			case 23: s = coco_string_create(L"\"014\" expected"); break;
			case 24: s = coco_string_create(L"\"015\" expected"); break;
			case 25: s = coco_string_create(L"\"012\" expected"); break;
			case 26: s = coco_string_create(L"\"013\" expected"); break;
			case 27: s = coco_string_create(L"??? expected"); break;
			case 28: s = coco_string_create(L"this symbol not expected in Definition"); break;
			case 29: s = coco_string_create(L"invalid Definition"); break;
			case 30: s = coco_string_create(L"this symbol not expected in Statement"); break;
			case 31: s = coco_string_create(L"invalid Statement"); break;
			case 32: s = coco_string_create(L"invalid Initializator"); break;
			case 33: s = coco_string_create(L"invalid LoopStatement"); break;
			case 34: s = coco_string_create(L"invalid AssignmentOperator"); break;
			case 35: s = coco_string_create(L"invalid Primary"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

