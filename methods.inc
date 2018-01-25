
bool IsDeclaredGlobal(Name name1)
{
    std::wstring name=std::wstring(name1);
    std::string::size_type found;
    std::wstring decls[2]={L"GARRDEF",L"GVARDEF"};
    for (int i=0;i<2;i++) {
        found = ParseList.find(L"("+decls[i]+L" (NAME "+name+L")",0);
        if (found!=std::string::npos)
            return true;
    }
    
    return false;
}

void UndecErr  (Name name)
{
    Name msg;
    wcscpy(msg,L"Undeclared ");
    wcscat(msg,name);
    SemErr (msg);
}

void AlreadyErr  (Name name)
{
    Name msg;
    wcscpy(msg,L"Already declared ");
    wcscat(msg,name);
    SemErr (msg);
}

std::wstring GetNodeName (int position)
{
    wstring NodeName;
    NodeName=L"";
    if (ParseList[position] != '(')
        return L"";
    position++;
    while (ParseList[position] >='A' && ParseList[position] <='Z' )
    {  NodeName+=ParseList[position];
        position++;
    }
    return NodeName;
}

std::wstring GetElemName (int position, int elemnum)
{
    wstring ElemName;
    position=ElemPos(position,elemnum);
    ElemName=L"";
    if (position>0)
      while (ParseList[position] !=' ' && ParseList[position] !=')' )
      {  
          ElemName+=ParseList[position];
          position++;
      }
    return ElemName;
}

int ElemPos(int position, int elemnum)
{
    int currel,brlevel;
    currel=0;
    position++;
    while (ParseList[position] >='A' && ParseList[position] <='Z' )
        position++;
    for (;;)
    {
        while (ParseList[position] ==' ' )
            position++;
        currel++;
        if (currel==elemnum && ParseList[position] !=')')
            return position;
        switch (ParseList[position])
        {
            case '(':
                brlevel=1;
                position++;
                while (brlevel !=0)
                {
                    while (ParseList[position] !='(' && ParseList[position] !='`'  && ParseList[position] !=')')
                        position++;
                    switch(ParseList[position])
                    {
                        case '(':
                            brlevel++;position++;
                            break;
                        case ')':
                            brlevel--;position++;
                            break;
                        case '`':
                            do {position++;} while (ParseList[position]!='`');position++;
                            break;
                    }
                }
                break;
            case ')':
                return -1;
                break;
            case '`':
                do
                {
                    position++;
                    
                } while (ParseList[position]!='`');
                position++;
                break;
            default:
                while (ParseList[position] !='(' && ParseList[position] !=' '  && ParseList[position] !=')')
                  position++;
        }
    }
    
}

bool Assignable(int position) {
	wstring nodeName = GetNodeName(position);
	if(nodeName == L"VAR" || nodeName == L"INDEX")
		return true;
	return false;	
}

