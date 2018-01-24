
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

void Compile(int position, int& brojac)
{
  wstring nodename,elemname;
  int par,z,q, treci;
  nodename=GetNodeName(position);
par=1;
if (nodename== L"ADD") {
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" ADD EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" ADD EAX,[" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" ADD EAX,EBX"<< endl;
    }
}
else if (nodename== L"ADDMOV") {
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac);
	if (GetNodeName(q)==L"BROJ")
	{
		wcout << L" ADD EAX,"<< GetElemName(q,1)<< endl ;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		wcout << L" ADD EAX,[" << GetElemName(q,1)<<"]"<< endl;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		wcout << L" PUSH EAX"<< endl;
		Compile(q,brojac);
		wcout << L" POP EBX"<< endl;
		wcout << L" ADD EAX,EBX"<< endl;
		wcout << L" MOV [EBX],EAX"<< endl;
	}
	
}
else if (nodename== L"AND") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" AND EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" AND EAX,[" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" AND EAX,EBX"<< endl;
    }
}
else if (nodename== L"BLOCK") {
    while ((z=ElemPos(position,par)) >0)
    {
      Compile(z,brojac);
      par++;
    }
}
else if (nodename== L"NUMLOCK" ) {
    while ((z=ElemPos(position,par)) >0)
    {
      Compile(z,brojac);
      par++;
    }
}
else if (nodename== L"DIV") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
	  if(GetElemName(q,1) == L"0")
		SemErr(L"Dijenjenje s nulom");
	  else
          wcout << L" DIV EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
		//provjeri dijeljenje s nulom	  
          wcout << L" DIV EAX,[" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" DIV EAX,EBX"<< endl;
    }
}
else if (nodename== L"DIVMOV") {
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac);
	if (GetNodeName(q)==L"BROJ")
	{
		if(GetElemName(q,1) == L"0")
			SemErr(L"Dijenjenje s nulom");
		else {
			wcout << L" DIV EAX,"<< GetElemName(q,1)<< endl ;
			wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;	
		}
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		wcout << L" DIV EAX,[" << GetElemName(q,1)<<"]"<< endl;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		wcout << L" PUSH EAX"<< endl;
		Compile(q,brojac);
		wcout << L" POP EBX"<< endl;
		wcout << L" DIV EAX,EBX"<< endl;
		wcout << L" MOV [EBX],EAX"<< endl;
	}
}
else if (nodename== L"EQU") { 
	z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JNE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JNE KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JNE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"GREATEREQUTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JL KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JL KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JL KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"GREATERTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JLE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JLE KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JLE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"IFELSE") { 
	brojac++;
	int novo = brojac;
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	treci=ElemPos(position,3);
	Compile(z,brojac);
	brojac++;
	Compile(q,brojac);
	wcout << L" J KRAJ" << novo + 1 << endl;	
	wcout << L"KRAJ"<< novo <<": " << endl;
	Compile(treci,brojac);
	wcout << L"KRAJ"<< novo + 1 <<": " << endl;
	//brojac++;
}
else if (nodename== L"IF") { 
	brojac++;
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	Compile(z,brojac);
	Compile(q,brojac);
	wcout << L"KRAJ"<< brojac <<": " << endl;
}
else if (nodename== L"LESSEQUTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JG KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JG KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JG KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"LESSTHAN") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JGE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JGE KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JGE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"MOV") {
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    if (GetNodeName(z)==L"VAR")
    {
        if (GetNodeName(q)==L"BROJ")
        {
          wcout << L" MOV DWORD [" << GetElemName(z,1)<<"],"<< GetElemName(q,1)<< endl ;
        }
        else
        if (GetNodeName(q)==L"VAR")
        {
          wcout << L" MOV EAX, [" << GetElemName(q,1)<<"]"<< endl;
          wcout << L" MOV [" << GetElemName(z,1)<<"],EAX"<< endl;
        }
        else
        {
          Compile(q,brojac);
          wcout << L" MOV [" << GetElemName(z,1)<<"],EAX"<< endl;
        }
    }
    else
    {
        if (GetNodeName(q)==L"BROJ")
        {
          Compile(z,brojac);
          wcout << L" MOV DWORD [EBX],"<< GetElemName(q,1)<< endl ;
        }
        else
        if (GetNodeName(q)==L"VAR")
        {
          Compile(z,brojac);
          wcout << L" MOV EAX, [" << GetElemName(q,1)<<"]"<< endl;
          wcout << L" MOV [EBX],EAX"<< endl;
        }
        else
        {
          Compile(q,brojac);
          wcout << L" PUSH EAX"<< endl;
          Compile(z,brojac);
          wcout << L" POP EAX"<< endl;
          wcout << L" MOV [EBX],EAX"<< endl;
        }


    }
}
else if (nodename== L"MUL") { 
	z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" MUL EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" MUL EAX,[" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" MUL EAX,EBX"<< endl;
    }
}
else if (nodename== L"MULTMOV") { 
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac);
	if (GetNodeName(q)==L"BROJ")
	{
		wcout << L" MUL EAX,"<< GetElemName(q,1)<< endl ;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		wcout << L" MUL EAX,[" << GetElemName(q,1)<<"]"<< endl;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		wcout << L" PUSH EAX"<< endl;
		Compile(q,brojac);
		wcout << L" POP EBX"<< endl;
		wcout << L" MUL EAX,EBX"<< endl;
		wcout << L" MOV [EBX],EAX"<< endl;
	}
}
else if (nodename== L"NEQU") { 
    z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" CMP EAX,"<< GetElemName(q,1)<< endl ;
	  wcout << L" JE KRAJ"<< brojac << endl;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" CMP EAX,[" << GetElemName(q,1)<<"]"<< endl;
	  wcout << L" JE KRAJ"<< brojac << endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" CMP EAX,EBX"<< endl;
	  wcout << L" JE KRAJ"<< brojac << endl;
    }
}
else if (nodename== L"SUB") {
z=ElemPos(position,1);
    q=ElemPos(position,2);
    Compile(z,brojac);
    if (GetNodeName(q)==L"BROJ")
    {
          wcout << L" SUB EAX,"<< GetElemName(q,1)<< endl ;
    }
    else
    if (GetNodeName(q)==L"VAR")
    {
          wcout << L" SUB EAX,[" << GetElemName(q,1)<<"]"<< endl;
    }
    else
    {
        wcout << L" PUSH EAX";
        Compile(q,brojac);
        wcout << L" POP EBX";
        wcout << L" SUB EAX,EBX"<< endl;
    }
}
else if (nodename== L"SUBMOV") { 
	z = ElemPos(position, 1);
	q = ElemPos(position, 2);
	Compile(z,brojac);
	if (GetNodeName(q)==L"BROJ")
	{
		wcout << L" SUB EAX,"<< GetElemName(q,1)<< endl ;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;	
	}
	else
	if (GetNodeName(q)==L"VAR")
	{
		wcout << L" SUB EAX,[" << GetElemName(q,1)<<"]"<< endl;
		wcout << L" MOV ["<< GetElemName(z,1) <<"],EAX"<< endl;		
	}
	else
	{
		wcout << L" PUSH EAX"<< endl;
		Compile(q,brojac);
		wcout << L" POP EBX"<< endl;
		wcout << L" SUB EAX,EBX"<< endl;
		wcout << L" MOV [EBX],EAX"<< endl;
	}
}
else if (nodename== L"VAR") {
    wcout << L" MOV EAX, [" << GetElemName(position,1)<<"]"<< endl;
}
else if (nodename== L"BROJ") {
    wcout << L" MOV EAX, " << GetElemName(position,1)<< endl;
}
else if (nodename== L"GVARDEF") { 
	z=ElemPos(position,1);
	wcout << L" MOV ["<< GetElemName(z,1) <<"], 0" << endl ;
}
else if (nodename== L"LOOP") { 
	z=ElemPos(position,1);
	q=ElemPos(position,2);
	brojac++;
	int nova = brojac;
	if (GetNodeName(z)==L"BROJ")
	{
		wcout << L" MOV CX," << GetElemName(z,1) << endl;
		wcout << L"LOOPSTART" << nova << ": " << endl;	
		wcout << L" PUSH CX" << endl;	
		Compile(q,brojac);
		wcout << L" POP CX" << endl;
		wcout << L" DEC CX" << endl;
		wcout << L" JNZ LOOPSTART" << nova << endl;
		
	}
	else
	if (GetNodeName(z)==L"VAR")
	{
		wcout << L" MOV CX,[" << GetElemName(z,1) << "]" << endl;
		wcout << L"LOOPSTART" << nova << ": " << endl;	
		wcout << L" PUSH CX" << endl;		
		Compile(q,brojac);
		wcout << L" POP CX" << endl;
		wcout << L" DEC CX" << endl;
		wcout << L" JNZ LOOPSTART" << nova << endl;		
	}
}






else if (nodename== L"INDEX") { }
else if (nodename== L"ASIZE") { }
else if (nodename== L"GARRDEF") { }
}






