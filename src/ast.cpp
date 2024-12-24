#include <iostream>
#include <cassert>
#include "ast.hpp"

static int koopacnt = 0;

/************************CompUnit*************************/

// CompUnit ::= FuncDef
void CompUnitAST::Dump() const {
  func_def->Dump();
}

int CompUnitAST::Calc() const {
  assert(0);
  return 0;
}

/**************************Decl***************************/

// Decl ::= ConstDecl | VarDecl;
void DeclAST::Dump() const {
  const_decl1_var_decl2->Dump();
}

int DeclAST::Calc() const {
  assert(0);
  return 0;
}

// ConstDecl ::= "const" BType ConstDefList ";";
// ConstDefList ::= ConstDef | ConstDefList "," ConstDef;
void ConstDeclAST::Dump() const {
  for(auto& const_def: *const_def_list)
    const_def->Dump();
}

int ConstDeclAST::Calc() const {
  assert(0);
  return 0;
}

// BType ::= "int";
void BTypeAST::Dump() const {
  assert(0);
  return;
}

int BTypeAST::Calc() const {
  assert(0);
  return 0;
}

// ConstDef ::= IDENT "=" ConstInitVal;
void ConstDefAST::Dump() const {
  insert_symbol(ident, SYM_TYPE_CONST, const_init_val->Calc());
}

int ConstDefAST::Calc() const {
  assert(0);
  return 0;
}

// ConstInitVal ::= ConstExp;
void ConstInitValAST::Dump() const {
  assert(0);
  return;
}

int ConstInitValAST::Calc() const {
  return const_exp->Calc();
}

// VarDecl ::= BType VarDefList ";";
// VarDefList ::= VarDef | VarDefList "," VarDef;
void VarDeclAST::Dump() const {
  for(auto& var_def : *var_def_list)
    var_def->Dump();
}

int VarDeclAST::Calc() const {
  assert(0);
  return 0;
}

// VarDef ::= IDENT | IDENT "=" InitVal;
void VarDefAST::Dump() const {
  // 先 alloc 一段内存
  // @x = alloc i32
  std::cout << "  @" << ident << " = alloc i32" << std::endl;
  if(type==2) {
    init_val->Dump();
    // 存入 InitVal
    // store %1, @x
    std::cout << "  store %" << koopacnt-1 << ", @" << ident << std::endl;
  }
  insert_symbol(ident, SYM_TYPE_VAR, 0);
}

int VarDefAST::Calc() const {
  assert(0);
  return 0;
}

// InitVal ::= Exp;
void InitValAST::Dump() const {
  exp->Dump();
}

int InitValAST::Calc() const {
  assert(0);
  return 0;
}


/**************************Func***************************/

// FuncDef ::= FuncType IDENT "(" ")" Block;
void FuncDefAST::Dump() const {
  // fun @main(): i32 {}
  std::cout << "fun @" << ident << "(): ";
  func_type->Dump();
  std::cout << " {" << std::endl;
  block->Dump();
  std::cout << std::endl << "}" << std::endl;
}

int FuncDefAST::Calc() const {
  assert(0);
  return 0;
}

// FuncType ::= "int";
void FuncTypeAST::Dump() const {
  std::cout << "i32";
}

int FuncTypeAST::Calc() const {
  assert(0);
  return 0;
}

/**************************Block***************************/

// Block ::= "{" BlockItemList "}";
// BlockItemList ::=  | BlockItemList BlockItem;
void BlockAST::Dump() const {
  std::cout << "%entry:" << std::endl;
  for(auto& block_item: *block_item_list)
  {
    block_item->Dump();
  }
}

int BlockAST::Calc() const {
  assert(0);
  return 0;
}

// BlockItem ::= Decl | Stmt;
void BlockItemAST::Dump() const {
  decl1_stmt2->Dump();
}

int BlockItemAST::Calc() const {
  assert(0);
  return 0;
}

// Stmt ::= LVal "=" Exp ";"
//        | "return" Exp ";";
void StmtAST::Dump() const {
  if(type==1) {
    exp->Dump();
    // 存入刚刚计算出的值
    // store %1, @x
    std::cout << "  store %" << koopacnt-1 << ", @";
    std::cout << dynamic_cast<LValAST*>(lval.get())->ident << std::endl;
  }
  else if(type==2) {
    exp->Dump();
    // ret %0
    std::cout << "  ret %" << koopacnt-1;
  }
}

int StmtAST::Calc() const {
  assert(0);
  return 0;
}

/***************************Exp***************************/

// Exp ::= LOrExp;
void ExpAST::Dump() const {
  lorexp->Dump();
}

int ExpAST::Calc() const {
  return lorexp->Calc();
}