void Compile(int position, int& brojac, wstringstream& ispis, wstringstream& data)
{
  wstring nodename,elemname;
  int par,z,q, treci;
  nodename=GetNodeName(position);
par=1;
if (nodename== L"ADD") {
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" ADD EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" ADD EAX,[v" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" ADD EAX,EBX"<< endl;
    }
}
else if (nodename== L"ADDMOV") {
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac,ispis,data);
	if (GetNodeName(q)==L"BROJ")
	{
		ispis << L" ADD EAX,"<< GetElemName(q,1)<< endl ;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		ispis << L" ADD EAX,[v" << GetElemName(q,1)<<"]"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		ispis << L" PUSH EAX"<< endl;
		Compile(q,brojac,ispis,data);
		ispis << L" POP EBX"<< endl;
		ispis << L" ADD EAX,EBX"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;
	}
	
}
else if (nodename== L"AND") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" AND EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" AND EAX,[v" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" AND EAX,EBX"<< endl;
    }
}
else if (nodename== L"BLOCK") {
    while ((z=ElemPos(position,par)) >0)
    {
      Compile(z,brojac,ispis,data);
      par++;
    }
}
else if (nodename== L"NUMLOCK" ) {
    while ((z=ElemPos(position,par)) >0)
    {
      Compile(z,brojac,ispis,data);
      par++;
    }
}
else if (nodename== L"DIV") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
	  if(GetElemName(q,1) == L"0")
		SemErr(L"Dijenjenje s nulom");
	  else
	{
	  ispis << L" MOV EDX, 0" << endl;
          ispis << L" MOV ECX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" DIV ECX" << endl;
	}
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
	  ispis << L" MOV EDX, 0" << endl;
          ispis << L" DIV ECX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" DIV ECX" << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP ECX";
        ispis << L" DIV ECX"<< endl;
    }
}
else if (nodename== L"DIVMOV") {
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac,ispis,data);
	if (GetNodeName(q)==L"BROJ")
	{
		if(GetElemName(q,1) == L"0")
			SemErr(L"Dijenjenje s nulom");
		else {

			  ispis << L" MOV EDX, 0" << endl;
			  ispis << L" MOV ECX,"<< GetElemName(q,1)<< endl ;
			  ispis << L" DIV ECX" << endl;
			  ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;	
		}
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		ispis << L" MOV EDX, 0" << endl;
		ispis << L" DIV ECX,[v" << GetElemName(q,1)<<"]"<< endl;
		ispis << L" DIV ECX" << endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		ispis << L" PUSH EAX"<< endl;
		Compile(q,brojac,ispis,data);
		ispis << L" POP ECX"<< endl;
		ispis << L" DIV ECX"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;
	}
}
else if (nodename== L"EQU") { 
	z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JNE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JNE KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JNE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"GREATEREQUTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JL KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JL KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JL KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"GREATERTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JLE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JLE KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JLE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"IFELSE") { 
	brojac++;
	int novo = brojac;
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	treci=ElemPos(position,3);
	Compile(z,brojac,ispis,data);
	brojac++;
	Compile(q,brojac,ispis,data);
	ispis << L" J KRAJ" << novo + 1 << endl;	
	ispis << L"KRAJ"<< novo <<": " << endl;
	Compile(treci,brojac,ispis,data);
	ispis << L"KRAJ"<< novo + 1 <<": " << endl;
	//brojac++;
}
else if (nodename== L"IF") { 
	brojac++;
	int nova = brojac;
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	Compile(z,brojac,ispis,data);
	Compile(q,brojac,ispis,data);
	ispis << L"KRAJ"<< nova <<": " << endl;
}
else if (nodename== L"LESSEQUTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JG KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JG KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JG KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"LESSTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JGE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JGE KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JGE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"MOV") {
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    if (GetNodeName(z)==L"VAR")
    {
        if (GetNodeName(q)==L"BROJ")
        {
          ispis << L" MOV DWORD [v" << GetElemName(z,1)<<"],"<< GetElemName(q,1)<< endl ;
        }
        else
        if (GetNodeName(q)==L"VAR")
        {
          ispis << L" MOV EAX, [v" << GetElemName(q,1)<<"]"<< endl;
          ispis << L" MOV [v" << GetElemName(z,1)<<"],EAX"<< endl;
        }
        else
        {
          Compile(q,brojac,ispis,data);
          ispis << L" MOV [v" << GetElemName(z,1)<<"],EAX"<< endl;
        }
    }
    else
    {
        if (GetNodeName(q)==L"BROJ")
        {
          Compile(z,brojac,ispis,data);
          ispis << L" MOV DWORD [EBX],"<< GetElemName(q,1)<< endl ;
        }
        else
        if (GetNodeName(q)==L"VAR")
        {
          Compile(z,brojac,ispis,data);
          ispis << L" MOV EAX, [v" << GetElemName(q,1)<<"]"<< endl;
          ispis << L" MOV [EBX],EAX"<< endl;
        }
        else
        {
          Compile(q,brojac,ispis,data);
          ispis << L" PUSH EAX"<< endl;
          Compile(z,brojac,ispis,data);
          ispis << L" POP EAX"<< endl;
          ispis << L" MOV [EBX],EAX"<< endl;
        }


    }
}
else if (nodename== L"MUL") { 
	z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" MOV ECX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" MUL ECX" << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" MOV ECX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" MUL ECX" << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP ECX";
        ispis << L" MUL ECX"<< endl;
    }
}
else if (nodename== L"MULTMOV") { 
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac,ispis,data);
	if (GetNodeName(q)==L"BROJ")
	{
		ispis << L" MOV ECX,"<< GetElemName(q,1)<< endl ;
	  	ispis << L" MUL ECX" << endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		ispis << L" MOV ECX,"<< GetElemName(q,1)<< endl ;
	  	ispis << L" MUL ECX" << endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		ispis << L" PUSH EAX"<< endl;
		Compile(q,brojac,ispis,data);
		ispis << L" POP ECX"<< endl;
		ispis << L" MUL ECX"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;
	}
}
else if (nodename== L"NEQU") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  ispis << L" JE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" CMP EAX,[v" << GetElemName(q,1)<<"]"<< endl;
	  ispis << L" JE KRAJ"<< brojac << endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" CMP EAX,EBX"<< endl;
	  ispis << L" JE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"SUB") {