// LVal ::= IDENT;
// 只有 LVal 出现在表达式中时会调用该Dump
// 如果 LVal 作为左值出现，则在父节点读取其ident
void LValAST::Dump() const {
  auto val = query_symbol(ident);
  assert(val->type != SYM_TYPE_UND);

  if(val->type == SYM_TYPE_CONST)
  {
    // 此处有优化空间
    // %0 = add 0, 233
    std::cout << "  %" << koopacnt << " = add 0, ";
    std::cout<< val->value << std::endl;
    koopacnt++;
  }
  else if(val->type == SYM_TYPE_VAR)
  {
    // 从内存读取 LVal
    // %0 = load @x
    std::cout << "  %" << koopacnt << " = load @" << ident << std::endl;
    koopacnt++;
  }
  return;
}

int LValAST::Calc() const {
  auto val = query_symbol(ident);
  assert(val->type == SYM_TYPE_CONST);
  return val->value;
}

// PrimaryExp ::= "(" Exp ")" | LVal | Number;
void PrimaryExpAST::Dump() const {
  if(type==1) {
    exp1_lval2->Dump();
  }
  else if(type==2) {
    exp1_lval2->Dump();
  }
  else if(type==3) {
    // %0 = add 0, 233
    std::cout << "  %" << koopacnt << " = add 0, ";
    std::cout<< number << std::endl;
    koopacnt++;
  }
}

int PrimaryExpAST::Calc() const {
  if(type==1) {
    return exp1_lval2->Calc();
  }
  else if(type==2) {
    return exp1_lval2->Calc();
  }
  else if(type==3) {
    return number;
  }
  assert(0);
  return 0;
}

// UnaryExp ::= PrimaryExp | UnaryOp UnaryExp;
// UnaryOp ::= "+" | "-" | "!"
void UnaryExpAST::Dump() const {
  if(type==1) {
    primaryexp1_unaryexp2->Dump();
  }
  else if(type==2) {
    primaryexp1_unaryexp2->Dump();
    if(unaryop=='-') {
      // %1 = sub 0, %0
      std::cout << "  %" << koopacnt << " = sub 0, %";
      std::cout << koopacnt-1 <<std::endl;
      koopacnt++;
    }
    else if(unaryop=='!') {
      // %1 = eq 0, %0
      std::cout << "  %" << koopacnt << " = eq 0, %";
      std::cout << koopacnt-1 <<std::endl;
      koopacnt++;
    }
  }
}

int UnaryExpAST::Calc() const {
  if(type==1) {
    return primaryexp1_unaryexp2->Calc();
  }
  else if(type==2) {
    int tmp = primaryexp1_unaryexp2->Calc();
    if(unaryop=='+') {
      return tmp;
    }
    else if(unaryop=='-') {
      return -tmp;
    }
    else if(unaryop=='!') {
      return !tmp;
    }
  }
  assert(0);
  return 0;
}

// MulExp ::= UnaryExp | MulExp MulOp UnaryExp;
// MulOp ::= "*" | "/" | "%"
void MulExpAST::Dump() const {
  if(type==1) {
    unaryexp->Dump();
  }
  else if(type==2) {
    mulexp->Dump();
    int left = koopacnt-1;
    unaryexp->Dump();
    int right = koopacnt-1;
    if(mulop=='*') {
      // %2 = mul %0, %1
      std::cout << "  %" << koopacnt << " = mul %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(mulop=='/') {
      // %2 = div %0, %1
      std::cout << "  %" << koopacnt << " = div %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(mulop=='%') {
      // %2 = mod %0, %1
      std::cout << "  %" << koopacnt << " = mod %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
  }
}

int MulExpAST::Calc() const {
  if(type==1) {
    return unaryexp->Calc();
  }
  else if(type==2) {
    int left = mulexp->Calc();
    int right = unaryexp->Calc();
    if(mulop=='*') {
      return left * right;
    }
    else if(mulop=='/') {
      return left / right;
    }
    else if(mulop=='%') {
      return left % right;
    }
  }
  assert(0);
  return 0;
}

// AddExp ::= MulExp | AddExp AddOp MulExp;
// AddOp ::= "+" | "-"
void AddExpAST::Dump() const {
  if(type==1) {
    mulexp->Dump();
  }
  else if(type==2) {
    addexp->Dump();
    int left = koopacnt-1;
    mulexp->Dump();
    int right = koopacnt-1;
    if(addop=='+') {
      // %2 = add %0, %1
      std::cout << "  %" << koopacnt << " = add %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(addop=='-') {
      // %2 = sub %0, %1
      std::cout << "  %" << koopacnt << " = sub %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
  }
}

int AddExpAST::Calc() const {
  if(type==1) {
    return mulexp->Calc();
  }
  else if(type==2) {
    int left = addexp->Calc();
    int right = mulexp->Calc();
    if(addop=='+') {
      return left + right;
    }
    else if(addop=='-') {
      return left - right;
    }
  }
  assert(0);
  return 0;
}

// RelExp ::= AddExp | RelExp RelOp AddExp;
// RelOp ::= "<" | ">" | "<=" | ">="
void RelExpAST::Dump() const {
  if(type==1) {
    addexp->Dump();
  }
  else if(type==2) {
    relexp->Dump();
    int left = koopacnt-1;
    addexp->Dump();
    int right = koopacnt-1;
    if(relop=="<") {
      // %2 = lt %0, %1
      std::cout << "  %" << koopacnt << " = lt %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(relop==">") {
      // %2 = gt %0, %1
      std::cout << "  %" << koopacnt << " = gt %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(relop=="<=") {
      // %2 = le %0, %1
      std::cout << "  %" << koopacnt << " = le %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(relop==">=") {
      // %2 = ge %0, %1
      std::cout << "  %" << koopacnt << " = ge %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
  }
}

int RelExpAST::Calc() const {
  if(type==1) {
    return addexp->Calc();
  }
  else if(type==2) {
    int left = relexp->Calc();
    int right = addexp->Calc();
    if(relop=="<") {
      return left < right;
    }
    else if(relop==">") {
      return left > right;
    }
    else if(relop=="<=") {
      return left <= right;
    }
    else if(relop==">=") {
      return left >= right;
    }
  }
  assert(0);
  return 0;
}

// EqExp ::= RelExp | EqExp EqOp RelExp;
// EqOp ::= "==" | "!="
void EqExpAST::Dump() const {
  if(type==1) {
    relexp->Dump();
  }
  else if(type==2) {
    eqexp->Dump();
    int left = koopacnt-1;
    relexp->Dump();
    int right = koopacnt-1;
    if(eqop=="==") {
      // %2 = eq %0, %1
      std::cout << "  %" << koopacnt << " = eq %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
    else if(eqop=="!=") {
      // %2 = ne %0, %1
      std::cout << "  %" << koopacnt << " = ne %";
      std::cout << left << ", %" << right << std::endl;
      koopacnt++;
    }
  }
}

int EqExpAST::Calc() const {
  if(type==1) {
    return relexp->Calc();
  }
  else if(type==2) {
    int left = eqexp->Calc();
    int right = relexp->Calc();
    if(eqop=="==") {
      return left == right;
    }
    else if(eqop=="!=") {
      return left != right;
    }
  }
  assert(0);
  return 0;
}

// LAndExp ::= EqExp | LAndExp "&&" EqExp;
void LAndExpAST::Dump() const {
  if(type==1) {
    eqexp->Dump();
  }
  else if(type==2) {
    landexp->Dump();
    int left = koopacnt-1;
    eqexp->Dump();
    int right = koopacnt-1;
    // A&&B <==> (A!=0)&(B!=0)
    // %2 = ne %0, 0
    std::cout << "  %" << koopacnt << " = ne %";
    std::cout << left << ", 0" << std::endl;
    left = koopacnt;
    koopacnt++;
    // %3 = ne %1, 0
    std::cout << "  %" << koopacnt << " = ne %";
    std::cout << right << ", 0" << std::endl;
    right = koopacnt;
    koopacnt++;
    // %4 = and %2, %3
    std::cout << "  %" << koopacnt << " = and %";
    std::cout << left << ", %" << right << std::endl;
    koopacnt++;
  }
}

int LAndExpAST::Calc() const {
  if(type==1) {
    return eqexp->Calc();
  }
  else if(type==2) {
    int left = landexp->Calc();
    int right = eqexp->Calc();
    return left && right;
  }
  assert(0);
  return 0;
}

// LOrExp  ::= LAndExp | LOrExp "||" LAndExp;
void LOrExpAST::Dump() const {
  if(type==1) {
    landexp->Dump();
  }
  else if(type==2) {
    lorexp->Dump();
    int left = koopacnt-1;
    landexp->Dump();
    int right = koopacnt-1;
    // A||B <==> (A!=0)|(B!=0)
    // %2 = ne %0, 0
    std::cout << "  %" << koopacnt << " = ne %";
    std::cout << left << ", 0" << std::endl;
    left = koopacnt;
    koopacnt++;
    // %3 = ne %1, 0
    std::cout << "  %" << koopacnt << " = ne %";
    std::cout << right << ", 0" << std::endl;
    right = koopacnt;
    koopacnt++;
    // %4 = or %2, %3
    std::cout << "  %" << koopacnt << " = or %";
    std::cout << left << ", %" << right << std::endl;
    koopacnt++;
  }
}

int LOrExpAST::Calc() const {
  if(type==1) {
    return landexp->Calc();
  }
  else if(type==2) {
    int left = lorexp->Calc();
    int right = landexp->Calc();
    return left || right;
  }
  assert(0);
  return 0;
}

// ConstExp ::= Exp;
void ConstExpAST::Dump() const {
  assert(0);
  return;
}

int ConstExpAST::Calc() const {
  return exp->Calc();
}