z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac,ispis,data);
    if (GetNodeName(q)==L"BROJ")
    {
          ispis << L" SUB EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          ispis << L" SUB EAX,[v" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        ispis << L" PUSH EAX";
        Compile(q,brojac,ispis,data);
        ispis << L" POP EBX";
        ispis << L" SUB EAX,EBX"<< endl;
    }
}
else if (nodename== L"SUBMOV") { 
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac,ispis,data);
	if (GetNodeName(q)==L"BROJ")
	{
		ispis << L" SUB EAX,"<< GetElemName(q,1)<< endl ;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		ispis << L" SUB EAX,[v" << GetElemName(q,1)<<"]"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		ispis << L" PUSH EAX"<< endl;
		Compile(q,brojac,ispis,data);
		ispis << L" POP EBX"<< endl;
		ispis << L" SUB EAX,EBX"<< endl;
		ispis << L" MOV [v" << GetElemName(z,1) <<"],EAX"<< endl;
	}
}
else if (nodename== L"VAR") {
    ispis << L" MOV EAX, [v" << GetElemName(position,1)<<"]"<< endl;
}
else if (nodename== L"BROJ") {
    ispis << L" MOV EAX, " << GetElemName(position,1)<< endl;
}
else if (nodename== L"GVARDEF") { 
	z=ElemPos(position,1);
	data << L" v" << GetElemName(z,1) << " DD 0" << endl;
}
else if (nodename== L"LOOP") { 
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	brojac++;
	int nova = brojac;
	if (GetNodeName(z)==L"BROJ")
	{
		ispis << L" MOV CX," << GetElemName(z,1) << endl;
		ispis << L"LOOPSTART" << nova << ": " << endl;	
		ispis << L" PUSH CX" << endl;	
		Compile(q,brojac,ispis,data);
		ispis << L" POP CX" << endl;
		ispis << L" DEC CX" << endl;
		ispis << L" JNZ LOOPSTART" << nova << endl;
		
	}
	else
	if (GetNodeName(z)==L"VAR")
	{
		ispis << L" MOV CX,[v" << GetElemName(z,1) << "]" << endl;
		ispis << L"LOOPSTART" << nova << ": " << endl;	
		ispis << L" PUSH CX" << endl;		
		Compile(q,brojac,ispis,data);
		ispis << L" POP CX" << endl;
		ispis << L" DEC CX" << endl;
		ispis << L" JNZ LOOPSTART" << nova << endl;		
	}
}
else if (nodename== L"PRINT") { 
	z=ElemPos(position,1);
	if (GetNodeName(z)==L"VAR")
	{
		ispis << L" MOV CX,0" << endl;
		ispis << L" MOV EAX,[v" << GetElemName(z,1) << "]" << endl;
		ispis << L" MOV EDX,-900" << endl;
		ispis << L" PUSH EDX" << endl;
		ispis << L"LOOP"<< brojac <<":" << endl;
		ispis << L" MOV EDX,0" << endl;
		ispis << L" MOV ECX,10" << endl;
		ispis << L" DIV ECX" << endl;
		ispis << L" ADD CX, 1" << endl;
		ispis << L" PUSH EDX" << endl;
		ispis << L" CMP EAX,0" << endl;
		ispis << L" JNE LOOP" << brojac << endl;
		brojac++;
		ispis << L"LOOP"<< brojac <<": " << endl;	
		ispis << L" POP DWORD [v" << GetElemName(z,1) << "]" << endl;	
		ispis << L" CMP DWORD [v" << GetElemName(z,1) << "], -900" << endl;
		ispis << L" JE KRAJISPISA"<< brojac << endl;
		ispis << L" ADD DWORD [v" << GetElemName(z,1) << "], 48" << endl;	
		ispis << L" MOV EAX,4" << endl;	
		ispis << L" MOV EBX,1" << endl;	
		ispis << L" MOV ECX,v" << GetElemName(z,1) << endl;	
		ispis << L" MOV EDX,1" << endl;	
		ispis << L" INT 0x80" << endl;	
		ispis << L" SUB CX,1" << endl;	
		ispis << L" SUB DWORD [v" << GetElemName(z,1) << "], 48" << endl;	
		ispis << L" CMP DWORD [v" << GetElemName(z,1) << "], -900" << endl;
		ispis << L" JNE LOOP"<< brojac << endl;
		ispis << L"KRAJISPISA"<< brojac << ":" << endl;	
		ispis << L" MOV DWORD [v" << GetElemName(z,1) << "], 32" << endl;
		ispis << L" MOV EAX,4" << endl;	
		ispis << L" MOV EBX,1" << endl;	
		ispis << L" MOV ECX,v" << GetElemName(z,1) << endl;	
		ispis << L" MOV EDX,1" << endl;	
		ispis << L" INT 0x80" << endl;	
		brojac++;
	}
}
}






